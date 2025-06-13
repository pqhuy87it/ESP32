//✈✈✈✈✈✈✈✈✈✈✈✈✈✈✈✈✈✈✈✈✈✈✈✈✈✈✈✈✈✈✈✈✈✈✈✈✈✈
char  ver[] = "170422";
//--------------------------------
#include "Settings_audio.h"   //lib1 or lib2 pcm,vs1053...
#include "Settings_sensor.h"  //nrf24,bme280...,pir,led
#include "Settings_control.h" //кнопки, энкодер,ик //buttons,encoder,ir 
#define pause_after_boot 10 //сек пауза после загрузки//pause after boot sec
//имя точки доступа и пароль будут читаться из файла /data/wifi,
//при первом запуске надо с подкл. телефоном к wifi сети ESP_METEO, password = "11111111"
// зайти с ТЕДЕФОНА! на  192.168.11.11 и сохранить ssid и password,
//(или перед заливкой в SPIFFS заполнить  файл /data/wifi  )
//(если страница не открывается не загружена папка /data в SPIFFS)
///можно в лоб забить перед загрузкой в SPIFFS(SD) /wifi (enter в конце строки)
//AP/password in the file / data/wifi (at the end of the line ENTER)

// если настройка или датчик не используется надо закомментировать #define
//if control or the sensor is not used it is necessary to comment out #define
//WROVER GPIOs 16 and 17 используется для psram, не использовать в проекте!!!
//WROVER GPIOs 16 and 17 are connected to the module’s integrated PSRAM.




#define debug  //можно закомментировать, будет меньше отладочной инф. и больше памяти
#define max_size_log 50000 //byte (50kb) если #log;; в config.txt и файл>max_size_log заменяю


//=====================================================================|
//          SD card + FOTO
//=====================================================================|
//#define pin_sd  4//32-def//5//D4 (lolin32 pro GPIO card -->4)
//#define c_mp3 TFT_ORANGE //цвет линий на экране MP3/line color on MP3 screen

//файлы фото в папке foto на SD и пронумерованы от 1  (1-99999.jpg)
//важно!!! скопировать все файлы /data/*.jpg (openW) в корневой SD карты
//важно!!! если esp8266 раскомментировать в файле  (if esp8266 in the file uncomment)
//TFT_eSPI\User_Setups\Setupx_xxxxx.h  #define SUPPORT_TRANSACTIONS
//-------------------------------------------------------------------------------

//====================================================================
//      ota_update       ftp           http_update           IoT
//====================================================================
//#define ota_update
const char ota_name[] = "meteo";
const unsigned int  t_period_read_alarm_txt = 5; //1-59 minute  period read alarm.txt esp32
//#define http_update   // ex. http://192.168.100.xx/update
//FTP для esp32 ver<2.0 использовать библ. из архива ESP32FTPServer-master_ESPv1-1.99.zip
#define ftp           // esp8266/esp8266    //username, password for ftp
//#define IoT          //config.txt #iot_...   https://www.thingspeak.com  lib-->ThingSpeak
//#define mqtt          //config.txt #mqtt_...  mosquitto  lib--> PubSubClient


//=====================================================================|
// альтернатива закомментированному "//#define  openw", не будет часов и фото  (#ifdef pin_sd), только интернет радио,mp3(#def pin_sd),fm(#def fm)
//alternative to the commented "//# define openw, "there will be no hours and photos (# ifdef pin_sd), only internet radio, mp3 (# def pin_sd),fm (# def fm)
//#define only_radio
//--------------------------------

//=====================================================================|
#define EN   //lang (by default RU)
#define hPa  //pressure hPa (by default RU мм/рт.столба)
//---------------------------------------------------------------------


//===============================TFT======================================
//выбрать разрешение дисплея / to choose resolution of the display
//настройки дисплея в(display settings in) User_Setup_Select.h и \User_Setups\Setup_XX.h
//=====================================================================
#define tft_320_240 //Setup1_ILI9341
//    или OR
//#define tft_480_320  //Setup20_ILI9488 (8266) Setup21_ILI9488 (esp32) Setup13_ILI9481_Parallel
//#define tft_400_300 //epd 4.2" GxEPD2_420

//#define  two_tft   //2xtft_320_240 or 2xtft_480_320
//#define lcd_display  //1602 or 2004
//#define no_tft //OFF  all display


const uint8_t rotation = 1; //1,3 3-SD сверху/3-SD top (not EPD) tft_320_240 or tft_480_320
//#define tft_no_invert     //если цвета в инверсии раскомментировать/if the colors in the inversion are uncomment


//======================================================================
//                      EPD          tft_400_300
//=======================================================================
#if defined (tft_400_300)
//` https://aliexpress.ru/wholesale?catId=&SearchText=%20e-Paper%204.2
static const uint8_t EPD_BUSY = 5;
static const uint8_t EPD_CS   = 15; // aka SS
static const uint8_t EPD_RST  = 4;
static const uint8_t EPD_DC   = 2;
static const uint8_t EPD_SCK  = 18;
static const uint8_t EPD_MISO = 19; // Master-In Slave-Out not used, as no data from display
static const uint8_t EPD_MOSI = 23;

#define  epd_rotation 0
#define  S1epd
#define  S2epd
#define  S3epd

#define  Sfepd //FOTO #ifdef pin_sd +/foto_bw/ *.bmp
#ifdef Sfepd
#define Sfepd_show_date//показывать дату на фото  /Show date in photos
#define Sfepd_show_Tu//показывать уличная темп. /show street T           
#define Sfepd_show_TH//показывать темп./влажность кв. /show T/H          
#endif //Sfepd

#endif //epd
//=====================================================================|




//======================================================================
//                      TWO_TFT                   TWO_TFT
//=======================================================================
// если 2 TFT и tft_320_240 или 480_320 //if 2 and tft_320_240 or 480_320
//используется (adafruit lib->320_240) (ILI9488-master ->320x480),  tft_Espi НЕ задействована
//настройки GPIO дисплеев ниже(display GPIO settings below)
#if defined (two_tft)
#define tft2_S6
// \meteo\!faq_schemes\screens\..\2TFT
// 320x240, только (what screen, only) tft2_S3,tft2_S4,tft2_S6,tft2_Sg
// 480x320, только (what screen, only)   tft2_S1, tft2_S2,tft2_S3,tft2_S4 (без часов,without a watch)

const uint8_t rotation2 = 3; // TFT num=2 // 1,3 3-SD сверху/top
//PIN TFT 1--------------
#define TFT_CLK 18
#define TFT_MOSI 23
#define TFT_CS    15
#define TFT_RST   -1
#define TFT_DC    2
#define TFT_MISO  -1

//PIN TFT 2---------------
#define TFT_CLK2 14
#define TFT_MOSI2 13
#define TFT_CS2   5
#define TFT_RST2  -1
#define TFT_DC2    33
#define TFT_MISO2  -1

#endif
//=====================================================================|



//==============================ESP32==parallel========================|
// если ESP32 c параллельным  интерфейсом  и периодически сбиваются часы, раскомментировать
//If the parallel interface ESP32 and the clock are periodically lost, uncomment
//
//#define  time_correct //only  esp32 parallel
//
//=====================================================================|



//=====================================================================|
//                  add      LCD display 1602 or 2004
//=====================================================================|
#ifdef lcd_display //если if define lcd_display

#define lcd_type 1      // 0 - 1602; 1 - 2004
#define lcd_addr 0x27   // адрес дисплея: 0x27-2004 у меня или 0x3f -1602 у меня.

//------------------------------SCREEN "text;X;Y;text;" X=0-15 Y=0-1 (lcd1602 #def lcd_type 0)
//          time on screen (big,mid,lit..)
//#define lcd_clock_b ";1;0;;" //big clock long 13x2
//    или //or
//#define lcd_clock_m ";0;0;;" //long 5x2
//    или //or
//#define lcd_clock_l "T;10;1;;" //small long 5x1

//#define lcd_day ";6;0;;" //long 2x1
//#define lcd_week ";6;1;;" //2x1 день недели транслит(править под себя в meteo.ino)
//#define lcd_pr ";0;3;mm;" //long 3x1 // hPa 4x1 (bme280...)
//#define lcd_hum_in ";7;1;%;" //3x1 (bme280...,wifi read)
//#define lcd_t_in "Ti ;10;0;c;" //2x1  (bme280...,wifi read)
//#define lcd_t_out "To ;10;1;;" //3x1 (-+temp) ds1820, www openW

/* example 1602
  #define lcd_day ";6;0;;" //long 2x1
  #define lcd_week ";6;1;;" //2x1 день недели транслит(править под себя в meteo.ino)
  #define lcd_t_in "Ti ;10;0;c;" //2x1  (bme280...,wifi read)
  #define lcd_t_out "To ;10;1;;" //3x1 (-+temp) ds1820, www openW
*/
//* example 2004
#define lcd_clock_b ";1;0;;" //long 13x2
#define lcd_day ";17;0;;" //long 2x1
#define lcd_week ";17;1;;" //2x1 день недели транслит(править под себя в meteo.ino)
#define lcd_pr ";0;3;mm;" //long 3x1 // hPa 4x1
#define lcd_hum_in ";7;3;%;" //3x1
#define lcd_t_in "Ti ;13;2;c;" //2x1
#define lcd_t_out "To ;13;3;;" //3x1 (-+temp)
//*/
#endif //lcd_display
//-------------------------------------------------------------------------------



//====================================================================
//              openWeatherMap
//====================================================================
//если есть ключ погоды и он записан  в /data/config.txt!!!
//If there is a weather key and it is written in/data/config.txt!
// если нет ключа , закомментировать #define  openw будут просто часы + датчики+fm(#ifdef FM)+(play www+mp3 ESP32)
//If there is no key, comment # define openw will simply be a clock+sensors+fm(#ifdef FM)+(play www+mp3 ESP32)
#define  openw
#ifdef openw  //if there is openW key. if is not present there will be just hours, to comment out #define openw
long t_period_ch = 2400; //секунды, частота запросов погоды t_period_ch/60=minute (openw 1 time 20 min.) 60sec*20
long t_period_now = 1200; // now t_period_now/60=minute

//RAIN ---------------RAIN------------RAIN-----------------RAIN
// сигн-ция осадков
//rain one point<rain_l ;rain_l<two  point <rain_m ; three point>rain_m (for three hours),>rain_m*2 blinks
float rain_l = 0.5; //сигнализация точками, если кол-во дождя  меньше 0.5мм одна точка, за 3 часовой период
float rain_m = 1; //0.5-1.00мм вкл средний две точки, больше сильный 3т.если >rain_m*2 мигает

#endif //openW
//-----------------------------------------------


//====================================================================
//      NTP       NTP           NTP           NTP
//====================================================================
//откуда берем время  4 варианта, можно свои
//where we take the time, you can your servers
const unsigned long t_period_ntp = 3600000; //частота запросов NTP 1 час
const char* ntpServerName1 = "ntp4.stratum1.ru";//vega.cbk.poznan.pl
const char* ntpServerName2 = "ntp5.stratum2.ru"; //резервный ntp
const char* ntpServerName3 = "time.nist.gov";
const char* ntpServerName4 = "by.pool.ntp.org";
//---------------------------------------------------------


//------------------------------define timechange_summer_winter -----------------------
//смена времени по правилу (зима лето)
//change of time according to the rule (winter and summer)
//lib Timezone libraries\Timezone\examples
//------------------------------------------------------------------------------------
//будет актуально если закомментировано #TIMEZONE в config.txt
//will be relevant if commented # TIMEZONE in config.txt

//#define timechange_summer_winter

//указать номер недели, день недели, месяц, время перевода часов,  количество минут UTC
//specify week number, day of week, month, time of hour transfer, number of UTC minutes (config.txt)
//week --> Last, First, Second, Third, Fourth
//dow --> Sun, Mon, Tue, Wed, Thu, Fri, Sat
//month --> Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec

#ifdef timechange_summer_winter
#include <Timezone.h> //week_win
//правило на летнее//summer rule
TimeChangeRule DST = {"DT", Second, Sun, Mar, 2, -180};    // Daylight time = UTC - 3 hours
//правило на зимнее //winter rule
TimeChangeRule STD = {"ST", First, Sun, Nov, 2, -240};     // Standard time = UTC -4 hours
#endif //timechange_summer_winter
//------------------------------------------------------------------------------------




//====================================================================
//      WF_SEND         WF_SEND       WF_SEND       WF_SEND
//====================================================================
//передача по WIFI если передавать нечего закомм-ть #define wf_send
//если есть датчики BME,DS,Si то можно передать на другие станции
// if there are sensors BME, DS, Si, then you can transmit to other stations

//#define wf_send //temp_kv temp_u h_kv
//------------------
#ifdef wf_send
//что передавать, закомм-ть что не надо
#define wf_send_Tu  //temp_u Темп улица (the outside)
#define wf_send_Tk  //temp_kv Темп кв если есть bme280 или si7021  (room temperature)
#define wf_send_Hh   //h_kv  влаж кв если есть bme280 или si7021   (room humidity)
//#define wf_send_Cm  //command  просто команда
#endif
//----------------------------------------------------------



//====================================================================
//      WI_READ     WI_READ         WI_READ         WI_READ
//====================================================================
//приём по WIFI других станций, если не надо принимать по wifi закомментировть #define wf_read
//какие данные будем слушать что не надо закомм-ть
//reception from other stations

//#define wf_read  //будем слушать

#ifdef wf_read
#define wf_read_Tu  //temp_u Темп улица, если есть DS или NRF надо закомментир.
#define wf_read_Tk  //temp_kv Темп кв если стоит bme280 или si7021 надо закомм-ть
#define wf_read_Hh   //h_kv  влаж кв если стоит bme280 или si7021 надо закомм-ть
//#define wf_read_Cm  //command  просто команда
#endif
//--------------------------------------------------------


//====================================================================
//       WI-FI reconn,esp restart
//====================================================================
const byte ntp_count_rep = 5; //количество неуд. попыток синхр. время, до reconnect кол-во попыток передачи данных по udp
const unsigned long t_ntp_rep_err = 300000; //5мин, время между неудачнывми попытками ntp (время уже синхр)
//reconnect=ntp_count_rep*t_ntp_rep_err
const byte wifi_count_rep = 3; //количество reconnect, после перегрузить
//reboot=reconnect*wifi_count_rep
const unsigned long wifi_rep_t = 40000; //время sec попытки соед. с АП, затем запуск как точки доступа
const unsigned long t_stay_ap = 600000; //время как точки доступа, потом перегрузить second=t_stay_ap/1000
//---------------WI-FI


//====================================================================
//                  Screen
//====================================================================
//если не надо закомментировать #define  Sxx (if no screen is needed then //#define Sxx)
//S0 - 320_240 no openW; S1-S9 - if openW ;Sf - if SD; Sn - night;

// как выглядят скрины S1-Sx смотри в \!faq_schemes\screens\..
//what screenshots look like S1-Sx , see in \!faq_schemes\screens\..

#ifdef openw
#define  S1 0
const byte S_t_bme = 255; //0-255sec (Scr1)сколько показывать темп,влаж bme280 /how many seconds to show bme280
const byte S_t_moon = 5; //sec луна how many seconds to show moon
#define  S2 0
#define  S3 0
#define  S4 0
#define  S5

#ifdef S5
const byte S5_t_ch = 20; //сколько секунд показывать 3 часовой прогноз на S5 , дальше на 4 дня
//how many seconds to show 3 hour forecast for S5, 4 days further
const byte S5_t_day = 30; //сколько секунд показывать дневной прогноз
#endif

#define  Sg
#define  S6 1

#endif //openW

#define  Sn //ночь + основной экран для 480_320 если нет openW, night and main screen for 480_320 if not #define openw for 480_320
#define Sf //фоторамка, если #define pin_sd +/foto/ *.jpg /photo frame if #define pin_sd +/foto/ *.jpg
#ifdef Sf
// время показа фото sec ->config.txt->Sf_t_slide
//time of demonstration of a photo sec ->config.txt->Sf_t_slide def=30sec
#define Sf_show_time//показывать время на фото  /Show time in photos
#define Sf_show_date//показывать дату на фото  /Show date in photos
#define Sf_show_Tu//показывать уличная темп. /show street T           
#define Sf_show_icon//показывать иконку погоды. /show icon openW          
#define Sf_show_TH//показывать темп./влажность кв. /show T/H          
const unsigned int Sf_c_clock = TFT_YELLOW; //   цвет часов
const unsigned int Sf_c_scr_frame = TFT_YELLOW;  //цвет рамки экрана/color of a frame of the screen
#endif

const byte t_ref_tft = 30; //scrsaver  min 1<t_ref_tft<60 обновление экрана /updating of the screen scrsaver minute(t)%t_ref_tft==0

//-----------------------------------------------------



//====================================================================
//      COLOR         COLOR             COLOR
//====================================================================
// цвета описание

#define c_www  TFT_CYAN ////цвет линий на экране WWW проигр/line color on WWW screen play

#define c_pr_ow  0x07E0 //цвет давления с opeW / Pressure colour with opeW

#define c_pr_bme 0x8FF8 //цвет давления с BME,wi_read,nrf24... / Pressure color with BME

//если проблема с бме, будет показано давление с openw, по цвету будет видно
#define c_holiday TFT_ORANGE  //цвет выходных в графике и календаре день недели
#define c_working 0x87E0 //цвет рабочих дней //color of the working days
#define c_monday TFT_CYAN //цвет понедельника //color of the monday
#define c_friday TFT_MAGENTA //цвет пятницы //color of the friday


#define c_date 0xB7FF  //цвет, DD
#define c_month 0xE720  //цвет, MM


//scr5 color 320-240
#if defined (S5) && defined(tft_320_240)
const int S5_color_clock = 0xFFF1; //цвет часов на S5 //color of hours on S5
#endif


//scr5 color 320-240
#if defined (S6) && defined(tft_320_240)
#if S6==1
const int S6_color_clock = 0xFFF1; //цвет часов на S6 //color of hours on S5

#endif
#endif



//scr3 color 480_320
#if (defined (S3)||defined (S4)) && defined(tft_480_320)
const int S3_color_clock = 0xFFF9; //цвет часов на S3,4 //color of hours on S3,4
#endif

//цвет ветра восток,запад на графике/color of wind East, West
const int Sg_c_wing_E = TFT_LIGHTGREY; //восточный ветер
const int Sg_c_wing_W = TFT_GREEN; //западный


//nigth
const int Sn_color_sec = 0x87F0; //цвет секунд scr nigth

//webradio
#define c_channel TFT_LIGHTGREY //цвет списка каналов webradio/ color of the list of channels
#define c_ch_line TFT_YELLOW // подчеркивание найденного канала /underlining of the found channel
#define c_ch_fnd TFT_YELLOW // цвет найденного канала в заголовке /Color of the found channel in the header

#define c_song TFT_YELLOW//цвет песни/ song
#define c_gr TFT_CYAN// //цвет группы/ groups


#ifdef openw
//-------UV------UV----------UV---------UV------------
//цвета по диапазонам ультрафиолета
//colors by UV range
const byte  uv1 = 3;
const int   uv1_color = TFT_GREEN; //< uv1<=3;
//3<uv2<=6;
const byte  uv2 = 6;
const int   uv2_color = TFT_GREENYELLOW;
//6<uv3<=8
const byte  uv3 = 8;
const int   uv3_color = TFT_YELLOW;
//8<uv4<=11
const byte  uv4 = 11;
const int   uv4_color = TFT_RED;
//11<uv3<=20
const byte  uv5 = 20;
const int   uv5_color = TFT_PURPLE;

#endif //openw

//============Temp=======Temp========Temp=========Temp=========
//диапазоны и цвет температуры
//temperature ranges and color
signed char t4 = 30;
const int   t4_color = 0xFA28; //temp>+30

signed char t3 = 25;
const int   t3_color = 0xFCC6; //+30>temp>+25

signed char t2 = 17;
const int   t2_color = 0xFFE0; //temp>+17

signed char t1 = 10;
const int   t1_color = 0xFFF0; //temp>+10

signed char t0 = 0;
const int   t0_color = 0xFFF9; //temp>0

signed char t_1 = -10;
const int   t_1_color = 0x07DF; //temp>-10

signed char t_2 = -15;
const int   t_2_color = 0x067F; //temp>-15

const int   t_3_color = TFT_BLUE; //temp=<t_1
//-----------------------------------------------------



//====================================================================
//    wifi_const      wifi_const         wifi_const
//====================================================================
#if defined (wf_send)||defined (wf_read)
const uint32_t wf_step = 15000; //с какой частотой прд прм wifi
const uint16_t localPort_wf = 54333; // Local port wifi
//заголовки передачи
const String k_temp_u = "25Tu;"; // T улица
const String k_temp_kv = "25Tk;"; // T кв
const String k_h_kv = "25Hh;";  // влажность
const String k_cmd = "25Cm;"; //своя команда
//String cmd = ""; //команда для примера сделал  Sn-след экран (для ver.IK), bp-beep,bp2->2 раза beep, rl реле
#endif
//---------------------------------------------------






//------------------------------------control ERR------------------------------------------

#if !defined (only_radio)&&!defined (openw) //no openW
#if defined(tft_320_240)
#ifndef S0
#define S0
#endif
#endif //240
#if defined(tft_480_320)
#ifndef Sn
#define Sn
#endif
#endif //480
#endif  //openW

#ifdef ESP8266
#undef only_radio
#endif


/*
  #if defined (pin_sd) //&& defined (ESP8266)
  #undef ftp
  #endif
*/
#ifndef pin_sd
#undef Sf
#undef Sfepd
#endif


#if (defined (only_radio)|| !defined (openw))&&defined (two_tft)
#error "ERROR -----> two_tft only with #def openw "
#endif

#ifdef only_radio
#undef openw
#undef Sn
#undef S0
#undef S1
#undef S2
#undef S3
#undef S4
#undef S5
#undef S6
#endif


#ifdef tft_400_300
#undef Sn
#undef S0
#undef S1
#undef S2
#undef S3
#undef S4
#undef S5
#undef S6
#undef Sg
#define epd
#endif


//#ifndef pin_led
//#undef pin_pir
//#endif


#ifdef vs1053
#undef audio_lib2
#undef PCM5102
#endif

#if defined (encoder_lib2)&&defined (encoder)&&defined (ESP8266)
#undef encoder_lib2
#endif

#if defined (encoder_lib2)&&defined (encoder)&&defined (ESP32)
#error "ERROR -----> select #define encoder OR  #define encoder_lib2"
#endif

#if defined (lcd_clock_b)&&defined (lcd_clock_m)
#undef lcd_clock_m
#endif


//--------------no TFT
#ifdef no_tft

#undef tft_320_240
#undef tft_480_320
#undef tft_400_300
#undef lcd_display

#ifdef openw
#undef openw
#endif
#ifdef S0
#undef S0
#endif
#ifdef Sn
#undef Sn
#endif
#ifdef Sf
#undef Sf
#endif

//#error "ERROR -----> select #define tft_320_240 OR  #define tft_480_320"
#endif



///--------------------------------------TWO error

#if defined (two_tft) && !defined (ESP32)
#error " ERROR -----> #define two_tft only for ESP32"
#endif

#if defined (two_tft) && defined (only_radio)
#error " ERROR -----> conflict #define two_tft and  #define only_radio"
#endif

#if defined (two_tft) && defined (tft_400_300)
#error " ERROR -----> conflict #define two_tft and  #define tft_400_300"
#endif



//320x240
// какой экран, только tft2_S2,tft2_S3,tft2_S4,tft2_Sg//what screen, only tft2_S2,tft2_S3,tft2_S4,tft2_Sg
//480x320
//tft2_S1    tft2_S2 tft2_S4

#if defined(tft_320_240) && defined (openw) && defined (two_tft)

#if defined (tft2_S6)
#ifdef S6
#undef S6
#define S6 0
#else
#define S6 0
#endif
#endif

#if defined (tft2_S3)
#ifdef S3
#undef S3
#define S3 0
#else
#define S3 0
#endif
#endif

#if defined (tft2_S4)
#ifdef S4
#undef S4
#define S4 0
#else
#define S4 0
#endif
#endif



#if defined (tft2_Sg)
#ifdef Sg
#undef Sg
#define Sg 0
#else
#define Sg 0
#endif
#endif



#if !defined (tft2_Sg)&& !defined (tft2_S6)&& !defined (tft2_S3)&& !defined (tft2_S4)
#error "ERROR ----->two_tft and tft_320_240 #define tft2_Sg,tft2_S6,tft2_S3 or tft2_S4"
#endif

#endif //tft_320_240 two_tft


//------------------------------------------------tft_480_320
#if defined(tft_480_320) && defined (openw)
#ifdef S6
#undef S6
#endif

#ifdef Sg
#undef Sg
#endif

#endif //


//----------------------------------2TFT
#if defined(tft_480_320) && defined (openw) && defined (two_tft)

#ifdef tft2_S1
#ifdef S1
#undef S1
#define S1 0
#else
#define S1 0
#endif
#endif


#ifdef tft2_S2
#ifdef S2
#undef S2
#define S2 0
#else
#define S2 0
#endif
#endif


#ifdef tft2_S3
#ifdef S3
#undef S3
#define S3 0
#else
#define S3 0
#endif
#endif



#ifdef tft2_S4
#ifdef S4
#undef S4
#define S4 0
#else
#define S4 0
#endif
#endif //2tft


#if !defined (tft2_S1)&& !defined (tft2_S2)&& !defined (tft2_S3)&& !defined (tft2_S4)
#error "ERROR ----->two_tft and tft_480_320 #define tft2_S1,tft2_S2 or tft2_S3 or tft2_S4"
#endif
#endif




#if defined (encoder_lib2)&& !defined (ESP32)
#error "ERROR ----->#define encoder_lib2 only for ESP32 !!! use #define encoder"
#endif

#if defined (fmSi4703)&& defined (Si4703)
#error "ERROR ----->#define 2FM fmSi4703 and  Si4703"
#endif

#if !defined (tft_320_240)&& !defined (tft_480_320)&& !defined (tft_400_300)&& !defined (two_tft)&& !defined (lcd_display)&& !defined (no_tft)
#error "ERROR ----->define tft_***** or define no_tft"
#endif





#ifndef debug
#ifdef max_size_log
#undef max_size_log
#endif
#endif



/*
  cmd="pin_H" +3.3v
  #define pin_out_hight
  cmd="pin_L" 0v
  #define pin_out_low
*/
/*
  //#define Scr_user1
  #if defined (Scr_user1)

  #define Time_480_194 "0;0;"
  #include "h\u1.h"

  #endif
*/
