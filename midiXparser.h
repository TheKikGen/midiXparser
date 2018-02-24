#pragma once

#ifndef midiXparser_h
#define midiXparser_h

#if ARDUINO
#include <Arduino.h>
#else
#include <inttypes.h>
typedef uint8_t byte;
#endif

/*

  midXparser
  A small footprint midi parser.
  Copyright (C) 2017/2018 by The KikGen labs.

  HEADER CLASS FILE
  
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
class midiXparser {
  uint8_t  m_midiMsg[3];
  uint8_t  m_nextMidiMsglen = 0;
  bool     m_sysExMode = false;
  bool     m_isByteCaptured=false;
  byte     m_readByte;
  bool     m_runningStatusPossible=false; 
  uint8_t  m_channelMsgFilterMask = B0000000;
  uint8_t  m_midiChannelFilter    = 0xFF;

  public:
    // Bits 6 to 0 map status 8n 9n An Bn Cn Dn En
    enum channelMsgFilterMaskValue {
        noteOffMsk           = B1000000,
        noteOnMsk            = B0100000,
        polyKeyPressureMsk   = B0010000,
        controlChangeMsk     = B0001000,
        programChangeMsk     = B0000100,        
        channelPressureMsk   = B0000010,
        pitchBendMsk         = B0000001                       
    };

    enum midiStatusValue {
        // CHANNEL VOICE
        noteOffStatus         = 0X80,
        noteOnStatus          = 0X90,
        polyKeyPressureStatus = 0XA0,
        controlChangeStatus   = 0XB0,
        programChangeStatus   = 0XC0,        
        channelPressureStatus = 0XD0,
        pitchBendStatus       = 0XE0,
        // SYSTEM COMMON
        soxStatus             = 0XF0,
        midiTimeCodeStatus    = 0XF1,
        songPosPointerStatus  = 0XF2,
        songSelectStatus      = 0XF3,
        reserved1Status       = 0XF4,
        reserved2Status       = 0XF5,
        tuneRequestStatus     = 0XF6,
        eoxStatus             = 0XF7,
        // REAL TIME
        timingClockStatus     = 0XF8,
        reserved3Status       = 0XF9,
        startStatus           = 0XFA,
        continueStatus        = 0XFB,
        stopStatus            = 0XFC,
        reserved4Status       = 0XFD,
        activeSensingStatus   = 0XFE,
        systemResetStatus     = 0XFF                                      
    };

    midiXparser();
    midiXparser(uint8_t channelMsgFilterMask) ;
    midiXparser(uint8_t  channelMsgFilterMask,uint8_t  midiChannelFilter);
  
    bool midiXparser::isByteCaptured() ;
    byte midiXparser::getByte() ;
    uint8_t * midiXparser::getMidiMsg();
    void midiXparser::setChannelMsgFilterMask(uint8_t channelMsgFilterMask);
    void midiXparser::setMidiChannelFilter(uint8_t midiChannelFilter);
    bool midiXparser::parse(byte readByte);

    // Utilities
    static unsigned midiXparser::encodeSysEx(const byte* inData, byte* outSysEx, unsigned inLength,bool fromMsbToLsb=true);
    static unsigned midiXparser::decodeSysEx(const byte* inSysEx, byte* outData, unsigned inLength,bool fromMsbToLsb=true);
    
};

#endif

