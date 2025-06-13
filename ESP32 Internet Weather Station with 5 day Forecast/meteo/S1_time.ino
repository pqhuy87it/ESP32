#if defined (S1) && defined(tft_320_240)&& defined (openw)
//time///////////////////////////////////////tttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttt
void S1_time()
{
  static byte last_min = 99;

  if (t % 2 == 0)
  {
    if (err_ntp == 0)
      tft.fillCircle(135, 105, 4, TFT_GREENYELLOW);
    else
      tft.fillCircle(135, 105, 4, TFT_RED);

    S1_alert(false);
  }
  else
  {
    S1_alert(true);
    //sec
    tft.fillCircle(135, 105, 4, TFT_BLACK);
  }

  if (ref_wh == true || refresh_all == true) //обновилась погода
  {
    S1_show_3ch();
    S1_show_5d();
    S1_graf_davlenie();
    S1_show_now();
  }
  else if (ref_temp_u == true || ref_wh_now == true) //есть данные с улицы
  {
    S1_show_now();

  }


  //MOON mmmmmmmmmmmmmmmm

  S1_th_moon(270, 72);



  //если мин не изм  и не надо все обновить выходим
  if (last_min == now_min && refresh_all == false) {
    return;
  }

  //-----------------------------------------------------------------
  last_min = now_min;


  //hour///hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh
  String st, fileName ;
  if (refresh_all || now_min == 0)
  {

    if (now_hour < 10)
    {
      fileName = "/n0_112.jpg";
      drawJpeg(fileName, 0, 0); //220

      fileName = "/n" + String(now_hour) + "_112.jpg";
      drawJpeg(fileName, 64, 0); //220
    }
    else
    {
      st = String(now_hour);
      fileName = "/n" + st.substring(0, 1) + "_112.jpg";
      drawJpeg(fileName, 0, 0); //220

      fileName = "/n" + st.substring(1) + "_112.jpg";
      drawJpeg(fileName, 64, 0); //220

    }

  }

  //mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm

  if (now_min < 10)
  {
    fileName = "/n0_112.jpg";
    drawJpeg(fileName, 140, 0); //220
    fileName = "/n" + String(now_min) + "_112.jpg";
    drawJpeg(fileName, 204, 0); //220

  }
  else
  {
    st = String(now_min);
    fileName = "/n" + st.substring(0, 1) + "_112.jpg";
    drawJpeg(fileName, 140, 0); //220
    fileName = "/n" + st.substring(1) + "_112.jpg";
    drawJpeg(fileName, 204, 0);

  }

  //полоса поперек времени, если не надо закомм. след. 2 строки
  //tft.drawFastHLine(0,55,270,TFT_BLACK) ;
  //tft.drawFastHLine(0,56,270,TFT_BLACK) ;




  //дата----data/ddddddddddddddddddddddddddddddddddddddddddddddd
  if (refresh_all == false && year(t) > 2015)
    return;



  /////day
  tft.fillRect(270, 0, 50, 112, TFT_BLACK);

  //wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww
  draw_week_day(270, 47);

  tft.setTextColor(c_date);
  tft.setFreeFont(v18b);

  String str = "";
  if (day(t) < 10)
    str = "0" + String(day(t));
  else
    str = String(day(t));
  tft.setCursor(270, 30);
  tft.print(str);
  tft.drawFastHLine(270, 33, 50, TFT_CYAN) ;

  if (month(t) < 10)
    str = "0" + String(month(t));
  else
    str = String(month(t));
  tft.setFreeFont(v9);
  tft.setTextColor(c_month);
  tft.setCursor(284, 47);
  tft.print(str);



  S1_th_moon(270, 72);


  tft.drawFastHLine(0, 113, 320, TFT_CYAN) ;

}





//--------------------------------------------------------------
//        S1_th_moon()
//--------------------------------------------------------------
void S1_th_moon(int x, int y)
{
  static uint32_t t_next_moon = 0;
  static uint32_t t_next_th = 0;
  static bool ref_moon = true;
  static bool ref_th = true;
  static bool show_moon = true;

  if (refresh_all == true || ref_temp_u == true || ref_wh_now == true || ref_wh == true)
  {
    String fileName = do_phasemoon() + "_32.jpg";;
    drawJpeg(fileName, x + 6, y + 3);
    t_next_moon = 0;
    t_next_th = 0;
    if (S_t_moon == 0) ref_th = true;
    if (S_t_bme == 0 || (temp_kv == 200 && h_kv == 200)) ref_moon = true;
    show_moon = true;
    return;
  }

  //--------------------------------


  if (millis() > t_next_th && (temp_kv != 200 || h_kv != 200) && S_t_bme > 0 && ref_th == true)
  {
    if (S_t_moon > 0 && show_moon == true)
    {
      S1_clear_moon(x, y);
      S_show_th_room(x, y);
    }
    t_next_moon = millis() + S_t_bme * 1000;
    ref_th = false;
    ref_moon = true;
    show_moon = false;
    //Serial.print(F( "t_next_moon=="));  Serial.println(t_next_moon);

    return;
  }

  //MOON mmmmmmmmmmmmmmmm
  if (millis() > t_next_moon && S_t_moon > 0 && ref_moon == true)
  {
    if (S_t_bme > 0 && (show_moon == false))
    {
      S1_clear_moon(x, y);

      String fileName = do_phasemoon() + "_32.jpg";;
      drawJpeg(fileName, x + 6, y + 3);
    }
    t_next_th = millis() + S_t_moon * 1000;
    ref_th = true;
    ref_moon = false;
    show_moon = true;
    //    Serial.print(F( "t_next_th=="));
    //   Serial.println(t_next_th);

  }




}



//-------------------------------------------------------------
//                  S1_clear_moon
///-------------------------------------------------------------

void S1_clear_moon(int x, int y)
{


  for (byte i = 0; i < 44; i++) {
    tft.drawFastVLine(x + i, y, 38, TFT_CYAN) ;
    if (i > 0)
      tft.drawFastVLine(x + i - 1, y, 38, TFT_BLACK) ;
    delay(15);
  }
  tft.fillRect(x, y, 44, 38, TFT_BLACK);
}


//===============================================================
//                          S1_alert
//===============================================================
void S1_alert(bool sh)
{

  if (sh == true)
  {

    //alert  ttttttttttttttttttttttttttttttttttttttttttt
    if (temperatura_ch[0] != 200)
    {

      if (temperatura_ch[0] >= t_max || temperatura_ch[1] >= t_max || temp_ch_n >= t_max) //t alert
        tft.fillRect(18, 117, 50, 3, TFT_YELLOW);

      if (temperatura_ch[0] <= t_min || temperatura_ch[1] <= t_min || temperatura_ch[2] <= t_min || temp_ch_n <= t_min) //t alert
        tft.fillRect(18, 117, 50, 3, TFT_CYAN);


    }

    //vvvvvvvvvvvvvvvvvvvvvvvvvvv
    if (v_ch[0] != 200)
    {
      if (v_ch[0] >= v_max || v_ch[1] >= v_max) //1,2 период по 3ч
        tft.fillRect(151, 139, 10, 3, TFT_CYAN);

    }

    //ppppppppppppppppppppppppppppppppppp
    int pp;
    if (pr_kv != 200) pp = pr_kv;
    else
      pp = davlenie0;

    if (pp != 200)
    {
      if (pp >= p_max || pp <= p_min)
        tft.fillRect(111, 157, 7, 5, TFT_GREEN);
    }

    //////uv
    if (uv >= uv_max)
      S_show_uv( false, 0, 115);


    //rainrrrrrrrrrrrrrrrrrrrrrrrrr
    if (rain_ch[0] >= rain_m * 2)
      do_draw_rain(74, 123, rain_ch[0], TFT_CYAN);

  }
  else //------------------------------------------------------
  {

    //////uv///////////////
    if (uv >= uv_max)
      S_show_uv( true, 0, 115);
    //ppppppppppppppppppppppppppppppppp

    tft.fillRect(111, 157, 7, 5, TFT_BLACK);

    //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

    tft.fillRect(151, 139, 10, 3, TFT_BLACK);


    //tttttttttttt
    tft.fillRect(18, 117, 50, 3, TFT_BLACK);


    //rainrrrrrrrrrrrrrrrrrrrrrrrrr
    if (rain_ch[0] >= rain_m * 2)
      do_draw_rain(74, 123, rain_ch[0], TFT_BLACK);

  }


}



#endif //openw
