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

static const  uint8_t midiXparser::m_systemCommonMsglen[]={
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

// Constructors
midiXparser::midiXparser() {

};

// Contructor with passing channel voice filter mask
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

// Used to check if the last byte parsed was captured
bool midiXparser::isByteCaptured() { return m_isByteCaptured; }

// Return the type of the last parsed midi message
midiXparser::midiMsgType  midiXparser::getMidiMsgType() { return m_midiParsedMsgType; }

// Return the type of a midi status (cf enum)
midiXparser::midiMsgType  midiXparser::getMidiStatusMsgType(midiXparser::midiStatusValue midiStatus) {

  if (midiStatus >= 0xF8 ) return realTimeMsgType;
  if (midiStatus >= 0xF0 ) return systemCommonMsgType;
  if (midiStatus >= 0x80 ) return channelVoiceMsgType;

  return noneMsgType ;
}

// Return the len of the last parsed midi message, including sysex
uint8_t  midiXparser::getMidiMsgLen() {
  if (m_midiParsedMsgType == sysExMsgType )         return m_sysExBufferIndex ;
  if (m_midiParsedMsgType == realTimeMsgType )      return 1 ;
  if (m_midiParsedMsgType == channelVoiceMsgType )  return 3 ;
  if (m_midiParsedMsgType == systemCommonMsgType )  return m_systemCommonMsglen[getMidiMsg()[0] & 0x0F] ;

  return 0;
}

// Return the len of a midistatus message (cf enum)
uint8_t midiXparser::getMidiStatusMsgLen(midiXparser::midiStatusValue midiStatus) {

  if (midiStatus >= 0xF8 ) return 1;
  if (midiStatus >= 0xF0 ) return m_systemCommonMsglen[midiStatus & 0x0F];
  if (midiStatus >= 0x80 ) return 3;

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

// Allocate a sysex buffer and start listening msg
// If sysExFilterToggle is false, the memory is released.
// This function uses dynamic memory allocation, so the DESTRUCTOR
// must clean everything.
bool midiXparser::setSysExFilter(bool sysExFilterToggle,int sysExBufferSize) {

    if ( sysExFilterToggle ) {
      if (sysExBufferSize <= 0) return false ;

      
      // Already allocated. Free buffer.
      if ( m_sysExBuffer!= NULL && m_sysExFilterToggle  ) {
        free(m_sysExBuffer);
        m_sysExBufferSize = 0;
        m_sysExBufferIndex=0;
        m_sysExBuffer = NULL;
        m_sysExFilterToggle = false;
      }

      // (re)Allocate the buffer
      m_sysExBuffer = calloc ( sysExBufferSize , sizeof(uint8_t) );
      if ( m_sysExBuffer != NULL ) {
           m_sysExBufferSize = sysExBufferSize ;
           m_sysExBufferIndex=0;
           m_sysExFilterToggle = true;
           return true;
      }
    }
    else

    // if an array was already allocated, free it
    if ( m_sysExBuffer!= NULL && m_sysExFilterToggle  ) {
        free(m_sysExBuffer);
        m_sysExBuffer = NULL;
        m_sysExBufferSize = 0;
        m_sysExBufferIndex=0;
        m_sysExFilterToggle = false;
        return true;
    }

    return false;

}
////////////////////////////////////////////////
// midiXParser MIDI PARSER
//-----------------------------------------------
// The main method.
// It parses midi byte per byte and return true
// if a message atching filters was red.
// Set also the byte capture flag
////////////////////////////////////////////////
bool midiXparser::parse(byte readByte) {

    // Store the passed byte so it can be sent back to serial
    // if not captured
    m_readByte = readByte;
    m_isByteCaptured=false;

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
            return m_isByteCaptured;
       }

       // Running status not possible at this point
       m_runningStatusPossible=false;

       // Reset current msg type and msg len
       m_midiCurrentMsgType = noneMsgType;
       m_nextMidiMsglen = 0;

       // Start SYSEX ? ------------------------------------------------------------
       if ( !m_sysExMode && readByte == soxStatus ) {
              m_sysExMode = true;
              m_sysExBufferIndex = 0;
              m_midiCurrentMsgType = sysExMsgType;
              m_isByteCaptured = m_sysExFilterToggle;
              return false;
       }

      // Clean end of Sysex.
      // Sysex can be terminated also by another status
      if (m_sysExMode && readByte == eoxStatus ) {
              m_sysExMode = false;
              m_midiParsedMsgType = sysExMsgType;
              m_isByteCaptured = m_sysExFilterToggle;
              return (m_sysExFilterToggle && m_sysExBufferIndex > 0);
      }

      // Channel messages between 0x80 and 0xEF ------------------------------------
      if ( readByte <= 0xEF ) {


          m_midiCurrentMsgType = channelVoiceMsgType;

          // Midi channel filter
          if ( m_midiChannelFilter != allChannel &&
                   (readByte & 0x0F) != m_midiChannelFilter ) {
                   if (m_sysExMode) {
                      m_sysExMode = false;
                      return m_sysExFilterToggle;
                   }
                   return false;
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
        m_nextMidiMsglen = getMidiStatusMsgLen(readByte);
        m_midiMsg[0] = readByte;
        m_nextMidiMsglen--;
      }

      // SysEx can be terminated with a midi status. Not a clean end of Sysex.
      // If the len of the current midi message is only 1 byte (eg Tune request), the arbitration will
      // go to the midi sysex instead of the tune request. So, we "consume" the event here.
      // This is the only case.
      if (m_sysExMode ) {
              m_sysExMode = false;
              if ( m_sysExBufferIndex > 0 ) {
                m_midiParsedMsgType = sysExMsgType;
                return true;
              }
      }

      // Case of 1 byte len midi msg (Tune request)
      if ( m_nextMidiMsglen == 0 && m_isByteCaptured) {
          m_midiParsedMsgType = m_midiCurrentMsgType;
          return true;
      }

    }

    // Midi Data from 00 to 0X7F --------------------------------------------------
    else {

          // Capture the SYSEX message if filter is set
          if (m_sysExMode ) {
              if (m_sysExFilterToggle) {
                  // Check overflow of the sysex buffer
                  if ( m_sysExBufferIndex == m_sysExBufferSize)
                      m_sysExMode = false;
                  else {
                      m_sysExBuffer[m_sysExBufferIndex++] =   readByte ;
                      m_isByteCaptured = true;
                  }
              } // Toggle
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

            m_midiMsg[3-m_nextMidiMsglen] = readByte;
            m_nextMidiMsglen--;
            m_isByteCaptured = true;

            // Message complete ?
            // Enable running status if it is a message channel.
            if (m_nextMidiMsglen == 0) {
                m_midiParsedMsgType = m_midiCurrentMsgType;
                if (m_midiParsedMsgType == channelVoiceMsgType) {
                  m_runningStatusPossible = true;
                }
                return true;
            }
          }

     } // Midi data from 00 to 0X7F

     // All other data here are purely ignored.
     // In respect of the MIDI specifications.

    return false;
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
