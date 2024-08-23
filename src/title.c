
#include "inc/mario.h"


#define pad_pressed M0
#define i M1

WRAPPED(extern void GameCoreRoutine());
WRAPPED(extern void LoadAreaPointer());
WRAPPED(extern void DrawMushroomIcon());
WRAPPED(extern void DemoEngine());
extern u8 WSelectBufferTemplate[6];


const unsigned char title_nmt[32*12];
static const char connect_text[] = "CONNECT TO SERVER REGION";
static const char region_text_list[][17] = {
  "<  NORTH AMER. >",
  "<  EAST EUROPE >",
  "<  LATIN AMER. >",
  "< S. EAST ASIA >",
  "<   ANTARTICA  >",
};
static const char ethernet_text[] = "ETHERNET";
static const char wifi_text[] = "WIFI";

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
      for (i = 0; i < 6; i++) {
        VRAM_Buffer1[i-1] = WSelectBufferTemplate[i];
        VRAM_Buffer1[3] = WorldNumber+1;
      }
    } else {
      // Select button must have been presesed, so redraw the MushroomIcon
      // And switch between 1 and 2 player modes
      NumberOfPlayers ^= 1;
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
  DISABLE_NMI();

  // Disable rendering
  DisableScreenFlag = 1;
  Mirror_PPUMASK &= (0b11100111);
  PPU.mask = Mirror_PPUMASK;
  

  // clear all the palettes
  VRAM_Buffer_AddrCtrl = 0; // Use the VRAM_Buffer1
  VRAM_Buffer1[0] = 0x3f;
  VRAM_Buffer1[1] = 0x00;
  VRAM_Buffer1[2] = 0x20 | 0b01000000; // repeat one byte 0x20 times
  VRAM_Buffer1[3] = 0x00;
  VRAM_Buffer1[4] = 0x00;
  VRAM_Buffer1_Offset = 5;
  for (i = 0; i < 0x20; i++) {
    PPU.vram.data = 0x0f;
  }
  
  ENABLE_NMI();
  // Wait for the VRAM buffer to be flushed during NMI to prevent graphic glitches
  while (NmiDisable == 0);
  DISABLE_NMI();

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

  ClearBuffersDrawIcon();
  flush_vram_buffer();
  // WriteTopScore();
  // flush_vram_buffer();

  ++OperMode_Task;
  // Re-enable drawing
  // Mirror_PPUMASK |= (~0b11100111);
  DisableScreenFlag = 0;
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
