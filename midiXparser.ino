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
  
  0xF0,0x77,0x77,0x78,0xF8,0x40,0x41,0xF7,
  0xF0,0x41,0x77,0x78,0xF8,0x40,0x41,0xF7,
  0xF0,0x77,0x77,0x77,0xF8,0x0,0x1,0x2,0xF8,0X44,0xF7,
  0xF0,0x77,0x77,0x77,0xF8,0x0,0x1,0x2,0xF8,
  0x80,0x50,0x40,
  0xF0,0x77,0x77,0x77,0xF8,0x40,0x41,0x42,0x43,0xF8,0X44,0xF7,
  0xF0,0x77,0x77,0x77,0x0B,'H','E','L','L','O',0xF7} ;

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
  serializer1("TEST 1A","default ",&midiParser,dummyMsg1,sizeof( dummyMsg1) );
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



void test20() {
  midiXparser midiParser;
  midiParser.setChannelVoiceMsgFilter(midiXparser::noteOffMsk |midiXparser::pitchBendMsk );  
  midiParser.setMidiChannelFilter(2);
  serializer1("TEST 1C","Note off channel 3 Pitch Bend",&midiParser,dummyMsg1,sizeof( dummyMsg1) );  
}

void test30() {
  midiXparser midiParser;
  midiParser.setChannelVoiceMsgFilter(midiXparser::noteOnMsk |midiXparser::controlChangeMsk );  
  midiParser.setMidiChannelFilter(1);
  serializer1("TEST 1D","Note on channel 2 CChange ",&midiParser,dummyMsg1,sizeof( dummyMsg1) );  
}

void test40() {
  midiXparser midiParser;
  midiParser.setChannelVoiceMsgFilter(midiXparser::noteOnMsk  );  
  midiParser.setMidiChannelFilter(2);
  serializer1("TEST 1E","Note on channel 3 with real time events mixed",&midiParser,dummyMsg2,sizeof( dummyMsg2) );  
}

void test50() {
  midiXparser midiParser;
  midiParser.setRealTimeMsgFilter(midiXparser::startMsk | midiXparser::stopMsk );
  midiParser.setChannelVoiceMsgFilter(midiXparser::noChannelVoiceMsgMsk );  
  midiParser.setMidiChannelFilter(1);
  serializer1("TEST 1F","Realtime Start Stop mixed with notes not filtered - channel 2",&midiParser,dummyMsg2,sizeof( dummyMsg2) );  
}

void test60() {
  midiXparser midiParser;
  midiParser.setRealTimeMsgFilter(midiXparser::startMsk | midiXparser::stopMsk );
  midiParser.setChannelVoiceMsgFilter(midiXparser::noteOffMsk | midiXparser::noteOnMsk );  
  midiParser.setSystemCommonMsgFilter(midiXparser::tuneRequestMsk | midiXparser::songPosPointerMsk);
  midiParser.setMidiChannelFilter(1);
  serializer1("TEST 1G","Start Stop with Tune Request / Song Pos /note off ch 2 / broken note on",&midiParser,dummyMsg2,sizeof( dummyMsg2) );  
}

void test70() {
  midiXparser midiParser;
  midiParser.setSysExFilter(true,64);
  serializer1("TEST 1H","SYSEX",&midiParser,dummyMsg3,sizeof( dummyMsg3) );  
}

void test80() {
  midiXparser midiParser;
  midiParser.setSysExFilter(true,6);
  serializer1("TEST 1I","SYSEX Size = 6",&midiParser,dummyMsg3,sizeof( dummyMsg3) );  
}


void test90() {
  Serial.println("TEST90 : setSysexFilter tests");
  Serial.println("-------------------------------------------------");
 
  
  
  midiXparser midiParser;
  
  int startMem = freeMemory();
  Serial.print("START Free Memory                          : ") ; Serial.println(startMem);
  
  midiParser.setSysExFilter(true,128);
  Serial.print("Free Memory after setSysExFilter(true,128) : ") ; Serial.println(freeMemory());
  
  midiParser.setSysExFilter(false);
  Serial.print("Free Memory after setSysExFilter(false)    : ") ; Serial.println(freeMemory());

  midiParser.setSysExFilter(true,32);
  Serial.print("Free Memory after setSysExFilter(true,32) : ") ; Serial.println(freeMemory()); 
  
  midiParser.setSysExFilter(true,64);
  Serial.print("Free Memory after setSysExFilter(true,64) : ") ; Serial.println(freeMemory()); 
  
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

    Serial.print(dummy[i],HEX); Serial.print ("-> ");
    
    if(  midiParser->isByteCaptured()) 
          Serial.print("Byte captured     : ");
    else  
          Serial.print("Byte non captured : ");
          
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


void setup() {
   
  Serial.begin(115200);  
  
  test10();  test11();   test12(); test13();test14(); test20();  test30();  test40();   test50(); 
  test60();   test70();  test80();  
  
  Serial.println("================== START ========================");
  Serial.println("SYSEX : Dynamic allocations tests");
  Serial.println("-------------------------------------------------");
  int startMem = freeMemory();
  Serial.print("Free Memory before calling test90          : ") ; Serial.println(startMem);
  Serial.println("-------------------------------------------------");
  test90(); 
  Serial.println("-------------------------------------------------");
  Serial.print("Free Memory gap after calling test90       : ") ; Serial.println(startMem-freeMemory());
  Serial.println("-------------------------------------------------");
}


void loop() {
 
}
