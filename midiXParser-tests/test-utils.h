/*

  midXparser
  A small footprint midi parser.
  Copyright (C) 2017/2018 by The KikGen labs.

  Validation TESTS - UTILITIES

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

void header(char const *title, char const * subTitle) ;
void footer(bool pass);
void serializer(midiXparser *midiParser,uint8_t dummy[], unsigned dummySize ) ;
bool midiCounterTests(
                   midiXparser *midiParser,
                   uint8_t * dummy,
                   const unsigned dummySize,
                   const unsigned expMsgCount,
                   const unsigned expCapturedBytes,
                   const unsigned expSysExMsgCount,
                   const unsigned expSysExErrorCount,
                   const unsigned expSysExLen
);
bool midiCheckContent(midiXparser *midiParser,uint8_t * dummy,const unsigned dummySize);

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
        if (  midiParser->getMidiMsgType() == midiXparser::sysExMsgTypeMsk ) {
              sysExMsgCount++;
              sysExLen += midiParser->getSysExMsgLen();
        }
     }
     if ( midiParser->isSysExError() ) sysExErrorCount++;
     if ( midiParser->isByteCaptured() ) capturedBytesCount++ ;
     else noCapturedBytesCount++;
   }
printf("msgcount %d  msgcountsys %d  sysexlen  %d syx err %d  cap %d  ",msgCount, sysExMsgCount,sysExLen,sysExErrorCount,capturedBytesCount);

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
    if ( midiParser->isSysExError() ) break;
  }

  if ( ! midiParser->isSysExError() && !m ) return false;


  for (unsigned i = 0; i < midiParser->getMidiMsgLen() ; i++ ) {
    if (midiParser->getMidiMsg()[i] !=  msg[i] ) {
    //  printf("%2x != %2x\n",midiParser->getMidiMsg()[i],msg[i]);
      return false;
    }
  }

  return true;

}

void header(char const *title, char const * subTitle) {
  printf("----------------------------------------------------------------------------\n");
  printf("TEST %s : %s", title, subTitle);
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
    if (msg) msgcount++ ;

    printf(" %2x ",dummy[i]);

    if ( midiParser->isByteCaptured()) {
        printf("c");
        capturedbytescount++ ;
    } else {
        printf(" ");
        nocapturedbytescount++;
    }

    if(  midiParser->isSysExMode()) printf("x");else printf(" ");

    if( midiParser->isSysExError()) {
        printf("e");
        sysexerrorcount++;
        }
    else printf(" ");

    printf(" |");

    if ( ++nl >= cr ) { printf("\n"); nl = 0 ; }
    if (msg || midiParser->isSysExError() ) {
          if ( nl < cr ) printf("\n");
          nl = 0;

         if ( midiParser->wasSysExMode() ) {
            if ( midiParser->isSysExError() ) printf("** ERROR **" );
            else { sysexmsgcount++; sysexlen += midiParser->getSysExMsgLen(); }

            printf("SYSEX MSG (len = %d) = [", midiParser->getSysExMsgLen() );

            if ( !midiParser->isSysExOnTheFly() ) {
              for (unsigned j = 0 ; j < midiParser->getSysExMsgLen() ; j++) {
                  printf("%2x," , midiParser->getSysExMsg()[j]);
              }
              printf("]\n");
           } else printf("on the fly - unbuffered]\n");

         }
         if ( msg && midiParser->getMidiMsgType() != midiXparser::sysExMsgTypeMsk ) {
          printf("MIDI MSG = [%2X",midiParser->getMidiMsg()[0]);
          if (midiParser->getMidiMsgLen() >=2) { printf(",%2x",midiParser->getMidiMsg()[1]);}
          if (midiParser->getMidiMsgLen() ==3) { printf(",%2x",midiParser->getMidiMsg()[2]);}
          printf("]\n");
         }
         printf("\n");
    }
 }
 printf("\nStream length                   : %d\n",dummySize);
 printf("Number of midi messages found   : %d\n",msgcount);
 printf("Number of valid SysEx msg found : %d\n",sysexmsgcount);
 printf("Number of sysex errors          : %d\n",sysexerrorcount);
 printf("Total length of sysex messages  : %d\n",sysexlen);
 printf("Number of bytes captured        : %d\n",capturedbytescount);
 printf("Number of bytes non captured    : %d\n",nocapturedbytescount);

 printf("--------------------------- END OF DUMP ------------------------------------\n");
}
