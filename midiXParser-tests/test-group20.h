void test20a() {

   //            ****************************************************
   header("20a","Program change check content                        ");

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

   //            ****************************************************
   header("20b","SYSEX check content                                 ");

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

void test21b() {

   //            ****************************************************
   header("21b","SYSEX Error check content                           ");

   uint8_t dummy[] = {0xF0,0x77,0x77,0x77,1,2,3,4,0X90,1,2};
   unsigned dummySize = sizeof(dummy);
   bool pass=false;

   midiXparser midiParser;
   midiParser.setSysExFilter(true,64);

   pass = midiCounterTests(&midiParser,dummy,dummySize,
     0 ,  // expMsgCount,
     8,  // expCapturedBytes,
     0 , // expSysExMsgCount,
     1 , // expSysExErrorCount,
     0 );// expSysExLen

   pass = pass && midiCheckContent(&midiParser,dummy,dummySize);

   footer(pass);
   if (!pass) serializer(&midiParser,dummy,dummySize );
}

void test20c() {

   //            ****************************************************
   header("20c","SYSEX buffer overflow                               ");

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

   //            ****************************************************
   header("20d","SYSEX - Buffered all msg mixed RealTIME             ");

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

   //            ****************************************************
   header("20e","Top lev. filter-sysExMsgTypeMsk -  Check content    ");

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

void test20f() {

   //            ****************************************************
   header("20f","SYSEX - Special case F0 F7                          ");

   uint8_t dummy[] = {0xF0, 0XF7};
   unsigned dummySize = sizeof(dummy);
   bool pass=false;

   midiXparser midiParser;
   midiParser.setMidiMsgFilter( midiXparser::allMsgTypeMsk );
   midiParser.setSysExFilter(true,64);

   pass = midiCounterTests(&midiParser,dummy,dummySize,
     1 ,  // expMsgCount,
     2, // expCapturedBytes,
     1 , // expSysExMsgCount,
     0 , // expSysExErrorCount,
     0 );// expSysExLen

   pass = pass && midiCheckContent(&midiParser,dummy,dummySize);

   footer(pass);
   if (!pass) serializer(&midiParser,dummy,dummySize );
}

void test20g() {

   //            ****************************************************
   header("20g","SYSEX - Special case F0 41 F7                       ");

   uint8_t dummy[] = {0xF0, 0x41,0XF7};
   unsigned dummySize = sizeof(dummy);
   bool pass=false;

   midiXparser midiParser;
   midiParser.setMidiMsgFilter( midiXparser::allMsgTypeMsk );
   midiParser.setSysExFilter(true,64);

   pass = midiCounterTests(&midiParser,dummy,dummySize,
     1 ,  // expMsgCount,
     3 , // expCapturedBytes,
     1 , // expSysExMsgCount,
     0 , // expSysExErrorCount,
     1 );// expSysExLen

   pass = pass && midiCheckContent(&midiParser,dummy,dummySize);

   footer(pass);
   if (!pass) serializer(&midiParser,dummy,dummySize );
}

void test20h() {

   //            ****************************************************
   header("20h","SYSEX Error with one byte not real time on the fly  ");

   uint8_t dummy[] = {0xF0, 1,2,3,4,5,0XF8,0xF6,0xF8 };
   unsigned dummySize = sizeof(dummy);
   bool pass=false;

   midiXparser midiParser;
   midiParser.setMidiMsgFilter( midiXparser::allMsgTypeMsk );
   midiParser.setSysExFilter(true,0);

   pass = midiCounterTests(&midiParser,dummy,dummySize,
     3 ,  // expMsgCount,
     9 , // expCapturedBytes,
     0 , // expSysExMsgCount,
     1 , // expSysExErrorCount,
     0 );// expSysExLen

   //pass = pass && midiCheckContent(&midiParser,dummy,dummySize);

   footer(pass);
   if (!pass) serializer(&midiParser,dummy,dummySize );
}

void test20i() {

  //            ****************************************************
  header("20i","Parse repeated SYSEX Error - notes on/off on the fly");

  uint8_t dummy[] = {0xF0, 1,2,3,4,5,
                   0xF0, 1,2,3,4,5,0xF7,  // error 1 - msg 1
                   0xF7,                  // error 2
                   0xF0, 1,2,3,4,5,       // error 3
                   0x90,0x3A,0x64,0x80,0x3A,0x40, //
                   0x90,0x3A,0x64,0x80,0x3A,0x40, // msg 5
                   0xF0,                          // error 4
                   0x90,0x3A,0x64,0x80,0x3A,0x40, //
                   0x90,0x3A,0x64,0x80,0x3A,0x40,
                   0x90,0x3A,0x64,0x80,0x3A,0x40,
                   0x90,0x3A,0x64,0x80,0x3A,0x40, // msg 13
                   0xF7,0xF0, 1,2,3,4,5,          // error 5
                   0x90,0x3A,0x64,0x80,0x3A,0x40, // error 6 - msg 15
    };
  unsigned dummySize = sizeof(dummy);
  bool pass=false;

  midiXparser midiParser;
  midiParser.setMidiMsgFilter( midiXparser::allMsgTypeMsk );

  pass = midiCounterTests(&midiParser,dummy,dummySize,
    15 ,  // expMsgCount,
    68,  // expCapturedBytes,
    1  , // expSysExMsgCount,
    6 ,  // expSysExErrorCount,
    5 ); // expSysExLen

  //pass = pass && midiCheckContent(&midiParser,dummy,dummySize);

  footer(pass);
  if (!pass) serializer(&midiParser,dummy,dummySize );
}
