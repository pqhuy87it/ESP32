#ifdef ESP32

#include "AudioFileSourceICYStream.h"
#include "AudioFileSourceBuffer.h" 
#include "AudioGeneratorMP3.h" 
#include "AudioFileSourceID3.h"

	#ifdef pin_sd
#include "SD.h"
#include "AudioFileSourceSD.h"
	#else
#include "SPIFFS.h"
#include "AudioFileSourceSPIFFS.h"
	#endif


#include "AudioOutputI2S.h" 


AudioGeneratorMP3 *mp3 = NULL;
AudioFileSourceICYStream *file = NULL;
AudioFileSourceBuffer *buff = NULL;


	#ifdef pin_sd
AudioFileSourceSD *file_mp3= NULL;	


String play_song_mp3="";
String play_song_g_mp3="";
String play_album_mp3="";
String play_year_mp3="";



	#else
AudioFileSourceSPIFFS *file_mp3= NULL;
	#endif
AudioFileSourceID3 *id3= NULL;


AudioOutputI2S *out = NULL;

//const int preallocateBufferSize = 16*1024;
//const int preallocateCodecSize = 85332; // AAC+SBR codec max mem needed

void *preallocateBuffer = NULL;
void *preallocateCodec = NULL;




//-----------------------------------

// Called when a metadata event occurs (i.e. an ID3 tag, an ICY block, etc.
void MDCallback(void *cbData, const char *type, bool isUnicode, const char *string)
{
  const char *ptr = reinterpret_cast<const char *>(cbData);
  (void) isUnicode; // Punt this ball for now
  // Note that the type and string may be in PROGMEM, so copy them to RAM for printf
char s1[32], s2[64];
String play_tit;
  strncpy_P(s1, type, sizeof(s1));
  s1[sizeof(s1)-1]=0;
  strncpy_P(s2, string, sizeof(s2));
  s2[sizeof(s2)-1]=0;
//#ifdef debug
//Serial.printf("METADATA(%s) '%s' = '%s'\n", ptr, s1, s2);
//#endif
play_tit=String(s1);
if (play_tit.indexOf("StreamTitle")>=0)
{
play_song= (const char*) s2;
play_song.trim();
if (play_song=="-") play_song="";
new_song=true;

}
// Serial.flush();
}

#ifdef pin_sd //mp3


//ID3 callback for: Title = 'Dreams (Deep Crowl Mix)'
//ID3 callback for: Album = 'Early Years [Compilation]'
//ID3 callback for: Year = '1999'
//ID3 callback for: Performer = 'Crematory'
void MDCallback_mp3(void *cbData, const char *type, bool isUnicode, const char *string)
{
  (void)cbData;
//#ifdef debug
//  Serial.printf("ID3 callback for: %s = '", type);
//#endif
String s_tmp="";
 char a;
 //char my[];
 
  if (isUnicode) {
    string += 2;
  }


  while (*string) {
//    char a = *(string++);
    a = *(string++);
    if (isUnicode) {
      string++;
    }
   

s_tmp=  s_tmp + String(a);
//#ifdef debug
// Serial.printf("%c", a);
//	Serial.print("s_tmp++");Serial.println(s_tmp);
//#endif
  }


//#ifdef debug
 //	Serial.print("s_tmp = ");Serial.println(s_tmp);
//#endif

 
if (type=="Title") 	
{

play_song_mp3=s_tmp;
//#ifdef debug
//Serial.print("play_song_mp3 = ");Serial.println(play_song_mp3);
//#endif
}
if (type=="Performer") 
{	
play_song_g_mp3= s_tmp;
//#ifdef debug
//Serial.print("play_song_g_mp3 = ");Serial.println(play_song_g_mp3);
//#endif

}



if (type=="Album") 
{	
play_album_mp3= s_tmp;
//#ifdef debug
//Serial.print("play_album_mp3 = ");Serial.println(play_album_mp3);
//#endif
}


if (type=="Year") 
{	
play_year_mp3= s_tmp;
//#ifdef debug
//Serial.print("play_year_mp3 = ");Serial.println(play_year_mp3);
//#endif
}



new_song_mp3=true;
//#ifdef debug
 // Serial.printf("'\n");
 // Serial.flush();
//#endif  
}

#endif




/*
// Called when there's a warning or error (like a buffer underflow or decode hiccup)
void StatusCallback(void *cbData, int code, const char *string)
{
  const char *ptr = reinterpret_cast<const char *>(cbData);
  // Note that the string may be in PROGMEM, so copy it to RAM for printf
  char s1[64];
  strncpy_P(s1, string, sizeof(s1));
  s1[sizeof(s1)-1]=0;
//  Serial.printf("STATUS(%s) '%d' = '%s'\n", ptr, code, s1);
//  Serial.flush();
}

*/

#endif // ESP32