#if defined (lcd_clock_m)
byte c0[8] = {B11111, B10001, B10001, B10001, B10001, B10001, B10001, B10001};
byte c1[8] = {B10001, B10001, B10001, B10001, B10001, B10001, B10001, B11111};
byte c2[8] = {B00001, B00001, B00001, B00001, B00001, B00001, B00001, B00001};
byte c3[8] = {B11111, B00001, B00001, B00001, B00001, B00001, B00001, B11111};
byte c4[8] = {B11111, B10000, B10000, B10000, B10000, B10000, B10000, B11111};
byte c5[8] = {B11111, B00001, B00001, B00001, B00001, B00001, B00001, B00001};
byte c6[8] = {B11111, B10001, B10001, B10001, B10001, B10001, B10001, B11111};
byte c7[8] = {B00001, B00001, B00001, B00001, B00001, B00001, B00001, B11111};


void load_num_m() {
  lcd.createChar(0, c0);
  lcd.createChar(1, c1);
  lcd.createChar(2, c2);
  lcd.createChar(3, c3);
  lcd.createChar(4, c4);
  lcd.createChar(5, c5);
  lcd.createChar(6, c6);
  lcd.createChar(7, c7);
}


void draw_num_m(byte dig, byte x, byte y) {
      lcd.setCursor(x, y);
      lcd.write(32);
      lcd.setCursor(x, y + 1);
      lcd.write(32);
      
  switch (dig) {
    case 0:
      lcd.setCursor(x, y);
      lcd.write(0);
      lcd.setCursor(x, y + 1);
      lcd.write(1);
      break;
    case 1:
      lcd.setCursor(x, y);
      lcd.write(2);
      lcd.setCursor(x, y + 1);
      lcd.write(2);
      break;
    case 2:
      lcd.setCursor(x, y);
      lcd.write(5);
      lcd.setCursor(x, y + 1);
      lcd.write(4);
      break;
    case 3:
      lcd.setCursor(x, y);
      lcd.write(3);
      lcd.setCursor(x, y + 1);
      lcd.write(7);
      break;
    case 4:
      lcd.setCursor(x, y);
      lcd.write(1);
      lcd.setCursor(x, y + 1);
      lcd.write(2);

      break;
    case 5:
      lcd.setCursor(x, y);
      lcd.write(4);
      lcd.setCursor(x, y + 1);
      lcd.write(7);
      break;
    case 6:
      lcd.setCursor(x, y);
      lcd.write(4);
      lcd.setCursor(x, y + 1);
      lcd.write(1);
      break;
    case 7:
      lcd.setCursor(x, y);
      lcd.write(5);
      lcd.setCursor(x, y + 1);
      lcd.write(2);
      break;
    case 8:
      lcd.setCursor(x, y);
      lcd.write(6);
      lcd.setCursor(x, y + 1);
      lcd.write(1);
      break;
    case 9:
      lcd.setCursor(x, y);
      lcd.write(6);
      lcd.setCursor(x, y + 1);
      lcd.write(7);
      break;
    case 10:
      lcd.setCursor(x, y);
      lcd.write(32);
      lcd.setCursor(x, y + 1);
      lcd.write(32);
      break;
  }
}


void drawClock_m(byte hours, byte minutes, byte x, byte y)
{
  draw_num_m(hours / 10, x, y);
  draw_num_m(hours % 10, x + 1, y);
  lcd.setCursor(x + 2, y + 1);
  lcd.print(".");
  lcd.setCursor(x + 2, y);
  lcd.print(".");
  draw_num_m(minutes / 10, x + 3, y);
  draw_num_m(minutes % 10, x + 4, y);
}

#endif