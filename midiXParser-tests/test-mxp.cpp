/*

  midXparser
  A small footprint midi parser.
  Copyright (C) 2017/2018 by The KikGen labs.

  Validation TESTS

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

#include <stdio.h>
#include <stdint.h>

#include "../midiXparser.h"

void header(char const *title, char const * subTitle) ;
void footer(bool pass);
void serializer(midiXparser *midiParser,uint8_t dummy[], unsigned dummySize ) ;


static  uint8_t dummyMsg1[] = {
  0XFA,
  0x90,0x3A,0x77,
  0x80,0x3A,0XF8,0x77,
  0x91,0x3B,0x64,
  0x81,0x3B,0x64,
  0x92,0x3C,0x70,
  0xFB,0xF6,
  0x82,0x3C,0x70,
  0xA0,0XF8,0x64,0x7F,
  0xB1,0x64,0x7F,
  0xC2,0x18,
  0xC2,0x20,
  0x7F, // Running status
  0xFC,
  0xD0,0x64,0x7F,
  0xE2,0x64,0x7F,
  0x41,0x42, // Running status
  0xFE
}  ;

 static  uint8_t dummyMsg2[] = {
   0xF0,0x77,0x77,0x78,0xF8,0x40,0x41,0x42,0x43,0xF7,
   0xF0,0x41,0x77,0x78,0xF8,0x40,0xF7,
   0xF0,0x77,0x77,0x77,0xF8,0x0,0x1,0x2,0xF8,0X44,0xF7,
   0xF0,0x77,0x77,0x77,0xF8,0x0,0x1,0x2,0xF8,
   0x80,0x50,0x40,
   0xF0,0x77,0x77,0x77,0xF8,0x40,0x41,0x42,0x43,0xF8,0X44,0xF7,
   0xF0,0x77,0x77,0xF0,0x0B,'H','E','L','L','O',0xF7} ;

bool midiCounterTests(
                    midiXparser *midiParser,
                    uint8_t * dummy,
                    const unsigned dummySize,
                    const unsigned expMsgCount,
                    const unsigned expCapturedBytes,
                    const unsigned expSysExMsgCount,
                    const unsigned expSysExErrorCount,
                    const unsigned expSysExLen
) {

   bool pass=false;
   unsigned msgCount = 0;
   unsigned capturedBytesCount = 0;
   unsigned noCapturedBytesCount = 0;
   unsigned sysExMsgCount = 0;
   unsigned sysExErrorCount = 0;
   unsigned sysExLen = 0;

   for (unsigned i = 0; i< dummySize ; i++ ) {
     if ( midiParser->parse( dummy[i] ) ) {
        msgCount++ ;
        if ( midiParser->getMidiMsgType() == midiXparser::sysExMsgTypeMsk ) {
          sysExMsgCount++;
          sysExLen += midiParser->getSysExMsgLen();
        } else
        if( midiParser->isSysExError()) sysExErrorCount++;

     }
     if ( midiParser->isByteCaptured() ) capturedBytesCount++ ;
     else noCapturedBytesCount++;
   }

   pass = (  msgCount == expMsgCount &&
             capturedBytesCount == expCapturedBytes &&
             sysExMsgCount == expSysExMsgCount &&
             sysExLen == expSysExLen &&
             dummySize == ( capturedBytesCount + noCapturedBytesCount)
          );
   return pass;
}


bool midiCheckContent(midiXparser *midiParser,uint8_t * dummy,const unsigned dummySize) {
  bool m = false;
  unsigned j = 0;
  uint8_t msg[64];

  for (unsigned i = 0; i< dummySize ; i++ ) {
    m = midiParser->parse( dummy[i] ) ;
    if ( midiParser->isByteCaptured() ) {
        if ( !midiParser->isSysExMode() ||
              (midiParser->isSysExMode() && (dummy[i] != 0xF0 && dummy[i] != 0xF7) ) )
              msg[j++] = midiParser->getByte();
    }
    if ( m ) break ;
  }

  if (!m) return false;


  for (unsigned i = 0; i < midiParser->getMidiMsgLen() ; i++ ) {
    if (midiParser->getMidiMsg()[i] !=  msg[i] ) {
      //printf("%2x != %2x\n",midiParser->getMidiMsg()[i],msg[i]);
      return false;
    }
  }

  return true;

}

void test10a() {
  //            **********************************************
  header("10a","Default settings - no filters & no channel    ");

  uint8_t * dummy = dummyMsg1;
  unsigned dummySize = sizeof(dummyMsg1);
  bool pass=false;

  midiXparser midiParser;

  pass = midiCounterTests(&midiParser,dummy,dummySize,
    0 , // expMsgCount,
    0 , // expCapturedBytes,
    0 , // expSysExMsgCount,
    0 , // expSysExErrorCount,
    0 ); // expSysExLen

  footer(pass);
  if (!pass) serializer(&midiParser,dummy,dummySize );
}

void test10b() {

   //            **********************************************
   header("10b","All msg - All channels                        ");

   uint8_t * dummy = dummyMsg1;
   unsigned dummySize = sizeof(dummyMsg1);
   bool pass=false;

   midiXparser midiParser;
   midiParser.setMidiChannelFilter(midiXparser::allChannel);
   midiParser.setMidiMsgFilter( midiXparser::allMsgTypeMsk );

   pass = midiCounterTests(&midiParser,dummy,dummySize,
     22 , // expMsgCount,
     dummySize, // expCapturedBytes,
     0 , // expSysExMsgCount,
     0 , // expSysExErrorCount,
     0 ); // expSysExLen

   footer(pass);
   if (!pass) serializer(&midiParser,dummy,dummySize );
}

void test10c() {

   //            **********************************************
   header("10c","Filter = Note on/off default all channels     ");

   uint8_t * dummy = dummyMsg1;
   unsigned dummySize = sizeof(dummyMsg1);
   bool pass=false;

   midiXparser midiParser;
   midiParser.setChannelVoiceMsgFilter(midiXparser::noteOffMsk | midiXparser::noteOnMsk );
   midiParser.setMidiChannelFilter(midiXparser::allChannel  );

   pass = midiCounterTests(&midiParser,dummy,dummySize,
     6 , // expMsgCount,
     18 , // expCapturedBytes,
     0 , // expSysExMsgCount,
     0 , // expSysExErrorCount,
     0 ); // expSysExLen

   footer(pass);
   if (!pass) serializer(&midiParser,dummy,dummySize );
}


void test10d() {

   //            **********************************************
   header("10d","filter NoMidiMsg but All channels             ");

   uint8_t * dummy = dummyMsg1;
   unsigned dummySize = sizeof(dummyMsg1);
   bool pass=false;

   midiXparser midiParser;
   midiParser.setMidiChannelFilter(midiXparser::allChannel);
   midiParser.setMidiMsgFilter( midiXparser::noneMsgTypeMsk );

   pass = midiCounterTests(&midiParser,dummy,dummySize,
     0 ,  // expMsgCount,
     0 , // expCapturedBytes,
     0 ,  // expSysExMsgCount,
     0 ,  // expSysExErrorCount,
     0 ); // expSysExLen

   footer(pass);
   if (!pass) serializer(&midiParser,dummy,dummySize );
}

void test10e() {

   //            **********************************************
   header("10e","Filter all messages on channel 2              ");

   uint8_t * dummy = dummyMsg1;
   unsigned dummySize = sizeof(dummyMsg1);
   bool pass=false;

   midiXparser midiParser;
   midiParser.setMidiChannelFilter(1);
   midiParser.setMidiMsgFilter( midiXparser::allMsgTypeMsk );

   pass = midiCounterTests(&midiParser,dummy,dummySize,
     10 ,  // expMsgCount,
     16 , // expCapturedBytes,
     0 ,  // expSysExMsgCount,
     0 ,  // expSysExErrorCount,
     0 ); // expSysExLen

   footer(pass);
   if (!pass) serializer(&midiParser,dummy,dummySize );
}

void test10f() {

   //            **********************************************
   header("10f","Filter all messages on channel 3              ");

   uint8_t * dummy = dummyMsg1;
   unsigned dummySize = sizeof(dummyMsg1);
   bool pass=false;

   midiXparser midiParser;
   midiParser.setMidiChannelFilter(2);
   midiParser.setMidiMsgFilter( midiXparser::allMsgTypeMsk );

   pass = midiCounterTests(&midiParser,dummy,dummySize,
     14 ,  // expMsgCount,
     23 , // expCapturedBytes,
     0 ,  // expSysExMsgCount,
     0 ,  // expSysExErrorCount,
     0 ); // expSysExLen

   footer(pass);
   if (!pass) serializer(&midiParser,dummy,dummySize );
}


void test10g() {

   //            **********************************************
   header("10g","Filter all msg on channel 3 - no Real Time    ");

   uint8_t * dummy = dummyMsg1;
   unsigned dummySize = sizeof(dummyMsg1);
   bool pass=false;

   midiXparser midiParser;
   midiParser.setMidiMsgFilter( midiXparser::allMsgTypeMsk );
   midiParser.setMidiChannelFilter(2);
   midiParser.setRealTimeMsgFilter(midiXparser::noRealTimeMsgMsk  );

   pass = midiCounterTests(&midiParser,dummy,dummySize,
     8 ,  // expMsgCount,
     17 , // expCapturedBytes,
     0 ,  // expSysExMsgCount,
     0 ,  // expSysExErrorCount,
     0 ); // expSysExLen

   footer(pass);
   if (!pass) serializer(&midiParser,dummy,dummySize );
}

void test10h() {

   //            **********************************************
   header("10h","Filter all Real Time                          ");

   uint8_t * dummy = dummyMsg1;
   unsigned dummySize = sizeof(dummyMsg1);
   bool pass=false;

   midiXparser midiParser;
   midiParser.setRealTimeMsgFilter(midiXparser::allRealTimeMsgMsk  );

   pass = midiCounterTests(&midiParser,dummy,dummySize,
     6 ,  // expMsgCount,
     6 , // expCapturedBytes,
     0 ,  // expSysExMsgCount,
     0 ,  // expSysExErrorCount,
     0 ); // expSysExLen

   footer(pass);
   if (!pass) serializer(&midiParser,dummy,dummySize );
}

void test10i() {

   //            **********************************************
   header("10i","Filter Real Time Timing Clock                 ");

   uint8_t * dummy = dummyMsg1;
   unsigned dummySize = sizeof(dummyMsg1);
   bool pass=false;

   midiXparser midiParser;
   midiParser.setRealTimeMsgFilter(midiXparser::timingClockMsk  );

   pass = midiCounterTests(&midiParser,dummy,dummySize,
     2 ,  // expMsgCount,
     2 , // expCapturedBytes,
     0 ,  // expSysExMsgCount,
     0 ,  // expSysExErrorCount,
     0 ); // expSysExLen

   footer(pass);
   if (!pass) serializer(&midiParser,dummy,dummySize );
}

void test10j() {

   //            **********************************************
   header("10j","Filter Real Time Start Stop Continue          ");

   uint8_t * dummy = dummyMsg1;
   unsigned dummySize = sizeof(dummyMsg1);
   bool pass=false;

   midiXparser midiParser;
   midiParser.setRealTimeMsgFilter(midiXparser::startMsk|midiXparser::continueMsk|midiXparser::stopMsk);

   pass = midiCounterTests(&midiParser,dummy,dummySize,
     3 ,  // expMsgCount,
     3 , // expCapturedBytes,
     0 ,  // expSysExMsgCount,
     0 ,  // expSysExErrorCount,
     0 ); // expSysExLen

   footer(pass);
   if (!pass) serializer(&midiParser,dummy,dummySize );
}

void test10k() {

   //            **********************************************
   header("10k","Filter program change & channel pressure      ");

   uint8_t * dummy = dummyMsg1;
   unsigned dummySize = sizeof(dummyMsg1);
   bool pass=false;

   midiXparser midiParser;
   midiParser.setChannelVoiceMsgFilter(midiXparser::programChangeMsk|midiXparser::channelPressureMsk);

   pass = midiCounterTests(&midiParser,dummy,dummySize,
     5 ,  // expMsgCount,
     8 , // expCapturedBytes,
     0 ,  // expSysExMsgCount,
     0 ,  // expSysExErrorCount,
     0 ); // expSysExLen

   footer(pass);
   if (!pass) serializer(&midiParser,dummy,dummySize );
}

void test10l() {

   //            **********************************************
   header("10l","Filter ctl chg - pitch bd with running status ");

   uint8_t * dummy = dummyMsg1;
   unsigned dummySize = sizeof(dummyMsg1);
   bool pass=false;

   midiXparser midiParser;
   midiParser.setChannelVoiceMsgFilter(midiXparser::controlChangeMsk|midiXparser::pitchBendMsk);

   pass = midiCounterTests(&midiParser,dummy,dummySize,
     3 ,  // expMsgCount,
     8 , // expCapturedBytes,
     0 ,  // expSysExMsgCount,
     0 ,  // expSysExErrorCount,
     0 ); // expSysExLen

   footer(pass);
   if (!pass) serializer(&midiParser,dummy,dummySize );
}

void test10m() {
   //            **********************************************
   header("10m","Top level filter - channelVoiceMsgTypeMsk Ch 1");

   uint8_t * dummy = dummyMsg1;
   unsigned dummySize = sizeof(dummyMsg1);
   bool pass=false;

   midiXparser midiParser;
   midiParser.setMidiChannelFilter(0);
   midiParser.setMidiMsgFilter( midiXparser::channelVoiceMsgTypeMsk );

   pass = midiCounterTests(&midiParser,dummy,dummySize,
     5 , // expMsgCount,
     12, // expCapturedBytes,
     0 , // expSysExMsgCount,
     0 , // expSysExErrorCount,
     0 ); // expSysExLen

   footer(pass);
   if (!pass) serializer(&midiParser,dummy,dummySize );
}

void test10n() {
   //            **********************************************
   header("10n","Top level filter-systemCommonMsgTypeMsk All Ch");

   uint8_t * dummy = dummyMsg1;
   unsigned dummySize = sizeof(dummyMsg1);
   bool pass=false;

   midiXparser midiParser;
   midiParser.setMidiChannelFilter(midiXparser::allChannel);
   midiParser.setMidiMsgFilter( midiXparser::systemCommonMsgTypeMsk );

   pass = midiCounterTests(&midiParser,dummy,dummySize,
     1 , // expMsgCount,
     1, // expCapturedBytes,
     0 , // expSysExMsgCount,
     0 , // expSysExErrorCount,
     0 ); // expSysExLen

   footer(pass);
   if (!pass) serializer(&midiParser,dummy,dummySize );
}

void test10o() {
      //            **********************************************
      header("10o","Top level filter-realTimeMsgTypeMsk     All Ch");

      uint8_t * dummy = dummyMsg1;
      unsigned dummySize = sizeof(dummyMsg1);
      bool pass=false;

      midiXparser midiParser;
      midiParser.setMidiChannelFilter(midiXparser::allChannel);
      midiParser.setMidiMsgFilter( midiXparser::realTimeMsgTypeMsk );

      pass = midiCounterTests(&midiParser,dummy,dummySize,
        6 , // expMsgCount,
        6, // expCapturedBytes,
        0 , // expSysExMsgCount,
        0 , // expSysExErrorCount,
        0 ); // expSysExLen

      footer(pass);
      if (!pass) serializer(&midiParser,dummy,dummySize );
}

void test10p() {
      //            **********************************************
      header("10p","Top level filter-sysExMsgTypeMsk        All Ch");

      uint8_t * dummy = dummyMsg2;
      unsigned dummySize = sizeof(dummyMsg2);
      bool pass=false;

      midiXparser midiParser;
      midiParser.setMidiChannelFilter(midiXparser::allChannel);
      midiParser.setMidiMsgFilter( midiXparser::sysExMsgTypeMsk );

      pass = midiCounterTests(&midiParser,dummy,dummySize,
        0 , // expMsgCount,
        52, // expCapturedBytes,
        0 , // expSysExMsgCount,
        2 , // expSysExErrorCount,
        0 ); // expSysExLen

      footer(pass);
      if (!pass) serializer(&midiParser,dummy,dummySize );
}


void test20a() {

   //            **********************************************
   header("20a","Program change check content                  ");

   uint8_t dummy[] = {0xB1,0x64,0x7F,0xC2,0x64};
   unsigned dummySize = sizeof(dummy);
   bool pass=false;

   midiXparser midiParser;
   midiParser.setChannelVoiceMsgFilter(midiXparser::programChangeMsk);

   pass = midiCounterTests(&midiParser,dummy,dummySize,
     1 ,  // expMsgCount,
     2 , // expCapturedBytes,
     0 , // expSysExMsgCount,
     0 , // expSysExErrorCount,
     0 );// expSysExLen

   pass = pass && midiCheckContent(&midiParser,dummy,dummySize);

   footer(pass);
   if (!pass) serializer(&midiParser,dummy,dummySize );
}

void test20b() {

   //            **********************************************
   header("20b","SYSEX check content                           ");

   uint8_t *dummy = dummyMsg2;
   unsigned dummySize = sizeof(dummyMsg2);
   bool pass=false;

   midiXparser midiParser;
   midiParser.setSysExFilter(true,64);

   pass = midiCounterTests(&midiParser,dummy,dummySize,
     5 ,  // expMsgCount,
     52, // expCapturedBytes,
     5 , // expSysExMsgCount,
     2 , // expSysExErrorCount,
     32 );// expSysExLen

   pass = pass && midiCheckContent(&midiParser,dummy,dummySize);

   footer(pass);
   if (!pass) serializer(&midiParser,dummy,dummySize );
}

void test20c() {

   //            **********************************************
   header("20c","SYSEX buffer overflow                         ");

   uint8_t dummy[] = {0xF0,0x77,0x77,0x77,1,2,3,4,5,6,7,8,0xF7};
   unsigned dummySize = sizeof(dummy);
   bool pass=false;

   midiXparser midiParser;
   midiParser.setSysExFilter(true,6);

   pass = midiCounterTests(&midiParser,dummy,dummySize,
     0 ,  // expMsgCount,
     7, // expCapturedBytes,
     0 , // expSysExMsgCount,
     1 , // expSysExErrorCount,
     0 );// expSysExLen

   footer(pass);
   if (!pass) serializer(&midiParser,dummy,dummySize );
}
void test20d() {

   //            **********************************************
   header("20d","SYSEX - Buffered all msg mixed RealTIME       ");

   uint8_t *dummy = dummyMsg2;
   unsigned dummySize = sizeof(dummyMsg2);
   bool pass=false;

   midiXparser midiParser;
   midiParser.setMidiMsgFilter( midiXparser::allMsgTypeMsk );
   midiParser.setSysExFilter(true,64);

   pass = midiCounterTests(&midiParser,dummy,dummySize,
     14 ,  // expMsgCount,
     63, // expCapturedBytes,
     5 , // expSysExMsgCount,
     2 , // expSysExErrorCount,
     32 );// expSysExLen

   //pass = pass && midiCheckContent(&midiParser,dummy,dummySize);

   footer(pass);
   if (!pass) serializer(&midiParser,dummy,dummySize );
}

void test20e() {

   //            **********************************************
   header("20e","Top lev. filter-sysExMsgTypeMsk -Check content");

   uint8_t *dummy = dummyMsg2;
   unsigned dummySize = sizeof(dummyMsg2);
   bool pass=false;

   midiXparser midiParser;
   midiParser.setMidiChannelFilter(midiXparser::allChannel);
   midiParser.setMidiMsgFilter( midiXparser::sysExMsgTypeMsk );
   midiParser.setSysExFilter(true,64);

   pass = midiCounterTests(&midiParser,dummy,dummySize,
     5 ,  // expMsgCount,
     52, // expCapturedBytes,
     5 , // expSysExMsgCount,
     2 , // expSysExErrorCount,
     32 );// expSysExLen

   footer(pass);
   if (!pass) serializer(&midiParser,dummy,dummySize );
}


void header(char const *title, char const * subTitle) {
  printf("----------------------------------------------------------------------------\n");
  printf("START TEST %s : %s", title, subTitle);
}

void footer(bool pass) {
  if (pass)
    printf("     PASS    \n");
  else
    printf(" *** FAIL ***\n");
  printf("----------------------------------------------------------------------------\n");
}


void serializer(midiXparser *midiParser,uint8_t dummy[], unsigned dummySize ) {

  const int cr=6;
  int nl =0;
  unsigned capturedbytescount = 0;
  unsigned nocapturedbytescount = 0;
  unsigned msgcount = 0;

  unsigned sysexlen = 0;
  unsigned sysexmsgcount = 0;
  unsigned sysexerrorcount = 0;

  for (unsigned i = 0; i< dummySize ; i++ ) {

    bool msg = midiParser->parse( dummy[i] );

    printf(" %2x ",dummy[i]);

    if ( midiParser->isByteCaptured()) printf("c"); else printf(" ");

    if(  midiParser->isSysExMode()) printf("x");else printf(" ");

    if( midiParser->isSysExError()) {
        printf("e");
        sysexerrorcount++;
      } else printf(" ");

    printf(" |");

    if ( ++nl >= cr ) { printf("\n"); nl = 0 ; }

    if ( midiParser->isByteCaptured() ) capturedbytescount++ ;
    else nocapturedbytescount++;

    if ( msg ) {
         msgcount++ ;
         if ( nl < cr ) printf("\n");
         nl = 0;

         if ( midiParser->getMidiMsgType() == midiXparser::sysExMsgTypeMsk ) {
           sysexmsgcount++;
           sysexlen += midiParser->getSysExMsgLen();

            printf("SYSEX MSG (len = %d) = [", midiParser->getSysExMsgLen() );
            for (unsigned j = 0 ; j < midiParser->getSysExMsgLen() ; j++) {
                printf("%2x," , midiParser->getSysExMsg()[j]);
            }
            printf("]\n");
         }

         else {

          printf("MIDI MSG = [%2X",midiParser->getMidiMsg()[0]);
          if (midiParser->getMidiMsgLen() >=2) { printf(",%2x",midiParser->getMidiMsg()[1]);}
          if (midiParser->getMidiMsgLen() ==3) { printf(",%2x",midiParser->getMidiMsg()[2]);}
          printf("]\n");
         }

         printf("\n");
    }


 }

 printf("\nStream length                  : %d\n",dummySize);
 printf("Number of midi messages found  : %d\n",msgcount);
 printf("Number of SysEx messages found : %d\n",sysexmsgcount);
 printf("Number of sysex errors         : %d\n",sysexerrorcount);
 printf("Total length of sysex messages : %d\n",sysexlen);
 printf("Number of bytes captured       : %d\n",capturedbytescount);
 printf("Number of bytes non captured   : %d\n",nocapturedbytescount);

 printf("--------------------------- END OF DUMP ------------------------------------\n");
}


int main() {

printf("\n============================================================================\n");
printf("midiXparser class TESTS\n");
printf("============================================================================\n\n");

test10a(); test10b(); test10c(); test10d(); test10e(); test10f(); test10g();
test10h(); test10i(); test10j(); test10k(); test10l(); test10m(); test10n();
test10o(); test10p();

test20a(); test20b(); test20c(); test20d(); test20e();

printf("\n======================== END OF TESTS ======================================\n\n");
}
