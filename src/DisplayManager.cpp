#include "DisplayManager.h"
#include <cstdint>

DisplayManager::DisplayManager() : tft(nullptr) {}

// ディスプレイの初期化
void DisplayManager::init() {
  if (tft == nullptr) {
    tft = new TFT_eSPI();
  }
  tft->init();
  tft->setRotation(1); // 横画面 (Landscape)
  tft->fillScreen(TFT_BLACK);
  tft->init();
  tft->setRotation(1); // 横画面 (Landscape)
  tft->fillScreen(TFT_BLACK);

  // タッチキャリブレーション（実測値またはデフォルト値）
  uint16_t calData[5] = {
      300, 3500, 300, 3500,
      1}; // [0]=minX, [1]=maxX, [2]=minY, [3]=maxY, [4]=rotation
  tft->setTouch(calData);

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

  // ステータスエリアをクリア (枠線は残すため、内側だけ塗りつぶす)
  tft->fillRect(2, STATUS_AREA_Y + 2, 316, STATUS_AREA_H - 4, TFT_BLACK);

  tft->setTextColor(color, TFT_BLACK);
  tft->setTextSize(2);    // 少し大きくする
  tft->setCursor(10, 30); // 枠内の適切な位置に合わせる
  tft->println(message);
}

// エラーメッセージを赤色で表示
void DisplayManager::showError(const String &message) {
  showStatus(message, TFT_RED);
}

TFT_eSPI *DisplayManager::getTft() { return tft; }

// GUIの枠組みを描画
void DisplayManager::drawGUI() {
  if (!tft)
    return;
  tft->fillScreen(TFT_BLACK);

  // ステータスエリアの枠
  tft->drawRect(0, STATUS_AREA_Y, 320, STATUS_AREA_H, TFT_BLUE);

  // ボタン描画 (VOL -)
  tft->fillRect(BTN_MINUS_X, BUTTON_AREA_Y, BTN_W, BTN_H, TFT_DARKGREY);
  tft->drawRect(BTN_MINUS_X, BUTTON_AREA_Y, BTN_W, BTN_H, TFT_WHITE);
  tft->setTextColor(TFT_WHITE, TFT_DARKGREY);
  tft->setTextSize(2);
  tft->setCursor(BTN_MINUS_X + 10, BUTTON_AREA_Y + 20); // テキスト位置微調整
  tft->print("VOL -");

  // ボタン描画 (VOL +)
  tft->fillRect(BTN_PLUS_X, BUTTON_AREA_Y, BTN_W, BTN_H, TFT_DARKGREY);
  tft->drawRect(BTN_PLUS_X, BUTTON_AREA_Y, BTN_W, BTN_H, TFT_WHITE);
  tft->setTextColor(TFT_WHITE, TFT_DARKGREY);
  tft->setCursor(BTN_PLUS_X + 10, BUTTON_AREA_Y + 20); // テキスト位置微調整
  tft->print("VOL +");

  // 音量ラベル
  tft->setTextColor(TFT_WHITE, TFT_BLACK);
  tft->setCursor(VOL_LABEL_X, VOL_TEXT_Y);
  tft->print("VOL:");
}

// 音量表示を更新
void DisplayManager::updateVolume(int volume) {
  if (!tft)
    return;
  // 前の数字を消すために黒で塗りつぶし
  // 前の数字を消すために黒で塗りつぶし
  tft->fillRect(VOL_VALUE_X, VOL_TEXT_Y, 40, 20, TFT_BLACK);

  tft->setTextColor(TFT_WHITE, TFT_BLACK);
  tft->setTextSize(2);
  tft->setCursor(VOL_VALUE_X, VOL_TEXT_Y);
  tft->print(volume);
}

// タッチ処理 (音量変更があればtrue)
// タッチ処理 (0:なし, 1:UP, -1:DOWN)
int DisplayManager::handleTouch() {
  if (!tft)
    return 0;

  uint16_t x, y;
  // タッチが押されているか確認
  if (tft->getTouch(&x, &y)) {
    uint16_t rawX = x;
    uint16_t rawY = y;

    // 座標補正 (180度回転のズレを修正)
    x = 320 - x;
    y = 240 - y;

    Serial.printf("Touch: Raw(%d,%d) -> Corr(%d,%d) (BTN-: %d-%d)\n", rawX,
                  rawY, x, y, BTN_MINUS_X, BTN_MINUS_X + BTN_W); // Debug Log

    // デバウンス処理が必要な場合はここに追加

    // VOL - ボタン判定
    if (x >= BTN_MINUS_X && x <= (BTN_MINUS_X + BTN_W) && y >= BUTTON_AREA_Y &&
        y <= (BUTTON_AREA_Y + BTN_H)) {
      return -1;
    }

    // VOL + ボタン判定
    if (x >= BTN_PLUS_X && x <= (BTN_PLUS_X + BTN_W) && y >= BUTTON_AREA_Y &&
        y <= (BUTTON_AREA_Y + BTN_H)) {
      return 1;
    }
  }
  return 0;
}
