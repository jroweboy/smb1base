
#include "inc/mario.h"

#if ENABLE_C_CODE

#if USE_CUSTOM_TITLESCREEN
PUSHSEG(TITLE)
#include "title.c"
POPSEG()
#endif

WRAPPED(extern void DrawAllMetasprites(););
WRAPPED(void flicker_wifi_lagging(););
WRAPPED(void UpdatePing());

extern u8 CalcPingCooldown;
extern u8 EllipseAnimation;
extern u8 ScanlinePpuMask[3];
extern u8 ScanlineTarget[4];

void init_area_callback() {
  init_ping_values();
  CalcPingCooldown = 0;
  EllipseAnimation = 0;
}

void after_game_callback() {

}

void after_frame_callback() {
  
  flicker_wifi_lagging();

  CalcPingCooldown++;
  if (CalcPingCooldown == 60) {
    CalcPingCooldown = 0;

    calculate_ping();
    UpdatePing();

    if (NotRespondingTimer > 0) {
      ScanlineTarget[1] = 70;
      Mirror_PPUMASK |= 0b11100001;
      ScanlinePpuMask[0] = Mirror_PPUMASK;
      // Setting pause status to 1 will pause without playing the sound
      GamePauseStatus = 1;

      #define i M0
      #define n R0
      // force draw all sprites during lag
      if (NotRespondingQueued) {
        NotRespondingQueued = 0;
        StartedNotRespondingPopup = 1;
        DrawAllMetasprites();
        // so we can move off screen any sprites are in the middle of the banner
        for (i = 0; i < 64; ++i) {
          n = i << 2;
          M1 = Sprite_Data[n];
          if (M1 > (74 - 16) && M1 < 141) {
            Sprite_Data[n] = 0xf8;
          }
        }
      }

      // and then animate the ellipsis ...
      ++EllipseAnimation;
      if (EllipseAnimation > 3) {
        EllipseAnimation = 0;
      }

      // Counter for the number of filled ellipses
      M1 = 0;
      for (i = 0; i < 3; ++i) {
        n = (61*4) + (i << 2);
        if (i < EllipseAnimation) {
          Sprite_Data[n] = 112;
        } else {
          // Clear out the dots that are after the current animation frame
          Sprite_Data[n] = 0xf8;
        }
        Sprite_Data[n+1] = 0x76;
        Sprite_Data[n+2] = 3;
        Sprite_Data[n+3] = (21*8) + (i << 3);
      }

      #undef i
    }
    if (StartedNotRespondingPopup && NotRespondingTimer == 0) {
      StartedNotRespondingPopup = 0;
      ScanlineTarget[1] = 0xff;
      Mirror_PPUMASK &= (~0b11100001);
      ScanlinePpuMask[0] = Mirror_PPUMASK;
      // PPU.mask = Mirror_PPUMASK;
      GamePauseStatus = 0;
    }
  }
}

#endif
