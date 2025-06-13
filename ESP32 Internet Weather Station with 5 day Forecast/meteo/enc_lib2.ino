

#if defined (encoder_lib2) && defined (ESP32)
//=======================================================================================
//                encoder
//=======================================================
void do_encoder()
{

  //  uint32_t static t_next = 0;
  int static oldpos = 0;
  static int num = -99;
  static uint32_t t_next_press_read = 0;
  static byte press_count_holded = 0;
  static String fn_cmd_tmp = "";
  byte press_count_esc = 0;
  byte press_count_play = 0 ;
  byte press_count_play_fm = 0;
  byte press_count_play_mp3 = 0 ;
  byte delta = 0;
  const byte delta_tick = 5;

#ifdef ESP32
  delta = delta + delta_tick;
  press_count_play = delta;
#endif
#if defined (fmSi4703)|| defined (fmRDA5807)
  delta = delta + delta_tick;
  press_count_play_fm = delta;
#endif
#if defined (pin_sd)&&defined (ESP32)
  delta = delta + delta_tick;
  press_count_play_mp3 = delta;
#endif
  if (delta > 0)
  {
    delta = delta + delta_tick;
    press_count_esc = delta;
  }
  static uint32_t t_num = 0; //разница при вращении, обнуляю при остановке

  char inc = 'x';
  static int value = 0;
  long  newpos = enc.getCount();

  // if (millis() > t_next)
  // {
  if (newpos != oldpos) {

    if  (newpos > oldpos) inc = '+';
    else inc = '-';
    oldpos = newpos;
    //   Serial.print("newpos=");  Serial.println(newpos);
    //      t_next = millis() + t_enc_sleep;
  }
  // }
  enc_btn.tick();  // обязательная функция отработки. Должна постоянно опрашиваться


  //clear num www
  if (millis() - t_num > t_enc_num_clear && num != -99)
  {
    draw_num(0, 'C');
    //    Serial.println("enc t_num=-99");
    num = -99;
  }

  if (s_info)
  {
    if (enc_btn.isSingle() || inc != 'x')
      fn_cmd = "Scr_info";
  }
  else
    //-----------NOT PLAY---------------NOT PLAY------------------NOT PLAY-----------------
    if (!play && !play_fm && !play_mp3 && !scr_timer )
    {

      if (inc == '+')
      {
        fn_cmd = "next"; inc = 'x';
        //    Serial.println("enc2 !play && !play_fm && !play_mp3=next");
      }
      if (inc == '-')
      {
        fn_cmd = "prev"; inc = 'x';
        //   Serial.println("enc2 !play && !play_fm && !play_mp3=prev");
      }
      //hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh
      if (enc_btn.isStep())
      {
        //   Serial.println("enc isHolded");
        if  (press_count_esc > 0)
        {
          //   if (t_next_press_read > millis()) return;
          //   t_next_press_read = millis() + 200;
          press_count_holded++;
#ifdef debug
          Serial.print("press_count_holded="); Serial.println(press_count_holded);
#endif
          if (press_count_holded >= press_count_esc )
          {
            if (press_count_holded == press_count_esc) draw_num(0, 'C');
            fn_cmd = "";
            fn_cmd_tmp = "";
            press_count_holded = 0;
#ifdef debug
            Serial.println("cmd press_count_holded=ESC");
#endif
            return;
          }
#if defined ( pin_sd) && defined ( ESP32)         //MP3
          if (press_count_play_mp3 > 0 && press_count_holded >= press_count_play_mp3 )
          {
            if (press_count_holded == press_count_play_mp3) draw_num(0, 'C');
            draw_play_mp3();
#ifdef debug
            Serial.println("cmd press_count_holded=play_MP3");
#endif
            fn_cmd_tmp = "play_mp3";
            return;
          }
#endif //pin_sd    MP3
          //FM
#if defined (fmSi4703)|| defined (fmRDA5807)
          if (press_count_play_fm > 0 && press_count_holded >= press_count_play_fm )
          {
            if (press_count_holded == press_count_play_fm)  draw_num(0, 'C');
            draw_play_fm();
            fn_cmd_tmp = "play_fm";
#ifdef debug
            Serial.println("cmd press_count_holded=play_fm");
#endif
            return;
          }
#endif //FM


#ifdef ESP32        //WWW
          if (press_count_play > 0 && press_count_holded >= press_count_play )
          {
            if (press_count_holded == press_count_play)  draw_num(0, 'C');
            draw_play();
            fn_cmd_tmp = "play";
#ifdef debug
            Serial.println("cmd press_count_holded=play");
#endif
            return;
          }

#endif //32

        }//delta


      } //isHolded

      //-------------------------------отпущена
      if (enc_btn.isRelease() && press_count_holded > 0) //отпущена
      {
        if (fn_cmd_tmp != "")
        {
          fn_cmd = fn_cmd_tmp;
          fn_cmd_tmp = "";
          press_count_holded = 0;
#ifdef debug
          Serial.println(F("enc.isReleaseHold()"));
#endif
          enc_btn.resetStates();
        }

      }
      //--------------------

#ifdef enc_onclick_noplay //"next"//next, prev, play, play_fm, play_mp3
      if (enc_btn.isSingle())
      {
#ifdef debug
        Serial.println(F("enc_onclick_noplay isSingle"));
#endif
        fn_cmd = enc_onclick_noplay;

      }

#endif//isSingle

#ifdef enc_ondblclick_noplay //"next"//next, prev, play, play_fm, play_mp3    
      if (enc_btn.isDouble())
      {
#ifdef debug
        Serial.println(F("enc_ondblclick_noplay isDouble"));
#endif
        fn_cmd = enc_ondblclick_noplay;

      }//isDouble
#endif



#ifdef enc_ontripleclick_noplay //"next"//next, prev, play, play_fm, play_mp3    
      if (enc_btn. isTriple())
      {
#ifdef debug
        Serial.println(F("enc_ontripleclick_noplay isTriple"));
#endif
        fn_cmd = enc_ontripleclick_noplay;

      }//isDouble
#endif



    } //!play


    else

      //--------------TIMER-----------------------------TIMER------------------------------
      if (scr_timer)
      {
        //isClick
        if (enc_btn.isSingle())
        {
          fn_cmd = "next";
#ifdef debug
          Serial.println("enc.isClick next");
#endif


        }

        if (inc == '+')
        {
          fn_cmd = "vl+"; inc = 'x';
#ifdef debug
          Serial.println("enc.isRight vl+");
#endif
        }

        if (inc == '-')
        {
          fn_cmd = "vl-"; inc = 'x';
#ifdef debug
          Serial.println("enc.isLeft vl-");
#endif
        }
        if (enc_btn.isHolded())
        {
          fn_cmd = "timer_start";
#ifdef debug
          Serial.println("enc.isHolded timer_start/stop");
#endif
        }

        if (enc_btn.isDouble())
        {
          fn_cmd = "timer";
#ifdef debug
          Serial.println("enc.isDouble timer exit");
#endif
        }//isDouble


      }
  //--EEEEEEEEEEEE-----TIMER------------------------------

      else

        //--------------PLAY-----------------------------MP3------------------------------

        if (play_mp3)
        {
#if defined (pin_sd)
          if (inc == '+')
          {
            fn_cmd = "next"; inc = 'x';
#ifdef debug
            Serial.println("enc.isRight next");
#endif
          }

          if (inc == '-')
          {
            fn_cmd = "prev"; inc = 'x';
#ifdef debug
            Serial.println("enc.isLeft prev");
#endif
          }

          if (enc_btn.isHolded())
          {
            fn_cmd = enc_hld_play_mp3;
#ifdef debug
            Serial.println(enc_hld_play_mp3);
#endif

          }//isHolded


          //isClick
          if (enc_btn.isSingle())
          {
            fn_cmd = enc_onclick_play_mp3;
#ifdef debug
            Serial.println(enc_onclick_play_mp3);
#endif
          }


          if (enc_btn.isDouble() )
          {

            fn_cmd = enc_ondblclick_play_mp3;
#ifdef debug
            Serial.println(enc_ondblclick_play_mp3);
#endif

          } //isDouble


          if (enc_btn.isTriple() )
          {

            fn_cmd = enc_ontripleclick_play_mp3;
#ifdef debug
            Serial.println(enc_ontripleclick_play_mp3);
#endif

          } //isTriple

#endif   //pin_sd mp3
        }
  //-----------------EEEEEEE-----------MP3----------------

        else

          //--------------PLAY------------WWW------------------------------
          if (play)
          {
#ifdef ESP32
            //Right
            if (inc == '+')
            {
              if (num == -99) num = ch;
              num++;
              if (num > ch_max) num = ch_min;
              draw_num(num, 'C');
#ifdef debug
              Serial.println("enc play Righ num++");
#endif
              t_num = millis();
              return;
            }

            //Left
            if (inc == '-')
            {
              if (num == -99) num = ch;
              num--;
              if (num < 1) num = ch_max;
              draw_num(num, 'C');
#ifdef debug
              Serial.println(F("enc play Left num--"));
#endif
              t_num = millis();
              return;

            }

            //hold start timer-------------------------
            if (enc_btn.isHolded())
            {
              if (num != -99)
              {
#ifdef debug
                Serial.println("enc Holded play Timer WWW");
#endif
                timer_start(num, 0);
                num = -99;
              }
              else
              {
#ifdef debug
                Serial.println(enc_hld_play);
#endif
                fn_cmd = enc_hld_play; //
              }
            }
            //isHolded

            if (enc_btn.isTriple())
            {
#ifdef debug
              Serial.println("enc play isTriple");
#endif
              if (num == -99)  fn_cmd = enc_ontripleclick_play;

            } //isTriple



            if (enc_btn.isDouble())
            {
#ifdef debug
              Serial.println("enc play isDouble");
#endif
              if (num == -99)  fn_cmd = enc_ondblclick_play;

            } //isDouble


            //isClick
            if (enc_btn.isSingle()) //isClick())
            {
#ifdef debug
              Serial.println("enc play isSingle");
#endif
              if (num != -99) //выбор канала
              {
                if (ch != num)
                {
                  ch_fnd = num; //выбор канала
                  fn_cmd = "play";
                }
                draw_num(0, 'C');
#ifdef debug
                Serial.println("enc t_num!=-99 play num");
#endif
                num = -99;
              }
              else if (t_timer != 0) //, иначе стоп таймер
              {
                timer_stop();
#ifdef debug
                Serial.print("enc TIMER stop="); Serial.println(t_timer);
#endif
                fn_cmd = "";
              }
              else
              {
                fn_cmd = enc_onclick_play; //STOP если канал не выбран и нет таймера
#ifdef debug
                Serial.println(enc_onclick_play);
#endif
              }

            } //isSingle

#endif //esp32
          } //play


  //--------------END------WWW--------PLAY----------------------




  if (fn_cmd != "")
  {
    if ((fn_cmd == "next" || fn_cmd == "prev" || fn_cmd == "fav"
         || fn_cmd == "play" || fn_cmd == "play_fm" || fn_cmd == "stop"
         || fn_cmd == "stop_fm")
        && (play || play_fm)
        && alarm_active == true)
    {
#ifdef debug
      Serial.println(F("fn_cmd_enc-->STOP timer alarm"));
#endif
      timer_alarm_stop();
      ch_prev_alarm_www = 0;
      ch_prev_alarm_fm = 0;
      ch_prev_alarm_mp3 = 0;

    }

#ifdef debug
    Serial.print(F("fn_cmd_enc=")); Serial.println(fn_cmd);
#endif
    //  enc_btn.resetStates();
    do_fn_btn_ir();
    fn_cmd = "";
    press_count_holded = 0;
    if (play || play_fm || play_mp3) draw_num(0, 'C');

  }

}

#endif //enc
