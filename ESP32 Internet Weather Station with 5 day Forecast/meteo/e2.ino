#if defined (tft_400_300) && defined (S2epd)
//void     S1_epd()

//---------------------------------------------------------------------
//                            S2_epd
//---------------------------------------------------------------------
void  S2_epd()
{

  //  if (!sync_t) return;

  if (refresh_all == true)
  {
    Serial.println("S2_epd");
    int x, y;
    tft.fillScreen(TFT_WHITE);
    //tft.clearScreen(TFT_WHITE);
    //   tft.setRotation(0);
    tft.setTextColor(TFT_BLACK);
    tft.setFullWindow();
    //  Serial.println("draw_Calendar");

    draw_Calendar(0, 135, 200, 0);
    draw_Calendar(200, 135, 200, 1);

    //dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd
    if (sync_t)  S_show_date();
    tft.drawFastVLine(125, 0, 80, TFT_BLACK) ;
    tft.drawFastVLine(124, 0, 80, TFT_BLACK) ;
    tft.drawFastHLine(0, 80, 399, TFT_BLACK) ;
    //3day
    draw_3d();
    tft.display(false); // full update

  }


  //setPartialWindow
  if (ref_temp_u == true || ref_wh_now == true || refresh_all == true)
  {
    Serial.println("S2_show_now");
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



//-------------------------------------------------------------------------------
//                  draw_3d
//-------------------------------------------------------------------------------

void   draw_3d()
{
  byte step_x = 133;
  int x = 0;
  byte y = 82;

  for (byte i = 1; i < 4; i++)
  {
    if (temperatura7[i] == 200) continue;
    //jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj
    tft.setFreeFont(m18);
    tft.setCursor(x + 8, y + 38);
    tft.print(wh_char(i, 'd'));

    do_draw_rain(x + 6, y + 8,  rain7[i], TFT_BLACK);


    //ttttttttttttttttttttttttttt
    tft.setCursor(x + 52, y + 25);
    //  tft.setFreeFont(v18b);
    tft.setFreeFont(v12b);
    if ( temperatura7[i] > -10)
    {
      tft.print(" ");
      tft.print(temperatura7[i]);
    }
    else
      tft.print(temperatura7[i]);

    //nnnnnnnnnnnnnnnnnnnnnnnn
    if (temperatura7n[i] != 200)
    {
      //   tft.setFreeFont(v12b);
      tft.setCursor(x + 52, y + 45);
      if (temperatura7n[i] >= 0)
      {
        tft.print(" ");
        tft.print(temperatura7n[i] );
      }
      else
        tft.print(temperatura7n[i] );
    }

    //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    tft.setFreeFont(m12);
    tft.setCursor(x + 100 ,  y + 34);
    tft.print(v_char(i, 'd'));
    tft.setFreeFont(v7b);
    tft.setCursor(x + 105 ,  y + 10);
    tft.print(veter7[i]);
    Serial.print("veter7 -"); Serial.println(veter7[i]);
    //pppppppppppppppppppppppppppppppppppppp
    tft.setFreeFont(v7b);
    tft.setCursor(x + 98 , y + 49);
    tft.print(davlenie7[i]);
    Serial.print("davlenie7 -"); Serial.println(davlenie7[i]);
    x = x + step_x;
    if (x > 0 && x < 399) tft.drawFastVLine(x, y, 50, TFT_BLACK) ; //верт разд линия
  }

}

#endif //S2
