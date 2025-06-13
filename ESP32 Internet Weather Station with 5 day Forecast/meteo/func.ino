#ifndef epd
//------------------------------------------------------------------------------
//                        do_scr_round
//------------------------------------------------------------------------------

void do_scr_round()
{

  static uint32_t t_next = 0;
  static uint32_t t_start = 0;

  //если сейчас на экране кот не учавствует в круге выхожу
  //if (day_ch[0]==0) return; //нет данных о погоде

  if (t_start > 0) //второй проход, считаю дельту
  {
    t_next = t_next + (millis() - t_start); //поправка на прорисовку экрана,2-4сек
    t_start = 0;
  }




  if (millis() < t_next) return;

  Serial.println(F("do_scr_round()"));

  //--------------------
#ifdef openw

  //1
#ifdef S1
  static uint32_t t_S1_next = 0;
  if (t_S1_show > 0 && millis() > t_S1_next) //second(t) чтобы сначало прорисовал минуты >2sec
  {
    t_S1_next = millis() + t_S1_show * 1000 + t_S2_show * 1000 + t_S3_show * 1000 + t_S4_show * 1000 + t_S5_show * 1000 + t_S6_show * 1000 + t_Sf_show * 1000 + t_Sg_show * 1000;
    if (do_find_scr( (char *)"S1")) scr_num_show(scr_number);
    Serial.println(F("S1_show"));
    t_next = millis() + t_S1_show * 1000;
    t_start = millis();

    return;
  }
#endif


  //2
#ifdef S2
  static uint32_t t_S2_next = 0;
  if (t_S2_show > 0 && millis() > t_S2_next) //second(t) чтобы сначало прорисовал минуты >2sec
  {
    t_S2_next = millis() + t_S1_show * 1000 + t_S2_show * 1000 + t_S3_show * 1000 + t_S4_show * 1000 + t_S5_show * 1000 + t_S6_show * 1000 + t_Sf_show * 1000 + t_Sg_show * 1000;
    if (do_find_scr( (char *)"S2")) scr_num_show(scr_number);
    Serial.println(F("S2_show"));
    t_next = millis() + t_S2_show * 1000;
    t_start = millis();
    return;
  }
#endif

  //3
#ifdef S3
  static uint32_t t_S3_next = 0;
  if (t_S3_show > 0 && millis() > t_S3_next) //second(t) чтобы сначало прорисовал минуты >2sec
  {
    t_S3_next = millis() + t_S1_show * 1000 + t_S2_show * 1000 + t_S3_show * 1000 + t_S4_show * 1000 + t_S5_show * 1000 + t_S6_show * 1000 + t_Sf_show * 1000 + t_Sg_show * 1000;
    if (do_find_scr( (char *)"S3")) scr_num_show(scr_number);
    Serial.println(F("S3_show"));
    t_next = millis() + t_S3_show * 1000;
    t_start = millis();
    return;
  }
#endif


  //4
#ifdef S4
  static uint32_t t_S4_next = 0;
  if (t_S4_show > 0 && millis() > t_S4_next) //second(t) чтобы сначало прорисовал минуты >2sec
  {
    t_S4_next = millis() + t_S1_show * 1000 + t_S2_show * 1000 + t_S3_show * 1000 + t_S4_show * 1000 + t_S5_show * 1000 + t_S6_show * 1000 + t_Sf_show * 1000 + t_Sg_show * 1000;
    if (do_find_scr( (char *)"S4")) scr_num_show(scr_number);
    Serial.println(F("S4_show"));
    t_next = millis() + t_S4_show * 1000;
    t_start = millis();
    return;
  }
#endif


  //5
#ifdef S5
  static uint32_t t_S5_next = 0;
  if (t_S5_show > 0 && millis() > t_S5_next) //second(t) чтобы сначало прорисовал минуты >2sec
  {
    t_S5_next = millis() + t_S1_show * 1000 + t_S2_show * 1000 + t_S3_show * 1000 + t_S4_show * 1000 + t_S5_show * 1000 + t_S6_show * 1000 + t_Sf_show * 1000 + t_Sg_show * 1000;

    if (do_find_scr( (char *)"S5")) scr_num_show(scr_number);
    Serial.println(F("S5_show"));
    t_next = millis() + t_S5_show * 1000;
    t_start = millis();
    return;
  }
#endif


  //6
#ifdef S6
  static uint32_t t_S6_next = 0;
  if (t_S6_show > 0 && millis() > t_S6_next) //second(t) чтобы сначало прорисовал минуты >2sec
  {
    t_S6_next = millis() + t_S1_show * 1000 + t_S2_show * 1000 + t_S3_show * 1000 + t_S4_show * 1000 + t_S5_show * 1000 + t_S6_show * 1000 + t_Sf_show * 1000 + t_Sg_show * 1000;

    if (do_find_scr( (char *)"S6")) scr_num_show(scr_number);
    Serial.println(F("S6_show"));
    t_next = millis() + t_S6_show * 1000;
    t_start = millis();
    return;
  }
#endif


  //Sf
#ifdef Sf
  static uint32_t t_Sf_next = 0;
  if (t_Sf_show > 0 && max_jpg_num > 0 && millis() > t_Sf_next) //second(t) чтобы сначало прорисовал минуты >2sec
  {
    t_Sf_next = millis() + t_S1_show * 1000 + t_S2_show * 1000 + t_S3_show * 1000 + t_S4_show * 1000 + t_S5_show * 1000 + t_S6_show * 1000 + t_Sf_show * 1000 + t_Sg_show * 1000;
    if (do_find_scr( (char *)"Sf")) scr_num_show(scr_number);
    Serial.println(F("Sf_show"));
    t_next = millis() + t_Sf_show * 1000;
    t_start = millis();
    return;
  }
#endif



  //Sg
#ifdef Sg
  static uint32_t t_Sg_next = 0;
  if (t_Sg_show > 0 && millis() > t_Sg_next) //second(t) чтобы сначало прорисовал минуты >2sec
  {
    t_Sg_next = millis() + t_S1_show * 1000 + t_S2_show * 1000 + t_S3_show * 1000 + t_S4_show * 1000 + t_S5_show * 1000 + t_S6_show * 1000 + t_Sf_show * 1000 + t_Sg_show * 1000;

    if (do_find_scr( (char *)"Sg")) scr_num_show(scr_number);
    Serial.println(F("Sg_show"));
    t_next = millis() + t_Sg_show * 1000;
    t_start = millis();
    return;
  }
#endif


#endif   //openw
  //--------------------



  //--------------------
#if !defined (openw) &&   defined (tft_320_240)&&   defined (pin_sd)
  //0
  static uint32_t t_S0_next = 0;
  if (t_S0_show > 0 && max_jpg_num > 0 && millis() > t_S0_next) //second(t) чтобы сначало прорисовал минуты >2sec
  {
    t_S0_next = millis() + t_S0_show * 1000 + t_Sf_show * 1000;
    if (do_find_scr( (char *)"S0")) scr_num_show(scr_number);
    Serial.println(F("S0_show"));
    t_next = millis() + t_S0_show * 1000;
    t_start = millis();
    return;
  }


  //F
#ifdef Sf
  if (t_Sf_show > 0 && max_jpg_num > 0 && millis() > t_Sf_next) //second(t) чтобы сначало прорисовал минуты >2sec
  {
    t_Sf_next = millis() + t_S0_show * 1000 + t_Sf_show * 1000;
    if (do_find_scr( (char *)"Sf")) scr_num_show(scr_number);
    Serial.println(F("Sf_show"));
    t_next = millis() + t_Sf_show * 1000;
    t_start = millis();
    return;
  }
#endif

#endif  //!defined (openw) &&   defined (tft_320_240)
  //--------------------



  //--------------------
#if !defined (openw) &&   defined (tft_480_320) &&   defined (pin_sd)


  if (t_S0_show > 0 && max_jpg_num > 0 && millis() > t_S0_next) //second(t) чтобы сначало прорисовал минуты >2sec
  {
    t_S0_next = millis() + t_S0_show * 1000 + t_Sf_show * 1000;
    if (do_find_scr( (char *)"Sn")) scr_num_show(scr_number);  //Sn=S0
    Serial.println(F("S0_show"));
    t_next = millis() + t_S0_show * 1000;
    t_start = millis();
    return;
  }

  //F
#ifdef Sf
  if (t_Sf_show > 0 && max_jpg_num > 0 && millis() > t_Sf_next) //second(t) чтобы сначало прорисовал минуты >2sec
  {
    t_Sf_next = millis() + t_S0_show * 1000 + t_Sf_show * 1000;
    if (do_find_scr( (char *)"Sf")) scr_num_show(scr_number);
    Serial.println(F("Sf_show"));
    t_next = millis() + t_Sf_show * 1000;
    t_start = millis();
    return;
  }
#endif  //Sf

#endif //!defined (openw) &&   defined (tft_480_320)
  //--------------------



}
#endif //#ifndef epd
//

//----------------------------------------------------------------------------
//                            do_sun
//----------------------------------------------------------------------------
//voshod zah
void do_sun()
{
  TimeLord tardis;

  tardis.TimeZone(TIMEZONE * 60);
  byte len;
  char Str[20] = "";
  len = lat.length() + 1;
  lat.toCharArray(Str, len);   // копирование String в массив myStr8
  float latf = atof(Str);  //  преобразование в float
  //Serial.print ("latf ");
  //Serial.println (latf);


  //Str[20]="";
  len = lon.length() + 1;
  lon.toCharArray(Str, len);   // копирование String в массив myStr8
  float lonf = atof(Str);  //  преобразование в float

  //Serial.print ("lonf ");
  //Serial.println (lonf);



  tardis.Position(latf, lonf); // tell TimeLord where in the world we are
  time_t tm = now();
  byte today[] = {  0, 0, 12, day(tm) , month(tm), year(tm) - 2000    }; // store today's date (at noon) in an array for TimeLord to use

  if (tardis.SunRise(today)) // if the sun will rise today (it might not, in the [ant]arctic)

  {
    t_sun_v = do_sun_format(today[tl_hour], today[tl_minute]);
    t_sun_v_hh = today[tl_hour];
#ifdef pin_led
    t_sun_v_min = today[tl_hour] * 60 + today[tl_minute];
#endif


  }

  if (tardis.SunSet(today)) // if the sun will set today (it might not, in the [ant]arctic)
  {
    t_sun_z = do_sun_format(today[tl_hour], today[tl_minute]);
    t_sun_z_hh = today[tl_hour];
#if defined (pin_led)||defined(pin_sunset_out_hight)||defined(pin_sunset_out_low)
    t_sun_z_min = today[tl_hour] * 60 + today[tl_minute];
#endif

  }
  Serial.println(t_sun_v);
  Serial.println(t_sun_z);
#if defined (pin_led)||defined(pin_sunset_out_hight)||defined(pin_sunset_out_low)
#ifdef debug
  Serial.println("t_sun_v_min=" );
  Serial.println(t_sun_v_min);
  Serial.println("t_sun_z_min=");
  Serial.println(t_sun_z_min);
#endif
#endif
}


//--------------------------------------------------------------
//            do_sun_format()
//--------------------------------------------------------------
String do_sun_format(byte h, byte m)
{
  String str = "";

  if (h < 10) str = "0";
  str = str + (String)h + ":";

  if (m < 10) str = str + "0";
  str = str + (String)m;

  return str;

}


//--------------------------------------------------------------------
//                     do_phasemoon
//--------------------------------------------------------------------
String do_phasemoon()
{

  time_t t = now();
  byte the_time_moon[] = {0, 0, 0, day(t), month(t), year(t) - 2000};
  float ph = myLord.MoonPhase(the_time_moon);



  byte f;
  if (ph < 0.02085) f = 23;
  else if (ph < 0.06255) f = 0;
  else if (ph < 0.10425) f = 1;
  else if (ph < 0.14595) f = 2;
  else if (ph < 0.18765) f = 3;
  else if (ph < 0.22935) f = 4;
  else if (ph < 0.27105) f = 5;
  else if (ph < 0.31275) f = 6;
  else if (ph < 0.35445) f = 7;
  else if (ph < 0.39615) f = 8;
  else if (ph < 0.43785) f = 9;
  else if (ph < 0.47955) f = 10;
  else if (ph < 0.52125) f = 11;
  else if (ph < 0.56295) f = 12;
  else if (ph < 0.60465) f = 13;
  else if (ph < 0.64635) f = 14;
  else if (ph < 0.68805) f = 15;
  else if (ph < 0.72975) f = 16;
  else if (ph < 0.77145) f = 17;
  else if (ph < 0.81315) f = 18;
  else if (ph < 0.85485) f = 19;
  else if (ph < 0.89655) f = 20;
  else if (ph < 0.93825) f = 21;
  else if (ph < 0.97995) f = 22;
  else
    f = 23;

  String fl = "/m" + String(f);

  return fl;
}


#ifndef epd
///////////////////////////
//цвет температуры
unsigned int  color_t(int c)
{
  unsigned int col = 0;;
  if (c > t4) col = t4_color;
  else if (c > t3) col = t3_color;
  else if (c > t2) col = t2_color;
  else if (c > t1) col = t1_color;
  else if (c > t0) col = t0_color; //>0
  else if (c > t_1) col = t_1_color;
  else if (c > t_2) col = t_2_color;
  else
    col = t_3_color;
  return col;
}

#endif //#ifndef epd




#ifdef pin_beep
//==========================================================
void do_beep(unsigned int Fr, unsigned int Dur)
{

#ifdef beep_act
#ifdef debug
  Serial.println("do_beep(beep_act)" );
#endif
  digitalWrite(pin_beep, HIGH ); // включаем пьезоизлучатель
  delay(Dur);  // на 1000 мс (1 сек),
  digitalWrite(pin_beep, LOW );
  return;
#endif

#if defined( beep_pass)&&defined( ESP8266)
#ifdef debug
  Serial.println("do_beep(beep_pass ESP8266)" );
#endif
  /*
    analogWriteFreq(Fr);
    analogWrite(pin_beep,500);
    delay(Dur);
    analogWrite(pin_beep,0);
  */


  noTone(pin_beep);
  tone (pin_beep, Fr);
  delay(Dur);
  noTone(pin_beep);


#endif

#if defined( beep_pass)&&defined( ESP32)
#ifdef debug
  Serial.println("do_beep(beep_pass ESP32)" );
#endif
  tone(pin_beep, Fr, Dur, 0);
#endif



}


#endif




//==============================================================
//                    scr_num_show
//==============================================================

void scr_num_show(signed char num)
{

  if (num == scr_count_pack)
    scr_number = 0;
  else if (num < 0)
    scr_number = scr_count_pack - 1;
  else
    scr_number = num;

  refresh_all = true;
  tft.fillScreen(TFT_BLACK);
#ifdef debug
  Serial.print(F("scr_number=")); Serial.println(scr_number);
  Serial.print("scr_name=");
  Serial.println(scr_name[scr_number]);
#endif


}


//====================================================
//                    scr_pack()
//===================================================
void scr_pack( )
{
#ifdef debug
  Serial.println(F("void scr_pack"));
  Serial.println(F("init scr"));
#endif
  char *scr_name_tmp[scr_count];

  for (byte i = 0; i < scr_count; i++)
  {
    scr_name[i] = (char *)"";
    scr_name_tmp[i] = (char *) "";
  }
  //-----------------------EPD

#ifdef S1epd
  scr_name[1] = (char *)"S1";
#ifdef debug
  Serial.println(F("scr_name[1]=S1epd"));
#endif
#endif

#ifdef S2epd
  scr_name[2] = (char *)"S2";
#ifdef debug
  Serial.println(F("scr_name[2]=S2epd"));
#endif
#endif//epd


#ifdef S3epd
  scr_name[3] = (char *)"S3";
#ifdef debug
  Serial.println(F("scr_name[3]=S3epd"));
#endif
#endif//epd


#ifdef S4epd
  scr_name[4] = (char *)"S4";
#ifdef debug
  Serial.println(F("scr_name[4]=S4epd"));
#endif
#endif//epd

#if defined (pin_sd) && defined (Sfepd)
  scr_name[10] = (char *)"Sf";
#ifdef debug
  Serial.println(F("scr_name[10]=Sf"));
#endif
#endif




#ifdef S0
  scr_name[0] = (char *) "S0";
#ifdef debug
  Serial.println(F("scr_name[0]=S0"));
#endif
#endif



#ifdef S1
#ifndef tft2_S1
  scr_name[1] = (char *)"S1";
#ifdef debug
  Serial.println(F("scr_name[1]=S1"));
#endif
#endif
#endif


#ifdef S2
#ifndef tft2_S2
  scr_name[2] = (char *)"S2";
#ifdef debug
  Serial.println(F("scr_name[2]=S2"));
#endif
#endif
#endif

#ifdef S3
#ifndef tft2_S3
  scr_name[3] = (char *)"S3";
#ifdef debug
  Serial.println(F("scr_name[3]=S3"));
#endif
#endif
#endif

#ifdef S4
#ifndef tft2_S4
  scr_name[4] = (char *) "S4";
#ifdef debug
  Serial.println(F("scr_name[4]=S4"));
#endif
#endif
#endif

#ifdef S5
#ifndef tft2_S5
  scr_name[5] = (char *)"S5";
#ifdef debug
  Serial.println(F("scr_name[5]=S5"));
#endif
#endif
#endif

#ifdef S6
#ifndef tft2_S6
  scr_name[6] = (char *)"S6";
#ifdef debug
  Serial.println(F("scr_name[6]=S6"));
#endif
#endif
#endif




#if defined (pin_sd) && defined (Sf)
  scr_name[10] = (char *)"Sf";
#ifdef debug
  Serial.println(F("scr_name[10]=Sf"));
#endif
#endif



#ifdef Sg
#ifndef tft2_Sg
  scr_name[11] = (char *) "Sg";
#ifdef debug
  Serial.println(F("scr_name[11]=Sg"));
#endif
#endif
#endif


#ifdef Sn //last
  scr_name[12] = (char *) "Sn";
#ifdef debug
  Serial.println(F("scr_name[12]=Sn"));
#endif
#endif


  byte i_in = 0;


  for (byte i = 0; i < scr_count; i++)
  {
    //#ifdef debug
    //Serial.print(F("scr_name=="));
    //Serial.println(scr_name[i]);
    //Serial.print(F(" i="));
    //Serial.println(i);
    //#endif

    if (scr_name[i] != (char *) "")
      //    if (scr_name[i] != 0)

    {
      //Serial.println("ADD");
      scr_name_tmp[i_in] = scr_name[i];
      i_in++;
      scr_count_pack = i_in;

    }

    scr_name[i] =  (char *)"";
  }

#ifdef debug
  Serial.print("scr_count_pack=");
  Serial.println(scr_count_pack);
#endif
  for (byte i = 0; i < scr_count; i++)
  {
    scr_name[i] = scr_name_tmp[i];
#ifdef debug
    Serial.print("SCR["); Serial.print(i); Serial.print("]=");
    Serial.println(scr_name[i]);
#endif
  }


}




//=========================================================
//                        do_find_scr
//=========================================================

bool do_find_scr(char sname[ ] )
{
  bool fnd = false;
  //#ifdef debug
  //  Serial.print("do_find_scr="); Serial.println(sname);
  //  Serial.print("sizeoff sname ="); Serial.println(sizeof(sname));
  //#endif

  for (byte i = 0; i < scr_count; ++i)
  {
    //#ifdef debug
    //Serial.print("scr_name[i] =");Serial.println(sizeof(scr_name[i]));
    //#endif
    if ( strcmp(scr_name[i], sname) == 0 )
    {
      fnd = true;
#ifdef debug
      Serial.print("Scr find="); Serial.println(i);
#endif
      scr_number = i;

      break;
    }

  }
#ifdef debug
  if (!fnd) Serial.print(F("NO find scr_name "));
  Serial.println(sname);
#endif

  return fnd;
}



#ifndef epd
//------------------------------------------------------------------------------------------
//                              do_c_frame
//------------------------------------------------------------------------------------------
void do_c_frame() //color frame
{
  static uint32_t t_next = 0;
  if (millis() < t_next && !ref_temp_u && !ref_kv_th) return;
  t_next = millis() + 15000;


  if (period_ulica == 0) c_tu_frame = TFT_CYAN;
  else if (millis() > period_ulica + t_ds_ref * 1000L * 2)  c_tu_frame = TFT_RED;
  else
    c_tu_frame = TFT_YELLOW;


  if (t_read_tk == 0) c_th_frame = TFT_CYAN;
  else if (millis() > t_read_tk + t_bme_ref * 1000L * 3)  c_th_frame = TFT_RED;
  else
    c_th_frame = TFT_YELLOW;


}

#endif //#ifndef epd

//------------------------------------------------------------------------------------------
//                              do_led
//------------------------------------------------------------------------------------------
#ifdef pin_led
void do_led(byte num)
{

  static byte val = 200;
  if (val == num) return;
  val = num;
  int led_tmp = 0;
  if (num == 0) led_on = false; //scr off
  else
  {
    led_on = true;
    led_val = num;
  }

  led_tmp = map(num, 0, 100, 0, 1023);
#ifdef ESP32
  ledcWrite(ledChannel, led_tmp);
#else
  analogWrite(pin_led, led_tmp);
#endif

#ifdef debug
  Serial.print(F( "led_%=")); Serial.println(num);
  Serial.print(F( "led_val=")); Serial.println(led_tmp);
#endif

}

#endif



//------------------------------------------------------------------------------------------
//                              do_sleep
//------------------------------------------------------------------------------------------
#if defined (pin_pir) && defined (ESP32)
bool do_sleep()
{
#ifdef debug
  Serial.println(F("-------L_sleep----------"));
#endif
  gpio_wakeup_enable((gpio_num_t)pin_pir, GPIO_INTR_HIGH_LEVEL);
  //  gpio_wakeup_enable ((gpio_num_t)SW, GPIO_INTR_LOW_LEVEL);
  esp_sleep_enable_gpio_wakeup();
  esp_light_sleep_start();




}
#endif




//------------------------------------------------------------------------------------------
//                              read_sensors_radio
//------------------------------------------------------------------------------------------


#ifdef only_radio //если  одно радио опрос лок. сенсоров
void read_sensors_radio()
{
  ///OTA
#ifdef ota_update
  if (WiFi.status() == WL_CONNECTED) ArduinoOTA.handle();
#endif

  //ntp
  do_ntp();

#ifdef  pin_ds1820
  do_read_ds1820();
#endif
#ifdef bme280
  do_read_bme();
#endif
#ifdef si7021
  do_read_si7021();
#endif
#ifdef pin_dht
  do_read_dht();
#endif

}

#endif



//=====================================================================
//                            serial_time_start
//=====================================================================

#ifdef debug
void serial_time_start()
{
  if (sync_t)
  {
    printDigits(hour()); Serial.print(":");
    printDigits(minute()); Serial.print(":");
    printDigits(second());
    Serial.print(" ");
  }

  str_millis(millis()); //ret log_str
  Serial.print(F("ESP works "));
  Serial.println(log_str);
#ifdef ESP32
  Serial.print(F(" total  mem = "));
  Serial.print(ESP.getHeapSize());
  Serial.print(F(" free  mem = "));
  Serial.println(ESP.getFreeHeap());
#endif
}


void printDigits(int digits) {

  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
#endif //debug
