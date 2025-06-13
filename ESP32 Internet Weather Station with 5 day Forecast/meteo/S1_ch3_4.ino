#if defined (S1) && defined(tft_480_320)&& defined (openw)


#if defined (two_tft) && defined (tft2_S1)
#define tft tft2
#endif


//------------------------------------------------------------------------
//                      S1_wh_ch
//------------------------------------------------------------------------
void S1_wh_ch()
{

  int y = 170;
  int x = 0;
  byte x_step = 80;

  int n;


  //tft.fillRect(0,160,239,160,TFT_BLACK);
  String s = "";
  for (byte i = 1; i < 4; i++) {
    //scr cl
    for (byte ii = 0; ii <= x_step; ii++) {
      if (ii < x_step)
        tft.drawFastVLine(x + ii + 1, 160, 160, TFT_CYAN) ;
      tft.drawFastVLine(x + ii, 160, 160, TFT_BLACK) ;
      delay(2);
    }

    //усди нет данных продолжаю
    if (temperatura_ch[i] == 200) continue;


    //dddddddddddddddddddddddddddddddddddd
    if (day_ch[i] < 10)
      s = (" 0");
    else
      s = (" ");

    s = s + String(day_ch[i]) + ":00";
    if (day_ch[i] >= t_sun_v_hh && day_ch[i] < t_sun_z_hh)
      tft.setTextColor(TFT_YELLOW);
    else
      tft.setTextColor(TFT_CYAN);

    tft.setFreeFont(v6);
    tft.setCursor(x + 20, y + 14);
    tft.print(s);
    tft.drawFastHLine(x + 21, y + 18, 39, TFT_WHITE) ; //время подч

    if (i > 1) tft.drawFastVLine(x, y, 320 - y, TFT_WHITE) ; //верт разд линия


    //icon
    tft.fillRect(x + 15, y + 24, 48, 48, TFT_BLACK);
    drawJpeg(icon_ch[i] + "48.jpg", x + 15, y + 24);

    //rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr

    do_draw_rain(x + 10, y + 28, rain_ch[i], TFT_CYAN);
    if (rain_ch[i] >= rain_l)
      do_draw_rain_mm(x + 1, y + 78, rain_ch[i], TFT_CYAN);



    ////tttttttttttttttttttttttttttttttttttttttttt

    unsigned int col;
    col = color_t(temperatura_ch[i]);
    tft.setTextColor(col);
    byte b;
    if (temperatura_ch[i] < -9) b = 0;
    else if (temperatura_ch[i] < 0 || temperatura_ch[i] > 9) b = 10;
    else
      b = 20;

    tft.setCursor(x + b + 15, y + 100);
    tft.setFreeFont(v12);
    tft.print(temperatura_ch[i]);

    //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

    do_deg(v_gr_ch[i]);
    if (wind_ico != "")
      drawJpeg(wind_ico, x + 2, y + 115);

    tft.setFreeFont(v9);
    tft.setTextColor(TFT_CYAN);
    tft.setCursor(x + 35, y + 135);
    tft.print(v_ch[i]);
    if (v_ch[i] < 10)
    { tft.setFreeFont(v6);
      tft.setTextColor(TFT_WHITE);
      tft.print(" m/c");
    }



    x = x + x_step;

  }


  tft.drawFastHLine(0, 160, 240, TFT_WHITE) ;
  tft.drawFastHLine(0, 162, 240, TFT_WHITE) ;


}





//------------------------------------------------------------------------
//                      S1_show_4day
//------------------------------------------------------------------------
void S1_show_7day()
{
  int y = 160;
  int x = 240;
  int n;
  byte x_step = 60;


  tft.drawFastVLine(x, y, 319 - y, TFT_WHITE) ;
  tft.drawFastVLine(x + 2, y, 319 - y, TFT_WHITE) ;

  tft.drawFastHLine(x, y, 479 - x, TFT_WHITE) ;
  tft.drawFastHLine(x, y + 2, 479 - x, TFT_WHITE) ;


  for (byte i = 1; i < 5; i++) {

    for (byte ii = 3; ii <= x_step; ii++) {
      if (ii < x_step)
        tft.drawFastVLine(x + ii + 1, y + 3, 157, TFT_CYAN) ;
      tft.drawFastVLine(x + ii, y + 3, 157, TFT_BLACK) ;
      delay(2);
    }

    if (temperatura7[i] == 200)
    {
      x = x + x_step;
      continue;
    }


    drawJpeg(icon7[i] + "32.jpg", x + 15, y + 20);


    ///tttttttttttttt
    unsigned int col;
    byte b;


    col = color_t(temperatura7[i]);
    tft.setTextColor(col);

    if (temperatura7[i] < -9)
      b = 12;
    else if (temperatura7[i] < 0 || temperatura7[i] > 9)
      b = 20;
    else
      b = 27;

    tft.setFreeFont(v9);
    //tttttttttttttttttttttttttttttttttttt

    tft.setCursor(x + b, y + 65);
    tft.print(temperatura7[i]);

    //nnnnnnnnnnnnnnnnnnn
    if (temperatura7n[i] < -9)
      b = 12;
    else if (temperatura7n[i] < 0 || temperatura7[i] > 9)
      b = 20;
    else
      b = 27;
    tft.setCursor(x + b, y + 85);
    col = color_t(temperatura7n[i]);
    tft.setTextColor(col);
    tft.print(temperatura7n[i]);

    if (temperatura7[i] >= t_max || temperatura7n[i] >= t_max ||
        temperatura7[i] <= t_min || temperatura7n[i] <= t_min)
    {
      tft.fillRect(x + 17, y + 68, 30, 3, TFT_YELLOW);

    }
    else
      tft.drawFastHLine(x + 20, y + 68, 30, TFT_WHITE) ;



    //day//ddddddddddddddddddddddddddddddddd
    if (i > 1)
      tft.drawFastVLine(x, y, 120, c_date) ;

    if (day7[i] < 10)
      b = 22;
    else
      b = 18;
    //выходные выделяю
    tft.setTextColor(color_day(holiday7[i]));


    tft.setCursor(x + b, y + 18);
    tft.print((int)(day7[i]));




    //rainrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr

    do_draw_rain(x + 9, y + 29, rain7[i] / 8, TFT_CYAN);



    //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    float gr = int(v_gr_7[i]);
    do_deg(gr);
    if (wind_ico != "");
    drawJpeg(wind_ico, x + 5, y + 88);

    tft.setCursor(x + 33, y + 115);
    tft.setFreeFont(v6);
    tft.setTextColor(TFT_CYAN);
    tft.print(veter7[i]);
    if (veter7[i] >= v_max)
    {
      tft.fillRect(x + 32, y + 119, 10, 3, TFT_YELLOW);
    }

    x = x + x_step;
  }



}


#if defined (two_tft) && defined (tft2_S1)
#undef tft 
#endif


#endif //S1
