# midiXparser
A small footprint midi parser class


The midiXparser class allows to create rapidily "low level" midi parsers with a small footprint in your applications.
It is platform agnostic so can be used with the Arduino platform or with the AVR c++ compiler.

midiXParser class has a full midi specs coverage, including running status and sysex, and do not rely on serial ports. 
You can pass a byte coming from an array (as done in demo sketch) or even a file.

The demo sketch contains many tests to validate midiXparser features you can use as examples for your own code.

### Examples

#### Declare a parser with a filter on Note off/ Note on


    midiXparser midiParser;
    midiParser.setChannelVoiceMsgFilter(midiXparser::noteOffMsk | midiXparser::noteOnMsk );  
    loop() 
    {
        if ( Serial.available() && ( midiParser.parse( Serial.read() ) {
        // Do something for notes on/off

        } 
        (...)  
    }

#### Capture sysex messages with buffering

    midiXparser midiParser;
    midiParser.setSysExFilter(true,64);
    loop() 
    {
        if ( Serial.available() && ( midiParser.parse( Serial.read() ) {
        // Do something for sysex
          if ( midiParser->getMidiMsgType() == midiXparser::sysExMsgType ) {
            for (int j = 0 ; j < midiParser.getSysExMsgLen() ; j++) { 
                Serial.print(midiParser->getSysExMsg()[j],HEX);
                Serial.print(" - ");            
                (...)  
            } 
           }
           (...)  
        }
    
#### Remove Active sense, clock and SongPointer from the MIDI flow

    midiXparser midiParser;
    midiParser.setRealTimeMsgFilter(midiXparser::activeSensingMsk | midiXparser::timingClockMsk );
    midiParser.setSystemCommonMsgFilter(midiXparser::songPosPointerMsk);
    loop() 
    {    
        if ( Serial.available() ) {
        
           if ( midiParser.parse( Serial.read() ) {
              // Do something for active sense, clock and song pos

           } else if (!midiParser.isByteCaptured()) Serial.write(midiParser.getByte() ) ;
           (...)
        }
    }


### Methods

#### bool parse(byte readByte)
    Parse a byte and return true if a complete midi message was detected, according to the filters set.
    This method is also used for sysex, buffered or unbuffered with setSysExFilter.
    parse method will never return true for a sysex error. 
    You have to check isSysexError() for an incomplete messsages.

#### uint8_t *getMidiMsg()
    Get a pointer on the parsed midi message.

#### uint8_t *getSysExMsg()
    Get a pointer on the sysex parsed message ONLY when buffered.  Do not use this buffer is you did not
    pass any size to setSysExFilter. Results will be impredictable....
    The parsed message contains only data, and never 0XF0 (SOX) at the beginning, and 0XF7 (EOX) at the end.

#### uint8_t getMidiMsgLen()
    Return the length of the last parsed message, including parsed sysex mssages.
    Return 0 if the parse method does not return true at the last call.    
      
#### unsigned getSysExMsgLen() ;
    Return the length of a sysex message, whithout SOX and EOX, complete or not.

#### byte getByte()
    Return the last byte parsed.

#### byte getPreviousByte()
    Return the previous byte parsed.

#### bool isSysExMode()
    Return true if the last call to the parse method has entering into system exclusive mode.  
    This method can be used in combination with the IsByteCaptured method to process sysex flows
    on the fly, without buffering.    

#### bool wasSysExMode()
    Return true if the last call to the parse method has stopped the sysex mode, because of EOX or an sysex error.
    The sysex message is stored if with Setfilter with a size was used.
    
#### bool isSysExError()
    Return true if the last call to the parse method has detected an abnormal end of sysex,
    generally a midi status (a byte > 0x80) no beeing the EOX byte, or a sysex buffer overflow.
  
#### bool isByteCaptured()  
    Return true if the last byte parsed is used to prepare a midi message.

#### uint8_t getMidiMsgType()
    Return the type of the last parsed midi message. 
    
    Types are defined by the following enumeration :
    . midiXparser::noneMsgType
    . midiXparser::channelVoiceMsgType
    . midiXparser::systemCommonMsgType
    . midiXparser::realTimeMsgType
    . midiXparser::sysExMsgType
    
#### STATIC uint8_t getMidiStatusMsgLen(uint8_t midiStatus)
    Return the standard length of a midi status. Midi status are defined 
    by the following enumaration :
    
    CHANNEL VOICE
    . midiXparser::noteOffStatus
    . midiXparser::noteOnStatus
    . midiXparser::polyKeyPressureStatus
    . midiXparser::controlChangeStatus
    . midiXparser::programChangeStatus
    . midiXparser::channelPressureStatus
    . midiXparser::pitchBendStatus
    
    SYSTEM COMMON
    . midiXparser::soxStatus
    . midiXparser::midiTimeCodeStatus
    . midiXparser::songPosPointerStatus
    . midiXparser::songSelectStatus
    . midiXparser::reserved1Status
    . midiXparser::reserved2Status
    . midiXparser::tuneRequestStatus
    . midiXparser::eoxStatus
    
    REAL TIME
     . midiXparser::timingClockStatus
     . midiXparser::reserved3Status
     . midiXparser::startStatus
     . midiXparser::continueStatus
     . midiXparser::stopStatus
     . midiXparser::reserved4Status
     . midiXparser::activeSensingStatus
     . midiXparser::systemResetStatus

#### STATIC uint8_t getMidiStatusMsgType(uint8_t midiStatus)    
    . Return the msg type of a midi status (see also getMidiMsgType)
 
#### void setMidiChannelFilter(uint8_t midiChannelFilter)
    . Set a filter to return only messages of the concerned midi channel.

#### void setChannelVoiceMsgFilter(uint8_t channelVoiceMsgFilterMask)
    TODO

#### void setSystemCommonMsgFilter(uint8_t systemCommonMsgFilterMask)
    TODO

#### void setRealTimeMsgFilter(uint8_t realTimeMsgFilterMask)
    TODO

#### void setMidiMsgFilter(allNoValues value)
    TODO

#### bool setSysExFilter(bool sysExFilterToggle,unsigned sysExBufferSize=0)
    . sysExFilter(true,0) will start filtering sysex messages without buffering.  You can
    use the isByteCaptured and the isSysexMode methods to process the sysex midiflow.
    . sysExFilter(true, nn) will start filtering sysex messages with buffering, so when 
    the parse method will return true, bytes will be directly available.
    A clean end of sysex will make the parse methode to return true, buffered or not.

#### (static) unsigned encodeSysEx(const byte* inData, byte* outSysEx, unsigned inLength,bool fromMsbToLsb=true)
    TODO

#### (static) unsigned decodeSysEx(const byte* inSysEx, byte* outData, unsigned inLength,bool fromMsbToLsb=true)
    TODO

