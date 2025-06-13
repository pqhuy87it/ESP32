



  #if defined (Scr_user1)
/*
  //====================================================
  //                S2_draw_clock
  //====================================================

  void S2_draw_clock(int x, int y)
  {
  tft.fillRect(x, y, 108, 33, TFT_BLACK);
  //////////////hour///hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh
  String st;
  tft.setTextColor(0xFFF1);
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


  //====================================================
  //                S2_show_moon
  //====================================================

  void S2_show_moon(int x, int y) //66x110
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


  //-----------------------------------------------------------------------
  //                        S2_time
  //-----------------------------------------------------------------------
  void do_Time_480_194(int x, int y, int color)
  {

  static byte last_min = 99; //чтобы не перирис минуты


  //time_t t =now();
  //время



  //tft.setFreeFont(v6);

  /*
    if (t % 2 == 0)
    {

      S2_alert(false);
      //ntp
      if (err_ntp == 0)
         tft.fillRoundRect(232,170,10,20,1,TFT_GREENYELLOW);
        //tft.fillCircle(232, 170, 4, TFT_GREENYELLOW);
      else
       tft.fillRoundRect(232,170,10,20,1,TFT_RED);
      //  tft.fillCircle(232, 170, 4,TFT_RED);
    } //sec black------------------------------------
    else
    {
      S2_alert(true);
      //sec
       tft.fillRoundRect(232,170,10,20,1,TFT_BLACK);
    //  tft.fillCircle(232, 170, 4, TFT_BLACK);
    }




    if (ref_temp_u == true || ref_wh_now == true || refresh_all == true) //есть данные now, Th
      S2_show_now();

    if (ref_wh == true || refresh_all == true) //обновилась погода
      S2_show_wh();


    if (ref_kv_th == true || refresh_all == true)
      S2_show_th();

*/


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
    fileName = "/n0_192.jpg";
    drawJpeg(fileName, x, y); //220
    fileName = "/n" + String(now_hour) + "_192.jpg";
    drawJpeg(fileName, x + 115, y); //220
  }
  else
  {
    st = String(now_hour);
    fileName = "/n" + st.substring(0, 1) + "_192.jpg";
    drawJpeg(fileName, x, y); //220
    fileName = "/n" + st.substring(1) + "_192.jpg";
    drawJpeg(fileName, x + 115, y); //220

  }


}



//////////////////min

if (now_min < 10)
{
  fileName = "/n0_192.jpg";
  drawJpeg(fileName, x + 250, y); //220
  fileName = "/n" + String(now_min) + "_192.jpg";
  drawJpeg(fileName, x + 365, y); //220
}
else
{
  st = String(now_min);
  fileName = "/n" + st.substring(0, 1) + "_192.jpg";
  drawJpeg(fileName, x + 250, y); //220
  fileName = "/n" + st.substring(1) + "_192.jpg";
  drawJpeg(fileName, x + 365, y); //220

}


/*
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

  byte b;
  if (temp_kv == 200 && h_kv == 200)
    b = 20;
  else
    b = 0;


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
*/

}

/*

  //====================================================
  //                S2_alert
  //====================================================

  void S2_alert(bool sh)
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




*/

#endif
