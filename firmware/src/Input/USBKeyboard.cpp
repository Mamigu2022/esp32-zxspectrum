#include <Arduino.h>
#include "USBKeyboard.h"
//#include "driver/gpio.h"
//#include "usb/usb_host.h"
//#include "USBHIDKeyboard.h"
//#include "hid_host.h"
//#include "hid_usage_keyboard.h"
//#include "hid_usage_mouse.h"
//#include "usb_host.h"
#ifdef TECLADO_USB
#include "Input/EspUsbHost.h"

uint8_t reporte[6];
bool symbol=false;
class MyEspUsbHost : public EspUsbHost {
  
  void onKeyboard(hid_keyboard_report_t report, hid_keyboard_report_t last_report){
   
   
   reporte[0]=report.keycode[0];
   reporte[1]=report.keycode[1];
   reporte[2]=report.keycode[2];
   reporte[3]=report.keycode[3];
   reporte[4]=report.keycode[4];
   //reporte[5]=report.keycode[5];
   
   if(report.modifier){reporte[1]=57;}

  }
};
const std::unordered_map<uint8_t  ,SpecKeys> codigos  = {
{0,SPECKEY_NONE},
  {30,SPECKEY_1},
  {31,SPECKEY_2},
  {32,SPECKEY_3},
  {33,SPECKEY_4},
  {34,SPECKEY_5},
  {35,SPECKEY_6},
  {36,SPECKEY_7},
  {37,SPECKEY_8},
  {38,SPECKEY_9},
  {39,SPECKEY_0},
  {20,SPECKEY_Q},
  {26,SPECKEY_W},
  {8,SPECKEY_E},
  {21,SPECKEY_R},
  {23,SPECKEY_T},
  {28,SPECKEY_Y},
  {24,SPECKEY_U},
  {12,SPECKEY_I},
  {18,SPECKEY_O},
  {19,SPECKEY_P},
  //{31,SPECKEY_SHIFT},
  {57,SPECKEY_SHIFT},
  {4,SPECKEY_A},
  //{'A',SPECKEY_A},
  {22,SPECKEY_S},
  {7,SPECKEY_D},
  {9,SPECKEY_F},
  {10,SPECKEY_G},
  {11,SPECKEY_H},
  {13,SPECKEY_J},
  {14,SPECKEY_K},
  {15,SPECKEY_L},
  //{'\r',SPECKEY_ENTER},
  {40,SPECKEY_ENTER},
  //{char(15),SPECKEY_SHIFT},
  {29,SPECKEY_Z},
  {27,SPECKEY_X},
  {6,SPECKEY_C},
  {25,SPECKEY_V},
  {5,SPECKEY_B},
  {17,SPECKEY_N},
  {16,SPECKEY_M},
  {43,SPECKEY_SYMB},
  {44,SPECKEY_SPACE},
  {82,JOYK_UP},
  {81,JOYK_DOWN},
  {80,JOYK_LEFT},
  {79,JOYK_RIGHT},
  {45,JOYK_FIRE},
  // special pseudo keys
  {42,SPECKEY_DEL},
  {41,SPECKEY_BREAK} 
};

#endif
#ifdef TECLADO_USB
MyEspUsbHost usbHost;
#endif
/* GPIO Pin number for quit from example logic */
//USBKeyboard::USBKeyboard(KeyEventType keyEvent ) : m_keyEvent(keyEvent)
USBKeyboard::USBKeyboard(KeyEventType keyEvent, PressKeyEventType keyPressEvent) : m_keyEvent(keyEvent), m_pressKeyEvent(keyPressEvent)
{
  //Serial.println("hola");
  #ifdef TECLADO_USB

  usbHost.begin();
  usbHost.setHIDLocal(HID_LOCAL_Spanish);
#endif  
  xTaskCreatePinnedToCore(usbkeyboardTask, "usbkeyboardTask", 4096, this, 1, NULL, 0);
}
SpecKeys keyCode[6];
void USBKeyboard::usbkeyboardTask(void *pParam)
{
  USBKeyboard *Keyboard = (USBKeyboard *)pParam;
  
  while (true){
    
    if (codigos.find(reporte[0])!=codigos.end()){
      keyCode[0]= codigos.at(reporte[0]);
      Serial.println(keyCode[0]);
      }
      if (codigos.find(reporte[1])!=codigos.end()){
      keyCode[1]= codigos.at(reporte[1]);}
      if (codigos.find(reporte[2])!=codigos.end()){
      keyCode[2]= codigos.at(reporte[2]);}
      if (codigos.find(reporte[3])!=codigos.end()){
      keyCode[3]= codigos.at(reporte[3]);}
      if (codigos.find(reporte[4])!=codigos.end()){
      keyCode[4]= codigos.at(reporte[4]);}
     // if (codigos.find(reporte[1])!=codigos.end()){
      //keyCode[5]= codigos.at(reporte[5]);}
    

  if (keyCode[0]==JOYK_LEFT){
    //Keyboard->m_keyEvent(SPECKEY_SHIFT,true);
    Keyboard->m_keyEvent(SPECKEY_5,true);
  }
    if (keyCode[0]==JOYK_RIGHT ){
    //Keyboard->m_keyEvent(SPECKEY_SHIFT,reporte[0]);
    Keyboard->m_keyEvent(SPECKEY_8,reporte[0]);
    }
    if (keyCode[0]==JOYK_UP) {
    //Keyboard->m_keyEvent(SPECKEY_SHIFT,reporte[0]);
    Keyboard->m_keyEvent(SPECKEY_7,reporte[0]);
    }
    if (keyCode[0]==JOYK_DOWN) {
   // Keyboard->m_keyEvent(SPECKEY_SHIFT,reporte[0]);
    Keyboard->m_keyEvent(SPECKEY_6,reporte[0]);
    }
    if (keyCode[0]==SPECKEY_DEL) {
    Keyboard->m_keyEvent(SPECKEY_SHIFT,reporte[0]);
    Keyboard->m_keyEvent(SPECKEY_0,reporte[0]);
    }
    if (keyCode[0]==SPECKEY_SHIFT) {
    Keyboard->m_keyEvent(SPECKEY_SHIFT,reporte[0]);
    Keyboard->m_keyEvent(SPECKEY_2,reporte[0]);
    }
    if (keyCode[0]==JOYK_FIRE){
    Keyboard->m_keyEvent(SPECKEY_SPACE,reporte[0]);
    } 
   if(keyCode[0]) Keyboard->m_keyEvent(keyCode[0],true);
   if(keyCode[1]) Keyboard->m_keyEvent(keyCode[1],true);
   if(keyCode[2]) Keyboard->m_keyEvent(keyCode[2],true);
   if(keyCode[3]) Keyboard->m_keyEvent(keyCode[3],true);
   if(keyCode[4]) Keyboard->m_keyEvent(keyCode[4],true);
  // if(keyCode[5]) Keyboard->m_keyEvent(keyCode[5],reporte[5]);
  
  
  vTaskDelay(10/ portTICK_PERIOD_MS);
 if (keyCode[0]==JOYK_LEFT){
    Keyboard->m_keyEvent(SPECKEY_SHIFT,false);
    Keyboard->m_keyEvent(SPECKEY_5,false);
  }
    if (keyCode[0]==JOYK_RIGHT ){
    Keyboard->m_keyEvent(SPECKEY_SHIFT,false);
    Keyboard->m_keyEvent(SPECKEY_8,false);
    }
    if (keyCode[0]==JOYK_UP) {
    Keyboard->m_keyEvent(SPECKEY_SHIFT,false);
    Keyboard->m_keyEvent(SPECKEY_7,false);
    }
    if (keyCode[0]==JOYK_DOWN) {
    Keyboard->m_keyEvent(SPECKEY_SHIFT,false);
    Keyboard->m_keyEvent(SPECKEY_6,false);
    }
    if (keyCode[0]==SPECKEY_DEL) {
    Keyboard->m_keyEvent(SPECKEY_SHIFT,false);
    Keyboard->m_keyEvent(SPECKEY_0,false);
    }
    if (keyCode[0]==SPECKEY_SHIFT) {
    Keyboard->m_keyEvent(SPECKEY_SHIFT,false);
    Keyboard->m_keyEvent(SPECKEY_2,false);
    }
    if (keyCode[0]==JOYK_FIRE){
   Keyboard->m_keyEvent(SPECKEY_SPACE,false);
   } 
   if(keyCode[0]) Keyboard->m_keyEvent(keyCode[0],false);
   if(keyCode[1]) Keyboard->m_keyEvent(keyCode[1],false);
   if(keyCode[2]) Keyboard->m_keyEvent(keyCode[2],false);
   if(keyCode[3]) Keyboard->m_keyEvent(keyCode[3],false);
   if(keyCode[4]) Keyboard->m_keyEvent(keyCode[4],false);
    //Keyboard->m_keyEvent(keyCode[5],false);
    
 
  //vTaskDelay(70);
  usbHost.task();
   }
 }



