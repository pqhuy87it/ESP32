#if defined (fmSi4703)|| defined (fmRDA5807)|| defined (fmSi4703_lib2)

//===============================================================================================================
//                  do_scr_play fm()
//===============================================================================================================

void do_scr_play_fm()
{
  static byte ch_tmp_fm = 254; //init ch

  //////////////////////////////////////////
  if (refresh_all) //первое вкл экрана
  {
    ch_tmp_fm = 254; //init
#ifdef tft_480_320
    //   tft.drawFastHLine(0, 50, 479, c_fm); //под песней
    tft.fillRect(0, 47, 479, 3, c_fm) ; //ch
    tft.drawFastHLine(130, 75, 349, c_fm) ; //ch
#endif
#ifdef tft_320_240
    //   tft.drawFastHLine(0, 50, 319, c_fm); //под песней
    tft.fillRect(0, 47, 319, 3, c_fm) ; //ch
    tft.drawFastHLine(130, 75, 189, c_fm) ; //ch
#endif



    tft.drawFastVLine(130, 50, 150, c_fm);
#ifdef tft_480_320
    tft.drawFastHLine(0, 200, 130, c_fm) ; //под часами
#endif
#ifdef tft_320_240
    tft.drawFastHLine(0, 200, 320, c_fm) ; //под часами
#endif
    do_draw_clock();
    fm_setFreq(freq_fm);
    fm_setVolume(volume_fm);
    draw_vol_fm();
    draw_mem();

  }


  //#ifdef ref_sensor_on_play //ref sensors
  if ( ref_kv_th || ref_temp_u)
  {
    if ((play_fm && ch_tmp_fm == ch_fm ))
    {
      Serial.println(F("ref_kv_th||ref_temp_u"));
#if defined(tft_320_240)&& defined (openw)
      do_show_th_play_ow();
#elif defined(tft_320_240)
      do_show_th_no_ow();
#elif defined(tft_480_320)
      do_show_th_n();
#endif

    }

  }//#endif //ref_sensor_on_play




  //канал смена ccccccccccccccccccccccccccc
  if (ch_tmp_fm != ch_fm)
  {

    Serial.println("ch_tmp!=ch");
    int w;
#ifdef tft_320_240
    w = 239 ;
#endif
#ifdef tft_480_320
    w = 479;
#endif
    tft.fillRect(0, 0, w, 40, TFT_BLACK); //ch
    tft.setFreeFont(v9mono);
    //  tft.setTextColor(TFT_YELLOW);
    tft.setCursor(10, 35);
    tft.setTextColor(TFT_WHITE);
    tft.print(ch_fm); tft.print(F(". "));
    tft.setTextColor(c_ch_fnd);
    tft.print(ch_name_fm.substring(0, 10) + " ");
    tft.setTextColor(TFT_CYAN);
    tft.print(String(freq_fm));

    if (!refresh_all)
    {
      fm_setFreq(freq_fm);
      fm_setVolume(volume_fm);
      draw_vol_fm();
      ch_tmp_fm = ch_fm;
      draw_mem();
    }


#if defined(tft_320_240)&& defined (openw)
    do_show_th_play_ow();
#elif defined(tft_320_240)
    do_show_th_no_ow();
#elif defined(tft_480_320)
    do_show_th_n();
#endif


    ch_tmp_fm = ch_fm;
  }

  //hh:mm
  do_draw_h_m();




}





//=====================================================================
//                            draw_vol_fm
//=====================================================================
void draw_vol_fm()
{
  tft.drawFastVLine(124, 119, 80, TFT_BLACK);
  tft.drawFastVLine(126, 119, 80, TFT_BLACK);

  int color;
  if (volume_fm > 13) color = TFT_MAGENTA;
  else if (volume_fm > 9) color = TFT_YELLOW;
  else if (volume_fm > 0) color = TFT_GREEN;
  else color = TFT_RED; //mute


  int vol = volume_fm * 4;
  tft.drawFastVLine(124, 199 - vol, vol, color);
  tft.drawFastVLine(126, 199 - vol, vol, color);

  //------------
  tft.fillRect(93, 180, 30, 16, TFT_BLACK);
  tft.setFreeFont(v9mono);
  tft.setTextColor(color);
  tft.setCursor(98, 195);
  if (volume_fm == 0)
    tft.print("M");
  else
    tft.print(volume_fm);




}



//=====================================================================
//                            do_mute_fm
//=====================================================================

void do_mute_fm()
{
  static int volume_tmp; //до mute
  static bool mute = false;

  if (!mute)//mute on
  {
    volume_tmp = volume_fm;
    volume_fm = 0;
#ifdef fmRDA5807
    FM.setMute(true);
#endif
  }
  else
  {
#ifdef fmRDA5807
    FM.setMute(false);
#endif
    volume_fm = volume_tmp;

  }
  fm_setVolume(volume_fm);
  draw_vol_fm();

  mute = !mute;

}





#endif //fmSi4703
