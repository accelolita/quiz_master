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
  // メッセージをブロードキャスト送信
  void broadcast(uint8_t data);

  // ステータス信号の定義
  static const uint8_t SIGNAL_QUESTION = 0x01;

private:
  esp_now_peer_info_t _broadcastPeerInfo;
};

#endif
