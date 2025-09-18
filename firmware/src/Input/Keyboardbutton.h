#pragma once
#include <Arduino.h>
#include "Serial.h"
#include <set>
#include <vector>
#include <string>
#include "../Emulator/keyboard_defs.h"

class Keyboardbutton {
  private:
    
    using KeyEventType = std::function<void(SpecKeys keyCode, bool isPressed)>;
    KeyEventType m_keyEvent;
   //void onKeyboardKey(uint8_t ascii, uint8_t keycode, uint8_t modifier)
    using PressKeyEventType = std::function<void(SpecKeys keyCode)>;
    PressKeyEventType m_pressKeyEvent;
    

  public:
    
    std::unordered_map<SpecKeys, int> isKeyPressed = {
      {SPECKEY_1, 0},
      {SPECKEY_2, 0},
      {SPECKEY_3, 0},
      {SPECKEY_4, 0},
      {SPECKEY_5, 0},
      {SPECKEY_6, 0},
      {SPECKEY_7, 0},
      {SPECKEY_8, 0},
      {SPECKEY_9, 0},
      {SPECKEY_0, 0},
      {SPECKEY_Q, 0},
      {SPECKEY_W, 0},
      {SPECKEY_E, 0},
      {SPECKEY_R, 0},
      {SPECKEY_T, 0},
      {SPECKEY_Y, 0},
      {SPECKEY_U, 0},
      {SPECKEY_I, 0},
      {SPECKEY_O, 0},
      {SPECKEY_P, 0},
      {SPECKEY_A, 0},
      {SPECKEY_S, 0},
      {SPECKEY_D, 0},
      {SPECKEY_F, 0},
      {SPECKEY_G, 0},
      {SPECKEY_H, 0},
      {SPECKEY_J, 0},
      {SPECKEY_K, 0},
      {SPECKEY_L, 0},
      {SPECKEY_ENTER, 0},
      {SPECKEY_SHIFT, 0},
      {SPECKEY_Z, 0},
      {SPECKEY_X, 0},
      {SPECKEY_C, 0},
      {SPECKEY_V, 0},
      {SPECKEY_B, 0},
      {SPECKEY_N, 0},
      {SPECKEY_M, 0},
      {SPECKEY_SYMB, 0},
      {SPECKEY_SPACE, 0}};

    Keyboardbutton(KeyEventType keyEvent, PressKeyEventType pressKeyEvent);
    static void keyboardbuttonTask(void *pParam);

    };