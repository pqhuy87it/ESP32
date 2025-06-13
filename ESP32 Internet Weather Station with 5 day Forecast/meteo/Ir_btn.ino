#if defined( pin_analog_key1)


void do_4key()
{
  int analog;
  static uint32_t t_next = 0;
  static byte k1 = 0, k2 = 0, k3 = 0, k4 = 0, k5 = 0;
  static byte k1f = 0, k2f = 0, k3f = 0, k4f = 0, k5f = 0;
  byte K = 1, KF = 2;
  if (millis() < t_next) return; //pause after cmd


  analog = analogRead(pin_analog_key1);


  /*
    if  (key1_test_min_max > 0)
    {
      t_next = millis() +  key1_test_min_max * 1000;
      Serial.println("key1_test= " + String(analog));
      return;
    }
  */

  //------11111111111111111111111111-------------------------
#ifdef key1btn_1

  if (analog >= key1btn1_min && analog <= key1btn1_max)
  {
    k1++;
#ifdef debug
    Serial.println("key1btn1 tick " + String(analog));
#endif

    if (k1 > K)
    {
      key1btn1.tick(true);
      k1 = 0;
      k1f = 0;
    }
  } else
  {
    k1f++;
    if (k1f > KF)
    {
      key1btn1.tick(false);
      k1f = 0;
    }

  }


  if (key1btn1.isSingle())
  {
#ifdef debug
    Serial.println("key1btn1.isSingle");
#endif
    if (play) fn_cmd = key1btn1_play;
    else if (play_mp3) fn_cmd = key1btn1_play_mp3;
    else if (play_fm) fn_cmd = key1btn1_play_fm;
    else if (scr_timer) fn_cmd = key1btn1_timer;
    else
      fn_cmd = key1btn1_noplay;
  }

  if (key1btn1.isDouble())
  {
#ifdef debug
    Serial.println("key1btn1.isDouble");
#endif
    if (play) fn_cmd = key1btn1_dbl_play;
    else if (play_mp3) fn_cmd = key1btn1_dbl_play_mp3;
    else if (play_fm) fn_cmd = key1btn1_dbl_play_fm;
    else if (scr_timer) fn_cmd = key1btn1_dbl_timer;
    else
      fn_cmd = key1btn1_dbl_noplay;
  }


  if (key1btn1.isTriple())
  {
#ifdef debug
    Serial.println("key1btn1.isTriple");
#endif
    if (play) fn_cmd = key1btn1_trpl_play;
    else if (play_mp3) fn_cmd = key1btn1_trpl_play_mp3;
    else if (play_fm) fn_cmd = key1btn1_trpl_play_fm;
    else
      fn_cmd = key1btn1_trpl_noplay;
  }


  if (key1btn1.isHolded())
  {
#ifdef debug
    Serial.println("key1btn1.isHolded");
#endif
    if (play) fn_cmd = key1btn1_hld_play;
    else if (play_mp3) fn_cmd = key1btn1_hld_play_mp3;
    else if (play_fm) fn_cmd = key1btn1_hld_play_fm;
    else if (scr_timer) fn_cmd = key1btn1_hld_timer;
    else
      fn_cmd = key1btn1_hld_noplay;
  }

#endif //key1btn1_min

  //-------------------------22222222222222222-----------------
#ifdef key1btn_2

  if (analog >= key1btn2_min && analog <= key1btn2_max)
  {
    k2++;
#ifdef debug
    Serial.println("key1btn2 tick " + String(analog));
#endif

    if (k2 > K)
    {
      key1btn2.tick(true);
      k2 = 0;
      k2f = 0;
    }
  } else
  {
    k2f++;
    if (k2f > KF)
    {
      key1btn2.tick(false);
      k2f = 0;
    }

  }


  if (key1btn2.isSingle())
  {
#ifdef debug
    Serial.println("key1btn2.isSingle");
#endif
    if (play) fn_cmd = key1btn2_play;
    else if (play_mp3) fn_cmd = key1btn2_play_mp3;
    else if (play_fm) fn_cmd = key1btn2_play_fm;
    else if (scr_timer) fn_cmd = key1btn2_timer;
    else
      fn_cmd = key1btn2_noplay;
  }

  if (key1btn2.isDouble())
  {
#ifdef debug
    Serial.println("key1btn2.isDouble");
#endif
    if (play) fn_cmd = key1btn2_dbl_play;
    else if (play_mp3) fn_cmd = key1btn2_dbl_play_mp3;
    else if (play_fm) fn_cmd = key1btn2_dbl_play_fm;
    else if (scr_timer) fn_cmd = key1btn2_dbl_timer;
    else
      fn_cmd = key1btn2_dbl_noplay;

  }


  if (key1btn2.isTriple())
  {
#ifdef debug
    Serial.println("key1btn2.isTriple");
#endif
    if (play) fn_cmd = key1btn2_trpl_play;
    else if (play_mp3) fn_cmd = key1btn2_trpl_play_mp3;
    else if (play_fm) fn_cmd = key1btn2_trpl_play_fm;
    else
      fn_cmd = key1btn2_trpl_noplay;

  }

  if (key1btn2.isHolded())
  {
#ifdef debug
    Serial.println("key1btn2.isHolded");
#endif
    if (play) fn_cmd = key1btn2_hld_play;
    else if (play_mp3) fn_cmd = key1btn2_hld_play_mp3;
    else if (play_fm) fn_cmd = key1btn2_hld_play_fm;
    else if (scr_timer) fn_cmd = key1btn2_hld_timer;
    else
      fn_cmd = key1btn2_hld_noplay;

  }

#endif //key1btn2_min


  //-----------------33333333333333333333333333333--------------
#ifdef key1btn_3

  if (analog >= key1btn3_min && analog <= key1btn3_max)
  {
    k3++;
#ifdef debug
    Serial.println("key1btn3 tick " + String(analog));
#endif

    if (k3 > K)
    {
      key1btn3.tick(true);
      k3 = 0;
      k3f = 0;
    }
  } else
  {
    k3f++;
    if (k3f > KF)
    {
      key1btn3.tick(false);
      k3f = 0;
    }

  }


  if (key1btn3.isSingle())
  {
#ifdef debug
    Serial.println("key1btn3.isSingle");
#endif

    if (play) fn_cmd = key1btn3_play;
    else if (play_mp3) fn_cmd = key1btn3_play_mp3;
    else if (play_fm) fn_cmd = key1btn3_play_fm;
    else if (scr_timer) fn_cmd = key1btn3_timer;
    else
      fn_cmd = key1btn3_noplay;
  }

  if (key1btn3.isDouble())
  {
#ifdef debug
    Serial.println("key1btn3.isDouble");
#endif
    if (play) fn_cmd = key1btn3_dbl_play;
    else if (play_mp3) fn_cmd = key1btn3_dbl_play_mp3;
    else if (play_fm) fn_cmd = key1btn3_dbl_play_fm;
    else if (scr_timer) fn_cmd = key1btn3_dbl_timer;
    else
      fn_cmd = key1btn3_dbl_noplay;
  }


  if (key1btn3.isTriple())
  {
#ifdef debug
    Serial.println("key1btn3.isTriple");
#endif
    if (play) fn_cmd = key1btn3_trpl_play;
    else if (play_mp3) fn_cmd = key1btn3_trpl_play_mp3;
    else if (play_fm) fn_cmd = key1btn3_trpl_play_fm;
    else
      fn_cmd = key1btn3_trpl_noplay;
  }


  if (key1btn3.isHolded())
  {
#ifdef debug
    Serial.println("key1btn3.isHolded");
#endif
    if (play) fn_cmd = key1btn3_hld_play;
    else if (play_mp3) fn_cmd = key1btn3_hld_play_mp3;
    else if (play_fm) fn_cmd = key1btn3_hld_play_fm;
    else if (scr_timer) fn_cmd = key1btn3_hld_timer;
    else
      fn_cmd = key1btn3_hld_noplay;
  }
#endif //key1btn3_min


  //---------44444444444444444444444444444444444444--------------
#ifdef key1btn_4

  if (analog >= key1btn4_min && analog <= key1btn4_max)
  {
    k4++;
#ifdef debug
    Serial.println("key1btn4 tick " + String(analog));
#endif

    if (k4 > K)
    {
      key1btn4.tick(true);
      k4 = 0;
      k4f = 0;
    }
  } else
  {
    k4f++;
    if (k4f > KF)
    {
      key1btn4.tick(false);
      k4f = 0;
    }

  }


  if (key1btn4.isSingle())
  {
#ifdef debug
    Serial.println("key1btn4.isSingle");
#endif

    if (play) fn_cmd = key1btn4_play;
    else if (play_mp3) fn_cmd = key1btn4_play_mp3;
    else if (play_fm) fn_cmd = key1btn4_play_fm;
    else if (scr_timer) fn_cmd = key1btn4_timer;
    else
      fn_cmd = key1btn4_noplay;
  }

  if (key1btn4.isDouble())
  {
#ifdef debug
    Serial.println("key1btn4.isDouble");
#endif
    if (play) fn_cmd = key1btn4_dbl_play;
    else if (play_mp3) fn_cmd = key1btn4_dbl_play_mp3;
    else if (play_fm) fn_cmd = key1btn4_dbl_play_fm;
    else if (scr_timer) fn_cmd = key1btn4_dbl_timer;
    else
      fn_cmd = key1btn4_dbl_noplay;
  }


  if (key1btn4.isTriple())
  {
#ifdef debug
    Serial.println("key1btn4.isTriple");
#endif
    if (play) fn_cmd = key1btn4_trpl_play;
    else if (play_mp3) fn_cmd = key1btn4_trpl_play_mp3;
    else if (play_fm) fn_cmd = key1btn4_trpl_play_fm;
    else
      fn_cmd = key1btn4_trpl_noplay;
  }


  if (key1btn4.isHolded())
  {
#ifdef debug
    Serial.println("key1btn4.isHolded");
#endif
    if (play) fn_cmd = key1btn4_hld_play;
    else if (play_mp3) fn_cmd = key1btn4_hld_play_mp3;
    else if (play_fm) fn_cmd = key1btn4_hld_play_fm;
    else if (scr_timer) fn_cmd = key1btn4_hld_timer;
    else
      fn_cmd = key1btn4_hld_noplay;
  }

#endif //4btn





  //---------55555555555555555555555555555555555555--------------
#ifdef key1btn_5

  if (analog >= key1btn5_min && analog <= key1btn5_max)
  {
    k5++;
#ifdef debug
    Serial.println("key1btn5 tick " + String(analog));
#endif

    if (k5 > K)
    {
      key1btn5.tick(true);
      k5 = 0;
      k5f = 0;
    }
  } else
  {
    k5f++;
    if (k5f > KF)
    {
      key1btn5.tick(false);
      k5f = 0;
    }

  }


  if (key1btn5.isSingle())
  {
#ifdef debug
    Serial.println("key1btn5.isSingle");
#endif

    if (play) fn_cmd = key1btn5_play;
    else if (play_mp3) fn_cmd = key1btn5_play_mp3;
    else if (play_fm) fn_cmd = key1btn5_play_fm;
    else if (scr_timer) fn_cmd = key1btn5_timer;
    else
      fn_cmd = key1btn5_noplay;
  }

  if (key1btn5.isDouble())
  {
#ifdef debug
    Serial.println("key1btn5.isDouble");
#endif
    if (play) fn_cmd = key1btn5_dbl_play;
    else if (play_mp3) fn_cmd = key1btn5_dbl_play_mp3;
    else if (play_fm) fn_cmd = key1btn5_dbl_play_fm;
    else if (scr_timer) fn_cmd = key1btn5_dbl_timer;
    else
      fn_cmd = key1btn5_dbl_noplay;
  }


  if (key1btn5.isTriple())
  {
#ifdef debug
    Serial.println("key1btn5.isTriple");
#endif
    if (play) fn_cmd = key1btn5_trpl_play;
    else if (play_mp3) fn_cmd = key1btn5_trpl_play_mp3;
    else if (play_fm) fn_cmd = key1btn5_trpl_play_fm;
    else
      fn_cmd = key1btn5_trpl_noplay;
  }


  if (key1btn5.isHolded())
  {
#ifdef debug
    Serial.println("key1btn5.isHolded");
#endif
    if (play) fn_cmd = key1btn5_hld_play;
    else if (play_mp3) fn_cmd = key1btn5_hld_play_mp3;
    else if (play_fm) fn_cmd = key1btn5_hld_play_fm;
    else if (scr_timer) fn_cmd = key1btn5_hld_timer;
    else
      fn_cmd = key1btn5_hld_noplay;
  }

#endif //5btn
  //==================================E 5----


  if (fn_cmd != "")
  {
    do_fn_btn_ir();
    fn_cmd = "";
    if (play || play_fm || play_mp3) draw_num(0, 'C');

    //   if (fn_cmd != "timer_start")
    t_next = millis() +  1000;

    k1 = 0; k2 = 0; k3 = 0; k4 = 0; k5 = 0;
    k1f = 0; k2f = 0; k3f = 0; k4f = 0; k5f = 0;

#ifdef key1btn_1
    key1btn1.tick(false);
#endif

#ifdef key1btn_2
    key1btn2.tick(false);
#endif
#ifdef key1btn_3
    key1btn3.tick(false);
#endif
#ifdef key1btn_4
    key1btn4.tick(false);
#endif
#ifdef key1btn_5
    key1btn5.tick(false);
#endif

  }
  //    else

}



#endif //key1btn4_min









#ifdef encoder
//=======================================================================================
//                encoder
//=======================================================================================
void do_encoder()
{
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
  // static uint32_t t_num_fm = 0; //разница при вращении, обнуляю при остановке

  enc.tick();




  //#ifdef ESP32

  //clear num www
  if (millis() - t_num > t_enc_num_clear && num != -99)
  {
    draw_num(0, 'C');
    Serial.println("enc t_num=-99");
    num = -99;
  }

  /*
    //clear num_fm
    if (millis() - t_num_fm > t_enc_num_clear && num_fm != -99)
    {
      draw_num(0, 'C');
      Serial.println("enc t_num_fm=-99");
      num_fm = -99;
    }
  */
  //play----------PLAY------------PLAY------------PLAY------------PLAY------------PLAY-------

  //for all play
  if (play_fm || play || play_mp3 || scr_timer)
  {


    //Volume vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    if (enc.isRightH())  //isRightH holded
    {
      fn_cmd = "vl+"; //
      Serial.println("enc fn_cmd=vl+");
    }

    if (enc.isLeftH()) //isLeftH holded
    {
      fn_cmd = "vl-"; //
      Serial.println("enc fn_cmd=vl-");
    }

  }



  //--------------PLAY-----------------------------TIMER------------------------------


  if (scr_timer)
  {
    //isClick
    if (enc.isSingle())
    {
      fn_cmd = "next";
#ifdef debug
      Serial.println("enc.isClick next");
#endif


    }

    if (enc.isRight())
    {
      fn_cmd = "vl+";
#ifdef debug
      Serial.println("enc.isRight vl+");
#endif
    }

    if (enc.isLeft())
    {
      fn_cmd = "vl-";
#ifdef debug
      Serial.println("enc.isLeft vl-");
#endif
    }
    if (enc.isHolded())
    {
      fn_cmd = "timer_start";
#ifdef debug
      Serial.println("enc.isHolded timer_start/stop");
#endif
    }

    if (enc.isDouble())
    {
      fn_cmd = "timer";
#ifdef debug
      Serial.println("enc.isDouble timer exit");
#endif
    }//isDouble


  }
  //--EEEEEEEEEEEE--PLAY---------------------------TIMER------------------------------


  //--------------PLAY-----------------------------MP3------------------------------

#if defined (pin_sd)

  if (play_mp3)
  {
    //isClick
    if (enc.isSingle())
    {
      fn_cmd = "stop_mp3";
#ifdef debug
      Serial.println("enc.isClick stop_mp3");
#endif


    }

    if (enc.isRight())
    {
      fn_cmd = "next";
#ifdef debug
      Serial.println("enc.isRight next");
#endif
    }

    if (enc.isLeft())
    {
      fn_cmd = "prev";
#ifdef debug
      Serial.println("enc.isLeft prev");
#endif
    }
    if (enc.isHolded())
    {
      fn_cmd = "mute";
#ifdef debug
      Serial.println("enc.isHolded mute");
#endif
    }

    if (enc.isDouble() )
    {

      fn_cmd = "mp3_random";
#ifdef debug
      Serial.print("enc mp3_random");
#endif


    } //isDouble




  }
#endif   //pin_sd mp3
  //-----------------EEEEEEE-----------MP3----------------





#if defined (fmSi4703)|| defined (fmRDA5807)
  //--------------PLAY-----------------------------FM------------------------------

  if (play_fm)
  {

    //isClick
    if (enc.isSingle()) //isClick())
    {

      Serial.println("enc play_fm isSingle");
      if (num != -99) //выбор канала
      {
        if (ch_fm != num)
        {
          ch_fm = num; //выбор канала
          fn_cmd = "play_fm";
        }
        draw_num(0, 'C');
        Serial.println("enc t_num!=-99 play_fm num");
        num = -99;
      }
      else if (t_timer != 0) //, иначе стоп таймер
      {
        timer_stop();
        Serial.print("enc TIMER stop fm="); Serial.println(t_timer);
        fn_cmd = "";
      }
      else
      {
        fn_cmd = "stop_fm"; //STOP если канал не выбран и нет таймера
        Serial.println("enc fn_cmd=stop_fm");
      }



    } //isSingle





    //Right
    if (enc.isRight())
    {
      if (num == -99) num = ch_fm;
      num++;
      if (num > ch_max_fm) num = ch_min_fm;
      draw_num(num, 'C');
      Serial.println("enc Righ num_fm++");
      t_num = millis();
      return;
    }

    //Left
    if (enc.isLeft())
    {
      if (num == -99) num = ch_fm;
      num--;
      if (num < 1) num = ch_max_fm;
      draw_num(num, 'C');
      Serial.println(F("enc Left num_fm--"));
      t_num = millis();
      return;

    }


    //favorite
    if (enc.isDouble() )
    {
      Serial.println("enc isDouble fm");

      if (num == -99) fn_cmd = "fav";

    } //isDouble





    //hold start timer-------------------------
    if (enc.isHolded())
    {
      if (num != -99)
      {
        Serial.println("Holded Timer FM");
        timer_start(num, 1);
        num = -99;

      }
      else
      {
        Serial.println("Holded mute FM");
        fn_cmd = "mute"; //
      }
    } //isHolded

  }
#endif   //play_fm 
  //-----------------EEEEEEE-----------FM----------------




  //--------------PLAY------------WWW------------------------------
#ifdef ESP32
  if (play)
  {
    //Right
    if (enc.isRight())
    {
      if (num == -99) num = ch;
      num++;
      if (num > ch_max) num = ch_min;
      draw_num(num, 'C');
      Serial.println("enc play Righ num++");
      t_num = millis();
      return;
    }

    //Left
    if (enc.isLeft())
    {
      if (num == -99) num = ch;
      num--;
      if (num < 1) num = ch_max;
      draw_num(num, 'C');
      Serial.println(F("enc play Left num--"));
      t_num = millis();
      return;

    }





    //hold start timer-------------------------
    if (enc.isHolded())
    {
      if (num != -99)
      {
        Serial.println("enc Holded play Timer WWW");
        timer_start(num, 0);
        num = -99;
      }
      else
      {
        Serial.println("enc Holded play mute");
        fn_cmd = "mute"; //
      }
    }

    //isHolded

    //favorite fffffffffffffffffffffffffffff
    if (enc.isDouble() && fav_count_real > 0) //>=2
    {
      Serial.println("enc play isDouble");
      if (num == -99)  fn_cmd = "fav";

    } //isDouble


    //isClick
    if (enc.isSingle()) //isClick())
    {

      Serial.println("enc play isSingle");
      if (num != -99) //выбор канала
      {
        if (ch != num)
        {
          ch_fnd = num; //выбор канала
          fn_cmd = "play";
        }
        draw_num(0, 'C');
        Serial.println("enc t_num!=-99 play num");
        num = -99;
      }
      else if (t_timer != 0) //, иначе стоп таймер
      {
        timer_stop();
        Serial.print("enc TIMER stop="); Serial.println(t_timer);
        fn_cmd = "";
      }
      else
      {
        fn_cmd = "stop"; //STOP если канал не выбран и нет таймера
        Serial.println("enc play fn_cmd=stop");
      }

    } //isSingle






  } //play

#endif //esp32
  //--------------END------WWW--------PLAY----------------------





  //-----------NOT PLAY---------------NOT PLAY------------------NOT PLAY-----------------
  if (!play && !play_fm && !play_mp3 && !scr_timer)
  {


    if (enc.isRight())
    {
      fn_cmd = "next";
      Serial.println("enc !play && !play_fm && !play_mp3=next");
    }
    if (enc.isLeft())
    {
      fn_cmd = "prev";
      Serial.println("enc !play && !play_fm && !play_mp3=prev");
    }
    //hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh
    if (enc.isHold())
    {
      //   Serial.println("enc isHolded");
      if  (press_count_esc > 0)
      {
        if (t_next_press_read > millis()) return;
        t_next_press_read = millis() + 200;
        press_count_holded++;
        Serial.print("press_count_holded="); Serial.println(press_count_holded);

        if (press_count_holded >= press_count_esc )
        {
          if (press_count_holded == press_count_esc) draw_num(0, 'C');
          fn_cmd = "";
          fn_cmd_tmp = "";
          press_count_holded = 0;
          Serial.println("cmd press_count_holded=ESC");
          return;
        }
#if defined ( pin_sd) && defined ( ESP32)         //MP3
        if (press_count_play_mp3 > 0 && press_count_holded >= press_count_play_mp3 )
        {
          if (press_count_holded == press_count_play_mp3) draw_num(0, 'C');
          draw_play_mp3();
          Serial.println("cmd press_count_holded=play_MP3");
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
          Serial.println("cmd press_count_holded=play_fm");
          return;
        }
#endif //FM


#ifdef ESP32        //WWW
        if (press_count_play > 0 && press_count_holded >= press_count_play )
        {
          if (press_count_holded == press_count_play)  draw_num(0, 'C');
          draw_play();
          fn_cmd_tmp = "play";
          Serial.println("cmd press_count_holded=play");
          return;
        }

#endif //32

      }//delta

    } //isHolded

    //-------------------------------отпущена
    if (enc.isReleaseHold()) //отпущена
    {
      if (fn_cmd_tmp != "")
      {
        fn_cmd = fn_cmd_tmp;
        fn_cmd_tmp = "";
        press_count_holded = 0;
        Serial.println(F("enc.isReleaseHold()"));
      }

    }
    //--------------------


#ifdef pin_led
    //led
    if (enc.isRightH())  //isRightH holded
    {
      fn_cmd = "led+"; //
      Serial.println("enc fn_cmd=led+");
    }

    if (enc.isLeftH()) //isLeftH holded
    {
      fn_cmd = "led-"; //
      Serial.println("enc fn_cmd=led-");
    }
#endif  //led


#ifdef enc_onclick_noplay //"next"//next, prev, play, play_fm, play_mp3
    if (enc.isSingle())
    {
#ifdef debug
      Serial.println(F("enc_onclick_noplay isSingle"));
#endif
      fn_cmd = enc_onclick_noplay;
    } //isSingle
#endif


#ifdef enc_ondblclick_noplay //"next"//next, prev, play, play_fm, play_mp3
    if (enc.isDouble())
    {
#ifdef debug
      Serial.println(F("enc_ondblclick_noplay isDouble"));
#endif
      fn_cmd = enc_ondblclick_noplay;
    }//isDouble
#endif




  } //E not play-------------eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee




  if (fn_cmd != "")
  {
    enc.resetStates();
    if ((fn_cmd == "next" || fn_cmd == "prev" || fn_cmd == "fav"
         || fn_cmd == "play" || fn_cmd == "play_fm" || fn_cmd == "stop"
         || fn_cmd == "stop_fm")
        && (play || play_fm)
        && alarm_active == true)
    {
      Serial.println(F("fn_cmd_enc-->STOP timer alarm"));
      timer_alarm_stop();
      ch_prev_alarm_www = 0;
      ch_prev_alarm_fm = 0;
      ch_prev_alarm_mp3 = 0;

    }
#ifdef debug
    Serial.print(F("fn_cmd_enc=")); Serial.println(fn_cmd);
#endif
    do_fn_btn_ir();
    fn_cmd = "";
    press_count_holded = 0;
    if (play || play_fm || play_mp3) draw_num(0, 'C');

  }

  //#endif //ESP32

  // enc.resetStates();
  /*
    #ifdef ESP8266
    //next SCR
    if (fn_cmd == "next" && !play && !play_fm && !play_mp3) //след. станция если уже играет
    {
      scr_number++;
      scr_num_show(scr_number);
    }

    //prev SCR
    if (fn_cmd == "prev" && !play && !play_fm && !play_mp3) //след. станция если уже играет
    {
      scr_number--;
      scr_num_show(scr_number);

    }
    fn_cmd = "";
    #endif //ESP8266

  */
}

#endif //enc





//======================================================================================
//                                     do_touch_btn()
//======================================================================================

#if (defined (pin_btn_touch)||defined(pin_btn_No_touch)) //&& defined (ESP32)


void do_touch_btn(byte touch_or_btn) //1-esp32 toch 2-butt
{
  static uint32_t t_next_touchRead = 0;
  if (t_next_touchRead > millis()) return;

  byte delta = 0;
  static byte touch_count = 0;
  const byte press_count_click = 1;
  const byte press_count_next = 3;
  const byte press_count_prev = 7;
  const byte press_count_stop_onplay = 10; //on play
  const byte press_count_esc_onplay = 13; //on play
#ifdef ESP32
  delta = delta + 4;
  const byte press_count_play = press_count_prev + delta;
#if defined (fmSi4703)|| defined (fmRDA5807)
  byte  press_count_play_fm;
  delta = delta + 4;
  press_count_play_fm = press_count_prev + delta;
#endif //esp si
#if defined (pin_sd)&&defined (ESP32)
  byte  press_count_play_mp3;
  delta = delta + 4;
  press_count_play_mp3 = press_count_prev + delta;
#endif //sd

#endif //esp32 


#ifdef ESP8266
#if defined (fmSi4703)|| defined (fmRDA5807)
  byte press_count_play_fm;
  delta = delta + 4; press_count_play_fm = press_count_prev + delta;
#endif //esp si
#endif //esp82 


  delta = delta + 4;
  const byte press_count_esc = press_count_prev + delta;

  static char fn_cmd_touch = '0'; //N,P,S,E,M



  int touch_value_read = 999;




#ifdef pin_btn_touch
  if (touch_or_btn == 1) touch_value_read = touchRead(pin_btn_touch);
#endif //pin_btn_touch


#ifdef pin_btn_No_touch
  if (touch_or_btn == 2 && digitalRead(pin_btn_No_touch) == LOW)

  {
    touch_value_read = touch_value;
#ifdef debug
    Serial.print(F("pin_btn_No_touch="));
    Serial.println(touch_value_read);
#endif
  }
#endif //pin_btn_No_touch




  //нажата----------------------------------------------

  if ( touch_value_read <= touch_value)

  {
    touch_count++;
    t_next_touchRead = millis() + 300;


#ifdef debug
    if (touch_count > 0) //ложные искл.
    {
      Serial.print(F("touch_value_read="));
      Serial.println(touch_value_read);
      Serial.print(F("touch_count==="));
      Serial.println(touch_count);
    }
#endif

    byte press_count_esc_tmp;
    if ((play || play_fm || play_mp3) )press_count_esc_tmp = press_count_esc_onplay;
    else
      press_count_esc_tmp = press_count_esc;

    if (touch_count >= press_count_esc_tmp) //отмена
    {
      if (touch_count == press_count_esc_tmp)
      {
        draw_num(0, 'C');
        fn_cmd_touch = '-'; //reset
        fn_cmd = "";
      }
      if (touch_count > press_count_esc_tmp + 3) //pause+round
      {
        touch_count = 0;
        fn_cmd_touch = '0';
      }

      return;
    }







    //STOP on play
    if ((play || play_fm || play_mp3) ) //stop
    {

      if (touch_count >= press_count_stop_onplay)
      {
        if (touch_count == press_count_stop_onplay)
        {
          draw_num(0, 'C');
          draw_stop();
        }
        fn_cmd_touch = 's';
        return;
      }


    }



    //-------------------------//begin PLAY----WWW----------------
#ifdef ESP32

    //-------------------------//begin PLAY-MP3-------------------
#ifdef pin_sd
    if (touch_count >= press_count_play_mp3 && !play_mp3) //do play
    {
      if (touch_count == press_count_play_mp3) draw_num(0, 'C');
      draw_play_mp3();

      fn_cmd_touch = 'M';
      return;
    }

#endif //pin_sd mp3



    //-------------------------//begin PLAY-FM-------------------
#if defined (fmSi4703)|| defined (fmRDA5807)
    if (touch_count >= press_count_play_fm && !play_fm) //do play
    {
      if (touch_count == press_count_play_fm) draw_num(0, 'C');
      draw_play_fm();

      fn_cmd_touch = 'F';

      return;
    }
#endif //si


    if (touch_count >= press_count_play && !play) //do play
    {
      if (touch_count == press_count_play) draw_num(0, 'C');
      draw_play();

      fn_cmd_touch = 'P';

      return;
    }

#endif //-----------32




#ifdef ESP8266
    //-------------------------//begin PLAY-FM-------------------
#if defined (fmSi4703)|| defined (fmRDA5807)
    if (touch_count >= press_count_play_fm && !play_fm) //do play
    {
      if (touch_count == press_count_play_fm) draw_num(0, 'C');
      draw_play_fm();

      fn_cmd_touch = 'F';

      return;
    }
#endif //si
#endif //8266


    //---------------------------------prev----------------
    if (touch_count >= press_count_prev)
    {
      if (touch_count == press_count_prev) draw_num(0, 'C');

      draw_prev();
      fn_cmd_touch = 'p';
      return;

    }


    //------------------------next----------------------
    if (touch_count >= press_count_next) //или экран или станция
    {

      if (touch_count == press_count_next) draw_num(0, 'C');

      draw_next();
      fn_cmd_touch = 'n';
      return;
    }


    //-------------------------one click-------------------------

    if (touch_count >= press_count_click) //
    {
#if defined( pin_btn_No_touch)
      if (touch_count == press_count_click) draw_num(0, 'C');
      fn_cmd_touch = do_onclick();
      Serial.print(F("do_onclick==="));
      Serial.println(fn_cmd_touch);
#endif //pin_btn_No_touch
      return;
    }



  }  ///отпущена--------------------------------------------------

  if (fn_cmd_touch == 'n') fn_cmd = "next";
  if (fn_cmd_touch == 'p') fn_cmd = "prev";
  if (fn_cmd_touch == 'f') fn_cmd = "fav";

  if (fn_cmd_touch == 'P') {
    fn_cmd = "play";
    //  timer_stop();
  }
  if (fn_cmd_touch == 'F') {
    fn_cmd = "play_fm";
    //   timer_stop();
  }

  if (fn_cmd_touch == 'M') {
    fn_cmd = "play_mp3";
    //   timer_stop();
  }

  if (fn_cmd_touch == 's') {
    if (play)    fn_cmd = "stop";
    if (play_fm)    fn_cmd = "stop_fm";
    if (play_mp3)    fn_cmd = "stop_mp3";
    //   timer_stop();
  }

  if (touch_count > 0 && fn_cmd != "")
  {
    do_fn_btn_ir();
    Serial.print(F("fn_cmd_touch=")); Serial.println(fn_cmd_touch);
    fn_cmd = "";
    if (play || play_fm || play_mp3) draw_num(0, 'C');

    if (touch_or_btn == 1)    t_next_touchRead = millis() + 2000; //для сенсорной пауза



  }

  touch_count = 0;
  fn_cmd_touch = '0';


}



//======================================================================================
//                    do_onclick
//======================================================================================
#if defined( pin_btn_No_touch)
char do_onclick()
{
  char c = '0';
#if defined(pin_btn_onclick_noplay)
  if (!play && !play_fm && !play_mp3)
  {
    //next prev play play_fm play_mp3
    if (pin_btn_onclick_noplay == "next")c =   'n';
    if (pin_btn_onclick_noplay == "prev")c =    'p';
    if (pin_btn_onclick_noplay == "play")c =    'P';
    if (pin_btn_onclick_noplay == "play_fm")c =   'F';
    if (pin_btn_onclick_noplay == "play_mp3")c =   'M';
  }
#endif //pin_btn_onclick_noplay
  //WWW
#if defined (ESP32) && defined(pin_btn_onclick_onplay)
  if (play)
  {
    //stop next prev fav
    if (pin_btn_onclick_onplay == "next")c =    'n';
    if (pin_btn_onclick_onplay == "prev")c =    'p';
    if (pin_btn_onclick_onplay == "stop")c =    's';
    if (pin_btn_onclick_onplay == "fav")c =   'f';
  }
#endif //esp32
  //FM
#if (defined(fmRDA5807) ||defined(fmSi4703))&& defined(pin_btn_onclick_onplay_fm)
  if (play_fm)
  {
    //stop next prev fav
    if (pin_btn_onclick_onplay_fm == "next")c =    'n';
    if (pin_btn_onclick_onplay_fm == "prev")c =    'p';
    if (pin_btn_onclick_onplay_fm == "stop")c =    's';
    if (pin_btn_onclick_onplay_fm == "fav")c =   'f';
  }
#endif //fm
#if defined(ESP32) &&defined(pin_sd)&& defined(pin_btn_onclick_onplay_mp3)
  if (play_mp3)
  {
    //stop next prev fav
    if (pin_btn_onclick_onplay_mp3 == "next")c =    'n';
    if (pin_btn_onclick_onplay_mp3 == "prev")c =    'p';
    if (pin_btn_onclick_onplay_mp3 == "stop")c =    's';
    //   if (pin_btn_onclick_onplay_mp3 == "fav")c =   'f';
  }
#endif //esp sd
  //next prev play play_fm play_mp3 fav
  if (c == 'n') draw_next();
  if (c == 'p') draw_prev();
#if defined (ESP32)
  if (c == 'P') draw_play();
#endif
#if defined (fmSi4703)|| defined (fmRDA5807)
  if (c == 'F') draw_play_fm();
#endif

#if defined (ESP32)&& defined (pin_sd)
  if (c == 'M') draw_play_mp3();
#endif
  if (c == 's') draw_stop();

  return c;
}

#endif //#if defined( pin_btn_No_touch)


#endif //button




//======================================================================
//              Print64
//=====================================================================

void Print64(uint64_t num) {

  char rev[128];
  char *p = rev + 1;

  while (num > 0) {
    *p++ = '0' + ( num % 10);
    num /= 10;
  }
  p--;
  /*Print the number which is now in reverse*/
  while (p > rev) {
    Serial.print(*p--);
  }
  Serial.println();
}







#if defined( pin_pir) //&& defined( pin_led)
//================================================================================================
//                do_read_pir
//================================================================================================
void do_read_pir()
{
  static uint32_t t_motion = 0;
  static uint32_t  t_next_read = 0;


  if (t_next_read > millis()) return;

  int val = analogRead(pin_pir);
  if ( val > 1000)
  {
    t_motion = millis() + t_pir_period;

#ifdef debug
    Serial.print(F("pir HIGH="));
    Serial.println(val);
#endif
#ifdef pin_led
    do_led(led_val);
#endif

    t_next_read = millis() + 10000;


#ifdef pin_pir_out_hight
    digitalWrite(pin_pir_out_hight, HIGH);
#endif

#ifdef pin_pir_out_low
    pinMode(pin_pir_out_low, LOW);
#endif



    return;
  }

  if (t_pir_period > 0 && millis() > t_motion)
  {
#ifdef pin_led
    do_led(0);
#endif

#ifdef pin_pir_out_hight
    digitalWrite(pin_pir_out_hight, LOW);
#endif


#ifdef pin_pir_out_low
    pinMode(pin_pir_out_low, HIGH);
#endif

  }

  t_next_read = millis() + 1000;



}
#endif //pin_pir






//==================================================================================================
//==================================================================================================
//                      do_read_ir
//==================================================================================================
//==================================================================================================

#if defined (pin_ir)
void do_read_ir()
{
  static time_t next_time = 0;
  unsigned long code_ir_tmp;
  static int bt1 = 99, bt2 = 99, bt3 = 99, btn = 0;
  static unsigned long t_start_bt1 = 0;

  fn_cmd = "";



  if (millis() < next_time)
  {

    //   if ( irrecv.decode( &results ))
    irrecv.resume(); // принимаем следующую команду
    return;
  }




  if (btn != 0 && millis() - t_start_bt1 > 10000 && !scr_timer) // канал стираю
  {
    bt1 = 99, bt2 = 99, bt3 = 99, btn = 0;
    draw_num(0, 'C');
  }

  //IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII
  if ( irrecv.decode( &results ))
  {
    // если данные пришли

    code_ir_tmp = results.value;
    irrecv.resume(); // принимаем следующую команду
#ifdef debug
    Serial.print("ir code=");
    Serial.println(code_ir_tmp);
#endif

    if (id_ir != "")
    {
      byte len = id_ir.length();
      String s = String(code_ir_tmp);
      if (id_ir != s.substring(0, len))
      {
#ifdef debug
        Serial.println("id_ir=>return");
#endif
        //        irrecv.resume(); // принимаем следующую команду
        next_time = millis() + t_ir_sleep_short;
        return;
      }
    }

    //ищу код в массиве
    for (int i = 0; i < fn_count; i++)
    {
      if (code[i] != code_ir_tmp)
      {
        //     Serial.println("code[i] !=code_ir_tmp");
        //     Serial.println(code[i]); Serial.println(code_ir_tmp);
        continue; //не нашёл код
      }
      //нашел
      fn_cmd = fn[i];
#ifdef debug
      Serial.print("find ir code===");
      Serial.println(fn_cmd);
#endif

      break;
    }
    /*
      #ifdef debug
        Serial.print("play=");
        Serial.println(play);
        Serial.print("play_mp3=");
        Serial.println(play_mp3);
        Serial.print("play_fm=");
        Serial.println(play_fm);
      #endif
    */



#if defined (pin_sd)&&defined (ESP32)
    //SCR  PLAY--SCR  PLAY--SCR  PLAY--SCR  PLAY--SCR  PLAY--SCR  PLAY--SCR  PLAY--SCR  PLAY--
    // ------MP3-----------MP3------MP3-----------MP3------MP3-----------MP3------MP3-------
    if (play_mp3 && fn_cmd != "")
    {
      if (fn_cmd == "play_mp3")
      {
        if (btn == 0)
        {
          if (t_timer == 0)fn_cmd = "stop_mp3"; //STOP если канал не выбран и нет таймера
          if (t_timer != 0) //, иначе стоп таймер
          {
            fn_cmd = "";
            timer_stop();
            Serial.print("TIMER stop");
          }

        }


        if (btn > 0 && track_find != curr_track) //выбор канала
          track_find = btn;


      } //e fn_cmd == "play_MP3"

      //TIMER start------   TIMER start------------TIMER start
      if (fn_cmd == "vl-" && btn > 0)
      {
        timer_start(btn, 1);
        bt1 = 99, bt2 = 99, bt3 = 99, btn = 0;
        fn_cmd = "";
      }


      if (fn_cmd == "fav")     fn_cmd = "mp3_random";

    }
    //E play-MP3---------------E play-MP3----------------E play-MP3---------------E play-MP3

#endif //sd




    //SCR  PLAY--SCR  PLAY--SCR  PLAY--SCR  PLAY--SCR  PLAY--SCR  PLAY--SCR  PLAY--SCR  PLAY--
    // ------FM-----------FM------FM-----------FM------FM-----------FM------FM-----------FM------
    if (play_fm && fn_cmd != "")
    {
      if (fn_cmd == "play_fm")
      {
        if (btn == 0)
        {
          if (t_timer == 0)fn_cmd = "stop_fm"; //STOP если канал не выбран и нет таймера
          if (t_timer != 0) //, иначе стоп таймер
          {
            fn_cmd = "";
            timer_stop();
            Serial.print("TIMER stop");
          }

        }

        else //btn!=0
          if (ch_fm != btn) //выбор канала
            ch_fm = btn;
          else fn_cmd = ""; //остаюсь на канале


      } //e fn_cmd == "play_fm"

      //TIMER start------   TIMER start------------TIMER start
      if (fn_cmd == "vl-" && btn > 0)
      {
        timer_start(btn, 1);
        bt1 = 99, bt2 = 99, bt3 = 99, btn = 0;
        fn_cmd = "";
      }

    }
    //E play-FM---------------E play-FM----------------E play-FM---------------E play-FM




    //SCR  PLAY--------SCR  PLAY---------------SCR  PLAY---------------SCR  PLAY--------------------
    // ------WWW-----------WWW------WWW-----------WWW------WWW-----------WWW------WWW
    if (play && fn_cmd != "")
    {
      if (fn_cmd == "play")
      {
        if (btn == 0)
        {
          if (t_timer == 0)fn_cmd = "stop"; //STOP если канал не выбран и нет таймера
          if (t_timer != 0) //, иначе стоп таймер
          {
            fn_cmd = "";
            timer_stop();
            Serial.print("TIMER stop");
          }

        }


        if (btn > 0 && ch != btn) //выбор канала
          ch_fnd = btn;


      }

      //TIMER start------   TIMER start------------TIMER start
      if (fn_cmd == "vl-" && btn > 0)
      {
        timer_start(btn, 0);
        bt1 = 99, bt2 = 99, bt3 = 99, btn = 0;
        fn_cmd = "";
      }

    }
    //E play--WWW---------------E play--WWW---------------------E play--WWW-------------------




    // SCR NOT PLAY-------SCR NOT PLAY--------SCR NOT PLAY--------SCR NOT PLAY-------
    // SCR NOT PLAY-------SCR NOT PLAY--------SCR NOT PLAY--------SCR NOT PLAY-------
    //----------WWW-----------------------WWW-----------------------WWW-----------------------WWW



    if (!play && !play_fm && !play_mp3 && fn_cmd != "")
    {
#ifdef debug
      Serial.print(F("btn=")); Serial.println(btn);
#endif
      //TIMER ------ SCR  ttttttttttttttttttttttttttttttttttt

      if (fn_cmd == "fav" ) //in-out scr_timer
      {
        fn_cmd = "timer";
      }
      else if (( fn_cmd == "play" || fn_cmd == "play_mp3" || fn_cmd == "play_fm") && scr_timer)
        fn_cmd = "timer_start";

      //-------------E timer
      else

        if (fn_cmd == "play" && btn > 0) //запуск выбранного канала
        {
          if (ch != btn) //ищу канал
            ch_fnd = btn;
          //       timer_stop(); //если play сброс таймера
          // Serial.println(F("TIMER stop"));
        }

      //----------FM-----------------------FM-----------------------FM-----------------------FM

        else if (fn_cmd == "play_fm" && btn > 0) //запуск выбранного канала
        {
          if (ch_fm != btn) //ищу канал
            ch_fm = btn;
          //        timer_stop(); //если play сброс таймера
          //    Serial.println(F("TIMER FM stop"));
        }

      //-----------EE-----FM---------------------



#ifdef pin_led
        else if (fn_cmd == "vl-")
        {
          fn_cmd = "led-";

        }

        else if (fn_cmd == "vl+")
        {
          fn_cmd = "led+";
        }
#endif //pin_led



    } //E !play




    if (fn_cmd == "" || fn_cmd == "vl+" || fn_cmd == "vl-" || fn_cmd == "mute" || btn > 0 || fn_cmd == "mp3_random")
      next_time = millis() + t_ir_sleep_short;
    else
      next_time = millis() + t_ir_sleep;



    if (fn_cmd != "")
    {

      if ((fn_cmd == "next" || fn_cmd == "prev" || fn_cmd == "fav"
           || fn_cmd == "play" || fn_cmd == "play_fm" || fn_cmd == "play_mp3" || fn_cmd == "stop"
           || fn_cmd == "stop_fm" || fn_cmd == "stop_mp3")
          && (play || play_fm)
          && alarm_active == true)
      {
        Serial.println(F("IR cmd-->STOP timer alarm"));
        timer_alarm_stop();
        ch_prev_alarm_www = 0;
        ch_prev_alarm_fm = 0;
        ch_prev_alarm_mp3 = 0;
      }


      Serial.print("IR cmd=="); Serial.println(fn_cmd);
      do_fn_btn_ir();
      bt1 = 99; bt2 = 99; bt3 = 99; btn = 0;
      if (play || play_fm || play_mp3) draw_num(0, 'C');
      irrecv.resume(); // принимаем следующую команду
      fn_cmd = "";
      return;
    }

    //---------BTN---------BTN---------BTN---------BTN---------BTN---------BTN---------BTN---------BTN---------
    //не нашел в функц ищу в кнопках 0-9

    if (fn_cmd == "")
    { //b
      for (byte i = 0; i < 10; i++)
      {
        if (code_btn[i] != code_ir_tmp) continue; //не нашёл код
        Serial.print("bt=");
        Serial.println(i);

        if (!scr_timer && bt1 == 99 && i == 0 && !play && !play_fm && !play_mp3 && scr_show_count > 1) //stop around scr
        {
          scr_round = !scr_round;
          draw_stop_round();
          break;
        }

#ifdef ESP32
        if (bt1 == 99 && i == 0 && play) //откл. показ группы песни
        {
#ifndef no_tft
          draw_no_song();
#endif
          break;
        }
#endif
        if (!scr_timer)
        {

          if (bt1 == 99)
          {
            bt1 = i;
            t_start_bt1 = millis();
          }
          else if (bt2 == 99) bt2 = i;
          else if (bt3 == 99) bt3 = i;


          if (bt1 != 99)
          {

            if (bt3 != 99) btn = bt1 * 100 + bt2 * 10 + bt3;
            else if (bt2 != 99) btn = bt1 * 10 + bt2;
            else

              btn = bt1;
          }

        }

        if (!scr_timer)      draw_num(btn, 'C');

        //-----------------------BUTT TIMER
        if (scr_timer && i >= 0) //number send
        {

          fn_cmd = "b" + String(i);
#ifdef debug
          Serial.print(F("TIMER btn=")); Serial.println(i);
#endif
          bt1 = 99, bt2 = 99, bt3 = 99, btn = 0;

          do_fn_btn_ir();
        }



#ifdef debug
        Serial.print("bt1=");
        Serial.println(bt1);
        Serial.print("bt2=");
        Serial.println(bt2);
        Serial.print("bt3=");
        Serial.println(bt3);
        Serial.print("btn=");
        Serial.println(btn);
#endif





        break;//нашел

      }



    } //e





    fn_cmd = "";
    //  irrecv.resume(); // принимаем следующую команду

  }



}

#endif //esp32 + ir  
