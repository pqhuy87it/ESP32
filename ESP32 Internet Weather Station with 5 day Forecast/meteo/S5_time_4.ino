#if defined (S5) && defined(tft_480_320)&& defined (openw)



//-----------------------------------------------------------------------
//                        S5_time
//-----------------------------------------------------------------------
void S5_time()
{

  static byte last_min = 99; //чтобы не перирис минуты

  if (t % 2 == 0)
  {

    S5_alert(false);
    //ntp
    if (err_ntp == 0)
      tft.fillCircle(135, 135, 4, TFT_GREENYELLOW);
    else
      tft.fillCircle(135, 135, 4, TFT_RED);

  } //sec black------------------------------------
  else
  {
    S5_alert(true);
    //sec
    tft.fillCircle(135, 135, 4, TFT_BLACK);

  }



  if (ref_temp_u == true || ref_wh_now == true || refresh_all == true) //есть данные now, Th
    S5_show_now();

  if (ref_wh == true || refresh_all == true) //обновилась погода
    S5_show_wh();


  if (ref_kv_th == true || refresh_all == true)
    S5_show_th();


  if   ( refresh_all == true) draw_Calendar(270, 10, 200, 0);



  if (last_min == now_min && refresh_all == false) {
    return;
  }
  last_min = now_min;




  //////////////hour///hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh
  String st;
  String fileName;

  if (refresh_all || now_min == 0)
  {
    if (now_hour < 10)
    {
      fileName = "/n0_112.jpg";
      drawJpeg(fileName, 0, 30); //220
      fileName = "/n" + String(now_hour) + "_112.jpg";
      drawJpeg(fileName, 64, 30); //220
    }
    else
    {
      st = String(now_hour);
      fileName = "/n" + st.substring(0, 1) + "_112.jpg";
      drawJpeg(fileName, 0, 30); //220
      fileName = "/n" + st.substring(1) + "_112.jpg";
      drawJpeg(fileName, 64, 30); //220

    }


  }



  //////////////////min

  if (now_min < 10)
  {
    fileName = "/n0_112.jpg";
    drawJpeg(fileName, 140, 30); //220
    fileName = "/n" + String(now_min) + "_112.jpg";
    drawJpeg(fileName, 204, 30); //220
  }
  else
  {
    st = String(now_min);
    fileName = "/n" + st.substring(0, 1) + "_112.jpg";
    drawJpeg(fileName, 140, 30); //220
    fileName = "/n" + st.substring(1) + "_112.jpg";
    drawJpeg(fileName, 204, 30); //220

  }





  //дата--------data/////////ddddddddddddddddddddddddddddddddddddddddddddddd
  if (refresh_all == false && year(t) > 2015) //день недели может совпасть(01-01-), добавил год
    return;



  ////dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd
  //tft.fillRect(0,195,103,60,TFT_BLACK); //data
  tft.drawFastHLine(0, 194, 480, TFT_CYAN) ;
  tft.drawFastHLine(0, 195, 480, TFT_CYAN) ;


  String str = "";
  if (day(t) < 10)
    str = "0" + String(day(t));
  else
    str = String(day(t));

  tft.setFreeFont(v24b);
  tft.setTextColor(c_date);

#ifdef def_th
  byte b = 0;
#else
  byte b = 20;
#endif

  tft.setCursor(5, 240 + b);
  tft.print(str);

  tft.setFreeFont(v12);
  if (month(t) < 10)
    str = ".0" + String(month(t));
  else
    str = "." + String(month(t));
  tft.setTextColor(c_month);
  tft.print(str);
  //#endif


  //день недели-------------------------
  draw_week_day(40, 241 + b);

  //moon
  fileName = do_phasemoon() + "_32.jpg";
  drawJpeg(fileName, 10, 155);

  //zzzzzzzzzzzzzzzzzzz
  tft.setFreeFont(v12);
  tft.setTextColor(TFT_YELLOW);
  tft.setCursor(70, 180);
  tft.print(t_sun_v);
  //zahod
  tft.setTextColor(TFT_ORANGE);
  tft.setCursor(170, 180);
  tft.print(t_sun_z);

}




//====================================================
//                S5_alert
//====================================================

void S5_alert(bool sh)
{

  if (sh == true)
  {

    //alert  ttttttttttttttttttttttttttttttttttttttttttt
    if (temperatura_ch[0] != 200)
    {

      if (temperatura_ch[0] >= t_max || temperatura_ch[1] >= t_max || temp_ch_n >= t_max) //t alert
        tft.drawFastHLine(232, 249, 60, TFT_YELLOW) ;

      if (temperatura_ch[0] <= t_min || temperatura_ch[1] <= t_min || temperatura_ch[2] <= t_min || temp_ch_n <= t_min) //t alert
        tft.drawFastHLine(232, 249, 60, TFT_CYAN) ;

    }

    //vvvvvvvvvvvvvvvvvvvvvvvvvvv
    if (v_ch[0] != 200)
    {
      if (v_ch[0] >= v_max || v_ch[1] >= v_max) //1,2 период по 3ч
        tft.fillRect(170, 316, 20, 3, TFT_CYAN);
    }

    //ppppppppppppppppppppppppppppppppppp
    int pp;
    if (pr_kv != 200) pp = pr_kv;
    else
      pp = davlenie0;
    if (pp != 200)
    {
      if (pp >= p_max || pp <= p_min)
        tft.fillRect(240, 316, 42, 3, TFT_GREEN);

    }


    //////uv
    if (uv >= uv_max)
      S_show_uv( false, 122, 198);
  }
  else //------------------------------------------------------
  {

    //uuuuuuuuuuuuuuuuuuu
    S_show_uv( true, 122, 198);


    //ppppppppppppppppppppppppppppppppp
    tft.fillRect(240, 316, 42, 3, TFT_BLACK);
    //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    tft.fillRect(170, 316, 20, 3, TFT_BLACK);

    //tttttttttttt
    tft.drawFastHLine(232, 249, 60, TFT_BLACK) ;


  }

}





#endif
