#if defined (pin_sd)&&defined (ESP32) &&!defined (no_tft)

//===============================================================================================================
//                  do_scr_play_mp3
//===============================================================================================================

void do_scr_play_mp3()
{
  static String track_name_tmp = "" ; //init ch
  static time_t t_pause_song = 0;
  static byte ref_wh = true;
  //////////////////////////////////////////
  if ( ref_kv_th || ref_temp_u || refresh_all) ref_wh = true; //ref on start or ref temp

  if (refresh_all) //первое вкл экрана
  {
    track_name_tmp = ""; //init
    show_song = true;
#ifdef tft_480_320
    //   tft.drawFastHLine(0, 50, 479, c_fm); //под песней
    tft.fillRect(0, 47, 479, 3, c_mp3) ; //ch
    tft.drawFastHLine(130, 75, 349, c_mp3) ; //ch
#endif
#ifdef tft_320_240
    //   tft.drawFastHLine(0, 50, 319, c_fm); //под песней
    tft.fillRect(0, 47, 319, 3, c_mp3) ; //ch
    tft.drawFastHLine(130, 75, 189, c_mp3) ; //ch
#endif



    tft.drawFastVLine(130, 50, 150, c_mp3);
    tft.drawFastVLine(131, 50, 150, c_mp3);
#ifdef tft_480_320
    tft.drawFastHLine(0, 200, 479, c_mp3) ; //под часами
    tft.drawFastHLine(0, 201, 479, c_mp3) ; //под часами
#endif
#ifdef tft_320_240
    tft.drawFastHLine(0, 200, 319, c_mp3) ; //под часами
#endif
    do_draw_clock();
    //   fm_setFreq(freq_fm);
    //   fm_setVolume(volume_fm);
    draw_vol();
    draw_mem();
    draw_random_mp3();
#if defined(tft_480_320) && defined(openw)
    draw_3ch();
#endif
  }


  //канал смена---------------------------------------
  if (track_name_tmp != track_name)
  {

    Serial.println("track_name_tmp!=track_name");
    tft.fillRect(132, 51, 189, 23, TFT_BLACK); //ch
    tft.setFreeFont(v9mono);
    // tft.setTextColor(TFT_YELLOW);
    tft.setCursor(140, 68);
    tft.setTextColor(TFT_WHITE);
    tft.print( "/mp3/" + track_name + ".mp3");

    // tft.print(track_name); tft.print(F(". "));

    if (!refresh_all)
    {
#if defined(tft_320_240)
      tft.fillRect(132, 85, 189, 110, TFT_BLACK);
#endif
#if defined(tft_480_320)
      tft.fillRect(132, 85, 349, 110, TFT_BLACK);
#endif
      draw_mem();
    }

    if ( ref_wh)
    {
#if defined(tft_320_240)&& defined (openw)
      do_show_th_play_ow();
#elif defined(tft_320_240)
      do_show_th_no_ow();
#elif defined(tft_480_320)
      do_show_th_n();
#endif
      ref_wh = false;
    }
    t_pause_song = millis() + 2000;
    track_name_tmp = track_name;
    new_song = true;

  }


  //что играет
  if (play_mp3 && millis() > t_pause_song && (new_song || b_draw_song))
    do_draw_song();

  //hh:mm
  if (!b_draw_song || new_song)  do_draw_h_m();

}




//------------------------------------------------------------------------------
//                      draw_3ch()
//------------------------------------------------------------------------------
#if defined(tft_480_320) && defined(openw)

void draw_3ch()
{


  byte y = 200;
  int x = 0;
  int n;
  String s = "";
  byte step = 36;

  //----------------------------------
  for (byte i = 1; i < 14; i++)
  {
    if (temperatura_ch[i] == 200) continue;


    if ((day_ch[i] == 0 || day_ch[i] == 1 || day_ch[i] == 2) && x > 0)
    {
      tft.drawFastVLine(x, y, 50, TFT_WHITE) ; //верт разд линия
      tft.drawFastVLine(x - 1, y, 50, TFT_WHITE) ; //верт разд линия
    }
    else if ((day_ch[i] == 12 || day_ch[i] == 13 || day_ch[i] == 14) && x > 0) //поправка на разные пояса
    {
      tft.drawFastVLine(x, y, 50, TFT_YELLOW) ; //верт разд линия
      tft.drawFastVLine(x - 1, y, 35, TFT_YELLOW) ; //верт разд линия
    }
    else if (x > 0) tft.drawFastVLine(x, y, 35, TFT_LIGHTGREY) ; //верт разд линия


    //jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj
    drawJpeg(icon_ch[i] + "32.jpg", x + 4, y + 5);


    //rain snowrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
    do_draw_rain(x + 4, y + 5, rain_ch[i], TFT_CYAN);



    //TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT
    unsigned int col;
    col = color_t(temperatura_ch[i]);
    tft.setTextColor(col);
    int tt = temperatura_ch[i];
    tft.setFreeFont(v9);
    if (tt < -9)
      tft.setCursor(x, y + 50);
    else
      tft.setCursor(x + 7, y + 50);
    tft.print(tt);



    x = x + step;
    if (x + step > 479) break;



  }






}

#endif //3ch

//=====================================================================
//                            draw_random_mp3
//=====================================================================

void draw_random_mp3()
{

  if   (mp3_random) tft.setTextColor(c_mp3);
  else   tft.setTextColor(TFT_BLACK);
  tft.setFreeFont(v9mono);
  tft.setCursor(93, 175);
  tft.print("R");
}
#endif //sd 


#if defined (pin_sd)&&defined (ESP32)
//=====================================================================
//                            do_mute_mp3
//=====================================================================
void do_mute_mp3()
{
  static int volume_tmp; //до mute
  static bool mute = false;
  if (refresh_all) //reset mute
  {
    mute = false;
    return;

  }
  if (!mute)//mute on
  {
    volume_tmp = volume_mp3;
    volume_mp3 = 0;
  }
  else
    volume_mp3 = volume_tmp;

  mute = !mute;
  set_vol_mp3();


}


//=====================================================================
//                            set_vol_mp3
//=====================================================================
void set_vol_mp3()
{
  //max_volume   = 21; lib2
  int volume_mp3_tmp;
  if (volume_mp3 > 100)volume_mp3 = 100;
  if (volume_mp3 < 0)volume_mp3 = 0;

#if defined (vs1053)|| defined (audio_lib2)
  volume_mp3_tmp = map(volume_mp3, 0, 100, 0, 21); //21 max
#ifdef def_coreo
  audioSetVolume(volume_mp3_tmp);
#else
  audio.setVolume(volume_mp3_tmp);
#endif
#else
  volume_mp3_tmp = map(volume_mp3, 0, 100, 0, 400);
  out->SetGain(((float)volume_mp3_tmp) / 100.0);
#endif

#ifdef debug
  Serial.print("volume_mp3=") ; Serial.println(volume_mp3) ;
#endif
#ifndef no_tft
  draw_vol();
#endif
}



//=====================================================================
//                            set_random_mp3
//=====================================================================
void set_random_mp3()
{


  if (!mp3_random)
    mp3_random = true;
  else
    mp3_random = false;
#ifndef no_tft
  draw_random_mp3();
#endif
}
#endif //esp32
