#if defined (fmSi4703)|| defined (fmRDA5807) || defined (fmSi4703_lib2)


bool do_select_num_fm(byte num_find)
{


  String filename = "/fm.txt";

#ifdef pin_sd
#ifdef debug
  Serial.print("SD ");
  Serial.println(filename);
#endif
  File f = SD.open( filename, FILE_READ);  // or, file handle reference for SD library
#else
#ifdef debug
  Serial.print("Spiffs ");
  Serial.println(filename);
#endif
  fs::File f = SPIFFS.open( filename, "r");    // File handle reference for SPIFFS
#endif


  if (!f) {
    Serial.print(filename);
    Serial.println(F(" no open,create"));
    return false;
  }
  //----------------------------
  String name = "";
  int fr = 0;
  byte vol = 0;
  bool find_num_fm = false;
  bool find_ch_fm = false;
  byte num_tmp = 254;

  const byte y_begin = 75;
  int x = 133;

#ifdef tft_320_240
  const byte y_step = 14;
  const byte x_step = 93;
  const int w = 320;
  const int h = 200;
  tft.setFreeFont(v6);
#endif
  //--------------------------
#ifdef tft_480_320
  tft.setFreeFont(v9mono);
  const byte y_step = 18;
  const byte x_step = 173;
  const int w = 480;
  const int h = 250;

#endif
  byte y = y_begin + 14;


  String str;
  ch_next_fm = 254;
  ch_prev_fm = 0;

  static int x_ch_old = 0;
  static int y_ch_old = 0;
  static byte ch_old_fm = 0;
  static int x_ch = 0;
  static int y_ch = 0;


#ifdef debug
  Serial.println(F("--------------------------------------fm.txt"));
#endif
  //---------------------------------------------------------
  while (f.available()) {
    yield();
    str = f.readStringUntil('\n');
    //Serial.println(str);
    //if (str.indexOf("{")>=0) (Serial.print("{=");Serial.println(str.indexOf("{")));
    if (str.indexOf("{") == 0 && refresh_all)
    {
      do_fav_ch(str, 1); //favor fm

    }
    if (str.indexOf("#") != 0) continue;
    int tz = 0;
    int tz2 = 0;
    find_num_fm = false;
    //tz=str.indexOf("#");
    //Serial.println("#");
    //Serial.println(tz);

    name = "";
    fr = 0;

    //num-------------------------------------------num-------------------
    tz2 = str.indexOf(";", tz + 1);
    if (tz > 0)
      tz2 = str.indexOf(";", tz + 1);
    if (tz2 > 0)
    {
      num_tmp = str.substring(tz + 1, tz2).toInt();
      //Serial.println(num_tmp);
      //max
      if (ch_max_fm < num_tmp) ch_max_fm = num_tmp;
      //min
      if (ch_min_fm > num_tmp) ch_min_fm = num_tmp;
      //next
      if (num_tmp > num_find) //в случае неудачи будет след канал
      {
        if (ch_next_fm > num_tmp)
        {
          ch_next_fm = num_tmp;

        }
      }

      //prev
      if (num_tmp < num_find) //в случае неудачи будет след канал
      {
        if (ch_prev_fm < num_tmp)
        {
          ch_prev_fm = num_tmp;

        }
      }


      //ffffffffffffff
      if (num_tmp == num_find)
      {
        ch_fm = num_tmp;
#ifdef debug
        Serial.print("find-------->");
        Serial.println(num_tmp);
#endif

        find_num_fm = true;
      }
      //name --------------------------------name--------------------------------
      tz = tz2;
      tz2 = str.indexOf(";", tz + 1);
      if (tz2 > 0)
      {
        //Serial.println(str.substring(tz+1,tz2));
        name = str.substring(tz + 1, tz2);
      }
      else
      {
        Serial.println(F("not found name fm"));
        find_num_fm = false;
        continue;
      }
      ///fr ----------------------------------------FReq----------------------------
      tz = tz2;
      tz2 = str.indexOf(";", tz + 1);
      if (tz2 > 0)
      {
        //Serial.println(str.substring(tz+1,tz2));
        String str_fr = str.substring(tz + 1, tz2);
        fr = str_fr.toInt();
        if (fr > 760 && fr < 1080); //оставляю
        else
        {
          Serial.println(F("FM freq err"));
          find_num_fm = false;
          continue;
        }
      }
      else
      {
        Serial.println(F("FM freq not found"));
        find_num_fm = false;
        continue;
      }

      //volume --------------------------------------vol------------
      tz = tz2;
      tz2 = str.indexOf(";", tz + 1);
      if (tz2 > 0 && !vol_rotate)
      {
        String str_vol = str.substring(tz + 1, tz2);



        if (str_vol.toInt() > 0 && str_vol.toInt() < 16 && find_num_fm)
        {
          vol = str_vol.toInt();  //оставляю
#ifdef debug
          Serial.print(F("vol =")); Serial.println(volume_fm);
#endif
        }

      }


      //2 колонки не заполнены
      //----------------------------------------------------------------------------------
      if (x < w && refresh_all) //2 колонки не заполнены

      {
        tft.setCursor(x, y);
        tft.setTextColor(c_channel);
        tft.print(num_tmp);
        if (num_tmp < 10) tft.print(" ");
        tft.print(name);
        //#ifdef debug
        //        Serial.print(num_tmp);
        //        Serial.println(name);
        //#endif
      }

      //ffffffffffffffffffffffffffffffffffffffffffffffffffff
      if (find_num_fm)
      {
        x_ch = x;
        y_ch = y;

        if (x_ch_old != 0 & y_ch_old != 0)
        {
          tft.setCursor(x_ch_old, y_ch_old);
          tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
          tft.print(ch_old_fm);
          tft.drawFastHLine(x_ch_old, y_ch_old + 3, x_step - 10, TFT_BLACK) ;
        }
        tft.setCursor(x_ch, y_ch);
        tft.setTextColor(TFT_YELLOW, TFT_RED);
        tft.print(ch_fm);
        tft.drawFastHLine(x_ch, y_ch + 3, x_step - 10, c_ch_line) ;
        x_ch_old = x_ch;
        y_ch_old = y_ch;
        ch_old_fm = ch_fm;

        freq_fm = fr;
        ch_name_fm = name;
        find_ch_fm = true;

        //    if (!refresh_all) break ;//нашел, выхожу, не первый запуск
      }


    }

    y = y + y_step;

    if (y > h - 3) //очищаю под вторую колонку каналов
    {
      y = y_begin + y_step;
      x = x + x_step;
      if (x < w - 30 && refresh_all)
      {
        tft.fillRect(x, y, w - x, h - y_begin, TFT_BLACK) ; //ch
        tft.drawFastVLine(x - 1, y_begin, h - y_begin, TFT_LIGHTGREY) ; //ch
      }


    }




  }
  f.close();

  if (ch_next_fm > ch_max_fm) ch_next_fm = ch_min_fm;
  if (ch_prev_fm < ch_min_fm) ch_prev_fm = ch_max_fm;

  if (!find_ch_fm )
  {
    if (ch_old_fm != 0) ch_fm = ch_old_fm;
    else
      ch_fm = ch_min_fm;
  }
  if (vol != 0)   volume_fm = vol;
  else
    volume_fm = vol_def_fm;

#ifdef debug
  Serial.print(F("ch_fm----------->"));
  Serial.println(ch_fm);
  Serial.print(F("ch_name_fm---->"));
  Serial.println(ch_name_fm);
  Serial.print(F("fr="));
  Serial.println(freq_fm);
  Serial.print(F("vol="));
  Serial.println(volume_fm);
  Serial.print(F("ch_max_fm="));
  Serial.println(ch_max_fm);
  Serial.print(F("ch_min_fm="));
  Serial.println(ch_min_fm);
  Serial.print(F("ch_prev_fm="));
  Serial.println(ch_prev_fm);
  Serial.print(F("ch_next_fm="));
  Serial.println(ch_next_fm);
  Serial.print(F("return find_ch_fm="));
  Serial.println(find_ch_fm);
#endif
  return find_ch_fm;
}


//-------------------------------------------------------------------------
//                              fm_setVolume
//-------------------------------------------------------------------------
void fm_setVolume(byte vol)
{
#ifdef fmRDA5807
  if (vol == 0) FM.setMute(true); else  FM.setMute(false);
#endif


  FM.setVolume(vol);
#ifdef debug
  Serial.print(F("volume FM="));
  Serial.println(vol);
#endif

}


//===============================================================================================================
//                  fm_setFreq
//===============================================================================================================

void fm_setFreq(int fr)
{
#ifdef fmSi4703
  FM.setChannel(fr);
#endif
#ifdef fmSi4703_lib2
  FM.setFrequency(fr * 10);
#endif

#ifdef fmRDA5807
  //FM.setFrequency(fr*10);
  FM.setBandFrequency(FIX_BAND, fr * 10);
#endif

#ifdef debug
  Serial.print(F("fm_setFreq ="));
  Serial.println(fr);
#endif
}



#endif // fm
