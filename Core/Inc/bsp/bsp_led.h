#ifndef BSP_LED_H_
#define BSP_LED_H_

#include <stdint.h>

typedef enum {
    LED_OFF = 0,
    LED_ON  = 1,
    LED_TOGGLE = 2
} LedState_t;

void HW_LED_Init(void);
void HW_LED_Set(LedState_t state);

#endif /* BSP_LED_H_ */
