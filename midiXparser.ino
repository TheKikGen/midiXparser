/*

  midXparser
  A small footprint midi parser.
  Copyright (C) 2017/2018 by The KikGen labs.

  EXAMPLE SKETCH
  
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
#include <SoftwareSerial.h>

#define RX 2
#define TX 3

SoftwareSerial midiSerial(RX, TX); // RX, TX

// Midi parser
midiXparser midiParser;


/////////////////////////////////////////////////
// MAIN
/////////////////////////////////////////////////

void setup() {
   
  Serial.begin(31250);      // Midi baud rate
  midiSerial.begin(31250);
  
  // Set midi parser filters.  Can also be passed when declaring the instance.
  midiParser.setMidiChannelFilter(1);
  midiParser.setChannelMsgFilterMask(midiXparser::noteOffMsk | midiXparser::noteOnMsk );

}

void loop() {


  // Capture a specific midi msg on Software serial, and send it to hardware serial
  if (midiSerial.available() ) {
    
      if ( midiParser.parse( midiSerial.read() ) ) {
      // As specified in the filters, we are here only if midi msg is note on or note off on channel 1
            
          // just for the demo...
          byte channel      = midiParser.getMidiMsg()[0] & 0x0F;
          byte midiCommand  = midiParser.getMidiMsg()[0] >> 4;
          byte note         = midiParser.getMidiMsg()[1];
          byte velocity     = midiParser.getMidiMsg()[2];

          // Echo back to harware serial
          Serial.write(midiParser.getMidiMsg()[0]);
          Serial.write(midiParser.getMidiMsg()[1]);
          Serial.write(midiParser.getMidiMsg()[2]);
        
      } 
      // if the parsed byte was not a part of a filtered midi msg, the isByteCaptured() return false.  
      // That allows the caller to send back the byte red to serial or to any interface...
      // So we are interface agnostic and fully transparent...
      else if( ! midiParser.isByteCaptured()) midiSerial.write(midiParser.getByte());                      
  } 

}
