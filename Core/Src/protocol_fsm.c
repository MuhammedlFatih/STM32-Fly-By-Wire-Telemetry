#include "protocol_fsm.h"
#include <stddef.h>

void FSM_Init(ProtocolFsm_t *fsm) {
    if (fsm == NULL) return;
    fsm->state = FSM_STATE_WAIT_HEADER;
    fsm->payload_len = 0;
    fsm->rx_index = 0;
    fsm->calculated_checksum = 0;
}

/**
 * @brief Gelen her tek bir baytı analiz eder.
 * @return true: Geçerli ve doğrulanmış tam bir paket tamamlandı.
 *         false: Paket henüz tamamlanmadı veya geçersiz bayt geldi.
 */
bool FSM_ProcessByte(ProtocolFsm_t *fsm, uint8_t byte, Packet_t *out_packet) {
    if (fsm == NULL || out_packet == NULL) {
        return false;
    }

    switch (fsm->state) {
        case FSM_STATE_WAIT_HEADER:
            if (byte == PROTOCOL_HEADER_BYTE) {
                fsm->calculated_checksum = byte; // Checksum hesabı header ile başlar
                fsm->state = FSM_STATE_WAIT_LEN;
            }
            break;

        case FSM_STATE_WAIT_LEN:
            /* Boyut 1 ile PROTOCOL_MAX_PAYLOAD arasında olmalı */
            if (byte > 0 && byte <= PROTOCOL_MAX_PAYLOAD) {
                fsm->payload_len = byte;
                fsm->rx_index = 0;
                fsm->calculated_checksum ^= byte;
                fsm->state = FSM_STATE_WAIT_PAYLOAD;
            } else {
                /* Geçersiz boyut -> Hata! Başa dön */
                fsm->state = FSM_STATE_WAIT_HEADER;
            }
            break;

        case FSM_STATE_WAIT_PAYLOAD:
            fsm->raw_payload[fsm->rx_index++] = byte;
            fsm->calculated_checksum ^= byte;

            /* İstenen bayt sayısı tamamlandı mı? */
            if (fsm->rx_index >= fsm->payload_len) {
                fsm->state = FSM_STATE_WAIT_CHECKSUM;
            }
            break;

        case FSM_STATE_WAIT_CHECKSUM:
            fsm->state = FSM_STATE_WAIT_HEADER; // Paket gelse de gelmese de sonraki arama başa döner

            /* Checksum doğrulandı mı? */
            if (byte == fsm->calculated_checksum) {
                /* Paketi ayrıştır ve dışarıya teslim et */
                out_packet->cmd_id = fsm->raw_payload[0];
                out_packet->data_len = fsm->payload_len - 1;

                for (uint8_t i = 0; i < out_packet->data_len; i++) {
                    out_packet->data[i] = fsm->raw_payload[1 + i];
                }
                return true; // Başarılı paket!
            }
            break;

        default:
            fsm->state = FSM_STATE_WAIT_HEADER;
            break;
    }

    return false;
}
