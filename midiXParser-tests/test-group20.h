void test20a() {

   //            ****************************************************
   header("20a","Program change check content                        ");

   uint8_t dummy[] = {0xB1,0x62,0x7F,0xC2,0x64,0xC2,0x24,0xB1,0x18,0x7F};
   unsigned dummySize = sizeof(dummy);
   bool pass=false;

   midiXparser midiParser;
    midiParser.setMidiMsgFilter( midiXparser::channelVoiceMsgTypeMsk );

   pass = midiCounterTests(&midiParser,dummy,dummySize,
     4 ,  // expMsgCount,
     10 , // expCapturedBytes,
     0 , // expSysExMsgCount,
     0 , // expSysExErrorCount,
     0 );// expSysExLen

   pass = pass && midiCheckContent(&midiParser,dummy,dummySize);

   footer(pass);
   if (!pass) serializer(&midiParser,dummy,dummySize );
}

void test20b() {
  header("20b","isMidiStatus validation                             ");
  uint8_t dummy[] = {
    0x83, 0x40,0x40,
    0x94, 0x40,0x40,
    0xB2, 0x40,0x40,
    0xC0, 0x40,0x40,
    0XF8,
    0xF2,0X01,0X01,
    0xF0,0x01,0x02,0xF7,
  };

  unsigned dummySize = sizeof(dummy);
  bool pass=false;
  midiXparser midiParser;
  midiParser.setMidiMsgFilter( midiXparser::allMsgTypeMsk );
  uint8_t count = 0;

  for ( uint8_t i=0 ; i< dummySize ; i++ ) {
    if ( midiParser.parse(dummy[i]) ) {
      if ( midiParser.isMidiStatus(midiXparser::noteOffStatus ) ) count++;
      if ( midiParser.isMidiStatus(midiXparser::controlChangeStatus ) ) count++;
      if ( midiParser.isMidiStatus(midiXparser::timingClockStatus ) ) count++;
      if ( midiParser.isMidiStatus(midiXparser::songPosPointerStatus ) ) count++;
      if ( midiParser.isMidiStatus(midiXparser::soxStatus ) ) count++;
      if ( midiParser.isMidiStatus(midiXparser::eoxStatus ) ) count++;
    }
  }

  pass = ( count == 4 ) ;
  footer(pass);
  if (!pass) serializer(&midiParser,dummy,dummySize );

}
void test21b() {


}

void test20c() {

}
void test20d() {


}

void test20e() {

}

void test20f() {

   //            ****************************************************
   header("20f","SYSEX - Special case F0 F7                          ");

   uint8_t dummy[] = {0xF0, 0XF7};
   unsigned dummySize = sizeof(dummy);
   bool pass=false;

   midiXparser midiParser;
   midiParser.setMidiMsgFilter( midiXparser::allMsgTypeMsk );

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
   header("20h","SYSEX Error with one byte                           ");

   uint8_t dummy[] = {0xF0, 1,2,3,4,5,0XF8,0xF6,0xF8 };
   unsigned dummySize = sizeof(dummy);
   bool pass=false;

   midiXparser midiParser;
   midiParser.setMidiMsgFilter( midiXparser::allMsgTypeMsk );

   pass = midiCounterTests(&midiParser,dummy,dummySize,
     3 ,  // expMsgCount,
     9 , // expCapturedBytes,
     0 , // expSysExMsgCount,
     1 , // expSysExErrorCount,
     5 );// expSysExLen

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
                   0x90,0xF7,0x3A,0x64,0x80,0x3A,0x40,
                   0x90,0x3A,0x64,0x80,0x3A,0x40, // msg 13
                   0xF7,0xF0,0xF0, 1,2,3,4,5,          // error 5
                   0x90,0x3A,0x64,0x80,0x3A,0x40, // error 6 - msg 15
                   0x90,0x3A,0x64,1,2,3,  0xF0 , 4,
                   0x80,0x3A,0x40,1,2,  0xF7  ,3,4
    };
  unsigned dummySize = sizeof(dummy);
  bool pass=false;

  midiXparser midiParser;
  midiParser.setMidiMsgFilter( midiXparser::allMsgTypeMsk );

  pass = midiCounterTests(&midiParser,dummy,dummySize,
    18 ,  // expMsgCount,
    84,  // expCapturedBytes,
    1  , // expSysExMsgCount,
    10 ,  // expSysExErrorCount,
    21 ); // expSysExLen

  //pass = pass && midiCheckContent(&midiParser,dummy,dummySize);

  footer(pass);
  if (!pass) serializer(&midiParser,dummy,dummySize );
}
