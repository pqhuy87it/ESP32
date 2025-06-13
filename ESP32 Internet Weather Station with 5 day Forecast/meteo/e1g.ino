#if defined (tft_400_300) && defined (S1epd)

//--------------------------------------------------------
//                   S4g_show
//--------------------------------------------------------
void S1g_show()//top string clock
{

  const int h_scr = 299;
  const byte y_ch = 6;
  int  y_line = 80;
  int h = int((h_scr - y_ch - 4 - y_line) / 3); //-4 снизу
  const byte w = 10; //col 400/40=

  // tft.fillRect(0, y_line, 479, h_scr - y_line, TFT_WHITE);

  // Serial.println(F("S4_draw_3ch_num"));
  S4_draw_3ch_num(y_line, w, h_scr); //top string clock


  y_line = y_line + y_ch + h;
  tft.drawFastHLine(0, y_line, 399, TFT_BLACK) ;
  // Serial.println(F("S4_graf_t"));

  S4_graf_t(y_line, w, h); //Ttttttttttttttttttttttttt
  // S4_graf_h(y_line, w, int(h / 2)); //hhhhhhhhhhhhhhhhhh

  y_line = y_line + h;
  tft.drawFastHLine(0, y_line, 399, TFT_BLACK) ;
  //  Serial.println(F("S4_graf_p"));
  S4_graf_p(y_line, w, h); //Ppppppppppppppppppppppppp
  y_line = y_line + int(h / 2);
  tft.drawFastHLine(0, y_line, 399, TFT_BLACK) ;
  // Serial.println(F("S4_graf_v"));
  S4_graf_v(y_line, w, int(h / 2)); //vvvvvv
  y_line = 300;
  tft.drawFastHLine(0, y_line, 399, TFT_BLACK) ;
  // Serial.println(F("S4_graf_h"));
  S4_graf_h(y_line, w, int(h / 2)); //hhhhhhhhhhhhhhhhhh



}




//------------------------------------------------------------------------------------
//                      S4_graf_t
//------------------------------------------------------------------------------------

void S4_graf_t(int y, byte w, byte h) //Ttttttttttttttttttttttttt

{

  int x = 0;
  int t_min_l = 200;
  int t_max_l = -200;
  bool today = true;
  int t_min_d = 200;
  int t_max_d = -200;
  byte count_3ch = 0  ;
  byte k = 2;

  byte h_mid = int(h / 2);
  for (byte i = 1; i <= entries + 1; i++)
  {
    if (temperatura_ch[i] == 200) break;
    if (t_min_l > temperatura_ch[i]) t_min_l = temperatura_ch[i];
    if (t_max_l < temperatura_ch[i]) t_max_l = temperatura_ch[i];
  }

  byte val = 25;


  if (t_max_l - t_min_l > val) k = 1;

  int t_mid = int((t_max_l + t_min_l) / 2);

  for (byte i = 1; i <= entries + 1; i++)
  {

    //begin day
    if (day_ch[i] + delta_tz == 0 && x > 0 || i == entries + 1)
    {

      if (count_3ch > 3)
      {
        byte b = int(w * 5); //all
        if (i == entries + 1 || today) b = 35; //last


        // tft.setFreeFont(v7b);
        tft.setFreeFont(v9b);
        tft.setCursor(x - b, y - 5);
        tft.print(t_min_d);
        // tft.setFreeFont(v9b);
        tft.setCursor(x - b, y - h + 15);
        tft.print(t_max_d);

      }

      t_min_d = 200;
      t_max_d = -200;
      count_3ch = 0;
      today = false;
    }

    if (t_min_d > temperatura_ch[i]) t_min_d = temperatura_ch[i];
    if (t_max_d < temperatura_ch[i]) t_max_d = temperatura_ch[i];
    count_3ch++;


    if (i < entries + 1)
    {


      tft.drawLine(x, y - h_mid + (t_mid - temperatura_ch[i])*k, x + w, y - h_mid + (t_mid - temperatura_ch[i + 1])*k, TFT_BLACK);
      tft.drawLine(x, y - 1 - h_mid + (t_mid - temperatura_ch[i])*k, x + w, y - 1 - h_mid + (t_mid - temperatura_ch[i + 1])*k, TFT_BLACK);

      if (temperatura_ch[i] >= t_max || temperatura_ch[i] <= t_min) tft.fillCircle(x, y - h_mid + (t_mid - temperatura_ch[i])*k, 3, TFT_BLACK);

    }

    x = x + w;

  }



}


//===================================================================================
//                                  Sf_graf_p
//===================================================================================
void S4_graf_p(int y, byte w, byte h)
{


  int x = 0;
  int p_min_l = 20000;
  int p_max_l = 1;
  byte k = 2;

  int p_min_d = 20000;
  int p_max_d = -1;
  byte count_3ch = 0;
  bool today = true;

  byte h_mid = int(h / 2);
  for (byte i = 1; i <= entries + 1; i++)
  {
    if (p_ch[i] == 0) break;
    if (p_min_l > p_ch[i]) p_min_l = p_ch[i];
    if (p_max_l < p_ch[i]) p_max_l = p_ch[i];
  }


  byte val = 25;


  if (p_max_l - p_min_l > val) k = 1;

  int p_mid = int((p_max_l + p_min_l) / 2);

  for (byte i = 1; i <= entries + 1; i++)
  {

    //begin day
    if (day_ch[i] + delta_tz == 0 && x > 0 || i == entries + 1)
    {


      if (count_3ch > 3)
      {
        byte b = int(w * 5); //all
        if (i == entries + 1 || today) b = 35; //last



        tft.setFreeFont(v7b);
        tft.setCursor(x - b, y - 5);
        tft.print(p_min_d);


        tft.setCursor(x - b, y - h + 15);
        tft.print(p_max_d);


      }

      p_min_d = 20000;
      p_max_d = -1;
      count_3ch = 0;
      today = false;
    }


    if (p_min_d > p_ch[i]) p_min_d = p_ch[i];
    if (p_max_d < p_ch[i]) p_max_d = p_ch[i];
    count_3ch++;



    if (i < entries + 1)
    {

      tft.drawLine(x, y - h_mid + (p_mid - p_ch[i])*k, x + w, y - h_mid + (p_mid - p_ch[i + 1])*k, TFT_BLACK);
      tft.drawLine(x, y - 1 - h_mid + (p_mid - p_ch[i])*k, x + w, y - 1 - h_mid + (p_mid - p_ch[i + 1])*k, TFT_BLACK);

      if (p_ch[i] >= p_max || p_ch[i] <= p_min) tft.fillCircle(x, y - h_mid + (p_mid - p_ch[i])*k, 2, TFT_BLACK);




    }
    /*
      Serial.print("p_ch=");
      Serial.println(p_ch[i]);
      Serial.print("x=");
      Serial.println(x);
      Serial.println(i);
    */

    x = x + w;

  }
}



//===================================================================================
//                                 S4_graf_v
//===================================================================================
void S4_graf_v(int y, byte w, byte h)
{



  byte k = 2;

  byte v_min_d = 200;
  byte v_max_d = -0;
  byte count_3ch = 0;
  bool today = true;

  int x = 0;
  //byte v_min_l=200;
  byte v_max_l = 0;
  byte count = 0;


  for (byte i = 1; i <= entries + 1; i++)
  {
    //if (v_min_l>v_ch[i]) v_min_l=v_ch[i];
    if (v_max_l < v_ch[i]) v_max_l = v_ch[i];

    count++;
  }

  //int v_mid=int((v_max_l+v_min_l)/2);

  if (v_max_l * k > h) k--;
  //  Serial.print("k=");   Serial.println(k);

  for (byte i = 1; i <= entries + 1; i++)
  {



    //begin day
    if (day_ch[i] + delta_tz == 0 && x > 0 || i == entries + 1)
    {

      if (count_3ch > 3)
      {
        byte b = int(w * 5); //all
        if (i == entries + 1 || today) b = 35; //last

        tft.setFreeFont(v7b);
        tft.setCursor(x - b , y - h + 10);
        tft.print(v_min_d);
        tft.print("-");
        tft.print(v_max_d);
        tft.print("m");

      }

      v_min_d = 200;
      v_max_d = 0;
      count_3ch = 0;
      today = false;
    }

    if (v_min_d > v_ch[i]) v_min_d = v_ch[i];
    if (v_max_d < v_ch[i]) v_max_d = v_ch[i];
    count_3ch++;





    if (i < entries + 1)
    {
      float gr = int(v_gr_ch[i]);
      do_deg(gr);
      tft.drawLine(x, y - 1 - v_ch[i]*k, x + w, y - 1 - v_ch[i + 1]*k, TFT_BLACK);
      tft.drawLine(x, y - 2 - v_ch[i]*k, x + w, y - 2 - v_ch[i + 1]*k, TFT_BLACK);

      if (v_ch[i] >= v_max)
      {
        tft.drawCircle(x, y - 1 - v_ch[i]*k, 2, TFT_BLACK);
      }
    }

    x = x + w;

  }
}





//===================================================================================
//                                 S4_graf_H
//===================================================================================
void S4_graf_h(int y, byte w, byte h)
{



  bool today = true;

  int x = 0;
  //byte h_min_l=200;
  //byte h_max_l=0;
  float r_mm = 0;
  bool r_mm_max = false; //пищу mm если было > среднего осадков за сутки
  byte count_3ch = 0;

  for (byte i = 1; i <= entries; i++)
  {


    //begin day
    if (day_ch[i] + delta_tz == 0 && x > 0 || i == entries + 1)
    {

      if (count_3ch > 3 && r_mm_max)
      {
        byte b = int(w * 5); //all
        if (i == entries + 1 || today) b = 35; //last

        tft.setFreeFont(v7b);
        tft.setCursor(x - b, y - 1);
        do_draw_rain_mm(x - b, y - 7, r_mm, TFT_BLACK);

      }

      r_mm = 0;
      r_mm_max = false;
      count_3ch = 0;
      today = false;

    } //day_ch
    count_3ch++;

    if (rain_ch[i] > rain_l)
    { r_mm_max = true;
      //  Serial.print("r_mm_max=true");
    }

    r_mm = r_mm + rain_ch[i];


    byte xx = 5;
    do_draw_rain(x + xx, y - h + 5, rain_ch[i], TFT_BLACK);
    if (icon_ch[i] == "/01d") tft.fillCircle(x + xx, y - int(h / 2), 3, TFT_BLACK);

    byte y_val1 = map(h_ch[i], 0, 100, 0, h);
    byte y_val2 = map(h_ch[i + 1], 0, 100, 0, h);
    tft.drawLine(x, y - y_val1, x + w, y - y_val2, TFT_BLACK);
    tft.drawLine(x, y - 1 - y_val1, x + w, y - 1 - y_val2, TFT_BLACK);


    x = x + w;

  }
}


//===================================================================================
//                                 S4_draw_3ch_num
//===================================================================================
void S4_draw_3ch_num(byte y, byte w, int h_scr) //top string clock
{

  int x = 0;

  byte h = 4;
  byte day_w = 0;
  uint32_t t_begin = t_ch_b;

  tft.drawFastHLine(0, y, 479, TFT_BLACK) ; //под 3ч
  y++;
  tft.drawFastHLine(0, y, 479, TFT_BLACK) ; //под 3ч
  y++;
  // Serial.print("entries= "); Serial.println(entries);


  for (byte i = 1; i <= entries; i++)
  {



    t_begin = t_begin + 10800;


    if (day_ch[i] + delta_tz == 0 && x > 0)
    {
      //   Serial.print("day_ch[i]="); Serial.println(day_ch[i]);
      //    Serial.print("delta_tz="); Serial.println(delta_tz);
      //    Serial.print("day_ch[i] + delta_tz--------------->"); Serial.println(day_ch[i] + delta_tz);
      tft.drawFastVLine(x, y, h_scr - y, TFT_BLACK) ; //верт разд линия 00-00

    }
    else
    {
      if (x > 0)
      {
        tft.drawFastVLine(x, y, h, TFT_BLACK) ; //верт разд линия
        tft.drawFastVLine(x, h_scr - h, h, TFT_BLACK) ; //bott
      }
    }

    x = x + w;
  } //for


}


#endif //S4
