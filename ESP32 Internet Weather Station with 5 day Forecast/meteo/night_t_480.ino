#if defined (Sn) && defined(tft_480_320)

void Sn_time()
{
  static byte last_min = 99; //чтобы не перирис минуты

  if (t % 2 == 0)
  {
    if (err_ntp == 0)
    {
      tft.fillRect(220, 60, 20, 20, Sn_color_sec);
      tft.fillRect(220, 180, 20, 20, Sn_color_sec);
    }
    else
    {
      tft.fillRect(220, 60, 20, 20, TFT_RED);
      tft.fillRect(220, 180, 20, 20, TFT_RED);

    }

  }
  else
  {
    tft.fillRect(220, 60, 20, 20, TFT_BLACK);
    tft.fillRect(220, 180, 20, 20, TFT_BLACK);
  }



  if (last_min == now_min && refresh_all == false && !ref_temp_u && !ref_kv_th)
  {
    return;
  }
  last_min = now_min;





  String st;
  String fileName;
  //////////////hour
  if (now_hour < 10)
  {
    fileName = "/n0n.jpg";
    drawJpeg(fileName, 0, 0); //220

    fileName = "/n" + String(now_hour) + "n.jpg";
    drawJpeg(fileName, 100, 0);
  }
  else
  {
    st = String(now_hour);
    fileName = "/n" + st.substring(0, 1) + "n.jpg";
    drawJpeg(fileName, 0, 0); //220
    fileName = "/n" + st.substring(1) + "n.jpg";
    drawJpeg(fileName, 100, 0);


  }


  //////////////////min///////////////////////////
  if (now_min < 10)
  {
    fileName = "/n0n.jpg";
    drawJpeg(fileName, 250, 0); //220

    fileName = "/n" + String(now_min) + "n.jpg";
    drawJpeg(fileName, 350, 0); //220

  }
  else
  {
    st = String(now_min);
    fileName = "/n" + st.substring(0, 1) + "n.jpg";
    drawJpeg(fileName, 250, 0); //220

    fileName = "/n" + st.substring(1) + "n.jpg";
    drawJpeg(fileName, 350, 0);

  }


  if (refresh_all || ref_temp_u || ref_kv_th || ref_wh_now)
  {
    if (ref_wh_now)  refresh_all = true; //ref all
    do_show_th_n();

  }


}
#endif




#if defined(tft_480_320)
//-------------------------------------------------------------------
//          do_show_th_n
//-------------------------------------------------------------------
void do_show_th_n()
{

  if (refresh_all) tft.fillRect(0, 254, 479, 65, TFT_BLACK);

  tft.drawFastHLine(0, 254, 479, TFT_GREENYELLOW) ;




  int  temp = 200;

#ifdef openw
  temp = temperatura_ch[0];
#endif //openw

  int color = TFT_LIGHTGREY;

  if (temp_u != 200)
  {
    temp = temp_u;
    color = c_tu_frame;
  }



  if (temp != 200)
  {
    tft.drawRect(225, 256, 4, 63, color) ;
    tft.setFreeFont(v24b);
    tft.setTextColor(color_t(temp));
#ifdef openw
    if (!refresh_all) tft.fillRect(305, 255, 105, 64, TFT_BLACK);
    tft.setCursor(305, 309);
#else
    if (!refresh_all) tft.fillRect(262, 255, 105, 64, TFT_BLACK);
    tft.setCursor(262, 309);
#endif
    tft.print(temp);
  }




  // if (temp_kv != 200 || h_kv != 200)
  S_show_th_room(421, 257);

  if (!refresh_all) return;

  //---------------------------------------------

#ifdef openw
  if (rain_ch[0] > 0)
    do_draw_rain(240, 274, rain_ch[0], TFT_CYAN);
  //icon
  drawJpeg(icon_ch[0] + "48.jpg", 250, 269); // текущая погода
  //vvvvvvvvvvv
#ifndef def_th
  int gr = int(v_gr_ch[0]);
  do_deg(gr);
  drawJpeg(wind_ico, 429, 259);
  tft.setCursor(429, 312);
  tft.setTextColor(TFT_CYAN);
  tft.setFreeFont(v12);
  tft.print(v_ch[0]);
#endif //th 
#endif //openw


  //pppppppppppppppppppppppppppppp
#ifndef openw
  if (pr_kv != 200)
  {
    tft.setTextColor(c_pr_bme);
    tft.setFreeFont(v12);
    tft.setCursor(367, 309);
    tft.print(pr_kv);
  }
#endif



  //dddddddddddddddddddddddddddddddddddddddddddddddddddd
  String st = "";
  if (day(t) < 10)
    st = "0" + String(day(t));
  else
    st = String(day(t));

  tft.setFreeFont(v24b);
  tft.setTextColor(c_date);
  tft.setCursor(0, 314);
  tft.print(st);

  if (month(t) < 10)
    st = "0" + String(month(t));
  else
    st = String(month(t));
  tft.setFreeFont(v18b);
  st = "." + st;
  tft.setTextColor(c_month);
  tft.print(st);

  //moon
  String fileName = do_phasemoon() + "_32.jpg";;
  drawJpeg(fileName, 180, 255);

  byte wd = weekday(t) - 1;
  if (wd == 0) wd = 7;

#ifdef EN
  drawJpeg("/w" + String(wd) + "e.jpg", 150, 287); //220
#else
  drawJpeg("/w" + String(wd) + ".jpg", 150, 287); //220
#endif


  //------------------------------


}





#endif
