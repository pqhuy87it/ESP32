//---------------------command-----------------------------------------
//команды \meteo\!faq_schemes\fn_cmd.xls
//lib GyverButton  (тонкие настройки GyverButton.h)
//---------------------command-----------------------------------------


//====================================================================
//IR      IR        IR            IR          IR
///====================================================================

//IR VS1838B ик приемник пин для инет радио или переключения экранов
// если нет IR приемника закоммент эту строку #define pin_ir
// VS1838B IR sensor
//if there is no IR receiver to comment out this line #define pin_ir

//#define pin_ir  34//D4  ////esp8266 D4 esp32 34
#ifdef pin_ir
const byte fn_count = 9; //сколько команд будем использовать в пульте
// (fn_count ) опишем какие команды(можно свои), затем  в файле data/ik.txt запишем коды для этих команд
const char * fn[] = {"play", "next", "prev", "vl+", "vl-", "mute", "play_fm", "play_mp3", "fav"};

const int t_ir_sleep = 600; //ms от залипания пульта пауза
const int t_ir_sleep_short = 200; //"vl+""vl-""mute",btn(0-9)
#endif
//---------------------------------------------------


//====================================================================
//      кнопка        button        кнопка     button
//====================================================================
//----------------------------------------pin_btn1
#define pin_btn1 13 //vp
#ifdef pin_btn1
#define btn1_type 0 //(0;1 ) 0 - pin_btn <--- BTN --- GND //1 - pin_btn <--- BTN --- VCC (like tp-223)

// No PLAY
#define btn1_noplay "next"      //isSingle
#define btn1_dbl_noplay "play"    //isDouble
#define btn1_trpl_noplay "tm;45"     //isTriple
#define btn1_hld_noplay "timer"      //isHolded

//Play WWW
#define btn1_play  "next"
#define btn1_dbl_play "stop"
#define btn1_trpl_play "next_pl"
#define btn1_hld_play "round"      //isHolded

//Play MP3
#define btn1_play_mp3  "next"
#define btn1_dbl_play_mp3 "mute"
#define btn1_trpl_play_mp3 "stop"
#define btn1_hld_play_mp3 "round"      //isHolded


//Play FM
#define btn1_play_fm  "next"
#define btn1_dbl_play_fm "prev"
#define btn1_trpl_play_fm "fav"
#define btn1_hld_play_fm "round"      //isHolded


//TIMER
#define btn1_timer  "next"
#define btn1_dbl_timer "prev"
#define btn1_trpl_timer "timer" //exit
#define btn1_hld_timer "timer_start"      //start-stop

#endif //btn1

//----------------------------------------pin_btn2
//#define pin_btn2 39 //vn
#ifdef pin_btn2
#define btn2_type 1 //(0;1 ) 0 - pin_btn <--- BTN --- GND //1 - pin_btn <--- BTN --- VCC (like tp-223)

// No PLAY
#define btn2_noplay "prev"      //isSingle
#define btn2_dbl_noplay "play_mp3"    //isDouble
#define btn2_trpl_noplay "Sf"     //isTriple
#define btn2_hld_noplay "Scr_info"      //isHolded
#
//Play WWW
#define btn2_play  "prev"
#define btn2_dbl_play "stop"
#define btn2_trpl_play "prev_pl"
#define btn2_hld_play "mute"      //isHolded


//Play MP3
#define btn2_play_mp3  "prev"
#define btn2_dbl_play_mp3 "mp3_random"
#define btn2_trpl_play_mp3 "stop"
#define btn2_hld_play_mp3 "mute"      //isHolded

//Play FM
#define btn2_play_fm  "next"
#define btn2_dbl_play_fm "prev"
#define btn2_trpl_play_fm "fav"
#define btn2_hld_play_fm "round"      //isHolded

//TIMER
#define btn2_timer  "vl+"
#define btn2_dbl_timer "vl-"
#define btn2_trpl_timer "timer"			//exit
#define btn2_hld_timer "timer_start"      //start-stop
#endif //btn2


//----------------------------------------pin_btn3
//#define pin_btn3 xx
#ifdef pin_btn3
#define btn3_type 0 //(0;1 ) 0 - pin_btn <--- BTN --- GND //1 - pin_btn <--- BTN --- VCC (like tp-223)

// No PLAY
#define btn3_noplay "next"      //isSingle
#define btn3_dbl_noplay "play"    //isDouble
#define btn3_trpl_noplay "Sf"     //isTriple
#define btn3_hld_noplay "timer"      //isHolded

//Play WWW
#define btn3_play  "next"
#define btn3_dbl_play "stop"
#define btn3_trpl_play "next_pl"
#define btn3_hld_play "round"      //isHolded

//Play MP3
#define btn3_play_mp3  "next"
#define btn3_dbl_play_mp3 "mute"
#define btn3_trpl_play_mp3 "stop"
#define btn3_hld_play_mp3 "round"      //isHolded


//Play FM
#define btn3_play_fm  "next"
#define btn3_dbl_play_fm "prev"
#define btn3_trpl_play_fm "fav"
#define btn3_hld_play_fm "round"      //isHolded


//TIMER
#define btn3_timer  "next"
#define btn3_dbl_timer "prev"
#define btn3_trpl_timer "timer" //exit
#define btn3_hld_timer "timer_start"      //start-stop

#endif //btn3


//----------------------------------------pin_btn4
//#define pin_btn4 xx
#ifdef pin_btn4
#define btn4_type 0 //(0;1 ) 0 - pin_btn <--- BTN --- GND //1 - pin_btn <--- BTN --- VCC (like tp-223)

// No PLAY
#define btn4_noplay "next"      //isSingle
#define btn4_dbl_noplay "play"    //isDouble
#define btn4_trpl_noplay "Sf"     //isTriple
#define btn4_hld_noplay "timer"      //isHolded

//Play WWW
#define btn4_play  "next"
#define btn4_dbl_play "stop"
#define btn4_trpl_play "next_pl"
#define btn4_hld_play "round"      //isHolded

//Play MP3
#define btn4_play_mp3  "next"
#define btn4_dbl_play_mp3 "mute"
#define btn4_trpl_play_mp3 "stop"
#define btn4_hld_play_mp3 "round"      //isHolded


//Play FM
#define btn4_play_fm  "next"
#define btn4_dbl_play_fm "prev"
#define btn4_trpl_play_fm "fav"
#define btn4_hld_play_fm "round"      //isHolded


//TIMER
#define btn4_timer  "next"
#define btn4_dbl_timer "prev"
#define btn4_trpl_timer "timer" //exit
#define btn4_hld_timer "timer_start"      //start-stop

#endif //btn4


#ifdef ESP32

//====================================================================
//              pin_btn_touch
//====================================================================
// esp32 pin_btn_touch пин на TOUCH PIN, замыкается касанием руки
// esp32 pin on the TOUCH PIN, closes with a hand touch
//кнопку надо удерживать/hold the button

#define pin_btn_touch  13 //  esp32-4 только ESP32/ only ESP32
//touch value
const int touch_value = 40; //меньше этого значения считаю нажатой (esp32)

//---------------------------------------------------------



//====================================================================
//  ESP32    1-5key        1-5key         ESP32AnalogRead
//====================================================================
//GyverButton\examples\analogKeyboard\analogKeyboard.ino
//рекомендуемые пины GPIOs 32 — 39 (ADC1)
// cmd ("next","prev","play","play_fm","play_mp3","stop","fav","vl-" (down),"vl+" (up),"mute","mp3_random","led+","led-","Sf" (scr foto),"Sn" (scr night) or "")
// cmd ("timer" (show screen timer or exit),"timer_start" (start/stop timer)
//cmd ("restart" (ESP.restart),
//min max value and test key (#key1_test_min_max) in config.txt
//my 4key val 1->1100-1300 2->1500-1900 3->2100-2400 4->2600-3000 pin---btn---+3.3
//my 5key red,black val 1->0-15 2->16-100 3->150-300 4->400-700 5->1000-1500 pin---btn---GND


//#define pin_analog_key1 32  //(Ok-->32)      (ADC1 32 — 39)
//ADC2 (10 каналов — GPIOs 0, 2, 4, 12 — 15 и 25 — 27) use WIFI.
#ifdef pin_analog_key1
//----------------------------KEY 1-----------------------
#define key1btn_1
//min max value in config.txt
#ifdef key1btn_1
// No PLAY
#define key1btn1_noplay "prev"      //isSingle
#define key1btn1_dbl_noplay "Scr_info"    //isDouble
#define key1btn1_trpl_noplay "timer"     //isTriple
#define key1btn1_hld_noplay "tm;90"      //isHolded
//Play WWW
#define key1btn1_play  "prev_scr"
#define key1btn1_dbl_play "prev_pl"
#define key1btn1_trpl_play "fav"
#define key1btn1_hld_play "mute"      //isHolded
//Play FM
#define key1btn1_play_fm "prev"
#define key1btn1_dbl_play_fm "vl-"
#define key1btn1_trpl_play_fm ""
#define key1btn1_hld_play_fm ""      //isHolded
//Play MP
#define key1btn1_play_mp3 "prev"
#define key1btn1_dbl_play_mp3 "vl-"
#define key1btn1_trpl_play_mp3 ""
#define key1btn1_hld_play_mp3 ""      //isHolded

//TIMER
#define key1btn1_timer "prev"
#define key1btn1_dbl_timer "timer" //exit
#define key1btn1_hld_timer "timer_start"      //isHolded

#endif //btn1

//----------------------------KEY 2-----------------------
#define key1btn_2
//min max value in config.txt
#ifdef key1btn_2
// No PLAY
#define key1btn2_noplay "play"  //isSingle
#define key1btn2_dbl_noplay "Sf"  //isDouble
#define key1btn2_trpl_noplay ""     //isTriple
#define key1btn2_hld_noplay "tm;10"      //isHolded
//Play WWW
#define key1btn2_play  "fav"   //isSingle
#define key1btn2_dbl_play "vl+" //isDouble
#define key1btn2_trpl_play ""   //isTriple
#define key1btn2_hld_play "mute"      //isHolded
//Play FM
#define key1btn2_play_fm "fav"
#define key1btn2_dbl_play_fm "vl+"
#define key1btn2_trpl_play_fm ""
#define key1btn2_hld_play_fm ""      //isHolded
//Play MP#
#define key1btn2_play_mp3 "fav"
#define key1btn2_dbl_play_mp3 "vl+"
#define key1btn2_trpl_play_mp3 ""
#define key1btn2_hld_play_mp3 ""      //isHolded

//TIMER
#define key1btn2_timer "vl+"
#define key1btn2_dbl_timer ""
#define key1btn2_hld_timer "timer_start"      //isHolded

#endif //btn2


//----------------------------KEY 3-----------------------
#define key1btn_3
//min max value in config.txt
#ifdef key1btn_3
// No PLAY
#define key1btn3_noplay "play_mp3"        //isSingle play WWW
#define key1btn3_dbl_noplay "Sn"      //isDouble
#define key1btn3_trpl_noplay ""     //isTriple
#define key1btn3_hld_noplay "tm;15"      //isHolded
//Play WWW
#define key1btn3_play  "fav"     //stop
#define key1btn3_dbl_play "vl-"
#define key1btn3_trpl_play ""
#define key1btn3_hld_play "mute"      //isHolded
//Play FM
#define key1btn3_play_fm "mute"
#define key1btn3_dbl_play_fm ""
#define key1btn3_trpl_play_fm ""
#define key1btn3_hld_play_fm ""      //isHolded
//Play MP3
#define key1btn3_play_mp3 "mute"
#define key1btn3_dbl_play_mp3 "fav"
#define key1btn3_trpl_play_mp3 ""
#define key1btn3_hld_play_mp3 "fav"      //isHolded
//TIMER
#define key1btn3_timer "vl-"
#define key1btn3_dbl_timer ""
#define key1btn3_hld_timer "timer_start"      //isHolded

#endif //btn3

//----------------------------KEY 4-----------------------
#define key1btn_4
//min max value in config.txt
#ifdef key1btn_4
// No PLAY
#define key1btn4_noplay "next"        //isSingle
#define key1btn4_dbl_noplay "Sf"      //isDouble
#define key1btn4_trpl_noplay "timer"     //isTriple
#define key1btn4_hld_noplay "tm;45"      //isHolded
//Play WWW
#define key1btn4_play  "next_scr"
#define key1btn4_dbl_play "next_pl"
#define key1btn4_trpl_play "fav"
#define key1btn4_hld_play "mute"      //isHolded
//Play FM
#define key1btn4_play_fm "next"
#define key1btn4_dbl_play_fm ""
#define key1btn4_trpl_play_fm ""
#define key1btn4_hld_play_fm ""      //isHolded
//Play MP3
#define key1btn4_play_mp3 "next"
#define key1btn4_dbl_play_mp3 ""
#define key1btn4_trpl_play_mp3 "" //or "mp3_random" random on/off
#define key1btn4_hld_play_mp3 "mute"      //isHolded

//TIMER
#define key1btn4_timer "next"
#define key1btn4_dbl_timer ""
#define key1btn4_hld_timer "timer_start"      //isHolded

#endif //btn4


//----------------------------KEY 5-----------------------
#define key1btn_5
//min max value in config.txt
#ifdef key1btn_5
// No PLAY
#define key1btn5_noplay "play"        //isSingle
#define key1btn5_dbl_noplay "play_bg"      //isDouble
#define key1btn5_trpl_noplay "timer"     //isTriple
#define key1btn5_hld_noplay "play_mp3"      //isHolded
//Play WWW
#define key1btn5_play  "stop"
#define key1btn5_dbl_play "play_bg"
#define key1btn5_trpl_play "fav"
#define key1btn5_hld_play "play"      //isHolded
//Play FM
#define key1btn5_play_fm "stop"
#define key1btn5_dbl_play_fm ""
#define key1btn5_trpl_play_fm ""
#define key1btn5_hld_play_fm ""      //isHolded
//Play MP3
#define key1btn5_play_mp3 "stop"
#define key1btn5_dbl_play_mp3 ""
#define key1btn5_trpl_play_mp3 "" //or "mp3_random" random on/off
#define key1btn5_hld_play_mp3 "mute"      //isHolded
// on TIMER
#define key1btn5_timer "timer" //exit
#define key1btn5_dbl_timer ""
#define key1btn5_hld_timer "timer"      //isHolded


#endif //btn5
#endif //4key end
#endif  //ESP32



//====================================================================
//      Encoder        Encoder        Encoder     Encoder
//====================================================================
//  другие настройки в GyverEncoder.h //other settings in the GyverEncoder.h
//\libraries\GyverEncoder\examples\encoder_demo\encoder_demo.ino

//влево-вправо смена эрана, короткое нажатие старт радио ,
//долгое нажатие-режим цифр, далее короткое старт радио на заданном канале, долгое старт таймера
//если радио--> влево, вправо цифры, далее долгое таймер, короткое переход на канал;
//долгое mute;нажать+влево-вправо громкость; короткое выход из таймера или стоп
//left-right screen change, short press start radio ,
//long press-digit mode, then short start of the radio on the specified channel, long start of the timer
//if radio--> left, right numbers, then long timer, short jump to channel;
//long mute; press + left-right volume; short exit timer or stop

//=====================================================================|

//#define encoder // ESP8266 GyverEncoder если есть энкодер раскомм=ть

#if defined (encoder)
#ifdef ESP32
#define CLK 16//32
#define DT  17//33
#define SW  35 //кнопка 
#endif
#ifdef ESP8266
#define CLK D1
#define DT  D2
#define SW  D4 //кнопка 
#endif //ESP8266
#define TYPE 1 // 0 или 1 (полушаговый энкодер или полношаговый) более подробно смотри в GyverEncoder.h 
const int t_enc_num_clear = 3000; // msec через сколько очищать выбор цифр

//настроить свою команду на click или  dblclick (если не играет)
//set your command to click or dblclick (if not playing)
#define enc_onclick_noplay "next"//timer,next, prev, play, play_fm, play_mp3
#define enc_ondblclick_noplay "timer"//timer,next, prev, play, play_fm, play_mp3

//использовать прерывание, если есть проблемы с енкодером
//use interrupt if there are problems with the encoder

#ifdef ESP32
#define encoder_int // только ESP32// for ESP32
#endif
#endif //encoder
//===========================end encoder==========================================|

//ИЛИ другая библиотека для енкодера только на ESP32
//OR another library for the encoder only ESP32

//=====================================================================|
//                    Encoder Module lib  ESP32Encoder ---->ESP32!!!!
//=====================================================================|
//ESP32Encoder-master\examples\Encoder\Encoder.ino
//влево-вправо смена эрана, короткое нажатие старт радио ,
//долгое нажатие-режим цифр, далее короткое старт радио на заданном канале, долгое старт таймера
//если радио--> влево, вправо цифры, далее долгое таймер, короткое переход на канал;
//долгое mute;нажать+влево-вправо громкость; короткое выход из таймера или стоп
//left-right screen change, short press start radio ,
//long press-digit mode, then short start of the radio on the specified channel, long start of the timer
//if radio--> left, right numbers, then long timer, short jump to channel;
//long mute; press + left-right volume; short exit timer or stop

//=============================ESP32 encoder========================================|
//#define  encoder_lib2 // ESP32 альтернатива GyverEncoder//replacement GyverEncoder
#if defined (encoder_lib2)
#define CLK 16//32    (16,17,4 2xtft)
#define DT  17//33
#define SW  35//35 //кнопка 

#define encType_single//encType_single,encType_half,encType_full  
//#define puType  UP //UP,DOWN,NONE  подтяжка встроенная в плату, если используется//internal PULL, if used
const int t_enc_num_clear = 3000; // msec через сколько очищать выбор цифр

//----------encoder BTN (GyverButton.h)
//#define   steptimeout 200 //интервал по кругу WWW--> MP3--> FM //circle interval  WWW--> MP3--> FM
//#define   setdebounce 50        // настройка антидребезга
//#define  settimeout 500         // настройка таймаута на удержание
//#define   setclicktimeout 200  // настройка таймаута между кликами
//#define    pull HIGH_PULL   //HIGH_PULL LOW_PULL


//настроить свою команду на click или  dblclick (если ничего не играет)
//set your command to click or dblclick (if not playing www fm mp3 )
#define enc_onclick_noplay "play"
#define enc_ondblclick_noplay "play_bg"
#define enc_ontripleclick_noplay "tm;50"


//Play WWW
#define enc_onclick_play "stop"
#define enc_ondblclick_play "fav"
#define enc_ontripleclick_play "next_pl"
#define enc_hld_play "play_bg"

//Play MP3
#define enc_onclick_play_mp3 "stop"
#define enc_ondblclick_play_mp3 "next"
#define enc_ontripleclick_play_mp3 "mp3_random"
#define enc_hld_play_mp3 "mute"

/*
  //Play FM
  #define enc_onclick_play_fm "play"//timer,next, prev, play, play_fm, play_mp3
  #define enc_ondblclick_play_fm "timer"//timer,next, prev, play, play_fm, play_mp3
  #define enc_ontripleclick_play_fm "Scr_info"//timer,next, prev, play, play_fm, play_mp3
*/



#endif //enc2
//============================end encoder_lib2=========================================
