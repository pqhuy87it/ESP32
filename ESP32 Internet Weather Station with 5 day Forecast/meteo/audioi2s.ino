#if defined(audio_lib2)||defined(vs1053)
void   do_play_beep(byte id)
{
  if (play || play_mp3) return; //not support

#if defined(vs1053) &&!defined(pin_sd)
  return;
#endif

  // String track_name_str = "";
  const char * track;
  if (id == 0) track = "/beep.mp3";
  if (id == 1) track = "/timer.mp3";

#ifdef debug
  Serial.print("play  /"); Serial.println(track);
#endif
  do_pin_on_play(2);

  int   volume_mp3_tmp = beep_mp3_vol;
  volume_mp3_tmp = map(beep_mp3_vol, 0, 100, 0, 21); //21 max


#ifdef def_core0
#ifdef debug
  Serial.println("core0 beep lib2 or vs1053");
#endif
  audioSetVolume(volume_mp3_tmp);
  playing = audioConnecttoFS(track, 0);
#endif //c0



#ifndef def_core0
#ifdef debug
  Serial.println("beep lib2 or vs1053");
#endif
  audio.setVolume(volume_mp3_tmp);
#ifdef pin_sd
  audio.connecttoSD(track);
#else
  audio.connecttoFS(SPIFFS, track);
#endif
#endif

#ifndef def_core0
#if defined(vs1053)
  uint32_t sz = audio.getFileSize();
  while (audio.getFilePos() < sz)
  {
    audio.loop();
  }
  audio.stop_mp3client();
#endif //vs

#if defined(audio_lib2)
  while (audio.isRunning())
  {
    audio.loop();
  }
  audio.stopSong();
#endif //ib
#endif //nC0

  do_pin_off_play(2);


}

#endif //lib vs



#if defined(audio_lib2)

//-------------------------------------------------------------------------------------
//                           audio_showstreamtitle
//-------------------------------------------------------------------------------------

void audio_showstreamtitle(const char *info) {
  if (!playing) return;

  static String sng = "";
  String sinfo = String(info);
  sinfo = sinfo.substring(0, 60);
  sinfo.replace("|", "\n");

#ifdef debug
  Serial.print("audio_showstreamtitle - ");
  Serial.println(info);
#endif
  sinfo.trim();
  if (sinfo != sng)
  {
    play_song = sinfo;
    new_song = true;
    sng =  sinfo;
  }


}


//-------------------------------------------------------------------------------------
//                           audio_info
//-------------------------------------------------------------------------------------
#ifdef debug
void audio_info(const char *info) {
  Serial.print(F("audio_info: ")); Serial.println(info);
}
#endif

//-------------------------------------------------------------------------------------
//                            audio_id3data  29 40 err
//-------------------------------------------------------------------------------------

void audio_id3data(const char *info) { //id3 metadata

  if (!play_mp3) return;
#ifdef debug
  Serial.print("id3data     "); Serial.println(info);
#endif
  String name = "";
  String str = "";
  // static String sng = "";
  String sinfo = String(info);
  int tz = sinfo.indexOf(":") ;
  if (tz == -1) return;
  name = sinfo.substring(0, tz + 1);
  str = sinfo.substring(tz + 1);
  str.trim();
  name.trim();

#ifdef debug
  Serial.print("name     "); Serial.println(name);
  Serial.print("str     "); Serial.println(str);
#endif
  if (name == "Year:") //song
  {
    play_year_mp3 =  str;
    //    Serial.print("Year     "); Serial.println(str);
  }
  else if (name == "Artist:") //song
  {
    play_song_g_mp3 =  str;
    //    Serial.print("Artist     "); Serial.println(str);

  }
  else if (name == "Album:") //song
  {
    play_album_mp3 =  str;
    //    Serial.print("Album     "); Serial.println(str);

  }
  else

    if (name == "Title:") //song
    {
      play_song_mp3 =  str;
      //      Serial.print("Title     "); Serial.println(str);

    }

  new_song = true;


}


//-------------------------------------------------------------------------------------
//                            void audio_eof_mp3
//-------------------------------------------------------------------------------------

void audio_eof_mp3(const char *info) { //end of file

#ifdef debug
  Serial.print("eof_mp3 ---------   "); Serial.println(info);
#endif
  if (play_mp3)
  {
    fn_cmd = "next";
    do_fn_btn_ir();
    fn_cmd = "";
    draw_num(0, 'C');
    fn_cmd = "";
  }


}


#ifdef debug
void audio_showstation(const char *info) {
  Serial.print(F("showstation     ")); Serial.println(info);
}

void audio_lasthost(const char *info) { //stream URL played
  Serial.print(F("lasthost    ")); Serial.println(info);
}
#endif //db



#endif // audio_lib2
