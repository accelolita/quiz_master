#include "NetworkManager.h"
#include <Arduino.h>

NetworkManager::NetworkManager() {}

// ネットワーク(ESP-NOW)の初期化
bool NetworkManager::init() {
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    return false;
  }

  esp_now_register_recv_cb(NetworkManager::onDataRecv);

  // ブロードキャスト用ピアの登録
  memset(&_broadcastPeerInfo, 0, sizeof(_broadcastPeerInfo));
  for (int i = 0; i < 6; ++i) {
    _broadcastPeerInfo.peer_addr[i] = 0xFF;
  }
  _broadcastPeerInfo.channel = 0;
  _broadcastPeerInfo.encrypt = false;

  if (esp_now_add_peer(&_broadcastPeerInfo) != ESP_OK) {
    Serial.println("Failed to add broadcast peer");
    return false;
  }

  return true;
}

// データ受信時の処理
void NetworkManager::onDataRecv(const uint8_t *mac, const uint8_t *incomingData,
                                int len) {
  Serial.print("Bytes received: ");
  Serial.println(len);
  // TODO: データ受信時のロジックやメインへのコールバックを実装する
}

// メッセージをブロードキャスト送信
void NetworkManager::broadcast(uint8_t data) {
  uint8_t broadcastAddr[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  esp_err_t result = esp_now_send(broadcastAddr, &data, sizeof(data));

  if (result == ESP_OK) {
    Serial.println("Broadcast Sent Successfully");
  } else {
    Serial.println("Error sending the data");
  }
}
