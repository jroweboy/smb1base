.include "common.inc"
.include "object.inc"
.include "metasprite.inc"

.import SpawnHammerObj, MoveVOffset, RemBridge

.export InitBowserFlame, KillAllEnemies

; frenzy.s
.export PutAtRightExtent

.segment "OBJECT"

;--------------------------------


.proc InitBowser
  jsr DuplicateEnemyObj     ;jump to create another bowser object
  stx BowserFront_Offset    ;save offset of first here
  lda #$00
  sta BowserBodyControls    ;initialize bowser's body controls
  sta BridgeCollapseOffset  ;and bridge collapse offset
  lda Enemy_X_Position,x
  sta BowserOrigXPos        ;store original horizontal position here
  lda #$df
  sta BowserFireBreathTimer ;store something here
  sta Enemy_MovingDir,x     ;and in moving direction
  lda #$20
  sta BowserFeetCounter     ;set bowser's feet timer and in enemy timer
  sta EnemyFrameTimer,x
  lda #$05
  sta BowserHitPoints       ;give bowser 5 hit points
  lsr
  sta BowserMovementSpeed   ;set default movement speed here
  rts
.endproc

;--------------------------------

RunBowserFlame:
  jsr ProcBowserFlame
  jsr GetEnemyOffscreenBits
  jsr RelativeEnemyPosition
  jsr GetEnemyBoundBox
  jsr PlayerEnemyCollision
  jmp OffscreenBoundsCheck

;-------------------------------------------------------------------------------------
;$04-$05 - used to store name table address in little endian order
.export BridgeCollapse
BridgeCollapse:
  ldx #BubbleMetasprite - EnemyMetasprite
  :
    lda EnemyMetasprite,x
    cmp #Bowser
    beq Skip
    lda #0
    sta EnemyMetasprite,x
  Skip:
    dex
    bpl :-

  ldx BowserFront_Offset    ;get enemy offset for bowser
  lda Enemy_ID,x            ;check enemy object identifier for bowser
  cmp #Bowser               ;if not found, branch ahead,
  bne SetM2                 ;metatile removal not necessary
  stx ObjectOffset          ;store as enemy offset here
  lda Enemy_State,x         ;if bowser in normal state, skip all of this
  beq RemoveBridge
  and #%01000000            ;if bowser's state has d6 clear, skip to silence music
  beq SetM2
  lda Enemy_Y_Position,x    ;check bowser's vertical coordinate
  cmp #$e0                  ;if bowser not yet low enough, skip this part ahead
  bcc MoveD_Bowser
SetM2:
  lda #Silence              ;silence music
  sta EventMusicQueue
  inc OperMode_Task         ;move onto next secondary mode in autoctrl mode
  jmp KillAllEnemies        ;jump to empty all enemy slots and then leave  

MoveD_Bowser:
  jsr MoveEnemySlowVert     ;do a sub to move bowser downwards
  jmp BowserGfxHandler      ;jump to draw bowser's front and rear, then leave

RemoveBridge:
  dec BowserFeetCounter     ;decrement timer to control bowser's feet
  bne NoBFall               ;if not expired, skip all of this
  lda #$04
  sta BowserFeetCounter     ;otherwise, set timer now
  lda BowserBodyControls
  eor #$01                  ;invert bit to control bowser's feet
  sta BowserBodyControls
  lda #$22                  ;put high byte of name table address here for now
  sta R5 
  ldy BridgeCollapseOffset  ;get bridge collapse offset here
  lda BridgeCollapseData,y  ;load low byte of name table address and store here
  sta R4 
  ldy VRAM_Buffer1_Offset   ;increment vram buffer offset
  iny
  ldx #$0c                  ;set offset for tile data for sub to draw blank metatile
  jsr RemBridge             ;do sub here to remove bowser's bridge metatiles
  ldx ObjectOffset          ;get enemy offset
  jsr MoveVOffset           ;set new vram buffer offset
  lda #Sfx_Blast            ;load the fireworks/gunfire sound into the square 2 sfx
  sta Square2SoundQueue     ;queue while at the same time loading the brick
  lda #Sfx_BrickShatter     ;shatter sound into the noise sfx queue thus
  sta NoiseSoundQueue       ;producing the unique sound of the bridge collapsing 
  inc BridgeCollapseOffset  ;increment bridge collapse offset
  lda BridgeCollapseOffset
  cmp #$0f                  ;if bridge collapse offset has not yet reached
  bne NoBFall               ;the end, go ahead and skip this part
  jsr InitVStf              ;initialize whatever vertical speed bowser has
  lda #%01000000
  sta Enemy_State,x         ;set bowser's state to one of defeated states (d6 set)
  lda #Sfx_BowserFall
  sta Square2SoundQueue     ;play bowser defeat sound
NoBFall:
  jmp BowserGfxHandler      ;jump to code that draws bowser

;--------------------------------

BridgeCollapseData:
  .byte $1a ;axe
  .byte $58 ;chain
  .byte $98, $96, $94, $92, $90, $8e, $8c ;bridge
  .byte $8a, $88, $86, $84, $82, $80
PRandomRange:
  .byte $21, $41, $11, $31

RunBowser:

  lda Enemy_State,x       ;if d5 in enemy state is not set
  and #%00100000          ;then branch elsewhere to run bowser
  beq BowserControl
  lda Enemy_Y_Position,x  ;otherwise check vertical position
  cmp #$e0                ;if above a certain point, branch to move defeated bowser
  bcc MoveD_Bowser        ;otherwise proceed to KillAllEnemies

KillAllEnemies:
  ldx #$04              ;start with last enemy slot
KillLoop:
    jsr EraseEnemyObject  ;branch to kill enemy objects
    dex                   ;move onto next enemy slot
    bpl KillLoop          ;do this until all slots are emptied
  sta EnemyFrenzyBuffer ;empty frenzy buffer
  ldx ObjectOffset      ;get enemy object offset and leave
  rts

BowserControl:
  lda #$00
  sta EnemyFrenzyBuffer      ;empty frenzy buffer
  lda TimerControl           ;if master timer control not set,
  beq ChkMouth               ;skip jump and execute code here
  jmp SkipToFB               ;otherwise, jump over a bunch of code
ChkMouth:
  lda BowserBodyControls     ;check bowser's mouth
  bpl FeetTmr                ;if bit clear, go ahead with code here
  jmp HammerChk              ;otherwise skip a whole section starting here
FeetTmr:
  dec BowserFeetCounter      ;decrement timer to control bowser's feet
  bne ResetMDr               ;if not expired, skip this part
  lda #$20                   ;otherwise, reset timer
  sta BowserFeetCounter        
  lda BowserBodyControls     ;and invert bit used
  eor #%00000001             ;to control bowser's feet
  sta BowserBodyControls
ResetMDr:
  lda FrameCounter           ;check frame counter
  and #%00001111             ;if not on every sixteenth frame, skip
  bne B_FaceP                ;ahead to continue code
  lda #$02                   ;otherwise reset moving/facing direction every
  sta Enemy_MovingDir,x      ;sixteen frames
B_FaceP:
  lda EnemyFrameTimer,x      ;if timer set here expired,
  beq GetPRCmp               ;branch to next section
  jsr PlayerEnemyDiff        ;get horizontal difference between player and bowser,
  bpl GetPRCmp               ;and branch if bowser to the right of the player
  lda #$01
  sta Enemy_MovingDir,x      ;set bowser to move and face to the right
  lda #$02
  sta BowserMovementSpeed    ;set movement speed
  lda #$20
  sta EnemyFrameTimer,x      ;set timer here
  sta BowserFireBreathTimer  ;set timer used for bowser's flame
  lda Enemy_X_Position,x        
  cmp #$c8                   ;if bowser to the right past a certain point,
  bcs HammerChk              ;skip ahead to some other section
GetPRCmp:
  lda FrameCounter           ;get frame counter
  and #%00000011
  bne HammerChk              ;execute this code every fourth frame, otherwise branch
  lda Enemy_X_Position,x
  cmp BowserOrigXPos         ;if bowser not at original horizontal position,
  bne GetDToO                ;branch to skip this part
  lda PseudoRandomBitReg,x
  and #%00000011             ;get pseudorandom offset
  tay
  lda PRandomRange,y         ;load value using pseudorandom offset
  sta MaxRangeFromOrigin     ;and store here
GetDToO:
  lda Enemy_X_Position,x
  clc                        ;add movement speed to bowser's horizontal
  adc BowserMovementSpeed    ;coordinate and save as new horizontal position
  sta Enemy_X_Position,x
  ldy Enemy_MovingDir,x
  cpy #$01                   ;if bowser moving and facing to the right, skip ahead
  beq HammerChk
  ldy #$ff                   ;set default movement speed here (move left)
  sec                        ;get difference of current vs. original
  sbc BowserOrigXPos         ;horizontal position
  bpl CompDToO               ;if current position to the right of original, skip ahead
  eor #$ff
  clc                        ;get two's compliment
  adc #$01
  ldy #$01                   ;set alternate movement speed here (move right)
CompDToO:
  cmp MaxRangeFromOrigin     ;compare difference with pseudorandom value
  bcc HammerChk              ;if difference < pseudorandom value, leave speed alone
  sty BowserMovementSpeed    ;otherwise change bowser's movement speed
HammerChk:
  lda EnemyFrameTimer,x      ;if timer set here not expired yet, skip ahead to
  bne MakeBJump              ;some other section of code
  jsr MoveEnemySlowVert      ;otherwise start by moving bowser downwards
  lda WorldNumber            ;check world number
  cmp #World6
  bcc SetHmrTmr              ;if world 1-5, skip this part (not time to throw hammers yet)
  lda FrameCounter
  and #%00000011             ;check to see if it's time to execute sub
  bne SetHmrTmr              ;if not, skip sub, otherwise
  jsr SpawnHammerObj         ;execute sub on every fourth frame to spawn misc object (hammer)
SetHmrTmr:
  lda Enemy_Y_Position,x     ;get current vertical position
  cmp #$80                   ;if still above a certain point
  bcc ChkFireB               ;then skip to world number check for flames
  lda PseudoRandomBitReg,x
  and #%00000011             ;get pseudorandom offset
  tay
  lda PRandomRange,y         ;get value using pseudorandom offset
  sta EnemyFrameTimer,x      ;set for timer here
SkipToFB:
  jmp ChkFireB               ;jump to execute flames code
MakeBJump:
  cmp #$01                   ;if timer not yet about to expire,
  bne ChkFireB               ;skip ahead to next part
  dec Enemy_Y_Position,x     ;otherwise decrement vertical coordinate
  jsr InitVStf               ;initialize movement amount
  lda #$fe
  sta Enemy_Y_Speed,x        ;set vertical speed to move bowser upwards
ChkFireB:
  lda WorldNumber            ;check world number here
  cmp #World8                ;world 8?
  beq SpawnFBr               ;if so, execute this part here
  cmp #World6                ;world 6-7?
  bcs BowserGfxHandler       ;if so, skip this part here
SpawnFBr:
  lda BowserFireBreathTimer  ;check timer here
  bne BowserGfxHandler       ;if not expired yet, skip all of this
  lda #$20
  sta BowserFireBreathTimer  ;set timer here
  lda BowserBodyControls
  eor #%10000000             ;invert bowser's mouth bit to open
  sta BowserBodyControls     ;and close bowser's mouth
  bmi ChkFireB               ;if bowser's mouth open, loop back
  jsr SetFlameTimer          ;get timing for bowser's flame
  ldy SecondaryHardMode
  beq SetFBTmr               ;if secondary hard mode flag not set, skip this
  sec
  sbc #$10                   ;otherwise subtract from value in A
SetFBTmr:
  sta BowserFireBreathTimer  ;set value as timer here
  lda #BowserFlame           ;put bowser's flame identifier
  sta EnemyFrenzyBuffer      ;in enemy frenzy buffer


;--------------------------------

BowserGfxHandler:
  jsr ProcessBowserHalf    ;do a sub here to process bowser's front
  ldy #$10                 ;load default value here to position bowser's rear
  lda Enemy_MovingDir,x    ;check moving direction
  lsr
  bcc CopyFToR             ;if moving left, use default
  ldy #$f0                 ;otherwise load alternate positioning value here
CopyFToR:
  tya                      ;move bowser's rear object position value to A
  clc
  adc Enemy_X_Position,x   ;add to bowser's front object horizontal coordinate
  ldy DuplicateObj_Offset  ;get bowser's rear object offset
  sta Enemy_X_Position,y   ;store A as bowser's rear horizontal coordinate
  lda Enemy_Y_Position,x
  clc                      ;add eight pixels to bowser's front object
  adc #$08                 ;vertical coordinate and store as vertical coordinate
  sta Enemy_Y_Position,y   ;for bowser's rear
  lda Enemy_State,x
  sta Enemy_State,y        ;copy enemy state directly from front to rear
  lda Enemy_MovingDir,x
  sta Enemy_MovingDir,y    ;copy moving direction also
  lda ObjectOffset         ;save enemy object offset of front to stack
  pha
  ldx DuplicateObj_Offset  ;put enemy object offset of rear as current
  stx ObjectOffset
  lda #Bowser              ;set bowser's enemy identifier
  sta Enemy_ID,x           ;store in bowser's rear object
  jsr ProcessBowserHalf    ;do a sub here to process bowser's rear
  pla
  sta ObjectOffset         ;get original enemy object offset
  tax
  lda #$00                 ;nullify bowser's front/rear graphics flag
  sta BowserGfxFlag
ExBGfxH:
  rts                      ;leave!

ExitEarly:
  rts

ProcessBowserHalf:
  inc BowserGfxFlag         ;increment bowser's graphics flag, then run subroutines
  jsr ChooseBowserMetasprite
  jsr SprObjectOffscrChk
  jsr GetEnemyOffscreenBits
  jsr RelativeEnemyPosition
  lda Enemy_State,x
  bne ExitEarly ;if either enemy object not in normal state, branch to leave
  lda #$0a
  sta Enemy_BoundBoxCtrl,x  ;set bounding box size control
  jsr GetEnemyBoundBox      ;get bounding box coordinates
  jmp PlayerEnemyCollision  ;do player-to-enemy collision detection

.proc ChooseBowserMetasprite
  ; 1 == drawing front. 2 == drawing rear
  lda BowserGfxFlag
  lsr
  bcs BowserFront
    ; Drawing bowsers rear
    ldy #METASPRITE_BOWSER_REAR_WALK_1
    ; branch if d0 not set (control's bowser's feet)
    lda BowserBodyControls
    and #1
    beq WriteMetasprite
      ldy #METASPRITE_BOWSER_REAR_WALK_2
      bne WriteMetasprite ; unconditional
BowserFront:
    ldy #METASPRITE_BOWSER_FRONT_MOUTH_OPEN
    ;branch if d7 not set (control's bowser's mouth)
    lda BowserBodyControls
    bpl WriteMetasprite
      ldy #METASPRITE_BOWSER_FRONT_MOUTH_CLOSED
WriteMetasprite:
  tya
  sta EnemyMetasprite,x
  lda Enemy_State,x
  and #%00100000
  beq BowserNotDefeated
    ; if bowser is defeated set the vertical flip flag
    lda BowserGfxFlag
    lsr
    lda #MetaspriteOffset{-8} | MSPR_VERTICAL_FLIP
    bcs DontOffsetBowserFrontHalf
      ; when the back side flips, it moves up 16px
      lda #MetaspriteOffset{-24} | MSPR_VERTICAL_FLIP
  DontOffsetBowserFrontHalf:
    sta EnemyVerticalFlip,x
BowserNotDefeated:
  rts
.endproc

;-------------------------------------------------------------------------------------
;$00 - used to hold movement force and tile number
;$01 - used to hold sprite attribute data

FlameTimerData:
      .byte $bf, $40, $bf, $bf, $bf, $40, $40, $bf

SetFlameTimer:
      ldy BowserFlameTimerCtrl  ;load counter as offset
      inc BowserFlameTimerCtrl  ;increment
      lda BowserFlameTimerCtrl  ;mask out all but 3 LSB
      and #%00000111            ;to keep in range of 0-7
      sta BowserFlameTimerCtrl
      lda FlameTimerData,y      ;load value to be used then leave
ExFl: rts

ProcBowserFlame:
         lda TimerControl            ;if master timer control flag set,
         bne SetGfxF                 ;skip all of this
         lda #$40                    ;load default movement force
         ldy SecondaryHardMode
         beq SFlmX                   ;if secondary hard mode flag not set, use default
         lda #$60                    ;otherwise load alternate movement force to go faster
SFlmX:   sta R0                      ;store value here
         lda Enemy_X_MoveForce,x
         sec                         ;subtract value from movement force
         sbc R0 
         sta Enemy_X_MoveForce,x     ;save new value
         lda Enemy_X_Position,x
         sbc #$01                    ;subtract one from horizontal position to move
         sta Enemy_X_Position,x      ;to the left
         lda Enemy_PageLoc,x
         sbc #$00                    ;subtract borrow from page location
         sta Enemy_PageLoc,x
         ldy BowserFlamePRandomOfs,x ;get some value here and use as offset
         lda Enemy_Y_Position,x      ;load vertical coordinate
         cmp FlameYPosData,y         ;compare against coordinate data using $0417,x as offset
         beq SetGfxF                 ;if equal, branch and do not modify coordinate
         clc
         adc Enemy_Y_MoveForce,x     ;otherwise add value here to coordinate and store
         sta Enemy_Y_Position,x      ;as new vertical coordinate
SetGfxF: 
  lda Enemy_State,x
  bne ExFlmeD
    jmp DrawBowserFlame
ExFlmeD: rts                        ;leave

.proc DrawBowserFlame
  ; implicit a == 0
  sta Enemy_SprAttrib,x
  lda FrameCounter
  and #%00000010
  beq :+
    ; invert vertical flip bit every 2 frames
    lda #OAM_FLIP_V
    sta Enemy_SprAttrib,x
  :
  lda #METASPRITE_BOWSER_FLAME
  sta EnemyMetasprite,x
  rts
.endproc

;--------------------------------

FlameYPosData:
  .byte $90, $80, $70, $90

FlameYMFAdderData:
  .byte $ff, $01

InitBowserFlame:
        lda FrenzyEnemyTimer        ;if timer not expired yet, branch to leave
        bne ExFlmeD
        sta Enemy_Y_MoveForce,x     ;reset something here
        lda NoiseSoundQueue
        ora #Sfx_BowserFlame        ;load bowser's flame sound into queue
        sta NoiseSoundQueue
        ldy BowserFront_Offset      ;get bowser's buffer offset
        lda Enemy_ID,y              ;check for bowser
        cmp #Bowser
        beq SpawnFromMouth          ;branch if found
        jsr SetFlameTimer           ;get timer data based on flame counter
        clc
        adc #$20                    ;add 32 frames by default
        ldy SecondaryHardMode
        beq SetFrT                  ;if secondary mode flag not set, use as timer setting
        sec
        sbc #$10                    ;otherwise subtract 16 frames for secondary hard mode
SetFrT: sta FrenzyEnemyTimer        ;set timer accordingly
        lda PseudoRandomBitReg,x
        and #%00000011              ;get 2 LSB from first part of LSFR
        sta BowserFlamePRandomOfs,x ;set here
        tay                         ;use as offset
        lda FlameYPosData,y         ;load vertical position based on pseudorandom offset

PutAtRightExtent:
      sta Enemy_Y_Position,x    ;set vertical position
      lda ScreenRight_X_Pos
      clc
      adc #$20                  ;place enemy 32 pixels beyond right side of screen
      sta Enemy_X_Position,x
      lda ScreenRight_PageLoc
      adc #$00                  ;add carry
      sta Enemy_PageLoc,x
      jmp FinishFlame           ;skip this part to finish setting values

SpawnFromMouth:
       lda Enemy_X_Position,y    ;get bowser's horizontal position
       sec
       sbc #$0e                  ;subtract 14 pixels
       sta Enemy_X_Position,x    ;save as flame's horizontal position
       lda Enemy_PageLoc,y
       sta Enemy_PageLoc,x       ;copy page location from bowser to flame
       lda Enemy_Y_Position,y
       clc                       ;add 8 pixels to bowser's vertical position
       adc #$08
       sta Enemy_Y_Position,x    ;save as flame's vertical position
       lda PseudoRandomBitReg,x
       and #%00000011            ;get 2 LSB from first part of LSFR
       sta Enemy_YMoveForceFractional,x     ;save here
       tay                       ;use as offset
       lda FlameYPosData,y       ;get value here using bits as offset
       ldy #$00                  ;load default offset
       cmp Enemy_Y_Position,x    ;compare value to flame's current vertical position
       bcc SetMF                 ;if less, do not increment offset
       iny                       ;otherwise increment now
SetMF: lda FlameYMFAdderData,y   ;get value here and save
       sta Enemy_Y_MoveForce,x   ;to vertical movement force
       lda #$00
       sta EnemyFrenzyBuffer     ;clear enemy frenzy buffer

FinishFlame:
      lda #$08                 ;set $08 for bounding box control
      sta Enemy_BoundBoxCtrl,x
      lda #$01                 ;set high byte of vertical and
      sta Enemy_Y_HighPos,x    ;enemy buffer flag
      sta Enemy_Flag,x
      lsr
      sta Enemy_X_MoveForce,x  ;initialize horizontal movement force, and
      sta Enemy_State,x        ;enemy state
      rts
