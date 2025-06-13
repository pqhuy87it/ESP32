#if defined (S1) && defined(tft_320_240)&& defined (openw)


//теущая погода
//=================================================================================================
//                             S1_show_now
//=================================================================================================

void S1_show_now()
{

  tft.fillRect(0, 114, 165, 51, TFT_BLACK);
  tft.drawFastHLine(0, 165, 165, TFT_CYAN) ;
  tft.drawFastHLine(0, 166, 165, TFT_CYAN) ;

  if (temperatura_ch[0] == 200 && temp_u == 200) return; //!=данные мз инета





  S_show_uv( true, 0, 115);

  int temp;
  temp = temperatura_ch[0];




  if (temp_u != 200)
  {
    temp = temp_u;
    ref_temp_u = false;

  }


  tft.drawFastVLine(165, 113, 127, c_tu_frame) ;
  tft.drawFastVLine(166, 113, 127, c_tu_frame) ;

  //tft.setFreeFont(v18b);
  tft.setFreeFont(v24);

  byte xx;


  if ((int)(temp) > 9)
  {
    xx = 0;
    drawJpeg(icon_ch[0] + "32.jpg", 80, 115); // текущая погода
  }
  else
  {
    xx = 10;
    drawJpeg(icon_ch[0] + "48.jpg", 75, 115); // текущая погода
  }

  tft.setTextColor(color_t(temp));
  tft.setCursor(xx + 11, 158);
  //tft.print((int)(temp));
  tft.print(abs(int(temp)));

  //nnnnnnnnnnnnnnnnnnnnnnnn
  if (temp_ch_n != 200)
  {
    tft.setTextColor(color_t(temp_ch_n));
    if (temp < -9) tft.setFreeFont(v6);
    else tft.setFreeFont(v9);
    tft.print(temp_ch_n);
  }


  if (temp < 0) //минус
  {
    tft.fillRect(xx, 135, 12, 6, color_t(temp));
    tft.drawRect(xx, 135, 12, 6, TFT_BLUE);

  }



  //rain rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
  do_draw_rain(74, 123, rain_ch[0], TFT_CYAN);
  if (rain_ch[0] >= rain_m) //rain_m
    do_draw_rain_mm(110, 135, rain_ch[0], TFT_CYAN);

  //========================================
  //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

  float gr = int(v_gr_ch[0]);
  do_deg(gr);
  if (wind_ico != "");
  drawJpeg(wind_ico, 121, 115);

  tft.setTextColor(TFT_CYAN);
  tft.setCursor(151, 130);
  if (v_ch[0] < 10)
    tft.setFreeFont(v9);
  else
    tft.setFreeFont(v6);

  tft.print(v_ch[0]);


  //ppppppppppppppppppppppppppppppppppppppppppppppppppppppp

  //123,140

  //тенеденция давления
  if (davlenie0 != 200 && davlenie7[1] != 200)
  {

    if (davlenie0 < davlenie7[1])
      draw_tr_up(122, 160);
    //tft.drawTriangle(128,160,125,148,122,160,TFT_YELLOW);
    if (davlenie0 > davlenie7[1])
      draw_tr_down(122, 160);

    //tft.drawTriangle(128,148,122,148,125,160,TFT_CYAN);
  }

#ifndef hPa
  tft.setFreeFont(v9);
#else
  tft.setFreeFont(v6);
#endif

  tft.setTextColor(c_pr_ow);
  tft.setCursor(130, 160);

  if (pr_kv != 200)
  {
    tft.setTextColor(c_pr_bme);
    tft.print(pr_kv);
  }
  else
    tft.print(davlenie0); //с поправкой


}



#endif
