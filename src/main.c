
#include "inc/mario.h"

#if ENABLE_C_CODE

#if USE_CUSTOM_TITLESCREEN
PUSHSEG(TITLE)
#include "title.c"
POPSEG()
#endif

WRAPPED(void flicker_wifi_lagging());
WRAPPED(void InitFollower());

extern u8 CalcPingCooldown;

void init_area_callback() {
  InitFollower();
  init_ping_values();
  CalcPingCooldown = 0;
}

void after_game_callback() {
}

void after_frame_callback() {

  flicker_wifi_lagging();

  CalcPingCooldown++;
  if (CalcPingCooldown == 60) {
    CalcPingCooldown = 0;

    calculate_ping();
  }
}

#endif
