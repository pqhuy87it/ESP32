
#ifdef openw

void do_openw()
{

  if (WiFi.status() == WL_CONNECTED) {
    if (millis() < lastConnectionTime_now) lastConnectionTime_now = 0;
    if (millis() - lastConnectionTime_now > postingInterval_now || lastConnectionTime_now == 0) {

      if (do_parse_now())  //zapros ch
      {

        ref_wh_now = true;
      }
    }

  }




  // wh 3 hour--------------------------------------
  if (WiFi.status() == WL_CONNECTED) {
    if (millis() < lastConnectionTime_ch) lastConnectionTime_ch = 0;
    if (millis() - lastConnectionTime_ch > postingInterval_ch || lastConnectionTime_ch == 0) {

      if (do_parse_ch())  //zapros ch
      {

        ref_wh = true;
      }
    }

  }



  //UV--------------------------------------
  if (WiFi.status() == WL_CONNECTED) {
    if (millis() < lastConnectionTime_uv) lastConnectionTime_uv = 0;
    if (millis() - lastConnectionTime_uv > postingInterval_uv || lastConnectionTime_uv == 0 ) {
      do_parse_uv(); //data in

    }
  }





}



//===================================================================================================
//            do_draw_rain_mm
//===================================================================================================

void do_draw_rain_mm(int x, int y, float mm, int color)
{


#if defined (two_tft)
  if (tft2_act)
  {
    tft2.setTextColor(color);
    tft2.setFreeFont(v6);
    tft2.setCursor(x, y);
    char outstr[5];
    dtostrf(mm, 4, 1, outstr);
    tft2.print(outstr);
  }
  else
  {
    tft.setTextColor(color);
    tft.setFreeFont(v6);
    tft.setCursor(x, y);
    char outstr[5];
    dtostrf(mm, 4, 1, outstr);
    tft.print(outstr);

  }

#else
  tft.setTextColor(color);
  tft.setFreeFont(v6);
  tft.setCursor(x, y);
  char outstr[5];
  dtostrf(mm, 4, 1, outstr);
  tft.print(outstr);

#endif




}




//===================================================================================================
//            do_draw_rain
//===================================================================================================

void do_draw_rain(int x, int y, float mm, int color)
{

  if (mm == 0)  return;
  byte step = 8;

#if defined (two_tft)
  if (tft2_act)
  {
    tft2.fillCircle(x, y, 2, color);
    if (mm >= rain_l) tft2.fillCircle(x, y + step, 2, color);
    if (mm >= rain_m) tft2.fillCircle(x, y + step * 2, 2, color);

  }
  else
  {
    tft.fillCircle(x, y, 2, color);
    if (mm >= rain_l) tft.fillCircle(x, y + step, 2, color);
    if (mm >= rain_m) tft.fillCircle(x, y + step * 2, 2, color);


  }
#else
  {
    tft.fillCircle(x, y, 2, color);
    if (mm >= rain_l) tft.fillCircle(x, y + step, 2, color);
    if (mm >= rain_m) tft.fillCircle(x, y + step * 2, 2, color);


  }
#endif



}





//===================================================================================================
//            S_show_uv
//===================================================================================================
//показ значка uv


void S_show_uv(bool vis, int x, int y)
{
  ///////////////UUUUUUUUVVVVVVVVVVVVV////////////////////////
  int u_color;
  x++;
  y++;

  if (vis == true)
    u_color = do_uvcolor();
  else
    u_color = TFT_BLACK;



#if defined (two_tft)
  if (tft2_act)
  {
    tft2.fillRoundRect(x, y, 8, 8, 3, u_color);
    tft2.drawLine(x + 2, y + 1, x + 16, y, u_color);
    tft2.drawLine(x + 2, y + 2, x + 16, y + 14, u_color);
    tft2.drawLine(x + 2, y + 1, x + 2, y + 14, u_color);
    if (int(uv) > 0)
    {
      tft2.setTextColor(TFT_YELLOW);
      tft2.setFreeFont(v6);
      tft2.setCursor(x, y + 24);
      tft2.print(int(uv));
    }


  }
  else
  {
    tft.fillRoundRect(x, y, 8, 8, 3, u_color);
    tft.drawLine(x + 2, y + 1, x + 16, y, u_color);
    tft.drawLine(x + 2, y + 2, x + 16, y + 14, u_color);
    tft.drawLine(x + 2, y + 1, x + 2, y + 14, u_color);
    if (int(uv) > 0)
    {
      tft.setTextColor(TFT_YELLOW);
      tft.setFreeFont(v6);
      tft.setCursor(x, y + 24);
      tft.print(int(uv));
    }


  }
#else
  tft.fillRoundRect(x, y, 8, 8, 3, u_color);
  tft.drawLine(x + 2, y + 1, x + 16, y, u_color);
  tft.drawLine(x + 2, y + 2, x + 16, y + 14, u_color);
  tft.drawLine(x + 2, y + 1, x + 2, y + 14, u_color);
  if (int(uv) > 0)
  {
    tft.setTextColor(TFT_YELLOW);
    tft.setFreeFont(v6);
    tft.setCursor(x, y + 24);
    tft.print(int(uv));
  }


#endif




}


//===================================================================================================
//            do_uvcolor
//===================================================================================================

int do_uvcolor()
{
  int color_uv;
  if (uv < 0) color_uv = TFT_BLACK; //нет данный закрашиваю
  else if (uv <= uv1)
    color_uv = uv1_color;
  else if (uv <= uv2)
    color_uv = uv2_color;
  else if (uv <= uv3)
    color_uv = uv3_color;
  else if (uv <= uv4)
    color_uv = uv4_color;
  else if (uv <= uv5)
    color_uv = uv5_color;

  return color_uv;
}



//==============================================================
//								do_deg
//==============================================================
//напр. ветра
void do_deg(float gr)
{

  wind_ico = "";
#ifdef Sg
  c_wind = 0;
#endif

  if (gr >= 0 && gr <= 30) {
    wind_ico = "/wind_0.jpg";
    c_wind = TFT_CYAN;
  }
  if (gr >= 31 && gr <= 60) {
    wind_ico = "/wind_2.jpg";
    c_wind = TFT_CYAN;
  }

  if (gr >= 61 && gr <= 120) {
    wind_ico = "/wind_4.jpg";  //восток
    c_wind = Sg_c_wing_E;
  }

  if (gr >= 121 && gr <= 150) {
    wind_ico = "/wind_6.jpg";
    c_wind = TFT_YELLOW;
  }
  if (gr >= 151 && gr <= 210) {
    wind_ico = "/wind_8.jpg";
    c_wind = TFT_YELLOW;
  }
  if (gr >= 211 && gr <= 240) {
    wind_ico = "/wind_10.jpg";
    c_wind = TFT_YELLOW;
  }
  //if (gr>=241 && gr<=300){wind_ico="/wind_12.jpg";c_wind=0x3DE4;}
  if (gr >= 241 && gr <= 300) {
    wind_ico = "/wind_12.jpg";  //запад
    c_wind = Sg_c_wing_W;
  }

  if (gr >= 301 && gr <= 330) {
    wind_ico = "/wind_14.jpg";
    c_wind = TFT_CYAN;
  }
  if (gr >= 331 && gr <= 360) {
    wind_ico = "/wind_0.jpg";
    c_wind = TFT_CYAN;
  }
}






//==============================================================
//                color_day
//==============================================================

unsigned int color_day(byte w_day)
{
  unsigned int color = 0;
  if (w_day == 1)
    color = c_holiday;
  else if (w_day == 7)
    color = c_holiday;
  else if (w_day == 2)
    color = c_monday;
  else if (w_day == 6)
    color = c_friday;
  else
    color = c_working;

  return color;
}

#endif  //openw
