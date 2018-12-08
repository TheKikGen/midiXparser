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


static  uint8_t dummyMsg1[] = {
  0x90,0x3A,0x77,
  0x80,0x3A,0x77,
  0x91,0x3B,0x64,
  0x81,0x3B,0x64,
  0x92,0x3C,0x70,
  0x82,0x3C,0x70,
  0xA0,0x64,0x7F,
  0xB1,0x64,0x7F,
  0xC2,0x64,0x7F,
  0xD0,0x64,0x7F,
  0xE2,0x64,0x7F,
  0XFA, 0xFC, 0xFE
}  ;

static  uint8_t dummyMsg2[] = {
  0x90,0xFC,0x3A,0x77,
  0x80,0x3A,0x77,0xFA,
  0xF8,0x91,0xF0,0x3B,0x64,
  0x81,0xFC,0x3B,0x64,0xF6,
  0x92,0xF9,0x3C,0x70,0xF2,0x10,0x11,
  0x82,0x3C,0x70,
   0x82,0x3C,0x70,
  0xA0,0x64,0x7F,
  0xB1,0x64,0x7F,
  0xC2,0x64,0x7F,
  0xD0,0x64,0x7F,
  0xE2,0x64,0x7F
  }  ;

static  uint8_t dummyMsg3[] = {

  0xF0,0x77,0x77,0x78,0xF8,0x40,0x41,0x42,0x43,0xF7,
  0xF0,0x41,0x77,0x78,0xF8,0x40,0xF7,
  0xF0,0x77,0x77,0x77,0xF8,0x0,0x1,0x2,0xF8,0X44,0xF7,
  0xF0,0x77,0x77,0x77,0xF8,0x0,0x1,0x2,0xF8,
  0x80,0x50,0x40,
  0xF0,0x77,0x77,0x77,0xF8,0x40,0x41,0x42,0x43,0xF8,0X44,0xF7,
  0xF0,0x77,0x77,0x77,0x0B,'H','E','L','L','O',0xF7} ;

static  uint8_t dummyMsg4[] = {
  0x90,0x3A,0x77,0x3A,0x00,
  0x80,0x3A,0x77,0xF8,0xB1,0x40,0x7F,0x41,0x7F,0x42,0x7F,
  0x91,0x3B,0xF8,0x64,0x2C,0x64,0x1A,0xF8,0x64,0x2B,0x64,
  0x81,0x3B,0x64
};

static  uint8_t dummyMsg5[] = {
  0x90,0x3A,0x77,
  0xF0,0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0xF7,
  0x80,0x3A,0x00,
  0xF0,0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0xF6,
  0x80,0x3A,0x00,
  0xF0, 0x01 , 0x02 , 0x03 , 0x04 , 0x05 , 0x06 , 0x07 , 0x08 , 0x80 , 0x00 , 0x01,
  0xF0,0x01 ,0x02 ,0x03 ,0x04 ,0x05 ,0x06 ,0x07 ,0x08 ,0xF0 ,0x01 ,0x02 ,0x03 ,0x04 ,0x05 ,0x06 ,0x07 ,0x08 ,0x09 ,0x10 ,0xF7,
  0x80,0x05, 0x06, 0x00,
};

static  uint8_t dummyUsb[] = {
  0x90, 0x3A,  0x77,
  0xF0, 0x01 , 0x02 , 0x03 , 0x04 , 0x05 , 0x06 , 0x07 , 0x08 , 0xF7,
  0x90, 0x3A,  0x77,
  0xF0, 0x01 , 0x02 , 0x03 , 0x04 , 0x05 ,
  0x80,0x00, 0x00,
  0x90, 0x3A,  0x77,
  0xF0, 0x01 , 0x02 , 0x03 , 0x04 , 0x05 , 0x06,
  0x80,0x00, 0x00,
  0xF0, 0x01 , 0x02 , 0x03 , 0x04 , 0x05 , 0x06, 0x07,
  0x80,0x00, 0x00,
  0xF0, 0x01 , 0x02 , 0x03 , 0x04 , 0x05 , 0x06, 0x07, 0x08 ,
  0x80,0x00, 0x00,
  0xF0, 0x01 , 0x02 , 0x03 , 0xF7,
};


// Program change issue
static  uint8_t dummyPCIssue[] = {
  0xB0, 0x34,  0x44,
  0xC0, 0x20,
  0x90, 0x3A,  0x77,
  0xC0, 0x21,
  0x90, 0x3A,  0x77,
  0xD0, 0x40,
  0x80, 0x3A,  0x77,
};

extern unsigned int __bss_end;
extern unsigned int __heap_start;
extern void *__brkval;

int freeMemory() {
  int free_memory;

  if((int)__brkval == 0)
     free_memory = ((int)&free_memory) - ((int)&__bss_end);
  else
    free_memory = ((int)&free_memory) - ((int)__brkval);

  return free_memory;
}


void test10() {

  midiXparser midiParser;
  serializer1("TEST 10","default ",&midiParser,dummyMsg1,sizeof( dummyMsg1) );
}



void test11() {
  midiXparser midiParser;
  midiParser.setChannelVoiceMsgFilter(midiXparser::noteOffMsk | midiXparser::noteOnMsk );
  //midiParser.setChannelMsgFilter(midiXparser::allChannel  );
  serializer1("TEST 11","Note on/off default all channels",&midiParser,dummyMsg1,sizeof( dummyMsg1));
}

void test12() {
  midiXparser midiParser;
  midiParser.setMidiChannelFilter(midiXparser::allChannel);
  midiParser.setMidiMsgFilter( midiXparser::allMidiMsg );
  serializer1("TEST 12","All msg - All channels",&midiParser,dummyMsg1,sizeof( dummyMsg1));
}

void test13() {
  midiXparser midiParser;
  midiParser.setMidiChannelFilter(midiXparser::allChannel);
  midiParser.setMidiMsgFilter( midiXparser::noMidiMsg );
  serializer1("TEST 13","no msg - All channels",&midiParser,dummyMsg1,sizeof( dummyMsg1));
}

void test14() {
  midiXparser midiParser;
  midiParser.setMidiMsgFilter( midiXparser::allMidiMsg );
  midiParser.setMidiChannelFilter(2);
  serializer1("TEST 14","All msg - channel 3",&midiParser,dummyMsg1,sizeof( dummyMsg1));
}

void test15() {
  midiXparser midiParser;
  //midiParser.setMidiMsgFilter( midiXparser::allMidiMsg );
  midiParser.setMidiChannelFilter(midiXparser::allChannel);
  midiParser.setChannelVoiceMsgFilter(midiXparser::controlChangeMsk );
  serializer1("TEST 15","CChange - All channels - Running status",&midiParser,dummyMsg4,sizeof( dummyMsg4));
}

void test16() {

  midiXparser midiParser;
  midiParser.setMidiChannelFilter(midiXparser::allChannel);
  midiParser.setMidiMsgFilter( midiXparser::allMidiMsg );

  serializer1("TEST 10","PC ISSUE",&midiParser,dummyPCIssue,sizeof( dummyPCIssue) );
}


void test20() {
  midiXparser midiParser;
  midiParser.setChannelVoiceMsgFilter(midiXparser::noteOffMsk |midiXparser::pitchBendMsk );
  midiParser.setMidiChannelFilter(2);
  serializer1("TEST 20","Note off channel 3 Pitch Bend",&midiParser,dummyMsg1,sizeof( dummyMsg1) );
}

void test30() {
  midiXparser midiParser;
  midiParser.setChannelVoiceMsgFilter(midiXparser::noteOnMsk |midiXparser::controlChangeMsk );
  midiParser.setMidiChannelFilter(1);
  serializer1("TEST 30","Note on channel 2 CChange ",&midiParser,dummyMsg1,sizeof( dummyMsg1) );
}

void test40() {
  midiXparser midiParser;
  midiParser.setChannelVoiceMsgFilter(midiXparser::noteOnMsk  );
  midiParser.setMidiChannelFilter(2);
  serializer1("TEST 40","Note on channel 3 with real time events mixed",&midiParser,dummyMsg2,sizeof( dummyMsg2) );
}

void test50() {
  midiXparser midiParser;
  midiParser.setRealTimeMsgFilter(midiXparser::startMsk | midiXparser::stopMsk );
  midiParser.setChannelVoiceMsgFilter(midiXparser::noChannelVoiceMsgMsk );
  midiParser.setMidiChannelFilter(1);
  serializer1("TEST 50","Realtime Start Stop mixed with notes not filtered - channel 2",&midiParser,dummyMsg2,sizeof( dummyMsg2) );
}

void test60() {
  midiXparser midiParser;
  midiParser.setRealTimeMsgFilter(midiXparser::startMsk | midiXparser::stopMsk );
  midiParser.setChannelVoiceMsgFilter(midiXparser::noteOffMsk | midiXparser::noteOnMsk );
  midiParser.setSystemCommonMsgFilter(midiXparser::tuneRequestMsk | midiXparser::songPosPointerMsk);
  midiParser.setMidiChannelFilter(1);
  serializer1("TEST 60","Start Stop with Tune Request / Song Pos /note off ch 2 / broken note on",&midiParser,dummyMsg2,sizeof( dummyMsg2) );
}

void test70() {
  midiXparser midiParser;
  midiParser.setSysExFilter(true,64);
  serializer1("TEST 70","SYSEX",&midiParser,dummyMsg3,sizeof( dummyMsg3) );
}

void test71() {
  midiXparser midiParser;
  midiParser.setSysExFilter(true);
  midiParser.setMidiMsgFilter( midiXparser::allMidiMsg );
  serializer1("TEST 71","SYSEX Not storing - on the fly mode",&midiParser,dummyMsg3,sizeof( dummyMsg3) );
}

void test72() {
  midiXparser midiParser;
  midiParser.setSysExFilter(true,0);
   midiParser.setMidiMsgFilter( midiXparser::allMidiMsg );
  serializer1("TEST 72","SYSEX not clean end ",&midiParser,dummyMsg5,sizeof( dummyMsg5) );
}

void test80() {
  midiXparser midiParser;
  midiParser.setSysExFilter(true,4);
  serializer1("TEST 1I","SYSEX OVERFLOW Size = 4",&midiParser,dummyMsg3,sizeof( dummyMsg3) );
}

void test90() {
  Serial.println("================== START ========================");
  Serial.println("TEST90 : SYSEXDynamic allocations tests");
  Serial.println("-------------------------------------------------");
  int startMem = freeMemory();
  Serial.print("Free Memory before calling test90          : ") ; Serial.println(startMem);
  Serial.println("-------------------------------------------------");
  test90b();
  Serial.println("-------------------------------------------------");
  Serial.print("Free Memory gap after calling test90       : ") ; Serial.println(startMem-freeMemory());
  Serial.println("==================== END ========================");
}

void test90b() {
  Serial.println("TEST90b: setSysexFilter tests");
  Serial.println("-------------------------------------------------");

  midiXparser midiParser;

  int startMem = freeMemory();
  Serial.print("START Free Memory                          : ") ; Serial.println(startMem);

  // On the fly
  midiParser.setSysExFilter(true);
  Serial.print("Free Memory after setSysExFilter(true) : ") ; Serial.println(freeMemory());

  midiParser.setSysExFilter(false);
  Serial.print("Free Memory after setSysExFilter(false)    : ") ; Serial.println(freeMemory());
  
  // Allocate
  midiParser.setSysExFilter(true,128);
  Serial.print("Free Memory after setSysExFilter(true,128) : ") ; Serial.println(freeMemory());
  // DeAllocate
  midiParser.setSysExFilter(false);
  Serial.print("Free Memory after setSysExFilter(false)    : ") ; Serial.println(freeMemory());
  
  // ReAllocate
  midiParser.setSysExFilter(true,32);
  Serial.print("Free Memory after setSysExFilter(true,32) : ") ; Serial.println(freeMemory());

  midiParser.setSysExFilter(true,64);
  Serial.print("Free Memory after setSysExFilter(true,64) : ") ; Serial.println(freeMemory());

  midiParser.setSysExFilter(false);
  Serial.print("Free Memory after setSysExFilter(false)    : ") ; Serial.println(freeMemory());

  // Change to "On the fly" mode
  midiParser.setSysExFilter(true,32);
  Serial.print("Free Memory after setSysExFilter(true,32) : ") ; Serial.println(freeMemory());

  midiParser.setSysExFilter(true);
  Serial.print("Free Memory after setSysExFilter(true) : ") ; Serial.println(freeMemory());

  midiParser.setSysExFilter(false);
  Serial.print("Free Memory after setSysExFilter(false)    : ") ; Serial.println(freeMemory());
  
  Serial.print("END - START Memory gap                     : ") ; Serial.println(startMem - freeMemory());
  
  midiParser.setSysExFilter(true,48);
  Serial.print("Test destructor - setSysExFilter(true,48) : ") ; Serial.println(freeMemory());

}


void serializer1(char * title,char * subTitle,midiXparser *midiParser,uint8_t dummy[], unsigned dummySize ) {

  Serial.println("================== START ========================");
  Serial.print(title);Serial.print(" : ");
  Serial.println(subTitle);
  Serial.println("-------------------------------------------------");

  for (int i = 0; i< dummySize ; i++ ) {

    bool msg = midiParser->parse( dummy[i] );



    if (dummy[i] < 0x10 ) Serial.print("0");
    Serial.print(dummy[i],HEX); Serial.print ("-> ");

    if(  midiParser->isByteCaptured())
          Serial.print("Byte captured      ");
    else
          Serial.print("Byte non captured  ");

     if(  midiParser->isSysExMode())
          Serial.print(" (SYSEX)");

     if(  midiParser->isSysExError())
          Serial.print(" (SYSEX ERR)");

     Serial.print(" : ");

    Serial.println(midiParser->getByte(),HEX) ;
    if ( msg ) {
         if ( midiParser->getMidiMsgType() == midiXparser::sysExMsgType ) {
            Serial.print("====> SYSEX Message received, len (getSysexMsgLen / getMidiMsgLen) =  : ");
            Serial.print(midiParser->getSysExMsgLen() );
            Serial.print(" / ");
            Serial.println(midiParser->getMidiMsgLen() );
            Serial.print("Message was (getSysex / getMidiMsg ) : " );
            for (int j = 0 ; j < midiParser->getSysExMsgLen() ; j++) {
                Serial.print(midiParser->getSysExMsg()[j],HEX);
                Serial.print(" / ");
                Serial.print(midiParser->getMidiMsg()[j],HEX);
                Serial.print(" - ");
            }
            Serial.println();

         }

         else {

          Serial.print("=======> Message received : ");
          Serial.print(midiParser->getMidiMsg()[0],HEX);
          if (midiParser->getMidiMsgLen() >=2) { Serial.print("-");Serial.print(midiParser->getMidiMsg()[1],HEX);}
          if (midiParser->getMidiMsgLen() ==3) { Serial.print("-");Serial.print(midiParser->getMidiMsg()[2],HEX);}

         }

         Serial.println();
    }

 }

 Serial.println("==================== END ========================");
}


////////////////////////////////////////////////
// SYSEX USB DEMO SIMULATION
///////////////////////////////////////////////

// Use this structure to send and receive packet to/from USB
union EVENT_t {
    uint32_t i;
    uint8_t  packet[4];
    //MIDI_EVENT_PACKET_t p;
};

#define SERIAL_INTERFACE_MAX 4

void scanMidiSysExToUsb( uint8_t cable, midiXparser* serialMidiParser ) {
  static union EVENT_t usbMidiSysExPacket[SERIAL_INTERFACE_MAX];
  static uint8_t packetLen[SERIAL_INTERFACE_MAX];
  static bool firstCall = true;
  byte readByte = serialMidiParser->getByte();

  // Initialize everything at the first call
  if (firstCall ) {
    firstCall = false;
    memset(&usbMidiSysExPacket[0],0,4*SERIAL_INTERFACE_MAX);
    memset(&packetLen[0],0,sizeof(uint8_t)*SERIAL_INTERFACE_MAX);
  }

  // Normal End of SysEx . Send the packet
  // CIN = 5/6/7  sysex ends with one/two/three bytes,
  // if ( readByte == midiXparser::eoxStatus ) {
  //     // Add the eox byte
  //     packetLen[cable]++;
  //     usbMidiSysExPacket[cable].b[ packetLen[cable] ] = readByte ;
  //     usbMidiSysExPacket[cable].p.cable = cable;
  //     usbMidiSysExPacket[cable].p.cin   = packetLen[cable] + 4;
  //     MidiUSB.writePacket(usbMidiSysExPacket[cable].i);    // Send to USB
  //     packetLen[cable] = 0; usbMidiSysExPacket[cable].i = 0;
  //     return;
  // }

  // Normal End of SysEx or : End of SysEx with error.
  // Force clean end of SYSEX as the midi usb driver
  // will not understand if we send the packet as is
  if ( readByte == midiXparser::eoxStatus || serialMidiParser->isSysExError() ) {
      // Force the eox byte in case we have a SYSEX error.
      packetLen[cable]++;
      usbMidiSysExPacket[cable].packet[ packetLen[cable] ] = midiXparser::eoxStatus;
      // CIN = 5/6/7  sysex ends with one/two/three bytes,
      usbMidiSysExPacket[cable].packet[0] = (cable << 4) + (packetLen[cable] + 4) ;
      //MidiUSB.writePacket(usbMidiSysExPacket[cable].i);    // Send to USB

      Serial.print("Packet (eox err): ");
      Serial.println( usbMidiSysExPacket[cable].i,HEX);

      packetLen[cable] = 0;
      memset(&(usbMidiSysExPacket[cable].packet[0]),0,4);
  }

  // Stop if not in sysexmode anymore here !
  // The SYSEX error could be caused by another SOX, or Midi status...
  if ( ! serialMidiParser->isSysExMode() ) return;

  // Fill USB sysex packet
  packetLen[cable]++;
  usbMidiSysExPacket[cable].packet[ packetLen[cable] ] = readByte ;

  // Packet complete ?
  if (packetLen[cable] == 3 ) {
      usbMidiSysExPacket[cable].packet[0] = (cable << 4) + 4 ; // Sysex start or continue
      //MidiUSB.writePacket(usbMidiSysExPacket[cable].i);    // Send to USB
      Serial.print("Packet (len 3): ");
      Serial.println( usbMidiSysExPacket[cable].i,HEX);

      packetLen[cable] = 0;
      memset(&(usbMidiSysExPacket[cable].packet[0]),0,4);
  }
}

void testUsbSysEx() {
  midiXparser serialMidiParser;
  serialMidiParser.setMidiChannelFilter(midiXparser::allChannel);
  serialMidiParser.setMidiMsgFilter( midiXparser::allMidiMsg );
  serialMidiParser.setSysExFilter(true,0);

  for (uint8_t d=0; d< sizeof(dummyUsb) ; d++ ) {

    if ( serialMidiParser.parse( dummyUsb[d] ) ) {
                //sendMidiSerialMsgToUsb( s, &serialMidiParser[s]);
                Serial.println("Midi MSG");
    } else

     // Check if a SYSEX msg is currently sent or terminated
     // as we proceed on the fly.
     if ( serialMidiParser.isByteCaptured() &&
          ( serialMidiParser.isSysExMode() ||
            serialMidiParser.getByte() == midiXparser::eoxStatus ||
            serialMidiParser.isSysExError()
          )
        )
     {
         // Process for eventual SYSEX unbuffered on the fly
        scanMidiSysExToUsb(1, &serialMidiParser) ;
     }

    } // for

}

void setup() {

  Serial.begin(115200);
//  testUsbSysEx();
 //test15();
  test16();
//  test10();  test11();   test12(); test13();test14();  test20();  test30();  test40();   test50();
//  test60();
  //test70();   test71();   test72() ;   test80();  test90(); test90b();

}


void loop() {

}
