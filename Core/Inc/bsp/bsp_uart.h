#ifndef BSP_UART_H
#define BSP_UART_H

#include <stdint.h>
#include <stdbool.h>

/* İş mantığının kullanacağı donanımdan bağımsız gönderme fonksiyonu */
bool BSP_UART_Send(uint8_t *data, uint16_t len);

#endif /* BSP_UART_H */
