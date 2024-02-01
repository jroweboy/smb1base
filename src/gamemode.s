
.include "common.inc"

; core.s ???
.import GameCoreRoutine, ScreenRoutines, UpdScrollVar
.import EnemiesAndLoopsCore, RelativePlayerPosition, PlayerGfxHandler
.import LoadAreaPointer, TerminateGame

.import IncModeTask_B

; objects/bowser.s
.import BridgeCollapse
; player.s
.import PlayerCtrlRoutine, ScrollScreen
; level.s
.import GetAreaDataAddrs

.export OperModeExecutionTree

; reset.s
.export PlayerEndWorld

.segment "CODE"

;-------------------------------------------------------------------------------------

;indirect jump routine called when
;$0770 is set to 1
.proc GameMode

  lda OperMode_Task
  jsr JumpEngine

  .word InitializeArea
  .word ScreenRoutines
  .word SecondaryGameSetup
  .word GameCoreRoutine
.endproc

;-------------------------------------------------------------------------------------
PrimaryGameSetup:
.import GetAreaMusic

  lda #$01
  sta FetchNewGameTimerFlag   ;set flag to load game timer from header
  sta PlayerSize              ;set player's size to small
  lda #$02
  sta NumberofLives           ;give each player three lives
  sta OffScr_NumberofLives
SecondaryGameSetup:
  lda #$00
  sta DisableScreenFlag     ;enable screen output
  tay
ClearVRLoop: sta VRAM_Buffer1-1,y      ;clear buffer at $0300-$03ff
  iny
  bne ClearVRLoop
  sta GameTimerExpiredFlag  ;clear game timer exp flag
  sta DisableIntermediate   ;clear skip lives display flag
  sta BackloadingFlag       ;clear value here
  lda #$ff
  sta BalPlatformAlignment  ;initialize balance platform assignment flag
  lda ScreenLeft_PageLoc    ;get left side page location
  lsr Mirror_PPUCTRL       ;shift LSB of ppu register #1 mirror out
  and #$01                  ;mask out all but LSB of page location
  ror                       ;rotate LSB of page location into carry then onto mirror
  rol Mirror_PPUCTRL       ;this is to set the proper PPU name table
  jsr GetAreaMusic          ;load proper music into queue
  inc Sprite0HitDetectFlag  ;set sprite #0 check flag
  inc OperMode_Task         ;increment to next task
  rts

;-------------------------------------------------------------------------------------
.proc OperModeExecutionTree
  lda OperMode     ;this is the heart of the entire program,
  jsr JumpEngine   ;most of what goes on starts here

  .word TitleScreenMode
  .word GameMode
  .word VictoryMode
  .word GameOverMode
.endproc

;-------------------------------------------------------------------------------------

.proc TitleScreenMode
  lda OperMode_Task
  jsr JumpEngine

  .word InitializeGame
  .word ScreenRoutines
  .word PrimaryGameSetup
  .word GameMenuRoutine
.endproc

InitializeGame:
.import InitializeMemory, LoadAreaPointer
  ldy #<WorldSelectNumber  ;clear all memory as in initialization procedure,
  jsr InitializeMemory     ;but this time, clear only as far as $076f
  ldy #AreaMusicBuffer_Alt - SoundMemory
ClrSndLoop:
  sta SoundMemory,y     ;clear out memory used
  dey                   ;by the sound engines
  bpl ClrSndLoop
  lda #$18              ;set demo timer
  sta DemoTimer
  jsr LoadAreaPointer

InitializeArea:

  ldy #<SecondaryMsgCounter                 ;clear all memory again, only as far as $074b
  jsr InitializeMemory     ;this is only necessary in game mode
  ldx #FRAME_TIMER_COUNT
  lda #$00
ClrTimersLoop:
    sta Timers,x             ;clear out memory between
    dex                      ;$0780 and $07a1
    bpl ClrTimersLoop
  lda HalfwayPage
  ldy AltEntranceControl   ;if AltEntranceControl not set, use halfway page, if any found
  beq StartPage
    lda EntrancePage         ;otherwise use saved entry page number here
StartPage:
  sta ScreenLeft_PageLoc   ;set as value here
  sta CurrentPageLoc       ;also set as current page
  sta BackloadingFlag      ;set flag here if halfway page or saved entry page number found
  jsr GetScreenPosition    ;get pixel coordinates for screen borders
  ldy #$20                 ;if on odd numbered page, use $2480 as start of rendering
  and #%00000001           ;otherwise use $2080, this address used later as name table
  beq SetInitNTHigh        ;address for rendering of game area
    ldy #$24
SetInitNTHigh:
  sty CurrentNTAddr_High   ;store name table address
  ldy #$80
  sty CurrentNTAddr_Low
  asl                      ;store LSB of page number in high nybble
  asl                      ;of block buffer column position
  asl
  asl
  sta BlockBufferColumnPos
  dec AreaObjectLength     ;set area object lengths for all empty
  dec AreaObjectLength+1
  dec AreaObjectLength+2
  lda #$0b                 ;set value for renderer to update 12 column sets
  sta ColumnSets           ;12 column sets = 24 metatile columns = 1 1/2 screens
  farcall GetAreaDataAddrs     ;get enemy and level addresses and load header
  lda PrimaryHardMode      ;check to see if primary hard mode has been activated
  bne SetSecHard           ;if so, activate the secondary no matter where we're at
    lda WorldNumber          ;otherwise check world number
    cmp #World5              ;if less than 5, do not activate secondary
    bcc CheckHalfway
      bne SetSecHard           ;if not equal to, then world > 5, thus activate
        lda LevelNumber          ;otherwise, world 5, so check level number
        cmp #Level3              ;if 1 or 2, do not set secondary hard mode flag
        bcc CheckHalfway
  SetSecHard:
          inc SecondaryHardMode    ;set secondary hard mode flag for areas 5-3 and beyond
  CheckHalfway:
    lda HalfwayPage
    beq DoneInitArea
      lda #$02                 ;if halfway page set, overwrite start position from header
      sta PlayerEntranceCtrl
DoneInitArea:
  lda #Silence             ;silence music
  sta AreaMusicQueue
  lda #$01                 ;disable screen output
  sta DisableScreenFlag
  inc OperMode_Task        ;increment one of the modes
  rts


;-------------------------------------------------------------------------------------
.proc GameOverMode
  lda OperMode_Task
  jsr JumpEngine
  
  .word SetupGameOver
  .word ScreenRoutines
  .word RunGameOver
.endproc

;-------------------------------------------------------------------------------------

.proc SetupGameOver
  lda #$00                  ;reset screen routine task control for title screen, game,
  sta ScreenRoutineTask     ;and game over modes
  sta Sprite0HitDetectFlag  ;disable sprite 0 check
  lda #GameOverMusic
  sta EventMusicQueue       ;put game over music in secondary queue
  inc DisableScreenFlag     ;disable screen output
  inc OperMode_Task         ;set secondary mode to 1
  rts
.endproc


;-------------------------------------------------------------------------------------

.proc RunGameOver
.import LoadAreaPointer, TransposePlayers
.export ContinueGame, TerminateGame

  lda #$00              ;reenable screen
  sta DisableScreenFlag
  lda SavedJoypad1Bits  ;check controller for start pressed
  and #Start_Button
  bne TerminateGame
  lda ScreenTimer       ;if not pressed, wait for
  bne GameIsOn          ;screen timer to expire
TerminateGame:
  lda #Silence          ;silence music
  sta EventMusicQueue
  jsr TransposePlayers  ;check if other player can keep
  bcc ContinueGame      ;going, and do so if possible
  lda WorldNumber       ;otherwise put world number of current
  sta ContinueWorld     ;player into secret continue function variable
  lda #$00
  sta OperMode_Task     ;reset all modes to title screen and
  sta ScreenTimer       ;leave
  sta OperMode
  rts

ContinueGame:
  jsr LoadAreaPointer       ;update level pointer with
  lda #$01                  ;actual world and area numbers, then
  sta PlayerSize            ;reset player's size, status, and
  inc FetchNewGameTimerFlag ;set game timer flag to reload
  lda #$00                  ;game timer from header
  sta TimerControl          ;also set flag for timers to count again
  sta PlayerStatus
  sta GameEngineSubroutine  ;reset task for game core
  sta OperMode_Task         ;set modes and leave
  lda #$01                  ;if in game over mode, switch back to
  sta OperMode              ;game mode, because game is still on
GameIsOn:
  rts
.endproc


;-------------------------------------------------------------------------------------

.proc GameMenuRoutine
.import LoadAreaPointer, GameCoreRoutine, DrawMushroomIcon

  ldy #$00
  lda SavedJoypad1Bits        ;check to see if either player pressed
  ora SavedJoypad2Bits        ;only the start button (either joypad)
  cmp #Start_Button
  beq StartGame
  cmp #A_Button+Start_Button  ;check to see if A + start was pressed
  bne ChkSelect               ;if not, branch to check select button
StartGame:
  jmp ChkContinue             ;if either start or A + start, execute here
ChkSelect:
  cmp #Select_Button          ;check to see if the select button was pressed
  beq SelectBLogic            ;if so, branch reset demo timer
  ldx DemoTimer               ;otherwise check demo timer
  bne ChkWorldSel             ;if demo timer not expired, branch to check world selection
  sta SelectTimer             ;set controller bits here if running demo
  jsr DemoEngine              ;run through the demo actions
  bcs ResetTitle              ;if carry flag set, demo over, thus branch
  jmp RunDemo                 ;otherwise, run game engine for demo
ChkWorldSel:
  ldx WorldSelectEnableFlag   ;check to see if world selection has been enabled
  beq NullJoypad
  cmp #B_Button               ;if so, check to see if the B button was pressed
  bne NullJoypad
  iny                         ;if so, increment Y and execute same code as select
SelectBLogic:
  lda DemoTimer               ;if select or B pressed, check demo timer one last time
  beq ResetTitle              ;if demo timer expired, branch to reset title screen mode
  lda #$18                    ;otherwise reset demo timer
  sta DemoTimer
  lda SelectTimer             ;check select/B button timer
  bne NullJoypad              ;if not expired, branch
  lda #$10                    ;otherwise reset select button timer
  sta SelectTimer
  cpy #$01                    ;was the B button pressed earlier?  if so, branch
  beq IncWorldSel             ;note this will not be run if world selection is disabled
  lda NumberOfPlayers         ;if no, must have been the select button, therefore
  eor #%00000001              ;change number of players and draw icon accordingly
  sta NumberOfPlayers
  jsr DrawMushroomIcon
  jmp NullJoypad
IncWorldSel:
  ldx WorldSelectNumber       ;increment world select number
  inx
  txa
  and #%00000111              ;mask out higher bits
  sta WorldSelectNumber       ;store as current world select number
  jsr GoContinue
UpdateShroom:
  lda WSelectBufferTemplate,x ;write template for world select in vram buffer
  sta VRAM_Buffer1-1,x        ;do this until all bytes are written
  inx
  cpx #$06
  bmi UpdateShroom
  ldy WorldNumber             ;get world number from variable and increment for
  iny                         ;proper display, and put in blank byte before
  sty VRAM_Buffer1+3          ;null terminator
NullJoypad:
  lda #$00                    ;clear joypad bits for player 1
  sta SavedJoypad1Bits
RunDemo:
  jsr GameCoreRoutine         ;run game engine
  lda GameEngineSubroutine    ;check to see if we're running lose life routine
  cmp #$06
  bne ExitMenu                ;if not, do not do all the resetting below
ResetTitle:
  lda #$00                    ;reset game modes, disable
  sta OperMode                ;sprite 0 check and disable
  sta OperMode_Task           ;screen output
  sta Sprite0HitDetectFlag
  inc DisableScreenFlag
  rts
ChkContinue:
  ldy DemoTimer               ;if timer for demo has expired, reset modes
  beq ResetTitle
  asl                         ;check to see if A button was also pushed
  bcc StartWorld1             ;if not, don't load continue function's world number
  lda ContinueWorld           ;load previously saved world number for secret
  jsr GoContinue              ;continue function when pressing A + start
StartWorld1:
  jsr LoadAreaPointer
  inc Hidden1UpFlag           ;set 1-up box flag for both players
  inc OffScr_Hidden1UpFlag
  inc FetchNewGameTimerFlag   ;set fetch new game timer flag
  inc OperMode                ;set next game mode
  lda WorldSelectEnableFlag   ;if world select flag is on, then primary
  sta PrimaryHardMode         ;hard mode must be on as well
  lda #$00
  sta OperMode_Task           ;set game mode here, and clear demo timer
  sta DemoTimer
  ldx #$17
  lda #$00
InitScores:
  sta ScoreAndCoinDisplay,x   ;clear player scores and coin displays
  dex
  bpl InitScores
ExitMenu:
  rts
GoContinue:
  sta WorldNumber             ;start both players at the first area
  sta OffScr_WorldNumber      ;of the previously saved world number
  ldx #$00                    ;note that on power-up using this function
  stx AreaNumber              ;will make no difference
  stx OffScr_AreaNumber   
  rts
              
WSelectBufferTemplate:
      .byte $04, $20, $73, $01, $00, $00

.endproc


;-------------------------------------------------------------------------------------

.proc DemoEngine
  ldx DemoAction         ;load current demo action
  lda DemoActionTimer    ;load current action timer
  bne DoAction           ;if timer still counting down, skip
  inx
  inc DemoAction         ;if expired, increment action, X, and
  sec                    ;set carry by default for demo over
  lda DemoTimingData-1,x ;get next timer
  sta DemoActionTimer    ;store as current timer
  beq DemoOver           ;if timer already at zero, skip
DoAction:
  lda DemoActionData-1,x ;get and perform action (current or next)
  sta SavedJoypad1Bits
  dec DemoActionTimer    ;decrement action timer
  clc                    ;clear carry if demo still going
DemoOver:
  rts

DemoActionData:
      .byte $01, $80, $02, $81, $41, $80, $01
      .byte $42, $c2, $02, $80, $41, $c1, $41, $c1
      .byte $01, $c1, $01, $02, $80, $00

DemoTimingData:
      .byte $9b, $10, $18, $05, $2c, $20, $24
      .byte $15, $5a, $10, $20, $28, $30, $20, $10
      .byte $80, $20, $30, $30, $01, $ff, $00
.endproc


;-------------------------------------------------------------------------------------

.proc VictoryMode

  lda #0
  sta PlayerOAMOffset
  lda #4 * 4 ; save enough room to draw the player first later
  sta CurrentOAMOffset
  
  jsr VictoryModeSubroutines  ;run victory mode subroutines
  lda OperMode_Task           ;get current task of victory mode
  beq AutoPlayer              ;if on bridge collapse, skip enemy processing
  ldx #$00
  stx ObjectOffset            ;otherwise reset enemy object offset 
  ; TODO Consolidate farcall 
  farcall EnemiesAndLoopsCore     ;and run enemy code
AutoPlayer:
  jsr RelativePlayerPosition  ;get player's relative coordinates
  farcall PlayerGfxHandler    ;draw the player, then leave
  
  farcall DrawAllMetasprites, jmp

.endproc

.proc VictoryModeSubroutines
  lda OperMode_Task
  jsr JumpEngine

  .word BridgeCollapseJmp
  .word SetupVictoryMode
  .word PlayerVictoryWalk
  .word PrintVictoryMessages
  .word PlayerEndWorld
.endproc

.proc BridgeCollapseJmp
  farcall BridgeCollapse, jmp
.endproc

;-------------------------------------------------------------------------------------

.proc SetupVictoryMode
  ldx ScreenRight_PageLoc  ;get page location of right side of screen
  inx                      ;increment to next page
  stx DestinationPageLoc   ;store here
  lda #EndOfCastleMusic
  sta EventMusicQueue      ;play win castle music
  ; jroweboy (just inline it its literally just one more byte)
  ; jmp IncModeTask_B        ;jump to set next major task in victory mode
  inc OperMode_Task
  rts
.endproc

;-------------------------------------------------------------------------------------
; TODO jroweboy move this to player.s maybe
.proc PlayerVictoryWalk
  ldy #$00                ;set value here to not walk player by default
  sty VictoryWalkControl
  lda Player_PageLoc      ;get player's page location
  cmp DestinationPageLoc  ;compare with destination page location
  bne PerformWalk         ;if page locations don't match, branch
  lda Player_X_Position   ;otherwise get player's horizontal position
  cmp #$60                ;compare with preset horizontal position
  bcs DontWalk            ;if still on other page, branch ahead
PerformWalk:
  inc VictoryWalkControl  ;otherwise increment value and Y
  iny                     ;note Y will be used to walk the player
DontWalk:
  tya                     ;put contents of Y in A and
  sta SavedJoypadBits
  farcall PlayerCtrlRoutine   ;use A to move player to the right or not
  lda ScreenLeft_PageLoc  ;check page location of left side of screen
  cmp DestinationPageLoc  ;against set value here
  beq ExitVWalk           ;branch if equal to change modes if necessary
  lda ScrollFractional
  clc                     ;do fixed point math on fractional part of scroll
  adc #$80        
  sta ScrollFractional    ;save fractional movement amount
  lda #$01                ;set 1 pixel per frame
  adc #$00                ;add carry from previous addition
  tay                     ;use as scroll amount
  farcall ScrollScreen        ;do sub to scroll the screen
  jsr UpdScrollVar        ;do another sub to update screen and scroll variables
  inc VictoryWalkControl  ;increment value to stay in this routine
ExitVWalk:
  lda VictoryWalkControl  ;load value set here
  ; jroweboy: Change from vanilla: just inc and rts here instead
  ; beq IncModeTask_A       ;if zero, branch to change modes
  ; rts                     ;otherwise leave
  bne DontIncModeTask
    inc OperMode_Task
DontIncModeTask:
  rts
.endproc

;-------------------------------------------------------------------------------------

.proc PrintVictoryMessages
  lda SecondaryMsgCounter   ;load secondary message counter
  bne IncMsgCounter         ;if set, branch to increment message counters
  lda PrimaryMsgCounter     ;otherwise load primary message counter
  beq ThankPlayer           ;if set to zero, branch to print first message
  ldy WorldNumber           ;check world number
  cpy #World8
  bne MRetainerMsg          ;if not at world 8, skip to next part
  cmp #$03                  ;check primary message counter again
  bcc IncMsgCounter         ;if not at 3 yet (world 8 only), branch to increment
  sbc #$01                  ;otherwise subtract one
  jmp ThankPlayer           ;and skip to next part
MRetainerMsg:
  cmp #$02                  ;check primary message counter
  bcc IncMsgCounter         ;if not at 2 yet (world 1-7 only), branch
ThankPlayer:
  tay                       ;put primary message counter into Y
  bne SecondPartMsg         ;if counter nonzero, skip this part, do not print first message
  lda CurrentPlayer         ;otherwise get player currently on the screen
  beq EvalForMusic          ;if mario, branch
  iny                       ;otherwise increment Y once for luigi and
  bne EvalForMusic          ;do an unconditional branch to the same place
SecondPartMsg:
  iny                       ;increment Y to do world 8's message
  lda WorldNumber
  cmp #World8               ;check world number
  beq EvalForMusic          ;if at world 8, branch to next part
  dey                       ;otherwise decrement Y for world 1-7's message
  cpy #$04                  ;if counter at 4 (world 1-7 only)
  bcs SetEndTimer           ;branch to set victory end timer
  cpy #$03                  ;if counter at 3 (world 1-7 only)
  bcs IncMsgCounter         ;branch to keep counting
EvalForMusic:
  cpy #$03                  ;if counter not yet at 3 (world 8 only), branch
  bne PrintMsg              ;to print message only (note world 1-7 will only
  lda #VictoryMusic         ;reach this code if counter = 0, and will always branch)
  sta EventMusicQueue       ;otherwise load victory music first (world 8 only)
PrintMsg:
  tya                       ;put primary message counter in A
  clc                       ;add $0c or 12 to counter thus giving an appropriate value,
  adc #$0c                  ;($0c-$0d = first), ($0e = world 1-7's), ($0f-$12 = world 8's)
  sta VRAM_Buffer_AddrCtrl  ;write message counter to vram address controller
IncMsgCounter:
  lda SecondaryMsgCounter
  clc
  adc #$04                      ;add four to secondary message counter
  sta SecondaryMsgCounter
  lda PrimaryMsgCounter
  adc #$00                      ;add carry to primary message counter
  sta PrimaryMsgCounter
  cmp #$07                      ;check primary counter one more time
SetEndTimer:
  bcc ExitMsgs                  ;if not reached value yet, branch to leave
  lda #$06
  sta WorldEndTimer             ;otherwise set world end timer
IncModeTask_A:
  inc OperMode_Task             ;move onto next task in mode
ExitMsgs:
  rts                           ;leave
.endproc

;-------------------------------------------------------------------------------------

.proc PlayerEndWorld

  lda WorldEndTimer          ;check to see if world end timer expired
  bne EndExitOne             ;branch to leave if not
  ldy WorldNumber            ;check world number
  cpy #World8                ;if on world 8, player is done with game, 
  bcs EndChkBButton          ;thus branch to read controller
  lda #$00
  sta AreaNumber             ;otherwise initialize area number used as offset
  sta LevelNumber            ;and level number control to start at area 1
  sta OperMode_Task          ;initialize secondary mode of operation
  ; jroweboy added:
  ; disable the screen since loading a new area pointer will change CHR banks
  inc DisableScreenFlag
  inc WorldNumber            ;increment world number to move onto the next world
  jsr LoadAreaPointer        ;get area address offset for the next area
  inc FetchNewGameTimerFlag  ;set flag to load game timer from header
  lda #MODE_GAMEPLAY
  sta OperMode               ;set mode of operation to game mode
EndExitOne:
  rts                        ;and leave
EndChkBButton:
  lda SavedJoypad1Bits
  ora SavedJoypad2Bits       ;check to see if B button was pressed on
  and #B_Button              ;either controller
  beq EndExitTwo             ;branch to leave if not
  lda #$01                   ;otherwise set world selection flag
  sta WorldSelectEnableFlag
  lda #$ff                   ;remove onscreen player's lives
  sta NumberofLives
  jsr TerminateGame          ;do sub to continue other player or end game
EndExitTwo:
  rts                        ;leave
.endproc
