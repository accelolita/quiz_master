#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>

// オーディオ管理クラス (DFPlayer Mini)
class AudioManager {
public:
  AudioManager(int rxPin, int txPin, int busyPin);
  bool init();                // 初期化
  void setVolume(int volume); // 音量設定
  int getVolume();            // 現在の音量取得
  void play(int fileNumber);  // 指定番号のファイル再生
  void playQuestion();        // 出題音 (001.mp3)
  void playCorrect();         // 正解音 (002.mp3)
  void playIncorrect();       // 不正解音 (003.mp3)
  bool isBusy();              // 再生中かどうか確認

  static const int VOL_MAX = 30;
  static const int VOL_MIN = 0;
  static const int VOL_DEFAULT = 20;

private:
  static const int SOUND_QUESTION = 1;
  static const int SOUND_CORRECT = 2;
  static const int SOUND_INCORRECT = 3;

private:
  int _rxPin;
  int _txPin;
  int _busyPin;
  int _volume; // 現在の音量
  DFRobotDFPlayerMini myDFPlayer;
  HardwareSerial *mySerial;
};

#endif
