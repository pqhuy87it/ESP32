// новый пример работы с аналоговой клавиатурой
/*
  Сигналы кнопок
  1023
  927
  856
  783
  671
  632
  590
  560
  504
  480
  455
  440
  399
  319
  255
  230
*/
#include "AnalogKey.h"
// указываем пин и количество кнопок
AnalogKey<33, 5> keys;

#include <GyverButton.h>
GButton btn1, btn2, btn3, btn4, btn5;

void setup() {
  Serial.begin(115200);
  // указываем сигналы кнопок аналоговой клавы
  keys.attach(0, 180);
  keys.attach(190, 300);
  keys.attach(305, 500);
  keys.attach(550, 700);
  keys.attach(1000, 1500);
}

void loop() {
  // тикаем кнопки как статусы аналоговых
  btn1.tick(keys.status(0));
  btn2.tick(keys.status(1));
  btn3.tick(keys.status(2));
  btn4.tick(keys.status(3));
  btn5.tick(keys.status(4));

  if (btn1.isClick()) Serial.println("click 1");
  if (btn2.isClick()) Serial.println("click 2");
  if (btn3.isClick()) Serial.println("click 3");
  if (btn4.isClick()) Serial.println("click 4");
  if (btn5.isClick()) Serial.println("click 5");

  if (btn1.isHolded()) Serial.println("click 1");
  if (btn2.isHolded()) Serial.println("hold 2");
  if (btn3.isHolded()) Serial.println("hold 3");
  if (btn4.isHolded()) Serial.println("hold 4");
  if (btn5.isHolded()) Serial.println("hold 5");
}
