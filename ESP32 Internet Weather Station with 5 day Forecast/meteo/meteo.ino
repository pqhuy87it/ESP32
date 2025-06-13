#include "h\color.h"
#include "settings.h"


//=========================================
#ifdef R433
//=========================================
#include <RH_ASK.h>
#include <GParser.h>
//RH_ASK(uint16_t speed = 2000, uint8_t rxPin = 11, uint8_t txPin = 12, uint8_t pttPin = 10, bool pttInverted = false);
RH_ASK r433(2000, pin_r433_rx, 0 , 0);
bool status_r433 = false;
#endif



#if defined(def_core0)
#include <esp_task_wdt.h>
TaskHandle_t audioplay;
enum : uint8_t {SET_VOLUME, GET_VOLUME, CONNECTTOHOST, CONNECTTOFS, STOPSONG, SETTONE, INBUFF_FILLED, INBUFF_FREE};

struct audioMessage {
  uint8_t     cmd;
  const char* txt;
  uint32_t    value;
  uint32_t    ret;
} audioTxMessage, audioRxMessage;

QueueHandle_t audioSetQueue = NULL;
QueueHandle_t audioGetQueue = NULL;

void CreateQueues() {
  audioSetQueue = xQueueCreate(10, sizeof(struct audioMessage));
  audioGetQueue = xQueueCreate(10, sizeof(struct audioMessage));
}
#endif //c0


#define ServerVersion "1.0"
String webpage = "";
#include "css.h"

#ifdef pin_sd
bool    SPIFFS_present = false;
bool   SD_present = true;
#else
bool    SPIFFS_present = true;
bool   SD_present = false;
#endif



#ifdef timechange_summer_winter
Timezone TZ(DST, STD);
#endif
byte now_min = 99;
byte now_hour = 99;
byte now_day = 99;
byte now_month = 99;
byte now_weekday = 99;
int now_year = 0;
String now_date = "";
String now_time = "";
int y_onload = 15;
uint32_t  tm_wh_now = 0, tm_wh_ch = 0, tm_ntp = 0;

#ifdef tft_320_240
const byte y_step = 14;
#endif
#ifdef tft_480_320
const byte y_step = 17;
#endif

//---------------------------------LCD------------------------------
#ifdef lcd_display
bool lcd_on = true;
String  lcd_val = "";

String lcd_txt1 = "";
int8_t lcd_x = 0;
int8_t lcd_y = 0;
String lcd_txt2 = "";
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#if (lcd_type == 1)
LiquidCrystal_I2C lcd(lcd_addr, 20, 4);
#else
LiquidCrystal_I2C lcd(lcd_addr, 16, 2);
#endif

//num_l
#include "h\lcd_num_m.h"
//num_b
#include "h\lcd_num_b.h"

#endif // lcd_display

#ifdef EN
const char *dayNames[]  = {
  "Su",
  "Mo",
  "Tu",
  "We",
  "Th",
  "Fr",
  "Sa",
};
#else

const char *dayNames[]  = {
  "BC",
  "\x9F\x9D",
  "BT",
  "CP",
  "\xA7\xA2",
  "\x9F\xA2",
  "\xA1\x91"
} ;
/*
   //const String dayNames[7]  = {

  "BC",
  "ПН",
  "BT",
  "CP",
  "ЧТ",
  "ПТ",
  "СБ",
  };
*/

#endif //EN


#if defined( pin_btn1)|| defined( pin_btn2)||defined( pin_btn3)|| defined( pin_btn4)
#include "GyverButton.h"
uint8_t press_count = 0;
#if defined( pin_btn1)
GButton btn1(pin_btn1, btn1_type);
#endif
#if defined( pin_btn2)
GButton btn2(pin_btn2, btn2_type);
#endif
#if defined( pin_btn3)
GButton btn3(pin_btn3, btn3_type);
#endif
#if defined( pin_btn4)
GButton btn4(pin_btn4, btn4_type);
#endif


#endif //btnt



#ifdef ESP32
#if defined( pin_analog_key1)|| defined( pin_analog_key2)

#include "GyverButton.h"
uint16_t key1_debounce = 65; // установка времени антидребезга
uint16_t key1_timeout = 300; // установка таймаута удержания
uint16_t key1_clicktimeout = 500; // установка таймаута между кликами
uint16_t key1_steptimeout = 400; // установка таймаута между инкрементами
uint16_t key1btn1_min = 9999, key1btn1_max = 9999;
uint16_t key1btn2_min = 9999, key1btn2_max = 9999;
uint16_t key1btn3_min = 9999, key1btn3_max = 9999;
uint16_t key1btn4_min = 9999, key1btn4_max = 9999;
uint16_t key1btn5_min = 9999, key1btn5_max = 9999;

byte key1_test_min_max = 0;

#ifdef key1btn_1
GButton key1btn1;
#endif
#ifdef key1btn_2
GButton key1btn2;
#endif
#ifdef key1btn_3
GButton key1btn3;
#endif
#ifdef key1btn_4
GButton key1btn4;
#endif
#ifdef key1btn_5
GButton key1btn5;
#endif
#endif//4key
#endif//32



//---------------------------------audio------------------------------
bool play = false;
bool play_bg = false;

#if defined (ESP32)
uint8_t pl_list = 0;
String pl_list_name = "";
uint8_t pl_list_max = 0;
String pl_list_file_name = "/i_stat.txt";
int volume = 50;
int volume_mp3 = 50;
byte ch_min = 254; //номер п.п 1 станции
byte ch_max = 0; //номер п.п последней станции
byte ch_next = 0; //
byte ch_prev = 0; //
bool new_song = true;
bool new_song_mp3 = true;
String ch_name = "";
String ch_url = "";
String play_song = "";
String play_err = "";
const byte rep_count = 2; //сколько попыток при ошибке открытия сайта
int retryms = 0;
bool b_draw_song = false; //метка отрисовки песни
bool playing = false;
#if defined (audio_lib2) ||defined (vs1053)

uint32_t buff_fill = 0;
uint32_t  buff_free = 0;
String play_song_mp3 = "";
String play_song_g_mp3 = "";
String play_album_mp3 = "";
String play_year_mp3 = "";
#endif
String stat_num = ";"; //список станций для быстрого поиска +num1;+num2;
#ifdef vs1053
#include "vs1053_ext.h"
bool status_vs1053 = false;
VS1053 audio(VS1053_CS, VS1053_DCS, VS1053_DREQ);
#elif defined (audio_lib2)&& defined (PCM5102)
#include "Audio.h"
Audio audio;
#elif defined (audio_lib2)&& !defined (PCM5102)
#include "Audio.h"
Audio audio(true, I2S_DAC_CHANNEL_BOTH_EN );// I2S_DAC_CHANNEL_LEFT_EN  I2S_DAC_CHANNEL_RIGHT_EN  I2S_DAC_CHANNEL_DISABLE
#else
#include "h\iradio.h"
#endif //lib2

#endif //ESP32

//------------------------------------------------------------


#if defined( beep_pass)&&defined( ESP32) &&defined( pin_beep)
#include <Tone32.h>
#endif


#if defined (pin_sd)&&defined (ESP32)
String track_name = "";
int track_find = 0;
unsigned int curr_track = 0;
byte vol_def_mp3 = 80 ; //1-100
bool mp3_random = false; //0 as is 1 -random
#endif
#if defined (ESP32)
byte vol_def = 80 ; //1-100 www
String logo_www = "";
#endif

#ifdef fmRDA5807
//#include <RDA5807.h>
#include <Wire.h>
#include <radio.h>
#include <RDA5807M.h>
//RDA5807 FM;
RDA5807M FM;
#endif //fmRDA5807



#ifdef  fmSi4703
#include <Wire.h>
#include "SparkFunSi4703rds.h"
Si4703_Breakout FM(pin_fm_rst, pin_fm_sdio, pin_fm_sclk); //fmSi4703
#endif

#ifdef  fmSi4703_lib2
#include <Wire.h>
#include <SI470X.h>
SI470X FM;
#endif



#if defined (fmSi4703)|| defined (fmRDA5807)||defined (fmSi4703_lib2)
int freq_fm = 924; // 94.5MHz
int volume_fm = 10;
int vol_def_fm = 15 ;
byte fav_ch_fm[fav_count_fm] = {0};
byte fav_count_real_fm = 0; //после упаковки если меньше чем задано
byte ch_min_fm = 254; //номер п.п 1 станции
byte ch_max_fm = 0; //номер п.п последней станции
byte ch_next_fm = 0; //
byte ch_prev_fm = 0; //
//bool new_song=true;
String ch_name_fm = "";
#endif
byte ch_fm = 1; //ном канала FM №1 начальный
byte ch = 1; //ном канала WWW №1 начальный
byte ch_fnd = 1; //ном канала WWW №1 начальный

int ch_mp3 = 1; //ном трека начальный


//---------------------------------TIMER-------------------------------------
byte tm_alarm_count = 0;
byte tm_alarm_period = 0;

#ifdef pin_beep
byte tm_alarm_count_beep = 0;
byte tm_alarm_period_beep = 0;
#endif

uint32_t  t_timer_scr = 0; // screen timer
uint32_t t_timer_boot = 0; //после сбоя чс
uint32_t t_timer = 0; //play  timer on -hand
uint32_t t_timer_b = 0;
uint32_t t_timer_alarm = 0; //play  timer on /alarm.txt
byte timer_id = 99; //0-www,1-fm, 2=mp3,3-beep
bool scr_timer = false;
bool  timer_alarm = false;
int h_timer = 0;
int m_timer = 0;
signed char h1 = 0, h2 = 0, m1 = 0, m2 = 0;
const byte col_count = 6;
#ifdef tft_480_320//---------------480--------------
const byte x_onload = 240;
const byte y_begin = 85;
const byte col_step = 75;
const byte col_1 = 0;
const byte col_2 = 130;//hh
const int col_3 = col_2 + col_step; //hh
const int col_4 = 40 + col_3 + col_step; //mm
const int col_5 = col_4 + col_step; //mm
const int col_6 = col_5 + col_step + 5; //mm
const int y_jpeg = y_begin + 50; //mm
#endif
#ifdef tft_320_240 //---------------320--------------
const byte x_onload = 160;
const byte y_begin = 82;
const byte col_step = 56;
const byte col_1 = 0;
const byte col_2 = 63;//hh
const int col_3 = col_2 + col_step; //hh
const int col_4 = 20 + col_3 + col_step; //mm
const int col_5 = col_4 + col_step; //mm
const int col_6 = col_5 + col_step + 5; //mm
const int y_jpeg = y_begin + 20; //mm
#endif
#ifdef tft_400_300 //---------------400--------------
const byte x_onload = 200;
const byte y_begin = 82;
const byte col_step = 56;
const byte col_1 = 0;
const byte col_2 = 63;//hh
const int col_3 = col_2 + col_step; //hh
const int col_4 = 20 + col_3 + col_step; //mm
const int col_5 = col_4 + col_step; //mm
const int col_6 = col_5 + col_step + 5; //mm
const int y_jpeg = y_begin + 20; //mm
#endif




bool play_fm = false;
bool play_mp3 = false;
bool start_on_alarm = false; //метка если запущен по расписанию, для выхода после или остаться, если уже играло

const char* ssid     = "";  //точка доступа
const char* password = "";
bool wifi_force_connect = false;
uint32_t t_wifi_force_connect = 30000; //30sec
int wifi_power = 0;



//clock
const int CX = 63;
const int CY = 122;
const byte R = 40;
//----------

byte t_b = 99, t_e = 99; //ночные часы беру из config
signed char delta_p = 0; //поправка на давление
signed char delta_t = 0; //поправка на температуру в комнате

int t_max = 28, t_min = -10, v_max = 7; //сигнализация на экран предельных значений темп и скорости.ветра, уфо
byte uv_max = 6; //ультрофиолет
unsigned int p_max = 760, p_min = 720 ; //давление для сигнализации в config.txt если есть там первично
byte beep_b = 99; // время "с" беру в config.txt
byte beep_e = 99; //"до" беру в config.txt
//время перезагрузки ESP
byte esp_restarts = 99;
byte last_hour = 99;
byte last_minute = 99; //alarm.txt shed
bool log_file = false; //log fie
String log_str = "";

#if !defined (max_size_log) &&defined (debug)
uint32_t max_size_log = 50000;
#endif



//SCR-------------------------------
bool s_info = false; //show #defines
const byte scr_count = 20;
byte scr_count_pack = 0;
signed char scr_number = 0;
char *scr_name[scr_count];
char scr_name_def[4];

byte scr_number_prev;
byte scr_show_count = 0; //если по кругу экраны >1 config.txt
uint16_t t_S1_show = 0;
uint16_t t_S2_show = 0;
uint16_t t_S3_show = 0;
uint16_t t_S4_show = 0;
uint16_t t_S5_show = 0;
uint16_t t_S6_show = 0;
uint16_t t_Sf_show = 0;
uint16_t t_S0_show = 0;
uint16_t t_Sg_show = 0;
bool scr_round = true;

//NTP--------------------------
signed char TIMEZONE = 99;
bool sync_t = false;
bool sync_force = false;
time_t t_ntp_update = 0;

byte t_next_shed = 0; //T чтения план-ка
byte next_shed_ch = 0;
char next_label;
bool alarm_active = false;
byte ch_prev_alarm_www = 0; //возврат на этот канал после таймера, если уже играло
byte pl_list_prev_alarm_www = 0;
byte ch_prev_alarm_fm = 0; //возврат на этот канал после таймера, если уже играло
int ch_prev_alarm_mp3 = 0; //возврат на этот канал после таймера, если уже играло

bool  vol_rotate = false; //если громеость менял, оставляю для всех каналов , до выхода их экр. радио
byte ref_tft_last = 99;
bool show_song = true;



struct alarm_structure {
  //bool  active = false; //volume
  char  label; //тип I inet,m mp3,f fm
  byte  h = 99; //hour
  int  m = 99; //min
  byte  ch_num = 0; // ch
  //String  fname = "";
  unsigned int t_long = 0; //length T millisec
  //byte  count=0; //повторов
  //byte  interval=0; //min
  //  byte  vol = 50; //volume

};
alarm_structure al;
bool play_alarm = false;
byte  beep_mp3_vol = 0; //volume

#ifdef pin_ir
//const byte fn_count = 9;
unsigned long code[fn_count] = {0};
//const char * fn[] = {  "play", "next", "prev", "vl+", "vl-", "mute", "play_fm", "play_mp3", "fav"};
unsigned long code_btn[10] = {0};
String id_ir = "";
#endif
int temp_kv = 200; //T_IN
int pr_kv = 200; //PRES_in
byte h_kv = 200; //Hum_in
bool ref_kv_th = false;
bool ref_wh = false;
bool ref_wh_now = false;
int temp_u = 200; //T_OUT
unsigned long period_ulica = 0; //во сколько принял Темп ул.
uint32_t t_read_tk = 0; //во сколько принял Темп кв
unsigned int c_tu_frame = TFT_CYAN;
unsigned int c_th_frame = TFT_CYAN;

String appid   = "KEY OPW";  //ключ ген. беру из config.txt
String lat     = "53.900002";
String lon     = "27.566668";
byte max_ch3 = 40; //&cnt=40 беру из conf, количество 3 часовых записей на 5дней,
// не больше 40 , можно меньше если зависает, просто уменьшится кол-во дней в прогнозе




#include <TimeLib.h>  // day of the week (1-7), Sunday is day 1
#include <TimeLord.h> //moon
TimeLord myLord;


#if defined (pin_led) && defined (ESP32)
#include "esp32-hal-ledc.h"
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 10; ///Resolution 8, 10, 12, 15
#endif

#if defined (pin_led)
signed char led_val = 100;

byte led_def = 100; // яркость TFT 0-100,TFT brightness 0-100
byte led_night = 100; // яркость ночью ,brightness at night
byte led_sunset = 100; // яркость после захода ,after sunset
#endif

bool led_on = true;
#if defined (pin_pir)
uint16_t t_pir_period = 0;
#endif

//--------------------encoder
#if defined (encoder)
#include "GyverEncoder.h"
Encoder enc(CLK, DT, SW, TYPE);

#if defined (encoder_int)
ICACHE_RAM_ATTR   void isrCLK() {
  enc.tick();  // отработка в прерывании
  // Serial.println("isrCLK");
}
ICACHE_RAM_ATTR   void isrDT() {
  enc.tick();  // отработка в прерывании
  // Serial.println("isrDT");
}

#endif
#endif //enc

//--------------------encoder LIB 2
#if defined (encoder_lib2) && defined (ESP32)
#include <ESP32Encoder.h>
ESP32Encoder enc;
#include "GyverButton.h"
GButton enc_btn(SW);
void IRAM_ATTR isr_enc_btn() {
  enc_btn.tick();  // отработка в прерывании
  //  Serial.println("isrCLK");
}
#endif //enc 2

#ifdef ESP32
byte fav_ch[fav_count] = {0};
byte fav_count_real = 0; //после упаковки если меньше чем задано
#endif


//=============openw=============================openw================================================
#ifdef openw
#include "openWeather.h"
OWMfiveForecast    owF5;
#include "OpenWeatherCurrent.h"
OpenWeatherMapCurrent owNOW;
#include "JsonStreamingParser.h"
#include "JsonListener.h"
JsonStreamingParser pars;

signed char delta_tz = 0; //для выравнивания т зоны к 3 часам запроса


String url_ch = "";
String url_now = "";
String url_uv = "";

#ifndef EN
const String url_ch_b = "http://api.openweathermap.org/data/2.5/forecast?lang=RU&units=metric&lat=";
const String url_now_b = "http://api.openweathermap.org/data/2.5/weather?lang=RU&units=metric&lat=";
#else
const String url_ch_b = "http://api.openweathermap.org/data/2.5/forecast?lang=EN&units=metric&lat=";
const String url_now_b = "http://api.openweathermap.org/data/2.5/weather?lang=EN&units=metric&lat=";
#endif //EN



const String url_uv_b = "http://api.openweathermap.org/data/2.5/uvi?appid=";





float rain_mm = 0; //количество осадков за день
int davlenie7[8];
int temperatura7[8];
int temperatura7n[8];
int p7_max = 200, p7_min = 2000;

byte day7[8];//
String icon7[8];//
byte holiday7[8];//выходные
float rain7[8];//осадки
byte veter7[8]; //скор ветра
String wind_ico = "";
int v_gr_7[8];
byte weekday7[8];



byte entries = 0; //count ret ch openW
// 3-часовой
#if defined (Sg) ||(defined (S4) && defined(tft_480_320))||defined (epd)
const byte ch_count = 41;
#elif (defined (S4) &&  defined(tft_320_240))||(defined (S3) && defined(tft_480_320))
const byte ch_count = 27; //
#else
const byte ch_count = 17;
#endif


int temperatura_ch[ch_count];
byte day_ch[ch_count];//
String icon_ch[ch_count];//
int v_gr_ch[ch_count];
byte v_ch[ch_count];
float rain_ch[ch_count]; //данные о дожде мм
uint32_t t_ch_b = 0;


#if defined (Sg)||(defined (S4) && defined(tft_480_320))||(defined (tft2_S4) && defined(tft_480_320))
int p_ch[ch_count];//давление
byte h_ch[ch_count];//влажность
#endif

#if defined (epd)
int p_ch[ch_count];//давление
byte h_ch[ch_count];//влажность
#endif


int c_wind = 0; //color wind

String descript_now = "";
int temp_ch_n = 200;
uint last_7day = 0;
int davlenie0 = 200;


unsigned long lastConnectionTime_now = 0;
unsigned long postingInterval_now = 0;

unsigned long lastConnectionTime_ch = 0;
unsigned long postingInterval_ch = 0;
unsigned long lastConnectionTime_uv = 0;
unsigned long postingInterval_uv = 0;

float uv = -1;
//String httpData_uv,httpData_ch;
#endif



//FTP================================
#ifdef ftp
#if defined (pin_sd) && defined (ESP32)
#include "ESP32FtpServer.h"
#else
#include <ESP8266FtpServer.h>
#endif

FtpServer ftpSrv;
#endif





#define FS_NO_GLOBALS //allow spiffs to coexist with SD card, define BEFORE including FS.h
#include <FS.h> //spiff file system

#ifdef pin_sd
#include "SD.h"
unsigned int max_jpg_num = 0;
int Sf_t_slide = 30; //config.txt
unsigned int max_num_mp3 = 0; //номер max treck
unsigned int min_num_mp3 = 0; //номер max treck
bool status_sd = false;
#endif

#ifdef ESP32
#include "SPIFFS.h" // Needed for ESP32 only
#endif



//==========ESP8266============ESP8266===============ESP8266==================ESP8266=========ESP8266
#ifdef ESP8266
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

ESP8266WebServer  server(80);
#ifdef http_update
#include <ESP8266HTTPUpdateServer.h>
ESP8266HTTPUpdateServer httpUpdater;
#define update_path "/update"
#endif

#endif


//===========ESP32=======================ESP32==================ESP32=================ESP32
#ifdef ESP32
#include <WebServer.h>
#include <WiFi.h>
#include <HTTPClient.h>
WebServer server(80);
#ifdef http_update
#include <Update.h>
#define update_path "/update"

const char* serverIndex =
  "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
  "<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
  "<input type='file' name='update'>"
  "<input type='submit' value='Update'>"
  "</form>"
  "<div id='prg'>progress: 0%</div>"
  "<script>"
  "$('form').submit(function(e){"
  "e.preventDefault();"
  "var form = $('#upload_form')[0];"
  "var data = new FormData(form);"
  " $.ajax({"
  "url: '/update',"
  "type: 'POST',"
  "data: data,"
  "contentType: false,"
  "processData:false,"
  "xhr: function() {"
  "var xhr = new window.XMLHttpRequest();"
  "xhr.upload.addEventListener('progress', function(evt) {"
  "if (evt.lengthComputable) {"
  "var per = evt.loaded / evt.total;"
  "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
  "}"
  "}, false);"
  "return xhr;"
  "},"
  "success:function(d, s) {"
  "console.log('success!')"
  "},"
  "error: function (a, b, c) {"
  "}"
  "});"
  "});"
  "</script>";

#endif

#endif

///--------------------mqtt--------------------------------------------------
#ifdef mqtt
const char* mqtt_server = "192.168.100.222";
unsigned int mqtt_port = 1883;
const char* mqtt_user;
const char* mqtt_password;
const char* mqtt_H_topic_send = "";
const char*  mqtt_T_topic_send = "";
const char* mqtt_T2_topic_send = "";
const char*  mqtt_P_topic_send = "";
String mqtt_H_topic_read = "";
String mqtt_T_topic_read = "";
String mqtt_T2_topic_read = "";
String mqtt_P_topic_read = "";

String mqtt_esp_name = "meteo";

uint32_t t_reconn_mqtt = 5;
unsigned long mqtt_period_send = 20; //period send sec>=20


#include <PubSubClient.h>
WiFiClient wifi_mqtt;
PubSubClient client_mqtt(wifi_mqtt);


#endif //mqtt



///--------------------IoT---------------------------------------------------
#ifdef IoT
#include "ThingSpeak.h"
WiFiClient  client_iot;
unsigned long iot_id = 0;
const char * iot_key = "";
unsigned long iot_period_send = 0; //period send sec>=20
byte iot_T_fld_send = 0; //temp квартира(room);ном поля(num field) bme280
byte iot_H_fld_send = 0; //humidity квартира(room) ;ном поля(num field) bme280
byte iot_T2_fld_send = 0; //temp улица(street)
byte iot_P_fld_send = 0; //давление(pressure)
byte iot_W_fld_send = 0; //скорость ветра (wind speed)
byte iot_R_fld_send = 0; //дождь мм(rain)

//read
unsigned long iot_id_read = 0;
const char * iot_key_read = "";
unsigned long iot_period_read = 0; //period send sec>=20
byte iot_T_fld_read = 0; //temp квартира(room);ном поля(num field) bme280
byte iot_H_fld_read = 0; //humidity квартира(room) ;ном поля(num field) bme280
byte iot_T2_fld_read = 0; //temp улица(street)
byte iot_P_fld_read = 0; //давление(pressure)
#endif


//=================================================================================================



#ifdef ota_update
#include <ArduinoOTA.h>
#endif




String fn_cmd = ""; //stop play next prev
String fn_cmd_tmp = ""; //tmp
//---------------------------------



//IR ESP8266========================================
#if defined(pin_ir) && defined(ESP8266)
//IR========================================
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
IRrecv irrecv(pin_ir); // указываем вывод, к которому подключен приемник
decode_results results;
unsigned long ir_code;
#endif

//IR ESP32========================================
#if defined(pin_ir) && defined(ESP32)
//IR==============================================
#include <IRremote.h>
IRrecv irrecv(pin_ir); // указываем вывод, к которому подключен приемник
decode_results results;
unsigned long ir_code;
#endif
//---------------------------------------------


#include "SPI.h"
#include <JPEGDecoder.h>
//#define FORMAT_SPIFFS_IF_FAILED true


const char* password_ap = "11111111";//ap
const char* ssid_ap = "ESP_METEO";//ap
const char* passw_new = "";//ap
const char* ssid_new = "";//ap
IPAddress local_ip(192, 168, 11, 11);
IPAddress gateway(192, 168, 11, 1);
IPAddress netmask(255, 255, 255, 0);





//=================================================================================
//              display
//=================================================================================
bool tft2_act = false; //метка что в данный момент активет tft 2 для jpeg

#if defined (two_tft) && defined (tft_320_240)
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
Adafruit_ILI9341 tft2 = Adafruit_ILI9341(TFT_CS2, TFT_DC2, TFT_MOSI2, TFT_CLK2, TFT_RST2, TFT_MISO2);
#define setFreeFont setFont
#elif defined (two_tft) && defined (tft_480_320)
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <ILI9488.h>
ILI9488 tft = ILI9488(TFT_CS, TFT_DC, TFT_RST);
//Adafruit_ILI9341 tft2 = Adafruit_ILI9341(TFT_CS2, TFT_DC2, TFT_MOSI2, TFT_CLK2, TFT_RST2, TFT_MISO2);
ILI9488 tft2 = ILI9488(TFT_CS2, TFT_DC2, TFT_MOSI2, TFT_CLK2, TFT_RST2, TFT_MISO2);
#define setFreeFont setFont
#elif defined (tft_400_300)
/*
  #ifdef pin_sd
  #define ENABLE_GxEPD2_GFX 1
  #include <GxEPD2_GFX.h>
  #include <GxEPD2_BW.h>
  //#include <GxEPD2_3C.h>
  GxEPD2_BW<GxEPD2_420, GxEPD2_420::HEIGHT> tft (GxEPD2_420( EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));
  #else //sd
  #define ENABLE_GxEPD2_GFX 1
  #include <GxEPD2_4G_4G.h>
  #include <GxEPD2_4G_BW.h>
  GxEPD2_4G_4G<GxEPD2_420, GxEPD2_420::HEIGHT> tft(GxEPD2_420( EPD_CS, EPD_DC,  EPD_RST, EPD_BUSY));
  #endif //no sd
*/
#define ENABLE_GxEPD2_GFX 1
#include <GxEPD2_GFX.h>
#include <GxEPD2_BW.h>
//#include <GxEPD2_3C.h>
GxEPD2_BW<GxEPD2_420, GxEPD2_420::HEIGHT> tft (GxEPD2_420( EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));

#define setFreeFont setFont

#else
#include "TFT_eSPI.h"
#ifdef tft_480_320
TFT_eSPI tft = TFT_eSPI(320, 480);
#else
TFT_eSPI tft = TFT_eSPI(240, 320);
#endif //TFT_eSPI
#endif //ifndef (two_tft) 



#include "h\CourierCyr6.h"
#include "h\CourierCyr9.h"
#include "h\verdana9.h"
#ifdef epd
#include "h\FreeMonoBold7.h"
#define v7b &FreeMonoBold7pt8b
#include "h\FreeMonoBold9.h"
#include "h\FreeMonoBold12.h"
#define v12b &FreeMonoBold12pt7b
#include "h\meteo12.h"
#define m12 &meteo12pt8b
#include "h\meteo18.h"
#define m18 &meteo18pt8b
#include "h\meteo26.h"
#define m26 &meteo26pt7b //icon Wh
#endif //epd

#ifdef two_tft //adafruit
#include "h\FreeMonoBold9.h"
#endif

#include "h\verdana12.h"
#include "h\verdana18.h"
#include "h\FreeMonoBold18.h"
#include "h\verdana24.h"
#include "h\verdana24b.h"



#define v6 &CourierCyr6pt8b
#define v9mono &CourierCyr9pt8b
#define v9b &FreeMonoBold9pt7b
//#define v9 &CourierCyr9pt8b
#define v9 &verdana9pt7b
#define v12 &verdana12pt7b
#define v18 &verdana18pt7b
#define v18b &FreeMonoBold18pt8b
#define v24 &verdana24pt7b
#define v24b &verdanab24pt7b



//=========================================
#ifdef pin_dht
//=========================================
#include "DHT.h"
DHT dht(pin_dht, DHTTYPE);
#endif
//------------------------------------------



//=========================================
#ifdef si7021
//=========================================
#include "SparkFun_Si7021_Breakout_Library.h"
#include <Wire.h>
Weather sensor_si;
#endif
//------------------------------------------

//=========================================
#ifdef bme280
//=========================================
#include <Wire.h>
#include <BME280I2C.h>
BME280I2C sensor_bme;    // Default : forced mode, standby time = 1000 ms
bool status_bme = false;
#endif
//------------------------------------------
//=========================================
#ifdef aht10
//=========================================
#include <Wire.h>
#include <AHT10.h>
AHT10 sensor_aht(aht10);
bool status_aht = false;
#endif
//------------------------------------------


//=========================================
#ifdef pin_ds1820
//=========================================
#include <OneWire.h>
#include <DallasTemperature.h>
OneWire oneWire(pin_ds1820);                                       // вход датчиков 18b20, аналоговый А1, он же 15 цифровой
DallasTemperature ds(&oneWire);
#endif
//-------------------------------------------
bool ref_temp_u = false;


const byte alert_max = 10; //nrf 433
byte alert_arr_id[alert_max] = {0};
bool  alert = true;

//=========================================
#ifdef nrf
//=========================================
#include <GParser.h>

#include <SPI.h>

#include "nRF24L01.h"
#include "RF24.h"
#ifdef ESP32
//SPIClass * vspi = NULL;
SPIClass* hspi = nullptr;
#define nrf_SCLK  14//18
#define nrf_MISO  12//19
#define nrf_MOSI  13//23
#endif //32
RF24 radio(pin_nrf_CE, pin_nrf_CS); // "создать" модуль на пинах 9 и 10 Для Уно
bool status_nrf = false;
#endif
//------------------------------------------

IPAddress timeServerIP;

///NTP===========================================
#include <WiFiUdp.h>
///NTP===========================================

byte err_ntp = 1;
byte ntp_count = 0;
unsigned int  localPort = 2390;      // local port to listen for UDP packets
unsigned long  ntp_time = 0;
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[ NTP_PACKET_SIZE];
byte ntp_num = 1;
String t_sun_v = "", t_sun_z = "";
#if defined (pin_led)||defined(pin_sunset_out_hight)||defined(pin_sunset_out_low)
int t_sun_v_min = 0, t_sun_z_min = 0;
#endif

int t_sun_v_hh = 99, t_sun_z_hh = 99;




WiFiUDP udp;


//send WIFI read--------------------------------
//=====================================
#if defined (wf_send)||  defined (wf_read)
//=====================================
IPAddress broadcastAddress;
WiFiUDP udp_wf;
#endif
//---------------------------------------

bool nigth = false;
time_t t;
bool refresh_all = true; //флаг что есть новые данными надо перерисовать

//SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void setup() {

  Serial.begin(115200);



  //----------------------PIN-OUT-ON-PLAY---------------------
  //-----------------HIGH
#ifdef pin_play_WWW_MP3_out_hight
  pinMode(pin_play_WWW_MP3_out_hight, OUTPUT);
  digitalWrite(pin_play_WWW_MP3_out_hight, LOW);
#endif //
#ifdef pin_play_MP3_out_hight
  pinMode(pin_play_MP3_out_hight, OUTPUT);
  digitalWrite(pin_play_MP3_out_hight, LOW);
#endif //
#ifdef pin_play_WWW_out_hight
  pinMode(pin_play_WWW_out_hight, OUTPUT);
  digitalWrite(pin_play_WWW_out_hight, LOW);
#endif //



#ifdef pin_playFM_out_hight
  pinMode(pin_playFM_out_hight, OUTPUT);
  digitalWrite(pin_playFM_out_hight, LOW);
#endif //
#ifdef pin_playALL_out_hight
  pinMode(pin_playALL_out_hight, OUTPUT);
  digitalWrite(pin_playALL_out_hight, LOW);
#endif //
#ifdef pin_night_out_hight
  pinMode(pin_night_out_hight, OUTPUT);
  digitalWrite(pin_night_out_hight, LOW);
#endif
#ifdef pin_sunset_out_hight
  pinMode(pin_sunset_out_hight, OUTPUT);
  digitalWrite(pin_sunset_out_hight, LOW);
#endif

  //-----------------LOW
#ifdef pin_play_WWW_MP3_out_low
  pinMode(pin_play_WWW_MP3_out_low, OUTPUT);
  digitalWrite(pin_play_WWW_MP3_out_low, HIGH);
#endif //
#ifdef pin_play_WWW_out_low
  pinMode(pin_play_WWW_out_low, OUTPUT);
  digitalWrite(pin_play_WWW_out_low, HIGH);
#endif //
#ifdef pin_play_MP3_out_low
  pinMode(pin_play_MP3_out_low, OUTPUT);
  digitalWrite(pin_play_MP3_out_low, HIGH);
#endif //

#ifdef pin_playFM_out_low
  pinMode(pin_playFM_out_low, OUTPUT);
  digitalWrite(pin_playFM_out_low, HIGH);
#endif //
#ifdef pin_playALL_out_low
  pinMode(pin_playALL_out_low, OUTPUT);
  digitalWrite(pin_playALL_out_low, HIGH);
#endif //
#ifdef pin_night_out_low
  pinMode(pin_night_out_low, OUTPUT);
  digitalWrite(pin_night_out_low, HIGH);
#endif
#ifdef pin_sunset_out_low
  pinMode(pin_sunset_out_low, OUTPUT);
  digitalWrite(pin_sunset_out_low, HIGH);
#endif




  //---------------------------------PIR-----------------------------
#if defined(pin_pir)
  pinMode(pin_pir, INPUT);
  digitalWrite( pin_pir, LOW);
  delay(500);
#endif

#ifdef pin_pir_out_hight
  pinMode(pin_pir_out_hight, OUTPUT);
  digitalWrite(pin_pir_out_hight, LOW);
#endif

#ifdef pin_pir_out_low
  pinMode(pin_pir_out_low, OUTPUT);
  digitalWrite(pin_pir_out_low, HIGH);
#endif


#if defined (pin_led) && defined (ESP32)
  pinMode(pin_led, OUTPUT);
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(pin_led, ledChannel);
  do_led(led_def);
#endif
#if defined (pin_led) && defined (ESP8266)
  pinMode(pin_led, OUTPUT);
  do_led(led_def);
#endif


  //  digitalWrite(TFT_CS, HIGH); // TFT screen chip select

#ifdef pin_sd
  digitalWrite(pin_sd, HIGH);
#endif


  //----------------------------------------TFT
#ifdef  two_tft
  tft2.begin();
  tft2.fillScreen(TFT_BLACK );
  tft2.setRotation(rotation2);
#endif

#if defined (tft_400_300)
  //tft.init(115200);
  tft.init(0);
  tft.setRotation(epd_rotation);
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK);
  tft.setFullWindow();
  do_show_define();
  tft.display(false);
#else
  tft.begin();
  tft.fillScreen(TFT_BLACK );
  tft.setRotation(rotation);
  tft.setTextWrap(0);
  //  tft.setCursor(0, 10);
  tft.setFreeFont(v6);
  //  tft.setTextColor(0x07E0);
  do_show_define();
#endif

#if !defined (EN) && !defined (two_tft)&& !defined (epd)
  tft.setAttribute(UTF8_SWITCH , false); //on utf-8
#endif

#if (!defined (EN) && defined (two_tft))|| defined (epd)
  //adafruit
  tft2.cp437(true);
#endif




#ifdef pin_sd
  while (!SD.begin(pin_sd))
  {
    //  tft.setTextColor(TFT_RED);
    Serial.println(F("SD ERR"));
    //   tft.println("SD ERR");
    print_info("SD_ERR");
    delay(2000);
  }//while
  status_sd = true;
  print_info("SD_OK");
  Serial.println(F("SD ok"));
  // tft.setTextColor(0x07E0);
#ifdef Sf
  max_jpg_num = do_maxnum_file_foto();
  Serial.print(F("max jpeg num="));
  Serial.println(max_jpg_num);
#endif //Sf
  //--------------mp3--------------
#ifdef ESP32
  do_maxnum_file_mp3();

  if (max_num_mp3 == 0)
  {
    Serial.println(F("ERR no mp3"));
  }
  else
  {
#ifdef debug
    Serial.print(F("max MP3 num="));
    Serial.println(max_num_mp3);
    Serial.print("track_min=");
    Serial.println(min_num_mp3);
#endif //debug
  }
#endif //esp32 mp3
  print_info("SD");
#else //SD
  SPIFFS.begin();
#endif
#ifdef debug
  do_log_max_size();//del big log
#endif
  // tft.setTextColor(0x07E0);

  if (do_read_config_txt())
    print_info("config OK");
  else
  {
    print_info("config.txt ERR");
    //   tft.setTextColor(0x07E0);
    delay(5000);
  }



  //----------------------------------pin_analog_key1------------------------
#ifdef ESP32
#if defined( pin_analog_key1)|| defined( pin_analog_key2)

#ifdef key1btn_1
  if (key1_debounce != 0)   key1btn1.setDebounce(key1_debounce);
  if (key1_timeout != 0)   key1btn1.setTimeout(key1_timeout);
  if (key1_clicktimeout != 0)   key1btn1.setClickTimeout(key1_clicktimeout);
  if (key1_steptimeout != 0)   key1btn1.setStepTimeout(key1_steptimeout);

#endif


#ifdef key1btn_2
  if (key1_debounce != 0)
  {
    //   Serial.print(F("key1_debounce = "));
    //    Serial.println(key1_debounce);
    key1btn2.setDebounce(key1_debounce);
  }
  if (key1_timeout != 0)   key1btn2.setTimeout(key1_timeout);
  if (key1_clicktimeout != 0)   key1btn2.setClickTimeout(key1_clicktimeout);
  if (key1_steptimeout != 0)   key1btn2.setStepTimeout(key1_steptimeout);
#endif


#ifdef key1btn_3
  if (key1_debounce != 0)   key1btn3.setDebounce(key1_debounce);
  if (key1_timeout != 0)   key1btn3.setTimeout(key1_timeout);
  if (key1_clicktimeout != 0)   key1btn3.setClickTimeout(key1_clicktimeout);
  if (key1_steptimeout != 0)   key1btn3.setStepTimeout(key1_steptimeout);
#endif


#ifdef key1btn_4
  if (key1_debounce != 0)   key1btn4.setDebounce(key1_debounce);
  if (key1_timeout != 0)   key1btn4.setTimeout(key1_timeout);
  if (key1_clicktimeout != 0)   key1btn4.setClickTimeout(key1_clicktimeout);
  if (key1_steptimeout != 0)   key1btn4.setStepTimeout(key1_steptimeout);
#endif


#ifdef key1btn_5
  if (key1_debounce != 0)   key1btn5.setDebounce(key1_debounce);
  if (key1_timeout != 0)   key1btn5.setTimeout(key1_timeout);
  if (key1_clicktimeout != 0)   key1btn5.setClickTimeout(key1_clicktimeout);
  if (key1_steptimeout != 0)   key1btn5.setStepTimeout(key1_steptimeout);
#endif
#endif
#endif //32


  //==================================fmSi4703

#ifdef  fmSi4703_lib2
  Wire.begin(pin_fm_sdio, pin_fm_sclk);
  FM.setup(pin_fm_rst, pin_fm_sdio);
  fm_setVolume(0);
  delay(500);
  fm_setFreq(freq_fm);
  FM.setRds(false);
  Serial.print(F("fmSi4703 lib2")); Serial.println(freq_fm);
#endif  //fmSi4703 lib2

#ifdef  fmSi4703
  Serial.print(F("fmSi4703 fr=")); Serial.println(freq_fm);
  FM.powerOn();
  fm_setVolume(0);
  fm_setFreq(freq_fm);
#endif  //fmSi4703

#ifdef fmRDA5807
  Wire.begin(pin_fm_sdio, pin_fm_sclk);
  //FM.setup();
  FM.init();
  Serial.print(F("fmRDA5 fr=")); Serial.println(freq_fm);
  // FM.setVolume(0);
#ifdef  set_audio_mono_fm
  FM.setMono(true);
#else
  FM.setMono(false);
#endif
  FM.setMute(true);
  delay(500);
#endif  //fmRDA

#if defined (pin_led) //после config.txt
  Serial.print(F("do_led"));
  do_led(led_def);

#endif


  log_str = (F("--------------ESP--boot"));
  do_log();



  //==================================
#ifdef bme280
  //==================================
  Wire.begin();
  if (!sensor_bme.begin())
  {
    log_str = (F("ERR BME280"));
  }
  else
  {
    status_bme = true;
    log_str = (F("BME280 OK"));
  }
  do_log();
  print_info("BME280");
  //==================================
#endif //bme


  //==================================
#ifdef aht10
  //==================================
  Wire.begin();
  if (!sensor_aht.begin())
  {
    log_str = (F("ERR AHT10"));
  }
  else
  {
    status_aht = true;
    log_str = (F("AHT10 OK"));
  }
  do_log();
  print_info("AHT10");
#endif //AHT10

  //==================================
#ifdef si7021
  //==================================
  Serial.println("#def Si7021 or HTU21D");
  sensor_si.begin();
#endif //si




  //----------------------------AUDIO----------------PCM
#ifdef ESP32 //radio
  // make sure we don't get killed for our long running tasks
#ifdef def_core0
  esp_task_wdt_init(10, false);
#endif

#if defined (vs1053)&&!defined(def_core0)
  Serial.println(F("vs1053"));
  audio.begin();
  uint32_t chipID = audio.printChipID();
  if (chipID == 0x00000000 || chipID == 0xFFFFFFFF)
  {
    Serial.println("Error: VS1053 not found");
  }
  else
  {
    status_vs1053 = true;
    Serial.print((F"VS1053 chipID ")); Serial.println(chipID);
    audio.setVolume(0);
  }
  print_info("VS1053");
#endif

  //--------
#if defined (audio_lib2)&& defined (PCM5102)
#ifndef def_core0
  Serial.println(F("lib2 PCM5102 No core0"));
  audio.setPinout(bclkPin, wclkPin, doutPin);
  audio.setVolume(5); // 0...21
#ifdef set_audio_mono
  audio.forceMono(true);
#endif //mono
#endif  //c0
#endif  //lib2

#if defined (audio_lib2)&& !defined (PCM5102) //no pcm
  Serial.println(F("lib2 setInternalDAC"));
#endif //i dac
  //-----------------------------audiolib1
  //
#if !defined (audio_lib2)&& !defined (PCM5102)&& !defined (vs1053) //no pcm
  // First, preallocate all the memory needed for the buffering and codecs, never to be freed
  Serial.println(F("preallocate the memory"));
  preallocateBuffer = malloc(preallocateBufferSize);
  preallocateCodec = malloc(preallocateCodecSize);
  if (!preallocateBuffer || !preallocateCodec)
  {
    Serial.printf_P(PSTR("FATAL ERROR:  Unable to preallocate %d bytes for app\n"), preallocateBufferSize + preallocateCodecSize);
    while (1) delay(1000); // Infinite halt
  }

#ifdef PCM5102
  out = new AudioOutputI2S(0, 0); //dac
  out->SetPinout(bclkPin, wclkPin, doutPin);
  Serial.println(F("PCM5102 EXT. DAC"));
  log_str = (F("PCM5102"));
  do_log();

#else
  out = new AudioOutputI2S(0, 1);
  Serial.println(F("ESP32  internal DAC"));
  //  tft.println(F("internal DAC"));
  log_str = (F("internal DAC"));
  do_log();
#endif

  file = NULL;
  buff = NULL;
  mp3 = NULL;
  file_mp3 = NULL;
#endif //lib1
  do_count_pl_list();
  print_info("PL");
#endif //ESP32

  //----------------



#ifdef pin_btn_touch
  log_str = ("pin_btn_touch=") + String(pin_btn_touch);
  do_log();
#endif


  //==================================
#ifdef pin_rele
  //==================================
  pinMode(pin_rele, OUTPUT);
  digitalWrite(pin_rele, rele_on_off);
  Serial.println ("RELE ok");

  log_str = (F("pin_rele=")) + String(pin_rele);
  do_log();

#endif
  //----------------------------------


  //==================================
#ifdef ds1820
  //==================================
  ds.begin();
  delay(500);
  log_str = (F("ds1820"));
  do_log();

#endif
  //-----------------------------------



  //==================================
#ifdef pin_ir
  //==================================
  irrecv.enableIRIn(); // запускаем прием
  if (do_read_file_ik())
  {
    Serial.println (F("Ir  OK"));
    log_str = (F("ir Ok"));
  }
  else
  {
    tft.setTextColor(TFT_RED);
    tft.println("read ik.txt   ERR ");
    tft.setTextColor(0x07E0);
    log_str = (F("read ik.txt   ERR"));
    delay(5000);

  }
  do_log();
#endif





  //------------------------------------
  //==================================
#if defined( pin_beep)
  //==================================
  pinMode(pin_beep, OUTPUT); //объявляем пин beep как выход
  digitalWrite(pin_beep, LOW );
  log_str = (F("beep OUTPUT"));
  do_log();
#endif
  //----------------------------------





#ifdef openw
  url_ch = url_ch_b + lat + "&lon=" + lon + "&appid=" + appid + "&cnt=" + String(max_ch3);
  url_now = url_now_b + lat + "&lon=" + lon + "&appid=" + appid;
  //UV
  url_uv = url_uv_b + appid + "&lat=" + lat + "&lon=" + lon;



#ifdef debug
  Serial.print(F("url_ch="));
  Serial.println(url_ch);
  Serial.print(F("url_now="));
  Serial.println(url_now);
  Serial.print(F("url_uv="));
  Serial.println(url_uv);
#endif

#endif


  //WIFI=======WIFI=======WIFI=======WIFI=======WIFI=======WIFI=======WIFI=======
  wifiConnect(false);
  print_info("IP");
  //WIFI=======WIFI=======WIFI=======WIFI=======WIFI=======WIFI=======WIFI=======


  //===========IoT==============IoT==================IoT
#ifdef IoT
  ThingSpeak.begin(client_iot);  // Initialize ThingSpeak

  log_str = (F("IoT"));
  do_log();

#endif

  ///--------------------mqtt--------------------------------------------------
#ifdef mqtt
  client_mqtt.setServer(mqtt_server, mqtt_port);
  client_mqtt.setCallback(callback_mqtt);

  log_str = (F("mqtt"));
  do_log();


#endif //mqtt





#if defined (http_update) && defined(ESP8266)
  httpUpdater.setup(&server, update_path);
#endif

#if defined (http_update) && defined(ESP32)
  server.on(update_path, HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverIndex);
  });
  /*handling uploading firmware file */
  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      /* flashing firmware to ESP*/
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });
#endif






  server.begin();
  Serial.println(F("HTTP server started"));


  if (WiFi.getMode() == WIFI_AP)
    goto end_setup;

  //===========================================
  delay(2000);


#ifdef ota_update
  //=====OTA==========================================================
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.setHostname(ota_name);
  ArduinoOTA.begin();

#endif


  //====FTP================================================
#ifdef ftp
  ftpSrv.begin("esp8266", "esp8266");   //username, password for ftp.
  Serial.println(F("ftp server started esp8266/esp8266"));
  //-----------------------------------------------------
#endif


  // tft.setTextColor(0x07E0);


  //===UDP SEND READ====================================
#if defined (wf_send)||  defined (wf_read)
  broadcastAddress = (uint32_t)WiFi.localIP() | ~((uint32_t)WiFi.subnetMask());
  Serial.print(F("Broadcast address: "));
  Serial.println(broadcastAddress);
  // tft.println();
  // tft.print(F("bct="));
  // tft.setTextColor(TFT_YELLOW);
  // tft.println(broadcastAddress);
  // tft.print("port=");
  // tft.println(localPort_wf);
  udp_wf.begin(localPort_wf);
#endif
  //-------------------------------------------
  Serial.println(F("Starting NTP"));


  //NTP=======================================
  udp.begin(localPort);
  delay(200);
  //--------------------------------------------

#ifdef debug
  do_log_max_size();//del big log
#endif


#ifdef only_radio
  fn_cmd = "play";
  do_fn_btn_ir();
#endif

#ifdef openw
  do_delta_tz();//поправка TZ для начала суток, если TZ не кратна 3
#endif



  //==================================
#ifdef pin_dht
  //==================================
  dht.begin();
  Serial.print(F("DHT pin="));
  Serial.println(pin_dht);
#endif


#ifdef pin_sd
  randomSeed(millis() );
#endif

  // tft.fillScreen(TFT_BLACK);

#ifdef two_tft
  // tft2.fillScreen(TFT_BLACK);
#endif



#ifdef tft_no_invert
  tft.invertDisplay( false );
#endif

  tft.setTextSize(1);

  //encoder
#ifdef CLK
  pinMode (CLK, INPUT_PULLUP);
#endif
#ifdef DT
  pinMode (DT, INPUT_PULLUP);
#endif
#ifdef SW
  pinMode (SW, INPUT_PULLUP);
#endif

#if defined (encoder)&& defined (encoder_int)
  attachInterrupt(digitalPinToInterrupt(CLK), isrCLK, CHANGE ); //CHANGE
  attachInterrupt(digitalPinToInterrupt(DT), isrDT, CHANGE ); //RISING
  //  attachInterrupt(digitalPinToInterrupt(SW), enc_int, CHANGE ); //RISING
#endif

#if defined (encoder_lib2) && defined (ESP32)
#ifdef encType_single//single,half,full  
  enc.attachSingleEdge(CLK, DT);
#elif encType_half
  attachHalfQuad(CLK, DT);
#elif encType_full
  attachFullQuad(CLK, DT);
#endif //type
#if defined (puType)
  ESP32Encoder::useInternalWeakPullResistors = puType;
#endif

  attachInterrupt(SW, isr_enc_btn, CHANGE ); //RISING

#if defined (steptimeout)
  enc_btn.setStepTimeout(steptimeout);
#endif
#if defined (setdebounce)
  enc_btn.setDebounce(setdebounce);// настройка антидребезга (по умолчанию 80 мс)
#endif
#if defined (settimeout)
  enc_btn.setTimeout(settimeout);        // настройка таймаута на удержание (по умолчанию 500 мс)
#endif
#if defined (setclicktimeout)
  enc_btn.setClickTimeout(setclicktimeout);   // настройка таймаута между кликами (по умолчанию 300 мс)
#endif
#if defined (pull)
  enc_btn.setType(pull);
#endif
  enc.clearCount();

#endif



  //---------------------------------LCD-----------------------------
#ifdef lcd_display
  lcd.init();
  lcd.setBacklight(1);
  lcd.print("OK");
  delay(1000);
  // lcd.clear();
  lcd.setBacklight(0);
  lcd_on = false;
#if defined (lcd_clock_b)
  load_num_b() ;
#endif
#if defined (lcd_clock_m)
  load_num_m() ;
#endif
#endif //lcd_display


  //------------------------------------------433
#ifdef R433
  if (!r433.init())
    log_str = (F("ERR R433"));

  else
  {
    log_str = (F("R433 Ok"));
    status_r433 = true;
  }
  do_log();
  print_info("R433");
#endif



  //==================================
#ifdef nrf
  //==================================

#ifdef ESP32
  hspi = new SPIClass(HSPI);
  hspi->begin(nrf_SCLK, nrf_MISO, nrf_MOSI, pin_nrf_CS); //SCLK, MISO, MOSI, SS
  pinMode(pin_nrf_CS, OUTPUT); //VSPI SS
  if (!radio.begin(hspi))
  {
    log_str = (F("ERR NRF24"));
  }
  else
  {
    status_nrf = true;
    log_str = (F("NRF start"));
  }
  do_log();
  print_info("NRF24");
#else //32
  if (!radio.begin())
    log_str = (F("ERR NRF24"));
  else
  {
    status_nrf = true;
    log_str = (F("NRF start"));
  }
  do_log();
  print_info("NRF24");
#endif
  radio.setPayloadSize(32);     //размер пакета, в байтах

  radio.setChannel(nrf_channel);                                       // Указываем канал приёма данных (от 0 до 127), 5 - значит приём данных осуществляется на частоте 2,405 ГГц (на одном канале может быть только 1 приёмник и до 6 передатчиков)
  delay(100);
#if nrf_palevel==1
  radio.setPALevel (RF24_PA_MIN);
#elif nrf_palevel==2
  radio.setPALevel (RF24_PA_LOW);
#elif nrf_palevel==3
  radio.setPALevel (RF24_PA_HIGH);
#elif nrf_palevel==4
  radio.setPALevel (RF24_PA_MAX);
#else
#error "ERR set nrf_palevel "
#endif

  // скорость обмена RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
#if nrf_speed==250
  radio.setDataRate (RF24_250KBPS);
#elif nrf_speed==1000
  radio.setPALevel (RF24_1MBPS);
#elif nrf_speed==2000
  radio.setPALevel (RF24_2MBPS);
#else
#error "ERR set nrf_speed "
#endif

  radio.openReadingPipe (1, 0xAABBCCDD11LL);
  radio.openReadingPipe (2, 0xAABBCCDD22LL);
  radio.openReadingPipe (3, 0xAABBCCDD33LL);
  radio.openReadingPipe (4, 0xAABBCCDD44LL);
  radio.openReadingPipe (5, 0xAABBCCDD55LL);

  radio.powerUp();
  radio.startListening();
  delay(150);
#endif  //nrf


  scr_pack( );

#ifndef openw
#ifdef tft_320_240
  do_find_scr("S0"); //big clock no openw
#endif

#ifdef tft_480_320
  if (!do_find_scr(scr_name_def))
  {
    String val = "Sn";
    byte len = val.length() + 1;
    char buf[len];
    val.toCharArray(buf, len);
    strcpy(scr_name_def, buf);
    do_find_scr("Sn"); //big clock no openw
  }
#endif

#endif  //openw

#ifdef openw
  if (!do_find_scr(scr_name_def))
    scr_number = 0;
#endif

#ifdef pause_after_boot
  delay(pause_after_boot * 1000L);
#endif

  scr_num_show(scr_number);
#ifdef debug
  log_str = (F("-----------end setup"));
  do_log();
#endif


#ifdef def_core0
  audioInit();
#endif



end_setup:;
  server.on("/",         HomePage);
  server.on("/download", File_Download);
  server.on("/upload",   File_Upload);
  server.on("/fupload",  HTTP_POST, []()
  {
    server.send(200);
  }, handleFileUpload);
  server.on("/wifi", wifi_ap);
  server.onNotFound(handleNotFound);

  server.on("/delete",   File_Delete);
#ifdef pin_sd
  server.on("/dir",      SD_dir);
#else
  server.on("/dir",      SPIFFS_dir);
#endif

}

// LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
//                                lOOP
// LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
void loop() {

  if (WiFi.getMode() == WIFI_AP)
  {
    server.handleClient();
    if (millis() > t_stay_ap + wifi_rep_t)
    {

      log_str = (F("ESP reboot after WIFI AP"));
      do_log();

      ESP.restart();
    }
    //если по какой то причине сразу не приконн.
    //перегр через 700sec.
    goto skip_all;
  }
  //--------------AP------------------------

#ifdef pin_btn1
  do_btn1();
#endif
#ifdef pin_btn2
  do_btn2();
#endif
#ifdef pin_btn3
  do_btn3();
#endif
#ifdef pin_btn4
  do_btn4();
#endif


  //----------------------------serial port
#ifdef debug
  do_serial(); //cmd
#endif  //SERIAL





  if (sync_t)//--------------- раз в минуту   ---------------------------
  {
    do_readnow();
#ifdef  time_correct
    now_correct() ;
#endif
    do_every_min();  //shed,beep,scr_sav,restart  t=now();
  }



  //ntp not sync
  if (WiFi.status() == WL_CONNECTED)
  {
    if (!sync_t || sync_force == true) do_ntp();
  }

  if (WiFi.status() != WL_CONNECTED && wifi_force_connect) wifiConnect(wifi_force_connect);


  if (WiFi.status() == WL_CONNECTED && !play && !play_mp3)
  {
    //FTP
#ifdef ftp
    ftpSrv.handleFTP();
#endif
    //WIFI read data
#ifdef wf_read
    do_wf_read();
#endif

  }





#ifdef lcd_display
  draw_lcd();
#endif //lcd_display



  //IR----------------------
#if defined (pin_ir)
  do_read_ir();
#endif


  //Encoder--------------------
#ifdef encoder
  do_encoder();
#endif

#if defined (encoder_lib2) && defined (ESP32)
  do_encoder();
#endif




#if defined( pin_analog_key1)|| defined( pin_analog_key2)
  do_4key();
#endif

  //wwww
  if (!play && !play_mp3)
  {
    server.handleClient();
  }



  if (t_timer > 0) //приоритет ручного таймера
    control_timer();
#ifdef ESP32
  else if (t_timer_scr == 0 && !timer_alarm && t_timer_alarm > 0) //приоритет ручного таймера
    control_timer_alarm();
#endif //32



  //-------------------Sensors------------- --------------

#ifdef  pin_ds1820
  do_read_ds1820();
#endif
#ifdef bme280
  do_read_bme();
#endif
#ifdef aht10
  do_read_aht();
#endif
#ifdef si7021
  do_read_si7021();
#endif

#ifdef pin_dht
  do_read_dht();
#endif


#ifndef epd
  do_c_frame() ;//color frame Tu Tkv
#endif //#ifndef epd




  //PIR----------------------
#if defined(pin_pir)
  do_read_pir();
#endif

  //  pin_btn------------------------
#if defined (pin_btn_touch) && defined( ESP32)
  do_touch_btn(1);
#endif







  // openw  before t=now()---------------
#ifdef openw
  if (!play && !play_mp3)
  {
#ifdef timechange_summer_winter
    if (sync_t) do_openw(); //zapros openW timezone from ntp
#else
    do_openw();
#endif
  }

#endif  //ow


#ifdef tft_400_300
  if (scr_name[scr_number] == "S1" ) S1_epd();
  if (scr_name[scr_number] == "S2" ) S2_epd();
  if (scr_name[scr_number] == "S3" ) S3_epd();
#ifdef Sfepd
  if (scr_name[scr_number] == "Sf") Sf_epd();
#endif
#endif


#ifdef only_radio //если  одно радио опрос лок. сенсоров
  read_sensors_radio();
#endif //radio



  //ulica
#ifdef nrf
  do_read_ulica();
  if (alert) do_alert();
#endif
#ifdef R433
  do_read_ulica();
  if (alert) do_alert();
#endif

  //========================================================================
  //            IF  NOT PLAY
  //========================================================================
#ifndef only_radio //если  одно радио этот блок искл.

  if ((!play  && !play_fm && !play_mp3 && !scr_timer && !s_info) || play_bg)
  {



    //ioT  ThingSpeak
#ifdef IoT
    if ( WiFi.status() == WL_CONNECTED)
    {
      if (iot_id > 0) do_send_iot();
      if (iot_id_read > 0) do_read_iot();
    }
#endif

    //mqtt
#ifdef mqtt
    if ( WiFi.status() == WL_CONNECTED)
    {
      if (!client_mqtt.connected())     reconn_mqtt();

      if (client_mqtt.connected())
      {
        do_send_mqtt();
        client_mqtt.loop();
      }
    }
#endif




    ///OTA
#ifdef ota_update
    if (WiFi.status() == WL_CONNECTED) ArduinoOTA.handle();
#endif


    //ntp
    do_ntp();

    /*
        //ulica
      #ifdef nrf
        do_read_ulica();
      #endif
    */
#ifdef wf_send
    if (WiFi.status() == WL_CONNECTED) do_wf_send();
#endif



    //-----------scr saver----------------------
    if (t_ref_tft > 0 && t_ref_tft < 60 && now_min % t_ref_tft == 0)
    {
      if (now_min != ref_tft_last)
      {
        refresh_all = true;
        tft.fillScreen(TFT_BLACK);
        ref_tft_last = now_min;
      }
    }
    //----------------


#ifdef Sn
    //nnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn
    if (scr_name_def != "Sn" && (now_hour >= t_b && now_hour < t_e && t_b < t_e) || (now_hour >= t_b && now_hour > t_e && t_b > t_e) || (now_hour < t_b && now_hour < t_e && t_b > t_e))
    {
      if (nigth == false) //1 raz
      {

        if (scr_name[scr_number] == "Sn" )
          do_find_scr(scr_name_def);// //если найдет будет по умолч иначе ночной останется
        scr_number_prev = scr_number;

        nigth = true;
        if (do_find_scr( (char *)"Sn"))
          scr_num_show(scr_number);//refresh tft
#if defined(pin_night_out_hight)||defined(pin_night_out_low)
#if defined(pin_night_out_hight)
        digitalWrite(pin_night_out_hight, HIGH);
#endif
#if defined(pin_night_out_low)
        digitalWrite(pin_night_out_low, LOW);
#endif
#endif//pin_night_out

      }

    }
    else   //день

      if (nigth == true)
      {
        //выход с ноч часов
        nigth = false;
        scr_num_show(scr_number_prev);
#if defined(pin_night_out_hight)||defined(pin_night_out_low)
#if defined(pin_night_out_hight)
        digitalWrite(pin_night_out_hight, LOW );
#endif
#if defined(pin_night_out_low)
        digitalWrite(pin_night_out_low, HIGH);
#endif
#endif//pin_night_out
      }
#endif

    //экраны по кругу----RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR
#ifndef epd
#ifdef  openw
    if (!nigth && scr_name[scr_number] != "Sn" && scr_show_count > 1 && scr_round)
      do_scr_round();
#endif
#endif //#ifndef epd

#if !defined (openw) &&   defined (tft_320_240)
    if (!nigth && scr_name[scr_number] != "Sn" && scr_show_count > 1 && scr_round)
      do_scr_round();
#endif

#if !defined (openw) &&   defined (tft_480_320)
    if (!nigth && scr_show_count > 1 && scr_round)
      do_scr_round();
#endif


    //------------------------------3 2 0 x 240 ----------------------------
    //------------------------------3 2 0 x 240 ----------------------------


#if defined (S0) && defined(tft_320_240) && !defined (openw)
    if (scr_name[scr_number] == "S0") S0_time();
#endif


#if defined (openw) && defined(tft_320_240)

#if defined (S1)
    if (scr_name[scr_number] == "S1") S1_time();
#endif

    //--------S2-----------------tft_320_240
#if defined (S2)
    if (scr_name[scr_number] == "S2")
    {
      S2_time();
    }
#endif //S2 

    //--------S3-----------------tft_320_240
#if defined (S3)
#if defined (tft2_S3)
    if (ref_wh == true )
    {
      tft2_act = true;
      S3_show();
      tft2_act = false;
    }
    //#endif
#else
    if (scr_name[scr_number] == "S3" && (ref_wh == true || refresh_all == true))
    {
      S3_show();
    }
#endif
#endif //S3 tft_320_240



    //-------------S4--------tft_320_240
#if defined (S4)
#if defined (tft2_S4)
    if (ref_wh == true)
    {
      tft2_act = true;
      S4_show_ch();
      tft2_act = false;
    }
    //#endif
#else

    if (scr_name[scr_number] == "S4" && (ref_wh == true || refresh_all == true))
      S4_show_ch();

#endif

#endif //S4


#if defined (S5)
    if (scr_name[scr_number] == "S5") S5_time();
#endif

    //--------S6-----------------tft_320_240
#if defined (S6)
#if defined (tft2_S6)
    tft2_act = true;
    S6_time();
    tft2_act = false;
#else

    if (scr_name[scr_number] == "S6")
    {
      S6_time();
    }
#endif
#endif //S6 tft_320_240 

#endif //-----openw-------------- tft_320_240 


    //------------------------------4 8 0 x 3 2 0----------
    //------------------------------4 8 0 x 3 2 0----------

#if defined(tft_480_320)&& defined (openw)


#if defined (S1)
#if  defined(tft2_S1)
    tft2_act = true;
    S1_time(); //no ref on cmd next or prev
    tft2_act = false;
#else
    if (scr_name[scr_number] == "S1") S1_time();
#endif
#endif

#if defined (S2)
#if  defined(tft2_S2)
    tft2_act = true;
    S2_time(); //no ref on cmd next or prev    S2_time();
    tft2_act = false;
#else
    if (scr_name[scr_number] == "S2") S2_time();
#endif
#endif //S2 


#if defined (S3)
#if  defined(tft2_S3)
    tft2_act = true;
    S3_time();
    tft2_act = false;
#else
    if (scr_name[scr_number] == "S3" )
    {
      S3_time();
    }
#endif
#endif //S3 

#if defined (S4)
#if  defined(tft2_S4)
    tft2_act = true;
    S4_time();
    tft2_act = false;
#else
    if (scr_name[scr_number] == "S4")
    {
      S4_time();
    }
#endif
#endif //S4


#if defined (S5)
    if (scr_name[scr_number] == "S5")
    {
      S5_time();
    }
#endif //S5

#endif //tft_480_320

    //--E--------------------4 8 0 x 3 2 0----------




#if defined (Sn)
    if (scr_name[scr_number] == "Sn") Sn_time();
#endif

#if defined (pin_sd) && defined (Sf)&& !defined (epd)
    if (scr_name[scr_number] == "Sf") Sf_time();
#endif

    //-------------------Sg----------------------------------
#if defined (Sg) && defined (openw)

#ifdef tft2_Sg
    if (ref_wh == true)
    {
      tft2_act = true;
      Sg_show();
      tft2_act = false;
    }
    //#endif
#else
    if (scr_name[scr_number] == "Sg" && (ref_wh == true || refresh_all == true))
      Sg_show();
#endif


#endif //Sg



    //-------------------scr_round----------------------------------
    if (scr_show_count > 1 && !scr_round && second(t) % 5 == 0 ) draw_stop_round(); //draw stop around
  }
  //---------------------IF  NOT PLAY

#endif //not radio



#if !defined(tft_400_300)&&!defined(no_tft)
  if (scr_timer)
  {
    do_scr_timer();
    if ( timer_alarm)
    {
#ifdef ESP32
      if (tm_alarm_count > 0)      do_timer_alarm();
#endif //32      
#ifdef pin_beep
      if (tm_alarm_count_beep > 0)  do_timer_alarm_beep();
#endif
    }
  }
#endif //!defined(tft_400_300)


  //===============================================
  //radio
  //===============================================
#ifdef ESP32

  if (play && !play_bg)
  {
#ifndef no_tft
    do_scr_play();
#endif

#if defined(audio_lib2)||defined(vs1053)
#ifndef def_core0
    audio.loop();
#endif //c0
#else
    do_play();
#endif
  }
#endif  //esp32


#if defined (fmSi4703)|| defined (fmRDA5807) || defined (fmSi4703_lib2)
  if (play_fm)
  {
    do_scr_play_fm();

  }
#endif   //fmSi4703


#if defined (pin_sd)&&defined (ESP32)
  if (play_mp3)
  {
#ifndef no_tft
    do_scr_play_mp3();
#endif

#if defined(audio_lib2)||defined(vs1053)
#ifndef def_core0
    audio.loop();
#endif //c0
#else
    do_play_mp3(); //lib1
#endif
  }
#endif   //pin_sd mp3



#ifdef pin_ir
  if (refresh_all)
  {
    irrecv.resume(); // принимаем следующую команду

  }
#endif


  refresh_all = false;
  ref_kv_th = false;
  ref_temp_u = false;
  ref_wh = false;
  ref_wh_now = false;

  fn_cmd_tmp = "";
skip_all:;

}

// E---loop---EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE




//------------------------------------------------------------------------------------
//                        do_every_min
//------------------------------------------------------------------------------------

void do_every_min()
{
  byte mm = now_min;
  byte hh = now_hour;




  //mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
  if (last_minute != mm)
  {
    last_minute = mm;
#ifdef debug
    if (!play) serial_time_start ();
#endif

    //заход
#if defined(pin_sunset_out_hight)||defined(pin_sunset_out_low)
    bool static b_sunset_ = false;
    int t_minute_now = hh * 60 + mm;
    if   (( t_minute_now > t_sun_z_min || t_minute_now < t_sun_v_min) && b_sunset_ == false && t_sun_z_min != 0)
    {

#if defined(pin_sunset_out_hight)
      digitalWrite(pin_sunset_out_hight, HIGH);
#endif
#if defined(pin_sunset_out_low)
      digitalWrite(pin_sunset_out_low, LOW);
#endif

#ifdef debug
      Serial.println(F("pin_sunset_out_ ON"));
#endif
      b_sunset_ = true;
    }
    else if   ( t_minute_now > t_sun_v_min && t_minute_now < t_sun_z_min && t_sun_v_min != 0)
    {

#if defined(pin_sunset_out_hight)
      digitalWrite(pin_sunset_out_hight, LOW);
#endif
#if defined(pin_sunset_out_low)
      digitalWrite(pin_sunset_out_low, HIGH );
#endif



#ifdef debug
      Serial.println(F("pin_sunset_out_ OFF"));
#endif

      b_sunset_ = false;



    }


#endif //pin_sunset


#ifdef pin_led
    bool static b_sunset = false, b_def = false;
    int t_min_now = hh * 60 + mm;
    if (!nigth)
    {

      if   (( t_min_now > t_sun_z_min || t_min_now < t_sun_v_min) && b_sunset == false && t_sun_z_min != 0)
      {
        if (led_on) do_led(led_sunset);
        else
          led_val = led_sunset;


#ifdef debug
        Serial.println(F("SunSet led_sunset"));
        Serial.println(b_sunset);
#endif

        b_sunset = true;
        b_def = false;
      }

      else if   ( t_min_now > t_sun_v_min && t_min_now < t_sun_z_min && b_def == false && t_sun_v_min != 0)
      {

        if (led_on) do_led(led_def);
        else
          led_val = led_def;


#ifdef debug
        Serial.println(F("SunRise led_def"));
        Serial.println(b_def);
#endif


        b_def = true;
        b_sunset = false;



      }

    }
    else  do_led(led_night);


#endif //led               


    //в начале часа принудительно обновляю погоду
#ifdef openw
    if (WiFi.status() == WL_CONNECTED)
    {
      if (mm == 7) lastConnectionTime_ch = 0;
      if (mm == 6) lastConnectionTime_now = 0;
    }
#endif //openw


#if defined (fmSi4703)|| defined (fmRDA5807)|| defined (ESP32)|| defined (fmSi4703_lib2)
    //read file shed alarm WWW FM
    if (!scr_timer)
    {
      if (mm == 0 || mm >= t_next_shed)
        do_read_file_shed();
      else if (!play  && !play_mp3 &&  mm % t_period_read_alarm_txt == 0)
        do_read_file_shed();
    }

#endif
#ifdef ESP32  //mp3 beep

    //--------------- раз в минуту



    //do alarm web radio or beep
    if (beep_mp3_vol != 0 && !nigth && scr_name[scr_number] != "Sn" && mm  == 0 && !play_mp3 && !play) //искл ночь и ночной экран
    {
      if (beep_b < beep_e && hh >= beep_b && hh <= beep_e)
        do_play_beep(0); //0 beep 1 alarm
    }

    //shed

    if (!scr_timer && al.m == mm && (al.label == 'I' || al.label == 'i') && WiFi.status() == WL_CONNECTED )
    {
      start_alarm_www();

    }


#endif //esp32 


#if defined (fmSi4703)|| defined (fmRDA5807)|| defined (fmSi4703_lib2)
    if (!scr_timer && al.m == mm && (al.label == 'F' || al.label == 'f'))
    {
      start_alarm_fm();

    }

#endif //FM


  }


  //hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh

  if (last_hour != hh) //раз в час и была синхр-я
  {
    last_hour = hh;

    //beep
#ifdef pin_beep

    if (beep_b < beep_e && mm == 0 && hh >= beep_b && hh <= beep_e && !nigth && scr_name[scr_number] != "Sn" )
      do_beep(fr, dur);
#endif




    //reset раз в сутки в 2 ч ночи чтобы работало надо после прошивки вручную сделать reset
    if (millis() > 600000 && hh == esp_restarts && mm < 5 && t_timer_scr == 0) //раз в сутки
    {
      Serial.println(F("ESP.reset()"));

      log_str = (F("ESP reboot sheduler"));
      do_log();



#ifdef ESP8266
      ESP.reset();
#endif
#ifdef ESP32
      ESP.restart();
#endif

    }


  }

}


//------------------------------------------------------------------------------------
//                        do_serial
//------------------------------------------------------------------------------------

#ifdef debug
void do_serial()
{
  if (Serial.available()) {
    String r = Serial.readString(); r.trim();
    if (r.length() > 10 && play)
    {
#if defined(audio_lib2)|| defined(vs1053)
#if defined(def_core0)
      audioStopSong(); //+vs1053
      playing = audioConnecttohost(r.c_str());
#else
#if defined(audio_lib2)
      audio.stopSong();
#endif
#if defined(vs1053)
      audio.stop_mp3client();
#endif

      playing = audio.connecttohost(r.c_str());
#endif
#endif //lib2



#if !defined(no_tft)
      do_scr_play();
      refresh_all = true;
#endif

    }
    //      log_i("free heap=%i", ESP.getFreeHeap());
    else if (r.length() > 0)
    {
      if (r == "n") fn_cmd = "next";
      else if (r == "p") fn_cmd = "prev";
      else if (r == "m") fn_cmd = "mute";
      else if (r == "f") fn_cmd = "fav";
      else if (r == "s") fn_cmd = "stop";
      else if (r == "-") fn_cmd = "vl-";
      else if (r == "+") fn_cmd = "vl+";
      else if (r == "nl") fn_cmd = "next_pl";
      else if (r == "pl") fn_cmd = "prev_pl";
      else if (r == "i") fn_cmd = "Scr_info";
      else if (r == "fm") fn_cmd = "play_fm";
      else if (r == "mp") fn_cmd = "play_mp3";
      else if (r == "t" && !play_fm && !play_mp3 && !play && !scr_timer) fn_cmd = "timer";
      else if (r == "rst") fn_cmd = "restart";
      else if (r == "beep0") do_play_beep(0);
      else if (r == "beep1") do_play_beep(1);
      else if (r == "ns") fn_cmd = "next_scr";
      else if (r == "ps") fn_cmd = "prev_scr";
      else if (r == "pbg") fn_cmd = "play_bg";
      else if (r == "alarm") //test 2min ch=1 pl=0
      {
        al.ch_num = 1;
        al.t_long = 120000;
        start_alarm_www();
      }
      else if (r.indexOf("tm;") == 0 && !play_fm && !play_mp3 && !play && !scr_timer)
      {
        String str = r.substring(3);
        Serial.print("TM"); Serial.println(str);
        uint32_t sec_tmp = 60 * str.toInt();
        Serial.print("sec_tmp"); Serial.println(sec_tmp);
        t_timer_boot = now() + sec_tmp;
        fn_cmd = "timer";
        do_fn_btn_ir();

        //   fn_cmd = "timer_start";
        //   do_fn_btn_ir();
      }
      else if (r.toInt() > 0 && r.toInt() < 100 && play)
      {
        ch_fnd = r.toInt();
        fn_cmd = "play";
      }

      else
        fn_cmd = r;
      do_fn_btn_ir();
      fn_cmd = "";
    }
  }
}
#endif  //SERIAL
