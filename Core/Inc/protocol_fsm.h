#ifndef PROTOCOL_FSM_H_
#define PROTOCOL_FSM_H_

#include <stdint.h>
#include <stdbool.h>

#define PROTOCOL_HEADER_BYTE   0xAA
#define PROTOCOL_MAX_PAYLOAD   32

/* FSM Durumları */
typedef enum {
    FSM_STATE_WAIT_HEADER = 0,
    FSM_STATE_WAIT_LEN,
    FSM_STATE_WAIT_PAYLOAD,
    FSM_STATE_WAIT_CHECKSUM
} FsmState_t;

/* Ayrıştırılan Tam Paket Yapısı */
typedef struct {
    uint8_t cmd_id;
    uint8_t data[PROTOCOL_MAX_PAYLOAD - 1];
    uint8_t data_len;
} Packet_t;

/* FSM Kontrol Bloğu */
typedef struct {
    FsmState_t state;
    uint8_t payload_len;
    uint8_t rx_index;
    uint8_t raw_payload[PROTOCOL_MAX_PAYLOAD];
    uint8_t calculated_checksum;
} ProtocolFsm_t;

/* Fonksiyon Bildirimleri */
void FSM_Init(ProtocolFsm_t *fsm);
bool FSM_ProcessByte(ProtocolFsm_t *fsm, uint8_t byte, Packet_t *out_packet);

#endif /* PROTOCOL_FSM_H_ */
