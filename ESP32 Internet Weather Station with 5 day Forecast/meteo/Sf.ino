#if defined (pin_sd) && defined (Sf)&&!defined (epd)
//-------------------------------------------------------------
//              Sf_time
//-------------------------------------------------------------
void Sf_time()
{
  static time_t next_time = 0;

  if (refresh_all) next_time = 0;


  if (max_jpg_num == 0)
  {
    Serial.println(F("count Foto=0"));
    tft.setCursor(40, 40);
    tft.setTextColor(TFT_ORANGE);
    tft.setFreeFont(v9mono);
    tft.print("count Foto=0");
    next_time = millis() + 2 * 1000L;
    return;
  }

#ifdef  Sf_show_time
  static byte last_min = 99;
  if (t % 2 == 0)
  {
    if (err_ntp == 0)
      tft.fillCircle(44, 20, 2, TFT_GREENYELLOW);
    else
      tft.fillCircle(44, 20, 2, TFT_RED);
  }
  else
  {
    tft.fillCircle(44, 20, 2, TFT_BLACK);
  }

#endif //Sf_show_time



  if (millis() < next_time)
  {
#ifdef  Sf_show_time
    if (last_min != now_min)
    {
      last_min = now_min;
      Sf_ref_clock();
    }
#endif

    return;
  }

  //ffffffffffffffffffffffffffffffffffffffffffffffffffffffffff
  unsigned int r_num = random(1, max_jpg_num + 1);
  String fname = "/foto/" + String(r_num) + ".jpg";
  //Serial.println(fname);
  //bool fnd = SD.exists(fname);
  File fnd = SD.open(fname.c_str(), FILE_READ);

  if (!fnd)
  {
#ifdef debug
    Serial.println("no file foto "); Serial.println(fname);
#endif
    return;
  }
  tft.fillScreen(TFT_BLACK);
  if (!drawJpeg(fname, 0, 0))
  {

    tft.setTextColor(TFT_RED);
    tft.setFreeFont(v9mono);
    tft.setCursor(40, 40);
    tft.print("ERR foto "); tft.print(fname);

    log_str = "ERR foto " + fname;
    do_log();

    next_time = millis() + 2 * 1000L;
    return;
  }

  if (!scr_round) draw_stop_round();


#ifdef  Sf_show_time
  if (sync_t)
    Sf_ref_clock();
#endif

  //dddddddddddddddddddddddddddddddddddddddddddddddddd
#ifdef Sf_show_date
  sf_ref_date();
#endif

  //ttttttttttttttttttttttttttttttttttttttttttttttttt
  int x = 5;
#ifdef Sf_show_Tu//показывать уличная темп.
  int temp;
  x = 68;
#ifdef  openw
  if (temperatura_ch[0] != 200) temp = temperatura_ch[0];
#endif

  if (temp_u != 200) temp = temp_u;

  if (temp != 200)
  {
    unsigned int col;
    col = color_t(temp);
    tft.setTextColor(col);
    tft.setFreeFont(v12);

#ifdef tft_320_240
    if (temp >= 0 || temp < 10)
      tft.setCursor(17, 233);
    else
      tft.setCursor(6, 233);
    tft.fillRoundRect(3, 206, 65, 32, 4, TFT_BLACK);
    tft.drawRoundRect(3, 206, 65, 32, 4, c_tu_frame);
#endif

#ifdef tft_480_320
    if (temp >= 0 || temp < 10)
      tft.setCursor(17, 311);
    else
      tft.setCursor(6, 311);
    tft.fillRoundRect(3, 286, 65, 32, 4, TFT_BLACK);
    tft.drawRoundRect(3, 286, 65, 32, 4, c_tu_frame);

#endif


    tft.print(temp);
    tft.setTextColor(TFT_WHITE);
    tft.setFreeFont(v6);
    tft.print("c");
  }
#endif //Sf_show_Tu


  //iiiiiiiiiiiiiiiiiiiiiiiiiiii
#if defined (Sf_show_icon)  && defined (openw)
#ifdef tft_320_240
  if (icon_ch[0] != "")
  {
    drawJpeg(icon_ch[0] + "32.jpg", x, 206); // текущая погода
    if (rain_ch[0] > 0)
    {
      x = x + 32;
      tft.fillRect(x, 206, 8, 32, TFT_BLACK);
      x = x + 4;
      do_draw_rain(x, 214, rain_ch[0], TFT_CYAN);

    }


  }
#endif //tft_320_240

#ifdef tft_480_320
  if (icon_ch[0] != "")
  {
    drawJpeg(icon_ch[0] + "32.jpg", x, 286); // текущая погода

    if (rain_ch[0] > 0)
    {
      x = x + 32;
      tft.fillRect(x, 286, 8, 32, TFT_BLACK);
      x = x + 4;
      do_draw_rain(x, 294, rain_ch[0], TFT_CYAN);

    }



  }
#endif //tft_480_320
#endif //Sf_show_icon





  //hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh
#ifdef Sf_show_TH//показывать уличная темп.
  if (temp_kv != 200 || h_kv != 200)
  {
#ifdef tft_320_240
    S_show_th_room(273, 200);
#endif
#ifdef tft_480_320
    S_show_th_room(426, 262);
#endif
  }
#endif







#ifdef tft_320_240
  tft.drawFastVLine(0, 0, 239, Sf_c_scr_frame) ;
  tft.drawFastVLine(319, 0, 239, Sf_c_scr_frame) ;
  tft.drawFastHLine(0, 0, 319, Sf_c_scr_frame) ;
  tft.drawFastHLine(0, 239, 319, Sf_c_scr_frame) ;
#endif

#ifdef tft_480_320
  tft.drawFastVLine(0, 0, 319, Sf_c_scr_frame) ;
  tft.drawFastVLine(479, 0, 319, Sf_c_scr_frame) ;
  tft.drawFastHLine(0, 0, 479, Sf_c_scr_frame) ;
  tft.drawFastHLine(0, 319, 479, Sf_c_scr_frame) ;

#endif

  next_time = millis() + Sf_t_slide * 1000L;

}




//--------------------------------------------------------------------------------
//                  Sf_ref_clock
//--------------------------------------------------------------------------------
#ifdef  Sf_show_time
void Sf_ref_clock()
{
  String st;
  tft.setTextColor(Sf_c_clock);
  tft.setFreeFont(v12);
  tft.fillRoundRect(3, 1, 80, 29, 4, TFT_BLACK);
  tft.drawRoundRect(3, 1, 80, 29, 4, TFT_CYAN);
  //hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh
  if (now_hour < 10)
    st = "0" + String(now_hour);
  else
    st = String(now_hour) + "   ";

  tft.setCursor(7, 23);
  tft.print(st);

  //mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm

  if (now_min < 10)
    st = "0" + String(now_min);
  else
    st = String(now_min);

  //tft.setTextColor(Sf_c_clock);

  tft.setCursor(50, 23);
  tft.print(st);






}
#endif  //Sf_time


//--------------------------------------------------------------------------------
//                  Sf_show_date
//--------------------------------------------------------------------------------

#ifdef Sf_show_date
void  sf_ref_date()
{
  String st;
  tft.setTextColor(c_date);
  tft.setFreeFont(v9mono);
  int x;
#ifdef tft_320_240
  x = 259;
#endif
#ifdef tft_480_320
  x = 419;
#endif
  tft.fillRoundRect(x, 1, 59, 20, 4, TFT_BLACK);
  tft.drawRoundRect(x, 1, 59, 20, 4, TFT_CYAN);
  x = x + 4;
  if (day(t) < 10)
    st = "0" + String(day(t));
  else
    st = String(day(t));
  tft.setCursor(x, 15);
  tft.print(st);

  tft.setTextColor(TFT_CYAN);
  tft.setCursor(x + 20, 15);
  tft.print("/");


  tft.setTextColor(c_month);
  if (month(t) < 10)
    st = "0" + String(month(t));
  else
    st = String(month(t));
  tft.setCursor(x + 30, 15);
  tft.print(st);

}
#endif //Sf_show_date



#endif //pin_sd
