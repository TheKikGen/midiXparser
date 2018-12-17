# midiXparser
A small footprint midi parser class


The midiXparser class allows to create rapidily "low level" midi parsers with a small footprint in your applications.
It is platform agnostic so can be used with the Arduino platform or with the AVR c++ compiler.

midiXParser class has a full midi specs coverage, including running status and sysex, and do not rely on serial ports. 
You can pass a byte coming from an array (as done in demo sketch) or even a file.

The demo sketch contains many tests to validate midiXparser features you can use as examples for your own code.

### Examples

#### Declare a parser with a filter on Note off/ Note on

    #include <midiXparser.h>

    midiXparser midiParser1, midiParser2;

    void setup()
    {
      Serial.begin(31250);
      midiParser1.setMidiMsgFilter( midiXparser::channelVoiceMsgTypeMsk );
      // We create a second parser here for the demo,
      // but this could be added to the filter of the 1st one to produce the same result.
      // By testing the midi status byte at getMidiMsg()[0]

      midiParser2.setRealTimeMsgFilter(midiXparser::realTimeMsgTypeMsk  );

    }

    void loop()
    {
      if ( Serial.available()  ) {

          byte receivedByte = Serial.read();

          if ( midiParser1.parse( receivedByte ) ) { // Do we received a channel voice msg ?

              // Set the channel # as enum ,  defined on channel 0.
              uint8_t midiStatus = midiParser1.getMidiMsg[0] & 0xF0;

              // Echo the note received
              if ( midiStatus == midiXparser::noteOffStatus || midiStatus==midiXparser::noteOnStatus) {
                delay(200);
                Serial.write(midiParser1.getMidiMsg(),midiParser1.getMidiMsgLen());
              }
          }

          if ( midiParser2.parse( receivedByte ) ) {
             // Do something for realtime msg....

          }

      }
    }

#### Capture sysex messages with buffering

    midiXparser midiParser;
    midiParser..setMidiMsgFilter( midiXparser::sysExMsgTypeMsk);
    
    loop() 
    {
        if ( Serial.available() && ( midiParser.parse( Serial.read() ) {

          if ( midiParser->getMidiMsgType() == midiXparser::sysExMsgType ) {
                  // Do something for sysex message END  
                  (...)
          }  else 
          
          if ( midiParser->isSysexMode() && midiParser->isByteCaptured() {
                // Do something for sysex data (storing, on the fly process...)
          }
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

#### uint8_t getMidiMsgLen()
    Return the length of the last parsed message, including parsed sysex mssages.
    Return 0 if the parse method does not return true at the last call.    
      
#### unsigned getSysExMsgLen() ;
    Return the length of a sysex message, whithout SOX and EOX, complete or not.

#### byte getByte()
    Return the last byte parsed.

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
 
#### void setMidiMsgFilter(allNoValues value)
    . filter is defined by combining the followings mask with a BINARY OR :
    
        noneMsgTypeMsk          = 0B0000,
        channelVoiceMsgTypeMsk  = 0B0001,
        systemCommonMsgTypeMsk  = 0B0010,
        realTimeMsgTypeMsk      = 0B0100,
        sysExMsgTypeMsk         = 0B1000,
        allMsgTypeMsk           = 0B1111
