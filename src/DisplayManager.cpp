#include "DisplayManager.h"
#include <cstdint>

DisplayManager::DisplayManager() : tft(nullptr), ts(nullptr) {}

// ディスプレイの初期化
void DisplayManager::init() {
  if (tft == nullptr) {
    tft = new TFT_eSPI();
  }
  tft->init();
  tft->setRotation(1); // 横画面 (Landscape)
  tft->fillScreen(TFT_BLACK);

  // XPT2046初期化
  if (ts == nullptr) {
    // TOUCH_IRQはplatformio.iniで定義されていると仮定 (GPIO 8)
#ifdef TOUCH_IRQ
    ts = new XPT2046_Touchscreen(TOUCH_CS, TOUCH_IRQ);
#else
    ts = new XPT2046_Touchscreen(TOUCH_CS);
#endif
    ts->begin();
    ts->setRotation(1); // 画面に合わせる
  }

  // Serial.println("DisplayManager::init() - TFT & Touch Init Done");
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

// タッチ処理 (0:なし, 1:UP, -1:DOWN)
int DisplayManager::handleTouch() {
  if (!ts || !ts->touched())
    return 0;

  TS_Point p = ts->getPoint();

  // マッピング: 生の値(0-4095) -> 画面座標(320x240)
  // XPT2046の座標系は実測して調整が必要だが、とりあえず標準的な範囲でマッピング
  // 必要に応じて map 関数の引数を調整してください
  // 例: map(value, min_raw, max_raw, min_screen, max_screen)

  // 反転が必要な場合を考慮して計算
  // マッピング: 生の値(0-4095) -> 画面座標(320x240)
  // 左上が(320,240)になっているとのことで、出力を反転させます (0,0)にするため
  // User Feedback: LT(-5,1), LB(-8,236), RT(308,-3) -> Adjusted Range
  int16_t x = map(p.x, 320, 3860, 320, 0);
  int16_t y = map(p.y, 250, 3800, 240, 0);

  // 以前のキャリブレーションデータに基づき、必要なら反転などの補正を追加
  // XPT2046ライブラリのsetRotation(1)である程度合うはずだが、ズレる場合はここで補正

  Serial.printf("Touch Raw: %d, %d -> Screen: %d, %d (BTN-: %d-%d)\n", p.x, p.y,
                x, y, BTN_MINUS_X, BTN_MINUS_X + BTN_W);

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

  return 0;
}
