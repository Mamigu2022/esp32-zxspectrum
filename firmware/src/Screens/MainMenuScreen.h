#pragma once

#include "PickerScreen.h"
#include "../Files/Files.h"
#include "ErrorScreen.h"
#include "AlphabetPicker.h"
#include "EmulatorScreen.h"
#include "VideoFilePickerScreen.h"
#include "GameFilePickerScreen.h"
#include "AboutScreen.h"
#include "TestScreen.h"

static const std::vector<std::string> gameValidExtensions = {".z80", ".sna", ".tap", ".tzx"};
static const std::vector<std::string> videoValidExtensions = {".avi"};

class MenuItem
{
public:
  MenuItem(const std::string &title, std::function<void()> onSelect) : title(title), onSelect(onSelect) {}
  std::string getTitle() const { return title; }
  // callback for when the item is selected
  std::function<void()> onSelect;

private:
  std::string title;
};

using MenuItemPtr = std::shared_ptr<MenuItem>;
using MenuItemVector = std::vector<MenuItemPtr>;


class MainMenuScreen : public PickerScreen<MenuItemPtr>
{
public:
  MainMenuScreen(Display &tft, HDMIDisplay *hdmiDisplay, AudioOutput *audioOutput, IFiles *files) 
  : PickerScreen("Main Menu", tft, hdmiDisplay, audioOutput, files)
  {
    // Main menu
    MenuItemVector menuItems = {
        std::make_shared<MenuItem>("48K ZX Spectrum", [&]()
                                   { this->run48K(); }),
        std::make_shared<MenuItem>("128K ZX Spectrum", [&]()
                                   { this->run128K(); }),
        std::make_shared<MenuItem>("Games", [&]()
                                   { this->showGames(); }),
        std::make_shared<MenuItem>("Snapshots", [&]()
                                   { this->showSnapshots(); }),
        std::make_shared<MenuItem>("Video Player", [&]()
                                   { this->showVideos(); }),
        std::make_shared<MenuItem>("About", [&]()
                                   { this->showAbout(); }),
#ifdef ENABLE_MSC
        std::make_shared<MenuItem>("Mount SD Card", [&]()
                                  { this->mountSDCard(); }),
#endif
    };
    setItems(menuItems);
  }

  void onItemSelect(MenuItemPtr item, int index)
  {
    item->onSelect();
  }

  void run48K()
  {
    EmulatorScreen *emulatorScreen = new EmulatorScreen(m_tft, m_hdmiDisplay, m_audioOutput, m_files);
    emulatorScreen->run("", models_enum::SPECMDL_48K);
    // touchKeyboard->setToggleMode(true);
    m_navigationStack->push(emulatorScreen);
  }
  void run128K()
  {
    EmulatorScreen *emulatorScreen = new EmulatorScreen(m_tft, m_hdmiDisplay, m_audioOutput, m_files);
    emulatorScreen->run("", models_enum::SPECMDL_128K);
    // touchKeyboard->setToggleMode(true);
    m_navigationStack->push(emulatorScreen);
  }

  template <class FilterPickerScreen_T> void showAlphabetPicker(
    std::string title,
    std::vector<std::string> no_sd_card_error,
    std::vector<std::string> no_files_error,
    std::string path,
    std::vector<std::string> extensions
    )
  {
    if (!m_files->isAvailable())
    {
      ErrorScreen *errorScreen = new ErrorScreen(
          no_sd_card_error,
          m_tft,
          m_hdmiDisplay,
          m_audioOutput,
          m_files);
      m_navigationStack->push(errorScreen);
      return;
    }
    drawBusy();
    FileLetterCountVector fileLetterCounts = m_files->getFileLetters(path.c_str(), extensions);
    if (fileLetterCounts.size() == 0)
    {
      ErrorScreen *errorScreen = new ErrorScreen(
        no_files_error,
        m_tft,
        m_hdmiDisplay,
        m_audioOutput,
        m_files);
      m_navigationStack->push(errorScreen);
      return;
    }
    AlphabetPicker<FilterPickerScreen_T> *alphabetPicker = new AlphabetPicker<FilterPickerScreen_T>(title, m_files,  m_tft, m_hdmiDisplay, m_audioOutput, path, extensions);
    alphabetPicker->setItems(fileLetterCounts);
    m_navigationStack->push(alphabetPicker);
  }

  void showGames()
  {
    showAlphabetPicker<GameFilePickerScreen>(
      "Games",
      {"No SD Card", "Insert an SD Card", "to load games"},
      {"No games found", "on the SD Card", "add Z80 or SNA files"},
      "/",
      gameValidExtensions
    );
  }

  void showSnapshots()
  {
    showAlphabetPicker<GameFilePickerScreen>(
      "Snapshots",
      {"No SD Card", "Insert an SD Card", "to load games"},
      {"No snapshots found", "on the SD Card", "save some snapshots", "during game play"},
      "/snapshots",
      gameValidExtensions
    );
  }

  void showVideos()
  {
    showAlphabetPicker<VideoFilePickerScreen>(
      "Videos",
      {"No SD Card", "Insert an SD Card", "to load games"},
      {"No videos found", "on the SD Card", "copy some AVI files", "onto the card"},
      "/",
      videoValidExtensions
    );
  }

  void mountSDCard()
  {
    startMSC();
  }

  void showAbout()
  {
    AboutScreen *aboutScreen = new AboutScreen(m_tft, m_hdmiDisplay, m_audioOutput, m_files);
    m_navigationStack->push(aboutScreen);
  }
};
