#include "bsp_uart.h"
#include "main.h" /* HAL kütüphaneleri sadece bu dosyada include edilir */

/* UART handle sadece donanım katmanında (burada) bilinir */
extern UART_HandleTypeDef huart2;

bool BSP_UART_Send(uint8_t *data, uint16_t len) {
    if (HAL_UART_Transmit(&huart2, data, len, 100) == HAL_OK) {
        return true;
    }
    return false;
}
