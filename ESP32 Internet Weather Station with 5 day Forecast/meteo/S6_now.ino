#if defined (S6) && defined(tft_320_240)&& defined (openw)



#if defined (two_tft) && defined (tft2_S6)
#define tft tft2
#endif

//------------------------------------------------------------
//                      S6_show_now()
//------------------------------------------------------------
void S6_show_now()
{

  int temp = 200;
  tft.fillRect(106, 179, 214, 61, TFT_BLACK);

  ///////////icon-WW--------------------------------------------
#ifdef debug
  Serial.print(F("icon today--->"));
  Serial.println(icon_ch[0] + "48.jpg");
#endif
  drawJpeg(icon_ch[0] + "48.jpg", 115, 180); //220
  //////////icon  vvvvvv----------------------------------------
  float gr = int(v_gr_ch[0]);
  do_deg(gr);
  if (wind_ico != "")
  {
    drawJpeg(wind_ico, 240, 180); //220
  }


  //ttttttttttttttttttttttttttttttttttttttttt
  if (temperatura_ch[0] == 200 && temp_u == 200) return; //!=данные мз инета

  temp = temperatura_ch[0];

  if (temp_u != 200)
  {
    temp = temp_u;
    //   ref_temp_u = false;

  }

  tft.drawFastVLine(104, 180, 60, c_tu_frame) ;
  tft.drawFastVLine(105, 180, 60, c_tu_frame) ;



  if (temp != 200)
  {
    tft.setFreeFont(v24b);
    uint32_t color_temp = color_t(temp);
    tft.setTextColor(color_temp);
    //tft.setCursor(163,225);
    if (temp < -9 || temp > 9)
    {
      tft.setCursor(163, 225);
      if (temp < -9)
      {
        tft.fillRect(150, 207, 13, 7, color_temp);
        tft.drawRect(150, 207, 13, 7, TFT_BLUE);
      }
    }
    else
    {
      tft.setCursor(173, 225);
      if (temp < 0)
      {
        tft.fillRect(160, 207, 13, 7, color_temp);
        tft.drawRect(160, 207, 13, 7, TFT_BLUE);
      }
    }


    //  tft.print((int)(temp));
    tft.print(abs(int(temp)));


    //nnnnnnnnnnnnnnnnnnnnnnnn
    if (temp_ch_n != 200)
    {
      color_temp = color_t(temp_ch_n);
      tft.setTextColor(color_temp);
      if ((int)(temp) > 9) tft.setFreeFont(v9);
      else tft.setFreeFont(v12);

      tft.print(temp_ch_n);


    }

    tft.setFreeFont(v6);
  }




  /////////////////////////////////////

  ////////ветер /////////////////////////////
  //тенденция ветра
  if (v_ch[0] != 200) //есть данные мз инета
  {

    if (v_ch[0] < v_ch[1])
      tft.drawTriangle(283, 200, 280, 190, 277, 200, TFT_YELLOW);
    if (v_ch[0] > v_ch[1])
      tft.drawTriangle(283, 190, 277, 190, 280, 200, TFT_CYAN);


    tft.setFreeFont(v12);
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.setCursor(287, 202);
    tft.print(v_ch[0]);
    tft.setFreeFont(v6);
    if (v_ch[0] < 10)
    {
      tft.setTextColor(TFT_WHITE);
      tft.print("m");
    }

  }


  //ppppppppppppppppppppppppppppppppppppppppppppppppppppppp
  if (davlenie0 != 200)
  {

    if (davlenie0 < davlenie7[1])
      tft.drawTriangle(283, 225, 280, 215, 277, 225, TFT_YELLOW);

    if (davlenie0 > davlenie7[1])
      tft.drawTriangle(283, 215, 277, 215, 280, 225, TFT_CYAN);



#ifndef hPa
    tft.setFreeFont(v9);
#else
    tft.setFreeFont(v6);
#endif

    tft.setTextColor(c_pr_ow);
    tft.setCursor(285, 225);

    if (pr_kv != 200)
    {
      tft.setTextColor(c_pr_bme);
      tft.print(pr_kv);
    }
    else
      tft.print(davlenie0);
    tft.setFreeFont(v6);


  }



  //prim///////////////////
#ifdef EN
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setCursor(125, 238);
  tft.setFreeFont(v6);
  tft.print(descript_now);
#endif



  //rrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
  do_draw_rain(110, 200, rain_ch[0], TFT_CYAN);

}




//=================================================================================================
//                  S6_show_th()
//=================================================================================================
void S6_show_th()
{
  if (temp_kv == 200 && h_kv == 200) return;


  tft.fillRect(58, 179, 43, 60, TFT_BLACK); //data
  tft.drawFastVLine(60, 179, 61, c_th_frame);

  if (temp_kv != 200) //bme
  {
    tft.setTextColor(TFT_YELLOW);
    tft.setFreeFont(v12);
    tft.setCursor(65, 210);
    tft.print(temp_kv);
    tft.setFreeFont(v6);
    tft.setTextColor(TFT_WHITE);
    tft.print("c");
  }

  //------------------hhhhhhhhhhhhhhhhhhhhhh

  if (h_kv == 200)return; //нет данных
  tft.setFreeFont(v9);
  tft.setTextColor(TFT_CYAN);
  tft.setCursor(65, 235);
  tft.print(h_kv);
  tft.setFreeFont(v6);
  tft.setTextColor(TFT_WHITE);
  tft.print("%");

}




#if defined (two_tft) && defined (tft2_S6)
#undef tft //tft2
#endif



#endif //openw
