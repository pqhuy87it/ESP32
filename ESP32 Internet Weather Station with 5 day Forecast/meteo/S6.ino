#if defined (S6) && defined(tft_320_240)&& defined (openw)
//time///////////////////////////////////////tttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttt


#if defined (two_tft) && defined (tft2_S6)
#define tft tft2
#endif

void S6_time()
{

  static byte last_min = 99; //чтобы не перирис минуты

/*
#if defined (two_tft) && defined (tft2_S6)
  bool refresh_all = false;
  if (last_min == 99) //on start scr
  {
    last_min = 100;
    refresh_all = true;
  }
#endif
*/


  if (t % 2 == 0)
  {

    S6_alert(false);

  }
  else
  {
    S6_alert(true);
    //sec
  }






  if (ref_temp_u == true || ref_wh_now == true || refresh_all == true) //есть данные с улицы
    S6_show_now();


  if (ref_kv_th == true || refresh_all == true) S6_show_th();



  //если мин не изм  и не надо все обновить выходим
  //-----------------


#if (S6 == 1)
  {

    if (last_min == now_min && refresh_all == false)       return;
    last_min = now_min;
    S6_draw_clock(210, 20);
  }
#endif


  //--------------------------------------------------------------------------------

  //дата--------data/////////ddddddddddddddddddddddddddddddddddddddddddddddd
  if (refresh_all == false && now_year > 2015) //день недели может совпасть(01-01-), добавил год
    return;


#if (S6 == 0) //no clock
  draw_Calendar(10, 10, 200, 0);
  S6_show_moon(230, 40);

#elif (S6 == 1) //lit clock
  draw_Calendar(0, 10, 200, 0);
  S6_show_moon(230, 60);

#endif

  ////dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd

  tft.fillRect(0, 178, 103, 61, TFT_BLACK); //data
  tft.drawFastHLine(0, 176, 320, TFT_CYAN) ;
  tft.drawFastHLine(0, 178, 320, TFT_CYAN) ;


  String str = "";
  if (now_day < 10)
    str = "0" + String(now_day);
  else
    str = String(now_day);

  tft.setFreeFont(v18b);
  tft.setTextColor(c_date);
  tft.setCursor(5, 207);
  tft.print(str);

  //tttttttttthhhhhhhhhhhhhhhhhhhhh
  if (temp_kv == 200 && h_kv == 200)
  {
    tft.setFreeFont(v12);
    if (now_month < 10)
      str = ".0" + String(now_month);
    else
      str = "." + String(now_month);
    tft.setTextColor(c_month);
    tft.print(str);
  }
  else

    S6_show_th();


  //день недели-------------------------
  draw_week_day(10, 208);






}



//====================================================
//                S6_draw_clock
//====================================================
#if (S6 == 1)
void S6_draw_clock(int x, int y)
{
  tft.fillRect(x, y, 108, 33, TFT_BLACK);
  //////////////hour///hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh
  String st;
  tft.setTextColor(S6_color_clock);
  tft.setFreeFont(v18b);
  if (now_hour < 10)
    st = "0" + String(now_hour);
  else
    st = String(now_hour);
  tft.setCursor(x + 1, y + 29);
  tft.print(st);
  tft.setTextColor(TFT_CYAN);
  tft.setFreeFont(v9b);
  tft.print(":");
  // st = st + ":";
  tft.setTextColor(0xFFF1);
  tft.setFreeFont(v18b);

  ///min mmmmmmmmmmmmmmmmmmmmmmmmmmm
  if (now_min < 10)
    st =  "0" + String(now_min);
  else
    st =  String(now_min);

  tft.print(st);
  tft.drawRoundRect(x, y, 108, 33, 2, TFT_LIGHTGREY);
}
#endif

//====================================================
//                S6_show_moon
//====================================================

void S6_show_moon(int x, int y) //66x110
{
  tft.fillRect(x, y, 66, 110, TFT_BLACK);
  //z-v luna
  //----------------------------------------------
  ////////////////восход зах.///////////////////////////////////
  tft.setFreeFont(v9);
  //  tft.fillTriangle(0, 115, 10, 115, 5, 110, TFT_YELLOW);
  tft.fillTriangle(x + 2, y + 18, x + 10, y + 18, x + 6, y + 8, TFT_YELLOW);

  tft.setTextColor(TFT_YELLOW);
  //  tft.setCursor(10, 118);
  tft.setCursor(x + 11, y + 18);
  tft.print(t_sun_v);
  //zahod
  //  tft.fillTriangle(0, 135, 10, 135, 5, 140, TFT_RED);
  tft.fillTriangle(x + 2, y + 30, x + 10, y + 30, x + 6, y + 40, TFT_RED);
  tft.setTextColor(TFT_ORANGE);
  //tft.setCursor(10, 141);
  tft.setCursor(x + 11, y + 41);
  tft.print(t_sun_z);
  tft.setFreeFont(v6);

  //---------------moon
  String fileName;
  fileName = do_phasemoon() + "_64.jpg";

  //  tft.drawFastHLine(0, 157, 65, TFT_YELLOW);
  // drawJpeg(fileName, 0, 160);
  drawJpeg(fileName, x, y + 45);
  tft.drawRoundRect(x, y, 66, 110, 5, TFT_LIGHTGREY);




}


//====================================================
//                S6_alert
//====================================================

void S6_alert(bool sh)
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
    if (temperatura_ch[0] != 200 && temperatura_ch[1] != 200)
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



#if defined (two_tft) && defined (tft2_S6)
#undef tft //tft2
#endif





#endif
