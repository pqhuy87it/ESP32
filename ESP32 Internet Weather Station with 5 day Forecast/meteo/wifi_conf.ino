//==================================================================
//                        wifiConnect
//==================================================================

void wifiConnect(bool force)
{
  static bool conn = false; //при повторных на экран не показываю
  static byte wifi_count_recon = 0; //счетчик reconn;
  static uint32_t t_next = 0;


  //reset networking
  //if (WiFi.getPersistent() == true)
  WiFi.persistent(false);
  //WiFi.softAPdisconnect(true);
  WiFi.disconnect(true);


  log_str = "WIFI disconnect";
  if (force)
    log_str = log_str + " Force" ;
  do_log();


  //WiFi.persistent(true);   //enable saving wifi config into SDK flash area


  delay(1000);
  //check for stored credentials

  const char * _ssid = "", *_pass = "";


  String filename = "/wifi";
#ifdef pin_sd
  Serial.print("SD ");
  Serial.println(filename);
  File  configFile = SD.open( filename, FILE_READ);  // or, file handle reference for SD library
#else
  Serial.print("Spiffs ");
  Serial.println(filename);
  fs::File   configFile = SPIFFS.open( filename, "r");    // File handle reference for SPIFFS
#endif

  if (configFile) {
    String str = configFile.readString();
    Serial.println("read file STA");
    Serial.println(str);

    if (str.length() > 0)
    {
      byte ind = str.indexOf(";");
      String s = str.substring(0, ind);

      byte ind2 = str.indexOf(";", ind + 1);
      String p = str.substring(ind + 1, ind2);

      Serial.println(p);

      _ssid = s.c_str();
      Serial.println("ssid");
      Serial.println(_ssid);
      _pass = p.c_str();
      Serial.println("passw");
      Serial.println(_pass);
      configFile.close();
      //   if (!conn) tft.println();

      //        WiFi.mode(WIFI_STA);
      WiFi.begin(_ssid, _pass);
      unsigned long startTime = millis();
      //      tft.setCursor(0, y_onload - y_step);
#if !defined(no_tft)
#ifdef tft_320_240
      tft.setCursor(0, 239);
#endif

#ifdef tft_480_320
      tft.setCursor(0, 329);
#endif


#ifdef tft_400_300
      tft.setCursor(0, 299);
#endif
#endif //no tft
      while (WiFi.status() != WL_CONNECTED)
      {
        delay(500);
#if !defined(no_tft)
        if (!conn)
        {
          tft.print("=");
        }
#endif
        Serial.print(".");
        if ((unsigned long)(millis() - startTime) >= wifi_rep_t) break; //30sec
      }

      if (WiFi.status() == WL_CONNECTED)
      {
        Serial.print(F("IP address: "));
        Serial.println(WiFi.localIP());
        //  wifi_rssi();
        conn = true;
        wifi_count_recon = 0;

        log_str = "WIFI connect Ok";
        do_log();





      }

    }
  }

  else
  {
    Serial.println(F("file /wifi not found "));
#if !defined(no_tft)
    tft.setTextColor(TFT_RED);
    print_info("file /wifi not found ");
    tft.setTextColor(0x07E0);
#endif
  }
  //SOFT------AP
  //------------------------------------------------------------------------------------
  if (WiFi.status() != WL_CONNECTED)
  {

    if (conn) //уже было соед-нение
    {
      if (force)
      {
        if ( millis() < t_next) return;
        t_next = millis() + t_wifi_force_connect;
      }



      if (!force) wifi_count_recon++;

      if (wifi_count_recon >= wifi_count_rep )
      {
        Serial.print(F("ESP reboot wifi_count_recon>wifi_count_rep"));

        log_str = (F("ESP reboot wifi_count_recon>wifi_count_rep"));
        do_log();

        ESP.restart();


      }

      if (log_file && (!force))
      {
        log_str = ("wifi_count_recon=") + String(wifi_count_recon) ;
        do_log();
      }

      return;
    }

    Serial.print(F("ERR WIFI set WIFI_AP"));
    WiFi.persistent(false);
    //WiFi.softAPdisconnect(true);
    WiFi.disconnect(true);
    WiFi.mode(WIFI_AP);
    delay(1000);
    Serial.println(WiFi.softAPConfig(local_ip, gateway, netmask) ? "Ready" : "Failed!");
    Serial.println(WiFi.softAP(ssid_ap, password_ap) ? "Ready" : "Failed!");
    //WiFi.persistent(true);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);

#if !defined(no_tft)
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(0x07E0);
    tft.setCursor(0, 40);
    tft.setTextColor(0x07E0);
    tft.print(F("connect to AP <ESP_METEO> "));
    tft.println(F(" passw=11111111 "));
    tft.print(F("from the phone to come http://"));
    tft.setTextColor(TFT_YELLOW);
    tft.println(myIP);
    log_str = "WIFI ERR no connection";
    do_log();
#endif //no tft

  }
#ifdef debug
  Serial.println("");
  WiFi.printDiag(Serial);
  Serial.println("-------------");
  Serial.println(WiFi.getMode());
#endif
}







//=================================================================
//    html                          handleSubmit()
//=================================================================
void handleSubmit() {
  String response = "<p>The ssid is ";
  response += server.arg("ssid");
  response += "<br>";
  response += "And the password is ";
  response += server.arg("Password");
  response += "<br>";

  response += "</P><BR>";
  response += "<H2><a href=\"/\">go home</a></H2><br>";
  server.send(200, "text/html", response);


  String s = server.arg("ssid");
  ssid_new = s.c_str();
  String p = server.arg("Password");
  passw_new = p.c_str();

  String filename = "/wifi";

#ifdef pin_sd
  Serial.print("SD ");
  Serial.println(filename);
  File  configFile = SD.open( filename, FILE_WRITE); // or, file handle reference for SD library
#else
  Serial.print("Spiffs ");
  Serial.println(filename);
  fs::File   configFile = SPIFFS.open( filename, "w");    // File handle reference for SPIFFS
#endif

  if (!configFile) {
    Serial.println(filename);
    Serial.println(F(" no open,create"));
    return ;
  }
  //----------------------------





  String str = s + ";" + p + ";";
  Serial.println("STA=");
  Serial.println(str);
  configFile.println(str);
  configFile.flush();
  configFile.close();

  delay(1000);
  ESP.restart();

}
void handleNotFound()
{
  server.send(404, "text/plain", "404 Not Found");
}



void wifi_ap() {
  if (server.hasArg("ssid") && server.hasArg("Password") )
  {
    Serial.println("handleSubmit");
    handleSubmit();
  }
  else {
    Serial.println("else handleSubmit");
    String filename = "/wifi.html";
#ifdef pin_sd
    Serial.print("SD ");
    Serial.println(filename);
    File f = SD.open( filename, FILE_READ);  // or, file handle reference for SD library
#else
    Serial.print("Spiffs ");
    Serial.println(filename);
    fs::File f = SPIFFS.open( filename, "r");    // File handle reference for SPIFFS
#endif

    server.streamFile(f , "text/html");
    Serial.println(f.readString());
    f.close();


  }
  append_page_footer();
  //  server.send(200, "text/html", webpage);

}
//-------------------------------------------------------
//                    get_rssi
//-------------------------------------------------------
int wifi_rssi()
{
  int32_t db = WiFi.RSSI();


  if (db <= -100) {
    wifi_power = 0;
  } else if (db >= -50) {
    wifi_power = 100;
  } else {
    wifi_power = 2 * (db + 100);
  }

#ifdef debug
  Serial.print(F("wifi dBm = ")); Serial.print(db);
  Serial.print(F(" ")); Serial.print(wifi_power); Serial.println(F("%"));
#endif
  return wifi_power;
}
