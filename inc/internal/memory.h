
#ifndef __MEMORY_H
#define __MEMORY_H

#include "common_types.h"

// A non exhaustive list of RAM values available for use.

// NOTE: These may not be kept up to date as frequently, so if something goes wrong
// check that its defined in asm properly



// Values defined in the CC65 runtime
// NOTE: These can be overwritten by the runtime if you call some function
// and be warned there's lots of "functions" that you don't think of as functions usually

extern u8 tmp1;
extern u8 tmp2;
extern u8 tmp3;
extern u8 tmp4;
extern u8* ptr1;
extern u8* ptr2;
extern u8* ptr3;
extern u8* ptr4;
extern u8 regsave;
extern u8 sreg;


// Temporary values should not be expected to last from frame to frame.
// They will likely be changed by other code after the function you are writing ends

// Temporary values that are used by the vanilla code
extern u8 R0;
extern u8 R1;
extern u8 R2;
extern u8 R3;
extern u8 R4;
extern u8 R5;
extern u8 R6;
extern u8 R7;
ZP(R0, R1, R2, R3, R4, R5, R6, R7);

// Don't use! Temporary values used in NMI in vanilla
extern u8 NmiR0;
extern u8 NmiR1;
ZP(NmiR0, NmiR1);

// New temporaries that can be reused wherever.
extern u8 M0;
extern u8 M1;
ZP(M0, M1);

// When looping through and processing objects, this stores the current object ID
extern u8 ObjectOffset;
// Counts up by one every frame. After reaching 255 this loops back to zero
extern u8 FrameCounter;
ZP(ObjectOffset, FrameCounter);

// Current input for player 1 and 2. Prefer to use the A_B_Buttons / Up_Down_Buttons if you need the current player input
extern u8 SavedJoypad1Bits;
extern u8 SavedJoypad2Bits;
#define SavedJoypadBits ((u8[])(&SavedJoypad1Bits))
ZP(SavedJoypad1Bits, SavedJoypad2Bits);

// Current value for what main code is running
extern u8 GameEngineSubroutine;
// Status flag for all enemies
extern u8 Enemy_Flag[7];
// Internal ID for the enemies (based on the hex value you'll find in places like smbutil)
extern u8 Enemy_ID[7];
ZP(GameEngineSubroutine, Enemy_Flag, Enemy_ID);

// Current state for all objects. Each value is defined by what each enemy type is, but there's a few that are common between most
extern u8 Player_State;
extern u8 Enemy_State[7];
extern u8 Fireball_State[2];
// Bump blocks and destroyed blocks go here
extern u8 Block_State[4];
// Things like hammers go here
extern u8 Misc_State[9];
ZP(Player_State, Enemy_State, Fireball_State, Block_State, Misc_State);

// 0 = mushroom, 1 = fireflower, 2 = star, 3 = 1-up
extern u8 PowerUpType[6];
extern u8 FireballBouncingFlag[2];
extern u8 Player_MovingDir;
ZP(PowerUpType, FireballBouncingFlag, Player_MovingDir);

extern u8 Player_X_Speed;
extern u8 Enemy_X_Speed[6];
extern u8 Fireball_X_Speed[2];
extern u8 Block_X_Speed[4];
extern u8 Misc_X_Speed[9];
ZP(Player_X_Speed, Enemy_X_Speed, Fireball_X_Speed, Block_X_Speed, Misc_X_Speed);

extern u8 Player_PageLoc;
extern u8 Enemy_PageLoc[6];
extern u8 Fireball_PageLoc[2];
extern u8 Block_PageLoc[4];
extern u8 Misc_PageLoc[9];
extern u8 Bubble_PageLoc[3];
ZP(Player_PageLoc, Enemy_PageLoc, Fireball_PageLoc, Block_PageLoc, Misc_PageLoc, Bubble_PageLoc);

extern u8 Player_X_Position;
extern u8 Enemy_X_Position[6];
extern u8 Fireball_X_Position[2];
extern u8 Block_X_Position[4];
extern u8 Misc_X_Position[9];
extern u8 Bubble_X_Position[3];
ZP(Player_X_Position, Enemy_X_Position, Fireball_X_Position, Block_X_Position, Misc_X_Position, Bubble_X_Position);

extern u8 Player_Y_Speed;
extern u8 Enemy_Y_Speed[6];
extern u8 Fireball_Y_Speed[2];
extern u8 Block_Y_Speed[4];
extern u8 Misc_Y_Speed[9];
ZP(Player_Y_Speed, Enemy_Y_Speed, Fireball_Y_Speed, Block_Y_Speed, Misc_Y_Speed);

extern u8 Player_Y_HighPos;
extern u8 Enemy_Y_HighPos[6];
extern u8 Fireball_Y_HighPos[2];
extern u8 Block_Y_HighPos[4];
extern u8 Misc_Y_HighPos[9];
extern u8 Bubble_Y_HighPos[3];
ZP(Player_Y_HighPos, Enemy_Y_HighPos, Fireball_Y_HighPos, Block_Y_HighPos, Misc_Y_HighPos, Bubble_Y_HighPos);

extern u8 Player_Y_Position;
extern u8 Enemy_Y_Position[6];
extern u8 Fireball_Y_Position[2];
extern u8 Block_Y_Position[4];
extern u8 Misc_Y_Position[9];
extern u8 Bubble_Y_Position[3];
ZP(Player_Y_Position, Enemy_Y_Position, Fireball_Y_Position, Block_Y_Position, Misc_Y_Position, Bubble_Y_Position);

// Pointers used in level loading to the current area and enemy data
extern u8* AreaData;
extern u8* EnemyData;

// Temp values used when handling sprites
extern u8 SpriteLocalTemp[4];
#define Local_eb (SpriteLocalTemp[0])
#define Local_ec (SpriteLocalTemp[1])
#define Local_ed (SpriteLocalTemp[2])
#define Local_ef (SpriteLocalTemp[3])

extern u8* MusicData;

// ; .segment "STACK"

// .segment "SHORTRAM"

extern u8 NoteLenLookupTblOfs;
extern u8 Square1SoundBuffer;
extern u8 Square2SoundBuffer;
extern u8 NoiseSoundBuffer;
extern u8 AreaMusicBuffer;

extern u8 MusicOffset_Square2;
extern u8 MusicOffset_Square1;
extern u8 MusicOffset_Triangle;

extern u8 PauseSoundQueue;
extern u8 AreaMusicQueue;
extern u8 EventMusicQueue;
extern u8 NoiseSoundQueue;
extern u8 Square2SoundQueue;
extern u8 Square1SoundQueue;

extern u8 FlagpoleFNum_Y_Pos;
extern u8 FlagpoleFNum_YMFDummy;
extern u8 FlagpoleScore;

extern u8 FloateyNum_Control[7];
extern u8 FloateyNum_X_Pos[7];
extern u8 FloateyNum_Y_Pos[7];

extern u8 ShellChainCounter[7];
extern u8 FloateyNum_Timer[8];
extern u8 DigitModifier[6];


extern u8 IrqNewScroll;
extern u8 IrqPPUCTRL;

extern u8 volatile NmiDisable;
extern u8 volatile NmiSkipped;
extern u8 ShouldSkipDrawSprites;

extern u8 IrqNextScanline;
extern u8 CurrentA;
extern u8 NextBank;
extern u8 SwitchToMainIRQ;
extern u8 IrqPointerJmp[3];


// ; segment "BSS"
// .segment "OAM"

extern u8 Sprite_Data[256];
// extern u8 Sprite_Y_Position;
// extern u8 Sprite_Tilenumber;
// extern u8 Sprite_Attributes;
// extern u8 Sprite_X_Position;
// Sprite_Data := Sprite_Y_Position

// .segment "BSS"

extern u8 Block_Buffer_1[208];
extern u8 Block_Buffer_2[208];
extern u8 BlockBufferColumnPos;
extern u8 MetatileBuffer[13];

extern u8 VRAM_Buffer1_Offset;
extern u8 VRAM_Buffer1[84];
extern u8 VRAM_Buffer2_Offset;
extern u8 VRAM_Buffer2[34];

extern u8 BowserBodyControls;
extern u8 BowserFeetCounter;
extern u8 BowserMovementSpeed;
extern u8 BowserOrigXPos;
extern u8 BowserFlameTimerCtrl;
extern u8 BowserFront_Offset;
extern u8 BridgeCollapseOffset;
extern u8 BowserGfxFlag;

extern u8 FirebarSpinSpeed[16];

// moved to abs ram
extern u8 HammerBroJumpTimer[9];

// Which direction the objects are facing. This is expanded from vanilla to fit all entities for metasprite rendering
extern u8 PlayerFacingDir;
extern u8 Enemy_MovingDir[25];

// ; moved to abs ram
extern u8 A_B_Buttons;
extern u8 Up_Down_Buttons;
extern u8 Left_Right_Buttons;
extern u8 PreviousA_B_Buttons;

extern u8 Vine_FlagOffset;
extern u8 Vine_Height;
extern u8 Vine_ObjOffset[3];
extern u8 Vine_Start_Y_Position[3];

extern u8 BalPlatformAlignment;
extern u8 Platform_X_Scroll;

extern u8 PlatformCollisionFlag[11];
#define HammerThrowingTimer PlatformCollisionFlag

extern u8 Player_Rel_XPos;
extern u8 Enemy_Rel_XPos;
extern u8 Fireball_Rel_XPos;
extern u8 Bubble_Rel_XPos;
extern u8 Block_Rel_XPos[2];
extern u8 Misc_Rel_XPos[5];

extern u8 Player_Rel_YPos;
extern u8 Enemy_Rel_YPos;
extern u8 Fireball_Rel_YPos;
extern u8 Bubble_Rel_YPos;
extern u8 Block_Rel_YPos[2];
extern u8 Misc_Rel_YPos[5];

extern u8 Player_SprAttrib;
extern u8 Enemy_SprAttrib[6];
extern u8 Fireball_SprAttrib[2];
extern u8 Block_SprAttrib[4];
extern u8 Misc_SprAttrib[9];
extern u8 Bubble_SprAttrib[3];

extern u8 Player_OffscreenBits;
extern u8 Enemy_OffscreenBits;
extern u8 FBall_OffscreenBits;
extern u8 Bubble_OffscreenBits;
extern u8 Block_OffscreenBits[2];
extern u8 Misc_OffscreenBits[2];
extern u8 EnemyOffscrBitsMasked[12];
extern u8 Block_Orig_YPos[2];
extern u8 Block_BBuf_Low[2];
extern u8 Block_Metatile[2];
extern u8 Block_PageLoc2[2];
extern u8 Block_RepFlag[2];
extern u8 SprDataOffset_Ctrl[2];
extern u8 Block_Orig_XPos[8];
extern u8 AttributeBuffer[7];

extern u8 SprObject_X_MoveForce;
extern u8 Enemy_X_MoveForce[21];


extern u8 Player_YMoveForceFractional;

extern u8 Enemy_YMoveForceFractional[21];

extern u8 Bubble_YMoveForceFractional[7];

extern u8 Player_Y_MoveForce;

extern u8 Enemy_Y_MoveForce[8];

extern u8 Block_Y_MoveForce[20];
extern u8 MaximumLeftSpeed;
extern u8 MaximumRightSpeed;

extern u8 Whirlpool_Offset;

extern u8 Whirlpool_PageLoc[6];

extern u8 Whirlpool_LeftExtent[6];

extern u8 Whirlpool_Length[6];

extern u8 Whirlpool_Flag[6];

extern u8 BowserHitPoints;
extern u8 StompChainCounter;
extern u8 Player_CollisionBits;
extern u8 Enemy_CollisionBits[8];

extern u8 Player_BoundBoxCtrl;

extern u8 Enemy_BoundBoxCtrl[6];
extern u8 Fireball_BoundBoxCtrl[2];
extern u8 Misc_BoundBoxCtrl[10];

extern u8 BoundingBox_UL_Corner;
extern u8 BoundingBox_UL_YPos;
extern u8 BoundingBox_LR_Corner;
extern u8 BoundingBox_DR_YPos;
extern u8 EnemyBoundingBoxCoord[80];

extern u8 HammerEnemyOffset[9];
extern u8 JumpCoinMiscOffset[5];
extern u8 BrickCoinTimerFlag[2];
extern u8 Misc_Collision_Flag[13];
extern u8 EnemyFrenzyBuffer;
extern u8 SecondaryHardMode;
extern u8 EnemyFrenzyQueue;
extern u8 FireballCounter;
extern u8 DuplicateObj_Offset[2];
extern u8 LakituReappearTimer[2];
extern u8 NumberofGroupEnemies;
extern u8 ColorRotateOffset;
extern u8 PlayerGfxOffset;
extern u8 WarpZoneControl;
extern u8 FireworksCounter[2];
extern u8 MultiLoopCorrectCntr;
extern u8 MultiLoopPassCntr;
extern u8 JumpspringForce;
extern u8 MaxRangeFromOrigin;
extern u8 BitMFilter;
extern u8 ChangeAreaTimer[2];


// Stores the current 
extern u8 PlayerOAMOffset;
extern u8 CurrentOAMOffset;
extern u8 OriginalOAMOffset;
extern u8 SpriteShuffleOffset;

extern u8 PlayerMetasprite;
extern u8 EnemyMetasprite[6];
extern u8 FireballMetasprite[2];
extern u8 BlockMetasprite[4];
extern u8 MiscMetasprite[9];
extern u8 BubbleMetasprite[3];

extern u8 PlayerVerticalFlip;
extern u8 EnemyVerticalFlip[6];

extern u8 Player_X_Scroll;
extern u8 Player_XSpeedAbsolute;
extern u8 FrictionAdderHigh;
extern u8 FrictionAdderLow;
extern u8 RunningSpeed;
extern u8 SwimmingFlag;
extern u8 Player_X_MoveForce;
extern u8 DiffToHaltJump;
extern u8 JumpOrigin_Y_HighPos;
extern u8 JumpOrigin_Y_Position;
extern u8 VerticalForce;
extern u8 VerticalForceDown;
extern u8 PlayerChangeSizeFlag;
extern u8 PlayerAnimTimerSet;
extern u8 PlayerAnimCtrl;
extern u8 JumpspringAnimCtrl;
extern u8 FlagpoleCollisionYPos;
extern u8 PlayerEntranceCtrl;
extern u8 FireballThrowingTimer;
extern u8 DeathMusicLoaded;
extern u8 FlagpoleSoundQueue;
extern u8 CrouchingFlag;
extern u8 GameTimerSetting;
extern u8 DisableCollisionDet;
extern u8 DemoAction;
extern u8 DemoActionTimer;
extern u8 PrimaryMsgCounter;

extern u8 ScreenLeft_PageLoc;
extern u8 ScreenRight_PageLoc;

extern u8 ScreenLeft_X_Pos;
extern u8 ScreenRight_X_Pos;

extern u8 ColumnSets;
extern u8 AreaParserTaskNum;
extern u8 CurrentNTAddr_High;
extern u8 CurrentNTAddr_Low;
extern u8 Sprite0HitDetectFlag;
extern u8 ScrollLock[2];
extern u8 CurrentPageLoc;
extern u8 CurrentColumnPos;
extern u8 TerrainControl;
extern u8 BackloadingFlag;
extern u8 BehindAreaParserFlag;
extern u8 AreaObjectPageLoc;
extern u8 AreaObjectPageSel;
extern u8 AreaDataOffset;
extern u8 AreaObjOffsetBuffer[3];
extern u8 AreaObjectLength[3];
extern u8 AreaStyle;
extern u8 StaircaseControl;
extern u8 AreaObjectHeight;
extern u8 MushroomLedgeHalfLen[3];
extern u8 EnemyDataOffset;
extern u8 EnemyObjectPageLoc;
extern u8 EnemyObjectPageSel;
extern u8 ScreenRoutineTask;
extern u8 ScrollThirtyTwo[2];
extern u8 HorizontalScroll;
extern u8 VerticalScroll;
extern u8 ForegroundScenery;
extern u8 BackgroundScenery;
extern u8 CloudTypeOverride;
extern u8 BackgroundColorCtrl;
extern u8 LoopCommand;
extern u8 StarFlagTaskControl;
extern u8 TimerControl;
extern u8 CoinTallyFor1Ups;
extern u8 SecondaryMsgCounter;


extern u8 DestinationPageLoc;
extern u8 VictoryWalkControl[5];

// ; notes:
// ; AreaType:
// ; Water = 0
// ; Ground = 1
// ; UnderGround = 2
// ; Castle = 3
extern u8 AreaType;

extern u8 AreaAddrsLOffset;
extern u8 AreaPointer;
extern u8 EntrancePage;
extern u8 AltEntranceControl;
// 0 = mario  1 = luigi
extern u8 CurrentPlayer;
// 1 = small  0 = big
extern u8 PlayerSize;
extern u8 Player_Pos_ForScroll;
// 0 = small, 1 = super, 2 = firey
extern u8 PlayerStatus;
extern u8 FetchNewGameTimerFlag;
extern u8 JoypadOverride;
extern u8 GameTimerExpiredFlag;

extern u8 NumberofLives;
extern u8 HalfwayPage;
extern u8 LevelNumber;
extern u8 Hidden1UpFlag;
extern u8 CoinTally;
extern u8 WorldNumber;
extern u8 AreaNumber;

extern u8 OffScr_NumberofLives;
extern u8 OffScr_HalfwayPage;
extern u8 OffScr_LevelNumber;
extern u8 OffScr_Hidden1UpFlag;
extern u8 OffScr_CoinTally;
extern u8 OffScr_WorldNumber;
extern u8 OffScr_AreaNumber;


extern u8 ScrollFractional;
extern u8 DisableIntermediate;
extern u8 PrimaryHardMode;
extern u8 WorldSelectNumber;

extern u8* OperMode;
extern u8 OperMode_Task;
extern u8 VRAM_Buffer_AddrCtrl;
extern u8 DisableScreenFlag;
extern u8 ScrollAmount;
extern u8 GamePauseStatus;
extern u8 GamePauseTimer;
extern u8 Mirror_PPUCTRL;
extern u8 Mirror_PPUMASK;
extern u8 NumberOfPlayers;

extern u8 IntervalTimerControl;

extern u8 SelectTimer;
extern u8 PlayerAnimTimer;
extern u8 JumpSwimTimer;
extern u8 RunningTimer;
extern u8 BlockBounceTimer;
extern u8 SideCollisionTimer;
extern u8 JumpspringTimer;
extern u8 GameTimerCtrlTimer[2];
extern u8 ClimbSideTimer;
extern u8 EnemyFrameTimer[5];
extern u8 FrenzyEnemyTimer;
extern u8 BowserFireBreathTimer;
extern u8 StompTimer;
extern u8 AirBubbleTimer[3];

extern u8 ScrollIntervalTimer;
extern u8 EnemyIntervalTimer[7];
extern u8 BrickCoinTimer;
extern u8 InjuryTimer;
extern u8 StarInvincibleTimer;
extern u8 ScreenTimer;
extern u8 WorldEndTimer;
extern u8 DemoTimer;

extern u8 PseudoRandomBitReg[9];

extern u8 MusicOffset_Noise;
extern u8 EventMusicBuffer;
extern u8 PauseSoundBuffer;
extern u8 Squ2_NoteLenBuffer;
extern u8 Squ2_NoteLenCounter;
extern u8 Squ2_EnvelopeDataCtrl;
extern u8 Squ1_NoteLenCounter;
extern u8 Squ1_EnvelopeDataCtrl;
extern u8 Tri_NoteLenBuffer;
extern u8 Tri_NoteLenCounter;
extern u8 Noise_BeatLenCounter;
extern u8 Squ1_SfxLenCounter[2];
extern u8 Squ2_SfxLenCounter;
extern u8 Sfx_SecondaryCounter;
extern u8 Noise_SfxLenCounter;
extern u8 DAC_Counter;
extern u8 NoiseDataLoopbackOfs;
extern u8 NoteLengthTblAdder;
extern u8 AreaMusicBuffer_Alt;

extern u8 PauseModeFlag;
extern u8 GroundMusicHeaderOfs;
extern u8 AltRegContentFlag;


extern u8 TopScoreDisplay[6];
extern u8 Player1ScoreDisplay[6];
extern u8 Player2ScoreDisplay[6];
extern u8 Player1CoinDisplay[2];
extern u8 Player2CoinDisplay[2];
extern u8 GameTimerDisplay[3];


extern u8 WorldSelectEnableFlag;

extern u8 ContinueWorld;


#if DEBUG_WORLD_SELECT
extern u8 DebugCooldown;
#endif

extern u8 WarmBootValidation;



#endif // __MEMORY_H