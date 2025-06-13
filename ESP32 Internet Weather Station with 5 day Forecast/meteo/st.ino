#if !defined(tft_400_300) &&  !defined(no_tft)
void do_scr_timer()
{

#if (defined(tft_480_320)&& defined (openw)&& defined (S3)) ||(defined(tft_320_240)&& defined (openw)&& defined (S5))

  static byte col = 1;
  static byte h_tmp = 0;
  static byte m_tmp = 0;
  //int h1, h2, m1, m2;

  if (refresh_all)
  {
    //  tft.drawRect(0, 0, 320, 240, TFT_YELLOW);
    init_timer();

    col = 1; //2,3-h,3,4-min
    h_timer = 0;
    m_timer = 0;
    h_tmp = 0;
    m_tmp = 0;
    h1 = 0, h2 = 0, m1 = 0, m2 = 0;
#ifdef tft_480_320
    tft.drawFastHLine(0, 80, 480, TFT_WHITE) ;
    tft.drawFastHLine(0, 81, 480, TFT_WHITE) ;
    //   tft.drawFastVLine(col_6, y_begin, 320 - y_begin, TFT_CYAN) ;
#endif
    do_read_timer_txt();

    //   drawJpeg("/exit.jpg", col_6 + 17, y_jpeg + 20); //220
  }
  //------------------------
  //BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
  if (t_timer_scr == 0 && fn_cmd.substring(0, 1) == "b" && col < 6 )
  {
    byte col_tmp = col;
    String s_num = fn_cmd.substring(1);
    Serial.print(F("s_num=")); Serial.println(s_num);
    int n = s_num.toInt();
    Serial.print(F("n=")); Serial.println(n);
    if (col == 1) do_read_timer_txt();
    else if (col == 2) {
      h1 = n;
      col++;
    }
    else if (col == 3) {
      h2 = n;
      col++;
    }
    else if (col == 4)
    {
      if (n < 6) {
        m1 = n;
        col++;
      }
    }
    else if (col == 5)
    {
      m2 = n;
    }

#ifdef debug
    Serial.print(F("fn_cmd.toInt=")); Serial.println(n);
#endif
    draw_timer();
    if (col_tmp != col)  draw_col(col);
  }
  //VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
  else if (t_timer_scr == 0 && (fn_cmd == "vl+" || fn_cmd == "vl-"))
  {

    if (col == 1) //file //11111111111111111111111111111
    {
      do_read_timer_txt();
    }


    if (col == 2) //22222222222222222222222222222222222222222222222222
    {

      if (fn_cmd == "vl+" )
      {
        h1++; if (h1 > 9) h1 = 0;
      }
      if (fn_cmd == "vl-" )
      {
        h1--; if (h1 < 0) h1 = 9;
      }
    }
    if (col == 3) //333333333333333333333333333333333333333333333
    {

      if (fn_cmd == "vl+" )
      {
        h2++; if (h2 > 9) h2 = 0;
      }
      if (fn_cmd == "vl-" )
      {
        h2--; if (h2 < 0) h2 = 9;
      }

    }

    if (col == 4) //4444444444444444444444444444444444444444444444
    {
      if (fn_cmd == "vl+" )
      {
        m1++; if (m1 > 5) m1 = 0;
      }
      if (fn_cmd == "vl-" )
      {
        m1--; if (m1 < 0) m1 = 5;
      }

    }


    if (col == 5) //5555555555555555555555555555555555555555555
    {

      if (fn_cmd == "vl+" )
      {
        m2++; if (m2 > 9) m2 = 0;
      }
      if (fn_cmd == "vl-" )
      {
        m2--; if (m2 < 0) m2 = 9;
      }

    }

    // if (col > 1 && col < 6) {draw_timer()};
    draw_timer();

  } //vl+-
  //-EE--------------------------VL


  if (refresh_all || fn_cmd == "next" || fn_cmd == "prev")
  {
    if (fn_cmd == "next")
    {
      col++; if (col > col_count) col = 1;
    }
    if (fn_cmd == "prev")
    {
      col--; if (col < 1) col = col_count;
    }
    if (col == 1)
    {
      do_read_timer_txt();
      draw_timer();

    }

    draw_col(col);
  }

  if (t_timer_scr != 0 || h_tmp != h_timer || m_tmp != m_timer || refresh_all)
  {
    draw_timer();
    h_tmp = h_timer;
    m_tmp = m_timer;

  }


  //------------------------------START---------------------------------------
  if ((fn_cmd == "timer_start" && col != 6 && (h_timer != 0 || m_timer != 0)) || t_timer_boot > 0 )
  {
    if ( t_timer_scr == 0)
    {

#ifdef ESP32
      do_timer_alarm(); //init;
#endif //32     
#ifdef pin_beep
      do_timer_alarm_beep(); //init;
#endif
      t_timer_b = now();
      if (t_timer_boot > 0)
      {
        t_timer_scr = t_timer_boot;
        Serial.print(F("t_timer boot")); Serial.println(t_timer_scr);
        draw_timer();//calc h_timer m_timer
      }
      else
      {

        t_timer_scr = t_timer_b + h_timer * 60 * 60  + m_timer * 60; //+59sec

      }
      Serial.println(F("t_timer START"));
      do_fwrite_timer();
      //  draw_timer_ani();
    }
    else // t_timer >0, STOP, not exit
    {
      init_timer();
      Serial.println(F("t_timer>0 STOP"));
    }
  }


  //col==6
  if ((fn_cmd == "timer_start" && col == 6) || (fn_cmd == "timer" )) //EXIT ir=fav
  {
    scr_timer = false;
    init_timer();
    tft.fillScreen(TFT_BLACK );
    scr_num_show(scr_number);
    Serial.println(F("t_timer STOP Exit"));
    do_fwrite_timer(); //t_timer=0, remove file tmp
    return;
  }

  draw_timer_ani();
#if defined(tft_480_320)&& defined (openw)
  S3_time();
#endif
#if defined(tft_320_240)&& defined (openw)
  S5_time();
#endif


  if (t_timer_boot > 0) t_timer_boot = 0;


#else
  Serial.println(F("320x240 no S5 or 320x480 no S3"));


#endif //S3 || S5

}

//#endif  //#if (defined(tft_480_320)&& defined (openw)





//==========================================================================================================
//                        draw_timer
//==========================================================================================================
void draw_timer()
{
  static byte m_tmp = 99;
  String st_jpg;
  String fileName;//n0_112
  static byte h_timer_tmp = 199, m_timer_tmp = 199;
  if (refresh_all)
  {
    h_timer_tmp = 199, m_timer_tmp = 199; m_tmp = 99;
    //   return;

  }

  if (t_timer_scr > 0) //no play , h,m draw
  {

    int32_t t_delta = t_timer_scr - now() + 59; //+60 sec
    m_timer = (int (t_delta / 60)) % 60;
    if (m_tmp == m_timer) return;
    m_tmp = m_timer;
    h_timer = int(t_delta / 3600);// % 24;
    Serial.print("t_delta="); Serial.println(t_delta);
    Serial.print(F("h_timer=")); Serial.println(h_timer);
    Serial.print("m_timer="); Serial.println(m_timer);
    if (m_timer == 0 && h_timer == 0) //END init
    {
      m_timer = 0;
      h_timer = 0;
      m_tmp = 99;
      timer_alarm = true;
      h_timer_tmp = 199, m_timer_tmp = 199;
#ifdef debug
      Serial.println(F("t_timer=0 END"));
      Serial.println("timer_Alarm true");
#endif
      init_timer();

    }

  }
  else //t_timer==0, not start
  {
    h_timer = h1 * 10 + h2;
    m_timer = m1 * 10 + m2;

  }


#ifdef debug
  Serial.print("h_timer ");
  Serial.println(h_timer);
  Serial.print("m_timer ");
  Serial.println(m_timer);
#endif

  //draw_HH
  if (h_timer_tmp != h_timer)
  {
    h1 = (int)(h_timer / 10  );
    h2 = h_timer - h1 * 10;

    h_timer_tmp = h_timer  ;
    if (h_timer < 10)
    {
      fileName = "/n0_112.jpg";
      drawJpeg(fileName, col_2, y_jpeg); //220
      fileName = "/n" + String(h_timer) + "_112.jpg";
      drawJpeg(fileName, col_3, y_jpeg);
    }
    else
    {
      st_jpg = String(h_timer);
      fileName = "/n" + st_jpg.substring(0, 1) + "_112.jpg";
      drawJpeg(fileName, col_2, y_jpeg); //220
      fileName = "/n" + st_jpg.substring(1) + "_112.jpg";
      drawJpeg(fileName, col_3, y_jpeg);
    }
  }

  //draw_MM
  if (m_timer_tmp != m_timer)
  {
    m_timer_tmp = m_timer  ;
    m1 = (int)(m_timer / 10  );
    m2 = m_timer - m1 * 10;

    if (m_timer < 10)
    {
      fileName = "/n0_112.jpg";
      drawJpeg(fileName, col_4, y_jpeg); //220
      fileName = "/n" + String(m_timer) + "_112.jpg";
      drawJpeg(fileName, col_5, y_jpeg);
    }
    else
    {
      st_jpg = String(m_timer);
      fileName = "/n" + st_jpg.substring(0, 1) + "_112.jpg";
      drawJpeg(fileName, col_4, y_jpeg); //220
      fileName = "/n" + st_jpg.substring(1) + "_112.jpg";
      drawJpeg(fileName, col_5, y_jpeg);

    }

  }


}





//==========================================================================================================
//                        draw_col
//==========================================================================================================

void draw_col(byte col)
{

#ifdef tft_480_320
  tft.fillRect(0, y_begin + 1, 480, 12, TFT_BLACK);
#else
  tft.fillRect(0, y_begin + 1, 320, 12, TFT_BLACK);
#endif
  if (col == 1)
  {
#ifdef tft_480_320
    tft.fillRect(col_1 + 5, y_begin + 1, col_2 - 35, 8, TFT_CYAN);
#else
    tft.fillRect(col_1 + 5, y_begin + 1, col_2 - 15, 8, TFT_CYAN);
#endif

  }
  else if (col == 2)     tft.fillRect(col_2, y_begin + 1, col_step - 10, 8, TFT_CYAN);
  else if (col == 3)     tft.fillRect(col_3, y_begin + 1, col_step - 10, 8, TFT_CYAN);
  else if (col == 4)     tft.fillRect(col_4, y_begin + 1, col_step - 10, 8, TFT_CYAN);
  else if (col == 5)     tft.fillRect(col_5, y_begin + 1, col_step - 10, 8, TFT_CYAN);

#ifdef tft_480_320
  else if (col == 6)     tft.fillRect(col_2 - 20, y_begin + 1, 480 - col_2, 12, TFT_RED);
#else
  else if (col == 6)   tft.fillRect(col_2, y_begin + 1, 320 - col_2, 12, TFT_RED);
#endif

  // Serial.print(F("col = "));
  //  Serial.println(col);

}



//==========================================================================================================
//                        do_read_timer_txt
//==========================================================================================================

bool do_read_timer_txt()
{
  static byte num;
  static int y_timer_old = 0;
  static int y_timer = 0;
  byte count = 0;
  String filename = "/timer.txt";
  static byte count_line = 0;

  int x = 1;
  bool find = false;

#ifdef pin_sd
  Serial.print("SD ");
  Serial.println(filename);
  File f = SD.open( filename, FILE_READ);  // or, file handle reference for SD library
#else
  Serial.print("Spiffs ");
  Serial.println(filename);
  fs::File f = SPIFFS.open( filename, "r");    // File handle reference for SPIFFS
#endif


  if (!f)
  {
    Serial.print(filename);
    Serial.println(F(" no open,create"));
    return false;
  }
  //----------------------------
  //String hh = "";
  //String mm = "";


#ifdef tft_320_240
  const byte y_step = 14;
  tft.setFreeFont(v6);
  const byte count_max = 11;
#endif

#ifdef tft_480_320
  const byte y_step = 18;
  const byte count_max = 13;
  tft.setFreeFont(v9mono);
#endif

  int y = y_begin + 13 + y_step;

  String str;


  if (refresh_all)
  {
    y_timer_old = 0;
    y_timer = y;
    count = 0;
    count_line = 0;
    num = 1;
#ifdef tft_480_320
    tft.drawFastHLine(0, y_begin + 15, 480, TFT_WHITE) ;
    tft.drawFastVLine(col_2 - 25, y_begin + 15, 320 - y_begin - 15, TFT_CYAN) ;
#else
    tft.drawFastHLine(0, y_begin + 14, 320, TFT_WHITE) ;
    tft.drawFastVLine(col_2 - 5, y_begin + 14, 240 - y_begin - 15, TFT_CYAN) ;
#endif
  }


  if (count_line > 0) //file
  {
    if ( fn_cmd == "vl-" )
    {
      num++;
      if (num > count_line) num = 1;
    }
    else if ( fn_cmd == "vl+" )
    {
      num--;
      if (num < 1) num = count_line;
    }
    else if ( fn_cmd != "b0" && fn_cmd.substring(0, 1) == "b")
    {
      String s = fn_cmd.substring(1);
      num = s.toInt() ;
    }
  }

  //  Serial.print(F("num = "));
  //  Serial.println(num);

#ifdef debug
  //  Serial.println(F("----------------timer.txt"));
  //  Serial.println(filename);

#endif
  //---------------------------------------------------------
  while (f.available()) {
    //   yield();
    str = f.readStringUntil('\n');
    //Serial.println(str);
    //if (str.indexOf("{")>=0) (Serial.print("{=");Serial.println(str.indexOf("{")));

    if (str.indexOf("#") != 0) continue;
    int tz = 0;
    int tz2 = 0;
    int hh = -1;
    int mm = -1;

    //num------------------------
    tz2 = str.indexOf(";", tz + 1);
    if (tz > 0)
      tz2 = str.indexOf(";", tz + 1);
    if (tz2 > 0) //hhhhhhhhhhhh
    {
      hh = str.substring(tz + 1, tz2).toInt();
      //  Serial.print("hh ");
      //  Serial.println(hh);
      if (hh >= 24 || hh < 0) continue;

      //mm --------------------------------
      tz = tz2;
      tz2 = str.indexOf(";", tz + 1);
      if (tz2 > 0) //mmmmmmmmmmmmmm
      {
        mm = str.substring(tz + 1, tz2).toInt();
        //      Serial.print("mm ");
        //      Serial.println(mm);
        if (mm < 0 || mm > 59) continue;
      }
      else
      {
        Serial.println(F("not found min"));
        continue;
      }
    } //hh
    count ++;
    if (count == count_max) break ;
    if (refresh_all) //draw
    {
      tft.setCursor(x, y);
      tft.setTextColor(TFT_CYAN);
      if (count < 10) tft.print(" ");
      tft.print(count);
      tft.print(" ");
      tft.setTextColor(TFT_WHITE);
      if (hh < 10) tft.print("0");
      tft.print(hh);
      tft.setTextColor(TFT_YELLOW);
      tft.print(":");
      tft.setTextColor(TFT_WHITE);
      if (mm < 10) tft.print("0");
      tft.print(mm);

    }
    //FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
    if (count == num)
    {
      find = true;
      y_timer = y;

      if (y_timer_old != 0)

#ifdef ESP32
        tft.drawFastHLine(1, y_timer_old + 3, col_2 - 35, TFT_BLACK) ;
      tft.drawFastHLine(1, y_timer + 3, col_2 - 35, TFT_YELLOW) ;
#else
        tft.drawFastHLine(1, y_timer_old + 3, col_2 - 7, TFT_BLACK) ;
      tft.drawFastHLine(1, y_timer + 3, col_2 - 7, TFT_YELLOW) ;
#endif

      y_timer_old = y_timer;

      h1 = (int)(hh / 10  );
      h2 = hh - h1 * 10;
      m1 = (int)(mm / 10  );
      m2 = mm - m1 * 10;

      //  h_timer = hh;
      //  m_timer = mm;
#ifdef debug
      //     Serial.print("found hh/mm ");
      //     Serial.print(h_timer);  Serial.print("/"); Serial.println(m_timer);
#endif
    } // if (count == num)


    if (refresh_all) count_line++;
    y = y + y_step;

  }//while

  return find;
}

//==========================================================================================================
//                        do_timer_alarm MP3
//==========================================================================================================
#ifdef ESP32
void do_timer_alarm()
{
  static uint32_t t_next = 0;
  static byte t_alarm_count = tm_alarm_count;




  if (timer_alarm == false) //init
  {
    Serial.println(F("t_alarm_count init"));
    t_alarm_count = tm_alarm_count;
    t_next = 0;
    return;
  }

  if (t_alarm_count == 0) return;
  if (millis() < t_next) return;

  do_play_beep(1);

  t_alarm_count--;
  t_next = millis() + tm_alarm_period * 1000L;
#ifdef debug
  Serial.print(F("t_alarm_count")) ; Serial.println(t_alarm_count) ;
#endif
  // if (t_alarm_count == 0) timer_alarm = false;

}
#endif //32
//==========================================================================================================
//                        do_timer_alarm BEEP
//==========================================================================================================
#ifdef pin_beep
void do_timer_alarm_beep()
{
  static uint32_t t_next = 0;
  static byte t_alarm_count = tm_alarm_count_beep;



  if (timer_alarm == false) //init
  {
    Serial.println(F("t_alarm_count init"));
    t_alarm_count = tm_alarm_count_beep;
    t_next = 0;
    return;
  }

  if (t_alarm_count == 0) return;
  if (millis() < t_next) return;



  do_beep(fr_alarm, dur_alarm);

  t_alarm_count--;
  t_next = millis() + tm_alarm_period_beep * 1000L;
#ifdef debug
  Serial.print(F("t_alarm_count beep")) ; Serial.println(t_alarm_count) ;
#endif

}

#endif //pin_beep




//==========================================================================================================
//                        draw_timer_ani
//==========================================================================================================

void draw_timer_ani()
{
  static uint32_t t_next = 0;
  static bool start = false;
  static int color = TFT_BLACK;

  if (refresh_all)
  {
    start = false;
    t_next = 0;
    color = TFT_BLACK;
    return;
  }

  //  if (millis() < t_next && start) return;

  if (t_timer_scr > 0)
  {
    if (millis() < t_next && start) return;
    if (!start)
    {
      start = true;
#ifdef tft_480_320
      tft.fillRect(col_2, 275, col_6 - col_2, 25, TFT_BLACK);
      tft.setFreeFont(v12);
      tft.setCursor(col_2 + 30, 300);
#else
      tft.fillRect(col_2, 215, col_6 - col_2, 23, TFT_BLACK);
      tft.setFreeFont(v12);
      tft.setCursor(col_2 + 20, 235);
#endif
      tft.setTextColor(TFT_YELLOW);
      if (hour(t_timer_b) < 10 )  tft.print("0");
      tft.print(hour(t_timer_b));
      tft.print(":");
      if (minute(t_timer_b) < 10 )  tft.print("0");
      tft.print(minute(t_timer_b));

#ifdef tft_480_320
      tft.setCursor(col_4 + 30, 300);
#else
      tft.setCursor(col_4 + 20, 235);
#endif
      tft.setTextColor(TFT_GREEN);
      if (hour(t_timer_scr) < 10 )  tft.print("0");
      tft.print(hour(t_timer_scr));
      tft.print(":");
      if (minute(t_timer_scr) < 10)   tft.print("0");
      tft.print(minute(t_timer_scr));
      if (t_timer_boot > 0) color = TFT_YELLOW; else color = TFT_GREEN;

    }



    if (millis() / 1000 % 2 == 0)
#ifdef tft_480_320
      tft.fillRect(col_4 - 35, 280, 20, 20, color);
    else tft.fillRect(col_4 - 35, 280, 20, 20, TFT_BLACK);
#else
      tft.fillRect(col_4 - 10, 220, 10, 10, color);
    else tft.fillRect(col_4 - 10, 220, 10, 10, TFT_BLACK);
#endif

  }
  else
  {
    if (start) //pause
    {
      start = false;
      t_next = 0;
#ifdef tft_480_320
      tft.fillRect(col_4 - 35, 280, 20, 20, TFT_RED);
#else
      tft.fillRect(col_4 - 10, 220, 10, 10, TFT_RED);
#endif
    }




  }


  t_next = millis() + 1000;
}

//==========================================================================================================
//                        init_timer
//==========================================================================================================

void init_timer()
{
  t_timer_scr = 0; t_timer_b = 0;
}


//-----------------------------------------------------------------------------
//               do_fwrite_timer
//-----------------------------------------------------------------------------

void do_fwrite_timer()
{

  //if (log_str == "") return;
  File f;

  String filename = "/timer.tmp";
#ifdef debug
  Serial.println("write /timer.tmp");
#endif

#ifdef pin_sd
#ifdef ESP32
  fs::FS &fs = SD;
  if (fs.exists(filename)) fs.remove(filename);
  f = fs.open(filename, FILE_APPEND);
#endif //32
#ifdef ESP8266
  if (SD.exists(filename)) SD.remove(filename);
  f = SD.open( filename, FILE_WRITE);  // or, file handle reference for SD library
#endif //66
#else //pin_sd
#ifdef ESP32
  fs::FS &fs = SPIFFS;
  if (fs.exists(filename)) fs.remove(filename);
  f = fs.open( filename, "a+");    // File handle reference for SPIFFS
#endif //32
#ifdef ESP8266
  // fs::FS &fs = SPIFFS;
  if (SPIFFS.exists(filename)) SPIFFS.remove(filename);
  f = SPIFFS.open( filename, "a+");    // File handle reference for SPIFFS
#endif

#endif

  if (t_timer_scr == 0) return; //просто стёр старый файл


  if (!f) {
    Serial.print(filename);
    Serial.println(F(" ERR timer file no open,create"));
    //   return;
  }



  String s_time = String(t_timer_scr) ;
#ifdef debug
  Serial.println("timer_end_tmp=" + s_time);
#endif

  f.println(s_time);
  f.close();


}




//-----------------------------------------------------------------------------
//               do_fread_timer
//-----------------------------------------------------------------------------

void do_fread_timer()
{
  File f;

  String filename = "/timer.tmp";
#ifdef debug
  Serial.println("read /timer.tmp");
#endif
#ifdef pin_sd
  f = SD.open( filename, FILE_READ);  // or, file handle reference for SD library
#else
  f = SPIFFS.open( filename, "r");    // File handle reference for SPIFFS
#endif


  if (!f) {
    Serial.println(filename);
    Serial.println(F("timer.tmp file no open,create"));
    return;
  }

  // while (f.available()) {
  uint32_t tmp;
  String    str = f.readStringUntil('\n');
  str.trim();
  char **pointer, *stringVar;
  //  unsigned long unsignedVar;
  char charBufVar[20];
  str.toCharArray(charBufVar, 20);
  stringVar = charBufVar;
  tmp = strtoul(stringVar, pointer, 10);
#ifdef debug
  Serial.print("old_timer="); Serial.println(tmp);
  Serial.print("now()="); Serial.println(now());
#endif


  if (tmp > now())
  {
    t_timer_boot = tmp;
    fn_cmd = "timer";
    do_fn_btn_ir();

#ifdef debug
    Serial.print("t_timer_boot="); Serial.println(t_timer_boot);
#endif

    //    fn_cmd = "";
  }
  else
#ifdef debug
    Serial.println("old timer<now()");
#endif
  f.close();


}

#endif //#if !defined(tft_400_300)
