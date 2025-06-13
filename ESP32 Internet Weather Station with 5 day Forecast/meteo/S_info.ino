
void s_info_show()
{
  tft.setFreeFont(v6);
  int x = 0;
  tft.fillScreen(TFT_BLACK );
  y_onload = 15;
  do_show_define();
  //--------------------------------
  print_info(""); //init
  print_info("IP");
#ifdef pin_sd
  print_info("SD");
#endif
#ifdef aht10
  print_info("AHT10");
#endif

#ifdef bme280
  print_info("BME280");
#endif

#ifdef nrf
  print_info("NRF24");
#endif

#ifdef R433
  print_info("R433");
#endif

#ifdef vs1053
  print_info("VS1053");
#endif

#if defined (ESP32)
  print_info("PL");
#endif

  print_info("tm_long");
  print_info("tm_ntp");
#if defined (openw)
  print_info("tm_wh_now");
  print_info("tm_wh_ch");
#endif



}
