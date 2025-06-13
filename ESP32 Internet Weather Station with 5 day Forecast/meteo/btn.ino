
#ifdef pin_btn1
//======================================================================================
//                                     do_btn1()
//======================================================================================
void  do_btn1()
{
  static bool btn_hold = false;
  btn1.tick();

  //------------------------isSingle
  if (btn1.isSingle())
  {
    Serial.println("Single btn1");       // проверка на один клик
#ifdef btn1_noplay
    if (!play && !play_mp3 && !play_fm && !scr_timer) fn_cmd = btn1_noplay;
#endif
#ifdef btn1_play
    if (play) fn_cmd = btn1_play;
#endif
#ifdef btn1_play_mp3
    if (play_mp3) fn_cmd = btn1_play_mp3;
#endif
#ifdef btn1_play_fm
    if (play_fm) fn_cmd = btn1_play_fm;
#endif
#ifdef btn1_timer
    if (scr_timer) fn_cmd = btn1_timer;
#endif

  }
  //------------------------isDouble
  if (btn1.isDouble())
  {
    Serial.println("Double btn1");       // проверка на двойной клик
#ifdef btn1_dbl_noplay
    if (!play && !play_mp3 && !play_fm && !scr_timer) fn_cmd = btn1_dbl_noplay;
#endif
#ifdef btn1_dbl_play
    if (play) fn_cmd = btn1_dbl_play;
#endif
#ifdef btn1_dbl_play_mp3
    if (play_mp3) fn_cmd = btn1_dbl_play_mp3;
#endif
#ifdef btn1_dbl_play_fm
    if (play_fm) fn_cmd = btn1_dbl_play_fm;
#endif
#ifdef btn1_dbl_timer
    if (scr_timer) fn_cmd = btn1_dbl_timer;
#endif

  }

  //------------------------isTriple

  if (btn1.isTriple())
  {
    Serial.println("Triple btn1");       // проверка на тройной клик
#ifdef btn1_trpl_noplay
    if (!play && !play_mp3 && !play_fm && !scr_timer) fn_cmd = btn1_trpl_noplay;
#endif
#ifdef btn1_trpl_play
    if (play) fn_cmd = btn1_trpl_play;
#endif
#ifdef btn1_trpl_play_mp3
    if (play_mp3) fn_cmd = btn1_trpl_play_mp3;
#endif
#ifdef btn1_trpl_play_fm
    if (play_fm) fn_cmd = btn1_trpl_play_fm;
#endif
#ifdef btn1_trpl_timer
    if (scr_timer) fn_cmd = btn1_trpl_timer;
#endif

  }

  //-------------------------
  if (btn1.isRelease())
  {
    btn_hold = false;
    Serial.println("Release btn1");
    if (press_count > 0 )
    {
      do_btn_round(200); //round
      press_count = 0;
      fn_cmd_tmp = "";
    }


  }


  //---------------------------------HOLD
  if (btn1.isHold()) {
    if (!btn_hold)
    {
      Serial.println("Holding btn1");
#ifdef btn1_hld_noplay
      if (!play && !play_mp3 && !play_fm && !scr_timer) fn_cmd = btn1_hld_noplay;
#endif
#ifdef btn1_hld_play
      if (play) fn_cmd = btn1_hld_play;
#endif
#ifdef btn1_hld_play_mp3
      if (play_mp3) fn_cmd = btn1_hld_play_mp3;
#endif
#ifdef btn1_hld_play_fm
      if (play_fm) fn_cmd = btn1_hld_play_fm;
#endif

#ifdef btn1_hld_timer
      if (scr_timer) fn_cmd = btn1_hld_timer;
#endif
      btn_hold = true;
      Serial.println(fn_cmd);
    }

  }



  //---------------------------------Step
  if (btn1.isStep()) {
    String fn_tmp = "";
#ifdef btn1_hld_noplay
    if (!play && !play_mp3 && !play_fm && !scr_timer) fn_tmp = btn1_hld_noplay;
#endif
#ifdef btn1_hld_play
    if (play) fn_tmp = btn1_hld_play;
#endif
#ifdef btn1_hld_play_mp3
    if (play_mp3) fn_tmp = btn1_hld_play_mp3;
#endif
#ifdef btn1_hld_play_fm
    if (play_fm) fn_tmp = btn1_hld_play_fm;
#endif

#ifdef btn1_hld_timer
    if (scr_timer) fn_tmp = btn1_hld_timer;
#endif

    if (fn_tmp == "round")
    {
      press_count++;
      Serial.println(press_count);
      do_btn_round(press_count);
      return;
    }

  }

  if (fn_cmd != "")
  {
    do_fn_btn_ir();
    fn_cmd = "";
    fn_cmd_tmp = "";
    if (play || play_fm || play_mp3) draw_num(0, 'C');

  }

}
#endif //btn1



#ifdef pin_btn2
//======================================================================================
//                                     do_btn2()
//======================================================================================
void  do_btn2()
{
  static bool btn_hold = false;
  btn2.tick();

  //------------------------isSingle
  if (btn2.isSingle())
  {
    Serial.println("Single btn2");       // проверка на один клик
#ifdef btn2_noplay
    if (!play && !play_mp3 && !play_fm && !scr_timer) fn_cmd = btn2_noplay;
#endif
#ifdef btn2_play
    if (play) fn_cmd = btn2_play;
#endif
#ifdef btn2_play_mp3
    if (play_mp3) fn_cmd = btn2_play_mp3;
#endif
#ifdef btn2_play_fm
    if (play_fm) fn_cmd = btn2_play_fm;
#endif
#ifdef btn2_timer
    if (scr_timer) fn_cmd = btn2_timer;
#endif

  }
  //------------------------isDouble
  if (btn2.isDouble())
  {
    Serial.println("Double btn2");       // проверка на двойной клик
#ifdef btn2_dbl_noplay
    if (!play && !play_mp3 && !play_fm && !scr_timer) fn_cmd = btn2_dbl_noplay;
#endif
#ifdef btn2_dbl_play
    if (play) fn_cmd = btn2_dbl_play;
#endif
#ifdef btn2_dbl_play_mp3
    if (play_mp3) fn_cmd = btn2_dbl_play_mp3;
#endif
#ifdef btn2_dbl_play_fm
    if (play_fm) fn_cmd = btn2_dbl_play_fm;
#endif
#ifdef btn2_dbl_timer
    if (scr_timer) fn_cmd = btn2_dbl_timer;
#endif

  }

  //------------------------isTriple

  if (btn2.isTriple())
  {
    Serial.println("Triple btn2");       // проверка на тройной клик
#ifdef btn2_trpl_noplay
    if (!play && !play_mp3 && !play_fm && !scr_timer) fn_cmd = btn2_trpl_noplay;
#endif
#ifdef btn2_trpl_play
    if (play) fn_cmd = btn2_trpl_play;
#endif
#ifdef btn2_trpl_play_mp3
    if (play_mp3) fn_cmd = btn2_trpl_play_mp3;
#endif
#ifdef btn2_trpl_play_fm
    if (play_fm) fn_cmd = btn2_trpl_play_fm;
#endif
#ifdef btn2_trpl_timer
    if (scr_timer) fn_cmd = btn2_trpl_timer;
#endif

  }

  //-------------------------
  if (btn2.isRelease())
  {
    btn_hold = false;
    Serial.println("Release btn2");
    if (press_count > 0 )
    {
      do_btn_round(200); //round
      press_count = 0;
      fn_cmd_tmp = "";
    }


  }


  //---------------------------------HOLD
  if (btn2.isHold()) {
    if (!btn_hold)
    {
      Serial.println("Holding btn2");
#ifdef btn2_hld_noplay
      if (!play && !play_mp3 && !play_fm && !scr_timer) fn_cmd = btn2_hld_noplay;
#endif
#ifdef btn2_hld_play
      if (play) fn_cmd = btn2_hld_play;
#endif
#ifdef btn2_hld_play_mp3
      if (play_mp3) fn_cmd = btn2_hld_play_mp3;
#endif
#ifdef btn2_hld_play_fm
      if (play_fm) fn_cmd = btn2_hld_play_fm;
#endif

#ifdef btn2_hld_timer
      if (scr_timer) fn_cmd = btn2_hld_timer;
#endif
      btn_hold = true;
      Serial.println(fn_cmd);
    }

  }



  //---------------------------------Step
  if (btn2.isStep()) {
    String fn_tmp = "";
#ifdef btn2_hld_noplay
    if (!play && !play_mp3 && !play_fm && !scr_timer) fn_tmp = btn2_hld_noplay;
#endif
#ifdef btn2_hld_play
    if (play) fn_tmp = btn2_hld_play;
#endif
#ifdef btn2_hld_play_mp3
    if (play_mp3) fn_tmp = btn2_hld_play_mp3;
#endif
#ifdef btn2_hld_play_fm
    if (play_fm) fn_tmp = btn2_hld_play_fm;
#endif

#ifdef btn2_hld_timer
    if (scr_timer) fn_tmp = btn2_hld_timer;
#endif

    if (fn_tmp == "round")
    {
      press_count++;
      Serial.println(press_count);
      do_btn_round(press_count);
      return;
    }

  }

  if (fn_cmd != "")
  {
    do_fn_btn_ir();
    fn_cmd = "";
    fn_cmd_tmp = "";
    if (play || play_fm || play_mp3) draw_num(0, 'C');

  }

}
#endif //btn2





#ifdef pin_btn3
//======================================================================================
//                                     do_btn3()
//======================================================================================
void  do_btn3()
{
  static bool btn_hold = false;
  btn3.tick();

  //------------------------isSingle
  if (btn3.isSingle())
  {
    Serial.println("Single btn3");       // проверка на один клик
#ifdef btn3_noplay
    if (!play && !play_mp3 && !play_fm && !scr_timer) fn_cmd = btn3_noplay;
#endif
#ifdef btn3_play
    if (play) fn_cmd = btn3_play;
#endif
#ifdef btn3_play_mp3
    if (play_mp3) fn_cmd = btn3_play_mp3;
#endif
#ifdef btn3_play_fm
    if (play_fm) fn_cmd = btn3_play_fm;
#endif
#ifdef btn3_timer
    if (scr_timer) fn_cmd = btn3_timer;
#endif

  }
  //------------------------isDouble
  if (btn3.isDouble())
  {
    Serial.println("Double btn3");       // проверка на двойной клик
#ifdef btn3_dbl_noplay
    if (!play && !play_mp3 && !play_fm && !scr_timer) fn_cmd = btn3_dbl_noplay;
#endif
#ifdef btn3_dbl_play
    if (play) fn_cmd = btn3_dbl_play;
#endif
#ifdef btn3_dbl_play_mp3
    if (play_mp3) fn_cmd = btn3_dbl_play_mp3;
#endif
#ifdef btn3_dbl_play_fm
    if (play_fm) fn_cmd = btn3_dbl_play_fm;
#endif
#ifdef btn3_dbl_timer
    if (scr_timer) fn_cmd = btn3_dbl_timer;
#endif

  }

  //------------------------isTriple

  if (btn3.isTriple())
  {
    Serial.println("Triple btn3");       // проверка на тройной клик
#ifdef btn3_trpl_noplay
    if (!play && !play_mp3 && !play_fm && !scr_timer) fn_cmd = btn3_trpl_noplay;
#endif
#ifdef btn3_trpl_play
    if (play) fn_cmd = btn3_trpl_play;
#endif
#ifdef btn3_trpl_play_mp3
    if (play_mp3) fn_cmd = btn3_trpl_play_mp3;
#endif
#ifdef btn3_trpl_play_fm
    if (play_fm) fn_cmd = btn3_trpl_play_fm;
#endif
#ifdef btn3_trpl_timer
    if (scr_timer) fn_cmd = btn3_trpl_timer;
#endif

  }

  //-------------------------
  if (btn3.isRelease())
  {
    btn_hold = false;
    Serial.println("Release btn3");
    if (press_count > 0 )
    {
      do_btn_round(200); //round
      press_count = 0;
      fn_cmd_tmp = "";
    }


  }


  //---------------------------------HOLD
  if (btn3.isHold()) {
    if (!btn_hold)
    {
      Serial.println("Holding btn3");
#ifdef btn3_hld_noplay
      if (!play && !play_mp3 && !play_fm && !scr_timer) fn_cmd = btn3_hld_noplay;
#endif
#ifdef btn3_hld_play
      if (play) fn_cmd = btn3_hld_play;
#endif
#ifdef btn3_hld_play_mp3
      if (play_mp3) fn_cmd = btn3_hld_play_mp3;
#endif
#ifdef btn3_hld_play_fm
      if (play_fm) fn_cmd = btn3_hld_play_fm;
#endif

#ifdef btn3_hld_timer
      if (scr_timer) fn_cmd = btn3_hld_timer;
#endif
      btn_hold = true;
      Serial.println(fn_cmd);
    }

  }



  //---------------------------------Step
  if (btn3.isStep()) {
    String fn_tmp = "";
#ifdef btn3_hld_noplay
    if (!play && !play_mp3 && !play_fm && !scr_timer) fn_tmp = btn3_hld_noplay;
#endif
#ifdef btn3_hld_play
    if (play) fn_tmp = btn3_hld_play;
#endif
#ifdef btn3_hld_play_mp3
    if (play_mp3) fn_tmp = btn3_hld_play_mp3;
#endif
#ifdef btn3_hld_play_fm
    if (play_fm) fn_tmp = btn3_hld_play_fm;
#endif

#ifdef btn3_hld_timer
    if (scr_timer) fn_tmp = btn3_hld_timer;
#endif

    if (fn_tmp == "round")
    {
      press_count++;
      Serial.println(press_count);
      do_btn_round(press_count);
      return;
    }

  }

  if (fn_cmd != "")
  {
    do_fn_btn_ir();
    fn_cmd = "";
    fn_cmd_tmp = "";
    if (play || play_fm || play_mp3) draw_num(0, 'C');

  }

}
#endif //btn3


#ifdef pin_btn4
//======================================================================================
//                                     do_btn4()
//======================================================================================
void  do_btn4()
{
  static bool btn_hold = false;
  btn4.tick();

  //------------------------isSingle
  if (btn4.isSingle())
  {
    Serial.println("Single btn4");       // проверка на один клик
#ifdef btn4_noplay
    if (!play && !play_mp3 && !play_fm && !scr_timer) fn_cmd = btn4_noplay;
#endif
#ifdef btn4_play
    if (play) fn_cmd = btn4_play;
#endif
#ifdef btn4_play_mp3
    if (play_mp3) fn_cmd = btn4_play_mp3;
#endif
#ifdef btn4_play_fm
    if (play_fm) fn_cmd = btn4_play_fm;
#endif
#ifdef btn4_timer
    if (scr_timer) fn_cmd = btn4_timer;
#endif

  }
  //------------------------isDouble
  if (btn4.isDouble())
  {
    Serial.println("Double btn4");       // проверка на двойной клик
#ifdef btn4_dbl_noplay
    if (!play && !play_mp3 && !play_fm && !scr_timer) fn_cmd = btn4_dbl_noplay;
#endif
#ifdef btn4_dbl_play
    if (play) fn_cmd = btn4_dbl_play;
#endif
#ifdef btn4_dbl_play_mp3
    if (play_mp3) fn_cmd = btn4_dbl_play_mp3;
#endif
#ifdef btn4_dbl_play_fm
    if (play_fm) fn_cmd = btn4_dbl_play_fm;
#endif
#ifdef btn4_dbl_timer
    if (scr_timer) fn_cmd = btn4_dbl_timer;
#endif

  }

  //------------------------isTriple

  if (btn4.isTriple())
  {
    Serial.println("Triple btn4");       // проверка на тройной клик
#ifdef btn4_trpl_noplay
    if (!play && !play_mp3 && !play_fm && !scr_timer) fn_cmd = btn4_trpl_noplay;
#endif
#ifdef btn4_trpl_play
    if (play) fn_cmd = btn4_trpl_play;
#endif
#ifdef btn4_trpl_play_mp3
    if (play_mp3) fn_cmd = btn4_trpl_play_mp3;
#endif
#ifdef btn4_trpl_play_fm
    if (play_fm) fn_cmd = btn4_trpl_play_fm;
#endif
#ifdef btn4_trpl_timer
    if (scr_timer) fn_cmd = btn4_trpl_timer;
#endif

  }

  //-------------------------
  if (btn4.isRelease())
  {
    btn_hold = false;
    Serial.println("Release btn4");
    if (press_count > 0 )
    {
      do_btn_round(200); //round
      press_count = 0;
      fn_cmd_tmp = "";
    }


  }


  //---------------------------------HOLD
  if (btn4.isHold()) {
    if (!btn_hold)
    {
      Serial.println("Holding btn4");
#ifdef btn4_hld_noplay
      if (!play && !play_mp3 && !play_fm && !scr_timer) fn_cmd = btn4_hld_noplay;
#endif
#ifdef btn4_hld_play
      if (play) fn_cmd = btn4_hld_play;
#endif
#ifdef btn4_hld_play_mp3
      if (play_mp3) fn_cmd = btn4_hld_play_mp3;
#endif
#ifdef btn4_hld_play_fm
      if (play_fm) fn_cmd = btn4_hld_play_fm;
#endif

#ifdef btn4_hld_timer
      if (scr_timer) fn_cmd = btn4_hld_timer;
#endif
      btn_hold = true;
      Serial.println(fn_cmd);
    }

  }



  //---------------------------------Step
  if (btn4.isStep()) {
    String fn_tmp = "";
#ifdef btn4_hld_noplay
    if (!play && !play_mp3 && !play_fm && !scr_timer) fn_tmp = btn4_hld_noplay;
#endif
#ifdef btn4_hld_play
    if (play) fn_tmp = btn4_hld_play;
#endif
#ifdef btn4_hld_play_mp3
    if (play_mp3) fn_tmp = btn4_hld_play_mp3;
#endif
#ifdef btn4_hld_play_fm
    if (play_fm) fn_tmp = btn4_hld_play_fm;
#endif

#ifdef btn4_hld_timer
    if (scr_timer) fn_tmp = btn4_hld_timer;
#endif

    if (fn_tmp == "round")
    {
      press_count++;
      Serial.println(press_count);
      do_btn_round(press_count);
      return;
    }

  }

  if (fn_cmd != "")
  {
    do_fn_btn_ir();
    fn_cmd = "";
    fn_cmd_tmp = "";
    if (play || play_fm || play_mp3) draw_num(0, 'C');

  }

}
#endif //btn4

#if defined( pin_btn1)|| defined( pin_btn2)||defined( pin_btn3)|| defined( pin_btn4)
//======================================================================================
//                                     do_btn_round()
//======================================================================================

void do_btn_round(uint8_t count) //1-esp32 toch; 2-butt
{


  static uint32_t t_next_touchRead = 0;
  static String cmd_tmp = "";

  if (count == 200) //отпущена
  {
    fn_cmd = cmd_tmp;
    cmd_tmp = "";
    return;
  }
  if (t_next_touchRead > millis()) return;
  t_next_touchRead = millis() + 300;
  Serial.print("cmd_tmp="); Serial.println(cmd_tmp);
  Serial.print("count="); Serial.println(count);
  byte delta = 0;
  //static byte touch_count = 0;
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
  byte press_count_esc_tmp;


  if ((play || play_fm || play_mp3) )press_count_esc_tmp = press_count_esc_onplay;
  else
    press_count_esc_tmp = press_count_esc;


  //-----------------------------------------------------------------------------
  if (count >= press_count_esc_tmp) //отмена
  {
    if (count == press_count_esc_tmp)
    {
      draw_num(0, 'C');
      cmd_tmp = "";
      press_count = 0;
    }
    return;
  }







  //STOP on play
  if ((play || play_fm || play_mp3) ) //stop
  {

    if (count >= press_count_stop_onplay)
    {
      if (count == press_count_stop_onplay)
      {
        draw_num(0, 'C');
        draw_stop();
      }
      if (play)    cmd_tmp = "stop";
      if (play_fm)    cmd_tmp = "stop_fm";
      if (play_mp3)    cmd_tmp = "stop_mp3";

      return;
    }


  }

#ifdef ESP32

  //-------------------------//begin PLAY-MP3-------------------
#ifdef pin_sd
  if (count >= press_count_play_mp3 && !play_mp3) //do play
  {
    if (count == press_count_play_mp3) draw_num(0, 'C');
    draw_play_mp3();

    cmd_tmp =  "play_mp3";
    return;
  }

#endif //pin_sd mp3



  //-------------------------//begin PLAY-FM-------------------
#if defined (fmSi4703)|| defined (fmRDA5807)
  if (count >= press_count_play_fm && !play_fm) //do play
  {
    if (count == press_count_play_fm) draw_num(0, 'C');
    draw_play_fm();

    cmd_tmp =  "play_fm";

    return;
  }
#endif //si


  if (count >= press_count_play && !play) //do play
  {
    if (count == press_count_play) draw_num(0, 'C');
    draw_play();

    cmd_tmp = "play";

    return;
  }

#endif //-----------32




#ifdef ESP8266
  //-------------------------//begin PLAY-FM-------------------
#if defined (fmSi4703)|| defined (fmRDA5807)
  if (count >= press_count_play_fm && !play_fm) //do play
  {
    if (count == press_count_play_fm) draw_num(0, 'C');
    draw_play_fm();

    cmd_tmp =  "play_fm";

    return;
  }
#endif //si
#endif //8266


  //---------------------------------prev----------------
  if (count >= press_count_prev)
  {
    if (count == press_count_prev) draw_num(0, 'C');

    draw_prev();
    cmd_tmp = "prev";
    return;

  }


  //------------------------next----------------------
  if (count >= press_count_next) //или экран или станция
  {

    if (count == press_count_next) draw_num(0, 'C');

    draw_next();
    cmd_tmp = "next";
    return;
  }


}
#endif //btn all
