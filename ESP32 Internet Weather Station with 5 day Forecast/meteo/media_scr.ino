#if defined (ESP32) && !defined(no_tft)
//===============================================================================================================
//                  do_scr_play()
//===============================================================================================================

void do_scr_play()
{
  if (play_bg) return;

  static bool logo_on_scr = false;
  static byte ch_tmp = 254; //init ch
  static time_t t_pause_song = 0;
  //////////////////////////////////////////
  if (refresh_all) //первое вкл экрана
  {
    ch_tmp = 254;
    show_song = true;
#ifdef tft_480_320
    //   tft.drawFastHLine(0, 50, 479, c_www); //под песней
    tft.fillRect(0, 47, 479, 3, c_www) ; //ch
    tft.drawFastHLine(130, 75, 349, c_www) ; //ch
#endif
#ifdef tft_320_240
    //    tft.drawFastHLine(0, 50, 319, c_www); //под песней
    tft.fillRect(0, 47, 319, 2, c_www) ; //ch
    tft.drawFastHLine(130, 75, 189, c_www) ; //ch
#endif

    tft.drawFastVLine(130, 50, 150, c_www);
#ifdef tft_480_320
    tft.drawFastHLine(0, 200, 130, c_www) ; //под часами
#endif
#ifdef tft_320_240
    tft.drawFastHLine(0, 200, 320, c_www) ; //под часами
#endif
    do_draw_clock();
    draw_mem();
    draw_vol();

  }


#ifdef ref_sensor_on_play //ref sensors
  if ( ref_kv_th || ref_temp_u)
    if (ch_tmp == ch)
    {
      Serial.println(F("ref_kv_th||ref_temp_u"));
#if defined(tft_320_240)&& defined (openw)
      do_show_th_play_ow();
#elif defined(tft_320_240)
      do_show_th_no_ow();
#elif defined(tft_480_320)
      do_show_th_n();
#endif

    }

#endif //ref_sensor_on_play




  //канал смена ccccccccccccccccccccccccccc
  if (ch_tmp != ch)
  {
    //    Serial.println("ch_tmp!=ch");
#if defined(tft_320_240)
    tft.fillRect(131, 51, 188, 23, TFT_BLACK); //ch
#endif
#if defined(tft_480_320)
    tft.fillRect(131, 51, 348, 23, TFT_BLACK); //ch
#endif


    if (play)
    {
      tft.setFreeFont(v9mono);
      tft.setCursor(134, 68);
      tft.setTextColor(TFT_WHITE);
      if (ch_name.length() > 20)  tft.setFreeFont(v6);
      tft.print(ch); tft.print(F("."));
      tft.setTextColor(c_ch_fnd);
      tft.print(ch_name);
#ifdef tft_480_320
      if  (logo_on_scr)
      {
        tft.fillRect(0, 210, 96, 32, TFT_BLACK);
        logo_on_scr = false;
      }
      if (logo_www != "")
      {
        drawJpeg(logo_www, 0, 210);
        logo_on_scr = true;
      }

#endif
      draw_vol();
      ch_tmp = ch;
    }

    draw_mem();



#if defined(tft_320_240)&& defined (openw)
    do_show_th_play_ow();
#elif defined(tft_320_240)
    do_show_th_no_ow();
#elif defined(tft_480_320)
    do_show_th_n();
#endif

    //pause show song
    //if (buff_fill != 0)
    t_pause_song = millis() + 7000;
    new_song = true;
#if defined (audio_lib2) ||defined (vs1053)
    draw_buff(true);
#endif
  }

  //--ssssssssssssssssssssssssssssssssssssssssssssss
  //что играет
  if (play && millis() > t_pause_song && (new_song || b_draw_song))
  {

    //    tft.loadFont( "font9");
    do_draw_song();
    //    tft.unloadFont();

  }
  //hh:mm
  if (!b_draw_song || new_song || refresh_all)
  {
    do_draw_h_m(); //перед титрами обновить часы
  }

  //draw fill buffer
#if defined (audio_lib2) ||defined (vs1053)
  if (playing) draw_buff(false);
#endif
}


//==============================================================
//              draw_buff()
//==============================================================
#if defined (audio_lib2) ||defined (vs1053)

void draw_buff(bool ref)
{
  uint16_t color ;
  static uint32_t t_next = 0;
  static uint32_t buff_fill_tmp = 0;
  static bool draw_codec = false;

  if (ref)
  {
    buff_fill_tmp = 0;
    draw_codec = false;

  }
  else if (t_next > millis()) return;

  if (!ref)   t_next = millis() + 15000;
  else
    t_next = millis() + 2000;

#ifdef def_core0
  buff_fill = audioInbuffFilled();
#else
#ifdef audio_lib2
  buff_fill = audio.inBufferFilled();
#else
#ifdef vs1053
  audio.bufferFilled();
#endif
#endif
#endif
  if (buff_fill_tmp == buff_fill && !ref) return;

  buff_fill_tmp = buff_fill;



#ifdef def_core0
  buff_free = audioInbuffFree();
#else
#ifdef audio_lib2
  buff_free = audio.inBufferFree();
#else
#ifdef vs1053
  audio.bufferFree();
#endif
#endif
#endif
#ifdef debug
  Serial.print("buff_fill: ");  Serial.println(buff_fill);
  Serial.print("buff_free: ");  Serial.println(buff_free);
#endif



  tft.fillRect(78, 199 - 30, 6, 30, TFT_BLACK);
  if (buff_fill > 3000) color = TFT_GREEN;
  else if (buff_fill > 1000) color = TFT_YELLOW;
  else
    color = TFT_RED;

  tft.drawRect(78, 199 - 30, 6, 30, color);
  if (buff_fill > buff_free ) color = TFT_GREEN;
  else if (buff_fill > 3000) color = TFT_YELLOW;
  else color = TFT_RED;

  int fill_line = 100 * buff_fill / (buff_fill + buff_free);
  byte fill_line_map = map(fill_line, 0, 100 , 0, 30);
  tft.fillRect(79, 169 + (30 - fill_line_map), 4, fill_line_map, color);



#ifdef tft_480_320  //codec bitr
  if (draw_codec) return;
  if (buff_fill == 0)  return;
#ifdef audio_lib2
  tft.setFreeFont(v9mono);
  tft.setCursor(350, 68);
  tft.setTextColor(TFT_CYAN); tft.print("(");
  tft.setTextColor(TFT_GREEN); tft.print( audio.getCodecname());
  tft.setTextColor(TFT_WHITE);
  tft.print("-"); tft.print( int( audio.getBitRate() / 1000));
  tft.setTextColor(TFT_CYAN); tft.print(")");
  draw_codec = true;
#endif //lib2

#endif //tft_480_320


}
#endif

//==============================================================
//              do_draw_song()
//==============================================================

void do_draw_song()
{
  static  uint16_t color = c_gr;
  uint16_t color_tmp = 0; //color one only char
  static byte count_ch_line_tmp = 0;
  static bool ch_34 = false; //34 "

  static bool rec_fill = true;
  static byte col_tft = 0;
  static int play_ch = 0;
  static byte x1 = 0;
  static byte y1 = 0;
  static String play_song_loc = "";
  static int play_song_len = 0;
  static unsigned long t_ref_song = 0;
  bool err = false;

#ifdef tft_320_240
  const  byte count_ch_line = 40;
  const  byte x_step = 8;
  const  byte y_step = 18;
  //  const byte  song_length = 80; // count_ch_line * 2;
  const byte rect_count = 32; // 320/x_step
#endif
#ifdef tft_480_320
  const  byte  count_ch_line = 40;
  //  const byte  song_length = 80; // count_ch_line * 2;
  const byte rect_count = 48;   //480/x_step
  const  byte x_step = 12;
  const  byte y_step = 20;
#endif

#ifdef tft_400_300
  const  byte count_ch_line = 40;
  const  byte x_step = 8;
  const  byte y_step = 18;
  //  const byte  song_length = 80; // count_ch_line * 2;
  const byte rect_count = 32; // 320/x_step
#endif


  if (millis() < t_ref_song) return;
  if (new_song == false && b_draw_song == false ) return;


  if (new_song == true) //b_draw_song = false; //сброс начинаю рисовать с нуля

    //  if (new_song == true) begin_draw = false; //сброс начинаю рисовать с нуля

    //------------------------------------------INIT----------------------------
    //------------------------------------------INIT----------------------------
    // if (begin_draw == false)
    //   if (new_song == true)
  {
    count_ch_line_tmp = 0;
    color = c_gr;
    rec_fill = true;
    if (play)
    {
      if (   play_err != "")
      {
        play_song_loc = play_err;
        err = true;
      }
      else
      {
        play_song_loc = play_song;
      }
    }
#ifdef pin_sd
    else if (play_mp3)     play_song_loc = play_song_mp3 + " - " + play_song_g_mp3;
#endif
    //    else if (play_fm)     play_song_loc = play_song_fm;

    play_song_len = play_song_loc.length();
    new_song = false;
    b_draw_song = true;
    col_tft = 0;
    play_ch = 0;
    x1 = 0;
    y1 = 0;

    /*
      #ifdef debug
        Serial.print("play_song_loc=");
        Serial.println(play_song_loc);
        Serial.print("play_song_len=");
        Serial.println(play_song_len);
      #endif
    */
  }//init
  //EEEE------------------------------------------INIT

  if (show_song == false) return;
  //----------------------------------------FILL-----------------
  if (x1 < count_ch_line && rec_fill) //закраска
  {
    tft.fillRect(x1 * x_step, y1, x_step, y_step , TFT_BLACK); //TFT_BLACK

    //    if (x1 == count_ch_line - 1)       tft.fillRect(x1 * x_step, y1, x_step, y_step + 3 , TFT_YELLOW); //TFT_BLACK

    if (!err)  t_ref_song = millis() + 100;
    x1++;
    //   Serial.print(x1);  Serial.print(y1);
    rec_fill = false;
    return;
  }


  if (!err)  t_ref_song = millis() + t_ref_song_period;

#ifdef tft_320_240
  tft.setFreeFont(v6);
  play_song_loc.toUpperCase();
#endif
#ifdef  tft_480_320
  tft.setFreeFont(v9mono);
#endif

  //группа
  // if (play_ch == 0) col_tft = 0; //init
  rec_fill = true;
  //----------------------------------SONG----------------------------
  while  ( count_ch_line_tmp < count_ch_line * 2) //две строки
  {
    while (play_ch < play_song_len)
    {
      tft.setCursor(col_tft * x_step, y1 + y_step - 4);
      tft.setTextColor(color);
      int int_play_ch = (int)play_song_loc.charAt(play_ch);


      if (int_play_ch > 207 && int_play_ch < 210) //RU
      {
        tft.print(play_song_loc.substring(play_ch, play_ch + 2));
        //       Serial.print("play_g_ch_subst ");
        //        Serial.println(play_song_loc.substring(play_ch, play_ch + 2));
        //        Serial.print("play_ch ");  Serial.println(play_song_loc.charAt(play_ch));
        //        Serial.print("play_ch_hex ");  Serial.println(int_play_ch, HEX);
        //       Serial.print("play_ch_hex +");  Serial.println(play_song_loc.charAt(play_ch + 1), HEX);
        play_ch++;
      }
      else if ( int_play_ch < 128) //EN
      {
        if (int_play_ch == 45) //-
        {
          color = c_song; //-
          color_tmp = TFT_WHITE;
        }
        else if (int_play_ch == 34 && ch_34 == false)  //"
        {
          color = TFT_WHITE; // "
          ch_34 = true; //open "
          color_tmp = 0x07E0;
        }
        else if (int_play_ch == 34 && ch_34 == true)  //"
        {
          color = c_gr; //"
          ch_34 = false; //close "
          color_tmp = 0x07E0;
        }
        else if (int_play_ch == 40 || int_play_ch == 41)  //(  )
        {
          color_tmp = 0x07E0; //( or )
        }

        if (color_tmp != 0)   tft.setTextColor(color_tmp);
        else
          tft.setTextColor(color); //-


        tft.print(play_song_loc.substring(play_ch, play_ch + 1));


        //        Serial.print("play_ch_EN ");  Serial.println(int_play_ch);
        //        Serial.print("play_ch_hex ");  Serial.println(int_play_ch, HEX);

      }

      play_ch ++;
      col_tft++;
      break;
    }
    count_ch_line_tmp++;
    //   Serial.print("count_ch_line_tmp ");  Serial.println(count_ch_line_tmp);
    if (count_ch_line_tmp >= count_ch_line && y1 != 20) //wrap
    {
      //     Serial.println("col_tft break line");
      col_tft = 0;
      x1 = 0; //fill 2 line
      y1 = 20; //fill 2 line
    }

    return; //fiil rect
  } //for

  //-----------------------mp3
#ifdef pin_sd
  //Serial.println("play_album_mp3 show");
  if (play_mp3)
  {
    if (play_album_mp3 != "")
    {
      //     tft.setFreeFont(v9mono);
      tft.setCursor(144, 105);
      tft.setTextColor(TFT_WHITE);
      tft.print("ALBUM");
      tft.setTextColor(TFT_CYAN);
      tft.setCursor(144, 130);

#ifdef tft_480_320
      if (play_album_mp3.length() > 31)  tft.setFreeFont(v6);
      tft.setCursor(135, 130);
#endif
#ifdef tft_320_240
      if (play_album_mp3.length() > 16)  tft.setFreeFont(v6);
      tft.setCursor(135, 130);
#endif

      tft.print(play_album_mp3);

    }


    //Serial.println("play_year_mp3 show");
    if (play_year_mp3 != "")
    {
      tft.setFreeFont(v9mono);
      // tft.setTextColor(TFT_YELLOW);
      tft.setCursor(134, 160);
      tft.setTextColor(TFT_WHITE);
      tft.print("YEAR   ");

      tft.setTextColor(c_mp3);
      tft.print(play_year_mp3);

    }
  } //mp3
#endif //sd

  b_draw_song = false; //
  //  Serial.println("b_draw_song = false");
}




//=====================================================================
//                            draw_vol
//=====================================================================
void draw_vol()
{
  if (play_bg) return;

  tft.fillRect(123, 99, 6, 100, TFT_BLACK) ;
  int color;
  int vol_tmp = 0;

  if (play) vol_tmp = volume;
  if (play_mp3) vol_tmp = volume_mp3;

  if (vol_tmp > 90) color = TFT_MAGENTA;
  else if (vol_tmp > 70) color = TFT_YELLOW;
  else color = TFT_GREEN;



  tft.drawFastVLine(125, 199 - vol_tmp, vol_tmp, color);
  tft.drawFastVLine(127, 199 - vol_tmp, vol_tmp, color);


  //------------
  tft.fillRect(93, 180, 30, 16, TFT_BLACK);
  tft.setFreeFont(v9mono);

  if (vol_tmp > 0)
  {
    tft.setTextColor(color);
    tft.setCursor(94, 195);
    tft.print(vol_tmp);
  }
  else
  {
    tft.setTextColor(TFT_RED);
    tft.setCursor(94, 195);
    tft.print("M");
  }


}



//=====================================================================
//                            draw_no_song
//=====================================================================
void draw_no_song()
{
  show_song = !show_song;

#ifdef tft_320_240
  if (show_song)
    tft.fillTriangle(300, 0, 319, 0, 319, 10, TFT_BLACK);
  else
    tft.fillTriangle(300, 0, 319, 0, 319, 10, TFT_RED);
#endif



#ifdef tft_480_320
  if (show_song)
    tft.fillTriangle(460, 0, 479, 0, 479, 10, TFT_BLACK);
  else
    tft.fillTriangle(460, 0, 479, 0, 479, 10, TFT_RED);
#endif



}





#endif //ESP32--------------------------------








//====================================================================
//                      do_show_th_play_ow openw
//====================================================================
#if defined(tft_320_240)&& defined (openw)
void do_show_th_play_ow()
{
  //  Serial.println("do_show_th_play openW");
  int  temp;
  if (refresh_all)  tft.fillRect(0, 201, 320, 38, TFT_BLACK);


  temp = temperatura_ch[0];
  if (temp_u != 200) temp = temp_u;
  tft.drawFastVLine(113, 200, 39, c_tu_frame) ;
  ///ttttttttttttttttttttttttttttt
  if (temp != 200)
  {
    tft.setFreeFont(v18);
    tft.setTextColor(color_t(temp));
    if (!refresh_all) tft.fillRect(160, 202, 70, 38, TFT_BLACK);
    tft.setCursor(160, 235);
    tft.print(temp);
  }

  //hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh
  S_show_th_room(272, 202);


  if (!refresh_all) return;



  String st = "";
  if (day(t) < 10)
    st = "0" + String(day(t));
  else
    st = String(day(t));

  tft.setFreeFont(v12);
  tft.setTextColor(c_date);
  tft.setCursor(0, 230);
  tft.print(st);

  if (month(t) < 10)
    st = "0" + String(month(t));
  else
    st = String(month(t));
  st = "." + st;
  tft.setFreeFont(v9);
  tft.setTextColor(c_month);
  tft.print(st);

  //weekwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww

  draw_week_day(63, 207);



  //rrrrrrrrrrrrrrrrrrrrrrrrrrrrr
  do_draw_rain(120, 210, rain_ch[0], TFT_CYAN);

  //icon
  drawJpeg(icon_ch[0] + "32.jpg", 125, 205); // текущая погода

  //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
  int gr = int(v_gr_ch[0]);
  do_deg(gr);
  tft.drawFastVLine(229, 200, 39, TFT_LIGHTGREY) ;
  drawJpeg(wind_ico, 230, 205);
  tft.setFreeFont(v6);
  tft.setCursor(254, 238);
  tft.setTextColor(TFT_CYAN);
  tft.print(v_ch[0]);


}
#endif //openw




#if defined(ESP32)||  defined (fmSi4703)|| defined (fmRDA5807) ||defined(pin_sd)

//=====================================================================
//                            draw_mem
//=====================================================================
void draw_mem()
{
  //mem
  tft.setFreeFont(v6);
  uint32_t mem_esp = ESP.getFreeHeap();
#ifdef ESP32
  const int m_max = 100000;
  const int m_mid = 50000;
#endif //32

#ifdef ESP8266
  const int m_max = 25000;
  const int m_mid = 10000;
#endif //32


  int color;
  if (mem_esp > m_max) color = TFT_GREEN;
  else if (mem_esp > m_mid) color = TFT_YELLOW;
  else if (mem_esp > 0) color = TFT_RED;
  tft.fillRect(0, 185, 90, 12, TFT_BLACK); //mem
  tft.setCursor(0, 194);
  tft.setTextColor(TFT_DARKGREY);
  tft.print("mem=");
  tft.setTextColor(color);
  tft.print(String(mem_esp));

}




//=============================================================
//                          do_draw_h_m()
//=============================================================

void do_draw_h_m()
{
  //static unsigned long next_mill=0;
  static byte last_min = 0;
  static byte last_hour = 0;
  static byte  a_m, a_ch;


  if (last_min == now_min && !refresh_all) {
    return;
  }
  draw_alarm();
  do_show_min(last_min, TFT_BLACK);
  do_show_min(now_min, TFT_CYAN);

  //////////////hour
  do_show_hour(last_hour, last_min, TFT_BLACK);
  do_show_hour(now_hour, now_min, TFT_YELLOW);


  last_min = now_min;
  last_hour = now_hour;

  if (refresh_all)
  {
    a_m = 99; a_ch = 0;
  }

  byte t_next = 99;
  byte ch_next = 0;

  if (al.m > last_min) {
    t_next = al.m;  //еще не стартовал план.
    ch_next = al.ch_num;
  }
  else if (t_next_shed != 99) {
    t_next = t_next_shed;  //есть след план.
    ch_next = next_shed_ch;
  }
  else
  {
    t_next = 99;
    ch_next = 0;
  }

  if (a_m != t_next && t_next != 99)
  {
    a_m = t_next; a_ch = ch_next;
    tft.setFreeFont(v6);
    tft.fillRect(0, 170, 60, 12, TFT_BLACK);

    tft.setTextColor(TFT_WHITE);
    tft.setCursor(0, 181);
    if (last_hour < 10) tft.print("0");
    tft.print(last_hour); tft.print(":");
    if (a_m < 10) tft.print("0");
    tft.print(a_m);
    tft.print(" ");
    if (next_label == 'i' || next_label == 'I')
      tft.setTextColor(c_www);
#if defined (fmSi4703)|| defined (fmRDA5807)
    if (next_label == 'f' || next_label == 'F')
      tft.setTextColor(c_fm);
#endif
    tft.print(a_ch);

  }


  if (last_min == 0 && last_hour == 0) //начало суток обн дату
  {
    refresh_all = true;

#if defined(tft_320_240)&& defined (openw)
    do_show_th_play_ow();
#elif defined(tft_320_240)
    do_show_th_no_ow();
#elif defined(tft_480_320)
    do_show_th_n();
#endif

    refresh_all = false;
  }


}



void do_draw_clock()
{

  float  alpha = 0;
  float  x, y;
  int color;
  byte rr ;
  for (int i = 1; i < 3; i++) tft.drawCircle(CX, CY, R + i, TFT_DARKGREY);
  //point
  for (int i = 0; i < 12; i++)
  {
    alpha = (30 * i + 270) * 0.0175;
    x = CX + (R + 3) * cos(alpha);
    y = CY + (R + 3) * sin(alpha);
    if (i % 3 == 0)
    {
      color = TFT_YELLOW;
      rr = 3;
    }
    else
    {
      color = TFT_CYAN;
      rr = 2;
    }
    tft.fillCircle(x, y, rr, color);
  }


  tft.setTextColor(TFT_CYAN);
  tft.setFreeFont(v9);
  tft.setCursor(CX + R + 7, CY + 8);
  tft.print("3");
  tft.setCursor(CX - 2, CY + R + 20);
  tft.print("6");
  tft.setCursor(CX - R - 20, CY + 8);
  tft.print("9");
  tft.setCursor(CX - 10, CY - R - 10);
  tft.print("12");

}



/////////mmmmmmmmmmmmmmmmmmmmm-------------
void do_show_min(byte num, int col)
{

  float x, y, x1, y1;
  float alpha = (6 * num + 270) * 0.0175;

  x = CX + (R * 0.1) * cos(alpha);
  y = CY + (R * 0.1) * sin(alpha);

  x1 = CX + (R * 0.9)  * cos(alpha);
  y1 = CY + (R * 0.9) * sin(alpha);


  //ctrelka
  //tft.drawLine(CX,CY,x,y,col);
  tft.drawLine(x, y, x1, y1, col);


}




//--------------hhhhhhhhhhhhhhhhhhhhhhhhh
void do_show_hour(byte h, byte m, int col)
{
  float x, y;
  float alpha = (30 * h + (m / 2) + 270) * 0.0175;


  x = CX + (R * 0.5) * cos(alpha);
  y = CY + (R * 0.5) * sin(alpha);

  //hhhhhhhhh
  tft.drawLine(CX, CY, x, y, col);
}


#endif  //ESP32 + tft


#ifdef ESP32
//=====================================================================
//                            do_mute
//=====================================================================
void do_mute()
{
  static int volume_tmp; //до mute
  static bool mute = false;

  if (refresh_all) //reset mute;
  {
    mute = false;
    //   Serial.println("do_mute() init");
    return;
  }

  if (!mute)//mute on
  {
    volume_tmp = volume;
    volume = 0;
  }
  else
    volume = volume_tmp;

  mute = !mute;
  set_vol_www();

}
#endif  //ESP32
