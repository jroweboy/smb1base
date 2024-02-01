
.include "common.inc"
.include "player.inc"

; objects/object.s
.import MiscObjectsCore, ProcessSingleEnemy, EnemiesAndLoopsCore, FloateyNumbersCore
; objects/cannon.s
.import ProcessCannons
; music.s
.import GetAreaMusic
; player.s
.import GameRoutines
.import ResetPalStar ; TODO likely relocatable
; screen_render.s
.import AreaParserTaskHandler
.import DrawBrickChunks
.import DrawBlock
.import DigitsMathRoutine
.import PrintStatusBarNumbers
.import AddToScore
.import FlagpoleGfxHandler
; collision.s
.import ForceInjury

; gamemode.s
.export GameCoreRoutine, UpdScrollVar

.segment "CODE"

;-------------------------------------------------------------------------------------
GameCoreRoutine:
  ldx CurrentPlayer          ;get which player is on the screen
  lda SavedJoypadBits,x      ;use appropriate player's controller bits
  sta SavedJoypadBits        ;as the master controller bits
  
  farcall GameRoutines           ;execute one of many possible subs

  lda #0
  sta PlayerOAMOffset
  lda #4 * 4 ; save enough room to draw the player first later
  sta CurrentOAMOffset

  lda OperMode_Task          ;check major task of operating mode
  cmp #$03                   ;if we are supposed to be here,
  bcs GameEngine             ;branch to the game engine itself
    rts
GameEngine:
  far OBJECT
    jsr ProcFireball_Bubble    ;process fireballs and air bubbles
    ldx #$00
ProcELoop:
      stx ObjectOffset           ;put incremented offset in X as enemy object offset
      jsr EnemiesAndLoopsCore    ;process enemy objects
      jsr FloateyNumbersCore  ;process floatey numbers
      inx
      cpx #$06                   ;do these two subroutines until the whole buffer is done
    bne ProcELoop
    jsr GetPlayerOffscreenBits ;get offscreen bits for player object
    jsr RelativePlayerPosition ;get relative coordinates for player object
    farcall PlayerGfxHandler       ;draw the player
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
  endfar

  lda ShouldSkipDrawSprites
  bne SkipDrawingCauseLagged
    farcall DrawAllMetasprites
SkipDrawingCauseLagged:
  lda #0
  sta ShouldSkipDrawSprites

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
          jsr GetAreaMusic       ;to re-attain appropriate level music
NoChgMus:
  ldy StarInvincibleTimer    ;get invincibility timer
  lda FrameCounter           ;get frame counter
  cpy #$08                   ;if timer still above certain point,
  bcs CycleTwo               ;branch to cycle player's palette quickly
    lsr                      ;otherwise, divide by 8 to cycle every eighth frame
    lsr
CycleTwo:
  lsr                        ;if branched here, divide by 2 to cycle every other frame
  sta R0 
  farcall CyclePlayerPalettePreload     ;do sub to cycle the palette (note: shares fire flower code)
  jmp SaveAB                 ;then skip this sub to finish up the game engine
ClrPlrPal:
  farcall ResetPalStar           ;do sub to clear player's palette bits in attributes
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
        farcall AreaParserTaskHandler, jmp  ;update the name table with more level graphics
ExitEng:
  rts                        ;and after all that, we're finally done!


;-------------------------------------------------------------------------------------
;$02 - used to store offset to block buffer
;$06-$07 - used to store block buffer address
.import WriteBlockMetatile
BlockObjMT_Updater:
  ldx #$01                  ;set offset to start with second block object
UpdateLoop:
    stx ObjectOffset          ;set offset here
    lda VRAM_Buffer1          ;if vram buffer already being used here,
    bne NextBUpd              ;branch to move onto next block object
    lda Block_RepFlag,x       ;if flag for block object already clear,
    beq NextBUpd              ;branch to move onto next block object
      lda Block_BBuf_Low,x      ;get low byte of block buffer
      sta R6                    ;store into block buffer address
      lda #>Block_Buffer_1
      sta R7                    ;set high byte of block buffer address
      lda Block_Orig_YPos,x     ;get original vertical coordinate of block object
      sta R2                    ;store here and use as offset to block buffer
      tay
      lda Block_Metatile,x      ;get metatile to be written
      sta (R6),y                ;write it to the block buffer
      jsr WriteBlockMetatile  ;do sub to replace metatile where block object is
      lda #$00
      sta Block_RepFlag,x       ;clear block object flag
NextBUpd:
    dex                       ;decrement block object offset
    bpl UpdateLoop            ;do this until both block objects are dealt with
  rts                       ;then leave

;-------------------------------------------------------------------------------------

; Because the metasprite is rendered after, we need to keep the block around for one
; extra frame in order to clear the metasprite on the same frame it would in vanilla
EliminateBlock:
  lda #0
  sta BlockMetasprite,x
  sta Block_State,x
  rts

BlockObjectsCore:
        lda Block_State,x           ;get state of block object
        bmi EliminateBlock
        beq UpdSte                  ;if not set, branch to leave
        and #$0f                    ;mask out high nybble
        pha                         ;push to stack
        tay                         ;put in Y for now
        txa
        clc
        adc #Block_Y_Speed - SprObject_Y_Speed  ;add 9 bytes to offset (note two block objects are created
        tax                         ;when using brick chunks, but only one offset for both)
        dey                         ;decrement Y to check for solid block state
        beq BouncingBlockHandler    ;branch if found, otherwise continue for brick chunks
        jsr ImposeGravityBlock      ;do sub to impose gravity on one block object object
        jsr MoveObjectHorizontally  ;do another sub to move horizontally
        ; txa
        ; clc                         ;move onto next block object
        ; adc #$02
        ; tax
        inx
        inx
        jsr ImposeGravityBlock      ;do sub to impose gravity on other block object
        jsr MoveObjectHorizontally  ;do another sub to move horizontally
        ldx ObjectOffset            ;get block object offset used for both
        jsr RelativeBlockPosition   ;get relative coordinates
        jsr GetBlockOffscreenBits   ;get offscreen information
        jsr DrawBrickChunks         ;draw the brick chunks
        pla                         ;get lower nybble of saved state
        ldy Block_Y_HighPos,x       ;check vertical high byte of block object
        beq UpdSte                  ;if above the screen, branch to kill it
        pha                         ;otherwise save state back into stack
        lda #$f0
        cmp Block_Y_Position+2,x    ;check to see if bottom block object went
        bcs ChkTop                  ;to the bottom of the screen, and branch if not
        sta Block_Y_Position+2,x    ;otherwise set offscreen coordinate
ChkTop: lda Block_Y_Position,x      ;get top block object's vertical coordinate
        cmp #$f0                    ;see if it went to the bottom of the screen
        pla                         ;pull block object state from stack
        bcc UpdSte                  ;if not, branch to save state
        bcs KillBlock               ;otherwise do unconditional branch to kill it

BouncingBlockHandler:
           jsr ImposeGravityBlock     ;do sub to impose gravity on block object
           ldx ObjectOffset           ;get block object offset
           jsr RelativeBlockPosition  ;get relative coordinates
           jsr GetBlockOffscreenBits  ;get offscreen information
           jsr DrawBlock              ;draw the block
           lda Block_Y_Position,x     ;get vertical coordinate
           and #$0f                   ;mask out high nybble
           cmp #$05                   ;check to see if low nybble wrapped around
           pla                        ;pull state from stack
           bcs UpdSte                 ;if still above amount, not time to kill block yet, thus branch
           lda #$01
           sta Block_RepFlag,x        ;otherwise set flag to replace metatile
KillBlock: lda #$80                   ;if branched here, nullify object state
           bne UpdSte
KeepBlock: lda #1
UpdSte:    sta Block_State,x          ;store contents of A in block object state
           rts

;-------------------------------------------------------------------------------------

RunGameTimer:
  lda OperMode               ;get primary mode of operation
  beq ExGTimer               ;branch to leave if in title screen mode
  lda GameEngineSubroutine
  cmp #$08                   ;if routine number less than eight running,
  bcc ExGTimer               ;branch to leave
  cmp #$0b                   ;if running death routine,
  beq ExGTimer               ;branch to leave
  lda Player_Y_HighPos
  cmp #$02                   ;if player below the screen,
  bcs ExGTimer               ;branch to leave regardless of level type
  lda GameTimerCtrlTimer     ;if game timer control not yet expired,
  bne ExGTimer               ;branch to leave
  lda GameTimerDisplay
  ora GameTimerDisplay+1     ;otherwise check game timer digits
  ora GameTimerDisplay+2
  beq TimeUpOn               ;if game timer digits at 000, branch to time-up code
  ldy GameTimerDisplay       ;otherwise check first digit
  dey                        ;if first digit not on 1,
  bne ResGTCtrl              ;branch to reset game timer control
  lda GameTimerDisplay+1     ;otherwise check second and third digits
  ora GameTimerDisplay+2
  bne ResGTCtrl              ;if timer not at 100, branch to reset game timer control
  lda #TimeRunningOutMusic
  sta EventMusicQueue        ;otherwise load time running out music
ResGTCtrl:
  lda #$18                   ;reset game timer control
  sta GameTimerCtrlTimer
  ldy #(GameTimerDisplay + 2 - DisplayDigits) ;set offset for last digit
  lda #-1                    ;set value to decrement game timer digit
  sta DigitModifier+5
  jsr DigitsMathRoutine      ;do sub to decrement game timer slowly
  lda #$a4                   ;set status nybbles to update game timer display
  jmp PrintStatusBarNumbers  ;do sub to update the display
TimeUpOn:
  sta PlayerStatus           ;init player status (note A will always be zero here)
  jsr ForceInjury            ;do sub to kill the player (note player is small here)
  inc GameTimerExpiredFlag   ;set game timer expiration flag
ExGTimer:
  rts                        ;leave

;-------------------------------------------------------------------------------------
;$00 - used in WhirlpoolActivate to store whirlpool length / 2, page location of center of whirlpool
;and also to store movement force exerted on player
;$01 - used in ProcessWhirlpools to store page location of right extent of whirlpool
;and in WhirlpoolActivate to store center of whirlpool
;$02 - used in ProcessWhirlpools to store right extent of whirlpool and in
;WhirlpoolActivate to store maximum vertical speed

ProcessWhirlpools:
        lda AreaType                ;check for water type level
        bne ExitWh                  ;branch to leave if not found
        sta Whirlpool_Flag          ;otherwise initialize whirlpool flag
        lda TimerControl            ;if master timer control set,
        bne ExitWh                  ;branch to leave
        ldy #$04                    ;otherwise start with last whirlpool data
WhLoop: lda Whirlpool_LeftExtent,y  ;get left extent of whirlpool
        clc
        adc Whirlpool_Length,y      ;add length of whirlpool
        sta R2                      ;store result as right extent here
        lda Whirlpool_PageLoc,y     ;get page location
        beq NextWh                  ;if none or page 0, branch to get next data
        adc #$00                    ;add carry
        sta R1                      ;store result as page location of right extent here
        lda Player_X_Position       ;get player's horizontal position
        sec
        sbc Whirlpool_LeftExtent,y  ;subtract left extent
        lda Player_PageLoc          ;get player's page location
        sbc Whirlpool_PageLoc,y     ;subtract borrow
        bmi NextWh                  ;if player too far left, branch to get next data
        lda R2                      ;otherwise get right extent
        sec
        sbc Player_X_Position       ;subtract player's horizontal coordinate
        lda R1                      ;get right extent's page location
        sbc Player_PageLoc          ;subtract borrow
        bpl WhirlpoolActivate       ;if player within right extent, branch to whirlpool code
NextWh: dey                         ;move onto next whirlpool data
        bpl WhLoop                  ;do this until all whirlpools are checked
ExitWh: rts                         ;leave

WhirlpoolActivate:
        lda Whirlpool_Length,y      ;get length of whirlpool
        lsr                         ;divide by 2
        sta R0                      ;save here
        lda Whirlpool_LeftExtent,y  ;get left extent of whirlpool
        clc
        adc R0                      ;add length divided by 2
        sta R1                      ;save as center of whirlpool
        lda Whirlpool_PageLoc,y     ;get page location
        adc #$00                    ;add carry
        sta R0                      ;save as page location of whirlpool center
        lda FrameCounter            ;get frame counter
        lsr                         ;shift d0 into carry (to run on every other frame)
        bcc WhPull                  ;if d0 not set, branch to last part of code
        lda R1                      ;get center
        sec
        sbc Player_X_Position       ;subtract player's horizontal coordinate
        lda R0                      ;get page location of center
        sbc Player_PageLoc          ;subtract borrow
        bpl LeftWh                  ;if player to the left of center, branch
        lda Player_X_Position       ;otherwise slowly pull player left, towards the center
        sec
        sbc #$01                    ;subtract one pixel
        sta Player_X_Position       ;set player's new horizontal coordinate
        lda Player_PageLoc
        sbc #$00                    ;subtract borrow
        jmp SetPWh                  ;jump to set player's new page location
LeftWh: lda Player_CollisionBits    ;get player's collision bits
        lsr                         ;shift d0 into carry
        bcc WhPull                  ;if d0 not set, branch
        lda Player_X_Position       ;otherwise slowly pull player right, towards the center
        clc
        adc #$01                    ;add one pixel
        sta Player_X_Position       ;set player's new horizontal coordinate
        lda Player_PageLoc
        adc #$00                    ;add carry
SetPWh: sta Player_PageLoc          ;set player's new page location
WhPull: lda #$10
        sta R0                      ;set vertical movement force
        lda #$01
        sta Whirlpool_Flag          ;set whirlpool flag to be used later
        sta R2                      ;also set maximum vertical speed
        lsr
        tax                         ;set X for player offset
        jmp ImposeGravity           ;jump to put whirlpool effect on player vertically, do not return

;-------------------------------------------------------------------------------------

FlagpoleScoreMods:
  .byte $05, $02, $08, $04, $01

FlagpoleScoreDigits:
  .byte $03, $03, $04, $04, $04

FlagpoleRoutine:
           ldx #$05                  ;set enemy object offset
           stx ObjectOffset          ;to special use slot
           lda Enemy_ID,x
           cmp #FlagpoleFlagObject   ;if flagpole flag not found,
           bne ExitFlagP             ;branch to leave
           lda GameEngineSubroutine
           cmp #$04                  ;if flagpole slide routine not running,
           bne SkipScore             ;branch to near the end of code
           lda Player_State
           cmp #$03                  ;if player state not climbing,
           bne SkipScore             ;branch to near the end of code
           lda Enemy_Y_Position,x    ;check flagpole flag's vertical coordinate
           cmp #$aa                  ;if flagpole flag down to a certain point,
           bcs GiveFPScr             ;branch to end the level
           lda Player_Y_Position     ;check player's vertical coordinate
           cmp #$a2                  ;if player down to a certain point,
           bcs GiveFPScr             ;branch to end the level
           lda Enemy_YMoveForceFractional,x
           adc #$ff                  ;add movement amount to dummy variable
           sta Enemy_YMoveForceFractional,x     ;save dummy variable
           lda Enemy_Y_Position,x    ;get flag's vertical coordinate
           adc #$01                  ;add 1 plus carry to move flag, and
           sta Enemy_Y_Position,x    ;store vertical coordinate
           lda FlagpoleFNum_YMFDummy
           sec                       ;subtract movement amount from dummy variable
           sbc #$ff
           sta FlagpoleFNum_YMFDummy ;save dummy variable
           lda FlagpoleFNum_Y_Pos
           sbc #$01                  ;subtract one plus borrow to move floatey number,
           sta FlagpoleFNum_Y_Pos    ;and store vertical coordinate here
SkipScore: jmp FPGfx                 ;jump to skip ahead and draw flag and floatey number
GiveFPScr: ldy FlagpoleScore         ;get score offset from earlier (when player touched flagpole)
           lda FlagpoleScoreMods,y   ;get amount to award player points
           ldx FlagpoleScoreDigits,y ;get digit with which to award points
           sta DigitModifier,x       ;store in digit modifier
           jsr AddToScore            ;do sub to award player points depending on height of collision
           lda #$05
           sta GameEngineSubroutine  ;set to run end-of-level subroutine on next frame
FPGfx:     ;jsr GetEnemyOffscreenBits ;get offscreen information
           ;jsr RelativeEnemyPosition ;get relative coordinates
           jmp FlagpoleGfxHandler    ;draw flagpole flag and floatey number
ExitFlagP: rts ; TODO check this RTS can be removed

;-------------------------------------------------------------------------------------

;$00 - used as temporary counter in ColorRotation

ColorRotatePalette:
       .byte $27, $27, $27, $17, $07, $17

BlankPalette:
       .byte $3f, $0c, $04, $ff, $ff, $ff, $ff, $00

;used based on area type
Palette3Data:
       .byte $0f, $07, $12, $0f 
       .byte $0f, $07, $17, $0f
       .byte $0f, $07, $17, $1c
       .byte $0f, $07, $17, $00

ColorRotation:
              lda FrameCounter         ;get frame counter
              and #$07                 ;mask out all but three LSB
              bne ExitColorRot         ;branch if not set to zero to do this every eighth frame
              ldx VRAM_Buffer1_Offset  ;check vram buffer offset
              cpx #$31
              bcs ExitColorRot         ;if offset over 48 bytes, branch to leave
              tay                      ;otherwise use frame counter's 3 LSB as offset here
GetBlankPal:  lda BlankPalette,y       ;get blank palette for palette 3
              sta VRAM_Buffer1,x       ;store it in the vram buffer
              inx                      ;increment offsets
              iny
              cpy #$08
              bcc GetBlankPal          ;do this until all bytes are copied
              ldx VRAM_Buffer1_Offset  ;get current vram buffer offset
              lda #$03
              sta R0                   ;set counter here
              lda AreaType             ;get area type
              asl                      ;multiply by 4 to get proper offset
              asl
              tay                      ;save as offset here
GetAreaPal:   lda Palette3Data,y       ;fetch palette to be written based on area type
              sta VRAM_Buffer1+3,x     ;store it to overwrite blank palette in vram buffer
              iny
              inx
              dec R0                   ;decrement counter
              bpl GetAreaPal           ;do this until the palette is all copied
              ldx VRAM_Buffer1_Offset  ;get current vram buffer offset
              ldy ColorRotateOffset    ;get color cycling offset
              lda ColorRotatePalette,y
              sta VRAM_Buffer1+4,x     ;get and store current color in second slot of palette
              lda VRAM_Buffer1_Offset
              clc                      ;add seven bytes to vram buffer offset
              adc #$07
              sta VRAM_Buffer1_Offset
              inc ColorRotateOffset    ;increment color cycling offset
              lda ColorRotateOffset
              cmp #$06                 ;check to see if it's still in range
              bcc ExitColorRot         ;if so, branch to leave
              lda #$00
              sta ColorRotateOffset    ;otherwise, init to keep it in range
ExitColorRot: rts                      ;leave
