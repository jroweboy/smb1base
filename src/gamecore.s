
.include "common.inc"

;-------------------------------------------------------------------------------------
.proc GameCoreRoutine
.import ProcFireball_Bubble, EnemiesAndLoopsCore, FloateyNumbersRoutine, GetPlayerOffscreenBits
.import PlayerGfxHandler, RelativePlayerPosition
.export GameCoreRoutine

  ldx CurrentPlayer          ;get which player is on the screen
  lda SavedJoypadBits,x      ;use appropriate player's controller bits
  sta SavedJoypadBits        ;as the master controller bits
  jsr GameRoutines           ;execute one of many possible subs
  lda OperMode_Task          ;check major task of operating mode
  cmp #$03                   ;if we are supposed to be here,
  bcs GameEngine             ;branch to the game engine itself
    rts
GameEngine:
  jsr ProcFireball_Bubble    ;process fireballs and air bubbles
  ldx #$00
ProcELoop:    stx ObjectOffset           ;put incremented offset in X as enemy object offset
  jsr EnemiesAndLoopsCore    ;process enemy objects
  jsr FloateyNumbersRoutine  ;process floatey numbers
  inx
  cpx #$06                   ;do these two subroutines until the whole buffer is done
  bne ProcELoop
  jsr GetPlayerOffscreenBits ;get offscreen bits for player object
  jsr RelativePlayerPosition ;get relative coordinates for player object
  jsr PlayerGfxHandler       ;draw the player
  jsr BlockObjMT_Updater     ;replace block objects with metatiles if necessary
  ldx #$01
  stx ObjectOffset           ;set offset for second
  jsr BlockObjectsCore       ;process second block object
  dex
  stx ObjectOffset           ;set offset for first
  jsr BlockObjectsCore       ;process first block object
  jsr MiscObjectsCore        ;process misc objects (hammer, jumping coins)
  jsr ProcessCannons         ;process bullet bill cannons
  jsr ProcessWhirlpools      ;process whirlpools
  jsr FlagpoleRoutine        ;process the flagpole
  jsr RunGameTimer           ;count down the game timer
  jsr ColorRotation          ;cycle one of the background colors
  lda Player_Y_HighPos
  cmp #$02                   ;if player is below the screen, don't bother with the music
  bpl NoChgMus
  lda StarInvincibleTimer    ;if star mario invincibility timer at zero,
  beq ClrPlrPal              ;skip this part
  cmp #$04
  bne NoChgMus               ;if not yet at a certain point, continue
  lda IntervalTimerControl   ;if interval timer not yet expired,
  bne NoChgMus               ;branch ahead, don't bother with the music
  jsr GetAreaMusic           ;to re-attain appropriate level music
NoChgMus:
  ldy StarInvincibleTimer    ;get invincibility timer
  lda FrameCounter           ;get frame counter
  cpy #$08                   ;if timer still above certain point,
  bcs CycleTwo               ;branch to cycle player's palette quickly
  lsr                        ;otherwise, divide by 8 to cycle every eighth frame
  lsr
CycleTwo:     lsr                        ;if branched here, divide by 2 to cycle every other frame
  jsr CyclePlayerPalette     ;do sub to cycle the palette (note: shares fire flower code)
  jmp SaveAB                 ;then skip this sub to finish up the game engine
ClrPlrPal:
  jsr ResetPalStar           ;do sub to clear player's palette bits in attributes
SaveAB:
  lda A_B_Buttons            ;save current A and B button
  sta PreviousA_B_Buttons    ;into temp variable to be used on next frame
  lda #$00
  sta Left_Right_Buttons     ;nullify left and right buttons temp variable
UpdScrollVar:
  lda VRAM_Buffer_AddrCtrl
  cmp #$06                   ;if vram address controller set to 6 (one of two $0341s)
  beq ExitEng                ;then branch to leave
  lda AreaParserTaskNum      ;otherwise check number of tasks
  bne RunParser
  lda ScrollThirtyTwo        ;get horizontal scroll in 0-31 or $00-$20 range
  cmp #$20                   ;check to see if exceeded $21
  bmi ExitEng                ;branch to leave if not
  lda ScrollThirtyTwo
  sbc #$20                   ;otherwise subtract $20 to set appropriately
  sta ScrollThirtyTwo        ;and store
  lda #$00                   ;reset vram buffer offset used in conjunction with
  sta VRAM_Buffer2_Offset    ;level graphics buffer at $0341-$035f
RunParser:
  jsr AreaParserTaskHandler  ;update the name table with more level graphics
ExitEng:
  rts                        ;and after all that, we're finally done!
.endproc

;-------------------------------------------------------------------------------------

.proc GameRoutines
      lda GameEngineSubroutine  ;run routine based on number (a few of these routines are   
      jsr JumpEngine            ;merely placeholders as conditions for other routines)

      .word Entrance_GameTimerSetup
      .word Vine_AutoClimb
      .word SideExitPipeEntry
      .word VerticalPipeEntry
      .word FlagpoleSlide
      .word PlayerEndLevel
      .word PlayerLoseLife
      .word PlayerEntrance
      .word PlayerCtrlRoutine
      .word PlayerChangeSize
      .word PlayerInjuryBlink
      .word PlayerDeath
      .word PlayerFireFlower
.endproc

;-------------------------------------------------------------------------------------

;page numbers are in order from -1 to -4
HalfwayPageNybbles:
      .byte $56, $40
      .byte $65, $70
      .byte $66, $40
      .byte $66, $40
      .byte $66, $40
      .byte $66, $60
      .byte $65, $70
      .byte $00, $00

.proc PlayerLoseLife
.import TransposePlayers, ContinueGame
  inc DisableScreenFlag    ;disable screen and sprite 0 check
  lda #$00
  sta Sprite0HitDetectFlag
  lda #Silence             ;silence music
  sta EventMusicQueue
  dec NumberofLives        ;take one life from player
  bpl StillInGame          ;if player still has lives, branch
  lda #$00
  sta OperMode_Task        ;initialize mode task,
  lda #GameOverModeValue   ;switch to game over mode
  sta OperMode             ;and leave
  rts
StillInGame:
  lda WorldNumber          ;multiply world number by 2 and use
  asl                      ;as offset
  tax
  lda LevelNumber          ;if in area -3 or -4, increment
  and #$02                 ;offset by one byte, otherwise
  beq GetHalfway           ;leave offset alone
  inx
GetHalfway:
  ldy HalfwayPageNybbles,x ;get halfway page number with offset
  lda LevelNumber          ;check area number's LSB
  lsr
  tya                      ;if in area -2 or -4, use lower nybble
  bcs MaskHPNyb
  lsr                      ;move higher nybble to lower if area
  lsr                      ;number is -1 or -3
  lsr
  lsr
MaskHPNyb:
  and #%00001111           ;mask out all but lower nybble
  cmp ScreenLeft_PageLoc
  beq SetHalfway           ;left side of screen must be at the halfway page,
  bcc SetHalfway           ;otherwise player must start at the
  lda #$00                 ;beginning of the level
SetHalfway:
  sta HalfwayPage          ;store as halfway page for player
  jsr TransposePlayers     ;switch players around if 2-player game
  jmp ContinueGame         ;continue the game
.endproc

PlayerStarting_X_Pos:
      .byte $28, $18
      .byte $38, $28

AltYPosOffset:
      .byte $08, $00

PlayerStarting_Y_Pos:
      .byte $00, $20, $b0, $50, $00, $00, $b0, $b0
      .byte $f0

PlayerBGPriorityData:
      .byte $00, $20, $00, $00, $00, $00, $00, $00

GameTimerData:
      .byte $20 ;dummy byte, used as part of bg priority data
      .byte $04, $03, $02

Entrance_GameTimerSetup:
          lda ScreenLeft_PageLoc      ;set current page for area objects
          sta Player_PageLoc          ;as page location for player
          lda #$28                    ;store value here
          sta VerticalForceDown       ;for fractional movement downwards if necessary
          lda #$01                    ;set high byte of player position and
          sta PlayerFacingDir         ;set facing direction so that player faces right
          sta Player_Y_HighPos
          lda #$00                    ;set player state to on the ground by default
          sta Player_State
          dec Player_CollisionBits    ;initialize player's collision bits
          ldy #$00                    ;initialize halfway page
          sty HalfwayPage      
          lda AreaType                ;check area type
          bne ChkStPos                ;if water type, set swimming flag, otherwise do not set
          iny
ChkStPos: sty SwimmingFlag
          ldx PlayerEntranceCtrl      ;get starting position loaded from header
          ldy AltEntranceControl      ;check alternate mode of entry flag for 0 or 1
          beq SetStPos
          cpy #$01
          beq SetStPos
          ldx AltYPosOffset-2,y       ;if not 0 or 1, override $0710 with new offset in X
SetStPos: lda PlayerStarting_X_Pos,y  ;load appropriate horizontal position
          sta Player_X_Position       ;and vertical positions for the player, using
          lda PlayerStarting_Y_Pos,x  ;AltEntranceControl as offset for horizontal and either $0710
          sta Player_Y_Position       ;or value that overwrote $0710 as offset for vertical
          lda PlayerBGPriorityData,x
          sta Player_SprAttrib        ;set player sprite attributes using offset in X
          jsr GetPlayerColors         ;get appropriate player palette
          ldy GameTimerSetting        ;get timer control value from header
          beq ChkOverR                ;if set to zero, branch (do not use dummy byte for this)
          lda FetchNewGameTimerFlag   ;do we need to set the game timer? if not, use 
          beq ChkOverR                ;old game timer setting
          lda GameTimerData,y         ;if game timer is set and game timer flag is also set,
          sta GameTimerDisplay        ;use value of game timer control for first digit of game timer
          lda #$01
          sta GameTimerDisplay+2      ;set last digit of game timer to 1
          lsr
          sta GameTimerDisplay+1      ;set second digit of game timer
          sta FetchNewGameTimerFlag   ;clear flag for game timer reset
          sta StarInvincibleTimer     ;clear star mario timer
ChkOverR: ldy JoypadOverride          ;if controller bits not set, branch to skip this part
          beq ChkSwimE
          lda #$03                    ;set player state to climbing
          sta Player_State
          ldx #$00                    ;set offset for first slot, for block object
          jsr InitBlock_XY_Pos
          lda #$f0                    ;set vertical coordinate for block object
          sta Block_Y_Position
          ldx #$05                    ;set offset in X for last enemy object buffer slot
          ldy #$00                    ;set offset in Y for object coordinates used earlier
          jsr Setup_Vine              ;do a sub to grow vine
ChkSwimE: ldy AreaType                ;if level not water-type,
          bne SetPESub                ;skip this subroutine
          jsr SetupBubble             ;otherwise, execute sub to set up air bubbles
SetPESub: lda #$07                    ;set to run player entrance subroutine
          sta GameEngineSubroutine    ;on the next frame of game engine
          rts

;-------------------------------------------------------------------------------------
;$02 - used to store offset to block buffer
;$06-$07 - used to store block buffer address
.import ReplaceBlockMetatile
BlockObjMT_Updater:
            ldx #$01                  ;set offset to start with second block object
UpdateLoop: stx ObjectOffset          ;set offset here
            lda VRAM_Buffer1          ;if vram buffer already being used here,
            bne NextBUpd              ;branch to move onto next block object
            lda Block_RepFlag,x       ;if flag for block object already clear,
            beq NextBUpd              ;branch to move onto next block object
            lda Block_BBuf_Low,x      ;get low byte of block buffer
            sta $06                   ;store into block buffer address
            lda #$05
            sta $07                   ;set high byte of block buffer address
            lda Block_Orig_YPos,x     ;get original vertical coordinate of block object
            sta $02                   ;store here and use as offset to block buffer
            tay
            lda Block_Metatile,x      ;get metatile to be written
            sta ($06),y               ;write it to the block buffer
            jsr ReplaceBlockMetatile  ;do sub to replace metatile where block object is
            lda #$00
            sta Block_RepFlag,x       ;clear block object flag
NextBUpd:   dex                       ;decrement block object offset
            bpl UpdateLoop            ;do this until both block objects are dealt with
            rts                       ;then leave
