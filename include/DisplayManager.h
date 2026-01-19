#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <TFT_eSPI.h>

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
  bool handleTouch(int &volume); // タッチ処理 (音量変更があればtrue)

  TFT_eSPI *getTft(); // TFTインスタンス取得（必要な場合）

private:
private:
  TFT_eSPI *tft;

  // GUI Layout Constants
  static const int STATUS_AREA_Y = 0;
  static const int STATUS_AREA_H = 100;
  static const int BUTTON_AREA_Y = 120;
  static const int BTN_W = 100;
  static const int BTN_H = 60;
  static const int BTN_MINUS_X = 20;
  static const int BTN_PLUS_X = 200;
  static const int VOL_TEXT_Y = 140;
};

#endif
