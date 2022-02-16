//================================================================================
//
// program name   : tinyESPNow.cpp
// date/author    : 2021/02/13 @chrmlinux03
//
// MIT License Copyright(c)2022 @chrmlinux03 https://github.com/chrmlinux/tinyESPNow
//
//================================================================================

#include <Arduino.h>
#include "tinyESPNow.h"

static bool __recved = false;
static uint8_t __myId = 0xFF;
static uint8_t *__transAry = NULL;
static uint32_t __aryOffset = 0;
static uint32_t __arySize = 0;
static uint32_t __sendOffset = 0;
static bool __aryFull = false;

//======================================
// onDataSent
//--------------------------------------
// 
//======================================
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
}

//======================================
// onDataRecv
//--------------------------------------
// +--+---...................--+ 
// |my|                        |
// |iD|   Data                 |
// +--+---...................--+
//======================================
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *recv, int recvCnt) {
  if (recv[0] == __myId) {
    memcpy(__transAry, recv, recvCnt);
    __recved = true;
  }
}

//======================================
// tinyESPNow
//--------------------------------------
// 
//======================================
tinyESPNow::tinyESPNow(void) {
}

tinyESPNow::~tinyESPNow(void) {
}

//======================================
// resultCheck
//--------------------------------------
// 
//======================================
void tinyESPNow::resultCheck(int cnt, esp_err_t result) {

  switch (result) {
    case ESP_OK:
      break;
    case ESP_ERR_ESPNOW_NOT_INIT:
      Serial.printf("%d:ESPNOW not Init.\n", cnt);
      break;
    case ESP_ERR_ESPNOW_ARG:
      Serial.printf("%d:Invalid Argument.\n", cnt);
      break;
    case ESP_ERR_ESPNOW_INTERNAL:
      Serial.printf("%d:Internal Error.\n", cnt);
      break;
    case ESP_ERR_ESPNOW_NO_MEM:
      Serial.printf("%d:esp_err_espnow_no_mem.\n", cnt);
      break;
    case ESP_ERR_ESPNOW_NOT_FOUND:
      Serial.printf("%d:Peer not found.\n", cnt);
      break;
    default:
      Serial.printf("%d:Not sure what happened.\n", cnt);
      break;
  }

}

//======================================
// get
//--------------------------------------
// 
//======================================
uint8_t* tinyESPNow::get(void) {
  __aryOffset = 0;
  __recved = false;
  return __transAry;
}

//======================================
// put
//--------------------------------------
// 
//======================================
uint16_t tinyESPNow::put(uint8_t *dt, uint32_t cnt) {
  __aryOffset = 0;
  __sendOffset = cnt;
  memcpy(__transAry, dt, __sendOffset);
  return 0;
}

//======================================
// begin
//--------------------------------------
// 
//======================================
uint16_t tinyESPNow::begin(uint8_t myId, uint32_t arySize) {
  __arySize = arySize;
  __transAry = new uint8_t[__arySize];

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  if (esp_now_init() == ESP_OK) {
    Serial.print("ESPNow Init Success\n");
  } else {
    Serial.print("ESPNow Init Failed\n");
    ESP.restart();
  }
  memset(&_slave, 0x0, sizeof(_slave));
  for (int i = 0; i < sizeof(_slave.peer_addr); ++i) {
    _slave.peer_addr[i] = (uint8_t)0xff;
  }
  esp_err_t addStatus = esp_now_add_peer(&_slave);
  esp_now_register_send_cb(&OnDataSent);
  esp_now_register_recv_cb(&OnDataRecv);

  __myId = myId;
  return 0;
}

//======================================
// isBufFull
//--------------------------------------
// 
//======================================
bool tinyESPNow::isAryFull(void) {
  return __aryFull;
}

//======================================
// isRecv
//--------------------------------------
// 
//======================================
bool tinyESPNow::isRecv(void) {
  return __recved;
}

//======================================
// end
//--------------------------------------
// 
//======================================
uint16_t tinyESPNow::end(void) {
  free(__transAry);
  return 0;
}

//======================================
// dump
//--------------------------------------
// 
//======================================
uint16_t tinyESPNow::dump(uint32_t pos, uint32_t len) {
  static uint16_t cnt = 0;
  Serial.println();
  for (uint32_t i = 0; i < len; i ++) {
    Serial.printf("%02x ", __transAry[pos + i]);
    cnt = (cnt + 1) % 16;
    if (!cnt) Serial.println();
  }

  __aryOffset = 0;
  __aryFull = false;
}

//======================================
// send
//--------------------------------------
// 
//======================================
uint16_t tinyESPNow::send(void) {
  esp_err_t result;
  result = esp_now_send(_slave.peer_addr, __transAry, __sendOffset);
  delay(ESPNOW_SEND_DELAY);
  resultCheck(-1, result);
}

//======================================
// update
//--------------------------------------
// 
//======================================
uint16_t tinyESPNow::update(void) {
  delay(1);
  return 0;
}
