#if defined (S1) && defined(tft_480_320)&& defined (openw)
//теущая погода
///////////////////////////////ssssssssssssssssssssssss

#if defined (two_tft) && defined (tft2_S1)
#define tft tft2
#endif


void S1_show_now()
{
  int x = 285;
  int y = 0;

  for (int ii = x; ii <= 479; ii++) {
    if (ii < 479)
      tft.drawFastVLine(ii + 1, y, 159, TFT_CYAN) ;
    tft.drawFastVLine(ii, y, 159, TFT_BLACK) ;
    delay(2);
  }


  if (temperatura_ch[0] == 200 && temp_u == 200) return; //!=данные мз инета

  //icon
  drawJpeg(icon_ch[0] + "96.jpg", x + 15, y + 10); // текущая погода


  //rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
  do_draw_rain(x + 11, y + 33, rain_ch[0], TFT_CYAN);
  if (rain_ch[0] >= rain_l)
    do_draw_rain_mm(x + 4, y + 102, rain_ch[0], TFT_CYAN);



  int temp;
  temp = temperatura_ch[0];


  if (temp_u != 200) //<30min
  {
    temp = temp_u;
    ref_temp_u = false;

  }

  tft.drawFastVLine(x, 0, 159, c_tu_frame) ;
  tft.drawFastVLine(x + 1, 0, 159, c_tu_frame) ;


  tft.setFreeFont(v24b);
  int  xx;


  if (temp < -9) xx = 0;
  else xx = 10;
  //tttttttttttttttttttttttttttttttttttttttttttttttttttttttt
  tft.setTextColor(color_t(temp));
  tft.setCursor(x + xx + 11, y + 143);
  tft.print(temp);

  //nnnnnnnnnnnnnnnnnnnnnnnn
  if (temp_ch_n != 200)
  {
    tft.setFreeFont(v12);
    if (temp > -10)
    {
      tft.setTextColor(TFT_BLACK, TFT_BLACK),
                       tft.print("/");
    }

    tft.setTextColor(color_t(temp_ch_n));

    tft.print(temp_ch_n);
  }

  //========================================
  //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

  int gr = int(v_gr_ch[0]);
  do_deg(gr);
  //if (wind_ico!="");
  drawJpeg(wind_ico, x + 110, y + 10);

  tft.setCursor(x + 150, y + 40);
  tft.setTextColor(TFT_CYAN);
  tft.setFreeFont(v12);
  tft.print(v_ch[0]);



  if (v_ch[0] < 10)
  {
    tft.setTextColor(TFT_WHITE);
    tft.setFreeFont(v6);
    tft.print(" m/c");
  }
  //return;

  //ppppppppppppppppppppppppppppppppppppppppppppppppppppppp
  xx = x + 110;
  int yy = y + 60;

  //тенеденция давления
  if (davlenie0 != 200 && davlenie7[1] != 200)
  {
    if (davlenie0 < davlenie7[1])
      draw_tr_up(xx, yy + 20);
    if (davlenie0 > davlenie7[1])
      draw_tr_down(xx, yy + 20);
  }

  tft.setFreeFont(v12);
  tft.setTextColor(c_pr_ow);
  tft.setCursor(xx + 10, yy + 20);

  if (pr_kv != 200)
  {
    tft.setTextColor(c_pr_bme);
    tft.print(pr_kv);
  }
  else if (davlenie0 != 200)
    tft.print(davlenie0);



#ifndef hPa
  tft.setTextColor(TFT_WHITE);
  tft.setFreeFont(v6);
  tft.print("mm");
#endif


}


#if defined (two_tft) && defined (tft2_S1)
#undef tft
#endif

#endif //S1
