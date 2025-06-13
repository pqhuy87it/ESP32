#ifdef lcd_display
//---------------------------------------------------------------------------
//                                          draw_lcd_val
//---------------------------------------------------------------------------
void draw_lcd_val(const char* val)
{
  /*
    int pr = 762;
    uint8_t hum = 88;
    int8_t t_in = 24;
    int8_t t_out = -15;
  */
  String sp = "                    ";//20
  // String sp = "********************";//20
  byte  len_txt2 = 0;
  byte  len_val = 2;
  if (lcd_val == "lcd_pr" || lcd_val == "lcd_t_out" || lcd_val == "lcd_hum_in") len_val = 3;
#ifdef hPa
  if (lcd_val == "lcd_pr") len_val = 4;
#endif

  /*
    pr = random(600, 800);
    hum = random(0, 100);
    t_in = random(5, 60);
    t_out = random(-50, 50);
  */
  if (!lcd_parse(val)) return;
  // Serial.print("lcd_val  "); Serial.println(lcd_val);



  if (lcd_txt2 != "") len_txt2 = lcd_txt2.length();

  lcd.setCursor(lcd_x, lcd_y);
  if (lcd_txt1 != "")
  {
    lcd.print(lcd_txt1);
    lcd_x = lcd_x + lcd_txt1.length();
  }

#ifdef lcd_day
  if (lcd_val == "lcd_day")
  {
    sp = sp.substring(0, len_val + len_txt2 );
    lcd.print(sp);
    lcd.setCursor(lcd_x, lcd_y);
    if (now_day < 10)lcd.print("0");
    lcd.print(now_day);
  }
#endif //lcd_day

#ifdef lcd_week
  if (lcd_val == "lcd_week")
  {
    sp = sp.substring(0, len_val + len_txt2);
    lcd.print(sp);
    lcd.setCursor(lcd_x, lcd_y);
    lcd.print(dayNames[now_week]);
  }
#endif //lcd_week


#ifdef lcd_pr
  if (lcd_val == "lcd_pr")
  {
    sp = sp.substring(0, len_val + len_txt2);
    lcd.print(sp);
    lcd.setCursor(lcd_x, lcd_y);
    if (pr_kv != 200)
      lcd.print(pr_kv);
#ifdef openw
    else if (davlenie0 != 200)   lcd.print(davlenie0);
#endif
  }
#endif //lcd_pr


#ifdef lcd_hum_in
  if (lcd_val == "lcd_hum_in")
  {
    sp = sp.substring(0, len_val + len_txt2);
    lcd.print(sp);
    lcd.setCursor(lcd_x, lcd_y);
    if (h_kv != 200)   lcd.print(h_kv);
  }
#endif //lcd_hum_in


#ifdef lcd_t_in
  if (lcd_val == "lcd_t_in")
  {
    sp = sp.substring(0, len_val + len_txt2);
    lcd.print(sp);
    lcd.setCursor(lcd_x, lcd_y);
    if (temp_kv != 200)      lcd.print(temp_kv);
  }
#endif //lcd_t_in

#ifdef lcd_t_out
  if (lcd_val == "lcd_t_out")
  {
    sp = sp.substring(0, len_val + len_txt2);
    lcd.print(sp);
    lcd.setCursor(lcd_x, lcd_y);
    if (temp_u != 200) lcd.print(temp_u);
#ifdef openw
    else   if (temperatura_ch[0] != 200) lcd.print(temperatura_ch[0]);
#endif
  }
#endif //lcd_t_out




  if (lcd_txt2 != "") lcd.print(lcd_txt2);
  /*
    #ifdef debug
    Serial.print("len_val "); Serial.println(len_val);
    Serial.print("sp.len "); Serial.println(sp.length());
    Serial.print("sp.substring ");  Serial.print("|");
    Serial.print(sp); Serial.println("|");
    #endif
  */
}




//---------------------------------------------------------------------------
//                                          lcd_parse
//---------------------------------------------------------------------------
bool  lcd_parse(String str)
{
  //--------------------------

  if (str.length() == 0) return false;
  str.trim();
#ifdef debug
  //  Serial.print("lcd_parse= "); Serial.println(str);
#endif
  bool ret = false;
  lcd_txt1 = "";
  lcd_x = -1;
  lcd_y = -1;
  lcd_txt2 = "";
  String  val = "";
  byte count_val = 0;

  int tz2 = str.indexOf(";") ;
  if (tz2 == -1)
  {
#ifdef debug
    //    Serial.println(F("';' -not found "));
#endif
    return ret;
  }
  int tz = 0;

  while (tz2 >= 0)
  {

    count_val++;
    val = str.substring(tz, tz2);

    tz = tz2 + 1;
    tz2 = str.indexOf(";", tz);

    if (count_val == 1) //text before
    {
      if (val != "")
        lcd_txt1 = val;
    }

    if (count_val == 2) //X
    {
      if (val.toInt() >= 0)
        lcd_x = val.toInt();
      else
      {
        Serial.println(F("ERR lcd_x"));
        return ret;
      }
    }
    if (count_val == 3) //Y
    {
      if (val.toInt() >= 0)
        lcd_y = val.toInt();
      else
      {
        Serial.println(F("ERR lcd_y"));
        return ret;
      }
    }
    if (count_val == 4) //text after
    {
      if (val != "")
        lcd_txt2 = val;
    }


  }

#ifdef debug
  // Serial.print("lcd_text1= "); Serial.println(lcd_txt1);
  // Serial.print("lcd_x= "); Serial.println(lcd_x);
  // Serial.print("lcd_y= "); Serial.println(lcd_y);
  // Serial.print("lcd_text2= "); Serial.println(lcd_txt2);
#endif

  ret = true;
  return ret;
}


#ifdef lcd_clock_l
void drawClock_l(byte hours, byte minutes, byte x, byte y)
{

  lcd.setCursor(x, y);
  if (lcd_txt1 != "") lcd.print(lcd_txt1);
  lcd.print(hours / 10);
  lcd.print(hours % 10);
  lcd.print(".");
  lcd.print(minutes / 10);
  lcd.print(minutes % 10);
  if (lcd_txt2 != "") lcd.print(lcd_txt2);
}
#endif //lcd_clock_l

#endif //lcd_display
