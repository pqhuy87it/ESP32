#if defined (S4) && defined(tft_320_240)&& defined (openw)


#if defined (two_tft) && defined (tft2_S4)
#define tft tft2
#endif




//--------------------------------------------------------
//		 S4_show_ch
//--------------------------------------------------------
//15x3 часовой прогноз


void S4_show_ch()
{
  byte y = 0;
  int x = 0;
  int n;
  String s = "";
  byte step = 35;


  //if (tft2_act)
  tft.fillScreen(TFT_BLACK);

  tft.drawFastHLine(0, y, 320, TFT_WHITE) ;
  tft.drawFastHLine(0, y + 15, 320, TFT_WHITE) ;

  //----------------------------------
  for (byte i = 1; i < 19; i++)
  {
    if (temperatura_ch[i] == 200) continue;


    //ddddddddddddddddddddddddddddddddddddddddddd
    if (day_ch[i] < 10)
      s = (" 0");
    else
      s = (" ");
    s = s + String(day_ch[i]);
    if (day_ch[i] >= t_sun_v_hh && day_ch[i] < t_sun_z_hh)
    {
      tft.drawRect(x + 1, y + 1, step - 1, 14, TFT_YELLOW);
      tft.setTextColor(TFT_YELLOW);
    }
    else
      tft.setTextColor(TFT_WHITE);

    tft.setFreeFont(v6);
    tft.setCursor(x + 8, y + 12);
    tft.print(s);


    if ((day_ch[i] == 0 || day_ch[i] == 1 || day_ch[i] == 2) && x > 0)
    {
      tft.drawFastVLine(x, y, 113, TFT_WHITE) ; //верт разд линия
      tft.drawFastVLine(x - 1, y, 113, TFT_WHITE) ; //верт разд линия
    }
    else if ((day_ch[i] == 12 || day_ch[i] == 13 || day_ch[i] == 14) && x > 0) //поправка на разные пояса
    {
      tft.drawFastVLine(x, y, 118, TFT_YELLOW) ; //верт разд линия
      tft.drawFastVLine(x - 1, y, 118, TFT_YELLOW) ; //верт разд линия
    }
    else if (x > 0) tft.drawFastVLine(x, y, 65, TFT_LIGHTGREY) ; //верт разд линия


    //jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj
    drawJpeg(icon_ch[i] + "32.jpg", x + 2, y + 22);


    //rain snowrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
    do_draw_rain(x + 4, y + 22, rain_ch[i], TFT_CYAN);
    if (rain_ch[i] >= rain_l)
      do_draw_rain_mm(x + 5, y + 58, rain_ch[i], TFT_CYAN);

    //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    float gr = int(v_gr_ch[i]);
    do_deg(gr);
    if (wind_ico != "")
    {
      drawJpeg(wind_ico, x + 1, y + 81);
    }
    tft.setFreeFont(v6);
    tft.setTextColor(TFT_CYAN);
    if (wind_ico == "/wind_10.jpg" || wind_ico == "/wind_2.jpg")
      tft.setCursor(x + 18, y + 113);
    else
      tft.setCursor(x + 3, y + 113);
    tft.print(v_ch[i]);



    //TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT
    unsigned int col;
    col = color_t(temperatura_ch[i]);
    tft.setTextColor(col);
    int tt = temperatura_ch[i];
    tft.setFreeFont(v9);
    if (tt < -9)
      tft.setCursor(x, y + 81);
    else
      tft.setCursor(x + 7, y + 81);
    tft.print(tt);



    x = x + step;
    if (x + step > 319)
    {
      x = 0;
      //y=117;
      y = 120;
      tft.drawFastHLine(0, y, 320, TFT_WHITE) ; //half
      tft.drawFastHLine(0, y + 15, 320, TFT_WHITE) ; //half
    }



  }




  tft.drawFastHLine(0, 239, 320, TFT_WHITE) ;

}



#if defined (two_tft) && defined (tft2_S4)
#undef tft tft2
#endif



#endif //openw
