#if defined (tft_400_300) && defined (S1epd)
//void     S1_epd()

//---------------------------------------------------------------------
//                            S1_epd
//---------------------------------------------------------------------
void  S1_epd()
{

  //  if (!sync_t) return;

  if (ref_wh == true || refresh_all == true)
  {
    Serial.println("S1_epd");
    int x, y;
    tft.fillScreen(TFT_WHITE);//GxEPD_LIGHTGREY TFT_WHITE
    //tft.clearScreen(TFT_WHITE);
    //   tft.setRotation(0);
    tft.setTextColor(TFT_BLACK);
    tft.setFullWindow();
    S1g_show();
    //dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd
    if (sync_t)  S_show_date();
    tft.drawFastVLine(125, 0, 80, TFT_BLACK) ;
    tft.drawFastVLine(124, 0, 80, TFT_BLACK) ;
    tft.display(false); // full update

  }


  //setPartialWindow

  if (ref_wh == true || ref_temp_u == true || ref_wh_now == true || refresh_all == true)
  {
    Serial.println("S1_show_now");
    Serial.println("S_show_th_room");
    S_show_now();
    S_show_th_room(346, 10);
  }
  else if (ref_kv_th == true)
  {
    Serial.println("S_show_th_room");
    S_show_th_room(346, 10);
  }

}


#endif// S1

#if defined (tft_400_300) && (defined (S1epd)||defined (S2epd))
//===================================================================================================
///                S_show_date
//===================================================================================================
void S_show_date()
{
  String str = "";
  if (day(t) < 10)
    str = "0" + String(day(t));
  else
    str = String(day(t));
  tft.setFreeFont(v24b);
  tft.setCursor(2, 40);


  tft.print(str);
  tft.setFreeFont(v9);
  tft.print(".");
  if (month(t) < 10)
    str = "0" + String(month(t));
  else
    str = String(month(t));
  tft.setFreeFont(v18b);
  tft.print(str);
  tft.setCursor(2, 75);
  byte wd = weekday(t);
  tft.print(dayNames[wd - 1]);

  //moon mmmmmmmmmmmmmmmm
  tft.setFreeFont(m18);
  tft.setCursor(52, 75);
  tft.print(moon_char());
  //vvvvvvvvvvvvvvvvvvvvv
  Serial.println("v7b");
  tft.setFreeFont(v7b);
  tft.setCursor(82, 58);
  tft.print(t_sun_v);
  //zahod
  tft.setCursor(82, 75);
  tft.print(t_sun_z);



}


//===================================================================================================
///                S_show_now
//===================================================================================================
void S_show_now()
{
  static String ref_tm_now = "";

  int x = 128;
  byte y = 0;
  int temp;
  temp = temperatura_ch[0];
  if (temp_u != 200)
  {
    temp = temp_u;
  }
  //rain snowrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
  // tft.setPartialWindow(x, y, 272, 80);
  tft.setPartialWindow(x, y, 272, 80);
  tft.firstPage();
  do
  {
    tft.fillScreen(GxEPD_WHITE);
    S_show_uv_epd(x, y);
    do_draw_rain(x + 5, y + 30, rain_ch[0], TFT_BLACK);
    //-------------iiiiicon
    tft.setFreeFont(m26);
    tft.setCursor(x + 10, y + 50);
    tft.print(wh_char(0, 'c'));
    tft.setFreeFont(v7b);
    tft.setCursor(x , y + 77);
    if (ref_wh_now) ref_tm_now = now_time;
    tft.print("rf-" + ref_tm_now);

    tft.setFreeFont(v18b);
    tft.setCursor(x + 70, y + 40);
    if (temp >= 0)
    {
      tft.print(" ");
      tft.print(temp);
    }
    else
      tft.print(temp);

    //nnnnnnnnnnnnnnnnnnnnnnnn
    if (temp_ch_n != 200)
    {

      //     tft.setFreeFont(v12b);
      tft.setCursor(x + 70, y + 74);
      if (temp_ch_n >= 0)
      {
        tft.print(" ");
        tft.print(temp_ch_n);
      }
      else
        tft.print(temp_ch_n);
    }

    //------------------------
    x = x + 135;
    tft.drawFastVLine(x, 0, 79, TFT_BLACK);
    tft.drawFastVLine(x + 2, 0, 79, TFT_BLACK);

    //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    tft.setFreeFont(m18);
    tft.setCursor(x + 3 , 32);
    tft.print(v_char(0, 'c'));

    if (v_ch[0] > 9)
      tft.setFreeFont(v12b);
    tft.print(v_ch[0]);

    //ppppppppppppppppppppppppppppppppppppppppppppppppppppppp

    if (davlenie0 != 200)
    {
      tft.setCursor(x + 3, 72);
      tft.setFreeFont(m12);
      if (davlenie0 < davlenie7[1]) tft.print(char(0xB1));
      if (davlenie0 > davlenie7[1]) tft.print(char(0xB2));

#ifndef hPa
      tft.setFreeFont(v18b);
#else
      tft.setFreeFont(v12b);
#endif
      if (pr_kv != 200)
      {
        tft.print(pr_kv);
      }
      else
        tft.print(davlenie0);
    }

    //   S1_graf_davlenie();

  }
  while (tft.nextPage());

  //  drawBmp("/48.bmp", x + 12, y, false, true, true);
}









#endif
