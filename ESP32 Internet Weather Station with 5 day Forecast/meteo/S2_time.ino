#if defined (S2) && defined(tft_320_240)&& defined (openw)
//time///////////////////////////////////////tttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttt


void S2_time()
{

  static byte last_min = 99; //чтобы не перирис минуты





  if (t % 2 == 0)
  {

    S2_alert(false);
    //ntp
    if (err_ntp == 0)
      tft.fillRoundRect(158, 155, 4, 20, 1, TFT_GREENYELLOW);
    else
      tft.fillRoundRect(158, 155, 4, 20, 1, TFT_RED);
  } //sec black------------------------------------
  else
  {
    S2_alert(true);
    //sec
    tft.fillRoundRect(158, 155, 4, 20, 1, TFT_BLACK);
  }


  if (ref_temp_u == true || ref_wh_now == true || refresh_all == true) //есть данные с улицы
    S2_show_now();


  if (ref_kv_th == true || refresh_all == true) S2_show_th();



  //если мин не изм  и не надо все обновить выходим
  //-----------------

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
      fileName = "/n0_176.jpg";
      drawJpeg(fileName, 0, 0); //220
      fileName = "/n" + String(now_hour) + "_176.jpg";
      drawJpeg(fileName, 80, 0); //220
    }
    else
    {
      st = String(now_hour);
      fileName = "/n" + st.substring(0, 1) + "_176.jpg";
      drawJpeg(fileName, 0, 0); //220
      fileName = "/n" + st.substring(1) + "_176.jpg";
      drawJpeg(fileName, 80, 0); //220

    }


  }



  //////////////////min

  if (now_min < 10)
  {
    fileName = "/n0_176.jpg";
    drawJpeg(fileName, 160, 0); //220
    fileName = "/n" + String(now_min) + "_176.jpg";
    drawJpeg(fileName, 240, 0); //220
  }
  else
  {
    st = String(now_min);
    fileName = "/n" + st.substring(0, 1) + "_176.jpg";
    drawJpeg(fileName, 160, 0); //220
    fileName = "/n" + st.substring(1) + "_176.jpg";
    drawJpeg(fileName, 240, 0); //220

  }



  //--------------------------------------------------------------------------------

  //дата--------data/////////ddddddddddddddddddddddddddddddddddddddddddddddd
  if (refresh_all == false && year(t) > 2015) //день недели может совпасть(01-01-), добавил год
    return;



  ////dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd
  tft.fillRect(0, 178, 103, 61, TFT_BLACK); //data
  tft.drawFastHLine(0, 176, 320, TFT_CYAN) ;
  tft.drawFastHLine(0, 178, 320, TFT_CYAN) ;


  String str = "";
  if (day(t) < 10)
    str = "0" + String(day(t));
  else
    str = String(day(t));

  tft.setFreeFont(v18b);
  tft.setTextColor(c_date);
  tft.setCursor(5, 207);
  tft.print(str);

  //tttttttttthhhhhhhhhhhhhhhhhhhhh
  if (temp_kv == 200 && h_kv == 200)
  {
    tft.setFreeFont(v12);
    if (month(t) < 10)
      str = ".0" + String(month(t));
    else
      str = "." + String(month(t));
    tft.setTextColor(c_month);
    tft.print(str);
  }
  else
    S2_show_th();


  //день недели-------------------------
  draw_week_day(10, 208);






}


//====================================================
//                S2_alert
//====================================================

void S2_alert(bool sh)
{

  if (sh == true)
  {
    //rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
    if (rain_ch[0] != 200)
      if (rain_ch[1] > rain_l)
        tft.fillCircle(112, 230, 3, TFT_CYAN);

    if (rain_ch[0] >= rain_m * 2)
      do_draw_rain(110, 200, rain_ch[0], TFT_CYAN);


    //alert  ttttttttttttttttttttttttttttttttttttttttttt
    if (temperatura_ch[0] != 200)
    {

      if (temperatura_ch[0] >= t_max || temperatura_ch[1] >= t_max || temp_ch_n >= t_max) //t alert
        tft.fillRect(166, 182, 57, 3, TFT_YELLOW);


      if (temperatura_ch[0] <= t_min || temperatura_ch[1] <= t_min || temperatura_ch[2] <= t_min || temp_ch_n <= t_min) //t alert
        tft.fillRect(166, 182, 57, 3, TFT_CYAN);
    }

    //vvvvvvvvvvvvvvvvvvvvvvvvvvv
    if (v_ch[0] != 200)
    {
      if (v_ch[0] >= v_max || v_ch[1] >= v_max) //1,2 период по 3ч
        tft.fillRect(285, 207, 30, 2, TFT_CYAN);
    }

    //ppppppppppppppppppppppppppppppppppp
    int pp;
    if (pr_kv != 200) pp = pr_kv;
    else
      pp = davlenie0;
    if (pp != 200)
    {
      if (pp >= p_max || pp <= p_min)
        tft.fillRect(285, 230, 32, 2, TFT_GREEN);

    }


    //////uv
    if (uv >= uv_max)
      S_show_uv( false, 108, 181);
  }
  else //------------------------------------------------------
  {

    //uuuuuuuuuuuuuuuuuuu
    S_show_uv( true, 108, 181);


    //ppppppppppppppppppppppppppppppppp
    tft.fillRect(285, 230, 32, 2, TFT_BLACK);

    //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    tft.fillRect(285, 207, 30, 2, TFT_BLACK);

    //rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
    tft.fillCircle(112, 230, 3, TFT_BLACK); //BLACK
    if (rain_ch[0] >= rain_m * 2)
      do_draw_rain(110, 200, rain_ch[0], TFT_BLACK);


    //tttttttttttt
    tft.fillRect(166, 182, 57, 3, TFT_BLACK);



  }

}





#endif
