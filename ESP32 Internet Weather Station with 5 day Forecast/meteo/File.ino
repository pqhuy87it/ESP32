


//==========================================================================================================
//                        do_read_config_txt
//==========================================================================================================

bool do_read_config_txt()
{

  String filename = "/config.txt";
  bool val_0 = false;
  scr_show_count = 0;

#ifdef pin_sd
  Serial.print("SD ");
  Serial.println(filename);
  File f = SD.open( filename, FILE_READ);  // or, file handle reference for SD library
#else
  Serial.print("Spiffs ");
  Serial.println(filename);
  fs::File f = SPIFFS.open( filename, "r");    // File handle reference for SPIFFS
#endif








  if (!f) {
    Serial.println(filename); Serial.println(F(" no open,create"));
    return false;
  }

  //----------------------------
  String name, val, str;
  // byte i = 0;
#ifdef debug
  Serial.println(F("-------config.txt-------------------------------"));
#endif
  //----------------------------------
  while (f.available()) {
    str = f.readStringUntil('\n');
    str.trim();
    //Serial.println(str); //Printing for debugging purpose
    int tz = str.indexOf("#") ;
    if (tz != 0) continue;

    //---------------------------
    int tz2 = str.indexOf(";", tz + 1);
    if (tz2 == -1) continue;
    //name
    name = str.substring(tz + 1, tz2);
    name.trim();
    //Serial.print("name==");Serial.println(name);

    //val-----------------
    tz = tz2 + 1;
    tz2 = str.indexOf(";", tz);
    if (tz2 == -1) continue;
    val = str.substring(tz, tz2);
    val.trim();
    if (val == "0") val_0 = true; else val_0 = false; //если =0
    /*
      #ifdef debug
      Serial.print("val zero");Serial.println(val_0);
      Serial.print("val==");Serial.println(val);
      Serial.print("val_toint==");Serial.println(val.toInt());
      #endif
    */
    //kkkkkkkkkkkkkkkkkkkkkkkkkkkkey
    if (name == "key")
    {
      if (val.length() > 0) appid = val;
      else appid = "";
#ifdef debug
      Serial.print("key=");
      Serial.println(appid);
#endif
    }

    if (name == "p_min")
    {
      if (val.toInt() > 0) p_min = val.toInt();
      else p_min = 0;
#ifdef debug
      Serial.print("p_min=");
      Serial.println(p_min);
#endif
    }

    if (name == "p_max")
    {
      if (val.toInt() > 0) p_max = val.toInt();
      else p_max = 10000;
#ifdef debug
      Serial.print("p_max=");
      Serial.println(p_max);
#endif
    }


    if (name == "v_max")
    {
      if (val.toInt() > 0) v_max = val.toInt();
      else v_max = 99;
#ifdef debug
      Serial.print("v_max=");
      Serial.println(v_max);
#endif
    }

    if (name == "TIMEZONE")
    {
      if (val.length() > 0) TIMEZONE = val.toInt();
      else TIMEZONE = 0;
#ifdef debug
      Serial.print("TIMEZONE=");
      Serial.println(TIMEZONE);
#endif
    }


    if (name == "lat")
    {
      if (val.length() > 0) lat = val;
      else lat = "";
#ifdef debug
      Serial.print("lat=");
      Serial.println(lat);
#endif
    }

    if (name == "lon")
    {
      if (val.length() > 0) lon = val;
      else lon = "";
#ifdef debug
      Serial.print("lon=");
      Serial.println(lon);
#endif
    }


    if (name == "t_max")
    {
      if (val.toInt() != 0 || val_0 == true) t_max = val.toInt();
      else t_max = 100;
#ifdef debug
      Serial.print("t_max=");
      Serial.println(t_max);
#endif
    }


    if (name == "t_min")
    {
      if (val.toInt() != 0 || val_0 == true) t_min = val.toInt();
      else t_min = -100;
#ifdef debug
      Serial.print("t_min=");
      Serial.println(t_min);
#endif
    }


    if (name == "uv_max")
    {
      if (val.toInt() != 0 || val_0 == true) uv_max = val.toInt();
      else uv_max = 100;
#ifdef debug
      Serial.print("uv_max=");
      Serial.println(uv_max);
#endif
    }


    if (name == "delta_p")
    {
      if (val.toInt() != 0 || val_0 == true) delta_p = val.toInt();
#ifdef debug
      Serial.print("delta_p=");
      Serial.println(delta_p);
#endif
    }

    if (name == "delta_t")
    {
      if (val.toInt() != 0 || val_0 == true) delta_t = val.toInt();
#ifdef debug
      Serial.print("delta_t=");
      Serial.println(delta_t);
#endif
    }

    if (name == "beep_b")
    {
      if (val.toInt() > 0 || val_0 == true) beep_b = val.toInt();
      else beep_b = 0;
#ifdef debug
      Serial.print("beep_b=");
      Serial.println(beep_b);
#endif
    }

    if (name == "beep_e")
    {
      if (val.toInt() > 0 || val_0 == true) beep_e = val.toInt();
      else beep_e = 0;
#ifdef debug
      Serial.print("beep_e=");
      Serial.println(beep_e);
#endif
    }


    if (name == "beep_mp3_vol")
    {
      int vol_mp3 = val.toInt();
      if (vol_mp3 > 0 && vol_mp3 < 101)
      {
        beep_mp3_vol = vol_mp3;
        if (beep_mp3_vol == 100) beep_mp3_vol = 99;

      }
      else beep_mp3_vol = 0;

#ifdef debug
      Serial.print(F("beep_mp3_vol="));
      Serial.println(beep_mp3_vol);
#endif
    }


#ifdef ESP32
    //WWW
    if (name == "play_vol")
    {
      int pl_vol = val.toInt();
      if (pl_vol > 0 && pl_vol < 101)
      {
        vol_def = pl_vol;
      }
      //    else pl_vol = vol_def_mp3; //init 80

#ifdef debug
      Serial.print(F("vol_def="));
      Serial.println(vol_def);
#endif
    }

#endif //32

#if defined (pin_sd)&&defined (ESP32)
    if (name == "play_mp3_vol")
    {
      int pl_vol_mp3 = val.toInt();
      if (pl_vol_mp3 > 0 && pl_vol_mp3 < 101)
      {
        vol_def_mp3 = pl_vol_mp3;
        //  if (volume_mp3 == 100) volume_mp3 = 99;

      }
      //     else volume_mp3 = vol_def_mp3; //init=80

#ifdef debug
      Serial.print(F("vol_def_mp3="));
      Serial.println(vol_def_mp3);
#endif
    }

    //sort
    if (name == "mp3_random")
    {
      mp3_random = true;

#ifdef debug
      Serial.print(F("mp3_random="));
      Serial.println(mp3_random);
#endif
    }
#endif //sd


#if defined (fmSi4703)|| defined (fmRDA5807)
    if (name == "play_fm_vol")
    {
      int vol_def_tmp = val.toInt();
      if (vol_def_tmp > 0 && vol_def_tmp < 16)
      {
        vol_def_fm = vol_def_tmp;
      }


#ifdef debug
      Serial.print(F("vol_def_fm="));
      Serial.println(vol_def_fm);
#endif
    }
#endif //FM



    if (name == "t_b")
    {
      if (val.toInt() != 0 || val_0 == true) t_b = val.toInt();
#ifdef debug
      Serial.print(F("t_b="));
      Serial.println(t_b);
#endif
    }


    if (name == "t_e")
    {
      if (val.toInt() != 0 || val_0 == true) t_e = val.toInt();
#ifdef debug
      Serial.print(F("t_e="));
      Serial.println(t_e);
#endif
    }


    if (name == "scr_name_def")
    {
      if (val.length() > 0)
      {
        byte len = val.length() + 1;
        char buf[len];
        val.toCharArray(buf, len);
        //scr_name_def=buf;
        strcpy(scr_name_def, buf);
#ifdef debug
        Serial.print(F("scr_name_def="));
        Serial.println(scr_name_def);
#endif

      }
    }


    if (name == "esp_restart")
    {
      if (val.toInt() > 0 || val_0 == true) esp_restarts = val.toInt();
#ifdef debug
      Serial.print(F("esp_restart="));
      Serial.println(esp_restarts);
#endif
    }


    //scr round

    if (name == "S1_show")
    {
      if (val.toInt() > 0)
      {
#ifdef S1
        t_S1_show = val.toInt();
#ifdef debug
        Serial.print(F("S1_show="));
        Serial.println(t_S1_show);
#endif
        scr_show_count++;
#endif
      }
    }


    if (name == "S2_show")
    {
      if (val.toInt() > 0)
      {
#ifdef S2
        t_S2_show = val.toInt();
#ifdef debug
        Serial.print(F("S2_show="));
        Serial.println(t_S2_show);
#endif
        scr_show_count++;
#endif
      }
    }


    if (name == "S3_show")
    {
#ifndef  tft2_S3
      if (val.toInt() > 0)
      {
#ifdef S3
        t_S3_show = val.toInt();
#ifdef debug
        Serial.print(F("S3_show="));
        Serial.println(t_S3_show);
#endif
        scr_show_count++;
#endif
      }
#endif //tft2_S3

    }


    if (name == "S4_show")
    {
#ifndef  tft2_S4
      if (val.toInt() > 0)
      {
#ifdef S4
        t_S4_show = val.toInt();
#ifdef debug
        Serial.print(F("S4_show="));
        Serial.println(t_S4_show);
#endif
        scr_show_count++;
#endif
      }
#endif //tft2_S4
    }


    if (name == "S5_show")
    {
      if (val.toInt() > 0)
      {
#ifdef S5
        t_S5_show = val.toInt();
#ifdef debug
        Serial.print(F("S5_show="));
        Serial.println(t_S5_show);
#endif
        scr_show_count++;
#endif
      }
    }


    if (name == "S6_show")
    {
      if (val.toInt() > 0)
      {
#ifdef S6
        t_S6_show = val.toInt();
#ifdef debug
        Serial.print(F("S6_show="));
        Serial.println(t_S6_show);
#endif
        scr_show_count++;
#endif
      }
    }



    if (name == "Sf_show")
    {
      if (val.toInt() > 0)
      {
#ifdef Sf
        t_Sf_show = val.toInt();
#ifdef debug
        Serial.print(F("Sf_show="));
        Serial.println(t_Sf_show);
#endif
        scr_show_count++;
#endif
      }
    }



    if (name == "Sg_show")
    {

#ifndef  tft2_Sg
      if (val.toInt() > 0)
      {
#ifdef Sg
        t_Sg_show = val.toInt();
#ifdef debug
        Serial.print(F("Sg_show="));
        Serial.println(t_Sg_show);
#endif
        scr_show_count++;
#endif
      }

#endif //tft2_Sg
    }




    if (name == "S0_show")
    {
      if (val.toInt() > 0)
      {
#ifndef openw
        t_S0_show = val.toInt();
#ifdef debug
        Serial.print(F("S0_show="));
        Serial.println(t_S0_show);
#endif
        scr_show_count++;
#endif
      }
    }




#if defined (Sf)||defined (Sfepd)
    if (name == "Sf_t_slide")
    {
      if (val.toInt() > 0)
      {

        Sf_t_slide = val.toInt();
#ifdef debug
        Serial.print(F("Sf_t_slide="));
        Serial.println(Sf_t_slide);
#endif

      }
    }


#endif

    //-----------------TIMER------------------------
    if (name == "tm_alarm_period")
    {
      if (val.toInt() > 0)
      {

        tm_alarm_period = val.toInt();
#ifdef debug
        Serial.print(name);
        Serial.println(tm_alarm_period);
#endif

      }
    }

    if (name == "tm_alarm_count")
    {
      if (val.toInt() > 0)
      {

        tm_alarm_count = val.toInt();
#ifdef debug
        Serial.print(name);
        Serial.println(tm_alarm_count);
#endif

      }
    }


#ifdef pin_beep
    if (name == "tm_alarm_count_beep")
    {
      if (val.toInt() > 0)
      {

        tm_alarm_count_beep = val.toInt();
#ifdef debug
        Serial.print(name);
        Serial.println(tm_alarm_count_beep);
#endif

      }
    }

    if (name == "tm_alarm_period_beep")
    {
      if (val.toInt() > 0)
      {

        tm_alarm_period_beep = val.toInt();
#ifdef debug
        Serial.print(name);
        Serial.println(tm_alarm_period_beep);
#endif

      }
    }

#endif //beep
    //----------------





    if (name == "log") //save log
    {
      log_file = true;
#ifdef debug
      Serial.print("log_file=");
      Serial.println(log_file);
#endif
    }




    //------------------------IoT-------------------------------------------
#ifdef IoT


    //iot_id-------------
    if (name == "iot_id" || name == "iot_id_read")
    {
      String iot_id_tmp;
      if (val.length() > 0)
      {
        iot_id_tmp = val;
        char **pointer, *stringVar;
        unsigned long unsignedVar;
        char charBufVar[20];
        iot_id_tmp.toCharArray(charBufVar, 20);
        stringVar = charBufVar;
        unsignedVar = strtoul(stringVar, pointer, 10);
        if (unsignedVar > 0)
        {
          if (name == "iot_id") iot_id = unsignedVar;
          if (name == "iot_id_read") iot_id_read = unsignedVar;

#ifdef debug
          Serial.print(name + "=");
          Serial.println(unsignedVar);
#endif
        }


      }
    }


    //const char * iot_key = "";
    if (name == "iot_key" || name == "iot_key_read")
    {
      if (val.length() > 0)
      {
        val.trim();

        unsigned char* buf = new unsigned char[50];
        val.getBytes(buf, 50, 0);
        if (name == "iot_key") iot_key = (const char*)buf;
        if (name == "iot_key_read") iot_key_read = (const char*)buf;


#ifdef debug
        Serial.print(name + "=");
        if (name == "iot_key") Serial.println(iot_key);
        if (name == "iot_key_read") Serial.println(iot_key_read);
#endif


      }
      else
        iot_key = "";

    }

    //period send sec>=20
    if (name == "iot_period_send" || name == "iot_period_read")
    {
      String iot_period_send_tmp;
      if (val.length() > 0)
      {
        iot_period_send_tmp = val;
        char **pointer, *stringVar;
        unsigned long unsignedVar;

        char charBufVar[20];
        iot_period_send_tmp.toCharArray(charBufVar, 20);

        stringVar = charBufVar;
        unsignedVar = strtoul(stringVar, pointer, 10); //=324234
        if (unsignedVar > 0)
        {
          if (name == "iot_period_send") iot_period_send = unsignedVar;
          if (name == "iot_period_read") iot_period_read = unsignedVar;
        }

#ifdef debug
        Serial.print(name + "=");
        Serial.println(unsignedVar);
#endif
      }
    }



    //temp квартира(room);ном поля(num field) bme280
    if (name == "iot_T_fld_send" || name == "iot_T_fld_read")
    {
      if (val.toInt() > 0 && val.toInt() < 10)
      {
        if (name == "iot_T_fld_send") iot_T_fld_send = val.toInt();
        if (name == "iot_T_fld_read") iot_T_fld_read = val.toInt();
      }
#ifdef debug
      Serial.print(name + "=");
      Serial.println(val.toInt());
#endif
    }


    //humidity квартира(room) ;ном поля(num field) bme280

    if (name == "iot_H_fld_send" || name == "iot_H_fld_read")
    {
      if (val.toInt() > 0 && val.toInt() < 10)
      {
        if (name == "iot_H_fld_send") iot_H_fld_send = val.toInt();
        if (name == "iot_H_fld_read") iot_H_fld_read = val.toInt();
#ifdef debug
        Serial.print(name + "=");
        Serial.println(val.toInt());
#endif
      }

    }


    //temp улица(street)
    if (name == "iot_T2_fld_send" || name == "iot_T2_fld_read")
    {
      if (val.toInt() > 0 && val.toInt() < 10)
      {
        if (name == "iot_T2_fld_send") iot_T2_fld_send = val.toInt();
        if (name == "iot_T2_fld_read") iot_T2_fld_read = val.toInt();

#ifdef debug
        Serial.print(name + "=");
        Serial.println(val.toInt());
#endif
      }

    }

    //давление(pressure)
    if (name == "iot_P_fld_send" || name == "iot_P_fld_read")
    {
      if (val.toInt() > 0 && val.toInt() < 10)
      {
        if (name == "iot_P_fld_send") iot_P_fld_send = val.toInt();
        if (name == "iot_P_fld_read") iot_P_fld_read = val.toInt();
#ifdef debug
        Serial.print(name + "=");
        Serial.println(val.toInt());
#endif
      }

    }

    //скорость ветра (wind speed)
    if (name == "iot_W_fld_send")
    {
      if (val.toInt() > 0 && val.toInt() < 10)
      {
        iot_W_fld_send = val.toInt();
#ifdef debug
        Serial.print(name + "=");
        Serial.println(iot_W_fld_send);
#endif
      }
    }


    //дождь мм(rain)
    if (name == "iot_R_fld_send")
    {
      if (val.toInt() > 0 && val.toInt() < 10) iot_R_fld_send = val.toInt();
#ifdef debug
      Serial.print(F("iot_R_fld_send="));
      Serial.println(iot_R_fld_send);
#endif
    }



#endif //iot


    //----------------------mqtt
#ifdef mqtt

    if (name == "mqtt_server")
    {
      if (val.length() > 0)
      {
        val.trim();

        unsigned char* buf = new unsigned char[20];
        val.getBytes(buf, 20, 0);
        mqtt_server = (const char*)buf;


#ifdef debug
        Serial.print(name + "=");
        Serial.println(mqtt_server);
#endif

      }

    }

    // port
    if (name == "mqtt_port")
    {
      if (val.toInt() > 0 )
      {
        mqtt_port = val.toInt();
#ifdef debug
        Serial.print(name + "=");
        Serial.println(mqtt_port);
#endif
      }
    }


    if (name == "mqtt_user")
    {
      if (val.length() > 0)
      {
        val.trim();

        unsigned char* buf = new unsigned char[20];
        val.getBytes(buf, 20, 0);
        mqtt_user = (const char*)buf;


#ifdef debug
        Serial.print(name + "=");
        Serial.println(mqtt_user);
#endif

      }

    }


    if (name == "mqtt_password")
    {
      if (val.length() > 0)
      {
        val.trim();

        unsigned char* buf = new unsigned char[20];
        val.getBytes(buf, 20, 0);
        mqtt_password = (const char*)buf;


#ifdef debug
        Serial.print(name + "=");
        Serial.println(mqtt_password);
#endif

      }

    }



    if (name == "mqtt_H_topic_send")
    {
      if (val.length() > 0)
      {
        val.trim();

        unsigned char* buf = new unsigned char[50];
        val.getBytes(buf, 50, 0);
        mqtt_H_topic_send = (const char*)buf;


#ifdef debug
        Serial.print(name + "=");
        Serial.println(mqtt_H_topic_send);
#endif
      }
    }


    if (name == "mqtt_T_topic_send")
    {
      if (val.length() > 0)
      {
        val.trim();

        unsigned char* buf = new unsigned char[50];
        val.getBytes(buf, 50, 0);
        mqtt_T_topic_send = (const char*)buf;


#ifdef debug
        Serial.print(name + "=");
        Serial.println(mqtt_T_topic_send);
#endif
      }
    }



    if (name == "mqtt_T2_topic_send")
    {
      if (val.length() > 0)
      {
        val.trim();

        unsigned char* buf = new unsigned char[50];
        val.getBytes(buf, 50, 0);
        mqtt_T2_topic_send = (const char*)buf;


#ifdef debug
        Serial.print(name + "=");
        Serial.println(mqtt_T2_topic_send);
#endif
      }
    }




    if (name == "mqtt_P_topic_send")
    {
      if (val.length() > 0)
      {
        val.trim();

        unsigned char* buf = new unsigned char[50];
        val.getBytes(buf, 50, 0);
        mqtt_P_topic_send = (const char*)buf;


#ifdef debug
        Serial.print(name + "=");
        Serial.println(mqtt_P_topic_send);
#endif
      }
    }



    if (name == "mqtt_esp_name")
    {
      if (val.length() > 0)
      {
        val.trim();
        mqtt_esp_name = val;

#ifdef debug
        Serial.print(name + "=");
        Serial.println(mqtt_esp_name);
#endif
      }
    }


    //period reconn mqtt on ERR
    if (name == "t_reconn_mqtt")
    {
      String t_reconn_mqtt_tmp;
      if (val.length() > 0)
      {
        t_reconn_mqtt_tmp = val;
        char **pointer, *stringVar;
        unsigned long unsignedVar;

        char charBufVar[20];
        t_reconn_mqtt_tmp.toCharArray(charBufVar, 20);

        stringVar = charBufVar;
        unsignedVar = strtoul(stringVar, pointer, 10);
        if (unsignedVar > 0)
        {
          t_reconn_mqtt = unsignedVar;
        }

#ifdef debug
        Serial.print(name + "=");
        Serial.println(unsignedVar);
#endif
      }
    }



    //mqtt_period_send
    if (name == "mqtt_period_send" )
    {
      String mqtt_period_send_tmp;
      if (val.length() > 0)
      {
        mqtt_period_send_tmp = val;
        char **pointer, *stringVar;
        unsigned long unsignedVar;

        char charBufVar[20];
        mqtt_period_send_tmp.toCharArray(charBufVar, 20);

        stringVar = charBufVar;
        unsignedVar = strtoul(stringVar, pointer, 10);
        if (unsignedVar > 0)
        {
          mqtt_period_send = unsignedVar;
        }

#ifdef debug
        Serial.print(name + "=");
        Serial.println(unsignedVar);
#endif
      }
    }


#endif //mqtt



    //----------------wifi force---------------------


    if (name == "wifi_force_connect")
    {
      if (val.toInt() > 0)
      {
        t_wifi_force_connect = val.toInt() * 1000L;

#ifdef debug
        Serial.print(name + "=");
        Serial.println(t_wifi_force_connect / 1000 );
#endif
      }
    }






    //----------------------------

    //-------------------LED--------------------------
#if defined (pin_led)
    //   byte led_sunset = 70; // яркость после захода ,after sunset
    //   int led_sunset_delta = 70; // поправка в минутах после захода+-мин ,correction in minutes after sunset+-


    if (name == "led_def")
    {
      if (val.toInt() >= 0 && val.toInt() < 101)
      {
        led_def = val.toInt();
        led_val = led_def;
#ifdef debug
        Serial.print(name + "=");
        Serial.println(led_def);
#endif
      }
    }




    if (name == "led_night")
    {
      if (val.toInt() >= 0 && val.toInt() < 101)
      {
        led_night = val.toInt();
#ifdef debug
        Serial.print(name + "=");
        Serial.println(led_night);
#endif
      }
    }



    if (name == "led_sunset")
    {
      if (val.toInt() >= 0 && val.toInt() < 101)
      {
        led_sunset = val.toInt();
#ifdef debug
        Serial.print(name + "=");
        Serial.println(led_sunset);
#endif
      }
    }




#endif //led


#if defined (pin_pir)
    if (name == "t_pir_period")
    {
      if (val.toInt() > 0)
      {
        t_pir_period = val.toInt() * 1000L;
#ifdef debug
        Serial.print(name + " m/sec=");
        Serial.println(t_pir_period);
#endif
      }
    }

#endif //t_pir_period

    //-----------------------------------------------KEY---------------
#if defined (pin_analog_key1)

    if (name == "key1_debounce")
    {
      if (val.toInt() > 0)
      {
        key1_debounce = val.toInt();
#ifdef debug
        Serial.print(name + "=" );
        Serial.println(key1_debounce);
#endif
      }
    }

    if (name == "key1_timeout")
    {
      if (val.toInt() > 0)
      {
        key1_timeout = val.toInt();
#ifdef debug
        Serial.print(name + "=" );
        Serial.println(key1_timeout);
#endif
      }
    }


    if (name == "key1_clicktimeout")
    {
      if (val.toInt() > 0)
      {
        key1_clicktimeout = val.toInt();
#ifdef debug
        Serial.print(name  + "=");
        Serial.println(key1_clicktimeout);
#endif
      }
    }


    if (name == "key1_steptimeout")
    {
      if (val.toInt() > 0)
      {
        key1_steptimeout = val.toInt();
#ifdef debug
        Serial.print(name + "=" );
        Serial.println(key1_steptimeout);
#endif
      }
    }
    /*
        if (name == "key1_test_min_max")
        {
          if (val.toInt() >= 0)
          {
            key1_test_min_max = val.toInt();
      #ifdef debug
            Serial.print(name  + "=");
            Serial.println(key1_test_min_max);
      #endif
          }
        }
    */
#endif //pin_analog_key1


#if defined (pin_analog_key1) && defined( key1btn_1)

    if (name == "key1btn1_min")
    {
      if (val.toInt() >= 0)
      {
        key1btn1_min = val.toInt();
#ifdef debug
        Serial.print(name  + "=");
        Serial.println(key1btn1_min);
#endif
      }
    }

    if (name == "key1btn1_max")
    {
      if (val.toInt() >= 0)
      {
        key1btn1_max = val.toInt();
#ifdef debug
        Serial.print(name  + "=");
        Serial.println(key1btn1_max);
#endif
      }
    }

#endif //k1

#if defined (pin_analog_key1) && defined(key1btn_2)
    if (name == "key1btn2_min")
    {
      if (val.toInt() >= 0)
      {
        key1btn2_min = val.toInt();
#ifdef debug
        Serial.print(name  + "=");
        Serial.println(key1btn2_min);
#endif
      }
    }

    if (name == "key1btn2_max")
    {
      if (val.toInt() >= 0)
      {
        key1btn2_max = val.toInt();
#ifdef debug
        Serial.print(name  + "=");
        Serial.println(key1btn2_max);
#endif
      }
    }
#endif //k2


#if defined (pin_analog_key1) && defined(key1btn_3)
    if (name == "key1btn3_min")
    {
      if (val.toInt() >= 0)
      {
        key1btn3_min = val.toInt();
#ifdef debug
        Serial.print(name  + "=");
        Serial.println(key1btn3_min);
#endif
      }
    }

    if (name == "key1btn3_max")
    {
      if (val.toInt() >= 0)
      {
        key1btn3_max = val.toInt();
#ifdef debug
        Serial.print(name + "=" );
        Serial.println(key1btn3_max);
#endif
      }
    }
#endif //k3


#if defined (pin_analog_key1) && defined(key1btn_4)
    if (name == "key1btn4_min")
    {
      if (val.toInt() >= 0)
      {
        key1btn4_min = val.toInt();
#ifdef debug
        Serial.print(name  + "=");
        Serial.println(key1btn4_min);
#endif
      }
    }

    if (name == "key1btn4_max")
    {
      if (val.toInt() >= 0)
      {
        key1btn4_max = val.toInt();
#ifdef debug
        Serial.print(name  + "=");
        Serial.println(key1btn4_max);
#endif
      }
    }
#endif //k4


#if defined (pin_analog_key1) && defined(key1btn_5)
    if (name == "key1btn5_min")
    {
      if (val.toInt() >= 0)
      {
        key1btn5_min = val.toInt();
#ifdef debug
        Serial.print(name + "=" );
        Serial.println(key1btn5_min);
#endif
      }
    }

    if (name == "key1btn5_max")
    {
      if (val.toInt() >= 0)
      {
        key1btn5_max = val.toInt();
#ifdef debug
        Serial.print(name  + "=");
        Serial.println(key1btn5_max);
#endif
      }
    }
#endif //k5




  } //while

#ifdef debug
  Serial.print(F("scr_show_count = "));
  Serial.println(scr_show_count);
#endif



  f.close();
#ifdef debug
  Serial.println(F("---- -end--config.txt---------- -"));
#endif

  return true;
}


#ifdef pin_ir
//==========================================================================================================
//                              do_read_file_ik
//==========================================================================================================
bool do_read_file_ik()
{
  bool find = false;
  String filename = "/ik.txt";
  String str_code = "";

#ifdef pin_sd
  Serial.print("SD ");
  Serial.println(filename);
  File f = SD.open( filename, FILE_READ);  // or, file handle reference for SD library
#else
  Serial.print("Spiffs ");
  Serial.println(filename);
  fs::File f = SPIFFS.open( filename, "r");    // File handle reference for SPIFFS
#endif

  if (!f) {
    Serial.print(filename);
    Serial.println(F(" no open, create"));
    return find;
  }




  Serial.println(F("open /ik.txt   IR code"));
  String str = "";
  //-------------------
  while (f.available()) {
    str = f.readStringUntil('\n');
    Serial.println(str);
    if (str.indexOf("//") == 0) continue;
    if (str == "") continue;
    char **pointer;
    char buf[11];
    int tz = 0;
    int tz2 = 0;
    String func = "";
    ir_code = 0;
    str_code = "";
    //---------------
    tz = str.indexOf("#");
    //Serial.println("#");
    //Serial.println(tz);
    if (tz >= 0)
    {

      //func------------------------
      tz2 = str.indexOf(";", tz + 1);
      if (tz2 > 0)
      {
        func = str.substring(tz + 1, tz2);
        //Serial.println(func);
      }

      //code ir-----------------
      tz = tz2;
      tz2 = str.indexOf(";", tz + 1);
      if (tz2 > 0)
      {
        //Serial.println(str.substring(tz+1,tz2));
        str_code = str.substring(tz + 1, tz2);
        str_code.trim();
        //Serial.println(code);
        str_code.toCharArray(buf, sizeof(buf));
        ir_code = atoll(buf);


#ifdef debug
        Serial.println(func);
        Serial.println(ir_code);
#endif
      }
      if (func != "" && ir_code != 0)
      {
        //если кнопка
        if (func.substring(0, 2) == "bt")
        {
          String func_btn = func.substring(2);
          int num_btn = func_btn.toInt();
          if (num_btn < 10 && num_btn >= 0) code_btn[num_btn] = ir_code;
          Serial.print ("num_btn="); Serial.println(num_btn);
          continue;
        }
        if (func.substring(0, 2) == "id")
        {

          if (ir_code > 0) id_ir = str_code;//string
#ifdef debug
          Serial.print ("id IR="); Serial.println(id_ir);
#endif
          continue;
        }
        //ищу фун
        for (int i = 0; i < fn_count; i++) {

          if (func == fn[i]) {
            code[i] = ir_code;
#ifdef debug
            Serial.print("FIND----->"); Serial.print (func);
            Serial.println(code[i]);
#endif
            find = true;
          }
        }



      }



    }

  }
  f.close();
  return find;
}

#endif // pin_ir



//--------------------------------------------------------
//                      do_count_pl_list
//--------------------------------------------------------
#if defined (ESP32)
void do_count_pl_list()
{
  byte count = 0;
  String filename;

  for (byte i = 1; i < 10; i++)
  {
    filename = "/i_stat" + String(i) + ".txt";

#if defined (pin_sd)
    if (SD.exists(filename))
    {
      Serial.print("ok SD ");
      Serial.println(filename);
      pl_list_max = i;
    }
    else
    {
      Serial.print("not SD ");
      Serial.println(filename);
      break;
    }

#else
    //    fl = SPIFFS.open( filename, "r");    // File handle reference for SPIFFS
    if (SPIFFS.exists(filename))
    {
      Serial.print("ok SPIFFS ");
      Serial.println(filename);
      pl_list_max = i;
    }
    else
    {
      Serial.print("not SPIFFS ");
      Serial.println(filename);
      break;
    }

#endif

  }
  Serial.print(F("pl_list_max "));
  Serial.println(pl_list_max);
}
#endif //32



//-----------------------------------------------------------------------------
//               do_log()
//-----------------------------------------------------------------------------

void do_log()
{
#ifdef debug
  //if (log_str == "") return;
  File f ;
  String filename;
  if (log_file)
  {

    filename = "/log.txt";

#if defined (pin_sd) &&defined (ESP32)
    Serial.print("SD ");
    Serial.println(filename);
    fs::FS &fs = SD;
    f = SD.open(filename, FILE_APPEND);//FILE_APPEND

#endif
#if defined (pin_sd) &&defined (ESP8266)
    Serial.print("SD ");
    Serial.println(filename);
    f = SD.open( filename, FILE_WRITE);  // or, file handle reference for SD library
#endif
#if !defined (pin_sd)
    Serial.print("Spiffs ");
    Serial.println(filename);
    //fs::File
    f = SPIFFS.open( filename, "a+");    // File handle reference for SPIFFS
#endif


    if (!f) {
      Serial.println(filename);
      Serial.println(F(" log_file no open,create"));
      //   return;
    }

  }//log_file

  //t=now();
  String t_str = "";
  if (now_hour < 10) t_str = "0" + String(now_hour);
  else
    t_str = String(now_hour);

  if (now_min < 10) t_str = t_str + ":0" + String(now_min);
  else
    t_str = t_str + ":" + String(now_min);

  if (second(t) < 10) t_str = t_str + ":0" + String(second(t));
  else
    t_str = t_str + ":" + String(second(t));



  log_str = t_str + ";" + log_str;

  Serial.print(F("log="));
  Serial.println(log_str);
  Serial.print(F("mem="));
  Serial.println(ESP.getFreeHeap());

  // Serial.print("log_file = "); Serial.println(log_str);
  //  Serial.print("f = "); Serial.println(f);

  if (log_file && f)
  {
    //  const char *str_ch = log_str.c_str();
    Serial.print("save log " );
    if (f.println(log_str))
      Serial.println("Ok");
    else
      Serial.println("ERR save");
    f.close();
  }
  log_str = "";

#endif// debug
}





//-----------------------------------------------------------------------------
//               do_log_max_size()
//-----------------------------------------------------------------------------
#ifdef debug
void do_log_max_size()
{
  //  File f;
  String filename;

  filename = "/log.txt";


#ifdef pin_sd
  Serial.print("SD ");
  Serial.println(filename);
#ifdef ESP32
  fs::FS &fs = SD;
  File f = fs.open(filename, FILE_READ);

  if (!f) {
    Serial.println(F(" log_file control MAX_SIZE not open"));
    return;
  }

  uint32_t log_size = f.size();
  Serial.print(F("log size="));
  Serial.println(log_size);
  if (log_size > max_size_log)
  {
    Serial.println("Removing " + filename);
    fs.remove(filename);
  }

#endif
#ifdef ESP8266
  File  f = SD.open( filename, FILE_READ);

  if (!f) {
    Serial.println(F(" log_file control MAX_SIZE not open"));
    return;
  }

  uint32_t log_size = f.size();
  Serial.print(F("log size="));
  Serial.println(log_size);
  if (log_size > max_size_log)
  {
    Serial.println("Removing " + filename);
    SD.remove(filename);
  }

#endif
#else
  Serial.print("Spiffs ");
  Serial.println(filename);
  fs::File   f = SPIFFS.open( filename, "r");    // File handle reference for SPIFFS

  if (!f) {
    Serial.println(F(" log_file control MAX_SIZE not open"));
    return;
  }

  uint32_t log_size = f.size();
  Serial.print(F("log size="));
  Serial.println(log_size);
  if (log_size > max_size_log)
  {
    f.close();
    delay(200);
    Serial.println("Removing " + filename);
    if (!SPIFFS.remove(filename))
      Serial.println("Delete big log failed");
    ;
  }
#endif


}

#endif












//--------------------------------------------------------
//                      do_maxnum_file_foto
//--------------------------------------------------------
#if defined (pin_sd) && defined (Sf)
unsigned int do_maxnum_file_foto()
{
  unsigned int max_num = 0;
  String s_fname = "";
  byte n1, n2;
  unsigned int max_num_tmp = 0;
  String s_fname_num;
  String dir_tmp = "";

  File fname;
  File dir;
  dir = SD.open("/");

  //Serial.println(F("maxnum /foto/*.jpg"));
#ifdef epd
  Serial.println(F("search max_num /foto_bw/*.bmp"));
  String dir_foto = "foto_bw";
#else
  //  tft.println(F("search max_num /foto/*.jpg"));
  String dir_foto = "foto";
#endif

  while (true) {
    File fname = dir.openNextFile();
    if (! fname)      break;      // no more files

    if (fname.isDirectory())
    {
      dir_tmp = fname.name();
      if (dir_tmp.indexOf(dir_foto) == -1)
      {
        fname.close();
        continue;
      }
      Serial.print("dir foto Ok - ");  Serial.println(dir_foto);
      //  Serial.println(dir_tmp);
      fname.close();
      break;
    }

  } //while




  if (dir_tmp == "")
  {
    Serial.print("NO dir foto - "); Serial.println(dir_foto);
    print_info("NO dir foto");
    return 0;
  }


  dir = SD.open(dir_tmp);

  while (true)
  {



    if (max_num_tmp % 3 == 0)
      tft.fillCircle(x_onload, y_onload, 3, TFT_YELLOW);
    else tft.fillCircle(x_onload, y_onload, 3, TFT_BLACK);

    fname =  dir.openNextFile();
    if (! fname)       break;

    s_fname = fname.name();
    n1 = s_fname.lastIndexOf("/");
    if (n1 == -1) n1 = 0; else n1++;
    n2 = s_fname.lastIndexOf(".");

    s_fname_num = s_fname.substring(n1, n2);

    max_num_tmp = s_fname_num.toInt();
    if (max_num_tmp > max_num) max_num = max_num_tmp;
    fname.close();

  }

  tft.fillCircle(x_onload, y_onload, 3, TFT_BLACK);
  return max_num;



}
#endif  //pin_sd


//--------------------------------------------------------
//                      do_maxnum_file_mp3
//--------------------------------------------------------
#if defined (pin_sd)&&defined (ESP32)
void do_maxnum_file_mp3()
{
  unsigned int num_tmp = 0;
  String s_fname = "";
  byte n1, n2;
  unsigned int max_num_tmp = 0;
  unsigned int min_num_tmp = 55555;
  // String track_name_str = "";
  String s_fname_num;
  String dir_tmp = "";

  File fname;
  File dir;
  dir = SD.open("/");

  //  Serial.println(F("maxnum /mp3/*.mp3"));
  // tft.println(F("search max_num /mp3/*.mp3"));


  while (true) {
    File fname = dir.openNextFile();
    if (! fname)      break;      // no more files

    if (fname.isDirectory())
    {
      dir_tmp = fname.name();
      if (dir_tmp.indexOf("mp3") == -1)
      {
        //      fname.close();
        continue;
      }
      Serial.println("dir /mp3 Ok");
      // Serial.println(dir_tmp);
      fname.close();
      break;
    }

  } //while




  if (dir_tmp == "")
  {
    Serial.println("NO dir /mp3/");
    print_info("NO dir mp3");
    return ;
  }


  dir = SD.open(dir_tmp);

  while (true)
  {
    if (max_num_tmp % 3 == 0)
      tft.fillCircle(x_onload, y_onload, 3, TFT_YELLOW);
    else tft.fillCircle(x_onload, y_onload, 3, TFT_BLACK);

    fname =  dir.openNextFile();
    if (! fname)       break;

    s_fname = fname.name();
    n1 = s_fname.lastIndexOf("/");
    if (n1 == -1) n1 = 0; else n1++;
    n2 = s_fname.lastIndexOf(".");

    s_fname_num = s_fname.substring(n1, n2);

    num_tmp = s_fname_num.toInt();
    if (min_num_tmp > num_tmp)
      min_num_tmp = num_tmp;

    if (max_num_tmp < num_tmp) max_num_tmp = num_tmp;
    fname.close();




  }
  tft.fillCircle(x_onload, y_onload, 3, TFT_BLACK);

  if (max_num_tmp > 0)
  {
    max_num_mp3 = max_num_tmp;
    min_num_mp3 = min_num_tmp;
  }




}



#endif  //pin_sd
