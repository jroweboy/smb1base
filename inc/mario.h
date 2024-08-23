

#ifndef __MARIO_H
#define __MARIO_H

#include "internal/charmap.h"
#include "internal/common_types.h"
#include "internal/common_nes.h"
#include "internal/memory.h"
#include "internal/metasprites.h"
#include "internal/options.h"

#include <peekpoke.h>


// Gameplay callbacks
void after_frame_callback();

// Title screen callbacks

#if USE_CUSTOM_TITLESCREEN
void title_screen_setup();
void title_screen_menu();
#endif

// Somewhat useful functions

extern u8 VRAM_Buffer_Offset[2];
extern u8 VRAM_AddrTable_Low[];
extern u8 VRAM_AddrTable_High[];
void flush_vram_buffer();

#endif
