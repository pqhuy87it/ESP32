#ifdef IoT
//------------------------------------------------------------------------
//                          do_send_iot()
//------------------------------------------------------------------------
void do_send_iot()
{

static uint32_t t_next=0;
byte f=0;

if (iot_period_send<18)
{
//Serial.print(F("---------ERR iot_period_send <15"));
return; 
}

if (millis()<t_next) return;


#ifdef debug
Serial.println(F("---IoT--send--"));
#endif

//ttttttttttttttt
if (iot_T_fld_send!=0&&temp_kv!=200)
{
#ifdef debug  
Serial.print(F("temp_kv="));
Serial.println(temp_kv);
#endif
ThingSpeak.setField(iot_T_fld_send, temp_kv);
f++;
}
//hhhhhhhhhhhhhhhhhhh
if (iot_H_fld_send!=0&&h_kv!=200)
{
#ifdef debug
Serial.print(F("h_kv="));  
Serial.println(h_kv);
#endif
ThingSpeak.setField(iot_H_fld_send, h_kv);
f++;
}
//Tu-----------
if (iot_T2_fld_send!=0&&temp_u!=200)
{
#ifdef debug
Serial.print(F("temp_u="));    
Serial.println(temp_u);
#endif
ThingSpeak.setField(iot_T2_fld_send, temp_u);
f++;
}


int P=200;
//ppppppppppppppppppppppppppppp  давление(pressure)
#ifdef openw
P=davlenie0;
#endif

#ifdef bme280
P=pr_kv;
#endif

if (iot_P_fld_send!=0&&(P!=200))
{
#ifdef debug
Serial.print(F("P="));    
Serial.println(P);
#endif
ThingSpeak.setField(iot_P_fld_send,P);
f++;
}


#ifdef openw
//wwwwwwwwwwwwwwwwwwwwwwwwww скорость ветра (wind speed)
if (iot_W_fld_send!=0&&v_ch[0]!=200)
{
#ifdef debug
Serial.print(F("Wind="));    
Serial.println(v_ch[0]);
#endif
ThingSpeak.setField(iot_W_fld_send,v_ch[0]);
f++;
}

//rrrrrrrrrrrrrrrrrrr  byte =6; //дождь мм(rain)

if (iot_R_fld_send!=0&&rain_ch[0]!=200) //есть ли данные, пров. по davlenie0.
{
#ifdef debug
Serial.print(F("Rain="));    
Serial.println(rain_ch[0]);  
#endif
ThingSpeak.setField(iot_R_fld_send,rain_ch[0]);
f++;
}
#endif //openw


#ifdef debug
Serial.print(F("id=")); 
Serial.print(iot_id); 
Serial.print(F("   iot_key=")); 
Serial.println(iot_key);  
#endif

if (f==0) 
{
Serial.println(F("IoT No fields, exit"));
t_next=millis()+60*1000L;
return;    
}
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(iot_id, iot_key);
  if(x == 200){

#ifdef debug
Serial.println(F("IoT update successful.-------------------------"));
#endif
 t_next=millis()+iot_period_send*1000L;


/*

log_str=("IoT f=")+String(f);
do_log();  

*/

  
  }
  else{
Serial.print(F("---- IoT send ERR code=" ));
Serial.println(String(x));
t_next=millis()+60*1000L;
  }




}



//------------------------------------------------------------------------
//                          do_read_iot()
//------------------------------------------------------------------------

void do_read_iot()
{

static uint32_t t_next=0;

if (millis()<t_next) return;
int int_tmp=-99;
int statusCode = 0;
byte err=0;
String str="";
#ifdef debug
Serial.println(F("---IoT--read--"));
#endif

//tttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttt
if (iot_T_fld_read!=0)
{
int_tmp = ThingSpeak.readLongField(iot_id_read, iot_T_fld_read, iot_key_read);  
statusCode = ThingSpeak.getLastReadStatus();
  if(statusCode == 200)
  {
if (int_tmp!=temp_kv) ref_kv_th=true;
temp_kv= int_tmp;
Serial.print("iot_T_fld_read="); Serial.println(temp_kv);
  str="iot_T_fld_read ok;";
t_read_tk=millis();

  
  }
  else
{
  str="iot_T_fld_read ERR;";
  err++;
Serial.println("ERR iot_T_fld_read code " + String(statusCode)); 

}
}
 

//hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh
if (iot_H_fld_read!=0)
{
int_tmp = ThingSpeak.readLongField(iot_id_read, iot_H_fld_read, iot_key_read);  
statusCode = ThingSpeak.getLastReadStatus();
  if(statusCode == 200)
  {
if (h_kv!=int_tmp) ref_kv_th=true;    
h_kv= int_tmp;
t_read_tk=millis();
Serial.print(F("iot_H_fld_read="));  
Serial.println(h_kv);
  str=str+"iot_H_fld_read ok;" ;
  }
  else
{
  str=str+"iot_H_fld_read ERR;";
  err++;  
Serial.println("ERR iot_H_fld_read code " + String(statusCode)); 

}
}
 


//Tu----TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT
if (iot_T2_fld_read!=0)
{
int_tmp = ThingSpeak.readLongField(iot_id_read, iot_T2_fld_read, iot_key_read);  
statusCode = ThingSpeak.getLastReadStatus();
  if(statusCode == 200)
  {
if (int_tmp!=temp_u) ref_temp_u=true;
temp_u= int_tmp;

Serial.print(F("iot_T2_fld_read="));    
Serial.println(temp_u);
str=str+"iot_T2_fld_read ok;";
period_ulica=millis();

  }
  else
{
  str=str+"iot_T2_fld_read ERR;";
  err++;    
Serial.println("ERR iot_T2_fld_read code " + String(statusCode)); 

}
}
 


//ppppppppppppppppppppppppppppp  давление(pressure)
if (iot_P_fld_read!=0)
{
int_tmp = ThingSpeak.readLongField(iot_id_read, iot_P_fld_read, iot_key_read);  
statusCode = ThingSpeak.getLastReadStatus();
  if(statusCode == 200)
  {
pr_kv= int_tmp;
//ref_temp_u==true;
Serial.print(F("iot_P_fld_read="));    
Serial.println(pr_kv);
  str=str+"iot_P_fld_read ok;";
  }
  else
  {

  str=str+"iot_P_fld_read ERR;";
  err++;    
Serial.println("ERR iot_P_fld_read code " + String(statusCode)); 

  }
}




/*

log_str=("IoT read ")+str;
log_str=log_str+" ERR_count="+String(err);
do_log();  

*/
if (err==0)
 t_next=millis()+iot_period_read*1000L;
else
t_next=millis()+60*1000L;



}

#endif
