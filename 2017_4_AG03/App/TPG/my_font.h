#ifndef _MY_FONT_H
#define _MY_FONT_H

#include <stdint.h>

#define MY_FONT_WIDTH        8
#define MY_FONT_HEIGHT       1
#define MY_FONT_ROWS         200
#define MY_FONT_COLS         384
#define MY_FONT_COUNT        9600
#define MY_FONT_BYTES_PER_CHAR 1
#define MY_FONT_TOTAL_BYTES   9600

extern const uint8_t my_font[9600];

#endif // _MY_FONT_H
