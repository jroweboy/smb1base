
#ifndef __JOYPAD_H
#define __JOYPAD_H

#define PAD_A       0x80
#define PAD_B       0x40
#define PAD_SELECT  0x20
#define PAD_START   0x10
#define PAD_UP      0x08
#define PAD_DOWN    0x04
#define PAD_LEFT    0x02
#define PAD_RIGHT   0x01

/* Masks for joy_read */
#define JOY_UP_MASK     0x10
#define JOY_DOWN_MASK   0x20
#define JOY_LEFT_MASK   0x40
#define JOY_RIGHT_MASK  0x80
#define JOY_BTN_1_MASK  0x01
#define JOY_BTN_2_MASK  0x02
#define JOY_BTN_3_MASK  0x04
#define JOY_BTN_4_MASK  0x08

#define JOY_BTN_A_MASK  JOY_BTN_1_MASK
#define JOY_BTN_B_MASK  JOY_BTN_2_MASK
#define JOY_SELECT_MASK JOY_BTN_3_MASK
#define JOY_START_MASK  JOY_BTN_4_MASK

#define JOY_BTN_A(v)    ((v) & JOY_BTN_A_MASK)
#define JOY_BTN_B(v)    ((v) & JOY_BTN_B_MASK)
#define JOY_SELECT(v)   ((v) & JOY_SELECT_MASK)
#define JOY_START(v)    ((v) & JOY_START_MASK)

#define JOY_PRESSED(v, buttons) (((v) & (buttons)) == (buttons))

/* Define hardware */

/* Picture Processing Unit */
struct __ppu {
    unsigned char control;
    unsigned char mask;                 /* color; show sprites, background */
      signed char volatile const status;
    struct {
        unsigned char address;
        unsigned char data;
    } sprite;
    unsigned char scroll;
    struct {
        unsigned char address;
        unsigned char data;
    } vram;
};
#define PPU             (*(struct __ppu*)0x2000)
#define SPRITE_DMA      (APU.sprite.dma)

#endif // __JOYPAD_H
