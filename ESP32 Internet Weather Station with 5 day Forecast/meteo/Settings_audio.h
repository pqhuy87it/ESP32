//=====================================================================|
//          AUDIO             AUDIO               AUDIO
//=====================================================================|

#define play_on_core0  //только audio_lib2+PCM5102 или VS1053 //audio_lib2+PCM5102 or VS1053

#ifdef ESP32   //AUDIO begin set
//количество избранныех каналов  /data/i_stat.txt favorite channels
#define fav_count 4 //count
//для кириллицы в названиях каналов, i_stat.txt сохранять в UTF-8


//===================VS1053b======VS1053b===VS1053b=====================
//#define vs1053
#ifdef vs1053
//#define SD_CS          5
#define SPI_MOSI      23
#define SPI_MISO      19
#define SPI_SCK       18
#define VS1053_CS     12//xcs
#define VS1053_DCS    13//xdcs
#define VS1053_DREQ   36
//xreset=EN
#endif //vs1053
//----------------------------------------------


//если раскомментировать "audio_lib2", вместо библиотеки ESP8266Audio будет ESP32-audioI2S ( С DAC PCM5102A,MAX98357A,... или без)
//if you uncomment "audio_lib2", instead of the library ESP8266Audio will be ESP32-audioI2S with DAC PCM5102A,MAX98357A,... or without
#define audio_lib2 //lib ESP32-audioI2S
//--------------------------------


//===========ESP32========WWW======MP3===DAC=====================
//#define set_audio_mono //ESP32 play_www,play_np3 mono (def stereo)
const  byte t_ref_song_period = 120; //скорость отрисовки песни меньще-быстрее/song rendering speed WWW less-quicker
const  byte t_ref_song_period_mp3 = 10; //MP3 скорость отрисовки песни меньще-быстрее/ mp3 song rendering speed less-quicker

//-------------PCM5102---or----MAX98357A------------Cjmcu-1334-Uda1334a-
//3.3V from ESP32 -> VCC, 3.3V, XMT
//GND from ESP32  ->
//GND, FLT, DMP, FMT, SCL
// BCLK->BCK, I2SO->DIN, and LRCLK(WS)->LCK

#define PCM5102 //ESP32 external DAC PCM5102,MAX98357A,Cjmcu

#ifdef PCM5102  //pins DAC                                       
const int bclkPin = 27; //    BCLK (max)	BCK (pcm)
const int wclkPin = 26; //	LRC (max)	LRCK (pcm)
const int doutPin = 25; //	DIN (max)	GIN  (pcm)
#endif     //PCM5102                                             

#ifndef audio_lib2
//только для ESP8266Audio //#define audio_lib2 закоментированно
// only for lib ESP8266Audio, not ESP32-audioI2S
const int preallocateBufferSize = 16 * 1024; //16*1024;8*1024;
const int preallocateCodecSize = 85332; // AAC+SBR codec max mem needed
#endif //lib2
#endif //ESP32
//---------------------------------------------------------




//====================================================================
//     FM rsdio  Si4703
//====================================================================
//#define fmSi4703 //SparkFunSi4703rds.h
//OR LIB2 alternative
//#define fmSi4703_lib2 //SI470X.h

#if defined(fmSi4703)|| defined(fmSi4703_lib2)
#define fav_count_fm 4   //избранные каналы двойной клик /data/fm.txt favorite channels Double Click FM
#define c_fm TFT_GREEN //цвет линий на экране FM/line color on FM screen

#ifdef ESP32
int pin_fm_sdio = 21;
int pin_fm_sclk = 22;
int pin_fm_rst = 19;
#endif
#ifdef ESP8266
int pin_fm_sdio = 4;
int pin_fm_sclk = 5;
int pin_fm_rst = 16;
#endif

#endif //fmSi4703
//--------------------------------

//====================================================================
//                 FM rsdio  fmRDA5807
//====================================================================
//#define fmRDA5807

#ifdef fmRDA5807
#define set_audio_mono_fm
#define fav_count_fm 4   //избранные каналы двойной клик /data/fm.txt favorite channels Double Click FM
#define c_fm TFT_GREEN //цвет линий на экране FM/line color on FM screen
#ifdef ESP32
int pin_fm_sdio = 21;
int pin_fm_sclk = 22;
#endif
#ifdef ESP8266
int pin_fm_sdio = 4;
int pin_fm_sclk = 5;
#endif
#define FIX_BAND     RADIO_BAND_FM   ///< The band that will be tuned by this sketch is FM.
#endif //fmRDA5807
//--------------------------------------------------


//====================================================================
//              PIN-OUT-CONTROL on  PLAY WWW,MP3,FM (HIGTH or LOW)
//====================================================================
//----------------------------------HIGTH
// PLAY www,mp3,fm,beep mp3
//+3v  на pin=xxx при проигрывании//+ 3v to pin = xxx when playing

//#define pin_play_WWW_MP3_out_hight 11

//#define pin_play_MP3_out_hight 11
//#define pin_play_WWW_out_hight 11
//#define pin_playFM_out_hight 12

//#define pin_playALL_out_hight 14 (www,mp3,fm)

//----------------------------------LOW
//GND   на pin=xxx при проигрывании//GND to pin = xxx when playing

//#define pin_play_WWW_MP3_out_low 11

//#define pin_play_WWW_out_low 11
//#define pin_play_MP3_out_low 11
//#define pin_playFM_out_low 12

//#define pin_playALL_out_low 14

//если ночные часы // if night hours
//#define pin_night_out_hight 14
//OR v=0
//#define pin_night_out_low 14

//если заход солнца, до восхода // if sunset
//#define pin_sunset_out_hight 14
//OR v=0
//#define pin_sunset_out_low 14

//----------------------------------------------


//====================================================================
//    BEEP          BEEP              BEEP    BEEP
//====================================================================

//BEEP any pin если не будем использовать зуммер, #define pin_beep закомментировать

#define pin_beep 2  //D0 //27

//пассивный ИЛИ  активный зуммер
#ifdef pin_beep //passive or active
#define beep_pass //пассивный sound buzzer passive
//OR
//#define beep_act  //активный  sound buzzer active
#endif
//beep настройка пасс зумм. если #define pin_beep не закоммент., каждый час (с beep_b,по beep_e) 0-23
const unsigned int fr = 900; //частота
const unsigned int dur = 70; //длит сигнала

//TIMER
const unsigned int fr_alarm = 900; //частота TIMER frequency (beep_pass)
const unsigned int dur_alarm = 200; //длит сигнала  duration  TIMER
//---------------------------------------------------


//------------------ERR
#if defined(ESP8266)&& defined( play_on_core0)
#undef play_on_core0
#endif

#if (defined(audio_lib2)&& defined( play_on_core0) && defined (PCM5102)&& defined (ESP32))\
||(defined (vs1053)&& defined (ESP32) && defined( play_on_core0))
#define def_core0
#define stack_size 5000
#define priority 1
#undef play_on_core0
#elif defined( play_on_core0)
#error "ERROR -----> play_on_core0 = #audio_lib2+PCM5102 or VS1053"
#endif

#if defined( fmSi4703) && defined( fmSi4703_lib2)
#error "ERROR -----> or fmSi4703 or fmSi4703_lib2"
#endif
#if defined( PCM5102) && defined( vs1053)
#error "ERROR -----> or PCM5102 or VS1053"
#endif
