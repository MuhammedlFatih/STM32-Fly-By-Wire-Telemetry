#include "cmd_dispatcher.h"
#include "bsp_uart.h"
#include "telemetry_api.h"
#include <stddef.h>

/* O(1) Fonksiyon İşaretçisi Tablosu İçin Tip Tanımı */
typedef void (*CmdHandlerFn_t)(const uint8_t *, uint16_t);

/* 8-bit komut ID'si için 256 elemanlı O(1) arama tablosu */
static CmdHandlerFn_t command_table[256];

/* Komut işleyici (handler) prototipi */
void CmdHandler_SensorTelemetry(const uint8_t *payload, uint16_t len);

void CmdDispatcher_Init(void) {
    /* Başlangıçta tüm tabloyu güvenli duruma (NULL) getir */
    for (uint16_t i = 0; i < 256; i++) {
        command_table[i] = NULL;
    }

    /* 0x03 komutunu sensör okuma fonksiyonuna bağla */
    command_table[0x03] = CmdHandler_SensorTelemetry;
}

/* Orijinal sözleşmene (.h) sadık kalındı */
void CmdDispatcher_Execute(const Packet_t *packet) {
    if (packet == NULL) return;

    /* switch-case olmaksızın O(1) karmaşıklığında doğrudan erişim ve çalıştırma */
    /* Not: Eğer Packet_t struct'ın içindeki değişken isimlerin farklıysa
       (örneğin 'cmd_id' yerine 'cmd', 'len' yerine 'length'), aşağıdaki 3 ismi kendi struct'ına göre düzelt. */
    if (command_table[packet->cmd_id] != NULL) {
        command_table[packet->cmd_id](packet->data, packet->data_len);
    }
}

void CmdHandler_SensorTelemetry(const uint8_t *payload, uint16_t len) {
    SensorData_t current_data;
    uint8_t tx_buf[10];

    /* 1. Veriyi Güvenli API'den Mutex Korumalı Olarak Çek */
    if (!Telemetry_GetSensorData(&current_data)) {
        return;
    }

    /* 2. Donanım Bağımsız Veri Paketi Oluşturma */
    tx_buf[0] = 0xAA;                                 /* Header */
    tx_buf[1] = 0x08;                                 /* Length */
    tx_buf[2] = 0x03;                                 /* Command ID */
    tx_buf[3] = (current_data.accel_x >> 8) & 0xFF;   /* X High */
    tx_buf[4] = current_data.accel_x & 0xFF;          /* X Low */
    tx_buf[5] = (current_data.accel_y >> 8) & 0xFF;   /* Y High */
    tx_buf[6] = current_data.accel_y & 0xFF;          /* Y Low */
    tx_buf[7] = (current_data.accel_z >> 8) & 0xFF;   /* Z High */
    tx_buf[8] = current_data.accel_z & 0xFF;          /* Z Low */
    tx_buf[9] = 0x08 ^ 0x03;                          /* Checksum (Length XOR Cmd_ID) */

    /* 3. BSP Katmanı Üzerinden İletim */
    BSP_UART_Send(tx_buf, 10);
}
