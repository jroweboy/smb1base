
.pushseg

.zeropage

R0: .res 1
R1: .res 1
R2: .res 1
R3: .res 1
R4: .res 1
R5: .res 1
R6: .res 1
R7: .res 1

NmiR0: .res 1
NmiR1: .res 1

M0: .res 1
M1: .res 1
M2: .res 1
M3: .res 1

ObjectOffset: .res 1
FrameCounter: .res 1

SavedJoypad1Bits: .res 1
SavedJoypad2Bits: .res 1
SavedJoypadBits := SavedJoypad1Bits


GameEngineSubroutine: .res 1
Enemy_Flag: .res 7
Enemy_ID: .res 7

Player_State: .res 1
Enemy_State: .res 6
Fireball_State: .res 2
Block_State: .res 4
Misc_State: .res 9

PlayerFacingDir: .res 1
Enemy_MovingDir: .res 25
PowerUpType: .res 1
FireballBouncingFlag: .res 2
HammerBroJumpTimer: .res 9
Player_MovingDir: .res 1

Player_X_Speed: .res 1
SprObject_X_Speed := Player_X_Speed
Enemy_X_Speed: .res 6
LakituMoveSpeed := Enemy_X_Speed
PiranhaPlant_Y_Speed := Enemy_X_Speed
ExplosionGfxCounter := Enemy_X_Speed
Jumpspring_FixedYPos := Enemy_X_Speed
RedPTroopaCenterYPos := Enemy_X_Speed
BlooperMoveSpeed := Enemy_X_Speed
XMoveSecondaryCounter := Enemy_X_Speed
CheepCheepMoveMFlag := Enemy_X_Speed
FirebarSpinState_Low := Enemy_X_Speed
YPlatformCenterYPos := Enemy_X_Speed
Fireball_X_Speed: .res 2
Block_X_Speed: .res 4
Misc_X_Speed: .res 9

Player_PageLoc: .res 1
SprObject_PageLoc := Player_PageLoc

Enemy_PageLoc: .res 6
Fireball_PageLoc: .res 2
Block_PageLoc: .res 4
Misc_PageLoc: .res 9
Bubble_PageLoc: .res 3

Player_X_Position: .res 1
SprObject_X_Position := Player_X_Position

Enemy_X_Position: .res 6
Fireball_X_Position: .res 2
Block_X_Position: .res 4
Misc_X_Position: .res 9
Bubble_X_Position: .res 3

Player_Y_Speed: .res 1
SprObject_Y_Speed := Player_Y_Speed

Enemy_Y_Speed: .res 6
FirebarSpinState_High := Enemy_Y_Speed
XMovePrimaryCounter := Enemy_Y_Speed
BlooperMoveCounter := Enemy_Y_Speed
LakituMoveDirection := Enemy_Y_Speed
ExplosionTimerCounter := Enemy_Y_Speed
PiranhaPlant_MoveFlag := Enemy_Y_Speed

Fireball_Y_Speed: .res 2
Block_Y_Speed: .res 4
Misc_Y_Speed: .res 9

Player_Y_HighPos: .res 1
SprObject_Y_HighPos := Player_Y_HighPos

Enemy_Y_HighPos: .res 6
Fireball_Y_HighPos: .res 2
Block_Y_HighPos: .res 4
Misc_Y_HighPos: .res 9
Bubble_Y_HighPos: .res 3

Player_Y_Position: .res 1
SprObject_Y_Position := Player_Y_Position

Enemy_Y_Position: .res 6
Fireball_Y_Position: .res 2
Block_Y_Position: .res 4
Misc_Y_Position: .res 9
Bubble_Y_Position: .res 3

AreaData: .res 2
AreaDataLow := AreaData
AreaDataHigh := AreaData+1

EnemyData: .res 2
EnemyDataLow := EnemyData
EnemyDataHigh := EnemyData + 1


SpriteLocalTemp: .res 4
Local_eb := SpriteLocalTemp + 0
Local_ec := SpriteLocalTemp + 1
Local_ed := SpriteLocalTemp + 2
; RESERVEZP Local_ee ; unused
Local_ef := SpriteLocalTemp + 3

MusicData: .res 2
MusicDataLow := MusicData
MusicDataHigh := MusicData+1

; .segment "STACK"

.segment "SHORTRAM"

NoteLenLookupTblOfs: .res 1
Square1SoundBuffer: .res 1
Square2SoundBuffer: .res 1
NoiseSoundBuffer: .res 1
AreaMusicBuffer: .res 1

MusicOffset_Square2: .res 1
MusicOffset_Square1: .res 1
MusicOffset_Triangle: .res 1

PauseSoundQueue: .res 1
AreaMusicQueue: .res 1
EventMusicQueue: .res 1
NoiseSoundQueue: .res 1
Square2SoundQueue: .res 1
Square1SoundQueue: .res 1

FlagpoleFNum_Y_Pos: .res 1
FlagpoleFNum_YMFDummy: .res 1
FlagpoleScore: .res 1

FloateyNum_Control: .res 7
FloateyNum_X_Pos: .res 7
FloateyNum_Y_Pos: .res 7

ShellChainCounter: .res 7
FloateyNum_Timer: .res 8
DigitModifier: .res 6


StackClear = DigitModifier+5


IrqNewScroll: .res 1
IrqPPUCTRL: .res 1

NmiDisable: .res 1
NmiSkipped: .res 1
ShouldSkipDrawSprites: .res 1

IrqNextScanline: .res 1
CurrentA: .res 1
NextBank: .res 1
SwitchToMainIRQ: .res 1
IrqPointerJmp: .res 3
IrqPointer := IrqPointerJmp + 1



; CurrentBank: .res 1
; BankShadow: .res 1
; TargetAddrJmp: .res 1
; TargetAddress: .res 2
; TargetAddrDummy: .res 1

; CurrentBank:                    .res  1
; BankShadow:                     .res  1
; TargetAddrJmp:                  .res  1
; TargetAddress:                  .res  2
; TargetAddrDummy:                .res  1

; segment "BSS"
.segment "OAM"

Sprite_Y_Position: .res 1
Sprite_Tilenumber: .res 1
Sprite_Attributes: .res 1
Sprite_X_Position: .res 1
Sprite_Data := Sprite_Y_Position

.segment "BSS"

Block_Buffer_1: .res 208
Block_Buffer_2: .res 208
BlockBufferColumnPos: .res 1
MetatileBuffer: .res 13

VRAM_Buffer1_Offset: .res 1
VRAM_Buffer1: .res 84
VRAM_Buffer2_Offset: .res 1
VRAM_Buffer2: .res 34
VRAM_Buffer1_PtrOffset = 0
VRAM_Buffer2_PtrOffset = VRAM_Buffer2_Offset - VRAM_Buffer1_Offset

BowserBodyControls: .res 1
BowserFeetCounter: .res 1
BowserMovementSpeed: .res 1
BowserOrigXPos: .res 1
BowserFlameTimerCtrl: .res 1
BowserFront_Offset: .res 1
BridgeCollapseOffset: .res 1
BowserGfxFlag: .res 1

FirebarSpinSpeed: .res 16

; moved to abs ram
A_B_Buttons: .res 1
Up_Down_Buttons: .res 1
Left_Right_Buttons: .res 1
PreviousA_B_Buttons: .res 1

Vine_FlagOffset: .res 1
Vine_Height: .res 1
Vine_ObjOffset: .res 3
Vine_Start_Y_Position: .res 3

BalPlatformAlignment: .res 1
Platform_X_Scroll: .res 1

PlatformCollisionFlag: .res 11
HammerThrowingTimer := PlatformCollisionFlag

Player_Rel_XPos: .res 1
SprObject_Rel_XPos := Player_Rel_XPos
Enemy_Rel_XPos: .res 1
Fireball_Rel_XPos: .res 1
Bubble_Rel_XPos: .res 1
Block_Rel_XPos: .res 2
Misc_Rel_XPos: .res 5

Player_Rel_YPos: .res 1
SprObject_Rel_YPos := Player_Rel_YPos
Enemy_Rel_YPos: .res 1
Fireball_Rel_YPos: .res 1
Bubble_Rel_YPos: .res 1
Block_Rel_YPos: .res 2
Misc_Rel_YPos: .res 5

Player_SprAttrib: .res 1
SprObject_SprAttrib := Player_SprAttrib
Enemy_SprAttrib: .res 6
Fireball_SprAttrib: .res 2
Block_SprAttrib: .res 4
Misc_SprAttrib: .res 9
Bubble_SprAttrib: .res 3

Player_OffscreenBits: .res 1
SprObject_OffscrBits := Player_OffscreenBits
Enemy_OffscreenBits: .res 1
FBall_OffscreenBits: .res 1
Bubble_OffscreenBits: .res 1
Block_OffscreenBits: .res 2
Misc_OffscreenBits: .res 2
EnemyOffscrBitsMasked: .res 12
Block_Orig_YPos: .res 2
Block_BBuf_Low: .res 2
Block_Metatile: .res 2
Block_PageLoc2: .res 2
Block_RepFlag: .res 2
SprDataOffset_Ctrl: .res 2
Block_Orig_XPos: .res 8
AttributeBuffer: .res 7

SprObject_X_MoveForce: .res 1
Enemy_X_MoveForce: .res 21
YPlatformTopYPos := Enemy_X_MoveForce
RedPTroopaOrigXPos := Enemy_X_MoveForce


Player_YMoveForceFractional: .res 1
SprObject_YMoveForceFractional := Player_YMoveForceFractional

Enemy_YMoveForceFractional: .res 21
BowserFlamePRandomOfs := Enemy_YMoveForceFractional
PiranhaPlantUpYPos := Enemy_YMoveForceFractional

Bubble_YMoveForceFractional: .res 7

Player_Y_MoveForce: .res 1
SprObject_Y_MoveForce := Player_Y_MoveForce

Enemy_Y_MoveForce: .res 8
CheepCheepOrigYPos := Enemy_Y_MoveForce
PiranhaPlantDownYPos := Enemy_Y_MoveForce

Block_Y_MoveForce: .res 20
MaximumLeftSpeed: .res 1
MaximumRightSpeed: .res 1

Whirlpool_Offset: .res 1
Cannon_Offset := Whirlpool_Offset

Whirlpool_PageLoc: .res 6
Cannon_PageLoc := Whirlpool_PageLoc

Whirlpool_LeftExtent: .res 6
Cannon_X_Position := Whirlpool_LeftExtent

Whirlpool_Length: .res 6
Cannon_Y_Position := Whirlpool_Length

Whirlpool_Flag: .res 6
Cannon_Timer := Whirlpool_Flag

BowserHitPoints: .res 1
StompChainCounter: .res 1
Player_CollisionBits: .res 1
Enemy_CollisionBits: .res 8

Player_BoundBoxCtrl: .res 1
SprObj_BoundBoxCtrl := Player_BoundBoxCtrl

Enemy_BoundBoxCtrl: .res 6
Fireball_BoundBoxCtrl: .res 2
Misc_BoundBoxCtrl: .res 10

BoundingBox_UL_Corner: .res 1
BoundingBox_UL_XPos := BoundingBox_UL_Corner
BoundingBox_UL_YPos: .res 1
BoundingBox_LR_Corner: .res 1
BoundingBox_DR_XPos := BoundingBox_LR_Corner
BoundingBox_DR_YPos: .res 1
EnemyBoundingBoxCoord: .res 80
HammerEnemyOffset: .res 9
JumpCoinMiscOffset: .res 5
BrickCoinTimerFlag: .res 2
Misc_Collision_Flag: .res 13
EnemyFrenzyBuffer: .res 1
SecondaryHardMode: .res 1
EnemyFrenzyQueue: .res 1
FireballCounter: .res 1
DuplicateObj_Offset: .res 2
LakituReappearTimer: .res 2
NumberofGroupEnemies: .res 1
ColorRotateOffset: .res 1
PlayerGfxOffset: .res 1
WarpZoneControl: .res 1
FireworksCounter: .res 2
MultiLoopCorrectCntr: .res 1
MultiLoopPassCntr: .res 1
JumpspringForce: .res 1
MaxRangeFromOrigin: .res 1
BitMFilter: .res 1
ChangeAreaTimer: .res 2

PlayerOAMOffset: .res 1
CurrentOAMOffset: .res 1
OriginalOAMOffset: .res 1
SpriteShuffleOffset: .res 1

PlayerMetasprite: .res 1
ObjectMetasprite := PlayerMetasprite
EnemyMetasprite: .res 6
FireballMetasprite: .res 2
BlockMetasprite: .res 4
MiscMetasprite: .res 9
BubbleMetasprite: .res 3

PlayerVerticalFlip: .res 1
ObjectVerticalFlip := PlayerVerticalFlip
EnemyVerticalFlip: .res 6

Player_X_Scroll: .res 1
Player_XSpeedAbsolute: .res 1
FrictionAdderHigh: .res 1
FrictionAdderLow: .res 1
RunningSpeed: .res 1
SwimmingFlag: .res 1
Player_X_MoveForce: .res 1
DiffToHaltJump: .res 1
JumpOrigin_Y_HighPos: .res 1
JumpOrigin_Y_Position: .res 1
VerticalForce: .res 1
VerticalForceDown: .res 1
PlayerChangeSizeFlag: .res 1
PlayerAnimTimerSet: .res 1
PlayerAnimCtrl: .res 1
JumpspringAnimCtrl: .res 1
FlagpoleCollisionYPos: .res 1
PlayerEntranceCtrl: .res 1
FireballThrowingTimer: .res 1
DeathMusicLoaded: .res 1
FlagpoleSoundQueue: .res 1
CrouchingFlag: .res 1
GameTimerSetting: .res 1
DisableCollisionDet: .res 1
DemoAction: .res 1
DemoActionTimer: .res 1
PrimaryMsgCounter: .res 1

ScreenEdge_PageLoc := ScreenLeft_PageLoc
ScreenLeft_PageLoc: .res 1
ScreenRight_PageLoc: .res 1

ScreenEdge_X_Pos := ScreenLeft_X_Pos
ScreenLeft_X_Pos: .res 1
ScreenRight_X_Pos: .res 1

ColumnSets: .res 1
AreaParserTaskNum: .res 1
CurrentNTAddr_High: .res 1
CurrentNTAddr_Low: .res 1
Sprite0HitDetectFlag: .res 1
ScrollLock: .res 2
CurrentPageLoc: .res 1
CurrentColumnPos: .res 1
TerrainControl: .res 1
BackloadingFlag: .res 1
BehindAreaParserFlag: .res 1
AreaObjectPageLoc: .res 1
AreaObjectPageSel: .res1 
AreaDataOffset: .res 1
AreaObjOffsetBuffer: .res 3
AreaObjectLength: .res 3
AreaStyle: .res 1
StaircaseControl: .res 1
AreaObjectHeight: .res 1
MushroomLedgeHalfLen: .res 3
EnemyDataOffset: .res 1
EnemyObjectPageLoc: .res 1
EnemyObjectPageSel: .res 1
ScreenRoutineTask: .res 1
ScrollThirtyTwo: .res 2
HorizontalScroll: .res 1
VerticalScroll: .res 1
ForegroundScenery: .res 1
BackgroundScenery: .res 1
CloudTypeOverride: .res 1
BackgroundColorCtrl: .res 1
LoopCommand: .res 1
StarFlagTaskControl: .res 1
TimerControl: .res 1
CoinTallyFor1Ups: .res 1
SecondaryMsgCounter: .res 1

FirebarSpinDirection := DestinationPageLoc
DestinationPageLoc: .res 1
VictoryWalkControl: .res 5

; notes:
; AreaType:
; Water = 0
; Ground = 1
; UnderGround = 2
; Castle = 3
AreaType: .res 1

AreaAddrsLOffset: .res 1
AreaPointer: .res 1
EntrancePage: .res 1
AltEntranceControl: .res 1
CurrentPlayer, 1 ; 0 = mario: .res 1 = luigi
PlayerSize, 1 ; 1 = small: .res 0 = big
Player_Pos_ForScroll: .res 1
PlayerStatus, 1 ; 0 = small, 1 = super: .res 2 = firey
FetchNewGameTimerFlag: .res 1
JoypadOverride: .res 1
GameTimerExpiredFlag: .res 1

OnscreenPlayerInfo := NumberofLives
NumberofLives: .res 1
HalfwayPage: .res 1
LevelNumber: .res 1
Hidden1UpFlag: .res 1
CoinTally: .res 1
WorldNumber: .res 1
AreaNumber: .res 1

OffscreenPlayerInfo := OffScr_NumberofLives
OffScr_NumberofLives: .res 1
OffScr_HalfwayPage: .res 1
OffScr_LevelNumber: .res 1
OffScr_Hidden1UpFlag: .res 1
OffScr_CoinTally: .res 1
OffScr_WorldNumber: .res 1
OffScr_AreaNumber: .res 1


ScrollFractional: .res 1
DisableIntermediate: .res 1
PrimaryHardMode: .res 1
WorldSelectNumber: .res 1

; $0770: .proc InitializeGame leaves ram below here alone ( y = $6f )

OperMode: .res 2
OperMode_Task: .res 1
VRAM_Buffer_AddrCtrl: .res 1
DisableScreenFlag: .res 1
ScrollAmount: .res 1
GamePauseStatus: .res 1
GamePauseTimer: .res 1
Mirror_PPUCTRL: .res 1
Mirror_PPUMASK: .res 1
NumberOfPlayers: .res 1

IntervalTimerControl: .res 1

Timers := SelectTimer
SelectTimer: .res 1
PlayerAnimTimer: .res 1
JumpSwimTimer: .res 1
RunningTimer: .res 1
BlockBounceTimer: .res 1
SideCollisionTimer: .res 1
JumpspringTimer: .res 1
GameTimerCtrlTimer: .res 2
ClimbSideTimer: .res 1
EnemyFrameTimer: .res 5
FrenzyEnemyTimer: .res 1
BowserFireBreathTimer: .res 1
StompTimer: .res 1
AirBubbleTimer: .res 3

FRAME_TIMER_COUNT = AirBubbleTimer - Timers

ScrollIntervalTimer: .res 1
EnemyIntervalTimer: .res 7
BrickCoinTimer: .res 1
InjuryTimer: .res 1
StarInvincibleTimer: .res 1
ScreenTimer: .res 1
WorldEndTimer: .res 1
DemoTimer: .res 1

ALL_TIMER_COUNT = DemoTimer - Timers

PseudoRandomBitReg: .res 9

SoundMemory := MusicOffset_Noise
MusicOffset_Noise: .res 1
EventMusicBuffer: .res 1
PauseSoundBuffer: .res 1
Squ2_NoteLenBuffer: .res 1
Squ2_NoteLenCounter: .res 1
Squ2_EnvelopeDataCtrl: .res 1
Squ1_NoteLenCounter: .res 1
Squ1_EnvelopeDataCtrl: .res 1
Tri_NoteLenBuffer: .res 1
Tri_NoteLenCounter: .res 1
Noise_BeatLenCounter: .res 1
Squ1_SfxLenCounter: .res 2
Squ2_SfxLenCounter: .res 1
Sfx_SecondaryCounter: .res 1
Noise_SfxLenCounter: .res 1
DAC_Counter: .res 1
NoiseDataLoopbackOfs: .res 1
NoteLengthTblAdder: .res 1
AreaMusicBuffer_Alt: .res 1

PauseModeFlag: .res 1
GroundMusicHeaderOfs: .res 1
AltRegContentFlag: .res 1

_WarmBootOffset = DisplayDigits

DisplayDigits := TopScoreDisplay
TopScoreDisplay: .res 6
PlayerScoreDisplay: .res 27
ScoreAndCoinDisplay := PlayerScoreDisplay
GameTimerDisplay: .res 4


WorldSelectEnableFlag: .res 1

ContinueWorld: .res 1

_ColdBootOffset = WarmBootValidation

.ifdef WORLD_HAX
DebugCooldown:                  .res  1
.endif

WarmBootValidation: .res 1

.popseg
