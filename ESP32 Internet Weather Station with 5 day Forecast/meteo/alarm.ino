#ifdef ESP32

//==========================================================================================================
//            start_alarm_www
//==========================================================================================================
void start_alarm_www()
{

  bool play_bg_tmp = false;
#ifdef debug
  Serial.println(F("------------------start_alarm_www"));
#endif
  if (t_timer > 0) // уже запущен ручной таймер
  {
#ifdef debug
    Serial.println(F("timer hand on, alarm exit"));
#endif
    return;
  }
  if (al.label == 'i' && (nigth == true || scr_name[scr_number] == "Sn")) return; //i -низкий приоритет для ночи и  экрана ноч.
  //start

  String cmd = "";
  if (play_fm)  cmd = "stop_fm";

  if (play)
  {
    play_bg_tmp = play_bg;
    cmd = "stop";
  }
  if (play_mp3)  cmd = "stop_mp3";

  if ( ch_prev_alarm_fm == 0 && ch_prev_alarm_www == 0 && ch_prev_alarm_mp3 == 0)
  {

#if defined (pin_sd)&&defined (ESP32)
    if (play_mp3 && ch_prev_alarm_mp3 == 0)
    {
      ch_prev_alarm_mp3 = curr_track;
#ifdef debug
      Serial.print(F("ch_prev_alarm_mp3=")); Serial.println(ch_prev_alarm_mp3);
#endif
    }
#endif //mp3

    if (play_fm && ch_prev_alarm_fm == 0)
    {
      ch_prev_alarm_fm = ch_fm;
#ifdef debug
      Serial.print(F("ch_prev_alarm_fm=")); Serial.println(ch_fm);
#endif
    }

    if (play && ch_prev_alarm_www == 0)
    {
      ch_prev_alarm_www = ch;
#ifdef debug
      Serial.print(F("ch_prev_alarm_www=")); Serial.println(ch);
#endif
    }

  }


  if (cmd != "") { //cmd stop
    fn_cmd = cmd;
    do_fn_btn_ir();
  }
  //---------------------------------------------

  pl_list_prev_alarm_www = pl_list;
  if (pl_list != 0)
  {
    pl_list = 0;
    do_pl_name(pl_list);
    refresh_all = true;
    tft.fillScreen(TFT_BLACK );
  }

  ch_fnd = al.ch_num;
  fn_cmd = "play";
  alarm_active = true;

  if (al.t_long > 0)
  {
    //  timer_id = 0;
    t_timer_alarm = millis() + al.t_long;

  }
  else
  { //просто запуск без таймера  и запоминания пред. канала
#ifdef debug
    Serial.println("alarm start t_timer_alarm = 0");
#endif
    t_timer_alarm = 0;
  }

#ifdef debug
  Serial.print(F("alarm min=")); Serial.println(al.m);
  Serial.print(F("al.ch_num=")); Serial.println(al.ch_num);
  Serial.print(F("al.t_long=")); Serial.println(al.t_long);
  Serial.println(F("------------------------alarm_www"));
#endif


  do_fn_btn_ir(); //play
  if (play_bg_tmp) //go BG play
  {
    fn_cmd = "play_bg";
    do_fn_btn_ir();
  }
  fn_cmd = "";

}


#endif //esp32








#if defined (fmSi4703)|| defined (fmRDA5807)|| defined (fmSi4703_lib2)
//==========================================================================================================
//              start_alarm_fm
//==========================================================================================================
void start_alarm_fm()
{


  if (t_timer > 0) // уже запущен ручной таймер
  {
#ifdef debug
    Serial.println(F("timer hand on, alarm exit"));
#endif
    return;
  }

  if (al.label == 'f' && (nigth == true || scr_name[scr_number] == "Sn")) return; //f -низкий приоритет для ночи и  экрана ноч.

#ifdef debug
  Serial.println(F("------------------start_alarm_FM"));
#endif

  //start
  String cmd = "";

  if (play_fm)  cmd = "stop_fm";
  if (play)  cmd = "stop";
  if (play_mp3)  cmd = "stop_mp3";

  if ( ch_prev_alarm_fm == 0 && ch_prev_alarm_www == 0 && ch_prev_alarm_mp3 == 0)
  {

#if defined (pin_sd)&&defined (ESP32)
    if (play_mp3 && ch_prev_alarm_mp3 == 0)
    {
      ch_prev_alarm_mp3 = curr_track;
#ifdef debug
      Serial.print(F("ch_prev_alarm_mp3=")); Serial.println(ch_prev_alarm_mp3);
#endif
    }
#endif //mp3


    if (play_fm && ch_prev_alarm_fm == 0)
    {
      ch_prev_alarm_fm = ch_fm;
#ifdef debug
      Serial.print(F("ch_prev_alarm_fm=")); Serial.println(ch_fm);
#endif
    }

    if (play && ch_prev_alarm_www == 0)
    {
      ch_prev_alarm_www = ch;
#ifdef debug
      Serial.print(F("ch_prev_alarm_www=")); Serial.println(ch);
#endif
    }

  }


  if (cmd != "") {
    fn_cmd = cmd;
    do_fn_btn_ir();
  }

  //---------------------------------------
  ch_fm = al.ch_num;
  fn_cmd = "play_fm";
  alarm_active = true;
  if (al.t_long > 0)
  {
    t_timer_alarm = millis() + al.t_long;
  }
  else
  { //просто запуск без таймера  и запоминания пред. канала
#ifdef debug
    Serial.println("alarm start t_timer_alarm = 0");
#endif
    t_timer_alarm = 0;
  }
#ifdef debug
  Serial.print(F("alarm start min=")); Serial.println(al.m);
  Serial.print(F("al.ch_fm=")); Serial.println(ch_fm);
  Serial.print(F("al.t_long=")); Serial.println(al.t_long);
  Serial.println(F("-----------------------alarm_FM"));
#endif


  do_fn_btn_ir();
  fn_cmd = "";

}


#endif //FM









//======================================================================================
//                   control_timer
//======================================================================================

void control_timer() //таймер на остановку на экране проигр.
{

  static uint32_t t_ref = 0;
  // static byte mnt_old = 99;
  static byte scr_n = 99;


  // only stop
  if (millis() >= t_timer) //STOP timer
  {
    // t_timer = 0;
    t_ref = 0;
    scr_n = 99;
    if (play)
    {

      fn_cmd = "stop";

    }


    else if (play_fm)
    {
      fn_cmd = "stop_fm";
    }
    else if (play_mp3)
    {

      fn_cmd = "stop_mp3";

    }

    timer_stop();
    do_fn_btn_ir();
    fn_cmd = "";


  }


}


#ifdef ESP32
//======================================================================================
//                   control_timer_alarm
//======================================================================================

void control_timer_alarm()
{

  // only stop
  if (millis() < t_timer_alarm) return;
  //STOP alarm timer
  bool play_bg_tmp = play_bg;

  fn_cmd = "stop";
#ifdef debug
  Serial.print(F("control_timer_alarm STOP(cmd)==")); Serial.println(fn_cmd);
  Serial.print(F("play_bg ")); Serial.println(play_bg);
#endif
  timer_alarm_stop();
  alarm_active = false;
  do_fn_btn_ir();
  fn_cmd = "";



  if (ch_prev_alarm_mp3 != 0) //если до таймера уже играло www возврат
  {
#ifdef debug
    Serial.print(F("ch_prev_alarm_mp3=")); Serial.println(ch_prev_alarm_mp3);
#endif
    ch_prev_alarm_mp3 = 0;
    fn_cmd = "play_mp3";

  }
  else if (ch_prev_alarm_www != 0) //если до таймера уже играло www возврат на канал
  {
    ch_fnd = ch_prev_alarm_www;
    if (pl_list != pl_list_prev_alarm_www)
    {
      pl_list = pl_list_prev_alarm_www;
      do_pl_name(pl_list);
      refresh_all = true;
    }
#ifdef debug
    Serial.print(F("ch=ch_prev_alarm_www=")); Serial.println(ch_prev_alarm_www);
    Serial.print(F("pl_list_prev_alarm_www=")); Serial.println(pl_list_prev_alarm_www);

#endif
    ch_prev_alarm_www = 0;
    pl_list_prev_alarm_www = 0;
    fn_cmd = "play";
  }
  else if (ch_prev_alarm_fm != 0) //если до таймера уже играло FM возврат на канал
  { ch_fm = ch_prev_alarm_fm;

#ifdef debug
    Serial.print(F("ch_fm=ch_prev_alarm_fm=")); Serial.println(ch_prev_alarm_fm);
#endif
    ch_prev_alarm_fm = 0;
    fn_cmd = "play_fm";
  }


  if (fn_cmd != "")
  {
    do_fn_btn_ir();
    if (play_bg_tmp) //было фоном до play
    {
      fn_cmd = "play_bg";
      do_fn_btn_ir();
    }

    fn_cmd = "";
  }

}


//=====================================================================
//                            timer_alarm_stop
//=====================================================================
void timer_alarm_stop()
{

#ifdef debug
  Serial.println(F("t_timer_alarm STOP"));
#endif
  t_timer_alarm = 0;
}


//=====================================================================
//                            timer_start
//=====================================================================
void timer_start(int minute_count, byte id)
{

  t_timer = millis() + 60000 * minute_count;
  timer_id = id;
  draw_num(minute_count, 'T');
  draw_num(0, 'C'); //clear num
  timer_alarm_stop();
#ifdef debug
  Serial.print(F("t_timer  start Id=")); Serial.println(id);
#endif
}

#endif //32

//=====================================================================
//                            timer_stop
//=====================================================================
void timer_stop()
{
  t_timer = 0;
  timer_id = 99;
  draw_num(0, 'T');
#ifdef debug
  Serial.println(F("t_timer  STOP"));
#endif
}







//#ifdef ESP32

//==========================================================================================================
//                                         do_read_file_shed
//==========================================================================================================
bool do_read_file_shed()
{
  bool parse_line = true;
  String filename = "/alarm.txt";
#ifdef pin_sd
#ifdef debug
  Serial.println("do_read_file_shed");
  Serial.print("SD ");
  Serial.println(filename);
#endif
  File  f = SD.open( filename, FILE_READ);  // or, file handle reference for SD library
#else
#ifdef debug
  Serial.print("Spiffs ");
  Serial.println(filename);
#endif
  fs::File   f = SPIFFS.open( filename, "r");    // File handle reference for SPIFFS
#endif

  if (!f) {
    Serial.println(filename);
    Serial.println(F(" no open,create"));
    return 0;
  }
  //----------------------------


  //al.active = false;
  al.label = 'x';
  al.h = 99; //hour
  al.m = 99; //min
  al.ch_num = 0; // ch
  //  al.fname = "";
  al.t_long = 0; //length T millisec
  // al.vol = 0; //volume
  t_next_shed = 99; //если пустой файл всегда t_next_shed>min
  next_shed_ch = 0;
  next_label = 'x';



  String P, str;
  byte i = 0;
  int tz2 = 0;

  int h = -1, m = -1;
  int t_long = -1;
  int ch_num = -1;
  //int count=-1,interval=-1,vol_up=-1;
  // int vol = -1;
  String label = "", fname = "";

  byte t_e_ch, t_b_ch;

  //t=now();
  //----------------------------------
  while (f.available()) {
    str = f.readStringUntil('\n');
    str.replace(" ", "");
    yield();


    int tz = -1;
    tz = str.indexOf("#");
    if (tz != 0)continue;

#ifdef debug
    Serial.println(str); //Printing for debugging purpose
#endif

    tz2 = 0;
    h = -1; m = -1;

    t_long = -1;
    ch_num = -1;
    //   vol = -1;
    label = "";
    fname = "";
    t_e_ch = 0; t_b_ch = 0;
    tz2 = tz + 2;
    //label
    P = str.substring(tz + 1, tz2);
    /*
      #ifdef debug
        Serial.print("P_Type=");
        Serial.println(P);
      #endif
    */
    if (P != "I" && P != "i" && P != "f" && P != "F" && P != "m" && P != "M") continue;

    label = P;
    tz = tz2;
    i = 1;
    //------------------------------------------------
    parse_line = true;
    while (str.indexOf(";", tz + 1) >= 0)
    {

      P = "";
      tz2 = str.indexOf(";", tz + 1);
      if (tz2 > 0)
      {

        P = str.substring(tz + 1, tz2);
        P.trim();
        if (P == "") P = "*";
        /*
          #ifdef debug
                Serial.print("P"); Serial.print(i); Serial.print("==");
                Serial.println(P);
          #endif
        */
        //hhhhhhhhhhhhhhhhhhhhhhhhhh
        if (i == 1)
        {
          if (P == "*") h = now_hour; //в текущий час
          else if (P.toInt() >= 0 && P.toInt() == now_hour) h = P.toInt();
          else
          {
            /*
              #ifdef debug
                        Serial.println(F("break  Hour"));
              #endif
            */
            parse_line = false;
            break;
          }
        }

        //MINUTE mmmmmmmmmmmmmmm
        if (i == 2)
        {
          if (P.toInt() >= 0 && P.toInt() >= now_min) m = P.toInt();
          else
          {
            /*
              #ifdef debug
                        Serial.println(F("break  Minute"));
              #endif
            */
            parse_line = false;
            break;
          }
        }




        //Day DDDDDDDDDDDDDDDDDDDDDDDDD
        if (i == 3)
        {
          if (P == "*") ; //today
          else if (P.toInt() > 0 && P.toInt() == day(t)) ; //today
          else
          {
            /*
              #ifdef debug
              Serial.println(F("break  Day"));
              #endif
            */
            break;
            parse_line = false;
          }
        }



        //Week WWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
        if (i == 4)
        {
          byte w;
          if (weekday(t) == 1)w = 7; //RU 1-7
          else w = weekday(t) - 1;
          if (P == "*") ; //today
          else if (P.toInt() > 0 && P.toInt() == w) ; //today
          else
          {
            /*
              #ifdef debug
              Serial.println(F("break  WEEK"));
              #endif
            */
            parse_line = false;
            break;
          }
        }




        //  CH-----CH--------CH-------file-  cccccccccccccccccccccccccccccc
        if (i == 5)
        {
          if (label == "I" || label == "i" || label == "f" || label == "F")
          {
            if (P.toInt() > 0) ch_num = P.toInt();
            else
            {
              /*
                #ifdef debug
                Serial.println("break  ch_num==0");
                #endif
              */
              parse_line = false;
              break;
            }
          }
          else
          {
            /*
              #ifdef debug
              Serial.println(F("break  ch/fname not found"));
              #endif
            */
            parse_line = false;
            break;
          }

        }



        //long llllllllllllllllllllllll   min
        if (i == 6)
        {
          if (P.toInt() > 0) t_long = P.toInt() * 60000;
          else
          {
            /*
              #ifdef debug
              Serial.println(F("t_long=0 only start"));
              #endif
            */
            //   break;
          }



        }



        //t_b ch--------------------------------------------
        if (i == 7)
        {
          if (P.toInt() >= 0 && P.toInt() <= 24)
          {
            t_b_ch = P.toInt();
            if (t_b_ch == 24) t_b_ch = 0;
            /*
              #ifdef debug
              Serial.print("t_b_ch ok="); Serial.println(t_b_ch);
              #endif
            */
          }
          else
          {
            /*
              #ifdef debug
                        Serial.println("break  t_b ch");
              #endif
            */
            parse_line = false;
            break;
          }
        }




        //t_e ch--------------------------------------------
        if (i == 8)
        {
          if (P.toInt() >= 0 && P.toInt() <= 24)
          {
            t_e_ch = P.toInt();
            if (t_e_ch == 0) t_e_ch = 24;
            /*
              #ifdef debug
                        Serial.print("t_e_ch ok="); Serial.println(t_e_ch);
              #endif
            */

          }
          else
          {
            t_b_ch = 0; //too esc
            t_e_ch = 0;
            /*
              #ifdef debug
                        Serial.println("break  t_e ch, t_b_ch undo");
              #endif
            */
            parse_line = false;
            break;
          }
        }




        i++;
        tz = tz2;
      }

    } //;while str ";"---------------------------



    //--------------------------------control t_b_ch t_e_ch--------------------------------
    if (t_e_ch > t_b_ch) // в один день
    {
      if (now_hour >= t_b_ch && now_hour < t_e_ch)
      {
        /*
          #ifdef debug
          Serial.println(F("t_e_ch > t_b_ch ----->OK"));
          #endif
        */
      }
      else
      {
        /*
          #ifdef debug
          Serial.println(F("begin-end break, now_hour >= t_b_ch && now_hour < t_e_ch"));
          #endif
        */
        parse_line = false; //

      }
    }

    //--------------  //начало и конец в разн день
    if (t_e_ch < t_b_ch )
    {
      /*
        #ifdef debug
        Serial.println(F("t_e_ch < t_b_ch"));
        #endif
      */
      if (now_hour < t_b_ch && now_hour >= t_e_ch)
      {
        parse_line = false; //
        /*
          #ifdef debug
                Serial.println(F("begin-end break, t_e_ch < t_b_ch"));
          #endif
        */
      }
    }



    /*
      #ifdef debug
        Serial.println(F("-------end line-----"));
      #endif

    */
    //--------------ALARM fill ------------------------------------
    if (parse_line && al.m > m)
    {

      if (al.m != 99 && (label == "I" || label == "i" || label == "f" || label == "F"))
      {
        t_next_shed = al.m; //
        next_shed_ch = al.ch_num;
        next_label = al.label;
      }

      al.label = label.charAt(0);
      al.h = h; //hour
      al.m = m; //min
      if (label == "I" || label == "i" || label == "f" || label == "F") al.ch_num = ch_num; // ch
      // if (label == "M" || label == "J" || label == "m" || label == "j" ) al.fname = fname; // ch


      if (t_long != -1) al.t_long = t_long; //length T millisec
      else al.t_long = 0;

      //   if (vol != -1) al.vol = vol; //volume
      // else al.vol = 0;
      /*
        #ifdef debug
            Serial.println(F("-----------------ALARM----------------"));
            do_print_alarm();
            Serial.println(F("--------END---------ALARM------"));
        #endif
      */
    }
    else if (parse_line)
    {
      if (t_next_shed > m && (label == "I" || label == "i" || label == "f" || label == "F"))
      {
        t_next_shed = m;
        next_shed_ch = ch_num;
        next_label = label.charAt(0);
      }

    }





  }//FILE

  if (al.h != 99)
#ifdef debug
    Serial.println(F("--------ACTUAL---------ALARM----------------"));
  do_print_alarm();
  Serial.println(F("--------END-----ACTUAL---------ALARM------------"));
#endif
  f.close();

  return parse_line;
}









//==========================================================================================================
//              do_print_alarm
//==========================================================================================================
#ifdef debug
void do_print_alarm()
{

  Serial.println(al.label);
  Serial.print(F("al.h="));
  Serial.println(al.h);
  Serial.print(F("al.m="));
  Serial.println(al.m);
  Serial.print(F("al.ch_num----------->"));
  Serial.println(al.ch_num);
  //Serial.print(F("al.fname="));
  // Serial.println(al.fname);
  Serial.print(F("al.t_long="));
  Serial.println(al.t_long);
  //Serial.print(F("al.count="));
  //Serial.println(al.count);
  //Serial.print(F("al.interval="));
  //Serial.println(al.interval);
  //Serial.print(F("al.vol="));
  //Serial.println(al.vol);
  //Serial.print(F("al.vol_up="));
  //Serial.println(al.vol_up);
  if (t_next_shed != 99)
  {
    Serial.print(F("t_next_shed="));
    Serial.println(t_next_shed);
    Serial.print(F("next_shed_ch="));
    Serial.println(next_shed_ch);
    Serial.print(F("next_label="));
    Serial.println(next_label);
  }



}
#endif  //debug
