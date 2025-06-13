
#if defined (nrf)||defined (R433)
//=======================================================
//                nrf24
//=======================================================
void do_read_ulica()
{
  static uint32_t t_next = 0;
  String id_name = "";
  int val[5] = {200}; //0-rezerv

#ifdef nrf
  byte pipeNo;
  if ( radio.available(&pipeNo)) // Non-blocking
  {

    char buff[32];
    uint8_t len = sizeof(buff);

     radio.read( &buff, sizeof(buff) );

#ifdef debug
    Serial.print(F("NRF str =")); Serial.println(buff);
    Serial.print(F("Pipe: "));    Serial.println(pipeNo);
#endif

    GParser data(buff, ';');
    byte count = data.split();
#ifdef debug
    Serial.print(F("nrf count val ")); Serial.println(count);
#endif

    for (byte i = 0; i < count; i++)
    {
      if (i == 0) {
        id_name = data[0];
        Serial.print(F("id_name : "));
        Serial.println( id_name);
      }
      else
      {
        val[i] = data.getInt(i);
        Serial.print("val"); Serial.print(i); Serial.print(":"); Serial.println(val[i]);
      }
    }
  }
  else return;
#endif //nrf
  //------------------------------------------433--------------------------------
#ifdef R433
  uint8_t buff[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buff);

  if (r433.recv(buff, &buflen)) // Non-blocking
  {
    String strth = (const char*) buff;
    Serial.println(strth);
#ifdef debug
    Serial.print(F("R433 str =")); Serial.println((char*)buff);
#endif

    GParser data((char*)buff, ';');
    byte count = data.split();
#ifdef debug
    Serial.print(F("r433 count val ")); Serial.println(count);
#endif

    for (byte i = 0; i < count; i++)
    {
      if (i == 0) {
        id_name = data[0];
        Serial.print(F("id_name : "));
        Serial.println( id_name);
      }
      else
      {
        val[i] = data.getInt(i);
        Serial.print("val"); Serial.print(i); Serial.print(":"); Serial.println(val[i]);
      }
    }
  }
  else return;
#endif //433 



  //room---------------------------room
  if ((id_name == "BME" || id_name == "AHT" || id_name == "SI" || id_name == "DHT" || id_name == "DS") && (val[4] == 1))
  {
    //Ttttttttttttttttttttttttttttttttttttttttttttttttt
#if defined (nrf_read_Tk) || defined (r433_read_Tk)
    if (val[1] > -50 &&  val[1] < 60)
    {
      if (temp_kv != val[1])
      {
        temp_kv = val[1];
        ref_kv_th = true;
      }
      t_read_tk = millis();
    }
#endif//nrf_read_Tk
    //Hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh
#if defined (nrf_read_Hh) || defined (r433_read_Hh)
    if  (( id_name != "DS") && (val[2]  > 0 && val[2] < 110))
    {
      if (h_kv != val[2])
      {
        h_kv = val[2];
        ref_kv_th = true;
      }
      t_read_tk = millis();
    }
#endif //nrf_read_Hh
  } //room

  //Pr BME BMP ppppppppppppppppppppppppppppppppppppppp
#if defined (nrf_read_Pr) || defined (r433_read_Pr)
  if (id_name == "BME" || id_name == "BMP") //pr in or out
  {

    if (val[3] > 300 && val[3] < 3000)
    {
      if (pr_kv != val[3])
      {
        pr_kv = val[3];
        ref_wh_now = true;
      }

    }
    else   pr_kv = 200;

  }
#endif //nrf_read_Pr
  //out sensor--------------------------OUT
  if ((id_name == "BME" || id_name == "AHT" || id_name == "SI" || id_name == "DHT" || id_name == "DS") && (val[4] == 0))
  {
    //Tttttttttttttttttttttttttttttttttttttttttttttttttt

#if defined (nrf_read_Tu) || defined (r433_read_Tu)
    if (val[1] > -50 && val[1] < 60)
    {
      if (temp_u != val[1])
      {
        temp_u = val[1];
        ref_temp_u = true;
      }
      period_ulica = millis();

    }
#endif // nrf_read_Tu

#if (defined (nrf_hum_out_TO_hum_in) && defined(nrf_read_Hh))\
    or (defined (r433_hum_out_TO_hum_in) && defined(r433_read_Hh)) //replace Hin<---Hout

    //Hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh
    if  (( id_name != "DS") && (val[2]  > 0 && val[2] < 110))
    {
      if ( h_kv != val[2])
      {
        h_kv = val[2];
        ref_kv_th = true;
      }
      t_read_tk = millis();
    }
#endif //Hg

  } //out


  //-------------------------------ALERT
  if (id_name == "ALR" && val[4] <= alert_max) //id<=max
  {

    alert = true; //flag ON
    alert_arr_id[val[4]] = val[1]; //count blink uint8_t
  }

  t_next = millis() + 500;

}






//================================================================
//                        do_alert
//================================================================
void do_alert() //id alert
{
  static uint32_t t_next = 0;
  bool alert_all = false;

  if (millis() < t_next) return;
  if (t % 2 == 0) //on
  {
    for (byte i = 0; i < alert_max; i++)
    {
      if (alert_arr_id[i] > 0)
      {
        draw_alert(i, true);
        alert_arr_id[i] -= 1;
        alert_all = true;
        //      Serial.print(F("i=" )); Serial.println(i);
        //      Serial.print(F("[i]= " )); Serial.println( alert_arr_id[i]);
      }
    }
  }
  else //off
  {
    for (byte i = 0; i < alert_max; i++)
    {
      if (alert_arr_id[i] > 0)
      {
        draw_alert(i, false);
        alert_all = true;
      }
    }


  }


  alert = alert_all;
  if (!alert)
  {
    t_next = 0;
    refresh_all = true;
    Serial.println(F("alert FALSE" ));
  }
  else
    t_next = millis()  + 1000;

}

//================================================================
//                        draw_alert
//================================================================
void  draw_alert(byte id, bool draw)
{
#if !defined(no_tft)
  uint16_t color = c_alr_id_def;
#if defined(tft_480_320)
  const  byte h_rect = 32;
  const  int w_rect = 479;
#endif
#if defined(tft_320_240)

  const  byte  h_rect = 24;
  const int w_rect = 319;
#endif


  if (draw)
  {
#ifdef c_alr_id0
    if (id == 0) color = c_alr_id0;
#endif
#ifdef c_alr_id1
    if (id == 1) color = c_alr_id1;
#endif
#ifdef c_alr_id2
    if (id == 2) color = c_alr_id2;
#endif
#ifdef c_alr_id3
    if (id == 3) color = c_alr_id3;
#endif
#ifdef c_alr_id4
    if (id == 4) color = c_alr_id4;
#endif
#ifdef c_alr_id5
    if (id == 5) color = c_alr_id5;
#endif
#ifdef c_alr_id6
    if (id == 6) color = c_alr_id6;
#endif
#ifdef c_alr_id7
    if (id == 7) color = c_alr_id7;
#endif
#ifdef c_alr_id8
    if (id == 8) color = c_alr_id8;
#endif
#ifdef c_alr_id9
    if (id == 9) color = c_alr_id9;
#endif
  }
  else
    color = TFT_BLACK;

  int  y = h_rect * id;
  tft.fillRect(0, y, w_rect, h_rect, color) ;
  if (draw)
  {
    tft.setTextColor(TFT_BLACK);
#ifdef tft_480_320
    tft.setFreeFont(v24);
#endif
#ifdef tft_320_240
    tft.setFreeFont(v18);
#endif

    tft.setCursor(20, y + h_rect);
    tft.print(id);
  }
#endif //no tft
}
#endif //nrf



//================================================================
//                        ds1820
//================================================================
#ifdef pin_ds1820
void do_read_ds1820()
{
  static uint32_t t_period_ds = 0;

  if (millis() < t_period_ds) return;
  t_period_ds = millis() + 10000; //on ERR



  ds.requestTemperatures();                               // считываем температуру с датчиков, на это требуется 750мс
  int tmp = int(round(ds.getTempCByIndex(0)));

  if (tmp < 60 && tmp > -60)
  {
#if ds1820_place //room
    if (temp_kv != tmp) ref_kv_th = true;
    t_read_tk = millis();
    temp_kv = tmp;
#else
    //street
    if (temp_u != tmp) ref_temp_u = true;
    temp_u = tmp;
    period_ulica = millis();
#endif


    t_period_ds = millis() + t_ds_ref * 1000; //5min

#ifdef debug
    log_str = "ds1820=" + String(tmp);
    do_log();
#endif

  }
  else
  {

    log_str = "ds1820 ERR temp_u=" + String(tmp);
    do_log();
    temp_u = 200;
  }

}
#endif



//================================================================
//                    bme280
//================================================================
#ifdef bme280
void do_read_bme()
{
  static time_t t_period_bme = 0;

  if (millis() < t_period_bme && temp_kv != 200) return;
  static time_t t_err = 0;
  if (millis() < t_err) return;
  t_err = millis() + t_bme_err * 1000;




  //float temp(NAN), hum(NAN), pres(NAN);
  float temp = 200, hum = 200, pres = 200;

  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
  BME280::PresUnit presUnit(BME280::PresUnit_hPa);
  sensor_bme.read(pres, temp, hum, tempUnit, presUnit);


#ifndef hPa
  pr_kv = round(pres * 0.75);
#else
  pr_kv = pres;
#endif




  if (pr_kv > 10000)
  {

    Serial.println("bme280 ERR P=" + String(pr_kv));

    pr_kv = 200; //err
  }



  temp_kv = round(temp) + delta_t;
  h_kv = round(hum);

#ifdef debug
  Serial.println(F("bme------------------"));
  Serial.println(pr_kv);
  Serial.println(temp_kv);
  Serial.println(h_kv);
#endif

  if (temp_kv < -50 || temp_kv > 70)
  {
#ifndef debug
    Serial.println("bme280 ERR temp_kv=" + String(temp_kv));
#else
    log_str = "bme280 ERR temp_kv=" + String(temp_kv);
    do_log();
#endif
    temp_kv = 200;



  }
#ifndef nrf_hum_out_TO_hum_in
  if (h_kv < 1 || h_kv > 100)
  {

#ifndef debug
    Serial.println("bme280 ERR h_kv=" + String(h_kv));
#else
    log_str = "bme280 ERR h_kv=" + String(h_kv);
    do_log();
#endif
    h_kv = 200;
  }
#endif


  if (temp_kv != 200)
  {
    ref_kv_th = true;
    t_read_tk = millis();
    t_period_bme = millis() + t_bme_ref * 1000; //5min
  }
}
#endif


//================================================================
//                    aht10
//================================================================
#ifdef aht10
void  do_read_aht()
{
  static time_t t_period_bme = 0;

  if (millis() < t_period_bme && temp_kv != 200) return;

  static time_t t_err = 0;
  if (millis() < t_err) return;
  t_err = millis() + t_bme_err * 1000;

  float temp = sensor_aht.readTemperature();
  float hum = sensor_aht.readHumidity();
  temp_kv = round(temp) + delta_t;
  h_kv = round(hum);

#ifdef debug
  Serial.println(F("aht------------------"));
  Serial.println(temp_kv);
  Serial.println(h_kv);
#endif debug


  if (temp_kv < -50 || temp_kv > 70)
  {

#ifndef debug
    Serial.println("AHT10 ERR temp_kv=" + String(temp_kv));
#else
    log_str = "AHT10 ERR temp_kv=" + String(temp_kv);
    do_log();
#endif

    temp_kv = 200;
  }
#ifndef nrf_hum_out_TO_hum_in
  if (h_kv < 1 || h_kv > 100)
  {


#ifndef debug
    Serial.println("AHT10 ERR H=" + String(h_kv));
#else
    log_str = "AHT10 ERR H=" + String(h_kv);
    do_log();
#endif
    h_kv = 200;
  }
#endif

  if (temp_kv != 200)
  {
    ref_kv_th = true;
    t_read_tk = millis();
    t_period_bme = millis() + t_bme_ref * 1000; //5min
  }


}
#endif



//================================================================
//                        si7021
//================================================================
#ifdef si7021
void do_read_si7021()
{
  static time_t t_period_bme = 0;

  if (millis() < t_period_bme && temp_kv != 200) return;

  static time_t t_err = 0;
  if (millis() < t_err) return;
  t_err = millis() + t_bme_err * 1000;


  float hum = sensor_si.getRH();
  //float temp = (sensor_si.getTempF()-32)/1.8;
  float temp = sensor_si.getTemp();
  temp_kv = round(temp) + delta_t;
  h_kv = round(hum);

#ifdef debug
  Serial.println(F("si7021------>"));
  Serial.println(temp_kv);
  Serial.println(h_kv);
#endif debug


  if (temp_kv < -50 || temp_kv > 70)
  {

#ifndef debug
    Serial.println("si7021 ERR temp_kv=" + String(temp_kv));
#else
    log_str = "si7021 ERR temp_kv=" + String(temp_kv);
    do_log();
#endif

    temp_kv = 200;
  }
#ifndef nrf_hum_out_TO_hum_in
  if (h_kv < 1 || h_kv > 100)
  {


#ifndef debug
    Serial.println("si7021 ERR H=" + String(h_kv));
#else
    log_str = "si7021 ERR H=" + String(h_kv);
    do_log();
#endif
    h_kv = 200;
  }
#endif

  if (temp_kv != 200)
  {
    ref_kv_th = true;
    t_read_tk = millis();
    t_period_bme = millis() + t_bme_ref * 1000; //5min
  }
  else
    sensor_si.reset();


}
#endif





//================================================================
//                        DHT
//================================================================
#ifdef pin_dht
void do_read_dht()
{
  static time_t t_period_bme = 0;

  if (millis() < t_period_bme && temp_kv != 200) return; //5min

  static time_t t_err = 0;
  if (millis() < t_err) return;
  t_err = millis() + t_bme_err * 1000;

  float hum = dht.readHumidity();
  float temp = dht.readTemperature();
  temp_kv = round(temp) + delta_t;
  h_kv = round(hum);



#ifdef debug
  Serial.println(F("si------------------"));
  Serial.println(temp_kv);
  Serial.println(h_kv);
#endif debug


  if (temp_kv < -50 || temp_kv > 70)
  {




#ifndef debug
    Serial.println( "DHT  ERR temp_kv=" + String(temp_kv));
#else
    log_str = "DHT  ERR temp_kv=" + String(temp_kv);
    do_log();
#endif




    temp_kv = 200;
  }
#ifndef nrf_hum_out_TO_hum_in
  if (h_kv < 1 || h_kv > 100)
  {

#ifndef debug
    Serial.println( "DHT ERR H=" + String(h_kv));
#else
    log_str = "DHT ERR H=" + String(h_kv);
    do_log();
#endif
    h_kv = 200;
  }
#endif

  if (temp_kv != 200)
  {
    ref_kv_th = true;
    t_read_tk = millis();
    t_period_bme = millis() + t_bme_ref * 1000; //5min
  }


}
#endif
