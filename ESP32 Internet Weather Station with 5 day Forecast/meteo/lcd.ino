#ifdef lcd_display
void draw_lcd()
{

  if  (play_mp3 || play || nigth)
  {
    if (lcd_on)
    {
      lcd.setBacklight(0);
      lcd_on = false;
    }
    return;
  }

  if (!lcd_on)
  {
    lcd.setBacklight(1);
    lcd_on = true;
  }

  static byte last_min = 99; //чтобы не перирис минуты
  static byte last_h = 99;
  if (last_min == now_min && ! refresh_all) return;
  if (refresh_all) lcd.clear();

  if (sync_t)
  {
#if defined (lcd_clock_b)||defined (lcd_clock_m)||defined (lcd_clock_l)
    lcd_time();
#endif


#ifdef lcd_day
    lcd_val = "lcd_day";
    draw_lcd_val(lcd_day);
    //  draw_lcd_day();
#endif //lcd_day



#ifdef lcd_week
    lcd_val = "lcd_week";
    draw_lcd_val(lcd_week);
#endif //lcd_week
  }
  //ppppppppppppppppppppppppppp
#ifdef lcd_pr
  lcd_val = "lcd_pr";
  draw_lcd_val(lcd_pr);
#endif

  //hhhhhhhhhhhhhhhhhhhhhhhhhh
#ifdef lcd_hum_in
  lcd_val = "lcd_hum_in";
  draw_lcd_val(lcd_hum_in);
#endif


#ifdef lcd_t_in
  lcd_val = "lcd_t_in";
  draw_lcd_val(lcd_t_in);
#endif
#ifdef lcd_t_out
  lcd_val = "lcd_t_out";
  draw_lcd_val(lcd_t_out);
#endif


  last_min = now_min;

}




#if defined (lcd_clock_b)||defined (lcd_clock_m)||defined (lcd_clock_l)
void lcd_time()
{
#ifdef lcd_clock_b
  lcd_parse(lcd_clock_b);
  drawClock_b(now_hour, now_min, lcd_x, lcd_y);
#endif //lcd_clock_b


#ifdef lcd_clock_m
  lcd_parse(lcd_clock_m);
  drawClock_m(now_hour, now_min, lcd_x, lcd_y);
#endif //lcd_clock_m

#ifdef lcd_clock_l
  lcd_parse(lcd_clock_l);
  drawClock_l(now_hour, now_min, lcd_x, lcd_y);
#endif //lcd_clock_l
}
#endif //all clock

#endif //lcd 
