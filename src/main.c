
#include "inc/mario.h"

#if ENABLE_C_CALLBACKS

void after_frame_callback() {
  if ((A_B_Buttons & PAD_A) != 0) {
    PlayerFacingDir = 0;
  }
}

#endif
