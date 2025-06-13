#ifdef mqtt
//------------------------------------------------------------------------
//                          do_send_mqtt()
//------------------------------------------------------------------------
void do_send_mqtt()
{

  static uint32_t t_next = 0;
  bool ret = false;

  if (millis() < t_next) return;


#ifdef debug
  Serial.println(F("---mqtt--send--"));
#endif

  //ttttttttttttttt
  if (mqtt_T_topic_send != "" && temp_kv != 200)
  {
#ifdef debug
    Serial.print(F("temp_kv=")); Serial.println(temp_kv);
#endif

    ret = client_mqtt.publish(mqtt_T_topic_send, String(temp_kv).c_str(), true);
    if (!ret) Serial.println("ERR send T_kv");
  }

  //hhhhhhhhhhhhhhhhhhh
  if (mqtt_H_topic_send != "" && h_kv != 200)
  {
#ifdef debug
    Serial.print(F("h_kv="));
    Serial.println(h_kv);
#endif
    //  delay(100);
    ret = client_mqtt.publish(mqtt_H_topic_send, String(h_kv).c_str(), true);
    if (!ret) Serial.println("ERR send Hum");
  }


  //Tu-----------
  if (mqtt_T2_topic_send != "" && temp_u != 200)
  {
#ifdef debug
    Serial.print(F("temp_u=")); Serial.println(temp_u);
#endif

    ret = client_mqtt.publish(mqtt_T2_topic_send, String(temp_u).c_str(), true);
    if (!ret) Serial.println("ERR send temp_u");

  }

  //ppppppppppppppppppppppppppppp  давление(pressure)
  int P = 200;
#ifdef openw
  P = davlenie0;
#endif

#ifdef bme280
  P = pr_kv;
#endif

  if (mqtt_P_topic_send != "" && (P != 200))
  {

#ifdef debug
    Serial.print(F("P=")); Serial.println(P);
#endif
    ret = client_mqtt.publish(mqtt_P_topic_send, String(P).c_str(), true);
    if (!ret) Serial.println("ERR send P");
  }


  t_next = millis() + mqtt_period_send * 1000L;

#ifdef debug
  Serial.println(F("----mqtt--end---"));
#endif

}



//---------------------------------------------------------------------------------
//                                        reconn_mqtt
//---------------------------------------------------------------------------------
void reconn_mqtt() {
  static uint32_t  t_next = 0;
  if (millis() < t_next) return;
  Serial.print("Attempting MQTT connection...");
  if (client_mqtt.connect(mqtt_esp_name.c_str(), mqtt_user, mqtt_password)) {
    Serial.println(F("mqtt connected"));
    client_mqtt.subscribe("home/#");
  } else {
    Serial.print("ERR mqtt connected, rc=");
    Serial.println(client_mqtt.state());
    //  Serial.println(" try again");
    // Wait 5 seconds before retrying

  }
  t_next = millis() +  t_reconn_mqtt * 1000L;

}


//---------------------------------------------------------------------------------
//                                        callback_mqtt
//---------------------------------------------------------------------------------

void callback_mqtt(char* topic, byte* payload, unsigned int length) {
/*
#ifdef debug
  Serial.print("Message read [");
  Serial.print(topic);
  Serial.print("] ");
#endif
  for (int i = 0; i < length; i++) {
#ifdef debug
    Serial.print((char)payload[i]);
#endif
    //----------------payload
  }
#ifdef debug
  Serial.println();
#endif
*/
}



#endif //mqtt
