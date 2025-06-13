//------------------------------------------------------------------------------------------
//                              do_show_define
//------------------------------------------------------------------------------------------
void do_show_define()
{
  String str = "";
#ifdef tft_320_240
  tft.setFreeFont(v6);
#endif
#ifdef tft_480_320
  tft.setFreeFont(v9mono);
#endif
#ifdef tft_400_300
  tft.setFreeFont(v7);
#endif
#ifdef debug
  Serial.println( "Compiled: " __DATE__ ", " __TIME__ ", " __VERSION__);
  Serial.print(F( "Arduino IDE version: ")); Serial.println( ARDUINO, DEC);
  Serial.print("SdkVersion: ");
  Serial.println(ESP.getSdkVersion());
#endif

  //  print_define (ver, -1);//meteo

  str = "V:" + String(ver);
#ifdef ESP8266
  //  print_define ( (char *)"ESP8266", -1);
  str = str + ";E8266";
#endif

#ifdef ESP32
  //  print_define ( (char *)"ESP32", -1);
  str = str + ";E32";
#endif


#ifdef debug
  //  print_define ((char *)"debug", -1);
  str = str + ";dbg";
#endif

#ifdef ftp
  //  print_define ((char *)"ftp", -1);
  str = str + ";ftp";
#endif


#ifdef IoT
  //  print_define ((char *)"IoT", -1);
  str = str + ";IoT";
#endif


#ifdef mqtt
  // print_define ((char *)"mqtt", -1);
  str = str + ";mqtt";
#endif
  print_define (str.c_str(), -1);


#ifdef tft_320_240
  print_define ((char *)"tft_320_240", -1);
#endif

#ifdef tft_480_320
  print_define ((char *)"tft_480_320", -1);
#endif

#ifdef tft_400_300
  print_define ((char *)"tft_400_300", -1);
  print_define ((char *)"EPD_BUSY", EPD_BUSY);
  print_define ((char *)"EPD_CS", EPD_CS);
  print_define ((char *)"EPD_RST", EPD_RST);
  print_define ((char *)"EPD_DC", EPD_DC);
  print_define ((char *)"EPD_SCK", EPD_SCK);
  print_define ((char *)"EPD_MISO", EPD_MISO);
  print_define ((char *)"EPD_MOSI", EPD_MOSI);
#endif

#ifdef epd

#ifdef S1epd
  str = str + "S1epd;";
#endif
#ifdef S2epd
  str = str + "S2epd;";
#endif
#ifdef S3epd
  str = str + "S3epd;";
#endif
#ifdef S4epd
  str = str + "S4epd;";
#endif
#ifdef Sfepd
  str = str + "Sfepd;";
#endif
  if (str != "")
    print_define ((char *)str, -1);

#endif //epd




#ifdef two_tft

#if defined (tft2_S1)
  print_define ((char *)"two_tft tft2_S1", -1);
#endif

#if defined (tft2_S2)
  print_define ((char *)"two_tft tft2_S2", -1);
#endif


#if defined (tft2_S3)
  print_define ((char *)"two_tft tft2_S3", -1);
#endif


#if defined (tft2_S4)
  print_define ((char *)"two_tft tft2_S4", -1);
#endif


#if defined (tft2_S5)
  print_define ((char *)"two_tft tft2_S5", -1);
#endif


#if defined (tft2_S6)
  print_define ((char *)"two_tft tft2_S6", -1);
#endif


#if defined (tft2_Sg)
  print_define ((char *)"two_tft tft2_Sg", -1);
#endif


#if defined (tft2_Sf)
  print_define ((char *)"two_tft tft2_Sf", -1);
#endif

#endif //two_tft


#ifdef only_radio
  print_define ((char *)"only_radio", -1);
#endif


#ifdef pin_sd
  print_define ((char *)"pin_sd", pin_sd);
#endif


#ifdef ota_update
  print_define ((char *)"ota_update", -1);
#endif




#ifdef http_update
  print_define ((char *)"http_update", -1);
#endif



#ifdef openw
  print_define ((char *)"openw", -1);
#else
  print_define ((char *)"NO openw", -1);
#endif


#ifdef wf_send_Tu
  print_define ((char *)"wf_send_Tu", -1);
#endif
#ifdef wf_send_Tk
  print_define ((char *)"wf_send_Tk", -1);
#endif
#ifdef wf_send_Hh
  print_define ((char *)"wf_send_Hh", -1);
#endif

#ifdef wf_read_Tu
  print_define ((char *)"wf_read_Tu", -1);
#endif
#ifdef wf_read_Tk
  print_define ((char *)"wf_read_Tk", -1);
#endif
#ifdef wf_read_Hh
  print_define ((char *)"wf_read_Hh", -1);
#endif

#ifdef pin_btn1
  print_define ((char *)"pin_btn1", pin_btn1);
#endif

#ifdef pin_btn2
  print_define ((char *)"pin_btn2", pin_btn2);
#endif

#ifdef pin_btn3
  print_define ((char *)"pin_btn3", pin_btn3);
#endif

#ifdef pin_btn4
  print_define ((char *)"pin_btn4", pin_btn4);
#endif


#ifdef pin_btn_touch
  print_define ((char *)"btn_touch", pin_btn_touch);
#endif

#ifdef pin_ir
  print_define ((char *)"pin_ir", pin_ir);
#endif

#ifdef R433
#ifdef pin_r433_rx
  print_define ((char *)"pin_r433_rx", pin_r433_rx);
#endif
#endif

#ifdef nrf
  print_define ((char *)"pin_nrf_CE", pin_nrf_CE);
  print_define ((char *)"pin_nrf_CS", pin_nrf_CS);
  str = "";
#ifdef nrf_read_Tu  //temp_u Темп улица //street temp
  str = str + "Tu;";
#endif
#ifdef nrf_read_Tk  //temp_u Темп улица //street temp
  str = str + "Tk;";
#endif
#ifdef nrf_read_Hh  //temp_u Темп улица //street temp
  str = str + "Hg;";
#endif
#ifdef nrf_read_Pr  //temp_u Темп улица //street temp
  str = str + "Pr;";
#endif
  if (str != "")
  {
    str = "NRF_r:" + str;
    print_define (str.c_str(), -1);
  }
#endif //nrf

  //---------------------WIFI
#ifdef wf_read
  str = "";
#ifdef wf_read_Tu  //temp_u Темп улица //street temp
  str = str + "Tu;";
#endif
#ifdef wf_read_Tk  //temp_u Темп улица //street temp
  str = str + "Tk;";
#endif
#ifdef wf_read_Hh  //temp_u Темп улица //street temp
  str = str + "Hg;";
#endif
  if (str != "")
  {
    str = "WF read:" + str;
    print_define (str.c_str(), -1);
  }
#endif //wifi


#ifdef wf_send
  str = "";
#ifdef wf_send_Tu  //temp_u Темп улица //street temp
  str = str + "Tu;";
#endif
#ifdef wf_send_Tk  //temp_u Темп улица //street temp
  str = str + "Tk;";
#endif
#ifdef wf_send_Hh  //temp_u Темп улица //street temp
  str = str + "Hg;";
#endif
  if (str != "")
  {
    str = "WF send:" + str;
    print_define (str.c_str(), -1);
  }
#endif //wifi


#ifdef pin_ds1820
  print_define ((char *)"pin_ds1820", pin_ds1820);
#endif

#ifdef si7021
  print_define ((char *)"si7021", -1);
#endif

#ifdef bme280
  print_define ((char *)"bme280", -1);
#endif


#ifdef pin_dht
  print_define ((char *)"pin_dht", pin_dht);
#endif



#ifdef pin_beep
  print_define ((char *)"pin_beep", pin_beep);
#endif



#ifdef encoder
  print_define ((char *)"enc_CLK", CLK);
  print_define ((char *)"enc_DT", DT);
  print_define ((char *)"enc_SW", SW);
  print_define ((char *)"enc_TYPE", TYPE);
#endif



#if defined (encoder_lib2)
  print_define ((char *)"encoder_lib2", -1);
  print_define ((char *)"enc_CLK", CLK);
  print_define ((char *)"enc_DT", DT);
  print_define ((char *)"enc_SW", SW);
#endif

#if defined (audio_lib2)
  print_define ((char *)"audio_lib2", -1);
#endif

#if defined (vs1053)
  print_define ((char *)"vs1053", -1);
#endif

#ifdef PCM5102
  print_define ((char *)"PCM5102", -1);
#endif



#ifdef pin_led
  print_define ((char *)"pin_led", pin_led);
#endif

#ifdef pin_pir
  print_define ((char *)"pin_pir", pin_pir);
#endif

#ifdef pin_pir_out_hight
  print_define ((char *)"pir_out_hight", pin_pir_out_hight);
#endif


#ifdef pin_pir_out_low
  print_define ((char *)"pir_out_low", pin_pir_out_low);
#endif

#if defined (fmSi4703)
  print_define ((char *)"Si4703_rst", pin_fm_rst);
  print_define ((char *)"Si4703_sdio", pin_fm_sdio);
  print_define ((char *)"Si4703_sclk", pin_fm_sclk);
#endif

#if defined (fmSi4703_lib2)
  print_define ((char *)"Si4703_rst", pin_fm_rst);
  print_define ((char *)"Si4703_sdio", pin_fm_sdio);
  print_define ((char *)"Si4703_sclk", pin_fm_sclk);
#endif


#if defined (fmRDA5807)
  print_define ((char *)"RDA5807 sdio", pin_fm_sdio);
  print_define ((char *)"RDA5807 sclk", pin_fm_sclk);

#endif

#if defined (pin_playWWW_out_hight)
  print_define ((char *)"playWWW_out_hight", pin_playWWW_out_hight);
#endif
#if defined (pin_playFM_out_hight)
  print_define ((char *)"playFM_out_hight", pin_playFM_out_hight);
#endif
#if defined (pin_playMP3_out_hight)
  print_define ((char *)"playMP3_out_hight", pin_playMP3_out_hight);
#endif
#if defined (pin_playALL_out_hight)
  print_define ((char *)"playALL_out_hight", pin_playALL_out_hight);
#endif

#if defined (pin_analog_key1)
  print_define ((char *)"analog_key1", pin_analog_key1);
#endif





#if defined (timechange_summer_winter)
  print_define ((char *)"ch_summer_winter", -1);
#endif


#if defined (lcd_display)
  print_define ((char *)"lcd_display", -1);
#endif

#ifdef aht10
  print_define ((char *)"aht10", -1);
#endif


#ifdef def_core0
  print_define ((char *)"play_core0", -1);
#endif



  //-------------------SCR------
  str = "";
#ifdef S0
  str = str + "S0;";
#endif



#ifdef S1
  str = str + "S1;";
#endif

#ifdef S2
  str = str + "S2;";
#endif

#ifdef S3
  str = str + "S3;";
#endif

#ifdef S4
  str = str + "S4;";
#endif

#ifdef S5
  str = str + "S5;";
#endif


#ifdef S6
  str = str + "S6;";
#endif

#ifdef Sf
  str = str + "Sf;";
#endif



#ifdef Sg
  str = str + "Sg;";
#endif


#ifdef Sn //last
  str = str + "Sn;";
#endif
  if (str != "")
  {
    print_define (str.c_str(), -2); //-1 new row
    str = "";
  }

  // print_define ((char *)"", -1); //end

}

//------------------------------------------------------------------------------------------
//                              print_define
//------------------------------------------------------------------------------------------
void print_define(const char * n_def, int8_t pin)
{
  Serial.print(F("#def "));
  Serial.print(n_def);
#if  !defined(no_tft)
  static byte x = 0;

  //long string
  if (pin == -2) //end
  {
    if (x > 0)       y_onload += y_step; //new line
    tft.setCursor(0, y_onload);
    tft.print(n_def);
    y_onload += 3;
#ifdef tft_320_240
    tft.drawFastHLine(0, y_onload, 320, TFT_CYAN);
#endif
#ifdef tft_480_320
    tft.drawFastHLine(0, y_onload, 480, TFT_CYAN);
#endif
#ifdef tft_400_300
    tft.drawFastHLine(0, y_onload, 400, TFT_BLACK);
#endif
    y_onload += y_step;
    x = 0;
    Serial.println("");
    return;
  }

#ifndef epd
  tft.setTextColor(TFT_LIGHTGREY);
#endif

  tft.setCursor(x, y_onload);
  tft.print(n_def);



  if (pin >= 0)
  {
    Serial.print(F(" pin="));
    Serial.println(pin);
#ifndef epd
    tft.setTextColor(TFT_CYAN);
#endif
    tft.print("-");
    tft.println(pin);
  }
  else Serial.println("");




  if (x == 0)
  {
#ifdef tft_320_240
    x = 180;
#endif
#if defined (tft_480_320)||defined (tft_400_300)
    x = 240;
#endif

  }
  else
  {
    y_onload += y_step;
    x = 0;
  }

#endif //no tft
}



//------------------------------------------------------------------------------------------
//                              print_info
//------------------------------------------------------------------------------------------

void print_info(const char * par)
{
#if !defined(no_tft)
  static byte x = 0;
  static byte n = 1;
  if (par == "")
  {
    x = 0;
    n = 1;
    return;
  }
  Serial.print("par---->");
  Serial.println(par);

#ifdef tft_320_240
  tft.setFreeFont(v6);
#endif
#ifdef tft_480_320
  tft.setFreeFont(v9mono);
#endif
#ifdef tft_400_300
  tft.setFreeFont(v7);
#endif
  tft.setCursor(x, y_onload);


#ifdef pin_sd
  if (strcmp(par, "SD_ERR") == 0)
  {
    tft.setTextColor(TFT_RED);
    tft.print(par);
    return;
  }
  if (strcmp(par, "SD_OK") == 0)
  {
    tft.setTextColor(TFT_GREEN);
    tft.print("SD");
    return;
  }

  if (strcmp(par, "SD") == 0  )
  {
    tft.setTextColor(TFT_GREEN);
    tft.print(par); tft.print(" ");
#ifdef Sf
    tft.setTextColor(TFT_YELLOW); tft.print("FOTO=");
    tft.setTextColor(TFT_CYAN);
    tft.print(max_jpg_num); tft.print(" ");
#endif //sf
#if defined (pin_sd) && defined (ESP32)
    tft.setTextColor(TFT_YELLOW);
    tft.print(" MP3=");
    tft.setTextColor(TFT_CYAN);
    tft.print(max_num_mp3);
#endif //sd esp32

  } //sd


  if (strcmp(par, "NO dir foto") == 0)
  {
    tft.setTextColor(TFT_RED);
    tft.print(par);
  }

  if (strcmp(par, "NO dir mp3") == 0)
  {
    tft.setTextColor(TFT_RED);
    tft.print(par);
  }
#endif //sd

#if defined (ESP32)
  if (strcmp(par, "PL") == 0)
  {
    tft.setTextColor(TFT_YELLOW);
    tft.print("Playlist=");
    tft.setTextColor(TFT_CYAN);
    tft.print(pl_list_max);
  }
#endif //esp32

  if (strcmp(par, "config OK") == 0)
  {
    tft.setTextColor(TFT_GREEN);
    tft.print(par);
  }
  if (strcmp(par, "config.txt ERR") == 0)
  {
    tft.setTextColor(TFT_RED);
    tft.print(par);
  }

#ifdef bme280
  if (strcmp(par, "BME280") == 0)
  {
    if (status_bme) tft.setTextColor(TFT_GREEN); else
      tft.setTextColor(TFT_RED);
    tft.print(par);
  }
#endif

#ifdef aht10
  if (strcmp(par, "AHT10") == 0)
  {
    if (status_aht) tft.setTextColor(TFT_GREEN); else
      tft.setTextColor(TFT_RED);
    tft.print(par);
  }
#endif

#ifdef nrf
  if (strcmp(par, "NRF24") == 0)
  {
    if (status_nrf) tft.setTextColor(TFT_GREEN); else
      tft.setTextColor(TFT_RED);
    tft.print(par);
  }
#endif


#ifdef R433
  if (strcmp(par, "R433") == 0)
  {
    if (status_r433) tft.setTextColor(TFT_GREEN); else
      tft.setTextColor(TFT_RED);
    tft.print(par);
  }
#endif


#ifdef vs1053
  if (strcmp(par, "VS1053") == 0)
  {
    if (status_vs1053) tft.setTextColor(TFT_GREEN); else
      tft.setTextColor(TFT_RED);
    tft.print(par);
  }
#endif

  if (strcmp(par, "NTP") == 0)
  {
    if (sync_t)
    {
      tft.setTextColor(TFT_GREEN);
      tft.print("NTP");
    }
    else
    {
      tft.setTextColor(TFT_RED);
      tft.print("NTP");
    }
  }

  if (strcmp(par, "IP") == 0)
  {
    // tft.setTextColor(TFT_YELLOW);    tft.print(par); tft.print(" ");
    tft.setTextColor(TFT_GREEN); tft.print(WiFi.localIP()); tft.print(" ");
    tft.setTextColor(TFT_CYAN); tft.print(String( wifi_rssi())); tft.print(F("%"));
  }

  if (strcmp(par, "tm_ntp") == 0)
  {
    tft.setTextColor(TFT_WHITE);
    tft.print("NTP sync ");
    if (tm_ntp > 0)
    {
      str_millis(millis() - tm_ntp);
      tft.setTextColor(TFT_CYAN); tft.print(log_str);
    }
  }

  if (strcmp(par, "tm_long") == 0)
  {
    tft.setTextColor(TFT_WHITE);
    tft.print("ESP start ");
    str_millis(millis());
    tft.setTextColor(TFT_CYAN); tft.print(log_str);
    //   tft.setTextColor(TFT_YELLOW); tft.print(F("%"));
  }

  if (strcmp(par, "tm_wh_now") == 0)
  {
    tft.setTextColor(TFT_WHITE);
    tft.print("OpenW now ");
    if (tm_wh_now > 0)
    {
      str_millis(millis() - tm_wh_now);
      tft.setTextColor(TFT_CYAN); tft.print(log_str);
    }
  }

  if (strcmp(par, "tm_wh_ch") == 0)
  {
    tft.setTextColor(TFT_WHITE);
    tft.print("OpenW 4d ");
    if (tm_wh_ch > 0)
    {
      str_millis(millis() - tm_wh_ch);
      tft.setTextColor(TFT_CYAN); tft.print(log_str);
    }
  }

  //------------------------------------------------------
  n++;
  if (n % 2 == 0)
  {
#ifdef tft_320_240
    x = 180;
#endif
#if defined (tft_480_320)||defined (tft_400_300)
    x = 240;
#endif
  }
  else
  {
    y_onload += y_step;
    x = 0;
  }
#endif
}

//------------------------------------------------------------------------------------------
//                              str_millis
//------------------------------------------------------------------------------------------
void str_millis(uint32_t tt)
{
  log_str = "";
  int mls = tt / 1000;
  int tmp = 0;
  tmp = mls / 60 / 60;
  if (tmp < 10)  log_str = log_str + "0" + String(tmp);
  else  log_str = log_str + ":"  + String(tmp);
  tmp = mls / 60 % 60;
  if (tmp < 10)  log_str = log_str + ":" + "0" + String(tmp);
  else  log_str = log_str + ":"  + String(tmp);

  tmp = mls  % 60;
  if (tmp < 10)  log_str = log_str + ":" + "0" + String(tmp);
  else  log_str = log_str + ":"  + String(tmp);
}
