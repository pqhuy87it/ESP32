#if defined (S5) && defined(tft_320_240)&& defined (openw)
//==============================================================================================
//                      S5_time
//==============================================================================================
void S5_time()
{


  static uint32_t t_next_ch = 0;
  static uint32_t t_next_day = 0;
  static uint32_t t_next = 0;
  if (refresh_all)
  {
    t_next_ch = 0;
    t_next_day = 0;
    t_next = 0;
  }






  tft.setFreeFont(v6);

  if (t % 2 == 0)
  {

    S5_alert(false);
    //ntp
    if (err_ntp == 0)
      tft.fillCircle(71, 22, 2, TFT_GREENYELLOW);
    else
      tft.fillCircle(71, 22, 2, TFT_RED);
  } //sec black------------------------------------
  else
  {
    S5_alert(true);
    //sec
    tft.fillCircle(71, 22, 2, TFT_BLACK);
  }







  if (ref_temp_u == true || ref_wh_now == true || refresh_all == true) //есть данные с улицы
  {
    S5_show_now();
    S_show_th_room(272, 36);
  }
  else if (ref_kv_th == true) S_show_th_room(272, 36);


  if (sync_t)
    S5_ref_clock();


  if (day_ch[0] == 99 ) return; //нет данных о погоде
  if (millis() < t_next) return;
  if (scr_timer) return;




  //-----------------screen 3hour-----------------
  if (S5_t_ch > 0 && millis() > t_next_ch)
  {
    t_next_ch = millis() + S5_t_ch * 1000 + S5_t_day * 1000;
    S5_show_ch();
    t_next = millis() + S5_t_ch * 1000;
    return;
  }



  //-----------------screen 4day-----------------
  if (S5_t_day > 0 && millis() > t_next_day)
  {
    t_next_day = millis() + S5_t_day * 1000 + S5_t_ch * 1000;
    S5_show_day();
    t_next = millis() + S5_t_day * 1000;
    return;
  }


}

//===============================================================================
//                    S5_ref_clock
//===============================================================================
void S5_ref_clock()
{
  static byte last_min = 99; //чтобы не перирис минуты

  //time_t t =now();



  if (last_min == now_min && refresh_all == false) return ;


  if (now_min == 0)
    tft.fillRect(0, 0, 148, 42, TFT_BLACK);
  else if (now_min % 10 == 0)
    tft.fillRect(80, 0, 68, 42, TFT_BLACK);
  else
    tft.fillRect(112, 0, 36, 42, TFT_BLACK);



  //////////////hour///hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh
  String st;
  tft.setTextColor(S5_color_clock);
  tft.setFreeFont(v24b);

  if (refresh_all || now_min == 0)
  {


    if (now_hour < 10)
      st = "0" + String(now_hour);
    else
      st = String(now_hour);

    tft.setCursor(0, 40);
    tft.print(st);
  }


  ///min mmmmmmmmmmmmmmmmmmmmmmmmmmm
  if (refresh_all || now_min == 0 || now_min % 10 == 0)
  {
    if (now_min < 10)
      st = "0" + String(now_min);
    else
      st = String(now_min);
    tft.setCursor(80, 40);
  }
  else
  {
    if (now_min < 10)
      st = String(now_min);
    else
      st = String(now_min % 10);

    if (!scr_timer)
    {

      for (int i = 1; i <= 3; i++)
      {
        tft.fillRect(112, 0, 36, 42, TFT_BLACK);
        if (i == 1) tft.setFreeFont(v18);
        if (i == 2) tft.setFreeFont(v12);
        if (i == 3) tft.setFreeFont(v9);
        tft.setCursor(112, 40);
        tft.print(last_min % 10);
        delay(200);
      }

      for (int i = 1; i <= 3; i++)
      {
        tft.fillRect(112, 0, 36, 42, TFT_BLACK);
        if (i == 1) tft.setFreeFont(v9);
        if (i == 2) tft.setFreeFont(v12);
        if (i == 3) tft.setFreeFont(v18);
        tft.setCursor(112, 40);
        tft.print(st);
        delay(200);
      }

      delay(50);

    } //timer
    tft.fillRect(112, 0, 36, 42, TFT_BLACK);
    tft.setFreeFont(v24b);
    tft.setCursor(112, 40);

  }


  tft.print(st);


  last_min = now_min;


  //---------------------------------------------------------

  //дата--------data             dddddddddddddddddddddddd
  if (refresh_all == false && year(t) > 2015)
    return;

  tft.drawFastHLine(0, 44, 149, TFT_LIGHTGREY) ;
  tft.drawFastHLine(0, 80, 320, TFT_GREENYELLOW) ;
  tft.drawFastHLine(0, 81, 320, TFT_GREENYELLOW) ;



  //dddddddddddddddddddddddddddddddddddddddddddddddddd
  tft.setFreeFont(v18b);
  String str = "";
  if (day(t) < 10)
    str = "0" + String(day(t));
  else
    str = String(day(t));

  tft.setTextColor(c_date);
  //tft.setCursor(5,70);
  tft.setCursor(0, 75);
  tft.print(str);


  //tft.setFreeFont(v9b);
  tft.setFreeFont(v6);
  if (month(t) < 10)
    str = "0" + String(month(t));
  else
    str = String(month(t));
  tft.setTextColor(c_month);
  tft.print(str);


  //moon
  String fileName = do_phasemoon() + "_32.jpg";
  drawJpeg(fileName, 117, 47);


  //день недели------wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww
  draw_week_day(70, 47);

}








//===================================================================================================
///                S5_show_now
//===================================================================================================
void S5_show_now()
{


  int x = 150;
  byte y = 0;
  tft.fillRect(x + 1, y, 127, 79, TFT_BLACK);

  drawJpeg(icon_ch[0] + "48.jpg", x + 12, y + 10);
  tft.drawFastVLine(x + 60, 0, 79, TFT_LIGHTGREY);


  //rain snowrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr

  do_draw_rain(x + 5, y + 33, rain_ch[0], TFT_CYAN);

  if (rain_ch[0] >= rain_l)
    do_draw_rain_mm(x + 20, y + 10, rain_ch[0], TFT_CYAN);



  //tttttttttttttttttttttttttttttttttttt


  int temp;
  temp = temperatura_ch[0];

  if (temp_u != 200)
  {
    temp = temp_u;
    // ref_temp_u = false;

  }


  tft.drawFastVLine(150, 0, 79, c_tu_frame) ;
  tft.drawFastVLine(149, 0, 79, c_tu_frame) ;

  tft.setTextColor(color_t(temp));

  if (temp > -10)
    tft.setFreeFont(v18b);
  else
    tft.setFreeFont(v12);

  if (temp >= 0 && temp < 10)
    tft.setCursor(x + 80, y + 25);
  else
    tft.setCursor(x + 65, y + 25);

  tft.print(temp);
  //nnnnnnnnnnnnnnnnnnnnnnnn
  if (temp_ch_n != 200)
  {

    tft.setTextColor(color_t(temp_ch_n));
    tft.setFreeFont(v12);
    if (temp >= 0 && temp < 10)
      tft.setCursor(x + 80, y + 46);
    else
      tft.setCursor(x + 66, y + 46);
    tft.print(temp_ch_n);
  }


  //ppppppppppppppppppppppppppppppppppppppppppppppppppppppp
  tft.drawFastVLine(x + 123, 0, 36, TFT_LIGHTGREY);
  if (davlenie0 != 200)
  {
    tft.fillRect(275, 0, 44, 28, TFT_BLACK);

    if (davlenie0 < davlenie7[1])
      tft.drawTriangle(283, 25, 280, 15, 277, 25, TFT_YELLOW);

    if (davlenie0 > davlenie7[1])
      tft.drawTriangle(283, 15, 277, 15, 280, 25, TFT_CYAN);


#ifndef hPa
    tft.setFreeFont(v9);
#else
    tft.setFreeFont(v6);
#endif

    tft.setTextColor(c_pr_ow);
    tft.setCursor(285, 25);


    if (pr_kv != 200)
    {
      tft.setTextColor(c_pr_bme);
      tft.print(pr_kv);
    }
    else
      tft.print(davlenie0);


    tft.setFreeFont(v6);
    tft.setTextColor(TFT_YELLOW);
    tft.setCursor(x + 10, y + 65);
    tft.print(t_sun_v);
    //zahod
    tft.setTextColor(TFT_ORANGE);
    tft.setCursor(x + 10, y + 78);
    tft.print(t_sun_z);





  }




  //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

  float gr = int(v_gr_ch[0]);
  do_deg(gr);

  drawJpeg(wind_ico, x + 62, y + 47);

  tft.setFreeFont(v9);
  tft.setTextColor(TFT_CYAN);
  tft.setCursor(x + 94, y + 72);
  tft.print(v_ch[0]);


  ////UV UUUUUUUUUUUUUUUUUUUUUUUUU
  S_show_uv(true, x, y);


}



//===================================================================================================
///                S5_show_day()
//===================================================================================================

void S5_show_day()
{

  int y = 85;
  int x = 0;
  byte b;


  //--------------------------------------------------------------------------
  for (byte i = 1; i <= 4; i++) {


    tft.fillRect(x, 82, 80, 159, TFT_BLACK);
    if (temperatura7[i] == 200)
    {
      x = x + 80;
      continue;
    }

    if (i > 1)
      tft.drawFastVLine(x, 81, 110, TFT_WHITE) ;


    //day//dddddddddddddddddddddddddddddddddddd
    if (day7[i] < 10)
      b = 28;
    else
      b = 22;
    //b=26;
    //выходные выделяю................
    tft.setFreeFont(v9);
    tft.setTextColor(color_day(holiday7[i]));

    tft.setCursor(x + b, y + 11);
    tft.print(day7[i]);

    //--------------------------------

    drawJpeg(icon7[i] + "48.jpg", x + 1, y + 17);



    //rain snowrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr

    do_draw_rain_mm(x + 13, y + 80, rain7[i], TFT_CYAN);

    ///ttttttttttttttttttttttttttttttttttttttt
    unsigned int col;
    byte xx;
    int tt;

    tt = temperatura7[i];

    col = color_t(tt);
    tft.setTextColor(col);
    tft.setFreeFont(v9);

    if (tt < -9)
      xx = 45;
    else
      xx = 50;

    tft.setCursor(x + xx, y + 40);
    tft.print(tt);



    if (temperatura7[i] >= t_max || temperatura7[i] <= t_min || temperatura7n[i] >= t_max || temperatura7n[i] <= t_min )
      tft.fillRect(x + xx, y + 42, 25, 3, TFT_YELLOW);
    else
      tft.drawFastHLine(x + xx, y + 45, 25, TFT_CYAN) ;

    //nnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn
    if (temperatura7n[i] != 200)
    {
      byte xx = 0;
      if (temperatura7n[i] < -9) //3
        xx = 45;
      else
        xx = 50;

      col = color_t(temperatura7n[i]);
      tft.setTextColor(col);
      tft.setCursor(x + xx, y + 61);
      tft.print(temperatura7n[i]);

    }




    //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    tft.setTextColor(TFT_CYAN);
    tft.setFreeFont(v9);
    tft.setCursor(x + 45, y + 105);

    float gr = int(v_gr_7[i]);
    tft.print(veter7[i]);

    do_deg(gr);
    drawJpeg(wind_ico, x + 10, y + 85);







    x = x + 80;

  }

  tft.drawFastHLine(0, y + 15, 320, TFT_CYAN) ;
  //pppppppppppppppppppppp
  tft.setTextColor(0x07E0);
  tft.setFreeFont(v6);
  tft.setCursor(10, 215);
  tft.print(p7_max);
  tft.setCursor(10, 230);
  tft.print(p7_min);


  S5_graf_davlenie();



}




//===================================================================================================
///                S5_show_ch()
//===================================================================================================

void S5_show_ch()
{

  int y = 85;
  int x = 0;
  byte b;



  //----------------------------------------------
  for (byte i = 1; i <= 4; i++) {

    if (temperatura_ch[i] == 200) continue;

    for (byte xx = 0; xx < 80; xx++)
    {

      tft.drawFastVLine(x + xx, 81, 159, TFT_WHITE) ;

      tft.drawFastVLine(x + xx, 81, 159, TFT_BLACK) ;
    }

    if (i > 1)
      tft.drawFastVLine(x, 81, 130, TFT_WHITE) ;







    //day//dddddddddddddddddddddddddddddddddddd
    String s = "";
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
    tft.setCursor(x + 20, y + 11);
    tft.print(s);
    tft.setFreeFont(v6);
    tft.print(":00");


    drawJpeg(icon_ch[i] + "48.jpg", x + 16, y + 17);



    //rain snowrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
    do_draw_rain(x + 10, y + 30, rain_ch[i], TFT_CYAN);
    if (rain_ch[i] >= rain_m)
      do_draw_rain_mm(x + 7, y + 70, rain_ch[i], TFT_CYAN);






    ///ttttttttttttttttttttttttttttttttttttttt
    unsigned int col;
    byte xx;
    int tt;
    tt = temperatura_ch[i];
    col = color_t(tt);
    tft.setTextColor(col);
    tft.setFreeFont(v12);

    tft.setCursor(x + 20, y + 93);
    tft.print(tt);



    //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

    tft.setTextColor(TFT_CYAN);
    tft.setFreeFont(v9);
    tft.setCursor(x + 45, y + 126);


    float gr = int(v_gr_ch[i]);
    tft.print(v_ch[i]);


    do_deg(gr);
    drawJpeg(wind_ico, x + 10, y + 101);

    x = x + 80;

  }

  tft.drawFastHLine(0, y + 15, 320, TFT_CYAN) ;


}



//===================================================================================================
//                    S5_graf_davlenie
//===================================================================================================
////pppppppppppppppppppppppppppppppppppppppppppppppppppp
void S5_graf_davlenie()
{


  if (davlenie7[0] == 200 || davlenie0 == 200) return;

  int y = 237;
  int x = 40;

  for (byte i = 1; i <= 4; i++)
  {


    if (davlenie7[i] >= p_max || davlenie7[i] <= p_min)
      tft.fillCircle(x, y - ((davlenie7[i] - davlenie7[0]) * 2), 3, 0x07E0);

    if (davlenie7[i + 1] == 200)break;

    if (i < 4)
      tft.drawLine(x, y - ((davlenie7[i] - davlenie7[0]) * 2), x + 80, y - (davlenie7[i + 1] - davlenie7[0]) * 2, 0x07E0);

    x = x + 80;
  }

}



//==========================================================================================
//                          S5_alert
//===========================================================================================
void S5_alert(bool sh)
{
  int x = 150;
  byte y = 0;



  if (sh == true)
  {

    //alert  ttttttttttttttttttttttttttttttttttttttttttt
    if (temperatura_ch[0] != 200)
    {

      if (temperatura_ch[0] >= t_max || temperatura_ch[1] >= t_max || temp_ch_n >= t_max) //t alert
        tft.fillRect(x + 63, y + 26, 50, 2, TFT_YELLOW);


      if (temperatura_ch[0] <= t_min || temperatura_ch[1] <= t_min || temperatura_ch[2] <= t_min || temp_ch_n <= t_min) //t alert
        //tft.drawFastHLine(x+63,y+26,20,TFT_CYAN);
        tft.fillRect(x + 63, y + 26, 50, 2, TFT_CYAN);
    }

    //vvvvvvvvvvvvvvvvvvvvvvvvvvv
    if (v_ch[0] != 200)
    {
      if (v_ch[0] >= v_max || v_ch[1] >= v_max) //1,2 период по 3ч
        tft.fillRect(x + 94, y + 75, 15, 3, TFT_CYAN);

    }

    //ppppppppppppppppppppppppppppppppppp
    int pp;
    if (pr_kv != 200) pp = pr_kv;
    else
      pp = davlenie0;

    if (pp != 200)
    {
      if (pp >= p_max || pp <= p_min)
        tft.fillRect(285, 28, 30, 3, TFT_GREEN);

    }

    //////uv
    if (uv >= uv_max)
      //S1_show_uv(false); //не показ значка uv
      S_show_uv( false, x, y);


    //rainrrrrrrrrrrrrrrrrrrrrrrrrr
    if (rain_ch[0] >= rain_m * 2) //155 33
      do_draw_rain(x + 5, y + 33, rain_ch[0], TFT_CYAN);

  }
  else //------------------------------------------------------------------
  {

    //////uv///////////////
    //S1_show_uv(true); //показ значка uv
    S_show_uv( true, x, y);
    //ppppppppppppppppppppppppppppppppp
    tft.fillRect(285, 28, 30, 3, TFT_BLACK);

    //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

    tft.fillRect(x + 94, y + 75, 15, 3, TFT_BLACK);

    //tttttttttttttttttttttttttttttttttttt
    tft.fillRect(x + 63, y + 26, 50, 2, TFT_BLACK);

    //rrrrrrrrrrrrrrrrrrrrrrrrrrrrain
    //rainrrrrrrrrrrrrrrrrrrrrrrrrr
    if (rain_ch[0] >= rain_m * 2)
      do_draw_rain(x + 5, y + 33, rain_ch[0], TFT_BLACK);

  }


}




#endif //openw
