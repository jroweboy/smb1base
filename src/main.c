
#include "inc/mario.h"

#if ENABLE_C_CODE

#if USE_CUSTOM_TITLESCREEN
PUSHSEG(TITLE)
#include "title.c"
POPSEG()
#endif

extern u8 CalcPingCooldown;

void init_area_callback() {
  init_ping_values();
}

void after_frame_callback() {
  // if ((A_B_Buttons & PAD_A) != 0) {
  //   PlayerFacingDir = 0;
  // }
  CalcPingCooldown++;
  if (CalcPingCooldown == 60) {
    CalcPingCooldown = 0;
    calculate_ping();
  }
}

#endif
