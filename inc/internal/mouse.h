
#ifndef __MOUSE_H
#define __MOUSE_H

#include "options.h"

#if USE_MOUSE_SUPPORT

#include "common_types.h"

typedef struct mouse_s {
  union {
    uint8_t status_computed;
    struct {
        uint8_t unused : 3;
        uint8_t right_pressed : 1;
        uint8_t left_pressed : 1;
        uint8_t right_released : 1;
        uint8_t left_released : 1;
        uint8_t connected : 1;
    };
  };
  union {
    uint8_t status_raw;
    struct {
#if MOUSE_CONFIG_SENSITIVITY
        uint8_t signature : 4;
        uint8_t sensitivity : 2;
#else
        uint8_t signature : 4;
        uint8_t unused2 : 2;
#endif 
        uint8_t left : 1;
        uint8_t right : 1;
    };
  };
  uint8_t y;
  uint8_t x;
} Mouse;

_Static_assert (sizeof (Mouse) == 4, "Mouse struct must be 4 bytes");

extern Mouse mouse;
#pragma zpsym("mouse");

#endif

#endif // __MOUSE_H