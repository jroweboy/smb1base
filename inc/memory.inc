
.ifndef __MEMORY_INC
__MEMORY_INC = 1

; -------------------------------------------------------------------------------------------------------
; ------------------------------R -------A --------M ----------------------------------------------------
; -------------------------------------------------------------------------------------------------------

.globalzp TempReg
.globalzp R0
.globalzp R1
.globalzp R2
.globalzp R3
.globalzp R4
.globalzp R5
.globalzp R6
.globalzp R7

.globalzp IrqTemp
.globalzp IrqR0
.globalzp IrqR1

.globalzp MainTemp
.globalzp M0
.globalzp M1
.globalzp M2
.globalzp M3

.globalzp ObjectOffset
.globalzp FrameCounter

; jroweboy moved to zp
.globalzp SavedJoypadBits
.globalzp SavedJoypad1Bits
.globalzp SavedJoypad2Bits

; moved to abs 
.global A_B_Buttons
.global Up_Down_Buttons
.global Left_Right_Buttons
.global PreviousA_B_Buttons

.globalzp GameEngineSubroutine
.globalzp Enemy_Flag
.globalzp Enemy_ID


.globalzp Player_State

.globalzp Enemy_State
.globalzp Fireball_State
.globalzp Block_State
.globalzp Misc_State


.globalzp PlayerFacingDir
.globalzp PowerUpType
.globalzp FireballBouncingFlag
.globalzp HammerBroJumpTimer
.globalzp Player_MovingDir
.globalzp Enemy_MovingDir


.globalzp SprObject_X_Speed
.globalzp Player_X_Speed

.globalzp Enemy_X_Speed
.globalzp LakituMoveSpeed
.globalzp PiranhaPlant_Y_Speed
.globalzp ExplosionGfxCounter
.globalzp Jumpspring_FixedYPos
.globalzp RedPTroopaCenterYPos
.globalzp BlooperMoveSpeed
.globalzp XMoveSecondaryCounter
.globalzp CheepCheepMoveMFlag
.globalzp FirebarSpinState_Low
.globalzp YPlatformCenterYPos

.globalzp Fireball_X_Speed
.globalzp Block_X_Speed
.globalzp Misc_X_Speed

.globalzp SprObject_PageLoc
.globalzp Player_PageLoc

.globalzp Enemy_PageLoc
.globalzp Fireball_PageLoc
.globalzp Block_PageLoc
.globalzp Misc_PageLoc
.globalzp Bubble_PageLoc


.globalzp SprObject_X_Position
.globalzp Player_X_Position

.globalzp Enemy_X_Position
.globalzp Fireball_X_Position
.globalzp Block_X_Position
.globalzp Misc_X_Position
.globalzp Bubble_X_Position

.globalzp Player_Y_Speed
.globalzp SprObject_Y_Speed

.globalzp FirebarSpinState_High
.globalzp XMovePrimaryCounter
.globalzp BlooperMoveCounter
.globalzp Enemy_Y_Speed
.globalzp LakituMoveDirection
.globalzp ExplosionTimerCounter
.globalzp PiranhaPlant_MoveFlag

.globalzp Fireball_Y_Speed
.globalzp Block_Y_Speed
.globalzp Misc_Y_Speed

.globalzp SprObject_Y_HighPos
.globalzp Player_Y_HighPos

.globalzp Enemy_Y_HighPos
.globalzp Fireball_Y_HighPos
.globalzp Block_Y_HighPos
.globalzp Misc_Y_HighPos
.globalzp Bubble_Y_HighPos

.globalzp Player_Y_Position
.globalzp SprObject_Y_Position

.globalzp Enemy_Y_Position
.globalzp Fireball_Y_Position
.globalzp Block_Y_Position
.globalzp Misc_Y_Position
.globalzp Bubble_Y_Position

.globalzp AreaData
.globalzp AreaDataLow
.globalzp AreaDataHigh

.globalzp EnemyData
.globalzp EnemyDataLow
.globalzp EnemyDataHigh

.globalzp SpriteLocalTemp
.globalzp Local_eb
.globalzp Local_ec
.globalzp Local_ed
; .globalzp Local_ee ; jroweboy: unused?
.globalzp Local_ef

.globalzp MusicData
.globalzp MusicDataLow
.globalzp MusicDataHigh


; ssdpcm zp vars

.globalzp idx_superblock, idx_block, idx_pcm_decode
.globalzp bits_bank, slopes_bank
.globalzp ptr_bitstream, ptr_slopes, superblock_length, last_sample
.globalzp tmp_irq_a, irq_latch_value

; .segment "STACK"
.global VerticalFlipFlag
.global FlagpoleFNum_Y_Pos
.global FlagpoleFNum_YMFDummy
.global FlagpoleScore

.global TempRam


.global FloateyNum_Control
.global FloateyNum_X_Pos
.global FloateyNum_Y_Pos

.global ShellChainCounter
.global FloateyNum_Timer
.global DigitModifier

.global NmiDisable
.global NmiSkipped
.global X_Magnitude
.global Y_Magnitude
.global Abs_X_Magnitude
.global Abs_Y_Magnitude

.global BhopInitalized

.global HoldingSlingshot
.global SlingPull_Rel_XPos
.global SlingPull_Rel_YPos

.global PlayPanic

.global IrqOldScroll
.global IrqNewScroll
.global IrqPPUCTRL
.global IrqNextScanline

.global PlayerAngle
.global GroundBounceChain
.global BounceForce
.global AngularMomentum
.global AngularMomentumTimer
.global InitialFireballYSpeed
.global SwitchToMainIRQ


.global NoteLenLookupTblOfs
.global Square1SoundBuffer
.global Square2SoundBuffer
.global NoiseSoundBuffer
.global AreaMusicBuffer

.global MusicOffset_Square2
.global MusicOffset_Square1
.global MusicOffset_Triangle

.global PauseSoundQueue
.global AreaMusicQueue
.global EventMusicQueue
.global NoiseSoundQueue
.global Square2SoundQueue
.global Square1SoundQueue


StackClear = DigitModifier

.global IrqPointerJmp
.global IrqPointer


; segment "BSS"

.global Sprite_Y_Position
.global Sprite_Data
.global Sprite_Tilenumber
.global Sprite_Attributes
.global Sprite_X_Position

.global VRAM_Buffer1_Offset
.global VRAM_Buffer1
.global VRAM_Buffer2_Offset
.global VRAM_Buffer2

.global BowserBodyControls
.global BowserFeetCounter
.global BowserMovementSpeed
.global BowserOrigXPos
.global BowserFlameTimerCtrl
.global BowserFront_Offset
.global BridgeCollapseOffset
.global BowserGfxFlag

.global FirebarSpinSpeed


.global Vine_FlagOffset
.global Vine_Height
.global Vine_ObjOffset
.global Vine_Start_Y_Position

.global BalPlatformAlignment
.global Platform_X_Scroll

.global HammerThrowingTimer
.global PlatformCollisionFlag

.global Player_Rel_XPos
.global SprObject_Rel_XPos

.global Enemy_Rel_XPos
.global Fireball_Rel_XPos
.global Bubble_Rel_XPos
.global Block_Rel_XPos
.global Misc_Rel_XPos
.global SprObject_Rel_YPos
.global Player_Rel_YPos
.global Enemy_Rel_YPos
.global Fireball_Rel_YPos
.global Bubble_Rel_YPos
.global Block_Rel_YPos
.global Misc_Rel_YPos

.global Player_SprAttrib
.global SprObject_SprAttrib

.global Enemy_SprAttrib

.global SprObject_OffscrBits
.global Player_OffscreenBits

.global Enemy_OffscreenBits
.global FBall_OffscreenBits
.global Bubble_OffscreenBits
.global Block_OffscreenBits
.global Misc_OffscreenBits
.global EnemyOffscrBitsMasked
.global Block_Orig_YPos
.global Block_BBuf_Low
.global Block_Metatile
.global Block_PageLoc2
.global Block_RepFlag
.global SprDataOffset_Ctrl
.global Block_Orig_XPos
.global AttributeBuffer
.global SprObject_X_MoveForce

.global Enemy_X_MoveForce
.global YPlatformTopYPos
.global RedPTroopaOrigXPos

.global SprObject_YMoveForceFractional
.global Player_YMoveForceFractional

.global Enemy_YMoveForceFractional
.global BowserFlamePRandomOfs
.global PiranhaPlantUpYPos

.global Bubble_YMoveForceFractional

.global Player_Y_MoveForce
.global SprObject_Y_MoveForce

.global CheepCheepOrigYPos
.global Enemy_Y_MoveForce
.global PiranhaPlantDownYPos

.global Block_Y_MoveForce
.global MaximumLeftSpeed
.global MaximumRightSpeed

.global Whirlpool_Offset
.global Cannon_Offset

.global Whirlpool_PageLoc
.global Cannon_PageLoc

.global Cannon_X_Position
.global Whirlpool_LeftExtent

.global Whirlpool_Length
.global Cannon_Y_Position

.global Enemy_PowerupType

.global Cannon_Timer
.global Whirlpool_Flag

.global BowserHitPoints
.global StompChainCounter
.global Player_CollisionBits
.global Enemy_CollisionBits

.global Player_BoundBoxCtrl
.global SprObj_BoundBoxCtrl

.global Enemy_BoundBoxCtrl
.global Fireball_BoundBoxCtrl
.global Misc_BoundBoxCtrl

.global BoundingBox_UL_XPos
.global BoundingBox_UL_Corner

.global BoundingBox_UL_YPos

.global BoundingBox_DR_XPos
.global BoundingBox_LR_Corner

.global BoundingBox_DR_YPos
.global EnemyBoundingBoxCoord
.global Block_Buffer_1
.global Block_Buffer_2
.global BlockBufferColumnPos
.global MetatileBuffer
.global HammerEnemyOffset
.global JumpCoinMiscOffset
.global BrickCoinTimerFlag
.global Misc_Collision_Flag
.global EnemyFrenzyBuffer
.global LakituObjectBuffer
.global LakituActionBuffer
.global LakituActionFlag
.global SecondaryHardMode
.global EnemyFrenzyQueue
.global FireballCounter
.global DuplicateObj_Offset
.global LakituReappearTimer
.global NumberofGroupEnemies
.global ColorRotateOffset
.global PlayerGfxOffset
.global WarpZoneControl
.global FireworksCounter
.global MultiLoopCorrectCntr
.global MultiLoopPassCntr
.global JumpspringForce
.global MaxRangeFromOrigin
.global BitMFilter
.global ChangeAreaTimer

.global PlayerOAMOffset
.global CurrentOAMOffset
.global OriginalOAMOffset
.global FollowerOAMOffset
.global PlayerFollowerDrawOrder
.global PlatformLastOAMOrder

.global SpriteShuffleOffset
.global SpriteShuffleTemp

.global LivesScreenTimer

.global Player_X_Scroll
.global Player_XSpeedAbsolute
.global FrictionAdderHigh
.global FrictionAdderLow
.global RunningSpeed
.global SwimmingFlag
.global Player_X_MoveForce
.global DiffToHaltJump
.global JumpOrigin_Y_HighPos
.global JumpOrigin_Y_Position
.global VerticalForce
.global VerticalForceDown
.global PlayerChangeSizeFlag
.global PlayerAnimTimerSet
.global PlayerAnimCtrl
.global JumpspringAnimCtrl
.global FlagpoleCollisionYPos
.global PlayerEntranceCtrl
.global FireballThrowingTimer
.global DeathMusicLoaded
.global FlagpoleSoundQueue
.global CrouchingFlag
.global GameTimerSetting
.global DisableCollisionDet
.global DemoAction
.global DemoActionTimer
.global PrimaryMsgCounter
.global PlayerMoonwalkFlag

.global ScreenEdge_PageLoc
.global ScreenLeft_PageLoc

.global ScreenRight_PageLoc

.global ScreenEdge_X_Pos
.global ScreenLeft_X_Pos

.global ScreenRight_X_Pos
.global ColumnSets
.global AreaParserTaskNum
.global CurrentNTAddr_High
.global CurrentNTAddr_Low
.global Sprite0HitDetectFlag
.global ScrollLock
.global CurrentPageLoc
.global CurrentColumnPos
.global TerrainControl
.global BackloadingFlag
.global BehindAreaParserFlag
.global AreaObjectPageLoc
.global AreaObjectPageSel
.global AreaDataOffset
.global AreaObjOffsetBuffer
.global AreaObjectLength
.global AreaStyle
.global StaircaseControl
.global AreaObjectHeight
.global MushroomLedgeHalfLen
.global EnemyDataOffset
.global EnemyObjectPageLoc
.global EnemyObjectPageSel
.global ScreenRoutineTask
.global ScrollThirtyTwo
.global HorizontalScroll
.global VerticalScroll
.global ForegroundScenery
.global BackgroundScenery
.global CloudTypeOverride
.global BackgroundColorCtrl
.global LoopCommand
.global StarFlagTaskControl
.global TimerControl
.global CoinTallyFor1Ups
.global SecondaryMsgCounter
.global JoypadBitMask
.global InPipeTransition
.global PipeXPosition
.global PipeYPosition
.global PipeExitTimer

.global FirebarSpinDirection
.global DestinationPageLoc
.global VictoryWalkControl

.global AreaType

.global AreaAddrsLOffset
.global AreaPointer
.global EntrancePage
.global AltEntranceControl
.global CurrentPlayer
.global PlayerSize
.global Player_Pos_ForScroll
.global PlayerStatus
.global FetchNewGameTimerFlag
.global JoypadOverride
.global GameTimerExpiredFlag

.global OnscreenPlayerInfo
.global NumberofLives

.global HalfwayPage
.global LevelNumber
; .global Hidden1UpFlag
.global CoinTally
.global CoinHudNumber
.global WorldNumber
.global AreaNumber

; .global OffscreenPlayerInfo
; .global OffScr_NumberofLives
; .global OffScr_HalfwayPage
; .global OffScr_LevelNumber
; .global OffScr_Hidden1UpFlag
; .global OffScr_CoinTally
; .global OffScr_WorldNumber
; .global OffScr_AreaNumber

.global ScrollFractional
.global DisableIntermediate
.global PrimaryHardMode
.global WorldSelectNumber

.global DiscoCycleIdx
.global CurrentFloorTileBank
.global CurrentDiscoColor

; $.global 0770

.global OperMode
.global OperMode_Task
.global VRAM_Buffer_AddrCtrl
.global DisableScreenFlag
.global ScrollAmount
.global GamePauseStatus
.global GamePauseTimer
.global Mirror_PPUCTRL
.global Mirror_PPUMASK
.global NumberOfPlayers
.global IntervalTimerControl

.global Timers
.global SelectTimer
.global PlayerAnimTimer
.global JumpSwimTimer
.global RunningTimer
.global BlockBounceTimer
.global SideCollisionTimer
.global JumpspringTimer
.global GameTimerCtrlTimer
.global ClimbSideTimer
.global EnemyFrameTimer
.global FrenzyEnemyTimer
.global BowserFireBreathTimer
.global StompTimer
.global AirBubbleTimer
; .global AirTimeTimer
FRAME_TIMER_COUNT = AirBubbleTimer - Timers

.global ScrollIntervalTimer
.global EnemyIntervalTimer
.global BrickCoinTimer
.global InjuryTimer
.global StarInvincibleTimer
.global ScreenTimer
.global WorldEndTimer
.global PlayerSwimmingTimer
.global LakituActionTimer
.global LakituPowerupTimer
.global LakituEnemyTimer
.global DeathTimer
ALL_TIMER_COUNT = DeathTimer - Timers

.global PseudoRandomBitReg

.global SoundMemory
.global MusicOffset_Noise
.global EventMusicBuffer
.global PauseSoundBuffer
.global Squ2_NoteLenBuffer
.global Squ2_NoteLenCounter
.global Squ2_EnvelopeDataCtrl
.global Squ1_NoteLenCounter
.global Squ1_EnvelopeDataCtrl
.global Tri_NoteLenBuffer
.global Tri_NoteLenCounter
.global Noise_BeatLenCounter
.global Squ1_SfxLenCounter
.global Squ2_SfxLenCounter
.global Sfx_SecondaryCounter
.global Noise_SfxLenCounter
.global DMCSoundBuffer
.global DpcmSampleQueue
.global NoiseDataLoopbackOfs
.global NoteLengthTblAdder
.global AreaMusicBuffer_Alt

.global PauseModeFlag
.global GroundMusicHeaderOfs
.global AltRegContentFlag


_WarmBootOffset = DisplayDigits

.global OriginalLeader
.global CurrentLeader
.global DisplayDigits
.global TopScoreDisplay

.global PlayerScoreDisplay
.global ScoreAndCoinDisplay

.global GameTimerDisplay
.global WorldSelectEnableFlag

.global ContinueWorld

_ColdBootOffset = WarmBootValidation

.global WarmBootValidation

; .segment "WRAM"

.global BhopValidation

.endif ; __MEMORY_INC
