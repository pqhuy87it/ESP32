#if defined (S1) && defined(tft_320_240)&& defined (openw)
//3333333333333333333333333--------------------------------------------------------
//3 часовой прогноз

void S1_show_3ch()
{

  int y = 165;
  int x = 0;
  //int n;
  tft.fillRect(0, 167, 165, 73, TFT_BLACK);

  String s = "";
  for (byte i = 1; i < 3; i++) {

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
    tft.setCursor(x + 40, y + 14);
    tft.print(s);
    tft.drawFastHLine(x + 43, y + 18, 36, TFT_WHITE) ; //время подч

    if (i > 1) tft.drawFastVLine(x, 165, 75, TFT_WHITE) ; //верт разд линия


    //icon

    if (rain_ch[i] > 0)
      drawJpeg(icon_ch[i] + "48.jpg", x + 8, y + 24);
    else
      drawJpeg(icon_ch[i] + "48.jpg", x + 2, y + 24);




    //rain rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
    do_draw_rain(x + 5, y + 30, rain_ch[i], TFT_CYAN);

    if (rain_ch[i] >= rain_m)
      do_draw_rain_mm(x + 4, 238, rain_ch[i], TFT_CYAN);


    ////tttttttttttttttttttttttttttttttttttttttttt

    unsigned int col;
    col = color_t(temperatura_ch[i]);
    tft.setTextColor(col);
    if (temperatura_ch[i] > -10)
    {
      tft.setCursor(x + 48, y + 42);
      tft.setFreeFont(v12);
    }
    else
    {
      tft.setCursor(x + 48, y + 37);
      tft.setFreeFont(v9);
    }
    tft.print(temperatura_ch[i]);

    //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

    do_deg(v_gr_ch[i]);
    if (wind_ico != "")
      drawJpeg(wind_ico, x + 50, 208);

    tft.setFreeFont(v6);

    tft.setTextColor(TFT_CYAN);
    if (v_ch[i] > 9)
      tft.setCursor(x + 35, 237);
    else
      tft.setCursor(x + 43, 237);
    tft.print(v_ch[i]);




    x = x + 83;

  }


  //zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz

  tft.setFreeFont(v6);

  tft.setTextColor(TFT_YELLOW);
  tft.setCursor(0, y + 12);
  tft.print(t_sun_v);
  //zahod
  tft.setTextColor(TFT_ORANGE);
  tft.setCursor(0, y + 24);
  tft.print(t_sun_z);
  tft.setFreeFont(v6);


}




//==========================================================================================
//                        S1_show_4d
//==========================================================================================
void S1_show_5d()
{
  int y = 115;
  int x = 170;
  int n;

  tft.fillRect(166, 116, 154, 124, TFT_BLACK);

  tft.drawFastVLine(166, 116, 124, TFT_CYAN) ;
  tft.drawFastHLine(170, y + 17, 150, TFT_WHITE) ;



  for (byte i = 1; i < 5; i++) {


    if (temperatura7[i] == 200) continue;
    drawJpeg(icon7[i] + "32.jpg", x + 5, y + 20);


    ///tttttttttttttt
    unsigned int col;
    byte b;


    col = color_t(temperatura7[i]);
    tft.setTextColor(col);

    if (temperatura7[i] >= 0 && temperatura7[i] < 10)
      b = 13;
    else if (temperatura7[i] < -9)
      b = 0;
    else
      b = 7;

    tft.setFreeFont(v9);
    //tttttttttttttttttttttttttttttttttttt

    tft.setCursor(x + b, y + 60);
    tft.print(temperatura7[i]);
    //tft.setFreeFont(v6);
    //tft.setCursor(x+b+4,y+77);
    //tft.setFreeFont(v8);

    if (temperatura7n[i] == 200) continue;
    tft.setCursor(x + b, y + 81);
    col = color_t(temperatura7n[i]);
    tft.setTextColor(col);
    tft.print(temperatura7n[i]);

    if (temperatura7[i] >= t_max || temperatura7n[i] >= t_max ||
        temperatura7[i] <= t_min || temperatura7n[i] <= t_min)
    {
      tft.fillRect(x + 5, y + 63, 30, 3, TFT_YELLOW);

    }
    else
      tft.drawFastHLine(x + 5, y + 65, 30, TFT_WHITE) ;



    //day//ddddddddddddddddddddddddddddddddd
    if (i > 1)
      tft.drawFastVLine(x, y, 17, c_date) ;

    if (day7[i] < 10)
      b = 15;
    else
      b = 10;
    //выходные выделяю................

    tft.setTextColor(color_day(holiday7[i]));

    tft.setCursor(x + b, y + 13);
    tft.print(day7[i]);



    //rainrrrrrrrrrrrrrrrrrrrrrrrrrrrr
    do_draw_rain(x + 3, y + 26, rain7[i] / 8, TFT_CYAN);

    //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    float gr = int(v_gr_7[i]);
    do_deg(gr);
    if (wind_ico != "");
    drawJpeg(wind_ico, x + 5, 208);

    tft.setCursor(x + 2, 237);
    tft.setFreeFont(v6);
    tft.setTextColor(TFT_CYAN);
    tft.print(veter7[i]);
    tft.setFreeFont(v6);

    x = x + 37;
  }



}




//========================================================================================
//                  S1_graf_davlenie
//========================================================================================

////pppppppppppppppppppppppppppppppppppppppppppppppppppp
void S1_graf_davlenie()
{


  int y = 215;
  int x = 170;
  int x_pmin = x;
  int davlenie_x = 1000;

  if (davlenie7[0] == 200 || davlenie0 == 200) return;
  tft.drawLine(x, y - ((davlenie0 - davlenie7[0])), x + 18, y - (davlenie7[1] - davlenie7[0]), 0x07E0);


  x = x + 18;

  for (byte i = 1; i <= 4; i++)
  {



    if (davlenie7[i] >= p_max || davlenie7[i] <= p_min)
      tft.fillCircle(x, y - ((davlenie7[i] - davlenie7[0])), 2, 0x07E0);

    if (davlenie_x > davlenie7[i])
    {
      davlenie_x = davlenie7[i];
      x_pmin = x;
    }
    if (davlenie7[i + 1] == 200) break;




    tft.drawLine(x, y - ((davlenie7[i] - davlenie7[0])), x + 36, y - (davlenie7[i + 1] - davlenie7[0]), 0x07E0);

    x = x + 36;
  }
  tft.setTextColor(0x07E0);
  tft.setFreeFont(v6);
  tft.setCursor(x_pmin, 207);
  tft.print(p7_min);
  tft.setCursor(x_pmin, 220);
  tft.print(p7_max);


}




//===================================================================================
//                                    S1_graf_t
//===================================================================================
void S1_graf_t()
{
  if (temperatura7[0] == 200) return;

  unsigned int col;

  int y = 225;
  int x = 188;
  int y_min = temperatura7[0];


  for (byte i = 1; i <= 3; i++)
  {

    if (temperatura7[i] >= t_max || temperatura7[i] <= t_min)
      tft.fillCircle(x, y - 2 * ((temperatura7[i] - y_min)), 3, col);

    if (temperatura7[i + 1] == 200) break;

    //col=color_t(temperatura7[i]);

    col = TFT_YELLOW;

    tft.drawLine(x, y - 2 * ((temperatura7[i] - y_min)), x + 36, y - 2 * ((temperatura7[i + 1] - y_min)), col);


    x = x + 36;

  }
}




#endif //openw
