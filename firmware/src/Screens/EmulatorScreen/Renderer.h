#pragma once
#include <freertos/FreeRTOS.h>
#include <string.h>
#include "../../TFT/Display.h"
#include "../../Serial.h"

void displayTask(void *pvParameters);

class HDMIDisplay;
class AudioOutput;
class Renderer {
private:
    Display &m_tft;
    AudioOutput *m_audioOutput = nullptr;
    HDMIDisplay *m_HDMIDisplay = nullptr;
    // holding area for pixels that are sent to the tft display
    uint16_t *pixelBuffer = nullptr;
    // what's currently on the spectrum screen
    uint8_t *currentScreenBuffer = nullptr;
    // what's currently on the TFT screen
    uint8_t *screenBuffer = nullptr;
    // the current borders of the spectrum screen
    uint8_t currentBorderColors[312] = {0};
    // the current borders on the TFT screen
    uint8_t drawnBorderColors[312] = {0};
    // control the drawing of the screen
    SemaphoreHandle_t m_displaySemaphore;
    // are we ready to draw?
    bool drawReady = true;
    // is this the first draw?
    bool firstDraw = true;
    // flash timer - every 32 frames
    uint16_t flashTimer = 0;
    const int screenWidth = TFT_WIDTH;
    const int screenHeight = TFT_HEIGHT;
    const int borderWidth = (screenWidth - 256) / 2;
    const int borderHeight = (screenHeight - 192) / 2;
    // draw the borders
    void drawBorder(int startPos, int endPos, int offset, int length, int drawWidth, int drawHeight, bool isSideBorders);
    // draw the screen
    void drawScreen();
    // draw the spectrum screen
    void drawSpectrumScreen();
    // draw the menu
    void drawMenu();
    // draw the time travel screen
    void drawTimeTravel();
    // display task - runs continuously and draws the screen
    // controlled bu the m_displaySemaphore
    friend void displayTask(void *pvParameters);
    // are we loading? - this is used for tzx files
    bool isLoading = false;
    uint16_t loadProgress = 0;
    // should we be rendering
    bool isRunning = false;
    // keep track of how many frames we've drawn
    uint32_t frameCount = 0;
public:
    Renderer(Display &tft, AudioOutput *audioOutput, HDMIDisplay *hdmiDisplay): m_tft(tft), m_audioOutput(audioOutput), m_HDMIDisplay(hdmiDisplay) {
      // enough for a row of 8 pixels
      pixelBuffer = (uint16_t *)malloc(256 * 8 * sizeof(uint16_t));
      // the spectrum screen is 256x192 pixels
      screenBuffer = (uint8_t *)malloc(6912);
      if (screenBuffer == NULL)
      {
        Serial.println("Failed to allocate screen buffer");
      }
      memset(screenBuffer, 0, 6912);
      currentScreenBuffer = (uint8_t *)malloc(6912);
      if (currentScreenBuffer == NULL)
      {
        Serial.println("Failed to allocate current screen buffer");
      }
      memset(currentScreenBuffer, 0, 6912);
      m_displaySemaphore = xSemaphoreCreateBinary();
    }
    void start() {
      xTaskCreatePinnedToCore(displayTask, "displayTask", 8192, this, 1, NULL, 1);
      isRunning = true;
    }
    ~Renderer() {
      free(pixelBuffer);
      free(screenBuffer);
      free(currentScreenBuffer);
    }
    void triggerDraw(const uint8_t *currentScreen, const uint8_t *borderColors) {
      if (drawReady) {
        drawReady = false;
        memcpy(currentScreenBuffer, currentScreen, 6912);
        memcpy(currentBorderColors, borderColors, 312);
        xSemaphoreGive(m_displaySemaphore);
      }
    }
    void setIsLoading(bool loading) {
      isLoading = loading;
    }
    void pause() {
      isRunning = false;
    }
    void resume() {
      isRunning = true;
      firstDraw = true;
    }
    uint32_t getFrameCount() {
      return frameCount;
    }
    void resetFrameCount() {
      frameCount = 0;
    }
    void setLoadProgress(uint16_t progress) {
      loadProgress = progress;
    }
    void setNeedsRedraw() {
      firstDraw = true;
    }
    void forceRedraw(const uint8_t *currentScreen = nullptr, const uint8_t *borderColors = nullptr) {
      if (currentScreen != nullptr) {
        memcpy(currentScreenBuffer, currentScreen, 6912);
      }
      if (borderColors != nullptr) {
        memcpy(currentBorderColors, borderColors, 312);
      }
      firstDraw = true;
      drawScreen();
    }
    bool isShowingMenu = false;
    bool isShowingTimeTravel = false;
};
