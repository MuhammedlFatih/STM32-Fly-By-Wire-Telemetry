#ifndef TELEMETRY_API_H
#define TELEMETRY_API_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    int16_t accel_x;
    int16_t accel_y;
    int16_t accel_z;
} SensorData_t;

/* Veriyi Mutex ile güvenli bir şekilde okumak için API */
bool Telemetry_GetSensorData(SensorData_t *out_data);

#endif /* TELEMETRY_API_H */
