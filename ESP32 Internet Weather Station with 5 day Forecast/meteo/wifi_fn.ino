#ifdef wf_send
//============================================================
//                           do_wf_send()
//============================================================

void do_wf_send()
{
  static uint32_t nextTime = 0;
  static byte udp_err = 0;

  if (millis() < nextTime) return;

  String str;
  char ch[10];

  for (int i = 1; i <= 4; i++)
  { str = "";
    //tttttttttttttttttttttttttttttttttttttttttttttttttttt
#ifdef wf_send_Tu
    if (i == 1)
    {

      if (temp_u == 200)
        continue;
      else
        str = k_temp_u + String(temp_u) + ";";

#ifdef  debug
      Serial.print("wf_send_Tu=");
      Serial.println(str);
#endif

    }
#endif

    //tttttttttttttttttttttkkkkkkkkkkkkkkkkkkkkkkkkkkk
#ifdef wf_send_Tk
    if (i == 2)
    {

      if (temp_kv == 200)
        continue;
      else
        str = k_temp_kv + String(temp_kv + delta_t) + ";";
#ifdef debug
      Serial.print("wf_send_Tk=");
      Serial.println(str);
#endif

    }

#endif

    //hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh
#ifdef wf_send_Hh
    if (i == 3)
    {
      if (h_kv == 200)
        continue;
      else
        str = k_h_kv + String(h_kv) + ";";
#ifdef debug
      Serial.print("wf_send_Hh=");
      Serial.println(str);
#endif

    }
#endif

    ///cccccccccccommand свои команды
#ifdef wf_send_Cm
    if (i == 4)
    {
      //команду сразу отправляю
      if (cmd == "")
        continue;
      else
        str = k_cmd + String(cmd) + ";";
      cmd = "";
#ifdef debug
      Serial.print("wf_send_Cm=");
      Serial.println(str);
#endif

    }
#endif





    if (str != "")
    {
      str.toCharArray(ch, str.length() + 1);

      if (! sendPacket(broadcastAddress, (uint8_t*)&ch, sizeof(ch)))
      {
        Serial.println(F("STR Error sending broadcast UDP packet!"));
        Serial.print(F("UDP Send ERR"));

        log_str = ("UDP Send ERR");
        do_log();



      }

    }
    delay(250);
  }


  nextTime = millis() + wf_step;



}

//----------------------------------------
bool sendPacket(const IPAddress& address, const uint8_t* buf, uint8_t bufSize) {
  udp_wf.beginPacket(address, localPort_wf);
  udp_wf.write(buf, bufSize);
  return (udp_wf.endPacket() == 1);
}

#endif
//----------------------------------------------------




//=====================================================================
//                      do_wf_read
//=====================================================================

#ifdef wf_read
void do_wf_read()
{
  static uint32_t nextTime = 0;

  if (millis() < nextTime) return;


  if (!udp_wf.parsePacket())
    return;
  receivePacket();
  nextTime = millis() + 700;
}

//-----------------------------------------
/*
  k_temp_u="25Tu";
  k_temp_kv="25Tk";
  k_h_kv="25Hh";
  k_command="25Cm"; //своя команда
*/


//=======================================================================
//                receivePacket
//=======================================================================

void receivePacket() {
  char ch[10];

  static uint32_t nextTime_ulica = 0;


  udp_wf.read((uint8_t*)&ch, sizeof(ch));
  udp_wf.flush();

  String str = (const char*) ch;

  //if (strth.substring(0,6)=="25UTH;")
  if (str.substring(0, 2) != k_temp_u.substring(0, 2)) return; //kod 25

  //определяю параметр
  byte ind = str.indexOf(";");
  String cm = str.substring(0, ind + 1); //25..;
  if (cm.length() == 0) return;
  //Serial.println(cm);
  byte ind2 = str.indexOf(";", ind + 1); //конец данных
  String val = str.substring(ind + 1, ind2);
  //Serial.println(val);
  if (val.length() == 0) return;
  //Tu-----------ttttttttttttttttttttttttttttttttttttttttt
  int tt;
#ifdef wf_read_Tu
  if (cm == k_temp_u)
  {
    if (nextTime_ulica > millis()) return;

    tt = val.toInt();
    if (tt < 70 && tt > -50)
    {
      if (tt != temp_u) ref_temp_u = true;
      temp_u = tt;
      period_ulica = millis();
      nextTime_ulica = millis() + t_ds_ref * 1000; //не чаще 5мин читаю Tu
#ifdef debug
      Serial.print("wf_read_Tu==");
      Serial.println(temp_u);
#endif
    }
  }
#endif

  //Tk--------ttttttttttttttttttttkkkkkkkkkkkkkkkkkkkkk
#ifdef wf_read_Tk
  if (cm == k_temp_kv)
  {
    tt = val.toInt();
    if (tt < 70 && tt > -50)
    {
      t_read_tk = millis();
      if (tt != temp_kv)
      {
        temp_kv = tt;

#ifdef debug
        Serial.print("wf_read_Tk==");
        Serial.println(temp_kv);
#endif

        ref_kv_th = true;
      }
      else
      {
        //#ifdef debug
        //        Serial.println(F("wf_read_Tk not change"));
        //#endif

      }



    }
  }
#endif
  //hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh
#ifdef wf_read_Hh
  //Hk
  if (cm == k_h_kv)
  {
    tt = val.toInt();
    if (tt > 0 && tt < 101)
    {
      if (tt != h_kv)
      {
        h_kv = tt;
        ref_kv_th = true;
#ifdef debug
        Serial.print("wf_read_Hh==");
        Serial.println(h_kv);
#endif


      }


      else
      {
        //#ifdef debug
        //        Serial.println(F("wf_read_Hh not change"));
        //#endif


      }
    }
  }
#endif


  //ccccccccccccccccccccccccccccccccccc
#ifdef wf_read_Cm
  //Hk
  if (cm == k_cmd)
  {
#ifdef debug
    Serial.print("wf_read_Cm==");
    Serial.println(cm);
    Serial.print("val==");
    Serial.println(val);
#endif

    //----------------
#ifdef pin_beep
    if (val == "bp") //beep
      do_beep(fr, dur);

    if (val == "bp2") //2beep
    {
      do_beep(fr, 100);
      delay(200);
      do_beep(fr, 100);
    }
#endif

#ifdef pin_rele
    if (val == "rl")
    {
      rele_on_off = !rele_on_off;
      digitalWrite(pin_rele, rele_on_off);
      Serial.print("rele_on_off==");
      Serial.println(rele_on_off);
    }
#endif



  }


#endif



  //if (! sendPacket(udp.remoteIP(), (uint8_t*)&otvet, sizeof(otvet)))
  //Serial.println(F("Error sending answering UDP packet!"));
}

#endif
