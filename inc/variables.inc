
.ifndef __VARIABLES_INC
.define __VARIABLES_INC 1

; GAME SPECIFIC DEFINES

.segment "ZP"

ENEMY_SLOTS     = 6
FIREBALL_SLOTS  = 2
BLOCK_SLOTS     = 4
MISC_SLOTS      = 9

; Temporary value registers
R0:                    .res 1
R1:                    .res 1
R2:                    .res 1
R3:                    .res 1
R4:                    .res 1
R5:                    .res 1
R6:                    .res 1
R7:                    .res 1

ObjectOffset:          .res 1 ; = $08

FrameCounter:          .res 1 ; = $09

A_B_Buttons:           .res 1 ; = $0a
PreviousA_B_Buttons:   .res 1 ; = $0d
Up_Down_Buttons:       .res 1 ; = $0b
Left_Right_Buttons:    .res 1 ; = $0c

GameEngineSubroutine:  .res 1 ; = $0e
Enemy_Flag:            .res 1 ; = $0f

Enemy_ID:              .res 1 ; = $16

Player_State:          .res 1 ; = $1d
Enemy_State:           .res 1 ; = $1e
Fireball_State:        .res 1 ; = $24
Block_State:           .res 1 ; = $26
Misc_State:            .res 1 ; = $2a

PlayerFacingDir:       .res 1 ; = $33
DestinationPageLoc:    .res 1 ; = $34
FirebarSpinDirection  = DestinationPageLoc ; :  .res 1 ; = $34
VictoryWalkControl:    .res 1 ; = $35

FireballBouncingFlag:  .res 1 ; = $3a
PowerUpType:           .res 1 ; = $39
HammerBroJumpTimer:    .res 1 ; = $3c
Player_MovingDir:      .res 1 ; = $45
Enemy_MovingDir:       .res 1 ; = $46

SprObject_X_Speed:     .res 1 ; = $57
Player_X_Speed = SprObject_X_Speed ; .res 1 ; = $57
Enemy_X_Speed:         .res 1 ; = $58
RedPTroopaCenterYPos  = Enemy_X_Speed ; :  .res 1 ; = $58
XMoveSecondaryCounter = Enemy_X_Speed ; : .res 1 ; = $58
CheepCheepMoveMFlag   = Enemy_X_Speed ; :   .res 1 ; = $58
LakituMoveSpeed       = Enemy_X_Speed ; :       .res 1 ; = $58
BlooperMoveSpeed      = Enemy_X_Speed ; :      .res 1 ; = $58
PiranhaPlant_Y_Speed  = Enemy_X_Speed ; :  .res 1 ; = $58
FirebarSpinState_Low  = Enemy_X_Speed ; :  .res 1 ; = $58
ExplosionGfxCounter   = Enemy_X_Speed ; :   .res 1 ; = $58
Fireball_X_Speed:      .res 1 ; = $5e
Block_X_Speed:         .res 1 ; = $60
Misc_X_Speed:          .res 1 ; = $64

Jumpspring_FixedYPos = Enemy_X_Speed ; :  .res 1 ; = $58
YPlatformCenterYPos  = Enemy_X_Speed ; :   .res 1 ; = $58

SprObject_PageLoc:     .res 1 ; = $6d
Player_PageLoc = SprObject_PageLoc ; :        .res 1 ; = $6d
Enemy_PageLoc:         .res 1 ; = $6e
Fireball_PageLoc:      .res 1 ; = $74
Block_PageLoc:         .res 1 ; = $76
Misc_PageLoc:          .res 1 ; = $7a
Bubble_PageLoc:        .res 1 ; = $83

SprObject_X_Position:  .res 1 ; = $86
Player_X_Position:     .res 1 ; = $86
Enemy_X_Position:      .res 1 ; = $87
Fireball_X_Position:   .res 1 ; = $8d
Block_X_Position:      .res 1 ; = $8f
Misc_X_Position:       .res 1 ; = $93
Bubble_X_Position:     .res 1 ; = $9c

SprObject_Y_Speed:     .res 1 ; = $9f
Player_Y_Speed = SprObject_Y_Speed ; :        .res 1 ; = $9f
Enemy_Y_Speed:         .res 1 ; = $a0
Fireball_Y_Speed:      .res 1 ; = $a6
Block_Y_Speed:         .res 1 ; = $a8
Misc_Y_Speed:          .res 1 ; = $ac

SprObject_Y_HighPos:   .res 1 ; = $b5
Player_Y_HighPos = SprObject_Y_HighPos ; :      .res 1 ; = $b5
Enemy_Y_HighPos:       .res 1 ; = $b6
Fireball_Y_HighPos:    .res 1 ; = $bc
Block_Y_HighPos:       .res 1 ; = $be
Misc_Y_HighPos:        .res 1 ; = $c2
Bubble_Y_HighPos:      .res 1 ; = $cb

SprObject_Y_Position:  .res 1 ; = $ce
Player_Y_Position = SprObject_Y_Position ; :     .res 1 ; = $ce
Enemy_Y_Position:      .res 1 ; = $cf
Fireball_Y_Position:   .res 1 ; = $d5
Block_Y_Position:      .res 1 ; = $d7
Misc_Y_Position:       .res 1 ; = $db
Bubble_Y_Position:     .res 1 ; = $e4

AreaData:              .res 1 ; = $e7
AreaDataLow:           .res 1 ; = $e7
AreaDataHigh:          .res 1 ; = $e8
EnemyData:             .res 1 ; = $e9
EnemyDataLow:          .res 1 ; = $e9
EnemyDataHigh:         .res 1 ; = $ea

PauseSoundQueue:       .res 1 ; = $fa
Square1SoundQueue:     .res 1 ; = $ff
Square2SoundQueue:     .res 1 ; = $fe
NoiseSoundQueue:       .res 1 ; = $fd
AreaMusicQueue:        .res 1 ; = $fb
EventMusicQueue:       .res 1 ; = $fc

Square1SoundBuffer:    .res 1 ; = $f1
Square2SoundBuffer:    .res 1 ; = $f2
NoiseSoundBuffer:      .res 1 ; = $f3
AreaMusicBuffer:       .res 1 ; = $f4

MusicData:             .res 1 ; = $f5
MusicDataLow:          .res 1 ; = $f5
MusicDataHigh:         .res 1 ; = $f6
MusicOffset_Square2:   .res 1 ; = $f7
MusicOffset_Square1:   .res 1 ; = $f8
MusicOffset_Triangle:  .res 1 ; = $f9
NoteLenLookupTblOfs:   .res 1 ; = $f0

LakituMoveDirection:   .res 1 ; = $a0
XMovePrimaryCounter:   .res 1 ; = $a0
BlooperMoveCounter:    .res 1 ; = $a0
FirebarSpinState_High: .res 1 ; = $a0
PiranhaPlant_MoveFlag: .res 1 ; = $a0
ExplosionTimerCounter: .res 1 ; = $a0



.segment "STACK"

VerticalFlipFlag:      .res 1 ; = $0109
FloateyNum_Control:    .res 1 ; = $0110
ShellChainCounter:     .res 1 ; = $0125
FloateyNum_Timer:      .res 1 ; = $012c
FloateyNum_X_Pos:      .res 1 ; = $0117
FloateyNum_Y_Pos:      .res 1 ; = $011e
FlagpoleFNum_Y_Pos:    .res 1 ; = $010d
FlagpoleFNum_YMFDummy: .res 1 ; = $010e
FlagpoleScore:         .res 1 ; = $010f

; Sprite_Data:           .res 1 ; = $0200
.segment "OAM"
Sprite_Y_Position:     .res 1 ; = $0200
Sprite_Tilenumber:     .res 1 ; = $0201
Sprite_Attributes:     .res 1 ; = $0202
Sprite_X_Position:     .res 1 ; = $0203
Sprite_Data = Sprite_Y_Position

.segment "BSS"

SavedJoypadBits:       .res 1 ; = $06fc
SavedJoypad1Bits:      .res 1 ; = $06fc
SavedJoypad2Bits:      .res 1 ; = $06fd
JoypadBitMask:         .res 1 ; = $074a
JoypadOverride:        .res 1 ; = $0758

Mirror_PPU_CTRL:  .res 1 ; = $0778
Mirror_PPU_MASK:  .res 1 ; = $0779

OperMode:              .res 1 ; = $0770
OperMode_Task:         .res 1 ; = $0772
ScreenRoutineTask:     .res 1 ; = $073c

GamePauseStatus:       .res 1 ; = $0776
GamePauseTimer:        .res 1 ; = $0777

DemoAction:            .res 1 ; = $0717
DemoActionTimer:       .res 1 ; = $0718

TimerControl:          .res 1 ; = $0747
IntervalTimerControl:  .res 1 ; = $077f

Timers:                .res 1 ; = $0780
SelectTimer:           .res 1 ; = $0780
PlayerAnimTimer:       .res 1 ; = $0781
JumpSwimTimer:         .res 1 ; = $0782
RunningTimer:          .res 1 ; = $0783
BlockBounceTimer:      .res 1 ; = $0784
SideCollisionTimer:    .res 1 ; = $0785
JumpspringTimer:       .res 1 ; = $0786
GameTimerCtrlTimer:    .res 1 ; = $0787
ClimbSideTimer:        .res 1 ; = $0789
EnemyFrameTimer:       .res 1 ; = $078a
FrenzyEnemyTimer:      .res 1 ; = $078f
BowserFireBreathTimer: .res 1 ; = $0790
StompTimer:            .res 1 ; = $0791
AirBubbleTimer:        .res 1 ; = $0792
ScrollIntervalTimer:   .res 1 ; = $0795
EnemyIntervalTimer:    .res 1 ; = $0796
BrickCoinTimer:        .res 1 ; = $079d
InjuryTimer:           .res 1 ; = $079e
StarInvincibleTimer:   .res 1 ; = $079f
ScreenTimer:           .res 1 ; = $07a0
WorldEndTimer:         .res 1 ; = $07a1
DemoTimer:             .res 1 ; = $07a2

ScreenEdge_PageLoc:    .res 1 ; = $071a
ScreenEdge_X_Pos:      .res 1 ; = $071c
ScreenLeft_PageLoc:    .res 1 ; = $071a
ScreenRight_PageLoc:   .res 1 ; = $071b
ScreenLeft_X_Pos:      .res 1 ; = $071c
ScreenRight_X_Pos:     .res 1 ; = $071d

ScrollFractional:      .res 1 ; = $0768
PrimaryMsgCounter:     .res 1 ; = $0719
SecondaryMsgCounter:   .res 1 ; = $0749

HorizontalScroll:      .res 1 ; = $073f
VerticalScroll:        .res 1 ; = $0740
ScrollLock:            .res 1 ; = $0723
ScrollThirtyTwo:       .res 1 ; = $073d
Player_X_Scroll:       .res 1 ; = $06ff
Player_Pos_ForScroll:  .res 1 ; = $0755
ScrollAmount:          .res 1 ; = $0775

AreaParserTaskNum:     .res 1 ; = $071f
ColumnSets:            .res 1 ; = $071e
CurrentPageLoc:        .res 1 ; = $0725
CurrentColumnPos:      .res 1 ; = $0726
BackloadingFlag:       .res 1 ; = $0728
BehindAreaParserFlag:  .res 1 ; = $0729
AreaObjectPageLoc:     .res 1 ; = $072a
AreaObjectPageSel:     .res 1 ; = $072b
AreaDataOffset:        .res 1 ; = $072c
AreaObjOffsetBuffer:   .res 1 ; = $072d
AreaObjectLength:      .res 1 ; = $0730
StaircaseControl:      .res 1 ; = $0734
AreaObjectHeight:      .res 1 ; = $0735
MushroomLedgeHalfLen:  .res 1 ; = $0736
EnemyDataOffset:       .res 1 ; = $0739
EnemyObjectPageLoc:    .res 1 ; = $073a
EnemyObjectPageSel:    .res 1 ; = $073b
MetatileBuffer:        .res 1 ; = $06a1
BlockBufferColumnPos:  .res 1 ; = $06a0
CurrentNTAddr_Low:     .res 1 ; = $0721
CurrentNTAddr_High:    .res 1 ; = $0720
AttributeBuffer:       .res 1 ; = $03f9

LoopCommand:           .res 1 ; = $0745

DisplayDigits:         .res 1 ; = $07d7
TopScoreDisplay:       .res 1 ; = $07d7
ScoreAndCoinDisplay:   .res 1 ; = $07dd
PlayerScoreDisplay:    .res 1 ; = $07dd
GameTimerDisplay:      .res 1 ; = $07f8
DigitModifier:         .res 1 ; = $0134

FlagpoleCollisionYPos: .res 1 ; = $070f
StompChainCounter:     .res 1 ; = $0484

VRAM_Buffer1_Offset:   .res 1 ; = $0300
VRAM_Buffer1:          .res 1 ; = $0301
VRAM_Buffer2_Offset:   .res 1 ; = $0340
VRAM_Buffer2:          .res 1 ; = $0341
VRAM_Buffer_AddrCtrl:  .res 1 ; = $0773
Sprite0HitDetectFlag:  .res 1 ; = $0722
DisableScreenFlag:     .res 1 ; = $0774
DisableIntermediate:   .res 1 ; = $0769
ColorRotateOffset:     .res 1 ; = $06d4

TerrainControl:        .res 1 ; = $0727
AreaStyle:             .res 1 ; = $0733
ForegroundScenery:     .res 1 ; = $0741
BackgroundScenery:     .res 1 ; = $0742
CloudTypeOverride:     .res 1 ; = $0743
BackgroundColorCtrl:   .res 1 ; = $0744
AreaType:              .res 1 ; = $074e
AreaAddrsLOffset:      .res 1 ; = $074f
AreaPointer:           .res 1 ; = $0750

PlayerEntranceCtrl:    .res 1 ; = $0710
GameTimerSetting:      .res 1 ; = $0715
AltEntranceControl:    .res 1 ; = $0752
EntrancePage:          .res 1 ; = $0751
NumberOfPlayers:       .res 1 ; = $077a
WarpZoneControl:       .res 1 ; = $06d6
ChangeAreaTimer:       .res 1 ; = $06de

MultiLoopCorrectCntr:  .res 1 ; = $06d9
MultiLoopPassCntr:     .res 1 ; = $06da

FetchNewGameTimerFlag: .res 1 ; = $0757
GameTimerExpiredFlag:  .res 1 ; = $0759

PrimaryHardMode:       .res 1 ; = $076a
SecondaryHardMode:     .res 1 ; = $06cc
WorldSelectNumber:     .res 1 ; = $076b
WorldSelectEnableFlag: .res 1 ; = $07fc
ContinueWorld:         .res 1 ; = $07fd

CurrentPlayer:         .res 1 ; = $0753
PlayerSize:            .res 1 ; = $0754
PlayerStatus:          .res 1 ; = $0756

OnscreenPlayerInfo:    .res 1 ; = $075a
NumberofLives:         .res 1 ; = $075a ;used by current player
HalfwayPage:           .res 1 ; = $075b
LevelNumber:           .res 1 ; = $075c ;the actual dash number
Hidden1UpFlag:         .res 1 ; = $075d
CoinTally:             .res 1 ; = $075e
WorldNumber:           .res 1 ; = $075f
AreaNumber:            .res 1 ; = $0760 ;internal number used to find areas

CoinTallyFor1Ups:      .res 1 ; = $0748

OffscreenPlayerInfo:   .res 1 ; = $0761
OffScr_NumberofLives:  .res 1 ; = $0761 ;used by offscreen player
OffScr_HalfwayPage:    .res 1 ; = $0762
OffScr_LevelNumber:    .res 1 ; = $0763
OffScr_Hidden1UpFlag:  .res 1 ; = $0764
OffScr_CoinTally:      .res 1 ; = $0765
OffScr_WorldNumber:    .res 1 ; = $0766
OffScr_AreaNumber:     .res 1 ; = $0767

BalPlatformAlignment:  .res 1 ; = $03a0
Platform_X_Scroll:     .res 1 ; = $03a1
PlatformCollisionFlag: .res 1 ; = $03a2
YPlatformTopYPos:      .res 1 ; = $0401

BrickCoinTimerFlag:    .res 1 ; = $06bc
StarFlagTaskControl:   .res 1 ; = $0746

PseudoRandomBitReg:    .res 1 ; = $07a7
WarmBootValidation:    .res 1 ; = $07ff

SprShuffleAmtOffset:   .res 1 ; = $06e0
SprShuffleAmt:         .res 1 ; = $06e1
SprDataOffset:         .res 1 ; = $06e4
Player_SprDataOffset:  .res 1 ; = $06e4
Enemy_SprDataOffset:   .res 1 ; = $06e5
Block_SprDataOffset:   .res 1 ; = $06ec
Alt_SprDataOffset:     .res 1 ; = $06ec
Bubble_SprDataOffset:  .res 1 ; = $06ee
FBall_SprDataOffset:   .res 1 ; = $06f1
Misc_SprDataOffset:    .res 1 ; = $06f3
SprDataOffset_Ctrl:    .res 1 ; = $03ee

JumpspringAnimCtrl:    .res 1 ; = $070e
JumpspringForce:       .res 1 ; = $06db

SprObject_Rel_XPos:    .res 1 ; = $03ad
Player_Rel_XPos:       .res 1 ; = $03ad
Enemy_Rel_XPos:        .res 1 ; = $03ae
Fireball_Rel_XPos:     .res 1 ; = $03af
Bubble_Rel_XPos:       .res 1 ; = $03b0
Block_Rel_XPos:        .res 1 ; = $03b1
Misc_Rel_XPos:         .res 1 ; = $03b3

SprObject_Rel_YPos:    .res 1 ; = $03b8
Player_Rel_YPos:       .res 1 ; = $03b8
Enemy_Rel_YPos:        .res 1 ; = $03b9
Fireball_Rel_YPos:     .res 1 ; = $03ba
Bubble_Rel_YPos:       .res 1 ; = $03bb
Block_Rel_YPos:        .res 1 ; = $03bc
Misc_Rel_YPos:         .res 1 ; = $03be

SprObject_SprAttrib:   .res 1 ; = $03c4
Player_SprAttrib:      .res 1 ; = $03c4
Enemy_SprAttrib:       .res 1 ; = $03c5

SprObject_X_MoveForce: .res 1 ; = $0400
Enemy_X_MoveForce:     .res 1 ; = $0401

SprObject_YMF_Dummy:   .res 1 ; = $0416
Player_YMF_Dummy:      .res 1 ; = $0416
Enemy_YMoveForceFractional:       .res 1 ; = $0417
Bubble_YMF_Dummy:      .res 1 ; = $042c

SprObject_Y_MoveForce: .res 1 ; = $0433
Player_Y_MoveForce:    .res 1 ; = $0433
Enemy_Y_MoveForce:     .res 1 ; = $0434
Block_Y_MoveForce:     .res 1 ; = $043c

DisableCollisionDet:   .res 1 ; = $0716
Player_CollisionBits:  .res 1 ; = $0490
Enemy_CollisionBits:   .res 1 ; = $0491

SprObj_BoundBoxCtrl:   .res 1 ; = $0499
Player_BoundBoxCtrl:   .res 1 ; = $0499
Enemy_BoundBoxCtrl:    .res 1 ; = $049a
Fireball_BoundBoxCtrl: .res 1 ; = $04a0
Misc_BoundBoxCtrl:     .res 1 ; = $04a2

EnemyFrenzyBuffer:     .res 1 ; = $06cb
EnemyFrenzyQueue:      .res 1 ; = $06cd

PlayerGfxOffset:       .res 1 ; = $06d5
Player_XSpeedAbsolute: .res 1 ; = $0700
FrictionAdderHigh:     .res 1 ; = $0701
FrictionAdderLow:      .res 1 ; = $0702
RunningSpeed:          .res 1 ; = $0703
SwimmingFlag:          .res 1 ; = $0704
Player_X_MoveForce:    .res 1 ; = $0705
DiffToHaltJump:        .res 1 ; = $0706
JumpOrigin_Y_HighPos:  .res 1 ; = $0707
JumpOrigin_Y_Position: .res 1 ; = $0708
VerticalForce:         .res 1 ; = $0709
VerticalForceDown:     .res 1 ; = $070a
PlayerChangeSizeFlag:  .res 1 ; = $070b
PlayerAnimTimerSet:    .res 1 ; = $070c
PlayerAnimCtrl:        .res 1 ; = $070d
DeathMusicLoaded:      .res 1 ; = $0712
FlagpoleSoundQueue:    .res 1 ; = $0713
CrouchingFlag:         .res 1 ; = $0714
MaximumLeftSpeed:      .res 1 ; = $0450
MaximumRightSpeed:     .res 1 ; = $0456

SprObject_OffscrBits:  .res 1 ; = $03d0
Player_OffscreenBits:  .res 1 ; = $03d0
Enemy_OffscreenBits:   .res 1 ; = $03d1
FBall_OffscreenBits:   .res 1 ; = $03d2
Bubble_OffscreenBits:  .res 1 ; = $03d3
Block_OffscreenBits:   .res 1 ; = $03d4
Misc_OffscreenBits:    .res 1 ; = $03d6
EnemyOffscrBitsMasked: .res 1 ; = $03d8

Cannon_Offset:         .res 1 ; = $046a
Cannon_PageLoc:        .res 1 ; = $046b
Cannon_X_Position:     .res 1 ; = $0471
Cannon_Y_Position:     .res 1 ; = $0477
Cannon_Timer:          .res 1 ; = $047d

Whirlpool_Offset:      .res 1 ; = $046a
Whirlpool_PageLoc:     .res 1 ; = $046b
Whirlpool_LeftExtent:  .res 1 ; = $0471
Whirlpool_Length:      .res 1 ; = $0477
Whirlpool_Flag:        .res 1 ; = $047d

VineFlagOffset:        .res 1 ; = $0398
VineHeight:            .res 1 ; = $0399
VineObjOffset:         .res 1 ; = $039a
VineStart_Y_Position:  .res 1 ; = $039d

Block_Orig_YPos:       .res 1 ; = $03e4
Block_BBuf_Low:        .res 1 ; = $03e6
Block_Metatile:        .res 1 ; = $03e8
Block_PageLoc2:        .res 1 ; = $03ea
Block_RepFlag:         .res 1 ; = $03ec
Block_ResidualCounter: .res 1 ; = $03f0
Block_Orig_XPos:       .res 1 ; = $03f1

BoundingBox_UL_XPos:   .res 1 ; = $04ac
BoundingBox_UL_YPos:   .res 1 ; = $04ad
BoundingBox_DR_XPos:   .res 1 ; = $04ae
BoundingBox_DR_YPos:   .res 1 ; = $04af
BoundingBox_UL_Corner: .res 1 ; = $04ac
BoundingBox_LR_Corner: .res 1 ; = $04ae
EnemyBoundingBoxCoord: .res 1 ; = $04b0

FireballCounter:       .res 1 ; = $06ce
FireballThrowingTimer: .res 1 ; = $0711

HammerEnemyOffset:     .res 1 ; = $06ae
JumpCoinMiscOffset:    .res 1 ; = $06b7

Block_Buffer_1:        .res 1 ; = $0500
Block_Buffer_2:        .res 1 ; = $05d0

HammerThrowingTimer:   .res 1 ; = $03a2
Misc_Collision_Flag:   .res 1 ; = $06be

RedPTroopaOrigXPos:    .res 1 ; = $0401

CheepCheepOrigYPos:    .res 1 ; = $0434
BitMFilter:            .res 1 ; = $06dd

LakituReappearTimer:   .res 1 ; = $06d1
FirebarSpinSpeed:      .res 1 ; = $0388

DuplicateObj_Offset:   .res 1 ; = $06cf
NumberofGroupEnemies:  .res 1 ; = $06d3

BowserBodyControls:    .res 1 ; = $0363
BowserFeetCounter:     .res 1 ; = $0364
BowserMovementSpeed:   .res 1 ; = $0365
BowserOrigXPos:        .res 1 ; = $0366
BowserFlameTimerCtrl:  .res 1 ; = $0367
BowserFront_Offset:    .res 1 ; = $0368
BridgeCollapseOffset:  .res 1 ; = $0369
BowserGfxFlag:         .res 1 ; = $036a
BowserHitPoints:       .res 1 ; = $0483
MaxRangeFromOrigin:    .res 1 ; = $06dc

BowserFlamePRandomOfs: .res 1 ; = $0417

PiranhaPlantUpYPos:    .res 1 ; = $0417
PiranhaPlantDownYPos:  .res 1 ; = $0434

FireworksCounter:      .res 1 ; = $06d7

;sound: relate.res 1 ; d defines
Squ2_NoteLenBuffer:    .res 1 ; = $07b3
Squ2_NoteLenCounter:   .res 1 ; = $07b4
Squ2_EnvelopeDataCtrl: .res 1 ; = $07b5
Squ1_NoteLenCounter:   .res 1 ; = $07b6
Squ1_EnvelopeDataCtrl: .res 1 ; = $07b7
Tri_NoteLenBuffer:     .res 1 ; = $07b8
Tri_NoteLenCounter:    .res 1 ; = $07b9
Noise_BeatLenCounter:  .res 1 ; = $07ba
Squ1_SfxLenCounter:    .res 1 ; = $07bb
Squ2_SfxLenCounter:    .res 1 ; = $07bd
Sfx_SecondaryCounter:  .res 1 ; = $07be
Noise_SfxLenCounter:   .res 1 ; = $07bf

EventMusicBuffer:      .res 1 ; = $07b1
PauseSoundBuffer:      .res 1 ; = $07b2
MusicOffset_Noise:     .res 1 ; = $07b0

DAC_Counter:           .res 1 ; = $07c0
NoiseDataLoopbackOfs:  .res 1 ; = $07c1
NoteLengthTblAdder:    .res 1 ; = $07c4
AreaMusicBuffer_Alt:   .res 1 ; = $07c5
PauseModeFlag:         .res 1 ; = $07c6
GroundMusicHeaderOfs:  .res 1 ; = $07c7
AltRegContentFlag:     .res 1 ; = $07ca

.endif ; __VARIABLES_INC
