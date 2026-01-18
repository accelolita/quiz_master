#include "AudioManager.h"

AudioManager::AudioManager(int rxPin, int txPin, int busyPin)
    : _rxPin(rxPin), _txPin(txPin), _busyPin(busyPin),
      mySerial(&Serial1) { // UART1を使用
}

// オーディオモジュールの初期化
bool AudioManager::init() {
  mySerial->begin(9600, SERIAL_8N1, _rxPin, _txPin);
  pinMode(_busyPin, INPUT);

  if (!myDFPlayer.begin(*mySerial)) {
    return false;
  }

  // デフォルト設定
  myDFPlayer.volume(20);
  return true;
}

// 音量を設定 (0-30)
void AudioManager::setVolume(int volume) { myDFPlayer.volume(volume); }

// 指定したファイル番号の音声を再生
void AudioManager::play(int fileNumber) { myDFPlayer.play(fileNumber); }

void AudioManager::playQuestion() { play(SOUND_QUESTION); }
void AudioManager::playCorrect() { play(SOUND_CORRECT); }
void AudioManager::playIncorrect() { play(SOUND_INCORRECT); }

// 再生中かどうかを判定
bool AudioManager::isBusy() {
  // BUSYピンは再生中にLOWになる
  return digitalRead(_busyPin) == LOW;
}
