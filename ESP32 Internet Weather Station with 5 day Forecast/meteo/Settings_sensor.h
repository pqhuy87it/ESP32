
//====================================================================
//                        R433 WL102-341,...
//====================================================================
//#define R433
#ifdef R433
#define pin_r433_rx  22       //gpio read
//что принимаем
#define r433_read_Tu  //temp_u Темп улица //street temp
#define r433_read_Tk  //temp_kv Темп квартира //room temp
#define r433_read_Hh   //h_kv  влаж //humidity
#define r433_read_Pr  //давление //pressure
//замещать влажность в квартире принятой влажностью с улицы по NRF24, если не надо закомм-ть
//если  у датчика значение 0,  "#define bme280 0" в set.h из проекта meteo_send.ino
//например  bme280 стоит на улице и мы хотим видеть влажность на экране
//replace the humidity in the apartment with the accepted humidity from the street according to NRF24, if not necessary, then for commen "//"
//if the sensor is 0, for example "#define bme280 0" in meteo_send.ino (in set.h)
#define nrf_hum_out_TO_hum_in

#endif //433

//====================================================================
//NRF24     NRF24       NRF24         NRF24
//====================================================================
//если нет приёма по nrf с закомментировать #define nrf
//ESP32 spi=HSPI nrf_SCLK=14,nrf_MISO=12,nrf_MOSI=13
//#define nrf
#ifdef nrf
#define  pin_nrf_CE  33//13; //Chip Enable
#define  pin_nrf_CS  32//12; //Chip Select
#define  nrf_channel 5  //канал в котором нет шумов = channel_nrf meteo
#define  nrf_palevel 3  //1-4 (1- MIN or 2-LOW 3-HIGH 4- MAX) //уровень мощности передатчика
#define  nrf_speed 1000 // скорость обмена 2000,1000,250 (nrf_2MBPS, nrf_1MBPS, nrf_250KBPS)
//что принимаем
#define nrf_read_Tu  //temp_u Темп улица //street temp
#define nrf_read_Tk  //temp_kv Темп квартира //room temp
#define nrf_read_Hh   //h_kv  влаж //humidity
#define nrf_read_Pr  //давление //pressure
//замещать влажность в квартире принятой влажностью с улицы по NRF24, если не надо закомм-ть
//если  у датчика значение 0,  "#define bme280 0" в set.h из проекта meteo_send.ino
//например  bme280 стоит на улице и мы хотим видеть влажность на экране
//replace the humidity in the apartment with the accepted humidity from the street according to NRF24, if not necessary, then for commen "//"
//if the sensor is 0, for example "#define bme280 0" in meteo_send.ino (in set.h)
#define nrf_hum_out_TO_hum_in
#endif //NRF
//#if defined (nrf;433)  //ALERT
//color alert id /data/color.h
#define c_alr_id_def TFT_GREENYELLOW //цвет alert по умолчанию, если не определён для id 
#define c_alr_id0 TFT_YELLOW  //id=0
#define c_alr_id1 TFT_CYAN
#define c_alr_id2 TFT_PINK
#define c_alr_id3 TFT_PURPLE
#define c_alr_id4 TFT_PINK
#define c_alr_id5 TFT_RED
#define c_alr_id6 TFT_MAGENTA
//#define c_alr_id7
//#define c_alr_id8
//#define c_alr_id9
//#endif
//---------------------------------------------------



//====================================================================
//      DS18B20       DS18B20           DS18B20
//====================================================================

//DS18B20 температура с датчика DS18B20 улица
//если нет надо можно закомм-ть #define pin_ds1820
//outdoor temperature
//#define pin_ds1820  2  //D4
const unsigned int t_ds_ref = 300 ; //sec T ref sensor, период запроса датчика
#define ds1820_place 0  //0 -street 1 - room
//---------------------------------------------------


//====================================================================
//BME280 or HTU21D or SI7021 or DHT  or AHT10
//====================================================================
//Si7021  если нет датчика закомментировать #define si7021

//#define si7021   //room HTU21D or SI7021
//ИЛИ OR
//bme280 если нет датчика bme(bmp) закомментировать #define bme280
//#define bme280    //room
//ИЛИ OR
//#define aht10 0x38 //address 0x38 or 0x39
//ИЛИ OR
//#define pin_dht D1   //room   выьрать ниже DHTTYPE (DHT 11 or DHT 22 or DHT 21)
#ifdef pin_dht
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
#endif

// Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.




const unsigned int t_bme_ref = 300 ; //sec T ref sensor, период обновления датчика SI,BME,DHT 5min def
const unsigned int t_bme_err = 10 ; //sec Retry on sensor error, повтор при ошибке датчика сек SI,BME,DHT 5min def


#define ref_sensor_on_play     //в режиме радио будут обновляться локальные (и #def wf_read) данные от bme bmp si dht ds, могут быть заикания(
//In radio mode local data from bme bmp si dht ds will be updated (and #def wf_read), there may be stutters (
//если закомментировать #define, то обновяться только при смене канала
//If you comment #define, update only when changing the channel
//------------------------------------------------



//==========TFT Led================TFT Led=============TFT Led================
//#define pin_led 33  //управление подсветкой, настройки в config.txt//Backlight control, config.txt settings

//            in config.txt if #define pin_led
//#t_pir_period; // яркость=0, brightness=0
//#led_def;100; // яркость TFT 0-100,TFT brightness 0-100
//#led_night;20; // яркость ночью ,brightness at night
//#led_sunset;50; // яркость после захода ,after sunset


//==========PIR================PIR=============PIR=====
//#define pin_pir 39//36  //включение  подсветки при движении  (PIR)  на время= #t_pir_period из config.txt
//turning on the backlight when driving (PIR) time=#t_pir_period of config.txt
#ifdef pin_pir
#define pin_pir_out_hight 27   //после движения держит высокий уровень на пине время=t_pir_period из config.txt
#define pin_pir_out_low 27
#endif                              //after the movement keeps the high level on the pin time=t_per_period of config.txt

//------------------------------------------------------------------------






//------------------------------------control ERR------------------------------------------
#ifdef ds1820_place=1
#define def_th //есть датчики темп или влаж.
#elif defined (nrf_read_Tk)||defined (nrf_read_Hh)\
|| defined (nrf_read_Tk) || defined (nrf_read_Hh)\
|| defined (si7021) || defined (bme280) \
|| defined (aht10) || defined (pin_dht) \
|| defined (wf_read_Tk) || defined (wf_read_Hh)
#define def_th //есть датчики темп или влаж.
#endif

#if defined(nrf) &&defined(R433)
#error "ERROR -----> only NRF or R433 !!!"
#endif
