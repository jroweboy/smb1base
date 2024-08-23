
#include "inc/mario.h"

#if ENABLE_C_CODE

#if USE_CUSTOM_TITLESCREEN
PUSHSEG(TITLE)
#include "title.c"
POPSEG()
#endif

extern u8 CalcPingCooldown;
extern u8 ScanlinePpuMask[3];
extern u8 ScanlineTarget[4];

void init_area_callback() {
  init_ping_values();
}

void after_game_callback() {

}

void after_frame_callback() {
  // if ((A_B_Buttons & PAD_A) != 0) {
  //   PlayerFacingDir = 0;
  // }
  CalcPingCooldown++;
  if (CalcPingCooldown == 60) {
    CalcPingCooldown = 0;

    calculate_ping();
    if (NotRespondingTimer > 0) {
      ScanlineTarget[1] = 70;
      Mirror_PPUMASK |= 0b11100001;
      ScanlinePpuMask[0] = Mirror_PPUMASK;
      // Setting pause status to 1 will pause without playing the sound
      GamePauseStatus = 1;
    }
    if (NotRespondingTimer == 0) {
      ScanlineTarget[1] = 0xff;
      Mirror_PPUMASK &= (~0b11100001);
      ScanlinePpuMask[0] = Mirror_PPUMASK;
      // PPU.mask = Mirror_PPUMASK;
      GamePauseStatus = 0;
    }
  }
}

#endif
