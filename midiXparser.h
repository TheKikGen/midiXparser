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
#pragma once

#ifndef midiXparser_h
#define midiXparser_h

#if ARDUINO
#include <Arduino.h>
#else
#include <inttypes.h>
typedef uint8_t byte;
#endif
#include <stdlib.h>

class midiXparser {
  uint8_t  m_midiMsg[3];
  uint8_t  m_midiMsgRealTime[1]; // Used for real time only
  uint8_t  m_nextMidiMsglen = 0;
  uint8_t  m_expectedMsgLen = 0;
  bool     m_sysExMode = false;
  bool     m_sysExError = false;
  bool     m_isByteCaptured=false;
  bool     m_isMsgReady=false;
  byte     m_readByte = 0;
  byte     m_previousReadByte = 0;
  bool     m_runningStatusPossible=false;
  uint8_t  m_channelVoiceMsgFilterMask = noChannelVoiceMsgMsk;
  uint8_t  m_systemCommonMsgFilterMask = noSystemCommonMsgMsk;
  uint8_t  m_realTimeMsgFilterMask = noRealTimeMsgMsk;
  uint8_t  m_midiChannelFilter    = allChannel;
  uint8_t  m_midiMsgTypeFilterMsk = noneMsgTypeMsk;
  uint8_t  m_midiParsedMsgType    = noneMsgTypeMsk;
  uint8_t  m_midiCurrentMsgType   = noneMsgTypeMsk;

  static const  uint8_t m_systemCommonMsglen[8];
  static const  uint8_t m_channelVoiceMsgMsglen[7];

  uint8_t* m_sysExBuffer = NULL;
  unsigned m_sysExBufferSize  = 0;
  unsigned m_sysExBufferIndex = 0;
  //bool     m_sysExFilterToggle = false;

  public:
    // Midi messages type
    enum midiMsgTypeMaskValue {
        noneMsgTypeMsk          = 0B0000,
        channelVoiceMsgTypeMsk  = 0B0001,
        systemCommonMsgTypeMsk  = 0B0010,
        realTimeMsgTypeMsk      = 0B0100,
        sysExMsgTypeMsk         = 0B1000,
        allMsgTypeMsk           = 0B1111
    };

    enum allNoValues {
        allChannel = 0xFF,
    };

    // Bits 6 to 0 map status 8n 9n An Bn Cn Dn En
    enum channelVoiceMsgFilterMaskValue {
        noteOffMsk            = 0B10000000,
        noteOnMsk             = 0B01000000,
        polyKeyPressureMsk    = 0B00100000,
        controlChangeMsk      = 0B00010000,
        programChangeMsk      = 0B00001000,
        channelPressureMsk    = 0B00000100,
        pitchBendMsk          = 0B00000010,
        allChannelVoiceMsgMsk = 0B11111110,
        noChannelVoiceMsgMsk  = 0B00000000,
    };

    // Bits 7 to 0 map realtime F8 to FF
    enum realTimeMsgFilterMaskValue {
        timingClockMsk      = 0B10000000,
        //reserved3Msk      = 0B01000000,
        startMsk            = 0B00100000,
        continueMsk         = 0B00010000,
        stopMsk             = 0B00001000,
        //reserved4Msk      = 0B00000100,
        activeSensingMsk    = 0B00000010,
        systemResetMsk      = 0B00000001,
        allRealTimeMsgMsk   = 0B10111011,
        noRealTimeMsgMsk    = 0B00000000
    };

    // Bits 7 to 0 map realtime (F0) to (F7)
    enum systemCommonMsgFilterMaskValue {
        //soxMsk              = 0B10000000,
        midiTimeCodeMsk       = 0B01000000,
        songPosPointerMsk     = 0B00100000,
        songSelectMsk         = 0B00010000,
        //reserved1Status     = 0B00001000,
        //reserved2Status     = 0B00000100,
        tuneRequestMsk        = 0B00000010,
        //eoxStatus           = 0B00000001,
        allSystemCommonMsgMsk = 0B01110010,
        noSystemCommonMsgMsk  = 0B00000000
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

    // Constructor
    midiXparser();
    midiXparser(uint8_t channelMsgFilterMask) ;
    midiXparser(uint8_t  channelMsgFilterMask,uint8_t  midiChannelFilter);

    // Destructor
    ~midiXparser();

    // Methods
    bool        isSysExMode() ;
    bool        isSysExError();
    bool        isByteCaptured() ;
    uint8_t     getMidiMsgType() ;
    static uint8_t     getMidiStatusMsgTypeMsk(uint8_t midiStatus) ;
    uint8_t     getMidiMsgLen();
    static uint8_t     getMidiStatusMsgLen(uint8_t midiStatus);
    uint8_t *   getMidiMsg();
    uint8_t *   getSysExMsg();
    byte        getByte() ;
    byte        getPreviousByte() ;
    unsigned    getSysExMsgLen() ;
    void        setMidiChannelFilter(uint8_t midiChannelFilter);
    void        setChannelVoiceMsgFilter(uint8_t channelVoiceMsgFilterMask);
    void        setSystemCommonMsgFilter(uint8_t systemCommonMsgFilterMask);
    void        setRealTimeMsgFilter(uint8_t realTimeMsgFilterMask);
    void        setMidiMsgFilter(midiMsgTypeMaskValue value);

    bool        setSysExFilter(bool sysExFilterToggle,unsigned sysExBufferSize=0);

    bool        parse(byte readByte);

    // Utilities
    static unsigned encodeSysEx(const byte* inData, byte* outSysEx, unsigned inLength,bool fromMsbToLsb=true);
    static unsigned decodeSysEx(const byte* inSysEx, byte* outData, unsigned inLength,bool fromMsbToLsb=true);

};



#endif
