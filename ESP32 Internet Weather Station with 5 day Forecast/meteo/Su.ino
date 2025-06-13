//void S2_show_th()


#ifndef epd
//--------------------------------------------------------------------
//        draw_week_day
//--------------------------------------------------------------------
void draw_week_day(int x, int y)
{


  if (!sync_t) return;


#ifdef EN
  drawJpeg("/w" + String(now_weekday) + "e.jpg", x, y); //220
#else
  drawJpeg("/w" + String(now_weekday) + ".jpg", x, y); //220
#endif
}
//==========================================================================================
//  tft_320_240          S_show_th_room
//==========================================================================================
#if defined(tft_320_240)
void S_show_th_room(int x, int y)
{

#ifdef two_tft
  if (tft2_act)
  {
    tft2.fillRect(x, y, 44, 38, TFT_BLACK);
    tft2.drawRoundRect(x, y, 44, 38, 3, c_th_frame);
    if (temp_kv == 200 && h_kv == 200) return;
    tft2.setTextColor(TFT_YELLOW);
    if (temp_kv != 200) //bme 270,73
    {
      tft2.setFreeFont(v9);
      tft2.setCursor(x + 4, y + 16);
      tft2.print(temp_kv);
      tft2.setFreeFont(v6);
      tft2.setTextColor(TFT_WHITE);
      tft2.print("c");
    }
    if (h_kv != 200) //bme
    {
      tft2.setFreeFont(v9);
      tft2.setTextColor(TFT_CYAN);
      tft2.setCursor(x + 4, y + 33);
      tft2.print(h_kv);
      tft2.setFreeFont(v6);
      tft2.setTextColor(TFT_WHITE);
      tft2.print("%");
    }
  }
  else
  {
    tft.fillRect(x, y, 44, 38, TFT_BLACK);
    tft.drawRoundRect(x, y, 44, 38, 3, c_th_frame);
    if (temp_kv == 200 && h_kv == 200) return;
    tft.setTextColor(TFT_YELLOW);
    if (temp_kv != 200) //bme 270,73
    {
      tft.setFreeFont(v9);
      tft.setCursor(x + 4, y + 16);
      tft.print(temp_kv);
      tft.setFreeFont(v6);
      tft.setTextColor(TFT_WHITE);
      tft.print("c");
    }
    if (h_kv != 200) //bme
    {
      tft.setFreeFont(v9);
      tft.setTextColor(TFT_CYAN);
      tft.setCursor(x + 4, y + 33);
      tft.print(h_kv);
      tft.setFreeFont(v6);
      tft.setTextColor(TFT_WHITE);
      tft.print("%");
    }
  }
#else
  tft.fillRect(x, y, 44, 38, TFT_BLACK);
  tft.drawRoundRect(x, y, 44, 38, 3, c_th_frame);
  if (temp_kv == 200 && h_kv == 200) return;
  tft.setTextColor(TFT_YELLOW);
  if (temp_kv != 200) //bme 270,73
  {
    tft.setFreeFont(v9);
    tft.setCursor(x + 4, y + 16);
    tft.print(temp_kv);
    tft.setFreeFont(v6);
    tft.setTextColor(TFT_WHITE);
    tft.print("c");
  }
  if (h_kv != 200) //bme
  {
    tft.setFreeFont(v9);
    tft.setTextColor(TFT_CYAN);
    tft.setCursor(x + 4, y + 33);
    tft.print(h_kv);
    tft.setFreeFont(v6);
    tft.setTextColor(TFT_WHITE);
    tft.print("%");
  }

#endif
}

#endif





//====================================================
// tft_480_320        S_show_th_room
//====================================================
#if defined(tft_480_320)
void S_show_th_room(int x, int y)
{

  // Serial.print ("tft2_act="); Serial.println (tft2_act);
#ifdef two_tft
  if (tft2_act)
  {
    tft2.fillRect(x, y, 52, 57, TFT_BLACK);
    tft2.drawRoundRect(x, y, 52, 57, 4, c_th_frame);
    if (temp_kv == 200 && h_kv == 200) return; //��� ������ bme
    tft2.setTextColor(TFT_YELLOW);
    tft2.setFreeFont(v12);

    if (temp_kv != 200) //bme
    {
      tft2.setCursor(x + 5, y + 23);
      tft2.print(temp_kv);
      tft2.setTextColor(TFT_WHITE);
      tft2.setFreeFont(v6);
      tft2.print("C");
    }
    if (h_kv != 200) //bme
    {
      tft2.setTextColor(TFT_CYAN);
      tft2.setFreeFont(v12);
      tft2.setCursor(x + 5, y + 52);
      tft2.print(h_kv);
      tft2.setTextColor(TFT_WHITE);
      tft2.setFreeFont(v6);
      tft2.print("%");
    }
  }
  else
  {
    tft.fillRect(x, y, 52, 57, TFT_BLACK);
    tft.drawRoundRect(x, y, 52, 57, 4, c_th_frame);
    if (temp_kv == 200 && h_kv == 200) return; //��� ������ bme
    tft.setTextColor(TFT_YELLOW);
    tft.setFreeFont(v12);

    if (temp_kv != 200) //bme
    {
      tft.setCursor(x + 5, y + 23);
      tft.print(temp_kv);
      tft.setTextColor(TFT_WHITE);
      tft.setFreeFont(v6);
      tft.print("C");
    }
    if (h_kv != 200) //bme
    {
      tft.setTextColor(TFT_CYAN);
      tft.setFreeFont(v12);
      tft.setCursor(x + 5, y + 52);
      tft.print(h_kv);
      tft.setTextColor(TFT_WHITE);
      tft.setFreeFont(v6);
      tft.print("%");
    }

  }

#else //e two_tft
  tft.fillRect(x, y, 52, 57, TFT_BLACK);
  tft.drawRoundRect(x, y, 52, 57, 4, c_th_frame);
  if (temp_kv == 200 && h_kv == 200) return; //��� ������ bme
  tft.setTextColor(TFT_YELLOW);
  if (temp_kv != 200) //bme
  {
    tft.setCursor(x + 5, y + 23);
    tft.setFreeFont(v12);
    tft.print(temp_kv);
    tft.setTextColor(TFT_WHITE);
    tft.setFreeFont(v6);
    tft.print("C");
  }
  if (h_kv != 200) //bme
  {
    tft.setTextColor(TFT_CYAN);
    tft.setFreeFont(v12);
    tft.setCursor(x + 5, y + 52);
    tft.print(h_kv);
    tft.setTextColor(TFT_WHITE);
    tft.setFreeFont(v6);
    tft.print("%");
  }
#endif
}
#endif //tft_480_320

#endif //#ifndef epd


//====================================================
//         epd         S_show_th_room
//====================================================
#if defined(epd)
void S_show_th_room(int x, int y)
{
  if (temp_kv == 200 && h_kv == 200) return; //��� ������ bme
  tft.setTextColor(TFT_BLACK);
  tft.setPartialWindow(x, y, 52, 57);
  tft.firstPage();
  do
  {
    tft.fillRect(x, y, 52, 57, TFT_WHITE);
    tft.drawRoundRect(x, y, 52, 57, 4, TFT_BLACK);
    tft.drawRoundRect(x + 1, y + 1, 52 - 2, 57 - 2, 4, TFT_BLACK);
    if (temp_kv != 200) //bme
    {
      tft.setCursor(x + 5, y + 23);
      tft.setFreeFont(v12b);
      tft.print(temp_kv);
      tft.setFreeFont(v9);
      tft.print("C");
    }
    if (h_kv != 200) //bme
    {
      tft.setFreeFont(v12b);
      tft.setCursor(x + 5, y + 50);
      tft.print(h_kv);
      tft.setFreeFont(v9);
      tft.print("%");
    }
  }
  while (tft.nextPage());
  //  delay(500);



}
#endif //epd
