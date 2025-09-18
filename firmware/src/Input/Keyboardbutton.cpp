#include <Arduino.h>
#include "Keyboardbutton.h"
#include "Serial.h"
//SemaphoreHandle_t keyboardMutex;
extern const uint8_t ROW_PINS[8] = {46, 1, 42, 41, 2, 13, 39, 40};    // Filas
extern const uint8_t COL_PINS[5] = {45, 38, 48, 21, 47};            // Columnas
bool keyMatrix[8][5];
SpecKeys rows[8][5] = {
      // F7FE
      {SPECKEY_1, SPECKEY_2, SPECKEY_3, SPECKEY_4, SPECKEY_5},
      // FBFE
      {SPECKEY_Q, SPECKEY_W, SPECKEY_E, SPECKEY_R, SPECKEY_T},
      // FDFE
      {SPECKEY_A, SPECKEY_S, SPECKEY_D, SPECKEY_F, SPECKEY_G},
      
      // EFFE
      {SPECKEY_0, SPECKEY_9, SPECKEY_8, SPECKEY_7, SPECKEY_6},
      {SPECKEY_P, SPECKEY_O, SPECKEY_I, SPECKEY_U, SPECKEY_Y},
 
      // FEFE
      {SPECKEY_SHIFT, SPECKEY_Z, SPECKEY_X, SPECKEY_C, SPECKEY_V},
      // BFFE
      {SPECKEY_ENTER, SPECKEY_L, SPECKEY_K, SPECKEY_J, SPECKEY_H},
      // 7FFE
      {SPECKEY_SPACE, SPECKEY_SYMB, SPECKEY_M, SPECKEY_N, SPECKEY_B}};

Keyboardbutton::Keyboardbutton(KeyEventType keyEvent, PressKeyEventType keyPressEvent) : m_keyEvent(keyEvent), m_pressKeyEvent(keyPressEvent)

{
 
//keyboardMutex = xSemaphoreCreateMutex();
xTaskCreatePinnedToCore(keyboardbuttonTask, "keyboardbuttonTask", 4096, this, 1, NULL, 0);

}
void Keyboardbutton::keyboardbuttonTask(void *pParam)
{
  
  
  Serial.println("Teclado ZXSPECTRUM");
Keyboardbutton *keyboard = (Keyboardbutton *)pParam;


while (true){
  //if (xSemaphoreTake(keyboardMutex, portMAX_DELAY)) {
  
// Escanea la matriz
  for (int i = 0; i < 8; i++) {
    // Activa la fila i
    digitalWrite(ROW_PINS[i], LOW);
    //delayMicroseconds(10); // Breve pausa para estabilidad

    // Lee todas las columnas para la fila activa
    for (int j = 0; j < 5; j++) {
      // LOW = tecla pulsada
      keyMatrix[i][j] = (digitalRead(COL_PINS[j]) == LOW);
    }

    // Desactiva la fila
    digitalWrite(ROW_PINS[i], HIGH);
  }

  // Ejemplo: Imprime el estado de las teclas en serie (puedes mapearlo al emulador como gustes)
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 5; j++) {

     if((keyMatrix[i][j])==1){
        bool presion=false;
        SpecKeys tecla=rows[i][j];
       
        keyboard->m_keyEvent(rows[i][j],true);//}
      //Serial.print(keyMatrix[i][j] ? "X" : ".");
      
    }else{
    keyboard->m_keyEvent(rows[i][j],false);
  }
   // Serial.println();
  }
    int leerjoyx=analogRead(6);
  int leerjoyy=analogRead(5);
  //Serial.print(leerjoyx);Serial.print("    ");Serial.println(leerjoyy);//Serial.print("    ");Serial.println(fuego);
 if (leerjoyx>4000){keyboard->m_keyEvent(JOYK_LEFT,true);}
  else {keyboard->m_keyEvent(JOYK_LEFT,false);} 
  if ((leerjoyx<4000)&&(leerjoyx>1500)){keyboard->m_keyEvent(JOYK_RIGHT,true);}
  else {keyboard->m_keyEvent(JOYK_RIGHT,false);}  
 if (leerjoyy>4000){keyboard->m_keyEvent(JOYK_UP,true);keyboard->m_keyEvent(SPECKEY_7,true);}
  else {keyboard->m_keyEvent(JOYK_UP,false);keyboard->m_keyEvent(SPECKEY_7,false);}
  if ((leerjoyy<4000)&&(leerjoyy>1500)){keyboard->m_keyEvent(JOYK_DOWN,true);keyboard->m_keyEvent(SPECKEY_6,true);}
  else {keyboard->m_keyEvent(JOYK_DOWN,false);keyboard->m_keyEvent(SPECKEY_6,false);}  
  if (digitalRead(4)==0 ){keyboard->m_keyEvent(JOYK_FIRE,true); }
  else{keyboard->m_keyEvent(JOYK_FIRE,false);}

}
  
//xSemaphoreGive(keyboardMutex);
//}
vTaskDelay(1/ portTICK_PERIOD_MS); 
}
}
