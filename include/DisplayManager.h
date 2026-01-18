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
  TFT_eSPI *getTft(); // TFTインスタンス取得（必要な場合）

private:
  TFT_eSPI *tft;
};

#endif
