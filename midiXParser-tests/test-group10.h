void test10a() {
  //            ****************************************************
  header("10a","Default settings - no filters & no channel          ");

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

   //            ****************************************************
   header("10b","All msg - All channels                              ");

   uint8_t * dummy = dummyMsg1;
   unsigned dummySize = sizeof(dummyMsg1);
   bool pass=false;

   midiXparser midiParser;
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

void test10m() {
   //            ****************************************************
   header("10m","Top level filter - channelVoiceMsgTypeMsk           ");

   uint8_t * dummy = dummyMsg1;
   unsigned dummySize = sizeof(dummyMsg1);
   bool pass=false;

   midiXparser midiParser;
   midiParser.setMidiMsgFilter( midiXparser::channelVoiceMsgTypeMsk );

   pass = midiCounterTests(&midiParser,dummy,dummySize,
     15 , // expMsgCount,
     37, // expCapturedBytes,
     0 , // expSysExMsgCount,
     0 , // expSysExErrorCount,
     0 ); // expSysExLen

   footer(pass);
   if (!pass) serializer(&midiParser,dummy,dummySize );
}

void test10n() {
   //            ****************************************************
   header("10n","Top level filter-systemCommonMsgTypeMsk       All Ch");

   uint8_t * dummy = dummyMsg1;
   unsigned dummySize = sizeof(dummyMsg1);
   bool pass=false;

   midiXparser midiParser;
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
      //            ****************************************************
      header("10o","Top level filter-realTimeMsgTypeMsk           All Ch");

      uint8_t * dummy = dummyMsg1;
      unsigned dummySize = sizeof(dummyMsg1);
      bool pass=false;

      midiXparser midiParser;
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
      //            ****************************************************
      header("10p","Top level filter-sysExMsgTypeMsk  on the fly  All Ch");

      uint8_t * dummy = dummyMsg2;
      unsigned dummySize = sizeof(dummyMsg2);
      bool pass=false;

      midiXparser midiParser;
      midiParser.setMidiMsgFilter( midiXparser::sysExMsgTypeMsk );

      pass = midiCounterTests(&midiParser,dummy,dummySize,
        5 , // expMsgCount,
        52, // expCapturedBytes,
        5 , // expSysExMsgCount,
        2 , // expSysExErrorCount,
        40 ); // expSysExLen

      footer(pass);
      if (!pass) serializer(&midiParser,dummy,dummySize );
}

void test10q() {
      //            ****************************************************
      header("10q","Top level filter realTime only mixed sysex    All Ch");

      uint8_t * dummy = dummyMsg2;
      unsigned dummySize = sizeof(dummyMsg2);
      bool pass=false;

      midiXparser midiParser;
      midiParser.setMidiMsgFilter( midiXparser::realTimeMsgTypeMsk );

      pass = midiCounterTests(&midiParser,dummy,dummySize,
        8 , // expMsgCount,
        8, // expCapturedBytes,
        0 , // expSysExMsgCount,
        0 , // expSysExErrorCount,
        0 ); // expSysExLen

      footer(pass);
      if (!pass) serializer(&midiParser,dummy,dummySize );
}
