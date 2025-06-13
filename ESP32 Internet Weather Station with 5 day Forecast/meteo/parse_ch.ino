#ifdef openw
bool do_parse_ch() {
  bool find = false;
  static byte max_ch3_tmp = max_ch3;
  static bool sync_w = false;
  static byte count_err = 0;

  url_ch = url_ch_b + lat + "&lon=" + lon + "&appid=" + appid + "&cnt=" + String(max_ch3_tmp);


  last_7day = 0;
  int temp;
  bool temp_ch_n_tmp = false;

  float pr = 200;
  uint32_t t;
  String ic = "";
  float r_mm = 0, r_mm_day = 0; //за сутки осадки
  byte days = 0; //для 4444
  bool new_day_begin = false;
  //первые 3 прогноз 3часовой
#ifdef debug
  Serial.print(F("do_parse_ch() begin mem="));
  Serial.println( ESP.getFreeHeap());
  time_t tm = now();
  Serial.println(url_ch);
  Serial.print(F("start ch3   "));
  Serial.print(hour(tm));
  Serial.print(F("-"));
  Serial.print(minute(tm));
  Serial.print(F("-"));
  Serial.println(second(tm));
  Serial.print(F("delta_tz= "));
  Serial.println(delta_tz);

#endif


  OWM_fiveForecast *ow_fcast5 = new OWM_fiveForecast[max_ch3_tmp];
  entries = owF5.updateForecast(ow_fcast5, max_ch3_tmp, url_ch);
  if (entries > 0) {


#ifdef debug
    log_str = "PARSE count_ch=" + String(entries);
    do_log();
    Serial.println(F("successfully  url_ch"));
    Serial.print(F("max_ch3_tmp="));
    Serial.println(max_ch3_tmp);
    Serial.print(F("entries get "));
    Serial.println(entries);
    Serial.print(F("delta TZ="));
    Serial.println(delta_tz);
#endif



    do_init_ch();



    //--------------------------------------------------------------
    for (byte ch = 0; ch <= entries; ++ch) {


      //время
      t = (ow_fcast5[ch].dt).toInt() + TIMEZONE * 3600L;
      r_mm = 0;
      if (ch == 0) t_ch_b = t; //для графиков цветом дни недели


      //tttttttttttttttttttttttttttttttttt
      temp = ow_fcast5[ch].temp;

      //pppppppppppppppppp pppppppppppppppppppppppppppp
      if (ow_fcast5[ch].pressure > 0)
      {
#ifndef hPa
        pr = int(ow_fcast5[ch].pressure * 0.75) + delta_p;
#else
        pr = ow_fcast5[ch].pressure + delta_p;
#endif

        //P max min for 40ch
        if (p7_max < pr) p7_max = pr;
        if (p7_min > pr) p7_min = pr;
      }
      //////icon
      ic = ow_fcast5[ch].icon;

      /////////rainrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
      if ((ow_fcast5[ch].rain) > 0.1) //отсеиваю мелочь
      {
        r_mm = ow_fcast5[ch].rain;
#ifdef debug
        Serial.print(F("rain="));
        Serial.println(ow_fcast5[ch].rain);
#endif
      }
      if ((ow_fcast5[ch].snow) > 0.1 && (ow_fcast5[ch].snow) > r_mm)
      {
        r_mm = ow_fcast5[ch].snow; ///20;
#ifdef debug
        Serial.print(F("snow="));
        Serial.println(ow_fcast5[ch].snow);
#endif
      }

      //rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
      if (hour(t) + delta_tz == 0) //begin NEW DAY (rain)
      {
        if (days > 0) //уже было начало суток
        { //итог по дождю за сутки
          rain7[days] = r_mm_day;
#ifdef debug
          Serial.print(F("days="));
          Serial.print(days);
          Serial.print(F(" summ rain7="));
          Serial.println(rain7[days]);
#endif
        }


        r_mm_day = 0;
        new_day_begin = true; //метка для начала отсчета дней

#ifdef debug
        Serial.println(F("----NEW DAY----"));
#endif

      }
      r_mm_day = r_mm_day + r_mm;


      //текущая ночь --------------------temp_ch_n---------------------
      if ((hour(t) + delta_tz == 0 || hour(t) + delta_tz == 3 || hour(t) + delta_tz == 6) && temp_ch_n_tmp == false) //temp_ch_n_tmp метка первой ночи для ночной погоды на сегодня
      {

        if (hour(t) + delta_tz == 6) temp_ch_n_tmp = true; //метка при первом проходе текущей ночи, чтобы не показ след ночь.
        if (temp_ch_n > temp)
          temp_ch_n = temp; //ночная темп
#ifdef debug
        Serial.print(F("now day=0; temp_ch_n="));
        Serial.println(temp);
#endif

      }
#ifdef debug
      Serial.println(F("---------------------"));
      Serial.print(F("ch="));
      Serial.println(ch);
      Serial.print(F("days-->"));
      Serial.println(days);
      Serial.print(F("dd:hh="));
      Serial.print(day(t));
      Serial.print(F(":"));
      Serial.println(hour(t));
      Serial.print(F("h+delta_tz="));
      Serial.println(hour(t) + delta_tz);
      Serial.print(F("temp:"));
      Serial.print(ow_fcast5[ch].temp);
      Serial.print(F(" Pr="));
      Serial.print(pr);
#ifdef Sg
      Serial.print(F(", H="));
      Serial.print(ow_fcast5[ch].h);
#endif
      Serial.print(", r_mm_rain/r_mm_snow: " + String(r_mm) + " ic=" + ow_fcast5[ch].icon);
      Serial.print(F(" r_mm_day="));
      Serial.println(r_mm_day);
      Serial.println(F("---------------------"));
#endif


      ////////////cccccccccccccccccccccccccccccchhhhhhhhhhhhhhhhhhhhhhhhhhh
      if (ch < ch_count - 1) //нулевой оставил под текущ погоду, заполняю данными
      {
#ifdef debug
        Serial.print(F("ch_count="));
        Serial.println(ch + 1);
#endif

        icon_ch[ch + 1] = "/" + ic;
        day_ch[ch + 1] = hour(t);
        temperatura_ch[ch + 1] = temp;
        v_gr_ch[ch + 1] = ow_fcast5[ch].w_deg;
        v_ch[ch + 1] = ow_fcast5[ch].w_speed;
        rain_ch[ch + 1] = r_mm;

#if defined (Sg)||(defined (S4) && defined(tft_480_320))||defined (epd)
        p_ch[ch + 1] = pr; //давление
        h_ch[ch + 1] = ow_fcast5[ch].h; //влажность
#endif


      }


      //44444444444444444444444444444444444444444444444444

      //----------------temperatura7n[days]---------------
      //ночные темп
      if (days > 0) //было уже начало дня в 12-00
        if (hour(t) + delta_tz == 21 || hour(t) + delta_tz == 0 || hour(t) + delta_tz == 3 || hour(t) + delta_tz == 6)
        {

          if (temperatura7n[days] > temp)
          {
            temperatura7n[days] = temp;
          }

#ifdef debug
          Serial.println(F("nnnnnnnnnnnnnnnnnnnnnnnnnnn"));
          Serial.print("hour=");
          Serial.println(hour(t));
          Serial.print("hour+delta=");
          Serial.println(hour(t) + delta_tz);
          Serial.print(F("temperatura7n[days]="));
          Serial.println(temperatura7n[days]);
          Serial.println(F("----------------------------"));
#endif

        }



      if (new_day_begin == false) continue; //если не было 00:00 первого дня, отсчет дней не начинаю

      //----------------------------------------------------------------------

      if (hour(t) + delta_tz == 15 || hour(t) + delta_tz == 18) //если в 15-00 Темп выше чем в 12-00
      {
        if (temperatura7[days] < temp)
          temperatura7[days] = temp;
#ifdef debug
        Serial.print(F("temperatura7n[days]15-18="));
        Serial.println(temperatura7n[days]);
#endif

      }


      if (hour(t) + delta_tz != 12) continue;

      //--------------------------
      //погода на 12ч  для экрана
      days++;
      last_7day = days;


      //////icon
      icon7[days] = "/" + ic;
      //Serial.println(icon7[days]);

      //дата
      day7[days] = day(t);
      if (weekday(t) == 1 || weekday(t) == 7)
        holiday7[days] = 1; //воскр=1;

      if (weekday(t) == 2)
        holiday7[days] = 2; //понедель;
      if (weekday(t) == 6)
        holiday7[days] = 6; //пятн;

      weekday7[days] = weekday(t);

      //////////////temp темп
      temperatura7[days] = temp;
      //для постр гр
      if (temperatura7[days] < temperatura7[0])  temperatura7[0] = temperatura7[days] ;

      ///////////pressure давл
      davlenie7[days] = pr;

      //для постр гр
      if (davlenie7[days] < davlenie7[0] || davlenie7[0] == 200)  davlenie7[0] = davlenie7[days] ;


      ///////////ветер
      veter7[days] = ow_fcast5[ch].w_speed;
      //для постр гр
      if (veter7[days] < veter7[0])  veter7[0] = veter7[days] ;

      //grad
      v_gr_7[days] = ow_fcast5[ch].w_deg;



#ifdef debug
      Serial.print(F("----------12-00---------------hour="));
      Serial.println(hour(t));
      Serial.print(F("days="));
      Serial.println(days);
      Serial.print(F("icon7="));
      Serial.println(icon7[days]);
      Serial.print(F("temperatura7="));
      Serial.println(temperatura7[days]);

      Serial.print(F("davlenie7="));
      Serial.println(davlenie7[days]);


      Serial.print(F("veter7="));
      Serial.println(veter7[days]);

      Serial.print(F("v_gr_7="));
      Serial.println(v_gr_7[days]);
#endif


    }//for
    //-----------


    max_ch3_tmp = max_ch3;
    if (entries == max_ch3 - 1)
      find = true;
    tm_wh_ch = millis();
#ifdef  debug
    if (find)   Serial.println(F("openW read all entries"));
    else
    {
      Serial.println(F("openW read  NOT ALL"));
      Serial.print(F("entries<>max_ch3 "));
      Serial.print(entries);   Serial.print("<>");   Serial.println(max_ch3);
    }
#endif

    count_err = 0;
    sync_w = true;

  }
  else //запрос не выполнен-------------------------------------------------
  {
    Serial.println(F("failed, error: url_ch"));
    Serial.println(url_ch);
    //уменьшаю кол-во записей в запросе
    if (sync_w == true)
    {
      //если уже были данные не тороплюсь, делаю неск попыток полного запроса
      if (count_err > 5 && max_ch3_tmp > 10) max_ch3_tmp = max_ch3_tmp - 5;
    }
    else
    {
      if  (count_err > 2 && max_ch3_tmp > 10) max_ch3_tmp = max_ch3_tmp - 5;
    }
    count_err++  ;


    log_str = "PARSE ERR count_err=" + String(count_err) + " max_ch3_tmp=" + String(max_ch3_tmp);
    do_log();





#ifdef  debug
    Serial.print(F("count_err===="));
    Serial.println(count_err);

    Serial.print(F("max_ch3=max_ch3-5===="));
    Serial.println(max_ch3_tmp);
#endif

  }


  if (nigth == false)
    postingInterval_ch = find ? t_period_ch * 1000L : 120L * 1000L;
  else
    postingInterval_ch = find ? t_period_ch * 2 * 1000L : 180L * 1000L * 2; //ночью реже

  lastConnectionTime_ch = millis();

  delete[] ow_fcast5;
#ifdef debug
  tm = now();
  Serial.print(F("do_parse_ch() end mem========="));
  Serial.println( ESP.getFreeHeap());
  Serial.print(F("end ch3  "));
  Serial.print(hour(tm));
  Serial.print(F("-"));
  Serial.print(minute(tm));
  Serial.print(F("-"));
  Serial.println(second(tm));
  Serial.print(F("next get openW sec")); Serial.println(postingInterval_ch / 1000);

#endif





  return find;

}











//=========================================================================================
//                                do_parse_uv
//=========================================================================================
void do_parse_uv()
{

  WiFiClient client;
  bool find = false;
  char c;
  unsigned long lostTest = 60000UL;
  bool isBody = false;
  unsigned long lost_do = millis();


  String s = "";

#ifdef debug
  Serial.print(F("UV="));
  Serial.println(url_uv);
#endif
  if (client.connect("api.openweathermap.org", 80))
  {
    client.print("GET " + url_uv + " HTTP/1.1\r\n"
                 "Host:api.openweathermap.org\r\n"
                 "Connection: close\r\n\r\n");


#ifdef debug
    Serial.println(F("UV connect"));
#endif
    while (client.connected() || client.available()) {
      if (client.available()) {
        if ((millis() - lost_do) > lostTest) {
          Serial.println ("lost in client with a timeout");
          break;
        }
        c = client.read();
        //-------------
        //Serial.print (c);
        //-------------
        if (c == '}' && isBody) break; //END
        if (c == '{') {
          isBody = true;
        }
        if (isBody) {
          s += c;
        }


      }
      // give WiFi and TCP/IP libraries a chance to handle pending events
      yield();
    }

#ifdef debug
//    Serial.print (F("UF indexOf val: "));  Serial.println (s);
#endif
    //поиск
    int f1, f2;
    if (s.indexOf("\"value\":", 7) > 0)
    {
      f1 = s.lastIndexOf(":");
      f2 = s.lastIndexOf("}");
      String val = s.substring(f1 + 1, f2);
      uv = val.toFloat();
      if (uv >= 0)find = true;
#ifdef debug
      Serial.print (F("uv_parse="));  Serial.println (s);
      Serial.print (F("uv="));     Serial.println (val);
#endif
    }
    else
    {
      Serial.print (F("ERR uv val not found"));
    }
    client.stop();

  } //conn
  else
  {
    Serial.print(F("UV error connect"));
  }


  postingInterval_uv = find ? 1800L * 1000L : 240L * 1000L; // UV
  lastConnectionTime_uv = millis();

}




//-----------------------------------------------------------------
//                          do_init_ch()
//-------------------------------------------------------------
void do_init_ch()
{
  p7_max = 200, p7_min = 2000;

  for (byte i = 1; i < ch_count; i++) { //в первой будет текущая погода
    icon_ch[i] = ""; //1 - min -700
    temperatura_ch[i] = 200; // 1 -min +100
    day_ch[i] = 200; //
    v_ch[i] = 0; //veter
    v_gr_ch[i] = 999; //напр.
    rain_ch[i] = 0; //rain

#if defined (Sg)||(defined (S4) && defined(tft_480_320))||defined (epd)
    p_ch[i] = 0; //давление
    h_ch[i] = 0; //влажность
#endif



  }

  temp_ch_n = 200;
  //777777777777777777777777777777777777777777777777
  for (byte i = 0; i < 8; i++) {
    icon7[i] = ""; //1 - min -700
    davlenie7[i] = 200;
    temperatura7[i] = 200;
    temperatura7n[i] = 200;
    day7[i] = 200; //
    holiday7[i] = 200;
    rain7[i] = 0;
    veter7[i] = 0;
    v_gr_7[i] = 999;
    weekday7[i] = 200;
  }


}



void do_delta_tz()
{
  if (TIMEZONE > 0)
  {
    delta_tz = TIMEZONE % 3;
    if (delta_tz == 1) delta_tz = -1;
    if (delta_tz == 2) delta_tz = -2;
  }
  if (TIMEZONE < 0)
  {
    delta_tz = (int)(TIMEZONE % 3);
    if (delta_tz == 1) delta_tz = -2;
    if (delta_tz == 2) delta_tz = -1;
  }
#ifdef debug
  Serial.print(F("delta_tz= "));
  Serial.println(delta_tz);
#endif
}


#endif
