#ifndef _TYPES_H_
#define _TYPES_H_

#include <stdint.h>
#include <stdbool.h>

typedef union _HWORD_VAL {
    uint16_t val;
    uint8_t v[2];
    struct {
        uint8_t lb;
        uint8_t hb;
    } bytes;
    struct {
        unsigned char b0: 1;
        unsigned char b1: 1;
        unsigned char b2: 1;
        unsigned char b3: 1;
        unsigned char b4: 1;
        unsigned char b5: 1;
        unsigned char b6: 1;
        unsigned char b7: 1;
        unsigned char b8: 1;
        unsigned char b9: 1;
        unsigned char b10: 1;
        unsigned char b11: 1;
        unsigned char b12: 1;
        unsigned char b13: 1;
        unsigned char b14: 1;
        unsigned char b15: 1;
    } bits;
} hword_val_t;

typedef union _WORD_VAL {
    uint32_t val;
    uint8_t v[4];
    struct {
        uint16_t lval;
        uint16_t hval;
    } hwords;
    struct {
        uint8_t byte0;
        uint8_t byte1;
        uint8_t byte2;
        uint8_t byte3;
    } bytes;
} word_val_t;

// usefull shortcut macros
#define VAL16(_x_) (_x_.val)
#define VAL16_L8(_x_) (_x_.bytes.lb)
#define VAL16_H8(_x_) (_x_.bytes.hb)
#define VAL16_B(_x_, _n_) (_x_.v[_n_])

#define VAL32(_x_) (_x_.val)
#define VAL32_L16(_x_) (_x_.hwords.lval)
#define VAL32_H16(_x_) (_x_.hwords.hval)
#define VAL32_B(_x_, _n_) (_x_.v[_n_])

#endif
