#pragma once
#include <Arduino.h>
#include "USB.h"
#include "USBHIDKeyboard.h"
//#include <unordered_map>
#include <set>
#include <vector>
#include <string>
//#include "EspUsbHost.h"
#include "../Emulator/keyboard_defs.h"
//#include "hid_host.h"
/*const std::unordered_map<SpecKeys, int> specKeyusbKey = {
    {SPECKEY_SHIFT, HID_KEY_SHIFT_LEFT},
    {SPECKEY_ENTER, HID_KEY_RETURN},
    {SPECKEY_SPACE, HID_KEY_SPACE},
    {SPECKEY_SYMB, HID_KEY_ALT_RIGHT},
    {SPECKEY_1, HID_KEY_1},
    {SPECKEY_2, HID_KEY_2},
    {SPECKEY_3, HID_KEY_3},
    {SPECKEY_4, HID_KEY_4},
    {SPECKEY_5, HID_KEY_5},
    {SPECKEY_6, HID_KEY_6},
    {SPECKEY_7, HID_KEY_7},
    {SPECKEY_8, HID_KEY_8},
    {SPECKEY_9, HID_KEY_9},
    {SPECKEY_0, HID_KEY_0},
    {SPECKEY_A, HID_KEY_A},
    {SPECKEY_B, HID_KEY_B},
    {SPECKEY_C, HID_KEY_C},
    {SPECKEY_D, HID_KEY_D},
    {SPECKEY_E, HID_KEY_E},
    {SPECKEY_F, HID_KEY_F},
    {SPECKEY_G, HID_KEY_G},
    {SPECKEY_H, HID_KEY_H},
    {SPECKEY_I, HID_KEY_I},
    {SPECKEY_J, HID_KEY_J},
    {SPECKEY_K, HID_KEY_K},
    {SPECKEY_L, HID_KEY_L},
    {SPECKEY_M, HID_KEY_M},
    {SPECKEY_N, HID_KEY_N},
    {SPECKEY_O, HID_KEY_O},
    {SPECKEY_P, HID_KEY_P},
    {SPECKEY_Q, HID_KEY_Q},
    {SPECKEY_R, HID_KEY_R},
    {SPECKEY_S, HID_KEY_S},
    {SPECKEY_T, HID_KEY_T},
    {SPECKEY_U, HID_KEY_U},
    {SPECKEY_V, HID_KEY_V},
    {SPECKEY_W, HID_KEY_W},
    {SPECKEY_X, HID_KEY_X},
    {SPECKEY_Y, HID_KEY_Y},
    {SPECKEY_Z, HID_KEY_Z},
};
*/
class USBKeyboard {
  private:
  
    using KeyEventType = std::function<void(SpecKeys keyCode, bool isPressed)>;
    KeyEventType m_keyEvent;
   //void onKeyboardKey(uint8_t ascii, uint8_t keycode, uint8_t modifier)
     using PressKeyEventType = std::function<void(SpecKeys keyCode)>;
    PressKeyEventType m_pressKeyEvent;
    
  public:
  //USBKeyboard(SpecKeys key, bool down);
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
   //USBKeyboard(KeyEventType keyEvent);
  USBKeyboard(KeyEventType keyEvent, PressKeyEventType pressKeyEvent);
  static void usbkeyboardTask(void *pParam);
  
  };