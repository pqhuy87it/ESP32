
bool do_ntp()
{


  static uint32_t t_ntp_sync = 0;

  int t_repeat;
  static time_t err_ntp_t = 0;
  if (sync_t == true && sync_force == false) t_repeat = t_ntp_rep_err; //уже было время синх.
  else
    t_repeat = 5000;

  //повтор запроса через 5с при ошибке если если не было синхр.
  if (millis() < err_ntp_t) return false;
  err_ntp_t = millis() + t_repeat; //повтор



  if (millis() < t_ntp_sync && err_ntp == 0 && WiFi.status() == WL_CONNECTED && sync_force == false) return true;



  err_ntp = 1;
  while (udp.parsePacket() > 0) ; // discard any previously received packets

  if (ntp_num == 5)ntp_num = 1;
  if (ntp_num == 1)
    WiFi.hostByName(ntpServerName1, timeServerIP);
  if (ntp_num == 2)
    WiFi.hostByName(ntpServerName2, timeServerIP);
  if (ntp_num == 3)
    WiFi.hostByName(ntpServerName3, timeServerIP);
  if (ntp_num == 4)
    WiFi.hostByName(ntpServerName4, timeServerIP);
  Serial.print("ntp="); Serial.println(ntp_num);

  //------------------------------------------
  sendNTPpacket(timeServerIP);
  // Wait till data is there or timeout...
  byte timeout = 0;
  int  size  = 0;
  do {
    delay ( 10 );
    size = udp.parsePacket();
    if (timeout > 100) break; // timeout after 1000 ms
    timeout++;
  } while (size == 0);



  // TRUE ntp-----------------------------
  if (size >= NTP_PACKET_SIZE) {

    t_ntp_update = millis() - (10 * (timeout + 1)); // Account for delay in reading the time
#ifdef debug
    Serial.print("t_ntp_update="); Serial.println(t_ntp_update);
#endif
    udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
    unsigned long secsSince1900;
    // convert four bytes starting at location 40 to a long integer
    secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
    secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
    secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
    secsSince1900 |= (unsigned long)packetBuffer[43];



#ifndef timechange_summer_winter
    if (TIMEZONE != 99) //on config.txt
    {
      ntp_time =  secsSince1900 - 2208988800UL + TIMEZONE * SECS_PER_HOUR;
      setTime(ntp_time);

#ifdef debug
      Serial.print("ntp_time TZ config.txt="); Serial.println(ntp_time);
#endif

    }
    else //not TZ in config.txt
    {
      Serial.print("ntp_time ERROR TIMEZONE not in  config.txt");

    }

#endif


#ifdef timechange_summer_winter
    if (TIMEZONE == 99) //not config.txt
    {
      TimeChangeRule *tcr;    // pointer to the time change rule, use to get TZ abbrev
      ntp_time =  secsSince1900 - 2208988800UL;
      time_t local = TZ.toLocal(ntp_time, &tcr);
      setTime(local);
      int32_t z = (local - ntp_time);
      z = z / 3600;
      TIMEZONE = z;
#ifdef debug
      Serial.print(F("ntp_time_S_W_=")); Serial.println(ntp_time);
      Serial.print(F("local_S_W_=")); Serial.println(local);
      //Serial.print("Z="); Serial.println(z);
      //     Serial.print("(local-ntp_time)/3600="); Serial.println(TIMEZONE);

#endif
    }
    else //TZ in config.txt
    {

      ntp_time =  secsSince1900 - 2208988800UL + TIMEZONE * SECS_PER_HOUR;
      setTime(ntp_time);
#ifdef debug
      Serial.print(F("ntp_time_S_W_TZ_from_config.txt=")); Serial.println(ntp_time);
#endif
    }
#endif //sum win


#if !defined(no_tft)&& !defined(epd)

    if (!sync_t) do_fread_timer();//если был сброс таймера до перезагр проверка, один раз
#endif //#ifndef epd

    tm_ntp = millis();
    sync_t = true;
    do_readnow();

    sync_force = false;
    ntp_count = 0;
    err_ntp = 0;
    t_ntp_sync = millis() + t_period_ntp;

#ifndef openw //заход - восх расчитываю если не с openw
    do_sun();
#endif
    refresh_all = true;


#ifdef debug
    log_str = "NTP OK ntp_num=" + String(ntp_num);
    do_log();
    Serial.println(F("NTP OK"));
#endif
    return true;
    //-----OK----------------OK----------------OK---------------------------------------


  }



  //false
  //-----------------------------------------------
  ntp_count++;
  Serial.println("No packet yet ");
#ifdef debug
  Serial.print(F("ERR ntp_count/num="));
  Serial.print(ntp_count);
  Serial.print("/");
  Serial.println(ntp_num);
#endif


  log_str = "NTP ERR ntp_count=" + String(ntp_count) + " ntp_num=" + String(ntp_num);
  if (WiFi.status() != WL_CONNECTED)
    log_str = log_str + " WIFI NOT CONNECTED";
  do_log();




  if (ntp_count > ntp_count_rep)
  {


    log_str = (F("NTP ERR  WIFI reconn"));
    do_log();


    Serial.println(F("-------WIFIntp reconnect"));;
    wifiConnect(false);
    ntp_count = 0;
    if (WiFi.status() == WL_CONNECTED)
    {
      err_ntp = 0;
      return true;
    }
  }





  ntp_num++; //следующ. ntp

  err_ntp = 1;





  return false;

}





//====================================================
//                  sendNTPpacket
//====================================================
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}




//====================================================
//                  now_correct
//====================================================
#ifdef  time_correct

time_t now_correct()
{

  if (!sync_t) return 0;

  time_t tm = ntp_time + (millis() - t_ntp_update) / 1000;
  time_t delta = 0;
  if (t > tm) delta = t - tm;
  if (tm > t)  delta = tm - t;
  //ERR ttttttttttttttttttttttttttt
  if (delta < 200) return t;
#ifdef debug
  Serial.print("delta=");
  Serial.println(delta);
  Serial.print("t now=");
  Serial.println(t);
  Serial.print("NTP now=");
  Serial.println(tm);
#endif
  setTime(tm);
  t = now();
  sync_force = true;
#ifdef debug

  Serial.print("ntp now=");
  Serial.println(tm);
  Serial.print("hour=");
  Serial.println(hour(tm));
  Serial.print("min=");
  Serial.println(minute(tm));
  Serial.print("day=");
  Serial.println(day(tm));
  Serial.print("month=");
  Serial.println(month(tm));
  Serial.print("year=");
  Serial.println(year(tm));




  log_str = "ERR do now_correct delta=" + delta;
  do_log();




#endif
  return tm ;
}
#endif //time_correct



//---------------------------------------------------------------------------
//                                          do_readnow
//---------------------------------------------------------------------------
void do_readnow()
{
  static uint32_t last = 0;
  t = now();
  if (millis() < last) return;


  if (now_min == minute(t))
  {
    last = millis() + 3000;
    return;
  }
  else
    now_min = minute(t);

  if (last == 0 || now_min == 0)
  {
    now_hour = hour(t);
    now_day = day(t);
    now_month = month(t);
    // now_weekday = weekday(t);
    if (weekday(t) == 1) now_weekday = 7; //RU 1-7
    else now_weekday = weekday(t) - 1;

    now_year =  year(t);
    if (now_hour == 0 && now_min == 0 && !play && !play_mp3 && !play_fm)
      refresh_all = true;

#ifdef debug
    Serial.print("now_hour "); Serial.println(now_hour);
    Serial.print("now_min "); Serial.println(now_min);
    Serial.print("day "); Serial.println(now_day);
    Serial.print("month "); Serial.println(now_month);
    Serial.print("now_weekday "); Serial.println(now_weekday);
    Serial.print("now_weekday name ");
    if (now_weekday == 7) Serial.println(dayNames[0]);
    else
      Serial.println(dayNames[now_weekday]);
#endif
  }


  //now_time
  if (now_hour < 10)
    now_time = "0" + String(now_hour);
  else
    now_time = String(now_hour);
  now_time = now_time + ":";
  if (now_min < 10)
    now_time = now_time + "0" + String(now_min);
  else
    now_time = now_time + String(now_min);



  //now_date
  if (now_day < 10)
    now_date = "0" + String(now_day);
  else
    now_date = String(now_day);
  now_date = now_date + ".";
  if (now_month < 10)
    now_date = now_date + "0" + String(now_month);
  else
    now_date = now_date + String(now_month);



  //    Serial.print("now_time "); Serial.println(now_time);
  //    Serial.print("now_date "); Serial.println(now_date);



}
