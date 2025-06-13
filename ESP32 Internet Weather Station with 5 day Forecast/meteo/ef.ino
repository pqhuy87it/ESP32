#if defined (tft_400_300) && defined (Sfepd)

//-------------------------------------------------------------
//              Sf_epd
//-------------------------------------------------------------
void Sf_epd()
{
  static time_t next_time = 0;

  if (refresh_all) next_time = 0;

  if (millis() < next_time)
  {
    return;
  }


  if (max_jpg_num == 0)
  {
    Serial.println(F("count Foto=0"));
    next_time = millis() + 2 * 1000L;
    return;
  }
  tft.setTextColor(TFT_BLACK);

  //ffffffffffffffffffffffffffffffffffffffffffffffffffffffffff
  unsigned int r_num = random(1, max_jpg_num + 1);
  String fname = "/foto_bw/" + String(r_num) + ".bmp";
  Serial.println(fname);
  File fnd = SD.open(fname.c_str(), FILE_READ);

  if (!fnd)
  {
#ifdef debug
    Serial.println("no file foto "); Serial.println(fname);
#endif
    return;
  }
  //drawBmp(const char *filename, int16_t x, int16_t y, bool with_color = true, bool partial_update = true, bool overwrite = true);
  drawBmp(fname, 0, 0, true, false, false);
  next_time = millis() + Sf_t_slide * 1000L;


  //dddddddddddddddddddddddddddddddddddddddddddddddddd
#ifdef Sfepd_show_date
  Sfepd_ref_date();
#endif

  //ttttttttttttttttttttttttttttttttttttttttttttttttt
#if defined (Sfepd_show_Tu)
  int temp;
  tft.setCursor(2, 295);
#ifdef  openw
  if (temperatura_ch[0] != 200) temp = temperatura_ch[0];
#endif
  if (temp_u != 200) temp = temp_u;

  //ico
  tft.setPartialWindow(0, 268, 80, 32 );
  tft.firstPage();
  do
  {
#if defined (openw)
    if (icon_ch[0] != "")
    {
      tft.setFreeFont(m18);
      tft.print(wh_char(0,'c'));
    }
#endif
    if (temp != 200)
    {
      tft.setFreeFont(v9b);
      tft.print(temp);
      tft.setFreeFont(v7b);
      tft.print("c");
    }
  }
  while (tft.nextPage());
#endif //Sf_show_Tu


  //hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh
#ifdef Sfepd_show_TH
  S_show_th_room(346, 0);
#endif







}



//--------------------------------------------------------------------------------
//                  Sfepd_ref_date
//--------------------------------------------------------------------------------

#ifdef Sfepd_show_date
void  Sfepd_ref_date()
{

  tft.setFreeFont(v9b);
  tft.setPartialWindow(0, 0, 64, 24);

  tft.firstPage();
  do
  {
    tft.fillRect(0, 0 , 64, 24,  TFT_WHITE);
    tft.setCursor(2, 20);
    tft.print(now_date);
  }
  while (tft.nextPage());


}
#endif //Sf_show_date


#endif //pin_sd
