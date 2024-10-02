
#include "inc/mario.h"

#if ENABLE_C_CODE

#if USE_CUSTOM_TITLESCREEN
PUSHSEG(TITLE)
#include "title.c"
POPSEG()
#endif



void after_frame_callback() {
#if USE_MOUSE_SUPPORT
  if (mouse.left) {
    Player_X_Position = mouse.x;
    Player_Y_Position = mouse.y;
  }
#endif
}

#endif
