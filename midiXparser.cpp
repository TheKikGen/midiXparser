/*
  midXparser
  A small footprint midi parser.
  Copyright (C) 2017/2018 by The KikGen labs.

  HEADER CLASS FILE - METHODS

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaim all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.

   Licence : MIT.
*/

#include "midiXparser.h"

const  uint8_t midiXparser::m_systemCommonMsglen[]={
        // SYSTEM COMMON
        0, // soxStatus             = 0XF0,
        2, // midiTimeCodeStatus    = 0XF1,
        3, // songPosPointerStatus  = 0XF2,
        2, // songSelectStatus      = 0XF3,
        0, // reserved1Status       = 0XF4,
        0, // reserved2Status       = 0XF5,
        1, // tuneRequestStatus     = 0XF6,
        0  // eoxStatus             = 0XF7,
};

const  uint8_t midiXparser::m_channelVoiceMsgMsglen[]={
        3, // noteOffStatus         = 0X80,
        3, // noteOnStatus          = 0X90,
        3, // polyKeyPressureStatus = 0XA0,
        3, // controlChangeStatus   = 0XB0,
        2, // programChangeStatus   = 0XC0,
        2, // channelPressureStatus = 0XD0,
        3, // pitchBendStatus       = 0XE0,
};

// Constructors
midiXparser::midiXparser() {

};

// Constructor with passing channel voice filter mask
midiXparser::midiXparser(uint8_t  channelVoiceMsgFilterMask) {
     setChannelVoiceMsgFilter(channelVoiceMsgFilterMask);
};

// Contructor with passing channel voice filter mask and midi channel
midiXparser::midiXparser(uint8_t  channelVoiceMsgFilterMask,uint8_t  midiChannelFilter) {
     setChannelVoiceMsgFilter(channelVoiceMsgFilterMask);
     setMidiChannelFilter(midiChannelFilter);
};

// DESTRUCTOR IS MANDATORY HERE TO FREE SYSEX BUFFER
midiXparser::~midiXparser() {
  setSysExFilter(false);
}

// Give the current sysex state,
bool midiXparser::isSysExMode() { return m_sysExMode ;}

bool midiXparser::wasSysExMode() {
  return  ( m_readByte == eoxStatus || m_sysExError );
}

// Give the status of the last SYSEX transmission.
bool midiXparser::isSysExError() { return m_sysExError ;}

// Return on the fly mode, sysex unbuffered
bool midiXparser::isSysExOnTheFly() { return (m_sysExBufferSize == 0) ;}

// Used to check if the last byte parsed was captured
bool midiXparser::isByteCaptured() { return m_isByteCaptured; }

// Return the type of the last parsed midi message
uint8_t  midiXparser::getMidiMsgType() { return m_midiParsedMsgType; }

// Return the type of a midi status (cf enum)
uint8_t  midiXparser::getMidiStatusMsgTypeMsk(uint8_t midiStatus) {

  if (midiStatus >= 0xF8 ) return realTimeMsgTypeMsk;
  if (midiStatus == 0XF7 || midiStatus == 0xF0 ) return sysExMsgTypeMsk;
  if (midiStatus >= 0xF0 ) return systemCommonMsgTypeMsk;
  if (midiStatus >= 0x80 ) return channelVoiceMsgTypeMsk;

  return noneMsgTypeMsk ;
}

// Return the len of the last parsed midi message, including sysex
uint8_t  midiXparser::getMidiMsgLen() {
  if (m_midiParsedMsgType == sysExMsgTypeMsk )         return getSysExMsgLen() ;
  if (m_midiParsedMsgType == realTimeMsgTypeMsk )      return 1 ;
  if (m_midiParsedMsgType == channelVoiceMsgTypeMsk )  return m_channelVoiceMsgMsglen[ (getMidiMsg()[0] >> 4) - 8 ] ;
  if (m_midiParsedMsgType == systemCommonMsgTypeMsk )  return m_systemCommonMsglen[getMidiMsg()[0] & 0x0F] ;

  return 0;
}

// Return the len of a midistatus message (cf enum)
// Nb: SOX or EOX return always 0.

uint8_t midiXparser::getMidiStatusMsgLen(uint8_t midiStatus) {
  if (midiStatus >= 0xF8 ) return 1;
  if (midiStatus >= 0xF0 ) return m_systemCommonMsglen[midiStatus & 0x0F];
  if (midiStatus >= 0x80 ) return m_channelVoiceMsgMsglen[ (midiStatus >> 4) - 8 ];
  return 0 ;
}

// Return the len of the last Sysex msg.
// This persists until the next sysex.
unsigned midiXparser::getSysExMsgLen() { return m_sysExMsgLen ;}

// Return the parsed message buffered
uint8_t * midiXparser::getMidiMsg() {

    switch (m_midiParsedMsgType) {
      case realTimeMsgTypeMsk:
        return m_midiMsgRealTime;
        break;
      case sysExMsgTypeMsk:
        return m_sysExBuffer;
        break;
    }

   return m_midiMsg ;
}

// Get the sysex internal buffer address. NULL if not allocated.
uint8_t * midiXparser::getSysExMsg() { return m_sysExBuffer; };

// Get the last byte parsed
byte midiXparser::getByte() { return m_readByte ;}

// Get the previous byte parsed.
// 2 parse method calls at a minimum
byte midiXparser::getPreviousByte() { return m_previousReadByte ;}

// Set filter for Midi Channel 0 to F
void midiXparser::setMidiChannelFilter(uint8_t midiChannelFilter) {
      m_midiChannelFilter = midiChannelFilter;
}

// Set filter mask for channel voice Msg
void midiXparser::setChannelVoiceMsgFilter(uint8_t channelVoiceMsgFilterMask) {
      m_channelVoiceMsgFilterMask = channelVoiceMsgFilterMask;
      if ( channelVoiceMsgFilterMask != noChannelVoiceMsgMsk)
          m_midiMsgTypeFilterMsk |= channelVoiceMsgTypeMsk;
      else
          m_midiMsgTypeFilterMsk &= ~channelVoiceMsgTypeMsk;
}

// Set filter mask for system Common Msg but Sysex.
// As sysex has a start/end behaviour, it is not possible to filter here.
// Use the setSysExFilter.
void midiXparser::setSystemCommonMsgFilter(uint8_t systemCommonMsgFilterMask) {
      m_systemCommonMsgFilterMask = systemCommonMsgFilterMask;
      if ( systemCommonMsgFilterMask != noSystemCommonMsgMsk)
          m_midiMsgTypeFilterMsk |= systemCommonMsgTypeMsk;
      else
          m_midiMsgTypeFilterMsk &= ~systemCommonMsgTypeMsk;
}

// Set filter mask for realtime Msg
void midiXparser::setRealTimeMsgFilter(uint8_t realTimeMsgFilterMask) {
      m_realTimeMsgFilterMask = realTimeMsgFilterMask;
      if ( realTimeMsgFilterMask != noRealTimeMsgMsk)
          m_midiMsgTypeFilterMsk |= realTimeMsgTypeMsk;
      else
          m_midiMsgTypeFilterMsk &= ~realTimeMsgTypeMsk;
}

// Set filter mask all/none for all midi Msg including Sysex
// For Sysex, the "on the fly" mode is activated by default
// To change that, you must call explicitly setSysExFilter again.
void midiXparser::setMidiMsgFilter(midiMsgTypeMaskValue midiMsgTypeFilterMsk) {

  // setMidiMsgFilter resets all sub-filters
  setChannelVoiceMsgFilter(noChannelVoiceMsgMsk);
  setSystemCommonMsgFilter(noSystemCommonMsgMsk) ;
  setRealTimeMsgFilter(noRealTimeMsgMsk);
  setSysExFilter(false);

  // Channel voice msg
  if (midiMsgTypeFilterMsk & channelVoiceMsgTypeMsk )
     setChannelVoiceMsgFilter(allChannelVoiceMsgMsk);

  // System common msg
  if (midiMsgTypeFilterMsk & systemCommonMsgTypeMsk )
     setSystemCommonMsgFilter(allSystemCommonMsgMsk) ;

  // real time msg
  if (midiMsgTypeFilterMsk & realTimeMsgTypeMsk )
     setRealTimeMsgFilter(allRealTimeMsgMsk);

  // Sysex msg
  if ( midiMsgTypeFilterMsk & sysExMsgTypeMsk )
     setSysExFilter(true,0);

}

//////////////////////////////////////////////////////////////////////////////
// Allocate a sysex buffer and start listening msg
//----------------------------------------------------------------------------
// If sysExFilterToggle is false, the memory is released.
// This function uses dynamic memory allocation, so the DESTRUCTOR
// must clean everything.
// If the size is 0 (default),then the sysex parsing will not store Bytes.
// That allows parsing on the fly without the memory limitation constraint,
// but will not return true on parsing, so you must use isByteCaptured().
//////////////////////////////////////////////////////////////////////////////
bool midiXparser::setSysExFilter(bool sysExFilter,unsigned sysExBufferSize) {

    // Already allocated. Free buffer.
    if ( m_sysExBuffer!= NULL) free(m_sysExBuffer);

    m_sysExBufferSize  = sysExBufferSize ;
    m_sysExMsgLen = 0 ;
    m_sysExBufferIndex = 0;
    m_sysExBuffer = NULL ;

    if ( sysExFilter ) {

      m_midiMsgTypeFilterMsk |= sysExMsgTypeMsk;

      // On the fly mode
      if (sysExBufferSize == 0 ) return true ;

      // (re)Allocate the buffer
      m_sysExBuffer = (uint8_t *)calloc ( sysExBufferSize , sizeof(uint8_t) );
      if ( m_sysExBuffer != NULL ) return true;
      else {
           // Allocation error. Disable SYSEX filter.
           m_sysExBufferSize  = 0 ;
           m_midiMsgTypeFilterMsk &= ~sysExMsgTypeMsk;
           return false;
       }
    }
    else
    {
      m_sysExBufferSize = 0;
      m_midiMsgTypeFilterMsk &= ~sysExMsgTypeMsk;
    }

    return true;

}
//////////////////////////////////////////////////////////////////////////////
// midiXParser MIDI PARSER
//----------------------------------------------------------------------------
// The main method.
// It parses midi byte per byte and return true if a message is matching filters.
// Set also the byte capture flag if a byte belong to a filtered message.
//////////////////////////////////////////////////////////////////////////////
bool midiXparser::parse(byte readByte) {

    // Store the passed byte so it can be sent back to serial
    // if not captured
    m_previousReadByte = m_readByte ;
    m_readByte = readByte;
    m_isByteCaptured=false;
    m_isMsgReady = false;
    // Clear any previous sysex error if not in sysex mode anymore
    m_sysExError = false;

    // MIDI Message status are starting at 0x80
    if ( readByte >= 0x80 )
    {
       // Real time messages must be processed as transparent for all other status
       if  ( readByte >= 0xF8 ) {
            // Apply midi channel filter mask
            m_isByteCaptured = ( m_realTimeMsgFilterMask & ( 1 << ( 15-(readByte & 0x0F) )  )  ) ? true:false;
            if (m_isByteCaptured) {
              m_midiParsedMsgType = realTimeMsgTypeMsk;
              m_midiMsgRealTime[0] = readByte;
            }
            m_isMsgReady = m_isByteCaptured;
            return m_isMsgReady;
       }

       // Running status not possible at this point
       m_runningStatusPossible=false;

       // Reset current msg type and msg len
       m_midiCurrentMsgType = noneMsgTypeMsk;
       m_nextMidiMsglen = 0;
       m_expectedMsgLen = 0;

       // Start SYSEX ---------------------------------------------------------

       // Clean end of Sysex.
       if ( readByte == eoxStatus ) {
            if (m_sysExMode  ) {
               m_sysExMode = false;
               // Apply filter
               if ( m_midiMsgTypeFilterMsk & sysExMsgTypeMsk ) {
                  m_midiParsedMsgType = sysExMsgTypeMsk;
                  m_isByteCaptured = true ;
                  m_sysExMsgLen = m_sysExBufferIndex;
                  m_isMsgReady = true;
               }
               return m_isMsgReady; // Even if no data !
            } // Isolated EOX without SOX.
            else {
              m_sysExError = true;
              m_sysExMsgLen = m_sysExBufferIndex = 0;
            }
            return false;
       }

       // SysEx can be terminated abnormally with a midi status.
       // In that case, the SYSEX msg will be dropped to proceed the midi command.
       // The message is still buffered. m_sysExError is set to true for this round.

       if (m_sysExMode ) {
              m_sysExMode = false;
              m_sysExError = true;
              m_sysExMsgLen = m_sysExBufferIndex;
       }

       // Start SYSEX
       if ( readByte == soxStatus ) {
              m_sysExMode = true;
              m_sysExBufferIndex = 0;
              m_midiCurrentMsgType = sysExMsgTypeMsk;
              m_isByteCaptured = ( m_midiMsgTypeFilterMsk & sysExMsgTypeMsk );

              return false;
       }

      // Channel messages between 0x80 and 0xEF ------------------------------------
      if ( readByte <= 0xEF ) {

          m_midiCurrentMsgType = channelVoiceMsgTypeMsk;

          // Midi channel filter
          if ( m_midiChannelFilter != allChannel &&
                   (readByte & 0x0F) != m_midiChannelFilter ) {
                   return false;
          }

          // Apply message channel filter mask
          m_isByteCaptured = ( m_channelVoiceMsgFilterMask & ( 1 << ( 15-(readByte >> 4) )  )  ) ? true:false;
      }

      // System common messages between 0xF0 and 0xF7 -----------------------------
      // but SOX / EOX
      else {
          m_midiCurrentMsgType = systemCommonMsgTypeMsk;
          m_isByteCaptured = ( m_systemCommonMsgFilterMask & ( 1 << (7 - (readByte & 0x0F) )  )  ) ? true:false;
      }

      if (m_isByteCaptured) {
        // get the len of the midi msg to parse
        m_expectedMsgLen = getMidiStatusMsgLen(readByte);
        m_nextMidiMsglen = m_expectedMsgLen ;
        m_midiMsg[0] = readByte;
        m_nextMidiMsglen--;

        // Case of 1 byte len midi msg (Tune request)
        if ( m_expectedMsgLen == 1 ) {
          m_midiParsedMsgType = systemCommonMsgTypeMsk;//m_midiCurrentMsgType;
          m_isMsgReady = true;
        }
      }

    }

    // Midi Data from 00 to 0X7F --------------------------------------------------
    else {

          // Capture the SYSEX message if filter is set
          // If m_sysExBufferSize is 0, do not store
          if (m_sysExMode ) {

              // Apply filter
              if (m_midiMsgTypeFilterMsk & sysExMsgTypeMsk) {
                  if (m_sysExBufferSize == 0 ) {
                      m_isByteCaptured = true; // On the fly
                      m_sysExBufferIndex++;
                  }
                  else
                  // Check overflow of the sysex buffer
                  if ( m_sysExBufferIndex == m_sysExBufferSize) {
                        m_sysExError = true;
                        m_sysExMode = false;
                        m_sysExMsgLen = m_sysExBufferIndex;
                  }
                  else {
                      m_sysExBuffer[m_sysExBufferIndex++] =   readByte ;
                      m_isByteCaptured = true;
                  }
              }
              return false;
          }

          // "Pure" midi message data

          // check if Running status set and if so, generate a true midi channel msg with
          // the previous one. Possible only if filters matchs.
          if (m_runningStatusPossible) {
                m_nextMidiMsglen = getMidiStatusMsgLen(m_midiMsg[0])-1;
                m_runningStatusPossible = false;
          }

          // Len was set only if filters matched before
          if ( m_nextMidiMsglen ) {

            m_midiMsg[m_expectedMsgLen-m_nextMidiMsglen] = readByte;
            m_nextMidiMsglen--;
            m_isByteCaptured = true;

            // Message complete ?
            // Enable running status if it is a message channel.
            if (m_nextMidiMsglen == 0) {
                m_midiParsedMsgType = m_midiCurrentMsgType;
                if (m_midiParsedMsgType == channelVoiceMsgTypeMsk) {
                  m_runningStatusPossible = true;
                }
                m_isMsgReady = true;
                return m_isMsgReady;
            }
          }

     } // Midi data from 00 to 0X7F

     // All other data here are purely ignored.
     // In respect of the MIDI specifications.

    return m_isMsgReady;
}

/////////////////////////////////////////////////
// UTILITY : ENCODE 8BITS TO 7BITS SYSEX
/////////////////////////////////////////////////
unsigned midiXparser::encodeSysEx(const byte* inData, byte* outSysEx, unsigned inLength,bool fromMsbToLsb)
{
    unsigned outLength  = 0;     // Num bytes in output array.
    byte count          = 0;     // Num 7bytes in a block.
    outSysEx[0]         = 0;

    for (unsigned i = 0; i < inLength; ++i)
    {
        const byte data = inData[i];
        const byte msb  = data >> 7;
        const byte body = data & 0x7f;

        outSysEx[0] |= (msb << (fromMsbToLsb ? 6-count : count ));
        outSysEx[1 + count] = body;

        if (count++ == 6)
        {
            outSysEx   += 8;
            outLength  += 8;
            outSysEx[0] = 0;
            count       = 0;
        }
    }
    return outLength + count + (count != 0 ? 1 : 0);
}

/////////////////////////////////////////////////
// UTILITY DECODE 7BITS SYSEX TO 8BITS
/////////////////////////////////////////////////
unsigned midiXparser::decodeSysEx(const byte* inSysEx, byte* outData, unsigned inLength,bool fromMsbToLsb)
{
    unsigned count  = 0;
    byte msbStorage = 0;
    byte byteIndex  = 0;

    for (unsigned i = 0; i < inLength; ++i)
    {
        if ((i % 8) == 0)
        {
            msbStorage = inSysEx[i];
            byteIndex  = 6;
        }
        else
        {
            const byte body = inSysEx[i];
            const byte msb  = ((msbStorage >> (fromMsbToLsb ? byteIndex : 6 - byteIndex) ) & 1) << 7;
            byteIndex--;
            outData[count++] = msb | body;
        }
    }
    return count;
}
