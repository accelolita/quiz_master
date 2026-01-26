#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <WiFi.h>
#include <esp_now.h>

// ネットワーク管理クラス (ESP-NOW)
class NetworkManager {
public:
  NetworkManager();
  bool init(); // 初期化
  // データ受信時のコールバック関数
  static void onDataRecv(const uint8_t *mac, const uint8_t *incomingData,
                         int len);
  // データ送信完了時のコールバック
  // データ送信完了時のコールバック
  static void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
  // メッセージをブロードキャスト送信 (生データ)
  void broadcast(uint8_t data);
  // メッセージをブロードキャスト送信 (ID + コマンド)
  void broadcast(uint8_t id, uint8_t cmd);

  // コマンド定義 (下位3ビット)
  static const uint8_t CMD_QUESTION = 0x01;          // 0b001: 出題
  static const uint8_t CMD_ANSWER_PERMISSION = 0x02; // 0b010: 回答権付与
  static const uint8_t CMD_CORRECT = 0x03;           // 0b011: 正解
  static const uint8_t CMD_INCORRECT = 0x04;         // 0b100: 不正解

  // ビットマスクとシフト
  static const uint8_t CMD_MASK = 0x07; // 0b00000111
  static const uint8_t ID_MASK = 0xF8;  // 0b11111000
  static const uint8_t ID_SHIFT = 3;

  // パケット生成ヘルパー
  static uint8_t createPacket(uint8_t id, uint8_t cmd) {
    return ((id << ID_SHIFT) & ID_MASK) | (cmd & CMD_MASK);
  }

  // シングルトンインスタンスへのポインタ (staticコールバックからアクセス用)
  static NetworkManager *instance;

private:
  esp_now_peer_info_t _broadcastPeerInfo;
};

#endif
