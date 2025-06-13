#if defined (S2) && defined(tft_480_320)&& defined (openw)

#if defined (two_tft) && defined (tft2_S2)
#define tft tft2
#endif



//------------------------------------------------------------
//                      S2_show_now()
//------------------------------------------------------------
void S2_show_now()
{
  int temp = 200;
  tft.fillRect(120, 196, 180, 123, TFT_BLACK);
  tft.drawFastVLine(300, 196, 123, TFT_CYAN) ;
  ///////////icon-WW--------------------------------------------
#ifdef debug
  Serial.print(F("icon today--->"));
  Serial.println(icon_ch[0] + "96.jpg");
#endif
  drawJpeg(icon_ch[0] + "96.jpg", 135, 195); //220

  //ttttttttttttttttttttttttttttttttttttttttt
  if (temperatura_ch[0] == 200 && temp_u == 200) return; //!=данные мз инета

  temp = temperatura_ch[0];


  if (temp_u != 200)
  {
    temp = temp_u;
    //    ref_temp_u = false;

  }

  tft.drawFastVLine(121, 196, 123, c_tu_frame) ;
  tft.drawFastVLine(120, 196, 123, c_tu_frame) ;

  if (temp != 200)
  {

    uint32_t color_temp = color_t(temp);
    tft.setTextColor(color_temp);
    if (temp < -9)
      tft.setFreeFont(v18);
    else
      tft.setFreeFont(v24b);

    tft.setCursor(232, 240);
    tft.print(int(temp));
    tft.drawFastHLine(232, 250, 60, TFT_CYAN) ;
    //nnnnnnnnnnnnnnnnnnnnnnnn
    if (temp_ch_n != 200)
    {
      color_temp = color_t(temp_ch_n);
      tft.setTextColor(color_temp);
      tft.setFreeFont(v18);
      tft.setCursor(235, 283);
      tft.print(temp_ch_n);


    }

  }




  //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
  float gr = int(v_gr_ch[0]);
  do_deg(gr);
  if (wind_ico != "")
  {
    drawJpeg(wind_ico, 125, 287); //220
  }

  //тенденция ветра
  if (v_ch[0] != 200) //есть данные мз инета
  {

    if (v_ch[0] < v_ch[1])
      draw_tr_up(160, 315);
    if (v_ch[0] > v_ch[1])
      draw_tr_down(160, 315);


    tft.setFreeFont(v12);
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.setCursor(170, 315);
    tft.print(v_ch[0]);
    if (v_ch[0] < 10)
    {
      tft.setFreeFont(v6);
      tft.setTextColor(TFT_WHITE);
      tft.print("m/s");
    }

  }


  //ppppppppppppppppppppppppppppppppppppppppppppppppppppppp
  if (davlenie0 != 200)
  {

    if (davlenie0 < davlenie7[1])
      draw_tr_up(230, 315);

    if (davlenie0 > davlenie7[1])
      draw_tr_down(230, 315);



#ifndef hPa
    tft.setFreeFont(v12);
#else
    tft.setFreeFont(v9);
#endif

    tft.setTextColor(c_pr_ow);
    tft.setCursor(240, 315);
    if (pr_kv != 200)
    {
      tft.setTextColor(c_pr_bme);
      tft.print(pr_kv);
    }
    else
      tft.print(davlenie0);


  }


  //rrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
  do_draw_rain(110, 200, rain_ch[0], TFT_CYAN);

}




//------------------------------------------------------------
//                      S2_show_wh()
//------------------------------------------------------------
void S2_show_wh()
{
  tft.fillRect(301, 196, 178, 123, TFT_BLACK);

  int y = 196;
  int x = 301;
  int tt;
  unsigned int col;
  //2222222222222222222222222222222222222222222222222222222222
  for (byte i = 1; i < 3; i++) {
    tt = temperatura_ch[i];


    if (tt == 200) continue;

    ////tttttttttttttttttttttttttttttttttttttttttt

    col = color_t(tt);
    tft.setTextColor(col);

    if (tt < -9)
    {
      tft.setFreeFont(v9);
      //icon
      drawJpeg(icon_ch[i] + "32.jpg", x + 7, y + 2);
      tft.setCursor(x + 40, y + 34);
    }
    else
    {
      tft.setFreeFont(v12);
      //icon
      drawJpeg(icon_ch[i] + "48.jpg", x + 7, y + 2);
      if (tt > 9 || tt < 0)
        tft.setCursor(x + 55, y + 34);
      else
        tft.setCursor(x + 60, y + 34);
    }

    tft.print(tt);


    //rain snowrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
    do_draw_rain(x + 6, y + 7, rain_ch[i], TFT_CYAN);

    //tft.drawFastHLine(x, 251, 86,color_day(holiday7[i]) ) ;



    x = x + 86;
    if (i == 1)
      tft.drawFastVLine(x - 1, y, 54, TFT_WHITE) ;

  }

  //tft.drawFastHLine(301, 250, 178, TFT_WHITE) ;

  //444444444444444444444444444444444444444444444444444

  y = 251;
  x = 301;
  for (byte i = 1; i < 5; i++) {
    tt = temperatura7[i];
    if (tt == 200) continue;
    //icon
    drawJpeg(icon7[i] + "32.jpg", x + 6, y + 2);
    tft.fillRect(x, y - 1, 41, 3, color_day(holiday7[i]) ) ;
    //rain snowrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
    do_draw_rain(x + 5, y + 6, rain7[i] / 8, TFT_CYAN);

    ////tttttttttttttttttttttttttttttttttttttttttt
    tft.setFreeFont(v9);
    col = color_t(tt);
    tft.setTextColor(col);
    byte b = 0;
    if (tt >= 0 || tt < 10)
      b = 16;
    else if (tt > 9)
      b = 8;

    tft.setCursor(x + b, y + 47);
    tft.print(tt);

    tft.drawFastHLine(x + b, y + 51, 20, TFT_WHITE) ;

    //nnnnnnnnnnnnnnnnnnnnn
    col = color_t(temperatura7n[i]);
    tft.setTextColor(col);
    tt = temperatura7n[i];

    b = 0;
    if (tt >= 0 || tt < 10)
      b = 16;
    else if (tt > 9)
      b = 8;

    tft.setCursor(x + b, y + 65);
    tft.print(tt);
    x = x + 44;
  }



}


//=================================================================================================
//                  S2_show_th()
//=================================================================================================

void S2_show_th()
{
#ifdef def_th
  tft.fillRect(0, 275, 119, 44, TFT_BLACK);
  tft.drawRoundRect(0, 275, 119, 45, 2, c_th_frame);

  if (temp_kv == 200 && h_kv == 200) return;



  tft.setTextColor(TFT_YELLOW);
  if (temp_kv != 200) //bme
  {
    tft.setFreeFont(v18);
    tft.setCursor(2, 312);
    tft.print(temp_kv);
    tft.setFreeFont(v6);
    tft.setTextColor(TFT_WHITE);
    tft.print("c ");
  }
  if (h_kv != 200) //bme
  {
    tft.setFreeFont(v18);
    tft.setTextColor(TFT_CYAN);
    //tft.setCursor(283,107);
    tft.print(h_kv);
    tft.setFreeFont(v6);
    tft.setTextColor(TFT_WHITE);
    tft.print("%");
  }

#endif //th
}



#if defined (two_tft) && defined (tft2_S2)
#undef tft
#endif

#endif //openw
