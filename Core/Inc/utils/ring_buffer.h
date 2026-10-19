#ifndef UTILS_RING_BUFFER_H_
#define UTILS_RING_BUFFER_H_

#include <stdint.h>
#include <stdbool.h>

/* İşlem sonuç durumları */
typedef enum {
    RING_BUFFER_OK = 0,
    RING_BUFFER_EMPTY,
    RING_BUFFER_FULL,
    RING_BUFFER_ERR_NULL_PTR
} RingBuffer_Status_t;

/* Ring Buffer Kontrol Yapısı */
typedef struct {
    uint8_t *storage;       /* Ham veri tamponuna işaretçi */
    uint16_t capacity;      /* Dizi kapasitesi (byte) */
    volatile uint16_t head; /* Yazma indeksi (Kesme/ISR artırır) */
    volatile uint16_t tail; /* Okuma indeksi (Uygulama/Task artırır) */
} RingBuffer_t;

/* Fonksiyon Bildirimleri */
RingBuffer_Status_t RingBuffer_Init(RingBuffer_t *rb, uint8_t *storage_buffer, uint16_t size);
RingBuffer_Status_t RingBuffer_Put(RingBuffer_t *rb, uint8_t data);
RingBuffer_Status_t RingBuffer_Get(RingBuffer_t *rb, uint8_t *data);
bool RingBuffer_IsEmpty(const RingBuffer_t *rb);
bool RingBuffer_IsFull(const RingBuffer_t *rb);
uint16_t RingBuffer_GetCount(const RingBuffer_t *rb);

#endif /* UTILS_RING_BUFFER_H_ */
