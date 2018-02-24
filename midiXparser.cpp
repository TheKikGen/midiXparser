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

midiXparser::midiXparser() {
    m_channelMsgFilterMask = noteOffMsk | noteOnMsk ;  
};

midiXparser::midiXparser(uint8_t  channelMsgFilterMask) {
     setChannelMsgFilterMask(channelMsgFilterMask);
};

midiXparser::midiXparser(uint8_t  channelMsgFilterMask,uint8_t  midiChannelFilter) {
     setChannelMsgFilterMask(channelMsgFilterMask);
     setMidiChannelFilter(midiChannelFilter);
};

// to Check if the last byte was captured in the midi msg
bool midiXparser::isByteCaptured() { return m_isByteCaptured; }

uint8_t * midiXparser::getMidiMsg() { return m_midiMsg ;}

byte midiXparser::getByte() { return m_readByte ;}

void midiXparser::setMidiChannelFilter(uint8_t midiChannelFilter) {
      m_midiChannelFilter = midiChannelFilter;
}

void midiXparser::setChannelMsgFilterMask(uint8_t channelMsgFilterMask) {
      m_channelMsgFilterMask = channelMsgFilterMask;
}

// Parse midi byte per byte
bool midiXparser::parse(byte readByte) {

    // Store the passed byte so it can be sent back to serial
    // if not captured
    m_readByte = readByte;
    m_isByteCaptured=false;

    // Real time events are not processed
    if  ( readByte >= timingClockStatus ) return false;

    // System common / exclusive are not processed here
    // but we take care...
    if  ( readByte >= soxStatus) {
        if ( readByte == soxStatus ) m_sysExMode = true;
        else m_sysExMode = false;
        m_runningStatusPossible=false; 
        return false;
    }

    // Channel messages
    if ( readByte >= noteOffStatus ) {

        m_sysExMode = false;
        m_runningStatusPossible=false; 
        
        // Midi channel filter
        if ( m_midiChannelFilter != 0xFF &&
                 (readByte & 0x0F) != m_midiChannelFilter ) {
             return false;
        }

        // Apply message channel filter mask
        m_isByteCaptured = ( m_channelMsgFilterMask & ( 1 << ( 14-(readByte >> 4) )  )  ) ? true:false;
        m_nextMidiMsglen = 2;
        m_midiMsg[0] = readByte;
    }
    else { // Midi Data from 00 to 0X7F

        // Capture the message eventually
        if (m_sysExMode ) return false;
        else
        if ( m_nextMidiMsglen ) {
          m_midiMsg[3-m_nextMidiMsglen] = readByte;
          // Apply message channel filter mask
          m_isByteCaptured = ( m_channelMsgFilterMask & ( 1 << ( 14-(m_midiMsg[0] >> 4) ) )  ) ? true:false;
          if (--m_nextMidiMsglen == 0) { 
              m_runningStatusPossible = true; 
              return m_isByteCaptured;
          }
        } else if (m_runningStatusPossible) {
          // check if Running status and if so, generate a true midi channel msg with
          // the previous  one.
              m_nextMidiMsglen = 1;
              m_midiMsg[1] = readByte;
              m_runningStatusPossible = false;
        } 
        
    } // Midi data from 00 to 0X7F
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
