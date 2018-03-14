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

#### Capture sysex messages

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
              // Do something for active sense  and clock

           } else if (!midiParser.isByteCaptured()) Serial.write(midiParser.getByte() ) ;
           (...)
        }
    }


