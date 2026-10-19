#ifndef CMD_DISPATCHER_H_
#define CMD_DISPATCHER_H_

#include <stdint.h>
#include "protocol_fsm.h"

/* Komut Kimlikleri (Command IDs) */
typedef enum {
    CMD_PING        = 0x01,
    CMD_LED_CONTROL = 0x02,
    CMD_GET_STATUS  = 0x03
} CommandID_t;

/* Fonksiyon Bildirimleri */
void CmdDispatcher_Init(void);
void CmdDispatcher_Execute(const Packet_t *packet);

#endif /* CMD_DISPATCHER_H_ */
