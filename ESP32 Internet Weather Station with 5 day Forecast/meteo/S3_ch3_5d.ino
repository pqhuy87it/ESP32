#if defined (S3) && defined(tft_320_240)&& defined (openw)


#if defined (two_tft) && defined (tft2_S3)
#define tft tft2
#endif

void S3_show()
{
  tft.fillScreen(TFT_BLACK);
  S3_show_3ch();
  S3_show_moon();
  S3_show_5d();
  S3_graf_t();
  S3_graf_davlenie();

}


//=================================================
//                        S3_show_3ch
//=================================================

//3 часовой прогноз
void S3_show_3ch()
{




  int y = 0;
  int x = 0; //65;
  int n;
  String s = "";
  for (byte i = 1; i < 5; i++) {

    ////////////////////////////////////////////////

    if (temperatura_ch[i] == 200) continue;
    //dddddddddddddddddddddddddddddddddddddddddddddd
    if (day_ch[i] < 10)
      s = (" 0");
    else
      s = (" ");
    s = s + String(day_ch[i]);
    if (day_ch[i] >= t_sun_v_hh && day_ch[i] < t_sun_z_hh)
      tft.setTextColor(TFT_YELLOW);
    else
      tft.setTextColor(TFT_CYAN);
    tft.setFreeFont(v9);
    tft.setCursor(x + 22, y + 15);
    tft.print(s);
    tft.setFreeFont(v6);
    tft.print(":00");

    tft.drawFastHLine(x + 22, y + 18, 45, TFT_WHITE) ; //время подч
    if (i > 1)
    {
      tft.drawFastVLine(x, 0, 100, TFT_WHITE) ; //верт разд линия
      tft.drawFastVLine(x - 1, 0, 100, TFT_WHITE) ; //верт разд линия
    }
    //icon
    drawJpeg(icon_ch[i] + "48.jpg", x + 1, y + 20);




    //rain snowrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
    do_draw_rain(x + 5, y + 20, rain_ch[i], TFT_CYAN);

    if (rain_ch[i] >= rain_m)
      do_draw_rain_mm(x + 48, y + 30, rain_ch[i], TFT_CYAN);



    ////tttttttttttttttttttttttttttttttttttttttttt
    tft.setFreeFont(v12);
    unsigned int col;
    col = color_t(temperatura_ch[i]);
    tft.setTextColor(col);
    int tt = temperatura_ch[i];

    //tt=-12;
    if (tt < -9)
      tft.setCursor(x + 35, y + 55);
    else
      tft.setCursor(x + 45, y + 55);

    tft.print(tt);


    //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    float gr = int(v_gr_ch[i]);
    do_deg(gr);
    drawJpeg(wind_ico, x + 2, 65);

    tft.setFreeFont(v9b);
    tft.setTextColor(TFT_CYAN);
    tft.setCursor(x + 38, y + 88);
    tft.print(v_ch[i]);

    tft.setFreeFont(v6);
    tft.setTextColor(TFT_WHITE);

    tft.print("m");

    x = x + 80;

  }



  tft.drawFastHLine(0, 99, 320, TFT_CYAN);
  tft.drawFastHLine(0, 98, 320, TFT_CYAN);

}



//============================================================================================================
///                S3_show_moon()
//============================================================================================================
void S3_show_moon()
{


  tft.drawFastVLine(65, 100, 140, TFT_YELLOW) ; //верт z-v
  tft.drawFastVLine(66, 100, 140, TFT_YELLOW) ; //верт z-v

  //z-v luna
  //----------------------------------------------
  ////////////////восход зах.///////////////////////////////////
  tft.setFreeFont(v9);
  tft.fillTriangle(0, 115, 10, 115, 5, 110, TFT_YELLOW);
  tft.setTextColor(TFT_YELLOW);
  tft.setCursor(10, 118);
  tft.print(t_sun_v);
  //zahod
  tft.fillTriangle(0, 135, 10, 135, 5, 140, TFT_RED);
  tft.setTextColor(TFT_ORANGE);
  tft.setCursor(10, 141);
  tft.print(t_sun_z);
  tft.setFreeFont(v6);

  //---------------moon
  String fileName;
  fileName = do_phasemoon() + "_64.jpg";

  tft.drawFastHLine(0, 157, 65, TFT_YELLOW);
  drawJpeg(fileName, 0, 160);



}

//============================================================================================================
///                S3_show_5d()
//============================================================================================================
void S3_show_5d()
{

  int y = 101;
  int x = 64;
  int n;


  //===============================================
  for (byte i = 1; i <= last_7day; i++) {

    if (temperatura7[i] == 200) continue;

    if (i == 1) //pppppppppppppppppppppp
    {
      tft.setTextColor(0x07E0);
      tft.setFreeFont(v6);
      tft.setCursor(x + 5, y + 120);
      tft.print(p7_max);
      tft.setCursor(x + 5, y + 135);
      tft.print(p7_min);

      tft.setFreeFont(v6);

    }




    if (i > 4)  continue;
    drawJpeg(icon7[i] + "32.jpg", x + 12, y + 15);


    ///ttttttttttttttttttttttttttttttttttttttt
    unsigned int col;
    byte b;

    col = color_t(temperatura7[i]);
    tft.setTextColor(col);
    tft.drawFastHLine(x + 10, y + 60, 30, TFT_CYAN) ;

    //tttttttttttttttttttttttttttttttttttt
    if (temperatura7[i] < -9) //3
      b = 0;
    else if (temperatura7[i] < 0 || temperatura7[i] > 9) //2
      b = 7;
    else
      b = 12; //1

    tft.setCursor(x + 10 + b, y + 57);
    tft.setFreeFont(v9);
    tft.print(temperatura7[i]);
    if (temperatura7[i] >= t_max || temperatura7[i] <= t_min)
    {
      tft.fillRect(x + 10, y + 59, 30, 3, TFT_YELLOW);
    }




    //nnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn
    if (temperatura7n[i] != 200)
    {


      col = color_t(temperatura7n[i]);
      tft.setTextColor(col);
      if (temperatura7n[i] < -9) //3
        b = 0;
      else if (temperatura7n[i] < 0 || temperatura7n[i] > 9) //2
        b = 7;
      else
        b = 12;

      tft.setCursor(x + 10 + b, y + 76);
      tft.print(temperatura7n[i]);
      if (temperatura7n[i] >= t_max || temperatura7n[i] <= t_min)
      {
        tft.fillRect(x + 10, y + 59, 30, 3, TFT_YELLOW);
      }



    }




    //day//dddddddddddddddddddddddddddddddddddd

    if (day7[i] < 10)
      b = 20;
    else
      b = 13;
    //выходные выделяю................
    tft.setTextColor(color_day(holiday7[i]));


    //dddddddddddddddddd

    tft.setCursor(x + b, y + 15);
    tft.setFreeFont(v9b);
    tft.print(day7[i]);






    //rainrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
    do_draw_rain(x + 8, y + 24, rain7[i] / 8, TFT_CYAN);


    //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    float gr = int(v_gr_7[i]);
    do_deg(gr);
    drawJpeg(wind_ico, x + 12, y + 78);

    tft.setTextColor(TFT_CYAN);
    tft.setFreeFont(v6);
    tft.setCursor(x + 5, y + 98);
    tft.print(veter7[i]);
    tft.setFreeFont(v6);



    x = x + 64;
  }

  S3_graf_t();
  //do_graf_veter();
  S3_graf_davlenie();



}


//====================================================================
//                    S3_graf_davlenie
//====================================================================
void S3_graf_davlenie()
{


  if (davlenie7[0] == 200 || davlenie0 == 200) return;

  int y = 235;
  int x = 96;

  for (byte i = 1; i < last_7day; i++)
  {


    if (davlenie7[i] >= p_max || davlenie7[i] <= p_min)
      tft.fillCircle(x, y - ((davlenie7[i] - davlenie7[0]) * 2), 3, 0x07E0);


    if (davlenie7[i + 1] == 200)break;

    tft.drawLine(x, y - ((davlenie7[i] - davlenie7[0]) * 2), x + 64, y - (davlenie7[i + 1] - davlenie7[0]) * 2, 0x07E0);


    x = x + 64;
  }

}



//tttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttt
void S3_graf_t()
{
  if (temperatura7[0] == 200) return;

  unsigned int col;
  int y = 230;
  int x = 96;
  int y_min = temperatura7[0];





  for (byte i = 1; i < last_7day; i++)
  {


    if (temperatura7[i] >= t_max || temperatura7[i] <= t_min)
      tft.fillCircle(x, y - 2 * ((temperatura7[i] - y_min)), 3, col);

    if (temperatura7[i + 1] == 200) break;

    col = color_t(temperatura7[i]);


    tft.drawLine(x, y - 2 * ((temperatura7[i] - y_min)), x + 64, y - 2 * ((temperatura7[i + 1] - y_min)), col);
    tft.drawLine(x, y - 2 * ((temperatura7[i] - y_min)) - 1, x + 64, y - 2 * ((temperatura7[i + 1] - y_min)) - 1, col);



    x = x + 64;

  }
}


#if defined (two_tft) && defined (tft2_S3)
#undef tft //tft2
#endif




#endif //openw
