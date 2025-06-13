#ifdef openw
bool do_parse_now()
{
  bool find = false;
  // float r_mm = 0;


#ifdef debug
  Serial.print(F("do_parse_now() begin mem="));
  Serial.println( ESP.getFreeHeap());
  time_t tm = now();
  Serial.println(url_now);
  Serial.print(F("start ch3   "));
  Serial.print(hour(tm));
  Serial.print(F("-"));
  Serial.print(minute(tm));
  Serial.print(F("-"));
  Serial.println(second(tm));
#endif
  //------------------------------------------------

  OpenWeatherMapCurrentData data;
  if (owNOW.updateCurrent(&data, url_now) == true)
    if (data.cod != 0)
    {
      do_init_now();
      find = true;


      //pppppppppppppppppp pppppppppppppppppppppppppppp
      if (data.pressure > 0)
      {
#ifndef hPa
        davlenie0 = int(data.pressure * 0.75) + delta_p;
#else
        davlenie0 = data.pressure + delta_p;
#endif
        if (p7_max < davlenie0) p7_max = davlenie0;
        if (p7_min > davlenie0) p7_min = davlenie0;
      }
      time_t time ;
      time = data.observationTime + TIMEZONE * 3600L;;
      day_ch[0] = hour(time);
#ifdef debug
      Serial.print(F("Time parse now="));
      Serial.println( ctime(&time));
#endif
      icon_ch[0] = "/" + data.icon;
      temperatura_ch[0] = data.temp;
      v_ch[0] = data.windSpeed; //veter
      v_gr_ch[0] = data.windDeg;

      /////////rainrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
      if (data.rain > 0.1) //отсеиваю мелочь
        rain_ch[0] = data.rain;
      if (data.snow > 0.1 && data.snow > data.rain)
        rain_ch[0] = data.snow; ///20);  //снег см примерно в мм дождя


      time = data.sunrise + TIMEZONE * 3600L;
      t_sun_v = do_sun_format(hour(time), minute(time));
      t_sun_v_hh = hour(time);
#ifdef pin_led
      t_sun_v_min = hour(time) * 60 + minute(time);
#endif


      time = data.sunset + TIMEZONE * 3600L;;
      t_sun_z = do_sun_format(hour(time), minute(time));
      t_sun_z_hh = hour(time);
#if defined (pin_led)||defined(pin_sunset_out_hight)||defined(pin_sunset_out_low)
      t_sun_z_min =  hour(time) * 60 + minute(time);
#ifdef debug
      Serial.print("t_sun_v_min=" );
      Serial.println(t_sun_v_min);
      Serial.print("t_sun_z_min=");
      Serial.println(t_sun_z_min);
#endif
#endif

      descript_now = data.description.c_str();


      log_str = "PARSE now P=" + String(davlenie0) + ";T=" + String(data.temp) + ";icon=" + data.icon + ";mm=" + String(rain_ch[0]);
      do_log();



#ifdef debug
      Serial.print(F("t_sun_v="));
      Serial.print(t_sun_v);
      Serial.print(F(" t_sun_z="));
      Serial.println(t_sun_z);
      Serial.print(F("description="));
      Serial.println( data.description.c_str());
      Serial.print(F("icon="));
      Serial.println( data.icon.c_str());
      Serial.print(F("temp="));
      Serial.println(  data.temp);
      Serial.print(F("pressure="));
#ifndef hPa
      Serial.println( data.pressure * 0.75);
#else
      Serial.println( data.pressure);
#endif

      //Serial.println( data.pressure);
      Serial.print(F("humidity="));
      Serial.println( data.humidity);
      Serial.print(F("windSpeed="));
      Serial.println(  data.windSpeed);
      Serial.print(F("windDeg="));
      Serial.println(data.windDeg);
      Serial.print(F("rain="));
      Serial.println(data.rain);
      Serial.print(F("snow="));
      Serial.println(data.snow);
      time = data.sunrise + TIMEZONE * 3600L;;
      Serial.print(F("sunrise="));
      Serial.println(ctime(&time));
      time = data.sunset + TIMEZONE * 3600L;;
      Serial.print(F("sunset="));
      Serial.println(  ctime(&time));
      Serial.print(F("cityName="));
      Serial.println( data.cityName.c_str());
      Serial.print(F("cod="));
      Serial.println( data.cod);
      Serial.println(F("---------------------------------------------------/\n"));
      Serial.print(F("do_parse_END mem="));
      Serial.println( ESP.getFreeHeap());
#endif
      tm_wh_now = millis();
    } //cod==200


  postingInterval_now = find ? t_period_now * 1000L : 180L * 1000L;
  lastConnectionTime_now = millis();





  return find;
}//do_parse_ch




//-----------------------------------------------------------------
//                          do_init_now()
//-------------------------------------------------------------
void do_init_now()
{

  icon_ch[0] = ""; //1 - min -700
  temperatura_ch[0] = 200; // 1 -min +100
  day_ch[0] = 200; //
  v_ch[0] = 0; //veter
  v_gr_ch[0] = 999; //напр.
  rain_ch[0] = 0; //rain
  descript_now = "";

}


#endif
