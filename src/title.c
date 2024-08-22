
#include "inc/mario.h"


#define pad_pressed M0
#define i M1

WRAPPED(extern void GameCoreRoutine());
WRAPPED(extern void LoadAreaPointer());
WRAPPED(extern void DrawMushroomIcon());
WRAPPED(extern void DemoEngine());
extern u8 WSelectBufferTemplate[6];


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
    for (i = 0; i < 
        (sizeof(Player1ScoreDisplay)
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
  DrawTitleScreen();
  flush_vram_buffer();
  ClearBuffersDrawIcon();
  flush_vram_buffer();
  WriteTopScore();
  flush_vram_buffer();

  // ++OperMode_Task;
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
