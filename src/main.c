
#include "inc/mario.h"

#if ENABLE_C_CODE

#if USE_CUSTOM_TITLESCREEN
PUSHSEG(TITLE)
#include "title.c"
POPSEG()
#endif

void sprite_render_callback() {
  
}

void before_frame_callback() {

}

void after_frame_callback() {
#if USE_MOUSE_SUPPORT
  if (!mouse.connected) {
    return;
  }
  if (mouse.left) {
    Player_X_Position = mouse.x + ScreenLeft_X_Pos;
    Player_Y_Position = mouse.y;
  }
#endif
}

#endif
