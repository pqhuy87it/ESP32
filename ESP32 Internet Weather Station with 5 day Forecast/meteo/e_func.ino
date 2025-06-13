#ifdef epd
//-------------------------------------------------------------------------------
//                  moon_char
//-------------------------------------------------------------------------------
char moon_char()
{
  String s = do_phasemoon();
  s = s.substring(2);
  int  ph = s.toInt();
#ifdef debug
  Serial.println("moon_char -" + s + " - " + String(92 + ph));
#endif
  return char(92 + ph);
}


// ------------------------------------------------------------------------------ -
//                  wh_char
//-------------------------------------------------------------------------------
char wh_char(byte n, char day_ch) //d or c
{
  char ch = 32;
  if (day_ch == 'c' && n < ch_count)
  {
    if (icon_ch[n] == "/01d") ch = 0x47;//1d=0x47
    else if (icon_ch[n] == "/01n") ch = 0x48; //1n=0x48
    else if (icon_ch[n] == "/02d") ch = 0x49; //2d=0x49
    else if (icon_ch[n] == "/02n") ch = 0x4A; //2n=0x4A
    else if (icon_ch[n] == "/03d") ch = 0x4B; //3d=0x4B
    else if (icon_ch[n] == "/03n") ch = 0x4B; //3n=0x4B
    else if (icon_ch[n] == "/04d") ch = 0x4C; //4d=0x4C
    else if (icon_ch[n] == "/04n") ch = 0x4C;  //4n=0x4C
    else if (icon_ch[n] == "/09d") ch = 0x4D; //9d=0x4D
    else if (icon_ch[n] == "/09n") ch = 0x4D; //9n=0x4D
    else if (icon_ch[n] == "/10d") ch = 0x4E; //10d=0x4E
    else if (icon_ch[n] == "/10n") ch = 0x4F; //10n=0x4F
    else if (icon_ch[n] == "/11d") ch = 0x50; //11d=0x50
    else if (icon_ch[n] == "/11n") ch = 0x50; //11n=0x50
    else if (icon_ch[n] == "/13d") ch = 0x51; //13d=0x51
    else if (icon_ch[n] == "/13n") ch = 0x51;  //13n=0x51
    else if (icon_ch[n] == "/50d") ch = 0x52;   //50d=0x52
    else if (icon_ch[n] == "/50n") ch = 0x52;   //50n=0x52
#ifdef debug
    Serial.println("wh_char ch-" + icon_ch[n] + " - " + char(ch));
#endif
    return char(ch);
  }
  else if (day_ch == 'd' && n < 8)
  {
    if (icon7[n] == "/01d") ch = 0x47;//1d=0x47
    else if (icon7[n] == "/01n") ch = 0x48; //1n=0x48
    else if (icon7[n] == "/02d") ch = 0x49; //2d=0x49
    else if (icon7[n] == "/02n") ch = 0x4A; //2n=0x4A
    else if (icon7[n] == "/03d") ch = 0x4B; //3d=0x4B
    else if (icon7[n] == "/03n") ch = 0x4B; //3n=0x4B
    else if (icon7[n] == "/04d") ch = 0x4C; //4d=0x4C
    else if (icon7[n] == "/04n") ch = 0x4C;  //4n=0x4C
    else if (icon7[n] == "/09d") ch = 0x4D; //9d=0x4D
    else if (icon7[n] == "/09n") ch = 0x4D; //9n=0x4D
    else if (icon7[n] == "/10d") ch = 0x4E; //10d=0x4E
    else if (icon7[n] == "/10n") ch = 0x4F; //10n=0x4F
    else if (icon7[n] == "/11d") ch = 0x50; //11d=0x50
    else if (icon7[n] == "/11n") ch = 0x50; //11n=0x50
    else if (icon7[n] == "/13d") ch = 0x51; //13d=0x51
    else if (icon7[n] == "/13n") ch = 0x51;  //13n=0x51
    else if (icon7[n] == "/50d") ch = 0x52;   //50d=0x52
    else if (icon7[n] == "/50n") ch = 0x52;   //50n=0x52
#ifdef debug
    Serial.println("wh_char day -" + icon7[n] + " - " + char(ch));
#endif
    return char(ch);
  }
  else
  {
    Serial.print("wh_char ERR n="); Serial.println(n);
    return ' ';
  }
}




//-------------------------------------------------------------------------------
//                  v_char
//-------------------------------------------------------------------------------
char v_char(byte n, char day_ch )
{
  float gr = 0;
  if (day_ch == 'c' && n < ch_count)  gr = int(v_gr_ch[n]);
  else if (day_ch == 'd' && n < 8)  gr = int(v_gr_7[n]);
  else
  {
    Serial.print("v_char ERR n="); Serial.println(n);
    return ' ';
  }


  char ch = 32;
  if (gr >= 0 && gr <= 30)
    ch = 0xA8;
  else if (gr >= 31 && gr <= 60) {
    ch = 0xA9;
  }
  else if (gr >= 61 && gr <= 120) {
    ch = 0xAA;
  }
  else if (gr >= 121 && gr <= 150) {
    ch = 0xAB;
  }
  else if (gr >= 151 && gr <= 210) {
    ch = 0xAC;
  }
  else if (gr >= 211 && gr <= 240) {
    ch = 0xAE;
  }
  else if (gr >= 241 && gr <= 300) {
    ch = 0xAF;
  }
  else if (gr >= 301 && gr <= 330) {
    ch = 0xB0;
  }
  else if (gr >= 331 && gr <= 360) {
    ch = 0xA8;
  }

#ifdef debug
  Serial.println("V_char -" + String(day_ch) + " " + String(gr) + " ch " + String(ch));
#endif
  return char(ch);
}





//----------------------------------------------------------------------------
//                          draw_Calendar
//----------------------------------------------------------------------------
void draw_Calendar(int x_cal, int y_cal, int w_cal, int8_t delta)
{

  tft.setTextColor(TFT_BLACK);
  // tft.drawRoundRect(x_cal  , y_cal, w_cal, 17, 3, TFT_BLACK);
  // tft.drawRoundRect(x_cal  , y_cal + 18, w_cal, 145, 5, TFT_BLACK);
  tft.drawRect(x_cal  , y_cal, w_cal, 17,  TFT_BLACK);
  tft.drawRect(x_cal  , y_cal + 18, w_cal, 145,  TFT_BLACK);


  //tft.drawRect(x_cal + 4 , y_cal + 18, w_cal + 4, 145, TFT_BLACK);

  // char *months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

  int tmp_year = now_year;
  byte tmp_month = now_month;
  byte tmp_day = now_day;
  if (delta == 1)
  {
    tmp_day = 1;
    tmp_month++;
    if (tmp_month > 12)
    {
      tmp_month = 1;
      tmp_year++;
    }

  }

  int tm_wday = getDayOfWeek(tmp_year, tmp_month, tmp_day);
  bool hld = false;
  hld = read_holiday(tmp_month);

  int daysinmonth = getDaysInMonth(tmp_month, tmp_year);

  //  int16_t fx;

  uint16_t w, h;
  int x = x_cal;
  // int y = 36;
  // int w_cal = 220;
  int y = y_cal + 10;



  // tft.setCursor((w_cal - w) / 2, y+18);
  // tft.print(strmonth);

  int curday = tmp_day - tm_wday;


  while (curday > 1)
    //   curday -= 7;
    curday -= 7;
  curday++;
  // Serial.print("curday-- "); Serial.println(curday);

  tft.setFreeFont(v6);
  for (int i = 0 ; i < 7; i++)
  {
    x = x_cal + i * (w_cal - 10) / 7;
    tft.setCursor(x + (w_cal / 7 - w) / 2 - 4, y );
    byte wd = i + 1 ;
    if (wd == 7) wd = 0;
    tft.print(dayNames[wd]);
  }

  // tft.drawFastHLine(0, y + 3, w_cal, TFT_LIGHTGREY) ;


  tft.setFreeFont(v9mono);
  y = y + 25;

  //-----------------------------------------------------
  while (curday <= daysinmonth)
  {
    for (int i = 0; i < 7; i++)
    {

      x = x_cal + i * (w_cal - 10) / 7;
      //========================holiday------------------------------------
      if (hld) {
        int res = log_str.indexOf(";" + String(curday) + ";");
        if (res >= 0) //tft.setTextColor(c_holiday);
        {
          //    tft.drawFastHLine(x - 2 + (w_cal - 8) / 7 / 2 - w / 2 - fx, y + h / 2 + 3, 26, c_holiday) ;
          //bot c_holiday
          tft.drawFastHLine(x + (w_cal - 8) / 7 / 2 - w / 2 , y + h / 2 + 4, 24, TFT_BLACK) ;
          //top
          tft.drawFastHLine(x + (w_cal - 8) / 7 / 2 - w / 2 , y + h / 2 - 14, 24, TFT_BLACK) ;
          //left
          tft.drawFastVLine(x + (w_cal - 8) / 7 / 2 - w / 2 , y + h / 2 - 14, 19, TFT_BLACK) ;
          //R
          tft.drawFastVLine(x + (w_cal - 8) / 7 / 2 - w / 2 + 24, y + h / 2 - 14, 19, TFT_BLACK) ;


        }
        //     Serial.print("fnd  "); Serial.println(";" + String(curday) + ";");
        //     Serial.print("res "); Serial.println(res);
      }
      if (curday >= 1 && curday <= daysinmonth)
      {
        //    tft.setCursor(x, y);

        int16_t fx;
        uint16_t w, h;
        String strday = String(curday);
        if  (tmp_year == now_year &&
             tmp_month == now_month && curday == tmp_day)
        {
          tft.setFreeFont(v9b);
          //     tft.setFreeFont(v6);
          tft.setCursor(x + (w_cal - 8) / 7 / 2 - w / 2, y + h / 2);
          tft.print(curday);
          tft.setFreeFont(v9mono);
          tft.drawFastHLine(x + (w_cal - 8) / 7 / 2 - w / 2 , y + h / 2 + 4, 24, TFT_BLACK) ;
          //top
          //        tft.drawFastHLine(x + (w_cal - 8) / 7 / 2 - w / 2 , y + h / 2 - 14, 24, TFT_BLACK) ;
          //left
          //         tft.drawFastVLine(x + (w_cal - 8) / 7 / 2 - w / 2 , y + h / 2 - 14, 19, TFT_BLACK) ;
          //R
          //        tft.drawFastVLine(x + (w_cal - 8) / 7 / 2 - w / 2 + 24, y + h / 2 - 14, 19, TFT_BLACK) ;

        }
        else
        {
          tft.setCursor(x + (w_cal - 8) / 7 / 2 - w / 2 , y + h / 2);
          tft.print(curday);
        }


      }
      curday++;
    }
    y += 24;

  }
  //  Serial.print("y += 24 "); Serial.println(y);

  // tft.drawFastHLine(0, y + 3, w_cal, TFT_LIGHTGREY) ;
}


//===================================================================================================
//            S_show_uv
//===================================================================================================
//показ значка uv


void S_show_uv_epd(int x, int y)
{
  x++;
  y++;

  tft.fillRoundRect(x, y, 8, 8, 3, TFT_BLACK);
  tft.drawLine(x + 2, y + 1, x + 16, y, TFT_BLACK);
  tft.drawLine(x + 2, y + 2, x + 16, y + 14, TFT_BLACK);
  tft.drawLine(x + 2, y + 1, x + 2, y + 14, TFT_BLACK);
  if (int(uv) > 0)
  {
    tft.setFreeFont(v7b);
    tft.setCursor(x, y + 24);
    tft.print(int(uv));
  }


}




//-------------
#endif //epd
