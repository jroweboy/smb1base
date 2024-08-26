
#include "inc/mario.h"


WRAPPED(extern void GameCoreRoutine());
WRAPPED(extern void LoadAreaPointer());
WRAPPED(extern void DrawMushroomIcon());
WRAPPED(extern void DemoEngine());
WRAPPED(void calculate_ping());
WRAPPED(void init_ping_values());
WRAPPED(extern void DrawAllMetasprites(););
WRAPPED(void UpdatePing());

extern u8 galois32();

extern u8 WSelectBufferTemplate[6];


extern u8 EnableWifi;
extern s8 ServerIndex;

extern u8 FlickerFever;
extern u8 LagSpikeCooldown;
extern u8 LagSpikeDuration;
extern u8 NotRespondingQueued;
extern u8 NotRespondingTimer;
extern u8 NotRespondingCount;
extern u8 StartedNotRespondingPopup;
extern u8 PlayerFrozenFlag;
extern u8 PlayerFrozenTimer;
extern u8 F_Frame;
extern u8 F_StopPoint;
extern u16 BasePing;
extern u16 CurrentPing;
extern u16 PingFlux;
extern u16 FrameDelayAmount;
extern u32 seed;

extern u8 NmiBackgroundProtect;
extern u8 ScanlinePpuMask[3];
extern u8 ScanlineTarget[4];
extern u8 EllipseAnimation;


void flicker_wifi_lagging() {
  // Flicker the wifi indicator in the middle of the screen
  if (StartedNotRespondingPopup == 0 || PlayerFrozenFlag != 0) {
    // Flicker by checking frame counter
    if ((LagSpikeDuration != 0 || PlayerFrozenFlag != 0) && (FlickerFever & 1)) {
      if (PlayerSize == 0) { // big mario
          if (Sprite_Data[3 * 4 + 0] > 0xf0) {
            return;
          }
          M0 = Sprite_Data[3 * 4 + 0] - 20;
          M1 = Sprite_Data[3 * 4 + 3];
      } else {
          if (Sprite_Data[1 * 4 + 0] > 0xf0) {
            return;
          }
          M0 = Sprite_Data[1 * 4 + 0] - 20;
          M1 = Sprite_Data[1 * 4 + 3];
      }
      Sprite_Data[59 * 4 + 0] = M0;
      Sprite_Data[59 * 4 + 1] = 0x1c;
      Sprite_Data[59 * 4 + 2] = 2;
      Sprite_Data[59 * 4 + 3] = M1;
      Sprite_Data[60 * 4 + 0] = M0;
      Sprite_Data[60 * 4 + 1] = 0x1e;
      Sprite_Data[60 * 4 + 2] = 2;
      Sprite_Data[60 * 4 + 3] = M1+8;
    } else {
      Sprite_Data[59 * 4 + 0] = 0xf8;
      Sprite_Data[60 * 4 + 0] = 0xf8;
    }
    FlickerFever ^= 1;
  }
}

#define SERVER_COUNT 4

const unsigned char title_nmt[32*12];
static const char connect_text[] = "CONNECT TO SERVER REGION";
static const char region_text_list[SERVER_COUNT][17] = {
  "<  NORTH AMER. >",
  "< U.K - EUROPE >",
  "< JAPAN - ASIA >",
  "<  ANTARCTICA  >",
};
static const char ethernet_text[] = "ETHERNET";
static const char wifi_text[] = "WIFI";

// not actually random, but random enough numbers
static u16 PingLUT[SERVER_COUNT] = {
  28,
  136,
  // 189,
  245,
  455
};

// not actually random, but random enough numbers
static u16 PingFluxLUT[SERVER_COUNT] = {
  23,
  // 36,
  70,
  141,
  200
};

void calculate_ping_display() {
#define temp_ping (*(u16*)&R4)
#define i M0
  // Convert from ping value into BCD for the PING display
  i = 0;
  temp_ping = CurrentPing;
  while (temp_ping >= 100) {
    temp_ping -= 100;
    ++i;
  }
  GameTimerDisplay[0] = i;
  i = 0;
  while (temp_ping >= 10) {
    temp_ping -= 10;
    ++i;
  }
  GameTimerDisplay[1] = i;
  GameTimerDisplay[2] = temp_ping;
#undef i
#undef temp_ping
}


void init_ping_values() {
  // Setup the RNG seed etc
  seed = 0xC053236C;
  seed ^= *(u32*)&PseudoRandomBitReg;
  BasePing = PingLUT[ServerIndex];
  PingFlux = PingFluxLUT[ServerIndex];
  CurrentPing = BasePing + PingFlux;
  calculate_ping_display();
  FrameDelayAmount = 0;
  StartedNotRespondingPopup = 0;
  EllipseAnimation = 0;
  PlayerFrozenTimer = 0;
  PlayerFrozenFlag = 0;
  LagSpikeCooldown = 5;
}

void calculate_ping() {
#define ping_flux (*(u16*)&M0)
#define i M0
#define rng (*(u16*)&R4)
#define temp_ping (*(u16*)&R4)
#define min_flux R6

  // Count down lag spike duration first, and when it expires start the cooldown
  if (LagSpikeDuration > 0) {
    --LagSpikeDuration;
  } else if (LagSpikeCooldown > 0) {
    --LagSpikeCooldown;
  }
  if (NotRespondingTimer > 0) {
    --NotRespondingTimer;
  }
  if (PlayerFrozenTimer > 0) {
    --PlayerFrozenTimer;
  }

  // calculate a random fluctuation from the base
  ping_flux = PingFlux + ((EnableWifi) ? 100 : 20);
  min_flux = 0;

  R4 = galois32();
  if (OperMode == 1 && OperMode_Task == 3 && GamePauseStatus == 0) {

    // If the player is frozen during a lag event
    // and check if the timer has expired for this event
    if (PlayerFrozenTimer == 0 && PlayerFrozenFlag == 1) {
      // State 1 is freezing the player in place
      PlayerFrozenFlag = 2;
      PlayerFrozenTimer = 2;
    } else if (PlayerFrozenTimer == 0 && PlayerFrozenFlag == 2) {
      // state 2 is where we start doing the play back
      PlayerFrozenFlag = 0;
    } else if (LagSpikeDuration == 0) {
      // If we aren't in a lag spike yet, check to see if we can start one
      NotRespondingCount = 0;
      if (LagSpikeCooldown == 0) {
        // One in 8 chance on wifi and 1 in 32 on ethernet to do a random ping spike
        // R5 = (EnableWifi) ? 0b00000111 : 0b00011111;
        // R5 = 0b1;
        R5 = 0xff;
        if ((R4 & R5) == R5) {
          if (EnableWifi) {
            ping_flux += 300;
            min_flux = 200;
            LagSpikeCooldown = ((R4 >> 3) & 0b0111) + 8; // 8 to 15 seconds
            LagSpikeDuration = (R4 & 0b111) + 8; // 8 to 16 seconds
          } else {
            ping_flux += 200;
            min_flux = 100;
            LagSpikeCooldown = ((R4 >> 3) & 0b1111) + 10; // 10 to 26 seconds
            LagSpikeDuration = (R4 & 0b11) + 4; // 4 to 7 seconds
          }
        } else {
          // Try a different roll to see if this is a GIGA FREEZE instead
          // R5 = (EnableWifi) ? 0b11100000 : 0b11111000;
          R5 = 0;
          if ((R4 & R5) == R5) {
            // Start a Giga freeze where the player can't move for a few seconds, and
            // then the player avatar "catches up"
            PlayerFrozenFlag = 1;
            PlayerFrozenTimer = 2;
            F_StopPoint = F_Frame;
            // ++F_Frame;
            LagSpikeCooldown = ((EnableWifi) ? 4 : 10) + 4;
          }
        }
      }
    } else {
      // We are in a lag spike, so just increase the flux
      if (EnableWifi) {
        ping_flux += 300;
        min_flux = 200;
      } else {
        ping_flux += 200;
        min_flux = 100;
      }
      
      NotRespondingCount++;

      // During a lag spike we have a chance to freeze up completely ( 1 in 16 )
      // Don't do this if not in gameplay tho, don't want to ruin the surprise
      R5 = (R4 & 0b1111) == 0b1111;
      // R5 = 1;
      if (NotRespondingTimer == 0 && NotRespondingCount > 2 && LagSpikeDuration > 3 && R5) {
        NotRespondingQueued = 1;
        NotRespondingTimer = LagSpikeDuration;
      }
    }
  }

  // R4 = galois32();
  R5 = galois32();
  // RNG is a 16 bit value and ping flux is more like a 10 bit value, so shift to
  // speed this up a lot
  rng >>= 6;
  ping_flux <<= 1;

  while (ping_flux < rng) {
    // rng -= ping_flux;
    rng >>= 1;
  }

  ping_flux >>= 1;
  while (ping_flux < rng) {
    rng -= ping_flux;
    // rng >>= 1;
  }

  CurrentPing = BasePing + min_flux + rng;
  if (CurrentPing > 999) {
    CurrentPing = 999;
  }

  calculate_ping_display();

  // Convert from ping value into delay amount
  // 65k / 999 is roughly 64 / 1 so this is a cheap good approximation
  // FrameDelayAmount = CurrentPing << 6;
  FrameDelayAmount = CurrentPing << 5;
  
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

  }
  if (StartedNotRespondingPopup && NotRespondingTimer == 0) {
    StartedNotRespondingPopup = 0;
    ScanlineTarget[1] = 0xff;
    Mirror_PPUMASK &= (~0b11100001);
    ScanlinePpuMask[0] = Mirror_PPUMASK;
    // PPU.mask = Mirror_PPUMASK;
    GamePauseStatus = 0;
  }

#undef ping_flux
#undef i
#undef rng
#undef temp_ping
#undef min_flux
}



#define pad_pressed M0
#define i M1

void reset_title() {
  OperMode = 0;
  OperMode_Task = 0;
  Sprite0HitDetectFlag = 0;
  ++DisableScreenFlag;
}

void update_world_area_number(u8 world_number) {
  WorldNumber = world_number;
  OffScr_WorldNumber = world_number;
  AreaNumber = 0;
  OffScr_AreaNumber = 0;
}

void select_b_logic() {
  // Check the demo timer to see if we need to reset
  if (DemoTimer == 0) {
    reset_title();
    return;
  }
  // Reset the DemoTimer
  DemoTimer = 0x18;

  // Check the cooldown on pressing either B or Select
  if (SelectTimer == 0) {
    SelectTimer = 0x10;
    // If B was pressed, increment the world select number
    if (JOY_PRESSED(pad_pressed, PAD_B)) {
      WorldSelectNumber = (WorldSelectNumber + 1) & 0b111;
      update_world_area_number(WorldSelectNumber);
      
      // UpdateShroom
      // Update the VRAM buffer with the template for the world select
      ++NmiBackgroundProtect;
      for (i = 0; i < 6; i++) {
        VRAM_Buffer1[i-1] = WSelectBufferTemplate[i];
        VRAM_Buffer1[3] = WorldNumber+1;
      }
      NmiBackgroundProtect = 0;
    } else {
      // Select button must have been presesed, so redraw the MushroomIcon
      // And switch between 1 and 2 player modes
      // NumberOfPlayers ^= 1;
      EnableWifi ^= 1;
      DrawMushroomIcon();
    }
  }
}

void title_screen_menu() {
  // If either player presses start or a + start
  pad_pressed = SavedJoypad1Bits | SavedJoypad2Bits;
  if (JOY_PRESSED(pad_pressed, PAD_START) || JOY_PRESSED(pad_pressed, (PAD_START | PAD_A))) {
    // ChkContinue
    // If the demo timer has expired, reset back to the menu
    if (DemoTimer == 0) {
      reset_title();
      return;
    }

    // If they are holding A, then run the continue world code
    if (JOY_PRESSED(pad_pressed, PAD_A)) {
      // Set the world and area number for both players
      update_world_area_number(ContinueWorld);
    }

    LoadAreaPointer();
    // Load the Hidden 1UP flag for both players
    ++Hidden1UpFlag;
    ++OffScr_Hidden1UpFlag;
    // Reload the game timer from the world/area list
    ++FetchNewGameTimerFlag;
    // Move to the next task (this ends calling this function)
    ++OperMode;

    // If you can choose the world, then you have beat the game and are starting in hardmode
    PrimaryHardMode = WorldSelectEnableFlag;
    OperMode_Task = 0;
    DemoTimer = 0;
    // Clear out the score displays as well
    for (i = 0; i < (sizeof(Player1ScoreDisplay)
          + sizeof(Player2ScoreDisplay)
          + sizeof(Player1CoinDisplay)
          + sizeof(Player2CoinDisplay)
          + sizeof(GameTimerDisplay)); ++i) {
      Player1ScoreDisplay[i] = 0;
    }
    return; 
  }

  // ChkSelect
  if (JOY_PRESSED(pad_pressed, PAD_SELECT)) {
    // Only Select is pressed, so the b logic is ignored here
    select_b_logic();
  } else {
    if (DemoTimer == 0) {
      // Move the controller bits into the cooldown for the select/b button
      SelectTimer = pad_pressed;
      DemoEngine();
#if __NES__
      __asm__("bcc %g", run_demo);
#endif
      // If the carry is set, then the demo is over so reset it
      reset_title();
      return;
    } else if (WorldSelectEnableFlag && JOY_PRESSED(pad_pressed, PAD_B)) {
      // if only the b button is pressed and the DemoTimer isn't expired
      // then run the code to update the screen
      select_b_logic();
    }
  }
  
  // Handle switching between servers
  if ((pad_pressed & (PAD_LEFT | PAD_RIGHT)) != 0 && SelectTimer == 0) {
    SelectTimer = 0x10;
    DemoTimer = 0x18;
    if (JOY_PRESSED(pad_pressed, PAD_LEFT)) {
      --ServerIndex;
      if (ServerIndex < 0) {
        ServerIndex = SERVER_COUNT - 1;
      }
    } else {
      ++ServerIndex;
      if (ServerIndex >= SERVER_COUNT) {
        ServerIndex = 0;
      }
    }

    BasePing = PingLUT[ServerIndex];
    PingFlux = PingFluxLUT[ServerIndex];

    ++NmiBackgroundProtect;
    // VRAM_Buffer_AddrCtrl = 0; // Use VRAM_Buffer1
    M0 = VRAM_Buffer1_Offset;
    // VRAM_Buffer1[M0++] = 0x22;
    ++M0;
    VRAM_Buffer1[M0++] = 0x68;
    VRAM_Buffer1[M0++] = 16; // Write 16 bytes

    for (i = 0; i < 16; ++i) {
      VRAM_Buffer1[M0] = region_text_list[ServerIndex][i];
      ++M0;
    }
    VRAM_Buffer1[M0] = 0x00;
    // Prevent incomplete buffers from being written when catching up
    // by putting the address last. The game only checks that the hi address is nonzero
    VRAM_Buffer1[VRAM_Buffer1_Offset] = 0x22;
    VRAM_Buffer1_Offset += M0;
    NmiBackgroundProtect = 0;
  }

  // NullJoypad
  SavedJoypad1Bits = 0;
run_demo:
  // RunDemo
  GameCoreRoutine();
  // If we are running the lose life routine
  if (GameEngineSubroutine == 6) {
    // ResetTitle
    reset_title();
    return;
  }
  // otherwise we are done
  return;
  
}

#undef i
#undef pad_pressed



WRAPPED(extern void WriteGameText(u8 text_id));
WRAPPED(extern void WriteBottomStatusLine());
WRAPPED(extern void DisplayTimeUp());
WRAPPED(extern void ResetSpritesAndScreenTimer());
WRAPPED(extern void DisplayIntermediate());
WRAPPED(extern void AreaParserTaskControl());
WRAPPED(extern void GetAreaPalette());
WRAPPED(extern void GetBackgroundColor());
WRAPPED(extern void GetAlternatePalette1());
WRAPPED(extern void DrawTitleScreen());
WRAPPED(extern void ClearBuffersDrawIcon());
WRAPPED(extern void WriteTopScore());

// WARNING: Only use this while NMI is disabled
extern void UpdateScreen();
void flush_vram_buffer() {
  // ++NmiBackgroundProtect;
  NmiR0 = VRAM_AddrTable_Low[VRAM_Buffer_AddrCtrl];
  NmiR1 = VRAM_AddrTable_High[VRAM_Buffer_AddrCtrl];
  UpdateScreen(); // update screen with buffer contents
  R0 = 0;
  
  // check for usage of $0341
  if (VRAM_Buffer_AddrCtrl == 6) {
    R0++;
  }

  R1 = VRAM_Buffer_Offset[R0];
  ((u8*) &VRAM_Buffer1_Offset)[R1] = 0;
  VRAM_Buffer1[R1] = 0;
  VRAM_Buffer_AddrCtrl = 0;
  // NmiBackgroundProtect = 0;
  
  // ENABLE_NMI();
  // // Wait for the VRAM buffer to be flushed during NMI to prevent graphic glitches
  // M1 = NmiSkipped;
  // while (M1 == *(volatile u8*)&NmiSkipped);
  // DISABLE_NMI();
  
}

void title_screen_setup() {
  #define w (*((u16*)&R0))
  #define i R0
  #define temp1 M0
  #define temp2 M1

  // static const void * const jumptable[] = {
  //   &&clear_screen,
  //   &&write_top_status_line,
  //   &&setup_palettes,
  // };
  // goto *jumptable[ScreenRoutineTask];

  // clear_screen:
  Mirror_PPUCTRL &= 0x7f;
  PPU.control = Mirror_PPUCTRL;

  // Disable rendering
  DisableScreenFlag = 1;
  Mirror_PPUMASK &= (0b11100111);
  PPU.mask = Mirror_PPUMASK;
  

  ++NmiBackgroundProtect;
  // clear all the palettes
  VRAM_Buffer_AddrCtrl = 0; // Use the VRAM_Buffer1
  VRAM_Buffer1[0] = 0x3f;
  VRAM_Buffer1[1] = 0x00;
  VRAM_Buffer1[2] = 0x20 | 0b01000000; // repeat one byte 0x20 times
  VRAM_Buffer1[3] = 0x00;
  VRAM_Buffer1[4] = 0x00;
  VRAM_Buffer1_Offset = 5;
  // for (i = 0; i < 0x20; i++) {
  //   PPU.vram.data = 0x0f;
  // }
  
  NmiBackgroundProtect = 0;

  flush_vram_buffer();

  // ENABLE_NMI();
  // // Wait for the VRAM buffer to be flushed during NMI to prevent graphic glitches
  // while (NmiDisable == 0);
  // DISABLE_NMI();

  // Clear out the attribute tables
  PPU.vram.address = 0x23;
  PPU.vram.address = 0xc0;
  for (i = 0; i < 0x40; i++) {
    PPU.vram.data = 0x00;
  }

  // Render into Nametable 0
  // And clear all the bytes of it
  PPU.vram.address = 0x20;
  PPU.vram.address = 0x00;
  for (w = 0; w < 0x400; w++) {
    PPU.vram.data = 0x24;
  }
  
  // reset scroll variables
  HorizontalScroll = 0;
  VerticalScroll = 0;
  PPU.scroll = 0;
  PPU.scroll = 0;

  // ++ScreenRoutineTask;
  // return;

  // Now draw the lines at the top of the screen
  WriteGameText(0);
  flush_vram_buffer();

  // Draw the second row of text
  WriteBottomStatusLine();
  flush_vram_buffer();

  // Render the level column by column until its done drawing
  temp1 = ScreenRoutineTask;
  while (temp1 == ScreenRoutineTask) {
    AreaParserTaskControl();
    flush_vram_buffer();
  }

  GetAreaPalette();
  flush_vram_buffer();
  GetBackgroundColor();
  flush_vram_buffer();
  GetAlternatePalette1();
  flush_vram_buffer();


  // Draw the custom SUPER CLOUD GAMIN BROS logo
  PPU.vram.address = 0x20;
  PPU.vram.address = 0x80;
  for (w = 0; w < sizeof(title_nmt); ++w) {
    PPU.vram.data = title_nmt[w];
  }
  // Draw the Connect to server region text
  PPU.vram.address = 0x22;
  PPU.vram.address = 0x24;
  for (i = 0; i < sizeof(connect_text)-1; ++i) {
    PPU.vram.data = connect_text[i];
  }
  PPU.vram.address = 0x22;
  PPU.vram.address = 0x68;
  for (i = 0; i < sizeof(region_text_list[0])-1; ++i) {
    PPU.vram.data = region_text_list[0][i];
  }
  PPU.vram.address = 0x22;
  PPU.vram.address = 0xad;
  for (i = 0; i < sizeof(ethernet_text)-1; ++i) {
    PPU.vram.data = ethernet_text[i];
  }
  PPU.vram.address = 0x22;
  PPU.vram.address = 0xed;
  for (i = 0; i < sizeof(wifi_text)-1; ++i) {
    PPU.vram.data = wifi_text[i];
  }


  // Update the attributes too
  // SUPER CLOUD GAMIN BROS LOGO
  PPU.vram.address = 0x23;
  PPU.vram.address = 0xc8;
  for (i = 0; i < 24; ++i) {
    PPU.vram.data = 0x55; // Use pallete 1
  }
  // Text on the next rows
  PPU.vram.address = 0x23;
  PPU.vram.address = 0xe0;
  for (i = 0; i < 8; ++i) {
    PPU.vram.data = 0xaa; // Use pallete 2
  }
  // Text on the the last rows
  PPU.vram.address = 0x23;
  PPU.vram.address = 0xea;
  // Mushroom Selector
  PPU.vram.data = 0x55; // Use pallete 1
  for (i = 0; i < 3; ++i) {
    PPU.vram.data = 0xaa; // Use pallete 2
  }

  // Draw the initial server and 

  // DrawTitleScreen();
  // flush_vram_buffer();

  // Clear buffers
  // i = 0;
  // do {
  //   VRAM_Buffer1[i] = 0;
  //   VRAM_Buffer1[i + 256] = 0;
  //   ++i;
  // }
  // while (i != 0);

  // Populate the seed with some random value
  EnableWifi = 0;
  ServerIndex = 0;

  init_ping_values();


  ClearBuffersDrawIcon();
  flush_vram_buffer();
  // WriteTopScore();
  // flush_vram_buffer();


  ++OperMode_Task;
  // Re-enable drawing
  // Mirror_PPUMASK |= (~0b11100111);
  DisableScreenFlag = 0;
  Mirror_PPUCTRL |= 0x80;
  PPU.control = Mirror_PPUCTRL;
  ENABLE_NMI();

// setup_palettes:
  // temp1 = BackgroundColorCtrl;
  // temp2 = PlayerStatus;
  // BackgroundColorCtrl = 0x00;
  // PlayerStatus = 0x00;

  // ++ScreenRoutineTask;
  return;
  

  #undef w
  #undef i
  #undef tmp
}

const unsigned char title_nmt[32*12]={
	0x24,0x24,0x24,0x24,0x24,0xc0,0xc4,0xc4,0xc4,0xc4,0xc4,0xc4,0xc4,0xc4,0xc4,0xc4,0xc4,0xc4,0xc4,0xc4,0xc4,0xc4,0xc4,0xc4,0xc4,0xc4,0xc1,0x24,0x24,0x24,0x24,0x24,
	0x24,0x24,0x24,0x24,0x24,0xc6,0xd0,0xd1,0xe7,0xe7,0xde,0xd1,0xd0,0xda,0xde,0xd1,0xd0,0xd1,0xd8,0x26,0xd0,0xd1,0xe7,0xe7,0xde,0xd1,0xc7,0x24,0x24,0x24,0x24,0x24,
	0x24,0x24,0x24,0x24,0x24,0xc6,0xd2,0xd3,0xdb,0xdb,0xdb,0xd9,0xdb,0xdc,0xdb,0xdf,0xdb,0xdd,0xdb,0x26,0xdb,0xdb,0xdb,0xdb,0xdb,0xc8,0xc7,0x24,0x24,0x24,0x24,0x24,
	0x24,0x24,0x24,0x24,0x24,0xc6,0xd4,0xd5,0xd4,0xd9,0xdb,0xe2,0xd4,0xda,0xdb,0xe0,0xd4,0xd5,0xde,0xda,0xd4,0xd9,0xd4,0xd9,0xde,0xd9,0xc7,0x24,0x24,0x24,0x24,0x24,
	0x24,0x24,0x24,0x24,0x24,0xc6,0xd6,0xd7,0xd6,0xd7,0xe1,0x26,0xd6,0xdd,0xe1,0xe1,0xd6,0xd7,0xe1,0xdd,0xd6,0xd7,0xd6,0xd7,0xe1,0xd7,0xc7,0x24,0x24,0x24,0x24,0x24,
	0x24,0x24,0x24,0x24,0x24,0xc6,0xd0,0xd1,0xd0,0xd1,0xd0,0xe8,0xd1,0xd8,0xd0,0xd1,0x26,0xde,0xd1,0xde,0xd1,0xd0,0xd1,0xd0,0xd1,0x26,0xc7,0x24,0x24,0x24,0x24,0x24,
	0x24,0x24,0x24,0x24,0x24,0xc6,0xdb,0xdd,0xdb,0xe3,0xdb,0xc8,0xdb,0xdb,0xdb,0xc8,0x26,0xdb,0xe3,0xdb,0xe0,0xdb,0xdb,0xdb,0xc8,0x26,0xc7,0x24,0x24,0x24,0x24,0x24,
	0x24,0x24,0x24,0x24,0x24,0xc6,0xdb,0xec,0xdb,0xdb,0xdb,0xdb,0xdb,0xdb,0xdb,0xdb,0x26,0xdb,0xdf,0xdb,0xdf,0xdb,0xdb,0xe4,0xe5,0x26,0xc7,0x24,0x24,0x24,0x24,0x24,
	0x24,0x24,0x24,0x24,0x24,0xc6,0xdb,0xed,0xde,0xeb,0xdb,0xdb,0xdb,0xdb,0xdb,0xdb,0x26,0xdb,0xe3,0xdb,0xe0,0xdb,0xdb,0xe6,0xe3,0x26,0xc7,0x24,0x24,0x24,0x24,0x24,
	0x24,0x24,0x24,0x24,0x24,0xc6,0xd4,0xd9,0xdb,0xdb,0xdb,0xdb,0xdb,0xdb,0xdb,0xdb,0x26,0xdb,0xd9,0xdb,0xdb,0xd4,0xd9,0xd4,0xd9,0xe7,0xc7,0x24,0x24,0x24,0x24,0x24,
	0x24,0x24,0x24,0x24,0x24,0xc2,0xe9,0xea,0xc9,0xc9,0xc9,0xc9,0xc9,0xc9,0xc9,0xc9,0xc5,0xe9,0xea,0xc9,0xc9,0xe9,0xea,0xe9,0xea,0xc9,0xc3,0x24,0x24,0x24,0x24,0x24,
	0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x2f,0x02,0x00,0x03,0x05,0x24,0x17,0x12,0x17,0x1d,0x0e,0x17,0x0d,0x18,0x24,0x24,0x24,0x24,0x24
};
