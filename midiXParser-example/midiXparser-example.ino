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

#include <midiXparser.h>

midiXparser midiParser1, midiParser2;

void setup()
{
  Serial.begin(31250);
  midiParser1.setMidiMsgFilter( midiXparser::channelVoiceMsgTypeMsk );
  // We create a second parser here for the demo,
  // but this could be added to the filter of the 1st one to produce the same result.
  // By testing the midi status byte at getMidiMsg()[0]

  midiParser2.setMidiMsgFilter(midiXparser::realTimeMsgTypeMsk  );

}

void loop()
{
  if ( Serial.available()  ) {

      byte receivedByte = Serial.read();

      if ( midiParser1.parse( receivedByte ) ) { // Do we received a channel voice msg ?

        // Echo the note received
         if ( midiParser1.isMidiStatus(midiXparser::noteOffStatus) || midiParser1.isMidiStatus(midiXparser::noteOnStatus) ) {
             delay(200);
             Serial.write(midiParser1.getMidiMsg(),midiParser1.getMidiMsgLen());
         }
      }

      if ( midiParser2.parse( receivedByte ) ) {
         // Do something for realtime msg....

      }

  }
}
