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

   //            ****************************************************
   header("10c","Filter = Note on/off default all channels           ");

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

   //            ****************************************************
   header("10d","filter NoMidiMsg but All channels                   ");

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

   //            ****************************************************
   header("10e","Filter all messages on channel 2                    ");

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

   //            ****************************************************
   header("10f","Filter all messages on channel 3                    ");

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

   //            ****************************************************
   header("10g","Filter all msg on channel 3 - no Real Time          ");

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

   //            ****************************************************
   header("10h","Filter all Real Time                                ");

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

   //            ****************************************************
   header("10i","Filter Real Time Timing Clock                       ");

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

   //            ****************************************************
   header("10j","Filter Real Time Start Stop Continue                ");

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

   //            ****************************************************
   header("10k","Filter program change & channel pressure            ");

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

   //            ****************************************************
   header("10l","Filter ctl chg - pitch bd with running status       ");

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
   //            ****************************************************
   header("10m","Top level filter - channelVoiceMsgTypeMsk       Ch 1");

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
   //            ****************************************************
   header("10n","Top level filter-systemCommonMsgTypeMsk       All Ch");

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
      //            ****************************************************
      header("10o","Top level filter-realTimeMsgTypeMsk           All Ch");

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
      //            ****************************************************
      header("10p","Top level filter-sysExMsgTypeMsk  on the fly  All Ch");

      uint8_t * dummy = dummyMsg2;
      unsigned dummySize = sizeof(dummyMsg2);
      bool pass=false;

      midiXparser midiParser;
      midiParser.setMidiChannelFilter(midiXparser::allChannel);
      midiParser.setMidiMsgFilter( midiXparser::sysExMsgTypeMsk );

      pass = midiCounterTests(&midiParser,dummy,dummySize,
        5 , // expMsgCount,
        52, // expCapturedBytes,
        5 , // expSysExMsgCount,
        2 , // expSysExErrorCount,
        32 ); // expSysExLen

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
      midiParser.setMidiChannelFilter(midiXparser::allChannel);
      midiParser.setMidiMsgFilter( midiXparser::realTimeMsgTypeMsk );

      pass = midiCounterTests(&midiParser,dummy,dummySize,
        8 , // expMsgCount,
        8, // expCapturedBytes,
        0 , // expSysExMsgCount,
        2 , // expSysExErrorCount,
        0 ); // expSysExLen

      footer(pass);
      if (!pass) serializer(&midiParser,dummy,dummySize );
}
