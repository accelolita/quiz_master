#include "AudioManager.h"
#include "DisplayManager.h"
#include "NetworkManager.h"
#include <Arduino.h>

// --- 定数定義 ---
#define DFPLAYER_TX 17
#define DFPLAYER_RX 18
#define DFPLAYER_BUSY 38

#define BTN_QUESTION 15
#define BTN_CORRECT 1
#define BTN_INCORRECT 2

// --- グローバルオブジェクト ---
DisplayManager display;
AudioManager audio(DFPLAYER_RX, DFPLAYER_TX, DFPLAYER_BUSY);
NetworkManager network;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n--- Quiz Master Start ---");

  // PSRAM Check
  if (psramFound()) {
    Serial.printf("PSRAM ID: %08X, Size: %d bytes\n", 0, ESP.getPsramSize());
    Serial.println("PSRAM Found and Initialized");
  } else {
    Serial.println("PSRAM NOT FOUND or FAILED");
  }

  // 1. ディスプレイ設定PSRAM Check
  if (psramFound()) {
    Serial.printf("PSRAM ID: %08X, Size: %d bytes\n", 0, ESP.getPsramSize());
    Serial.println("PSRAM Found and Initialized");
  } else {
    Serial.println("PSRAM NOT FOUND or FAILED");
  }

  // 1. ディスプレイ設定
  delay(2000); // コールドブート時のホワイトスクリーン対策
  Serial.println("Debug: Calling display.init()");
  display.init();
  Serial.println("Debug: Calling display.showWelcome()");
  display.showWelcome();
  Serial.println("Display Initialized");

  // 2. オーディオ設定
  Serial.println("Initializing DFPlayer...");
  Serial.println("Debug: Calling audio.init()");
  if (audio.init()) {
    Serial.println("DFPlayer online.");
    display.showStatus("Audio OK");
    // audio.play(1); // 起動音などを再生する場合
  } else {
    Serial.println("Unexpected error: DFPlayer not detected!");
    display.showError("Audio Error!");
  }

  // 3. ESP-NOW設定
  Serial.println("Debug: Calling network.init()");
  if (network.init()) {
    Serial.println("ESP-NOW Initialized");
    display.showStatus("ESP-NOW OK");
  } else {
    Serial.println("Error initializing ESP-NOW");
    display.showError("Network Error!");
  }

  // 4. ボタン設定
  pinMode(BTN_QUESTION, INPUT_PULLUP);
  pinMode(BTN_CORRECT, INPUT_PULLUP);
  pinMode(BTN_INCORRECT, INPUT_PULLUP);

  // 4. GUI描画 & 初期状態表示
  display.drawGUI();
  display.showStatus("System Ready");
  display.updateVolume(audio.getVolume());

  Serial.println("System Ready.");
}

void loop() {
  // タッチ操作による音量変更
  // タッチ操作による音量変更
  int touchResult = display.handleTouch();
  if (touchResult != 0) {
    // ボタンが押されたときだけ現在の音量を取得して変更する
    int currentVolume = audio.getVolume();
    int newVolume = currentVolume + touchResult;

    // setVolume側で範囲制限(0-30 or 1-31)されるのでそのままセット
    audio.setVolume(newVolume);

    // 反映された値を再取得して表示更新
    display.updateVolume(audio.getVolume());

    delay(100); // 簡易デバウンス
  }
  // メインループのロジック

  // 簡易デバウンス処理
  static unsigned long lastDebounceTime = 0;
  unsigned long debounceDelay = 200;

  if (millis() - lastDebounceTime > debounceDelay) {
    if (digitalRead(BTN_QUESTION) == LOW) {
      Serial.println("Question Button Pressed");
      audio.playQuestion();
      display.showStatus("Question!");
      network.broadcast(NetworkManager::SIGNAL_QUESTION);
      lastDebounceTime = millis();
    } else if (digitalRead(BTN_CORRECT) == LOW) {
      Serial.println("Correct Button Pressed");
      audio.playCorrect();
      display.showStatus("Correct!");
      lastDebounceTime = millis();
    } else if (digitalRead(BTN_INCORRECT) == LOW) {
      Serial.println("Incorrect Button Pressed");
      audio.playIncorrect();
      display.showStatus("Incorrect!");
      lastDebounceTime = millis();
    }
  }

  delay(10);
}
