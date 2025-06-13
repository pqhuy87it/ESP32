#ifndef openw 
#ifdef tft_320_240)
//time///////////////////////////////////////tttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttt
void S0_time()
{
static byte last_min=99;//чтобы не перирис минуты

//time_t t =now(); 
//время




if (t%2==0)
{

//ntp
if (err_ntp==0) 
tft.fillRoundRect(158,155,4,20,1,TFT_GREENYELLOW);
else
tft.fillRoundRect(158,155,4,20,1,TFT_RED);
} //sec black------------------------------------
else
{

 //sec
tft.fillRoundRect(158,155,4,20,1,TFT_BLACK);
}


//если мин не изм  и не надо все обновить выходим
//-----------------

if (last_min==now_min&&refresh_all==false) {return;}
last_min=now_min;


if (ref_temp_u==true||ref_kv_th==true||refresh_all==true) //есть данные с улицы и кв
do_show_th_no_ow();

//-----------------------------------------------------------------



//////////////hour///hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh
String st;
String fileName;


if (now_hour<10) 
{
fileName="/n0_176.jpg";
 drawJpeg(fileName,0,0);//220
fileName="/n"+String(now_hour)+"_176.jpg";
 drawJpeg(fileName,80,0);//220
  }
else
{
st=String(now_hour);
fileName="/n"+st.substring(0,1)+"_176.jpg";
drawJpeg(fileName,0,0);//220
fileName="/n"+st.substring(1)+"_176.jpg";
drawJpeg(fileName,80,0);//220

}






//////////////////min

if (now_min<10) 
{
fileName="/n0_176.jpg";
 drawJpeg(fileName,160,0);//220
fileName="/n"+String(now_min)+"_176.jpg";
drawJpeg(fileName,240,0);//220
 }
else
{
st=String(now_min);
fileName="/n"+st.substring(0,1)+"_176.jpg";
drawJpeg(fileName,160,0);//220
fileName="/n"+st.substring(1)+"_176.jpg";
drawJpeg(fileName,240,0);//220

}



//--------------------------------------------------------------------------------

//дата--------data/////////ddddddddddddddddddddddddddddddddddddddddddddddd
if (refresh_all==false&&year(t)>2015) //день недели может совпасть(01-01-), добавил год
  return;


////////////////восход зах.///////////////////////////////////
tft.setFreeFont(v9);
tft.fillTriangle(10,195,20,195,15,190,TFT_YELLOW);
tft.setTextColor(TFT_YELLOW);
tft.setCursor(25,195);
tft.print(t_sun_v);
//zahod243
//tft.fillTriangle(90,190,100,190,95,195,TFT_RED);
tft.fillTriangle(243,190,253,190,248,195,TFT_RED);
tft.setTextColor(TFT_ORANGE);
tft.setCursor(263,195);
tft.print(t_sun_z);
tft.setFreeFont(v6);



tft.drawFastHLine(0,200,320,TFT_LIGHTGREY) ; //под часами


}


//============================================================
//                      do_show_th_no_ow NO openw 
//====================================================================

void do_show_th_no_ow()
{
Serial.println("do_show_th_play noopenw");  
if (refresh_all)  tft.fillRect(0,201,320,38, TFT_BLACK);
int  x=168;

//tttttttttttttttttttttttttttttttttttttttttttttt
if (temp_u!=200)
{
tft.setFreeFont(v18);
tft.setTextColor(color_t(temp_u));
if (!refresh_all) tft.fillRect(x,202,70,37,TFT_BLACK);
tft.setCursor(x,235);
tft.print(temp_u);
//if (ref_temp_u)  ref_temp_u=false;
}

//hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh
S_show_th_room(233,199);


//ppppppppppppppppppppppppppppp
x=285;
if (pr_kv!=200)
{
tft.setTextColor(c_pr_bme);
tft.setFreeFont(v9);
tft.setCursor(x,233);
tft.print(pr_kv);

}
 

   

//#endif

if (!refresh_all) return;  



tft.drawFastVLine(130,200,39,TFT_LIGHTGREY) ;
tft.drawFastVLine(164,200,39,TFT_LIGHTGREY) ;


//ddddddddddddddddddddddddddddddddddddd
//time_t t =now();

String st="";
if (day(t)<10) 
st="0"+String(day(t));
else
st=String(day(t));

tft.setFreeFont(v12);
tft.setTextColor(TFT_WHITE);
tft.setCursor(0,230);
tft.print(st);

tft.setTextColor(TFT_LIGHTGREY);
if (month(t)<10) 
st="0"+String(month(t));
else
st=String(month(t));
st="."+st;
tft.setFreeFont(v9);
tft.print(st);

//week wwwwwwwwwwwwwwwwwwwwwwwwwww
draw_week_day(63,207);
 
//---------------moon

String fileName;
fileName=do_phasemoon()+"_32.jpg";
drawJpeg(fileName,131,205);



}
#endif //320-240
#endif //no openw
 
