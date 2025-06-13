#if defined (tft_400_300) && defined (S3epd)
//void     S1_epd()

//---------------------------------------------------------------------
//                            S3_epd
//---------------------------------------------------------------------
void  S3_epd()
{

  //  if (!sync_t) return;

  if (ref_wh == true || refresh_all == true)
  {
    Serial.println("S3_epd");
    tft.fillScreen(TFT_WHITE);
    tft.setTextColor(TFT_BLACK);
    tft.setFullWindow();
    //dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd
    if (sync_t)  S_show_date();
    tft.drawFastVLine(125, 0, 80, TFT_BLACK) ;
    tft.drawFastVLine(124, 0, 80, TFT_BLACK) ;
    tft.drawFastHLine(0, 80, 399, TFT_BLACK) ;
    draw_3ch_epd();
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

//===================================================================================================
///                draw_3ch_epd
//===================================================================================================
void draw_3ch_epd()
{
  byte row = 1;
  int y = 82;
  int x = 0;
  int n;
  String s = "";
  byte step = 49;
  tft.drawFastHLine(0, y, 399, TFT_BLACK) ; //half
  tft.drawFastHLine(0, y + 15, 399, TFT_BLACK) ; //half
  for (byte i = 1; i < 19; i++)
  {
    if (temperatura_ch[i] == 200) continue;


    //ddddddddddddddddddddddddddddddddddddddddddd
    if (day_ch[i] < 10)
      s = (" 0");
    else
      s = (" ");
    s = s + String(day_ch[i]);

    tft.setFreeFont(v7b);
    tft.setCursor(x + 8, y + 10);
    tft.print(s);


    if ((day_ch[i] == 0 || day_ch[i] == 1 || day_ch[i] == 2) && x > 0)
    {
      tft.drawFastVLine(x, y, 113, TFT_BLACK) ; //верт разд линия
      tft.drawFastVLine(x - 1, y, 113, TFT_BLACK) ; //верт разд линия
    }
    else if (x > 0) tft.drawFastVLine(x, y, 65, TFT_BLACK) ; //верт разд линия


    //jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj
    tft.setFreeFont(m18);
    tft.setCursor(x + 4, y + 49);
    tft.print(wh_char(i, 'c'));

    //rain snowrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
    do_draw_rain(x + 4, y + 20, rain_ch[i], TFT_BLACK);
    //TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT
    unsigned int col;
    int tt = temperatura_ch[i];
    tft.setFreeFont(v9b);
    tft.setCursor(x + 10, y + 73);
    if (tt > -10) tft.print(" ");
    tft.print(tt);

    //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    tft.setFreeFont(m12);
    tft.setCursor(x + 1 , y + 98);
    tft.print(v_char(i, 'c'));
    tft.setFreeFont(v7b);
    tft.print(v_ch[i]);



    x = x + step;
    if (x + step > 399)
    {
      x = 0;
      y = 190;
      row++;
      if (row > 2) break;
      tft.drawFastHLine(0, y, 399, TFT_BLACK) ; //half
      tft.drawFastHLine(0, y + 15, 399, TFT_BLACK) ; //half
    }

  }





}

#endif //S3
