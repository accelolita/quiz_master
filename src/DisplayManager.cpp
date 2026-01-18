#include "DisplayManager.h"

DisplayManager::DisplayManager() : tft(nullptr) {}

// ディスプレイの初期化
void DisplayManager::init() {
  if (tft == nullptr) {
    tft = new TFT_eSPI();
  }
  tft->init();
  tft->setRotation(1); // 横画面 (Landscape)
  tft->fillScreen(TFT_BLACK);
  // Serial.println("DisplayManager::init() - TFT Init Skipped");
}

// 起動時のウェルカム画面を表示
void DisplayManager::showWelcome() {
  if (!tft)
    return;
  tft->fillScreen(TFT_BLACK);
  tft->setTextColor(TFT_WHITE, TFT_BLACK);
  tft->setTextSize(2);
  tft->setCursor(10, 10);
  tft->println("Quiz Master");

  tft->setTextSize(1);
  tft->setCursor(10, 40);
  tft->println("Initializing...");
  // Serial.println("DisplayManager::showWelcome() - Skipped");
}

// ステータスメッセージを表示
void DisplayManager::showStatus(const String &message, uint16_t color) {
  if (!tft)
    return;
  tft->setTextColor(color, TFT_BLACK);
  tft->println(message);
  // Serial.println("DisplayManager::showStatus() - Skipped: " + message);
}

// エラーメッセージを赤色で表示
void DisplayManager::showError(const String &message) {
  showStatus(message, TFT_RED);
}

TFT_eSPI *DisplayManager::getTft() { return tft; }
