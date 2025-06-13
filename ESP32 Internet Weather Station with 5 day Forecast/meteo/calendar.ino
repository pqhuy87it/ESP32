#ifndef epd
//----------------------------------------------------------------------------
//                          draw_Calendar_2tft
//----------------------------------------------------------------------------
#ifdef two_tft
#define tft tft2
void draw_Calendar_2tft(int x_cal, int y_cal, int w_cal, int8_t delta)
{

  tft.fillRect(x_cal, y_cal, w_cal + 7, y_cal + 159, TFT_BLACK);
  tft.drawRoundRect(x_cal + 3 , y_cal + 18, w_cal + 4, y_cal + 139, 5, TFT_LIGHTGREY);

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

  // int16_t fx;

  uint16_t w, h;
  int x = x_cal;
  // int y = 36;
  // int w_cal = 220;
  int y = y_cal + 10;




  int curday = tmp_day - tm_wday;


  while (curday > 1)
    //   curday -= 7;
    curday -= 7;
  curday++;
  //  Serial.print("curday-- "); Serial.println(curday);

  tft.setFreeFont(v6);
  for (int i = 0 ; i < 7; i++)
  {
    x = x_cal + i * (w_cal - 10) / 7;
    tft.setCursor(x + (w_cal / 7 - w) / 2 - 4, y );
    if (i > 4) tft.setTextColor(c_holiday);
    else if (i == 4) tft.setTextColor(c_friday);
    else if (i > 0) tft.setTextColor(c_working);
    else if (i == 0) tft.setTextColor(c_monday);
    byte wd = i + 1 ;
    if (wd == 7) wd = 0;
    tft.print(dayNames[wd]);
  }

  // tft.drawFastHLine(0, y + 3, w_cal, TFT_LIGHTGREY) ;


  tft.setFreeFont(v9mono);
  tft.setTextColor(TFT_LIGHTGREY);
  y = y + 25;

  //-----------------------------------------------------
  while (curday <= daysinmonth)
  {
    for (int i = 0; i < 7; i++)
    {

      if (i > 4) tft.setTextColor(TFT_ORANGE);
      else if (curday < now_day) tft.setTextColor(TFT_LIGHTGREY);
      else
        tft.setTextColor(TFT_WHITE);


      x = x_cal + i * (w_cal - 10) / 7;
      //========================holiday------------------------------------
      if (hld) {
        int res = log_str.indexOf(";" + String(curday) + ";");
        if (res >= 0) //tft.setTextColor(c_holiday);
        {
          //    tft.drawFastHLine(x - 2 + (w_cal - 8) / 7 / 2 - w / 2 - fx, y + h / 2 + 3, 26, c_holiday) ;
          //top c_holiday
          tft.drawFastHLine(x + (w_cal - 8) / 7 / 2 - w / 2 , y + h / 2 + 4, 24, TFT_CYAN) ;
          //bot
          tft.drawFastHLine(x + (w_cal - 8) / 7 / 2 - w / 2 , y + h / 2 - 14, 24, TFT_CYAN) ;


          //left
          tft.drawFastVLine(x + (w_cal - 8) / 7 / 2 - w / 2 , y + h / 2 - 14, 19, TFT_CYAN) ;
          //R
          tft.drawFastVLine(x + (w_cal - 8) / 7 / 2 - w / 2 + 24, y + h / 2 - 14, 19, TFT_CYAN) ;


        }
        //     Serial.print("fnd  "); Serial.println(";" + String(curday) + ";");
        //     Serial.print("res "); Serial.println(res);
      }
      if (curday >= 1 && curday <= daysinmonth)
      {
        //    tft.setCursor(x, y);

        //   int16_t fx;
        uint16_t w, h;
        String strday = String(curday);
        if  (tmp_year == now_year &&
             tmp_month == now_month && curday == tmp_day)
        {
          tft.setFreeFont(v9b);
          //     tft.setFreeFont(v6);
          tft.setTextColor(TFT_YELLOW);
          tft.setCursor(x + (w_cal - 8) / 7 / 2 - w / 2, y + h / 2);
          tft.print(curday);
          tft.setTextColor(TFT_WHITE);
          //  tft.setFreeFont(v6);
          tft.setFreeFont(v9mono);
          tft.drawFastHLine(x + (w_cal - 8) / 7 / 2 - w / 2 , y + h / 2 + 4, 24, TFT_YELLOW) ;
          //top
          tft.drawFastHLine(x + (w_cal - 8) / 7 / 2 - w / 2 , y + h / 2 - 14, 24, TFT_YELLOW) ;
          //left
          tft.drawFastVLine(x + (w_cal - 8) / 7 / 2 - w / 2 , y + h / 2 - 14, 19, TFT_YELLOW) ;
          //R
          tft.drawFastVLine(x + (w_cal - 8) / 7 / 2 - w / 2 + 24, y + h / 2 - 14, 19, TFT_YELLOW) ;

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

#undef tft //tft2
#endif //2tft




//----------------------------------------------------------------------------
//                          draw_Calendar
//----------------------------------------------------------------------------
void draw_Calendar(int x_cal, int y_cal, int w_cal, int8_t delta)
{

#ifdef two_tft
  if (tft2_act)
  {
    draw_Calendar_2tft(x_cal, y_cal, w_cal, delta);
    return;
  }
#endif

  tft.fillRect(x_cal, y_cal, w_cal + 7, y_cal + 159, TFT_BLACK);
  //tft.drawRoundRect(x_cal, y_cal, w_cal + 7, y_cal + 155,5, TFT_GREEN);
  tft.drawRoundRect(x_cal + 3 , y_cal + 18, w_cal + 4, y_cal + 139, 5, TFT_LIGHTGREY);

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
  // int tm_wday = weekday(t);
  //  Serial.println("drawing calendar for " + String(months[tmp_month - 1]) + " " + String(tmp_year));

  // draw calendar
  // String stryear = String(pickdate->tm_year);
  //int y = 18;

  // tft.setCursor(8, 18);
  //  tft.print(stryear);

  //  String strmonth = months[tmp_month];
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
  // Serial.print("tmp_day "); Serial.println(tmp_day);
  //  Serial.print("tmp_m "); Serial.println(tmp_month);
  //  Serial.print("tmp_y "); Serial.println(tmp_year);
  //  Serial.print("tm_wday "); Serial.println(tm_wday);
  //  Serial.print("curday "); Serial.println(curday);

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
    if (i > 4) tft.setTextColor(c_holiday);
    else if (i == 4) tft.setTextColor(c_friday);
    else if (i > 0) tft.setTextColor(c_working);
    else if (i == 0) tft.setTextColor(c_monday);
    byte wd = i + 1 ;
    if (wd == 7) wd = 0;
    tft.print(dayNames[wd]);
  }

  // tft.drawFastHLine(0, y + 3, w_cal, TFT_LIGHTGREY) ;


  tft.setFreeFont(v9mono);
  tft.setTextColor(TFT_LIGHTGREY);
  y = y + 25;

  //-----------------------------------------------------
  while (curday <= daysinmonth)
  {
    for (int i = 0; i < 7; i++)
    {

      if (i > 4) tft.setTextColor(TFT_ORANGE);
      else if (curday < now_day) tft.setTextColor(TFT_LIGHTGREY);
      else
        tft.setTextColor(TFT_WHITE);




      //    else if (i == 4) tft.setTextColor(c_friday);
      //    else if (i > 1) tft.setTextColor(c_working);
      //    else if (i == 1) tft.setTextColor(c_monday);

      //    x = 4 + i * (w_cal - 8) / 7;
      x = x_cal + i * (w_cal - 10) / 7;
      //========================holiday------------------------------------
      if (hld) {
        int res = log_str.indexOf(";" + String(curday) + ";");
        if (res >= 0) //tft.setTextColor(c_holiday);
        {
          //    tft.drawFastHLine(x - 2 + (w_cal - 8) / 7 / 2 - w / 2 - fx, y + h / 2 + 3, 26, c_holiday) ;
          //bot c_holiday
          tft.drawFastHLine(x + (w_cal - 8) / 7 / 2 - w / 2 , y + h / 2 + 4, 24, TFT_CYAN) ;
          //top
          tft.drawFastHLine(x + (w_cal - 8) / 7 / 2 - w / 2 , y + h / 2 - 14, 24, TFT_CYAN) ;
          //left
          tft.drawFastVLine(x + (w_cal - 8) / 7 / 2 - w / 2 , y + h / 2 - 14, 19, TFT_CYAN) ;
          //R
          tft.drawFastVLine(x + (w_cal - 8) / 7 / 2 - w / 2 + 24, y + h / 2 - 14, 19, TFT_CYAN) ;


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
          tft.setTextColor(TFT_YELLOW);
          tft.setCursor(x + (w_cal - 8) / 7 / 2 - w / 2, y + h / 2);
          tft.print(curday);
          tft.setTextColor(TFT_WHITE);
          //  tft.setFreeFont(v6);
          tft.setFreeFont(v9mono);
          tft.drawFastHLine(x + (w_cal - 8) / 7 / 2 - w / 2 , y + h / 2 + 4, 24, TFT_YELLOW) ;
          //top
          tft.drawFastHLine(x + (w_cal - 8) / 7 / 2 - w / 2 , y + h / 2 - 14, 24, TFT_YELLOW) ;
          //left
          tft.drawFastVLine(x + (w_cal - 8) / 7 / 2 - w / 2 , y + h / 2 - 14, 19, TFT_YELLOW) ;
          //R
          tft.drawFastVLine(x + (w_cal - 8) / 7 / 2 - w / 2 + 24, y + h / 2 - 14, 19, TFT_YELLOW) ;

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


#endif // no epd



bool isLeapYear(int year) {
  if (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0))
    return true;
  return false;
}

int getDaysInMonth(int month, int year) {
  int daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if (month != 2)
    return daysInMonth[(month - 1) % 12];
  if (isLeapYear(year))
    return 29;
  return 28;
}

int getDayOfWeek(int year, int month, int day)
{
  uint16_t months[] = {
    0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365
  };   // days until 1st of month

  uint32_t days = year * 365;        // days until year
  for (uint16_t i = 4; i < year; i += 4) if (isLeapYear(i) ) days++;     // adjust leap years, test only multiple of 4 of course

  days += months[month - 1] + day;  // add the days of this year
  if ((month > 2) && isLeapYear(year)) days++;  // adjust 1 if this year is a leap year, but only after febr

  // make Sunday 0
  days--;
  if (days < 0)
    days += 7;
  return days % 7;   // remove all multiples of 7
}





//==========================================================================================================
//                              read_holiday
//==========================================================================================================
bool read_holiday(byte m)
{
  bool find = false;
  String filename = "/holiday.txt";

#ifdef pin_sd
  //  Serial.print("SD ");
  //  Serial.println(filename);
  File f = SD.open( filename, FILE_READ);  // or, file handle reference for SD library
#else
  //  Serial.print("Spiffs ");
  //  Serial.println(filename);
  fs::File f = SPIFFS.open( filename, "r");    // File handle reference for SPIFFS
#endif

  if (!f) {
    Serial.print(filename);
    Serial.println(F(" no open"));
    return find;
  }




  //  Serial.println(F("open /holiday.txt"));
  String str = "";
  //-------------------
  while (f.available()) {
    str = f.readStringUntil('\n');
#ifdef debug
    Serial.println(str);
#endif
    if (str.indexOf("//") == 0) continue;
    if (str == "") continue;
    int tz = 0;
    int tz2 = 0;
    //---------------
    tz = str.indexOf("#");
    if (tz >= 0)
    {

      //month------------------------
      tz2 = str.indexOf(";", tz + 1);
      if (tz2 > 0)
      {
        log_str = str.substring(tz + 1, tz2);
        //       Serial.print("month "); Serial.println(log_str);
      }
      else continue;

      if (log_str.toInt() != m) continue;

      log_str = str.substring(tz2);
      //      Serial.print("month string"); Serial.println(log_str);
      find = true;
      break;
    }
  }
  f.close();
  return find;
}
