
//==========================================================================================================
//                         do_fn_btn_ir
//==========================================================================================================

void do_fn_btn_ir()
{

  bool fnd_www = false;
  bool ch_fnd_fm = false;
  static byte fav = 0;

#ifdef debug
  Serial.print(F("do_fn_btn_ir---->")) ; Serial.println(fn_cmd) ;
#endif

  if (fn_cmd == "") return;
  fn_cmd_tmp = fn_cmd;

  if (s_info) //exit  info
  {
    fn_cmd = "Scr_info";
  }
  //------------------------------------play_bg
  if (fn_cmd == "play_bg" && !s_info && !scr_timer && !play_mp3 && !play_fm)
  {
    if (!play && !play_bg)
    {
      fn_cmd = "play";
      play_bg = true;
    }
    else if (play && play_bg)
    {
      fn_cmd = "stop";
      play_bg = false;
    }
    else if (play && !play_bg)
    {
      play_bg = true;
      tft.fillScreen(TFT_BLACK);
      refresh_all = true;
      Serial.print("play_bg"); Serial.println(play_bg);
      return;
    }
    Serial.print("play_bg"); Serial.println(play_bg);

  }

  if (s_info)
  {
    if (fn_cmd == "play_fm" || fn_cmd == "play_mp3" || fn_cmd == "play")//alarm
    {
      s_info = false; //при выходе на осн экран после alarm
#ifdef epd
      return;
#endif
    }
    else if (fn_cmd != "Scr_info"   ) return; //only exit scr
  }

  if (fn_cmd == "Scr_info" && !play && !play_mp3 && !play_fm && !scr_timer)
  {
    s_info = !s_info;
    if (s_info)
    {
      s_info_show();
    }
    else //exit s_info
    {
      tft.fillScreen(TFT_BLACK);
      refresh_all = true;
    }
    fn_cmd = "";
    return;
  }

  if (fn_cmd == "restart")
  {
    ESP.restart();
    return;
  }

  //--------------------------TIMER----------------------------------------------
#if !defined(tft_400_300) &&!defined(no_tft)
  if (!play_fm && !play_mp3 && !play && !scr_timer && sync_t)
  {
    if (fn_cmd == "timer" ) //scr timer
    {

      scr_timer = true;
      tft.fillScreen(TFT_BLACK);
      refresh_all = true; //первый запуск
      //  do_scr_timer() ;
      //  refresh_all = false;
      fn_cmd = "";
      return;
    }

    if (fn_cmd.indexOf("tm;") == 0)
    {
      String str = fn_cmd.substring(3);
      Serial.print("TM"); Serial.println(str);
      uint32_t sec_tmp = 60 * str.toInt();
      Serial.print("sec_tmp"); Serial.println(sec_tmp);
      t_timer_boot = now() + sec_tmp;
      scr_timer = true;
      tft.fillScreen(TFT_BLACK);
      refresh_all = true; //первый запуск
      fn_cmd = "";
      return;
    }
  }
  if (scr_timer) //уже экран таймера
  {
    if (timer_alarm)
      timer_alarm = false; //любая команда сброс alarm
    do_scr_timer() ;
    fn_cmd = "";
    return;
  }


#endif //#if !defined(tft_400_300)
  //------EEE--------------------TIMER----------------------------------------------



  //----------------------------------------MP3------------------------------------------
#if defined (pin_sd)&&defined (ESP32)
  //-------------STOP
  if ((fn_cmd == "stop_mp3" || fn_cmd == "stop") && play_mp3)
  {
    //#ifndef only_radio
    draw_stop();
    play_mp3 = false;

#if defined(def_core0)
    audioStopSong(); //+vs1053
#elif  defined(audio_lib2)
    audio.stopSong();
#elif  defined(vs1053)
    audio.stop_mp3client();
#else
    StopPlaying(); //off
#endif

    vol_rotate = false;
    scr_num_show(scr_number);
    do_pin_off_play(2);
    return;
  }

  //-------------PLAY MP3------------------------------------------------------------
  if (fn_cmd == "play_mp3")
  {

    if (!play_mp3)
    {
      if (  max_num_mp3 == 0)
      {
        Serial.println("MP3 count=0");
        return;
      }
      tft.fillScreen(TFT_BLACK);
      refresh_all = true; //первый запуск
      do_mute_mp3();//reset mute
      play_mp3 = true;
      play = false;
      play_fm = false;

      volume_mp3 = vol_def_mp3;
      set_vol_mp3();
      do_pin_on_play(2);

    }

    do_play_track(0);//0-start 1 nxt 2 prv

    return;
  }


  if (fn_cmd == "next" && play_mp3) //след. станция если уже играет
  {

    draw_next();
    do_play_track(1);//0-start 1 nxt 2 prv
    return;
  }


  if (fn_cmd == "prev" && play_mp3) //след. станция если уже играет
  {

    draw_prev();
    do_play_track(2);//0-start 1 nxt 2 prv
    return;
  }

  if (fn_cmd == "mute" && play_mp3) //volume
  {

    do_mute_mp3();
    return;
  }

  if (fn_cmd == "vl+" && play_mp3) //volume
  {
    volume_mp3 = volume_mp3 + 5;
    set_vol_mp3();
    vol_rotate = true;
    return;
  }

  if (fn_cmd == "vl-" && play_mp3) //volume
  {
    volume_mp3 = volume_mp3 - 5;
    set_vol_mp3();
    vol_rotate = true;
    return;

  }

  if ((fn_cmd == "mp3_random" || fn_cmd == "fav") && play_mp3) //volume
  {
    set_random_mp3();
    return;

  }

#endif //pin_sd mp3




  //----------------------------------------FM------------------------------------------
#if defined (fmSi4703)|| defined (fmRDA5807)|| defined (fmSi4703_lib2)
  static byte fav_fm = 0;


  //-------------STOP
  if ((fn_cmd == "stop_fm" || fn_cmd == "stop" ) && play_fm)
  {
    Serial.println("stop_fm");
    //  ch_prev_alarm = 0;
    //draw_stop();
    fm_setVolume(0);
    play_fm = false;
    vol_rotate = false;
#ifdef fmRDA5807
    FM.setMute(true);
#endif
    do_pin_off_play(3);
    scr_num_show(scr_number);
    return;
  }

  //------------------
  if (fn_cmd == "fav" && play_fm)
  {
    if (fav_count_real_fm == 0) return;
    {
      if (ch_fm == fav_ch_fm[fav_fm])
      {
        fav_fm++;  //если в данный момент играет избр.
        if (fav_fm >= fav_count_real_fm) fav_fm = 0;
      }

      ch_fm = fav_ch_fm[fav_fm];
      fav_fm++;
      if (fav_fm >= fav_count_real_fm) fav_fm = 0;
      ch_fnd_fm = do_select_num_fm(ch_fm);
      if (ch_fnd_fm)
      {
        Serial.print("favor fm="); Serial.println(ch_fm);
      }
      else
      {
        Serial.print("favor not found fm="); Serial.println(ch_fm);
      }

    }

    return;
  }
  //-------------PLAY
  if (fn_cmd == "play_fm")
  {
    Serial.print("ch_fnd_fm="); Serial.println(ch_fm);

    if (!play_fm)
    {
      tft.fillScreen(TFT_BLACK);
      refresh_all = true; //первый запуск
      play_fm = true;
      play = false;
      play_mp3 = false;
      Serial.println("FM ref_all");

      do_pin_on_play(3);

    }

    ch_fnd_fm = do_select_num_fm(ch_fm);
    if (!ch_fnd_fm) do_select_num_fm(ch_fm);//прорисовываю не найденный канал
    return;
  }


  if (fn_cmd == "vl+" && play_fm) //volume
  {
    volume_fm = volume_fm + 1;
    if (volume_fm > 15) volume_fm = 15;
#ifdef debug
    Serial.print("volume fm=") ; Serial.println(volume_fm) ;
#endif
    fm_setVolume(volume_fm);
    vol_rotate = true;
    draw_vol_fm();
    vol_def_fm = volume_fm;
    return;
  }

  if (fn_cmd == "vl-" && play_fm) //volume
  {
    volume_fm = volume_fm - 1;
    if (volume_fm < 0) volume_fm = 0;
#ifdef debug
    Serial.print("volume fm=") ; Serial.println(volume_fm) ;
#endif
    fm_setVolume(volume_fm);
    vol_rotate = true;
    draw_vol_fm();
    vol_def_fm = volume_fm;
    return;
  }

  if (fn_cmd == "mute" && play_fm) //volume
  {
    // draw_vol_fm();
    do_mute_fm();
    return;
  }


  if (fn_cmd == "prev" && play_fm) //prev. станция если уже играет
  {
    draw_prev();
    if (ch_fm == ch_min_fm)
      ch_fm = ch_max_fm;
    else
      ch_fm = ch_prev_fm;
    ch_fnd_fm = do_select_num_fm(ch_fm);

    return;
  }

  if (fn_cmd == "next" && play_fm)       //next stat.
  {
    draw_next();
    if (ch_fm == ch_max_fm)
      ch_fm = ch_min_fm;
    else
      ch_fm = ch_next_fm;
    ch_fnd_fm = do_select_num_fm(ch_fm);

    return;
  }


#endif // fmSi4703
  //------------------------------------EEEEEEE--------FM--------------------



  //------------------------NOT  PLAY----------------------------------------
  if (!play && !play_fm && !play_mp3) //след. станция если уже играет
  {


    // SCR Sfoto
#ifdef pin_sd
    if (fn_cmd == "Sf") //след. станция если уже играет
    {
      if (do_find_scr( (char *)"Sf"))
        scr_num_show(scr_number);
      return;
    }
#endif
    // SCR Sn
    if (fn_cmd == "Sn")
    {
      if (do_find_scr( (char *)"Sn"))
        scr_num_show(scr_number);
      return;
    }





#ifdef pin_led
    if (fn_cmd == "led+") //volume
    {
      led_val = led_val + 5;
      if (led_val > 100) led_val = 0;
      do_led(led_val);

      return;
    }
    if (fn_cmd == "led-" ) //volume
    {
      led_val = led_val - 5;
      if (led_val < 0) led_val = 100;
      do_led(led_val);
      return;
    }

#endif //led



    //next SCR
    if (fn_cmd == "next" || fn_cmd == "next_scr") //след. станция если уже играет
    {
      scr_number++;
      scr_num_show(scr_number);
      return;
    }

    //prev SCR
    if (fn_cmd == "prev" || fn_cmd == "prev_scr" ) //след. станция если уже играет
    {
      scr_number--;
      scr_num_show(scr_number);

      return;
    }


  } //----------------NOT PLAY


  //----------------PLAY---------------WWWWWWW------------------------------------------------

#ifdef ESP32

  //next SCR
  if (fn_cmd == "next_scr" && play ) //след. станция если уже играет
  {
    if (play_bg)
    {
      scr_number++;
      scr_num_show(scr_number);
      return;
    }
    fn_cmd = "next";
  }

  //prev SCR
  if (fn_cmd == "prev_scr" && play ) //след. станция если уже играет
  {
    if (play_bg)
    {
      scr_number--;
      scr_num_show(scr_number);
      return;
    }
    fn_cmd = "prev" ;
  }



  if (fn_cmd == "play_bg" && play && play_bg) //exit bg
  {
    tft.fillScreen(TFT_BLACK);
    refresh_all = true;
    play_bg = false;
    fnd_www = do_select_num_st(ch);
    return;
  }

  if (fn_cmd == "vl+" && play) //volume
  {
    volume = volume + 5;
    set_vol_www();
    vol_rotate = true;
    return;
  }



  if (fn_cmd == "vl-" && play) //volume
  {
    volume = volume - 5;
    set_vol_www();
    vol_rotate = true;
    return;
  }

  if (fn_cmd == "mute" && play) //volume
  {
    do_mute();
    return;
  }

  if (fn_cmd == "stop"  && play) //stop
  {
    //  ch_prev_alarm = 0;

    vol_rotate = false;
#ifndef only_radio
    draw_stop();

#if defined(def_core0)
    audioStopSong(); //+vs1053
#elif  defined(audio_lib2)
    audio.stopSong();
#elif  defined(vs1053)
    audio.stop_mp3client();
#else
    StopPlaying(); //off
#endif
    play_song = "";
    playing = false;
    play = false;
    play_bg = false;
    do_pin_off_play(1);
    scr_num_show(scr_number);
    return;
#else
    fn_cmd = "next";
    Serial.print(F("only radio ")) ;
    Serial.println(fn_cmd) ;

#endif //only_radio
  }

  //---------------------ELSE begin
  if (fn_cmd == "play" && play && play_bg)  //show player
  {
    if (play_bg) play_bg = false;
    refresh_all = true;
    tft.fillScreen(TFT_BLACK);
  }
  else if (fn_cmd == "play"  && !play) //play on
  {
    tft.fillScreen(TFT_BLACK);
    refresh_all = true; //первый запуск
    do_mute(); //reset mute;
    //    draw_vol();
    do_pin_on_play(1);
    play_mp3 = false;
    play_fm = false;
    play = true;

  }
  else
    //prev stat.
    if (fn_cmd == "prev" && play) //prev. станция если уже играет
    {
      if (!play_bg) draw_prev();
      if (ch == ch_min)
        ch_fnd = ch_max;
      else
        ch_fnd = ch_prev;
    }
    else
      //next stat.
      if (fn_cmd == "next" && play) //след. станция если уже играет
      {
        if (!play_bg) draw_next();
        if (ch == ch_max)
          ch_fnd = ch_min;
        else
          ch_fnd = ch_next;
      }
      else
        //pl_list
        if (fn_cmd == "next_pl" && play && pl_list_max > 0) //след. станция если уже играет
        {
          if (pl_list < pl_list_max) pl_list++;
          else pl_list = 0;
          do_pl_name(pl_list);
          if (!play_bg)
          {
            tft.fillScreen(TFT_BLACK);
            refresh_all = true;
          }
          ch_fnd = 1;

#if defined(def_core0)
          audioStopSong(); //+vs1053
#elif  defined(audio_lib2)
          audio.stopSong();
#elif  defined(vs1053)
          audio.stop_mp3client();
#else
          StopPlaying(); //off
#endif
          playing = false;
        }
        else
          //pl_list--
          if (fn_cmd == "prev_pl" && play && pl_list_max > 0) //след. станция если уже играет
          {
            if (pl_list > 0) pl_list--;
            else pl_list = pl_list_max;
            do_pl_name(pl_list);
            if (!play_bg)
            {
              tft.fillScreen(TFT_BLACK);
              refresh_all = true;
            }
            ch_fnd = 1;

#if defined( def_core0)
            audioStopSong(); //+vs1053
#elif  defined(audio_lib2)
            audio.stopSong();
#elif  defined(vs1053)
            audio.stop_mp3client();
#else
            StopPlaying(); //off
#endif
            playing = false;
          }
          else if (fn_cmd == "fav" && play)
          {
            if (ch == fav_ch[fav]) {
              fav++;  //если в данный момент играет избр.
              if (fav >= fav_count_real) fav = 0;
            }

            ch_fnd = fav_ch[fav];
            fav++; if (fav >= fav_count_real) fav = 0;
            //  fn_cmd = "play";
#ifdef debug
            Serial.print("favor ch_fnd=");
            Serial.println(ch_fnd);
#endif

          }
          else if (fn_cmd == "play" && play) ;//ch
          else
            return;


  fnd_www = do_select_num_st(ch_fnd);
  if (!fnd_www)
  {
    Serial.print(F("ch not found==")) ;
    Serial.println(ch_fnd);
    draw_num(0, 'C');
    return;
  }

  //--------------------------------------------------connect
#if defined(audio_lib2)||defined(vs1053)
  const char *url = ch_url.c_str();
  set_vol_www();
  if (!play_bg) tft.fillCircle(120, 85, 4, TFT_RED );
#if defined(def_core0)
  playing = audioConnecttohost(url);
  buff_fill = audioInbuffFilled();
  buff_free = audioInbuffFree();

  if (playing && !play_bg) tft.fillCircle(120, 85, 4, TFT_GREEN );

#else
  playing = audio.connecttohost(url);
  if (playing && !play_bg) tft.fillCircle(120, 85, 4, TFT_GREEN );
#endif //lib2 vs
#else
  do_create_http();// <---  set_vol_www();
#endif

  delay(500);
  draw_num(0, 'C');


#endif // ESP32

}



#ifdef ESP32

//-----------------------------------------------------------------
//                    do_pl_name
//-----------------------------------------------------------------
bool do_pl_name(uint8_t pl)
{

  if  (pl == 0) pl_list_file_name = "/i_stat.txt";
  else
    pl_list_file_name = "/i_stat" + String(pl) + ".txt";
#ifdef debug
  Serial.print("pl_list_file_name " ); Serial.println(pl_list_file_name );
  Serial.print(F("pl_list==")) ;
  Serial.println(pl_list);
#endif
}



//======================================================================================
//                    do_select_num_st
//======================================================================================
bool do_select_num_st(byte num_find)
{
  //  static byte pl_list_t = 1;
  static int x_ch_old = 0;
  static int y_ch_old = 0;
  static byte ch_old = 0;
  static int x_ch = 0;
  static int y_ch = 0;

  if (refresh_all  )
  {
    stat_num = ";";
    ch_min = 254; //номер п.п 1 станции
    ch_max = 0; //номер п.п последней станции
    pl_list_name = "";
    x_ch_old = 0;
    y_ch_old = 0;
    ch_old = 0;
    x_ch = 0;
    y_ch = 0;
  }
  if (stat_num != ";" && num_find <= ch_max && num_find >= ch_min)
  {
    if (stat_num.indexOf(";" + String(num_find) + ";") == -1 )
    {
#ifdef debug
      Serial.print("stat_num not found ");
      Serial.print(stat_num); Serial.print("  ");
      Serial.println(";" + String(num_find) + ";");
#endif
      return false;
    }
  }
  String filename = pl_list_file_name;
#ifdef pin_sd
#ifdef debug
  Serial.print("SD ");
  Serial.println(filename);
#endif
  File f = SD.open( filename, FILE_READ);  // or, file handle reference for SD library
#else
#ifdef debug
  Serial.print("Spiffs ");
  Serial.println(filename);
#endif
  fs::File f = SPIFFS.open( filename, "r");    // File handle reference for SPIFFS
#endif


  if (!f) {
    Serial.print(filename);
    Serial.println(F(" no open,create"));
    return false;
  }
  //----------------------------
  ch_next = 254;
  ch_prev = 0;

  String name = "";
  String url = "";
  bool find_num = false;
  bool find_ch = false;
  byte num_tmp = 254;
  String logo_tmp = "";

  const byte y_begin = 75;
  int x = 133;



#ifdef tft_320_240
  const byte y_step = 14;
  const byte x_step = 93;
  const int w = 320;
  const int h = 200;
  tft.setFreeFont(v6);
#endif
  //--------------------------
#ifdef tft_480_320
  tft.setFreeFont(v9mono);
  const byte y_step = 18;
  const byte x_step = 173;
  const int w = 480;
  const int h = 250;
#endif


#ifdef tft_400_300
  const byte y_step = 14;
  const byte x_step = 93;
  const int w = 320;
  const int h = 200;
  tft.setFreeFont(v6);
#endif
  byte y = y_begin + 14;


  byte vol = vol_def;
  String str;



#ifdef debug
  Serial.print(F("pl list name:")); Serial.println(filename);
  Serial.print("num_fnd:");  Serial.println(num_find);
#endif
  //---------------------------------------------------------
  while (f.available()) {
    yield();
    str = f.readStringUntil('\n');
    //Serial.println(str);
    //if (str.indexOf("{")>=0) (Serial.print("{=");Serial.println(str.indexOf("{")));
    if (str.indexOf("{") == 0 && refresh_all)
    {
      //#if defined(ESP32) && (defined(encoder)||defined(encoder_lib2))
      do_fav_ch(str, 0); //start play
      continue;
      //#endif
    }


    if (str.indexOf("#") != 0) continue;
    int tz = 0;
    int tz2 = 0;
    find_num = false;

    name = "";
    url = "";
    logo_tmp = "";
    //num------------------------
    tz2 = str.indexOf(";", tz + 1);
    if (tz2 > 0)
    {
      if (refresh_all) //pl_list_name
      {
        String pl_name = str.substring(tz + 1, tz2);
        if (pl_name == "pl_list_name")
        {
          tz = tz2;
          tz2 = str.indexOf(";", tz + 1);
          pl_list_name = str.substring(tz + 1, tz2);
#ifdef debug
          Serial.print("pl_list_name:");  Serial.println(pl_list_name);
#endif
          continue;
        }
      }

      num_tmp = str.substring(tz + 1, tz2).toInt();

      //ffffffffffffff
      if (num_tmp == num_find)
      {
        ch = num_tmp;
#ifdef debug
        Serial.print("find------------>");
        Serial.println(num_tmp);
#endif

        find_num = true;
      }
      //name --------------------------------
      tz = tz2;
      tz2 = str.indexOf(";", tz + 1);
      if (tz2 > 0)
      {
        //Serial.println(str.substring(tz+1,tz2));
        name = str.substring(tz + 1, tz2);
      }
      else
      {
#ifdef debug
        Serial.println(F("not found name ch"));
#endif
        find_num = false;
        continue;
      }
      ///url -------------------------------
      tz = tz2;
      tz2 = str.indexOf(";", tz + 1);
      if (tz2 > 0)
      {
        //Serial.println(str.substring(tz+1,tz2));
        url = str.substring(tz + 1, tz2);
      }
      else
      {
#ifdef debug
        Serial.println(F("not found name URL ch"));
#endif
        find_num = false;
        continue;
      }


      //---------------------------
      //      Serial.print("num_tmp ");   Serial.println(num_tmp);
      //max
      if (ch_max < num_tmp) ch_max = num_tmp;
      //     Serial.print("ch_max ");   Serial.println(ch_max);
      //min
      if (ch_min > num_tmp) ch_min = num_tmp;
      //     Serial.print("ch_min ");   Serial.println(ch_min);

      //next
      if (num_tmp > num_find) //в случае неудачи будет след канал
      {
        if (ch_next > num_tmp)
        {
          ch_next = num_tmp;
          //       Serial.print("ch_next ");   Serial.println(ch_next);
        }
      }

      //prev
      if (num_tmp < num_find) //в случае неудачи будет след канал
      {
        if (ch_prev < num_tmp)
        {
          ch_prev = num_tmp;
          //        Serial.print("ch_prev ");   Serial.println(ch_prev);
        }
      }
      //---------------------------



      //volume vvvvvvvvvvvvvvvvvvvvvvvvv ++++++   LOGO
      tz = tz2;
      tz2 = str.indexOf(";", tz + 1);
      if (tz2 > 0 && find_num) //если ещё не крутил громкость иначе оставляю
      {
        String str_vol = str.substring(tz + 1, tz2);
        vol = str_vol.toInt();
        if (vol > 0 && vol < 101) ;//оставляю
        else
          vol = vol_def;
        //logo
        tz = tz2;
        tz2 = str.indexOf(";", tz + 1);
        if (tz2 > 0 && find_num)
        {
          logo_tmp = "/" + str.substring(tz + 1, tz2);
        }

      }

      //2 колонки не заполнены
      //----------------------------------------------------------------------------------
#ifndef no_tft
      if (!play_bg) //в фоне не рисую
      {
        if (x < w && refresh_all ) //2 колонки не заполнены
        {
          tft.setCursor(x, y);
          tft.setTextColor(TFT_CYAN);
          tft.print(num_tmp); tft.print(" ");
          tft.setTextColor(c_channel);
          //  if (num_tmp < 10) tft.print(" ");
          tft.print(name);

        }
      } //bg
#endif

      if (refresh_all ) //string for search ch
      {
        //         Serial.print("stat_num ");
        //         Serial.println(stat_num);
        stat_num = stat_num + num_tmp + ";";
      }
      ////--------------------------------find
      if (find_num)
      {
        x_ch = x;
        y_ch = y;
#ifndef no_tft
        if (!play_bg)
        {
          if (x_ch_old != 0 & y_ch_old != 0)
          {
            tft.setCursor(x_ch_old, y_ch_old);
            //   tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
            tft.setTextColor(TFT_CYAN);
            tft.print(ch_old);
            tft.drawFastHLine(x_ch_old, y_ch_old + 3, x_step - 10, TFT_BLACK) ;
          }
          tft.setCursor(x_ch, y_ch);
          tft.setTextColor(TFT_YELLOW);
          tft.print(ch);
          tft.drawFastHLine(x_ch, y_ch + 3, x_step - 10, c_ch_line) ;
        }//bg
#endif //no tft

        x_ch_old = x_ch;
        y_ch_old = y_ch;
        ch_old = ch;
        ch_url = url;
        ch_name = name;
        find_ch = true;
        if (!vol_rotate) volume = vol;
        logo_www = logo_tmp;


      }//---------------------------E find


    }


#ifndef no_tft
    if (!play_bg)
    {
      y = y + y_step;
      if (y > h - 3) //очищаю под вторую колонку каналов
      {
        y = y_begin + y_step;
        x = x + x_step;

        if (x < w - 30 && refresh_all)
        {
          tft.fillRect(x, y, w - x, h - y_begin, TFT_BLACK) ; //ch
          tft.drawFastVLine(x - 1, y_begin, h - y_begin, TFT_LIGHTGREY) ; //ch
        }
      }
    }//bg
#endif //no tft



  }
  f.close();

#if defined(tft_480_320)&&!defined(no_tft)
  if (!play_bg)
  {
    if (refresh_all)
    {
      tft.setTextColor(TFT_YELLOW);
      tft.setFreeFont(v12);
      tft.setCursor(103, 227);
      tft.print(pl_list);
      tft.setFreeFont(v9mono);
      tft.setCursor(100, 248);
      tft.print(pl_list_name);
    }
  } //bg
#endif


  if (ch_next > ch_max) ch_next = ch_min;
  if (ch_prev < ch_min) ch_prev = ch_max;
  if (!find_ch && ch_old != 0 && !alarm_active) ch = ch_old;
  if (!find_ch && alarm_active)
  {
    ch = num_find;
#ifdef EN
    ch_name = String(num_find) + " NOT FOUND in playlist 0 ";
#else
    ch_name = String(num_find) + " НЕТ КАНАЛА в плейлисте 0";
#endif
  }

#ifdef debug
  Serial.print(F("find_ch---true/false--->"));
  Serial.println(find_ch);
  Serial.print(F("fnd_ch------------->"));
  Serial.println(num_tmp);
  Serial.print(F("ch------------->"));
  Serial.println(ch);
  Serial.print(F("ch_name----->"));
  Serial.println(ch_name);
  Serial.print(F("url="));
  Serial.println(ch_url);
  Serial.print(F("vol="));
  Serial.println(volume);
  Serial.print(F("logo_www="));
  Serial.println(logo_www);
  Serial.print(F("ch_max="));
  Serial.println(ch_max);
  Serial.print(F("ch_min="));
  Serial.println(ch_min);
  Serial.print(F("ch_prev="));
  Serial.println(ch_prev);
  Serial.print(F("ch_next="));
  Serial.println(ch_next);
  Serial.print("stat_num ");
  Serial.println(stat_num);
  Serial.print("pl_list_file_name ");
  Serial.println(pl_list_file_name);
#endif

  //  if (new_pl) pl_list_t = pl_list;
  return find_ch;
}



#ifdef pin_sd
//=================================================================================================
//                    do_play_track
//=================================================================================================
void do_play_track(byte id ) //0 play,1 next,2 prev
{
  bool fnd = false;
  String filename;
  unsigned int track_tmp = curr_track; //if play

  if (id == 0) //start or random
  {
    if (track_find > 0)  track_tmp = track_find; //поиск с пульта
    else if (curr_track == 0 && !mp3_random)   track_tmp = min_num_mp3; //1 start
    else if (mp3_random) track_tmp = random(min_num_mp3, max_num_mp3 + 1);


  }

  if ((id == 1 || id == 2) && mp3_random) //next prev
  {
    track_tmp = random(min_num_mp3, max_num_mp3 + 1);
    //сразу ищу ,не нашел ищу в цикле
    if (track_tmp != curr_track)
    {
      filename = "/mp3/" + String(track_tmp) + ".mp3";
      fnd = SD.exists(filename);

    }
  }
#ifdef debug
  Serial.print("do_play_track ID=");  Serial.println(id);
  Serial.print("track_find ");  Serial.println(track_find);
  Serial.print("curr_track ");  Serial.println(curr_track);
#endif

  //----------

  int count_break = 0; //max_num_mp3
  while (!fnd)
  {
    //id == 0 //start, find
    if ((id == 1 || mp3_random) && (track_find == 0)) //next
    {
      if (track_tmp < max_num_mp3)
        track_tmp++;
      else track_tmp = min_num_mp3;
    }
    if (id == 2) //prev
    {
      if (track_tmp > min_num_mp3)
        track_tmp--;
      else track_tmp = max_num_mp3;
    }

    filename = "/mp3/" + String(track_tmp) + ".mp3";
    fnd = SD.exists(filename);

#ifdef debug
    if (!fnd)    {
      Serial.print("NOT exists ");
      Serial.println(filename);

    }
    else
    {
      Serial.print("ok found track_tmp ");
      Serial.println(filename);
    }
#endif


    if (track_find != 0 && !fnd)
    { //поиск с пульта один цикл
      track_find = 0;
      break;
    }

    count_break++;
    if (count_break > max_num_mp3)
    {
      Serial.print("count_break = max ");
      Serial.println(count_break);
#ifdef debug

#endif
      break; //из бескон цикла
    }

  }//while
  //----------
  track_find = 0;
  if (count_break > max_num_mp3 && !fnd)
  {
#ifdef debug
    Serial.print("do_play_track() ");
    Serial.println("count_break > max_num_mp3 break");
#endif
    fn_cmd = "stop_mp3";
    do_fn_btn_ir();
    fn_cmd = "";
    return;

  }


  if (!fnd) return;
  curr_track = track_tmp;
  track_name = String(curr_track);
#ifdef debug
  Serial.print("do_play_track()=");
  Serial.println(track_name);
#endif


#if defined(audio_lib2)||defined(vs1053)
  String track_name_str = "/mp3/" + track_name + ".mp3";
  const char * track = track_name_str.c_str();
#endif

  if (play_mp3)
  {
#if defined (audio_lib2)||defined (vs1053)
#if defined( def_core0)
    audioStopSong(); //+vs1053
    playing = audioConnecttoFS(track, 0);
#else //c0
#if defined (vs1053)
    audio.stop_mp3client();
#else
    audio.stopSong();
#endif    //vs
    audio.connecttoSD(track);
#endif
    //#endif

#else //lib2
    StopPlaying();
    do_create_mp3(); //0-beep 1 play_mp3
#endif

    //  track_tmp = 0;
  }
}

#endif //sd


//=======================================================
//      set_vol_www
//=======================================================
void set_vol_www()
{
  //max_volume   = 21; lib2
  if (volume > 100)volume = 100;
  if (volume < 0)volume = 0;
  int volume_www_tmp;
#if defined (vs1053)||defined (audio_lib2)
  volume_www_tmp = map(volume, 0, 100, 0, 21); //21 max
#ifdef def_coreo
  audioSetVolume(volume_www_tmp);
#else
  audio.setVolume(volume_www_tmp);
#endif
#else//lib2 vs
  volume_www_tmp = map(volume, 0, 100, 0, 400);
  out->SetGain(((float)volume_www_tmp) / 100.0);
#endif
#ifndef no_tft
  draw_vol();
#endif

#ifdef debug
  Serial.print("volume_www=") ; Serial.print(volume) ;
#if defined (vs1053)||defined (audio_lib2)
  Serial.print(" volume_tmp=") ; Serial.println(volume_www_tmp) ;
#else
  Serial.print(" volume_tmp=") ; Serial.println((float)volume_www_tmp / 100.0) ;
#endif

#endif

}








//------------------------------------------------------------=
//#ifndef audio_lib2
#if !defined(audio_lib2)&& !defined(vs1053)
//---------------------------------------------------------
//                  do_play
//---------------------------------------------------------
void do_play()
{

  if (mp3) {
    if (mp3->isRunning())
    {
      if (!mp3->loop())
      {
        StopPlaying();
        Serial.println("!mp3->loop()");
      }
    }
  } else {


    if (millis() > retryms) {
      retryms = 0;
      Serial.println("if (millis()>retryms)");
      do_create_http();
    }

  }


}




//==========================================================
//			do_create_http
//==========================================================

void do_create_http()
{
  static byte rep = 0;

  if (rep > rep_count) //3 попытки проигрывания, если неудача играю след.
  {
    rep = 0;
    Serial.println("if (rep>rep_count) ");

    // do_select_num_st(ch_next);
    fn_cmd = "next";
    do_fn_btn_ir();
    fn_cmd = "";

  }
  else
    StopPlaying();

  //file = new AudioFileSourceHTTPStream;
  file = new AudioFileSourceICYStream;
  file->RegisterMetadataCB(MDCallback, (void*)"ICY");
  delay(500);

  bool f_open = file->open(ch_url.c_str());


  if (f_open == true) {
    //   Serial.println("f_open==true ");

    buff = new AudioFileSourceBuffer(file, preallocateBuffer, preallocateBufferSize);
    mp3 = new AudioGeneratorMP3(preallocateCodec, preallocateCodecSize);
    mp3->begin(buff, out);
    if (!mp3->isRunning()) {
      Serial.println(F("Can't connect to URL"));
      Serial.println(ch_url);
      tft.fillCircle(120, 85, 4, TFT_YELLOW);
      StopPlaying();
      retryms = millis() + 1000;
      rep++;
      return;
    }

    set_vol_www();
#ifdef set_audio_mono
    out->SetOutputModeMono(true) ;
    tft.fillCircle(120, 85, 4, TFT_BLACK);
#endif
  }
  else
  { //error open
    Serial.println(F("Can't open file->URL"));
    rep++;
    tft.fillCircle(120, 85, 4, TFT_YELLOW);
    StopPlaying();
    retryms = millis() + 1000;
    yield();
  }

#ifdef debug
  Serial.print(F("rep_count="));
  Serial.println(rep);

#endif


}

//#endif //ESP32





//=======================================================
//			StopPlaying
//=======================================================

void StopPlaying()
{
#ifdef debug
  Serial.print(F("begin StopPlaying()="));
  Serial.println( ESP.getFreeHeap());
#endif

  if (mp3) {
    out->SetGain(0);
    mp3->stop();
    delete mp3;
    mp3 = NULL;
    Serial.println("delete mp3");
  }

  if (buff) {
    buff->close();
    delete buff;
    buff = NULL;
    Serial.println("delete buff");
  }
  if (file) {
    file->close();
    delete file;
    file = NULL;
    Serial.println("delete file");
  }


  if (id3) {
    Serial.println("delete id3");
    id3->close();
    delete id3;
    id3 = NULL;
    Serial.println("delete ok id3");
  }


  if (file_mp3) {
    file_mp3->close();
    delete file_mp3;
    file_mp3 = NULL;
    Serial.println("delete file_mp3");
  }



#ifdef debug
  Serial.print(F("end StopPlaying()="));
  Serial.println( ESP.getFreeHeap());
#endif

  if (WiFi.status() != WL_CONNECTED)
  {
    play = false;
    t_timer = 0;
    //    scr_num_show(scr_number);
  }


}



//=========================================================================
//                    play_beep
//=========================================================================
void  do_play_beep(byte id)
{
  bool f_open = false;
#ifdef pin_sd
  Serial.println(" do_play_beep SD ");
  file_mp3 = new AudioFileSourceSD();
#else
  Serial.println("do_play_beep Spiffs ");
  file_mp3 = new AudioFileSourceSPIFFS;
#endif
  String filename;
  if (id == 0) filename = "/beep.mp3";
  if (id == 1) filename = "/timer.mp3";
  const char * fl = filename.c_str();

  f_open = file_mp3->open(fl);
#ifdef debug
  Serial.print("play  /"); Serial.println(filename);
#endif

  if (!f_open)
  {
    Serial.print(F(" Can't open ")); Serial.println(filename);
    StopPlaying(); // in StopPlaying do_play_track(1);
    return;
  }
  do_pin_on_play(2);

  buff = new AudioFileSourceBuffer(file_mp3, preallocateBuffer, preallocateBufferSize);
  mp3 = new AudioGeneratorMP3(preallocateCodec, preallocateCodecSize);
  mp3->begin(buff, out);
  out->SetGain(((float)beep_mp3_vol * 4) / 100.0);

  while (mp3->isRunning())
  {
    if (mp3->isRunning()) {
      if (!mp3->loop()) mp3->stop();
    } else break;
  }

  StopPlaying();

  do_pin_off_play(2);

#ifdef debug
  Serial.print(F("end ")); Serial.println(filename);
#endif
}


//=========================================================================
//                    do_play_mp3
//=========================================================================
void  do_play_mp3()
{

  if (mp3)
    if (mp3->isRunning())

      if (!mp3->loop())
      {
        Serial.println("!mp3->loop()");
        StopPlaying();
#ifdef pin_sd
        if (play_mp3) do_play_track(1); //end, next
#endif
      }




}




//=========================================================================
//                    do_create_mp3
//=========================================================================

void do_create_mp3() //0-beep 1 play_mp3
{
  bool f_open = false;

#ifdef pin_sd
  Serial.println(" do_create_mp3 SD ");
  file_mp3 = new AudioFileSourceSD();
#else
  Serial.println("do_create_mp3 Spiffs ");
  file_mp3 = new AudioFileSourceSPIFFS;
#endif

  id3 = new AudioFileSourceID3(file_mp3);
#ifdef pin_sd
  id3->RegisterMetadataCB(MDCallback_mp3, (void*)"ID3TAG");
#endif

#ifdef pin_sd
  String track_name_str = "/mp3/" + track_name + ".mp3";
  const char * track = track_name_str.c_str();
  f_open = file_mp3->open(track);
#ifdef debug
  //  Serial.print(" track_name_str="); Serial.println(track_name_str);
  Serial.print(" do_create_mp3 track="); Serial.println(track);
#endif

#endif

  if (!f_open)
  {
    Serial.println(F(" Can't open file mp3"));
    StopPlaying(); // in StopPlaying do_play_track(1);
    return;
  }


  mp3 = new AudioGeneratorMP3(preallocateCodec, preallocateCodecSize);
  mp3->begin(id3, out);

  if (!mp3->isRunning()) {
    Serial.println(F("Can't play mp3"));
    StopPlaying();
    //ERR next()
#ifdef pin_sd
    do_play_track(1);
#endif
    return;
  }



#ifdef pin_sd
  set_vol_mp3();
#endif
#ifdef set_audio_mono
  out->SetOutputModeMono(true) ;
#endif

}
#endif  //audio_lib2
#endif //32
//----------------------------32




//======================================================================================
//                    do_fav_ch
//======================================================================================
#if defined (fmSi4703)|| defined (fmRDA5807)||defined (ESP32)|| defined (fmSi4703_lib2)
void do_fav_ch(String str, byte www_fm) //0-www 1-fm
{
  String s;

  if (www_fm == 0)
    fav_count_real = 0;
#if defined (fmSi4703)|| defined (fmRDA5807)|| defined (fmSi4703_lib2)
  if (www_fm == 1)
    fav_count_real_fm = 0;
#endif  //fm

  //{1;4;13;20;}
  int val = -1;
  int tz = str.indexOf("{");
  int tz2 = str.indexOf("}");
  s = str.substring(tz + 1, tz2)  ;
#ifdef debug
  Serial.print("do_fav_ch--------->");
  Serial.println(s);
#endif
  tz++;
  tz2 = tz;
  tz2 = str.indexOf(";", tz + 1);
  //-----------------------------------
  while (tz2 > 0)
  {
    s = str.substring(tz, tz2);
    //   Serial.print("s=");
    //  Serial.println(s);
    val = s.toInt();
    //   Serial.print("val=");

    if (val > 0 && val < 256)
    {
      //    Serial.println(val);
      if (www_fm == 0) //www
      {
        fav_ch[fav_count_real] = val;
        fav_count_real++;
        if (fav_count_real >= fav_count) break;
      }

#if defined (fmSi4703)|| defined (fmRDA5807)|| defined (fmSi4703_lib2)
      if (www_fm == 1) //fm
      {
        fav_ch_fm[fav_count_real_fm] = val;
        fav_count_real_fm++;
        if (fav_count_real_fm >= fav_count_fm) break;
      }
#endif //FM

    }
    else
    {
      Serial.print(" ERR=");  Serial.println(val);
    }

    tz = tz2 + 1;
    tz2 = str.indexOf(";", tz);
    //    Serial.println(tz); Serial.println(tz2);
  }   //wh

}

#endif //FM ,esp32 





//--------------------DRAW------------------------------------------
//--------------------------------------------------------------------
//        draw_tr_up
//--------------------------------------------------------------------
void draw_tr_up(int x, int y)
{
#ifdef tft_320_240
#ifdef two_tft
  if (tft2_act)
    tft2.drawTriangle(x, y, x + 6, y, x + 3, y - 10, TFT_YELLOW);
  else
    tft.drawTriangle(x, y, x + 6, y, x + 3, y - 10, TFT_YELLOW);
#else
  tft.drawTriangle(x, y, x + 6, y, x + 3, y - 10, TFT_YELLOW);
#endif
#endif

#ifdef tft_480_320
#ifdef two_tft
  if (tft2_act)
    tft2.drawTriangle(x, y, x + 6, y, x + 3, y - 12, TFT_YELLOW);
  else
    tft.drawTriangle(x, y, x + 6, y, x + 3, y - 12, TFT_YELLOW);
#else
  tft.drawTriangle(x, y, x + 6, y, x + 3, y - 12, TFT_YELLOW);
#endif
#endif
}


//--------------------------------------------------------------------
//        draw_tr_down
//--------------------------------------------------------------------
void draw_tr_down(int x, int y)
{
#ifdef tft_320_240
#ifdef two_tft
  if (tft2_act)
    tft2.drawTriangle(x + 3, y, x, y - 10, x + 6, y - 10, TFT_CYAN);
  else
    tft.drawTriangle(x + 3, y, x, y - 10, x + 6, y - 10, TFT_CYAN);
#else
  tft.drawTriangle(x + 3, y, x, y - 10, x + 6, y - 10, TFT_CYAN);
#endif
#endif


#ifdef tft_480_320
#ifdef two_tft
  if (tft2_act)
    tft2.drawTriangle(x + 3, y, x, y - 12, x + 6, y - 12, TFT_CYAN);
  else
    tft.drawTriangle(x + 3, y, x, y - 12, x + 6, y - 12, TFT_CYAN);
#else
  tft.drawTriangle(x + 3, y, x, y - 12, x + 6, y - 12, TFT_CYAN);
#endif
#endif

}









//=====================================================================
//                            draw_next
//=====================================================================
void draw_next()
{
  if (play || play_fm || play_mp3) tft.drawTriangle(120, 65, 105, 60, 105, 70, TFT_YELLOW);
  else
    tft.drawTriangle(20, 10, 5, 5, 5, 15, TFT_YELLOW);

}



//=====================================================================
//                            draw_prev
//=====================================================================

void draw_prev()
{
  if  (play || play_fm || play_mp3) tft.drawTriangle(120, 60, 120, 70, 105, 65, TFT_YELLOW);
  else
    tft.drawTriangle(20, 5, 20, 15, 5, 10, TFT_YELLOW);
}





//=====================================================================
//                            draw_stop
//=====================================================================

void  draw_stop()
{

  //  tft.fillCircle(120, 85, 4, TFT_RED);
  tft.fillCircle(120, 65, 4, TFT_RED);

}



//=====================================================================
//                            draw_play
//=====================================================================
#if defined( ESP32)
void  draw_play() //на главном экране
{
  if (play)
    tft.fillCircle(135, 20, 4, c_www);
  else
  {
    tft.setFreeFont(v9mono);
    tft.setCursor(0, 10);
    tft.setTextColor( c_www);
    tft.print("WWW");

  }

}

#endif

//=====================================================================
//                            draw_play_fm
//=====================================================================
#if defined (fmSi4703)|| defined (fmRDA5807)|| defined (fmSi4703_lib2)
void  draw_play_fm() //на главном экране
{
  if (play_fm) return;
  tft.setFreeFont(v9mono);
  tft.setCursor(0, 10);
  tft.setTextColor(c_fm);
  tft.print("FM");

}

#endif



//=====================================================================
//                            draw_play_mp3
//=====================================================================
#if defined( pin_sd)&&defined( ESP32)
void  draw_play_mp3() //на главном экране
{
  if (play_mp3) return;
  tft.setFreeFont(v9mono);
  tft.setCursor(0, 10);
  tft.setTextColor(c_mp3);
  tft.print("MP3");


}

#endif
//=====================================================================
//                            draw_stop_round
//=====================================================================
void draw_stop_round()
{

#ifdef tft_320_240
  if (scr_round)
    tft.fillTriangle(300, 0, 319, 0, 319, 10, TFT_BLACK);
  else
    tft.fillTriangle(300, 0, 319, 0, 319, 10, TFT_RED);
#endif



#ifdef tft_480_320
  if (scr_round)
    tft.fillTriangle(460, 0, 479, 0, 479, 10, TFT_BLACK);
  else
    tft.fillTriangle(460, 0, 479, 0, 479, 10, TFT_RED);
#endif



}



//=====================================================================
//                            draw_num
//=====================================================================
void draw_num(int b, char c)
{
  if (play && play_bg) return;
  int color;
  byte x, y;
  if (c == 'T') //отрисовка таймера
    color = TFT_RED; //timer
  else
    color = TFT_WHITE; //ch



  if ((play || play_fm || play_mp3 ) && c == 'T')
  {
    tft.fillRect(5, 58, 35, 14, TFT_BLACK);
    x = 5, y = 70;
  }
  else if ((play || play_fm || play_mp3) && c == 'C')
  {
    x = 95, y = 70;
    tft.fillRect(95, 58, 32, 14, TFT_BLACK);
  }
  else if (!play && !play_fm && !play_mp3) //все остальные экраны
  {
    x = 0, y = 15;
    tft.fillRect(0, 0, 35, 20, TFT_BLACK);
    //tft.drawRoundRect(x,y,30,20,3,color);
  }


  if (b == 0) return; //просто стереть цифры

  tft.setTextColor(color);
  tft.setFreeFont(v9);
  tft.setCursor(x, y);
  tft.print(b);
}



//=====================================================================
//                             draw_alarm()
//=====================================================================

void  draw_alarm()
{
  int color = TFT_BLACK;
  if (t_timer_alarm > 0)
  {
    if (play) color = c_www;
#if defined (fmSi4703)|| defined (fmRDA5807)|| defined (fmSi4703_lib2)
    if (play_fm) color = c_fm;
#endif
  }
  tft.setFreeFont(v6);
  tft.setTextColor(color);
  tft.setCursor(0, 165);
  tft.print("Tm");
}


//=====================================================================
//                             do_pin_on_play()
//=====================================================================

void  do_pin_on_play(byte id) //0-WWW,MP3  1-WWW 2 -MP  3 - FM  99-all
{
  byte id_ = id;

  if (id_ == 1)
  {
#if defined (pin_play_WWW_out_hight)
    digitalWrite(pin_play_WWW_out_hight, HIGH);
#endif

#if defined (pin_play_WWW_out_low)
    digitalWrite(pin_play_WWW_out_low, LOW);
#endif
    id_ = 0; //www_mp3
  }

  if (id_ == 2)
  {
#if defined (pin_play_MP3_out_hight)
    digitalWrite(pin_play_MP3_out_hight, HIGH);
#endif
#if defined (pin_play_MP3_out_low)
    digitalWrite(pin_play_MP3_out_low, LOW);
#endif
    id_ = 0; //www_mp3
  }

  if (id_ == 3)
  {
#if defined (pin_playFM_out_hight)
    digitalWrite(pin_playFM_out_hight, HIGH);
#endif
#if defined (pin_playFM_out_low)
    digitalWrite(pin_playFM_out_low, LOW);
#endif

  }

  if (id_ == 0)
  {
#if defined (pin_play_WWW_MP3_out_hight)
    digitalWrite(pin_play_WWW_MP3_out_hight, HIGH);
#endif
#if defined (pin_play_WWW_MP3_out_low)
    digitalWrite(pin_play_WWW_MP3_out_low, LOW);
#endif
  }


#if    defined (pin_playALL_out_hight)
  digitalWrite(pin_playALL_out_hight, HIGH);
#endif
#if    defined (pin_playALL_out_low)
  digitalWrite(pin_playALL_out_low, LOW);
#endif



}


//=====================================================================
//                             do_pin_off_play()
//=====================================================================

void  do_pin_off_play(byte id) //0-WWW,MP3  1-WWW 2 -MP  3 - FM
{
  byte id_ = id;

  if (id_ == 1)
  {
#if defined (pin_play_WWW_out_hight)
    digitalWrite(pin_play_WWW_out_hight, LOW);
#endif
#if defined (pin_play_WWW_out_low)
    digitalWrite(pin_play_WWW_out_low, HIGH);
#endif
    id_ = 0; //www_mp3
  }


  if (id_ == 2)
  {
#if defined (pin_play_MP3_out_hight)
    digitalWrite(pin_play_MP3_out_hight, LOW);
#endif
#if defined (pin_play_MP3_out_low)
    digitalWrite(pin_play_MP3_out_low, HIGH);
#endif
    id_ = 0; //www_mp3
  }




  if (id_ == 3)
  {
#if defined (pin_playFM_out_hight)
    digitalWrite(pin_playFM_out_hight, LOW );
#endif
#if defined (pin_playFM_out_low)
    digitalWrite(pin_playFM_out_low, HIGH);
#endif

  }


  if (id_ == 0) //www mp3
  {
#if defined (pin_play_WWW_MP3_out_hight)
    digitalWrite(pin_play_WWW_MP3_out_hight, LOW);
#endif
#if defined (pin_play_WWW_MP3_out_low)
    digitalWrite(pin_play_WWW_MP3_out_low, HIGH);
#endif
  }


  //all
#if    defined (pin_playALL_out_hight)
  digitalWrite(pin_playALL_out_hight, LOW );
#endif
#if    defined (pin_playALL_out_low)
  digitalWrite(pin_playALL_out_low, HIGH );
#endif



}
