#include "bsp_led.h"
#include "stm32f4xx_hal.h"

#define LED_PORT  GPIOA
#define LED_PIN   GPIO_PIN_5

void HW_LED_Init(void) {
    // CubeMX GPIO'yu başlattığı için boş kalabilir
}

void HW_LED_Set(LedState_t state) {
    switch (state) {
        case LED_OFF:
            HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
            break;
        case LED_ON:
            HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
            break;
        case LED_TOGGLE:
            HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
            break;
        default:
            break;
    }
}
