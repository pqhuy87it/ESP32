#if defined( def_core0)

void audioInit() {
  xTaskCreatePinnedToCore(
    audioTask,                /* Функция для задачи  MAIN LOOP */
    "audioplay",              /* Имя задачи */
    stack_size,               /* Размер стека */
    NULL,                     /* Параметр задачи */
    priority,                 /* Приоритет */
    &audioplay,               /* Выполняемая операция */
    0                         /* Номер ядра, на котором она должна выполняться */
  );
}

void audioTask( void * pvParameters ) {
  esp_task_wdt_init(10, false);
  disableCore0WDT();
#ifdef debug
  Serial.print("----------------audioTask running on core ");
  Serial.println(xPortGetCoreID());
#endif
  CreateQueues();
  if (!audioSetQueue || !audioGetQueue) {
    Serial.println("Error: queues are not initialized");
    return;
  }
  struct audioMessage audioRxTaskMessage;
  struct audioMessage audioTxTaskMessage;

#ifdef audio_lib2
  audio.setPinout(bclkPin, wclkPin, doutPin);
#endif

#ifdef vs1053
  audio.begin(); // Initialize VS1053 player
  uint32_t chipID = audio.printChipID();
  if (chipID == 0x00000000 || chipID == 0xFFFFFFFF)
  {
    Serial.println("Error: VS1053 not found");

  }
  else
  {
    status_vs1053 = true;
    audio.setVolume(0);
  }
  print_info("VS1053");
#endif


#ifdef set_audio_mono
  audio.forceMono(true);
#endif
  audio.setVolume(15); // 0...21

  while (true) {
    micros(); //update overflow
    if (xQueueReceive(audioSetQueue, &audioRxTaskMessage, 1) == pdPASS) {
      if (audioRxTaskMessage.cmd == SET_VOLUME) {
        audioTxTaskMessage.cmd = SET_VOLUME;
        audio.setVolume(audioRxTaskMessage.value);
        audioTxTaskMessage.ret = 1;
        xQueueSend(audioGetQueue, &audioTxTaskMessage, portMAX_DELAY);
      }
      //----------------HOST
      else if (audioRxTaskMessage.cmd == CONNECTTOHOST) {
        audioTxTaskMessage.cmd = CONNECTTOHOST;
        audioTxTaskMessage.ret = audio.connecttohost(audioRxTaskMessage.txt);
        xQueueSend(audioGetQueue, &audioTxTaskMessage, portMAX_DELAY);
      }
      //----------------SD
      else if (audioRxTaskMessage.cmd == CONNECTTOFS) {
        audioTxTaskMessage.cmd = CONNECTTOFS;
#ifdef pin_sd
        audioTxTaskMessage.ret = audio.connecttoFS(SD, audioRxTaskMessage.txt);
#else //spiffs
        audioTxTaskMessage.ret = audio.connecttoFS(SPIFFS, audioRxTaskMessage.txt);
        //     audioTxTaskMessage.ret = audio.connecttoFS(SD, audioRxTaskMessage.txt, audioRxTaskMessage.value);
#endif
        xQueueSend(audioGetQueue, &audioTxTaskMessage, portMAX_DELAY);
      }
      else if (audioRxTaskMessage.cmd == GET_VOLUME) {
        audioTxTaskMessage.cmd = GET_VOLUME;
        audioTxTaskMessage.ret = audio.getVolume();
        xQueueSend(audioGetQueue, &audioTxTaskMessage, portMAX_DELAY);
      }
      //----------------STOP
      else if (audioRxTaskMessage.cmd == STOPSONG) {
        audioTxTaskMessage.cmd = STOPSONG;
#ifdef audio_lib2
        audioTxTaskMessage.ret = audio.stopSong();
#else
        audioTxTaskMessage.ret =  audio.stop_mp3client(); //vs
#endif
        xQueueSend(audioGetQueue, &audioTxTaskMessage, portMAX_DELAY);
      }
      //----------------BUFF
      else if (audioRxTaskMessage.cmd == INBUFF_FILLED) {
        audioTxTaskMessage.cmd = INBUFF_FILLED;
#ifdef audio_lib2
        audioTxTaskMessage.ret = audio.inBufferFilled();
#else
        audioTxTaskMessage.ret = audio.bufferFilled(); //vs
#endif
        xQueueSend(audioGetQueue, &audioTxTaskMessage, portMAX_DELAY);
      }
      else if (audioRxTaskMessage.cmd == INBUFF_FREE) {
        audioTxTaskMessage.cmd = INBUFF_FREE;
#ifdef audio_lib2
        audioTxTaskMessage.ret = audio.inBufferFree();
#else
        audioTxTaskMessage.ret = audio.bufferFree(); //vs
#endif
        xQueueSend(audioGetQueue, &audioTxTaskMessage, portMAX_DELAY);
      }

    }
    if (playing)  audio.loop();
    else
      vTaskDelay(10);
  }
}
//-------------------------------------------------------------------
boolean audioConnecttohost(const char* host) {
//  Serial.print("audioConnecttohost "); Serial.println(host);
  audioTxMessage.cmd = CONNECTTOHOST;
  audioTxMessage.txt = host;
  audioMessage RX = transmitReceive(audioTxMessage);
  return RX.ret;
}

uint32_t audioStopSong() {
 // Serial.println("audioStopSong ");// Serial.println(filename);
  audioTxMessage.cmd = STOPSONG;
  audioMessage RX = transmitReceive(audioTxMessage);
  return RX.ret;
}

boolean audioConnecttoFS(const char* filename, uint32_t resumeFilePos) {
//  Serial.print("audioConnecttoFS "); Serial.println(filename);

  audioTxMessage.cmd = CONNECTTOFS;
  audioTxMessage.txt = filename;
  audioTxMessage.value = resumeFilePos;
  audioMessage RX = transmitReceive(audioTxMessage);
  return RX.ret;
}
uint32_t audioInbuffFilled() {
  audioTxMessage.cmd = INBUFF_FILLED;
  audioMessage RX = transmitReceive(audioTxMessage);
  return RX.ret;
}

uint32_t audioInbuffFree() {
  audioTxMessage.cmd = INBUFF_FREE;
  audioMessage RX = transmitReceive(audioTxMessage);
  return RX.ret;
}


void audioSetVolume(uint8_t vol) {
  audioTxMessage.cmd = SET_VOLUME;
  audioTxMessage.value = vol;
  audioMessage RX = transmitReceive(audioTxMessage);
  (void)RX;
}



audioMessage transmitReceive(audioMessage msg) {
  xQueueSend(audioSetQueue, &msg, portMAX_DELAY);
  if (xQueueReceive(audioGetQueue, &audioRxMessage, portMAX_DELAY) == pdPASS) {
    if (msg.cmd != audioRxMessage.cmd) {
      Serial.println(F("Error: wrong reply from message queue"));
    }
  }
  return audioRxMessage;
}


#endif
