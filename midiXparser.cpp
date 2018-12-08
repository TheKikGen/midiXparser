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

// Give the current sysex state, even if not filtering bytes
bool midiXparser::isSysExMode() { return m_sysExMode ;}

// Give the status of the last SYSEX transmission.
bool midiXparser::isSysExError() { return m_sysExError ;}

// Used to check if the last byte parsed was captured
bool midiXparser::isByteCaptured() { return m_isByteCaptured; }

// Return the type of the last parsed midi message
uint8_t  midiXparser::getMidiMsgType() { return m_midiParsedMsgType; }

// Return the type of a midi status (cf enum)
uint8_t  midiXparser::getMidiStatusMsgType(uint8_t midiStatus) {

  if (midiStatus >= 0xF8 ) return realTimeMsgType;
  if (midiStatus >= 0xF0 ) return systemCommonMsgType;
  if (midiStatus >= 0x80 ) return channelVoiceMsgType;

  return noneMsgType ;
}

// Return the len of the last parsed midi message, including sysex
uint8_t  midiXparser::getMidiMsgLen() {
  if (m_midiParsedMsgType == sysExMsgType )         return m_sysExBufferIndex ;
  if (m_midiParsedMsgType == realTimeMsgType )      return 1 ;
  if (m_midiParsedMsgType == channelVoiceMsgType )  return m_channelVoiceMsgMsglen[ (getMidiMsg()[0] >> 4) - 8 ] ;
  if (m_midiParsedMsgType == systemCommonMsgType )  return m_systemCommonMsglen[getMidiMsg()[0] & 0x0F] ;

  return 0;
}

// Return the len of a midistatus message (cf enum)
uint8_t midiXparser::getMidiStatusMsgLen(uint8_t midiStatus) {

  if (midiStatus >= 0xF8 ) return 1;
  if (midiStatus >= 0xF0 ) return m_systemCommonMsglen[midiStatus & 0x0F];
  if (midiStatus >= 0x80 ) return m_channelVoiceMsgMsglen[ (midiStatus >> 4) - 8 ];

  return 0 ;
}

// Return the len of the last Sysex msg.
// This persists until the next sysex.
unsigned midiXparser::getSysExMsgLen() { return m_sysExBufferIndex ;}

uint8_t * midiXparser::getMidiMsg() {

    switch (m_midiParsedMsgType) {

      case realTimeMsgType:
        return m_midiMsgRealTime;
        break;
      case sysExMsgType:
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

// Set filter for Midi Channel
void midiXparser::setMidiChannelFilter(uint8_t midiChannelFilter) {
      m_midiChannelFilter = midiChannelFilter;
}

// Set filter mask for channel voice Msg
void midiXparser::setChannelVoiceMsgFilter(uint8_t channelVoiceMsgFilterMask) {
      m_channelVoiceMsgFilterMask = channelVoiceMsgFilterMask;
}

// Set filter mask for system Common Msg
void midiXparser::setSystemCommonMsgFilter(uint8_t systemCommonMsgFilterMask) {
      m_systemCommonMsgFilterMask = systemCommonMsgFilterMask;
}

// Set filter mask for realtime Msg
void midiXparser::setRealTimeMsgFilter(uint8_t realTimeMsgFilterMask) {
      m_realTimeMsgFilterMask = realTimeMsgFilterMask;
}

// Set filter mask all/none for all midi Msg but Sysex
void midiXparser::setMidiMsgFilter(allNoValues value) {

   if (value == allMidiMsg ) {
      setChannelVoiceMsgFilter(allChannelVoiceMsgMsk);
      setSystemCommonMsgFilter(allSystemCommonMsgMsk) ;
      setRealTimeMsgFilter(allRealTimeMsgMsk);
      return;
   }

   if (value == noMidiMsg ) {
      setChannelVoiceMsgFilter(noChannelVoiceMsgMsk);
      setSystemCommonMsgFilter(noSystemCommonMsgMsk) ;
      setRealTimeMsgFilter(noRealTimeMsgMsk);
      return;
   }

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
bool midiXparser::setSysExFilter(bool sysExFilterToggle,unsigned sysExBufferSize) {

    // Already allocated. Free buffer.
    if ( m_sysExBuffer!= NULL) free(m_sysExBuffer);

    m_sysExBufferSize  = sysExBufferSize ;
    m_sysExBufferIndex = 0 ;
    m_sysExFilterToggle = sysExFilterToggle;
    m_sysExBuffer = NULL ;

    if ( sysExFilterToggle ) {
      // On the fly mode
      if (sysExBufferSize == 0 ) return true ;

      // (re)Allocate the buffer
      m_sysExBuffer = (uint8_t *)calloc ( sysExBufferSize , sizeof(uint8_t) );
      if ( m_sysExBuffer != NULL ) return true;
      else {
           // Allocation error. Disable SYSEX filter.
           m_sysExBufferSize  = 0 ;
           m_sysExFilterToggle = false;
           return false;
       }
    } else m_sysExBufferSize = 0;

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

    // Clear any previous sysex error
    if (m_sysExError) m_sysExError = false;

    // MIDI Message status are starting at 0x80
    if ( readByte >= 0x80 )
    {
       // Real time messages must be processed as transparent for all other status
       if  ( readByte >= 0xF8 ) {
            // Apply message channel filter mask
            m_isByteCaptured = ( m_realTimeMsgFilterMask & ( 1 << ( 15-(readByte & 0x0F) )  )  ) ? true:false;
            if (m_isByteCaptured) {
              m_midiParsedMsgType = realTimeMsgType;
              m_midiMsgRealTime[0] = readByte;
            }
            m_isMsgReady = m_isByteCaptured;
            return m_isMsgReady;
       }

       // Running status not possible at this point
       m_runningStatusPossible=false;

       // Reset current msg type and msg len
       m_midiCurrentMsgType = noneMsgType;
       m_nextMidiMsglen = 0;
       m_expectedMsgLen = 0;

       // Start SYSEX ---------------------------------------------------------

       // Clean end of Sysex.
       // Sysex can be terminated abnormally also by another status
       if (m_sysExMode && readByte == eoxStatus ) {
               m_sysExMode = false;
               m_sysExError = false;
               m_midiParsedMsgType = sysExMsgType;
               m_isByteCaptured = m_sysExFilterToggle;
               m_isMsgReady = (m_sysExFilterToggle && m_sysExBufferIndex > 0);
               return m_isMsgReady;
       }

       // SysEx can be terminated abonrmally with a midi status.
       // In that case, the SYSEX msg will be dropped to proceed the midi command.
       // The message is still buffered. m_sysExError is set to true for this round.

       if (m_sysExMode ) {
              m_sysExMode = false;
              m_sysExError = true;
       }

       // Start SYSEX
       if ( readByte == soxStatus ) {
              m_sysExMode = true;
              m_sysExBufferIndex = 0;
              m_midiCurrentMsgType = sysExMsgType;
              m_isByteCaptured = m_sysExFilterToggle;
              m_isMsgReady = false;
              return m_isMsgReady;
       }

      // Channel messages between 0x80 and 0xEF ------------------------------------
      if ( readByte <= 0xEF ) {

          m_midiCurrentMsgType = channelVoiceMsgType;

          // Midi channel filter
          if ( m_midiChannelFilter != allChannel &&
                   (readByte & 0x0F) != m_midiChannelFilter ) {
                   m_isMsgReady = false;
                   return m_isMsgReady;
          }

          // Apply message channel filter mask
          m_isByteCaptured = ( m_channelVoiceMsgFilterMask & ( 1 << ( 15-(readByte >> 4) )  )  ) ? true:false;
      }

      // System common messages between 0xF0 and 0xF7 -----------------------------
      else {
          m_midiCurrentMsgType = systemCommonMsgType;
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
          m_midiParsedMsgType = m_midiCurrentMsgType;
          m_isMsgReady = true;
          return m_isMsgReady;
        }
      }

    }

    // Midi Data from 00 to 0X7F --------------------------------------------------
    else {

          // Capture the SYSEX message if filter is set
          // If m_sysExBufferSize is 0, do not store
          if (m_sysExMode ) {
              if (m_sysExFilterToggle) {
                  if (m_sysExBufferSize == 0 ) m_isByteCaptured = true; // On the fly
                  else
                  // Check overflow of the sysex buffer
                  if ( m_sysExBufferIndex == m_sysExBufferSize) {
                        m_sysExError = true;
                        m_sysExMode = false;
                  }
                  else {
                      m_sysExBuffer[m_sysExBufferIndex++] =   readByte ;
                      m_isByteCaptured = true;
                  }
              } // Toggle
              m_isMsgReady = false;
              return m_isMsgReady;
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
                if (m_midiParsedMsgType == channelVoiceMsgType) {
                  m_runningStatusPossible = true;
                }
                m_isMsgReady = true;
                return m_isMsgReady;
            }
          }

     } // Midi data from 00 to 0X7F

     // All other data here are purely ignored.
     // In respect of the MIDI specifications.
     m_isMsgReady = false;

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
