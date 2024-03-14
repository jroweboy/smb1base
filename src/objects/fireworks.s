
.include "common.inc"
.include "object.inc"
.include "metasprite.inc"

; sprite_render.s
.import DrawExplosion_Fireworks, DigitsMathRoutine, UpdateNumber

.segment "OBJECT"

;--------------------------------

.import ExplosionTiles
RunFireworks:
  dec ExplosionTimerCounter,x ;decrement explosion timing counter here
  bne SetupExpl               ;if not expired, skip this part
    lda #$08
    sta ExplosionTimerCounter,x ;reset counter
    inc ExplosionGfxCounter,x   ;increment explosion graphics counter
    lda ExplosionGfxCounter,x
    cmp #$03                    ;check explosion graphics counter
    bcs FireworksSoundScore     ;if at a certain point, branch to kill this object
SetupExpl:
  ; jsr RelativeEnemyPosition   ;get relative coordinates of explosion
  ; lda Enemy_Rel_YPos          ;copy relative coordinates
  ; sta Fireball_Rel_YPos       ;from the enemy object to the fireball object
  ; lda Enemy_Rel_XPos          ;first vertical, then horizontal
  ; sta Fireball_Rel_XPos
;      ldy Enemy_SprDataOffset,x   ;get OAM data offset
  ; AllocSpr 4
  ldy ExplosionGfxCounter,x   ;get explosion graphics counter
  lda ExplosionTiles,y        ;get tile number using offset
  sta EnemyMetasprite,x
  ; prevent rotation of the fireball from bleeding into the explosion
  lda #0
  sta Enemy_SprAttrib,x
  rts
  ; jmp DrawExplosion_Fireworks ;do a sub to draw the explosion then leave
FireworksSoundScore:
  lda #$00               ;disable enemy buffer flag
  sta Enemy_Flag,x
  sta EnemyMetasprite,x
  lda #Sfx_Blast         ;play fireworks/gunfire sound
  sta Square2SoundQueue
  lda #$05               ;set part of score modifier for 500 points
  sta DigitModifier+4
  jmp EndAreaPoints     ;jump to award points accordingly then leave

;--------------------------------

; StarFlagYPosAdder:
;   .byte $00, $00, $08, $08

; StarFlagXPosAdder:
;   .byte $00, $08, $00, $08

; StarFlagTileData:
;   .byte STAR_FLAG_TOP_LEFT, STAR_FLAG_TOP_RIGHT, STAR_FLAG_BOT_LEFT, STAR_FLAG_BOT_RIGHT

RunStarFlagObj:
      lda #$00                 ;initialize enemy frenzy buffer
      sta EnemyFrenzyBuffer
      lda StarFlagTaskControl  ;check star flag object task number here
      cmp #$05                 ;if greater than 5, branch to exit
      bcs StarFlagExit
      jsr JumpEngine           ;otherwise jump to appropriate sub
      
      .word StarFlagExit
      .word GameTimerFireworks
      .word AwardGameTimerPoints
      .word RaiseFlagSetoffFWorks
      .word DelayToAreaEnd

GameTimerFireworks:
        ldy #$05               ;set default state for star flag object
        lda GameTimerDisplay+2 ;get game timer's last digit
        cmp #$01
        beq SetFWC             ;if last digit of game timer set to 1, skip ahead
        ldy #$03               ;otherwise load new value for state
        cmp #$03
        beq SetFWC             ;if last digit of game timer set to 3, skip ahead
        ldy #$00               ;otherwise load one more potential value for state
        cmp #$06
        beq SetFWC             ;if last digit of game timer set to 6, skip ahead
        lda #$ff               ;otherwise set value for no fireworks
SetFWC: sta FireworksCounter   ;set fireworks counter here
        sty Enemy_State,x      ;set whatever state we have in star flag object

IncrementSFTask1:
      inc StarFlagTaskControl  ;increment star flag object task number

StarFlagExit:
      rts                      ;leave

AwardGameTimerPoints:
         lda GameTimerDisplay   ;check all game timer digits for any intervals left
         ora GameTimerDisplay+1
         ora GameTimerDisplay+2
         beq IncrementSFTask1   ;if no time left on game timer at all, branch to next task
         lda FrameCounter
         and #%00000100         ;check frame counter for d2 set (skip ahead
         beq NoTTick            ;for four frames every four frames) branch if not set
         lda #Sfx_TimerTick
         sta Square2SoundQueue  ;load timer tick sound
NoTTick: ldy #$23               ;set offset here to subtract from game timer's last digit
         lda #$ff               ;set adder here to $ff, or -1, to subtract one
         sta DigitModifier+5    ;from the last digit of the game timer
         jsr DigitsMathRoutine  ;subtract digit
         lda #$05               ;set now to add 50 points
         sta DigitModifier+5    ;per game timer interval subtracted
EndAreaPoints:
         ldy #$0b               ;load offset for mario's score by default
         lda CurrentPlayer      ;check player on the screen
         beq ELPGive            ;if mario, do not change
         ldy #$11               ;otherwise load offset for luigi's score
ELPGive: jsr DigitsMathRoutine  ;award 50 points per game timer interval
         lda CurrentPlayer      ;get player on the screen (or 500 points per
         asl                    ;fireworks explosion if branched here from there)
         asl                    ;shift to high nybble
         asl
         asl
         ora #%00000100         ;add four to set nybble for game timer
         jmp UpdateNumber       ;jump to print the new score and game timer

RaiseFlagSetoffFWorks:
         lda Enemy_Y_Position,x  ;check star flag's vertical position
         cmp #$72                ;against preset value
         bcc SetoffF             ;if star flag higher vertically, branch to other code
         dec Enemy_Y_Position,x  ;otherwise, raise star flag by one pixel
         jmp DrawStarFlag        ;and skip this part here
SetoffF: lda FireworksCounter    ;check fireworks counter
         beq DrawFlagSetTimer    ;if no fireworks left to go off, skip this part
         bmi DrawFlagSetTimer    ;if no fireworks set to go off, skip this part
         lda #Fireworks
         sta EnemyFrenzyBuffer   ;otherwise set fireworks object in frenzy queue

DrawStarFlag:
;   jsr RelativeEnemyPosition  ;get relative coordinates of star flag
;   ReserveSpr 4
;   ldx #$03                   ;do four sprites
; DSFLoop:
;     lda Enemy_Rel_YPos         ;get relative vertical coordinate
;     clc
;     adc StarFlagYPosAdder,x    ;add Y coordinate adder data
;     sta Sprite_Y_Position,y    ;store as Y coordinate
;     lda StarFlagTileData,x     ;get tile number
;     sta Sprite_Tilenumber,y    ;store as tile number
;     lda #$22                   ;set palette and background priority bits
;     sta Sprite_Attributes,y    ;store as attributes
;     lda Enemy_Rel_XPos         ;get relative horizontal coordinate
;     clc
;     adc StarFlagXPosAdder,x    ;add X coordinate adder data
;     sta Sprite_X_Position,y    ;store as X coordinate
;     iny
;     iny                        ;increment OAM data offset four bytes
;     iny                        ;for next sprite
;     iny
;     dex                        ;move onto next sprite
;     bpl DSFLoop                ;do this until all sprites are done
;   UpdateOAMPosition
;   ldx ObjectOffset           ;get enemy object offset and leave
  lda #METASPRITE_MISC_STAR_FLAG
  sta EnemyMetasprite,x
  lda #OAM_BACKGROUND_PRIORTY
  sta Enemy_SprAttrib,x
  lda #1
  sta Enemy_MovingDir,x
  rts

DrawFlagSetTimer:
  jsr DrawStarFlag          ;do sub to draw star flag
  lda #$06
  sta EnemyIntervalTimer,x  ;set interval timer here

IncrementSFTask2:
  inc StarFlagTaskControl   ;move onto next task
  rts

DelayToAreaEnd:
  jsr DrawStarFlag          ;do sub to draw star flag
  lda EnemyIntervalTimer,x  ;if interval timer set in previous task
  bne StarFlagExit2         ;not yet expired, branch to leave
  lda EventMusicBuffer      ;if event music buffer empty,
  beq IncrementSFTask2      ;branch to increment task

StarFlagExit2:
  rts                       ;otherwise leave
