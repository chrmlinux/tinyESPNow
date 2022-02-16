//================================================================================
//
// program name   : tinyESPNow.h
// date/author    : 2021/02/13 @chrmlinux03
//
// License
// MIT Copyright(c)2022 @chrmlinux03 https://github.com/chrmlinux/tinyESPNow
//
//================================================================================

#ifndef _TINYESPNOW_H_
#define _TINYESPNOW_H_

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

#define STX (0x02) // Start Of Text
#define ETX (0x03) // End Of Text
#define ESPNOW_SEND_DELAY (3)

class  tinyESPNow {

  public:

    tinyESPNow(void);
    ~tinyESPNow(void);

    uint16_t begin(uint8_t, uint32_t);
    uint16_t end(void);
    uint16_t update(void);
    uint16_t dump(uint32_t, uint32_t);
    uint16_t put(uint8_t*, uint32_t);
    uint16_t send(void);
    uint8_t* get(void);
    bool isAryFull(void);
    bool isRecv(void);

  private:

    void resultCheck(int, esp_err_t);
    esp_now_peer_info_t _slave;

};

#endif
