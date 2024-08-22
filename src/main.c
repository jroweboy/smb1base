
#include "inc/mario.h"

#if ENABLE_C_CODE

#if USE_CUSTOM_TITLESCREEN
PUSHSEG(TITLE)
#include "title.c"
POPSEG()
#endif



void after_frame_callback() {
  // if ((A_B_Buttons & PAD_A) != 0) {
  //   PlayerFacingDir = 0;
  // }
}

#endif
