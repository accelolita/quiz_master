#include "NetworkManager.h"
#include "AudioManager.h"
#include <Arduino.h>

extern AudioManager audio;

NetworkManager *NetworkManager::instance = nullptr;

NetworkManager::NetworkManager() { instance = this; }

// ネットワーク(ESP-NOW)の初期化
bool NetworkManager::init() {
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return false;
  }
  Serial.println(WiFi.macAddress());
  esp_now_register_recv_cb(NetworkManager::onDataRecv);
  esp_now_register_send_cb(NetworkManager::onDataSent);

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

  if (len == 1) {
    uint8_t receivedId = incomingData[0];
    Serial.print("Child ID Received: ");
    Serial.println(receivedId);

    // 子機からIDを受け取ったら、回答権付与をブロードキャストする
    // 本来は早い者勝ち判定等のロジックが必要だが、ここでは即応答する例とする
    if (instance) {
      Serial.print("Broadcasting Answer Permission for ID: ");
      Serial.println(receivedId);
      audio.playAnswerPermission(); // 回答権付与音を鳴らす
      instance->broadcast(receivedId, CMD_ANSWER_PERMISSION);
    }
  }
}

// データ送信完了時のコールバック
void NetworkManager::onDataSent(const uint8_t *mac_addr,
                                esp_now_send_status_t status) {
  // 送信頻度が高い場合はログを抑制したほうが良いかもしれないが一旦残す
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success"
                                                : "Delivery Fail");
}

// メッセージをブロードキャスト送信 (生データ)
void NetworkManager::broadcast(uint8_t data) {
  Serial.println("Debug: NetworkManager::broadcast called");
  uint8_t broadcastAddr[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  esp_err_t result = esp_now_send(broadcastAddr, &data, sizeof(data));

  if (result == ESP_OK) {
    Serial.println("Broadcast Sent Successfully (Queued)");
  } else {
    Serial.print("Error sending the data. Error Code: ");
    Serial.println(result);
  }
}

// メッセージをブロードキャスト送信 (ID + コマンド)
void NetworkManager::broadcast(uint8_t id, uint8_t cmd) {
  uint8_t packet = createPacket(id, cmd);
  Serial.printf("Broadcasting Packet: ID=%d, CMD=%d, HEX=0x%02X\n", id, cmd,
                packet);
  broadcast(packet);
}
