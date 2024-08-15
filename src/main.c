
#include "inc/mario.h"

void after_frame_callback() {
  if ((A_B_Buttons & PAD_A) != 0) {
    PlayerFacingDir = 0;
  }
}
