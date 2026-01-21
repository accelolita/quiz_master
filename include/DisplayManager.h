#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

// ディスプレイ管理クラス
class DisplayManager {
public:
  DisplayManager();
  void init();        // 初期化
  void showWelcome(); // ウェルカム画面表示
  void showStatus(const String &message,
                  uint16_t color = TFT_WHITE); // ステータス表示
  void showError(const String &message);       // エラー表示

  void drawGUI();                // GUI描画 (ボタン枠など)
  void updateVolume(int volume); // 音量表示更新
  int handleTouch();             // タッチ処理 (0:なし, 1:UP, -1:DOWN)

  TFT_eSPI *getTft(); // TFTインスタンス取得（必要な場合）

private:
private:
  TFT_eSPI *tft;
  XPT2046_Touchscreen *ts;

  // GUI Layout Constants
  static const int STATUS_AREA_Y = 0;
  static const int STATUS_AREA_H = 100;
  static const int BUTTON_AREA_Y = 120;
  static const int BTN_W = 80; // ボタン幅を少し狭く
  static const int BTN_H = 60;
  static const int BTN_MINUS_X = 10; // 左寄せ
  static const int BTN_PLUS_X = 230; // 右寄せ
  static const int VOL_TEXT_Y = 140;
  static const int VOL_LABEL_X = 110; // "VOL:" ラベル位置
  static const int VOL_VALUE_X = 170; // 数値表示位置
};

#endif
