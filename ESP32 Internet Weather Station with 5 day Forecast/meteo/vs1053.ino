#ifdef vs1053

void vs1053_showstreamtitle(const char *info)
{ // called from vs1053
  Serial.print("STREAMTITLE:  ");
  Serial.println(info);                           // Show title
  if (!play) return;

  static String sng = "";
  String sinfo = String(info);
  sinfo.replace("|", "\n");
#ifdef debug
  Serial.print("audio_showstreamtitle - ");
  Serial.println(info);
#endif
  play_song.trim();
  if (sinfo != sng)
  {
    play_song = sinfo;
    new_song = true;
    sng =  sinfo;
  }


}



void vs1053_showstreaminfo(const char *info) {      // called from vs1053
  Serial.print("STREAMINFO:   ");
  Serial.println(info);                           // Show streaminfo
}

void vs1053_eof_mp3(const char *info)
{ // called from vs1053
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


void vs1053_id3data(const char *info) { //id3 metadata

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

  new_song_mp3 = true;

}

/*

  void vs1053_info(const char *info) {                // called from vs1053
    Serial.print("DEBUG:        ");
    Serial.println(info);                           // debug infos
  }
*/


#endif // vs1053
