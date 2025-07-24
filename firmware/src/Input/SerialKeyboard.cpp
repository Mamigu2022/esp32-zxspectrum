#include <Arduino.h>
#include "Serial.h"
#include "SerialKeyboard.h"

SerialKeyboard::SerialKeyboard(KeyEventType keyEvent) : m_keyEvent(keyEvent)
{
  Serial.println("hola");
  xTaskCreatePinnedToCore(keyboardTask, "keyboardTask", 4096, this, 1, NULL, 0);
}

void SerialKeyboard::keyboardTask(void *pParam)
{
  SerialKeyboard *keyboard = (SerialKeyboard *)pParam;
  
  while (true)
  {
    if (Serial.available() > 0)
    {
      String message = Serial.readStringUntil('\n');
      
      message.trim();
      String key;
      bool down = false;
      if (message.startsWith("down:"))
      {
        key = message.substring(5);
        Serial.println(key);
        down = true;
      }
      else if (message.startsWith("up:"))
      {
        key = message.substring(3);
      }
      SpecKeys keyValue = SpecKeys(key.toInt());
      Serial.println(keyValue);
      keyboard->m_keyEvent(keyValue, down);
    }
    vTaskDelay(10);
  }
}