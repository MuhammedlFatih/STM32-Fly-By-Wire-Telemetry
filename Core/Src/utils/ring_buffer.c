#include "ring_buffer.h"
#include <stddef.h>

/* Bellek başlatma */
RingBuffer_Status_t RingBuffer_Init(RingBuffer_t *rb, uint8_t *storage_buffer, uint16_t size) {
    if (rb == NULL || storage_buffer == NULL || size == 0) {
        return RING_BUFFER_ERR_NULL_PTR;
    }

    rb->storage = storage_buffer;
    rb->capacity = size;
    rb->head = 0;
    rb->tail = 0;

    return RING_BUFFER_OK;
}

/* Boş mu? head ve tail aynı yerdeyse okunacak yeni bir şey yoktur */
bool RingBuffer_IsEmpty(const RingBuffer_t *rb) {
    if (rb == NULL) {
        return true;
    }
    return (rb->head == rb->tail);
}

/* Dolu mu? head bir adım atarsa tail ile çakışacak mı? */
bool RingBuffer_IsFull(const RingBuffer_t *rb) {
    if (rb == NULL) {
        return false;
    }
    uint16_t next_head = (rb->head + 1) % rb->capacity;
    return (next_head == rb->tail);
}

/* VERİ YAZMA (Genelde UART Kesmesi çağırır) */
RingBuffer_Status_t RingBuffer_Put(RingBuffer_t *rb, uint8_t data) {
    if (rb == NULL || rb->storage == NULL) {
        return RING_BUFFER_ERR_NULL_PTR;
    }

    /* 1. Kontrol: Doluysa üzerine yazıp eski veriyi ezme */
    if (RingBuffer_IsFull(rb)) {
        return RING_BUFFER_FULL;
    }

    /* 2. Yazma: head'in gösterdiği hücreye veriyi koy */
    rb->storage[rb->head] = data;

    /* 3. İlerleme: head parmağını bir sonraki kutuya kaydır (sona geldiyse başa dön) */
    rb->head = (rb->head + 1) % rb->capacity;

    return RING_BUFFER_OK;
}

/* VERİ OKUMA (Ana döngü veya Parser çağırır) */
RingBuffer_Status_t RingBuffer_Get(RingBuffer_t *rb, uint8_t *data) {
    if (rb == NULL || rb->storage == NULL || data == NULL) {
        return RING_BUFFER_ERR_NULL_PTR;
    }

    /* 1. Kontrol: Boşsa okunacak veri yoktur */
    if (RingBuffer_IsEmpty(rb)) {
        return RING_BUFFER_EMPTY;
    }

    /* 2. Okuma: tail'in gösterdiği hücredeki veriyi al */
    *data = rb->storage[rb->tail];

    /* 3. İlerleme: tail parmağını bir sonraki kutuya kaydır */
    rb->tail = (rb->tail + 1) % rb->capacity;

    return RING_BUFFER_OK;
}
