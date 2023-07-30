
.include "common.inc"
.include "object.inc"

; sprite_render.s
.import DrawExplosion_Fireworks, DigitsMathRoutine, UpdateNumber

.segment "OBJECT"

;--------------------------------

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
  jsr RelativeEnemyPosition   ;get relative coordinates of explosion
  lda Enemy_Rel_YPos          ;copy relative coordinates
  sta Fireball_Rel_YPos       ;from the enemy object to the fireball object
  lda Enemy_Rel_XPos          ;first vertical, then horizontal
  sta Fireball_Rel_XPos
;      ldy Enemy_SprDataOffset,x   ;get OAM data offset
  lda ExplosionGfxCounter,x   ;get explosion graphics counter
  jmp DrawExplosion_Fireworks ;do a sub to draw the explosion then leave

FireworksSoundScore:
  lda #$00               ;disable enemy buffer flag
  sta Enemy_Flag,x
  lda #Sfx_Blast         ;play fireworks/gunfire sound
  sta Square2SoundQueue
  ; lda #$05               ;set part of score modifier for 500 points
  ; sta DigitModifier+4
  ; jmp EndAreaPoints     ;jump to award points accordingly then leave
  rts

;--------------------------------

StarFlagYPosAdder:
  .byte $00, $00, $08, $08

StarFlagXPosAdder:
  .byte $00, $08, $00, $08

StarFlagTileData:
  .byte STAR_FLAG_TOP_LEFT, STAR_FLAG_TOP_RIGHT, STAR_FLAG_BOT_LEFT, STAR_FLAG_BOT_RIGHT
  ; .byte $54, $55, $56, $57

RunStarFlagObj:
  lda #$00                 ;initialize enemy frenzy buffer
  ; force to clear the lakitu buffer on level end so he doesn't mess with fireworks
  sta LakituActionBuffer
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
  ; ldy #$05               ;set default state for star flag object
  ; lda GameTimerDisplay+2 ;get game timer's last digit
  ; cmp #$01
  ; beq SetFWC             ;if last digit of game timer set to 1, skip ahead
  ; ldy #$03               ;otherwise load new value for state
  ; cmp #$03
  ; beq SetFWC             ;if last digit of game timer set to 3, skip ahead
  ; ldy #$00               ;otherwise load one more potential value for state
  ; cmp #$06
  ; beq SetFWC             ;if last digit of game timer set to 6, skip ahead
  lda #$ff               ;otherwise set value for no fireworks
SetFWC:
  sta FireworksCounter   ;set fireworks counter here
  sty Enemy_State,x      ;set whatever state we have in star flag object
  ; jroweboy set a little delay for the award timer so it doesn't end too soon
  lda #8
  sta EnemyIntervalTimer,x
  
IncrementSFTask1:
  inc StarFlagTaskControl  ;increment star flag object task number
StarFlagExit:
  rts                      ;leave

AwardGameTimerPoints: ; jroweboy skip adding gametimer points
  lda EnemyIntervalTimer, x
  beq IncrementSFTask1
  rts

RaiseFlagSetoffFWorks:
  lda Enemy_Y_Position,x  ;check star flag's vertical position
  cmp #$72                ;against preset value
  bcc SetoffF             ;if star flag higher vertically, branch to other code
  dec Enemy_Y_Position,x  ;otherwise, raise star flag by one pixel
  jmp DrawStarFlag        ;and skip this part here
SetoffF:
  lda FireworksCounter    ;check fireworks counter
  beq DrawFlagSetTimer    ;if no fireworks left to go off, skip this part
  bmi DrawFlagSetTimer    ;if no fireworks set to go off, skip this part
  lda #Fireworks
  sta EnemyFrenzyBuffer   ;otherwise set fireworks object in frenzy queue

DrawStarFlag:
  jsr RelativeEnemyPosition  ;get relative coordinates of star flag
      ;    ldy Enemy_SprDataOffset,x  ;get OAM data offset
ReserveSpr 4
  ldx #$03                   ;do four sprites
DSFLoop:
    lda Enemy_Rel_YPos         ;get relative vertical coordinate
    clc
    adc StarFlagYPosAdder,x    ;add Y coordinate adder data
    sta Sprite_Y_Position,y    ;store as Y coordinate
    lda StarFlagTileData,x     ;get tile number
    sta Sprite_Tilenumber,y    ;store as tile number
    lda #$22                   ;set palette and background priority bits
    sta Sprite_Attributes,y    ;store as attributes
    lda Enemy_Rel_XPos         ;get relative horizontal coordinate
    clc
    adc StarFlagXPosAdder,x    ;add X coordinate adder data
    sta Sprite_X_Position,y    ;store as X coordinate
    iny
    iny                        ;increment OAM data offset four bytes
    iny                        ;for next sprite
    iny
    dex                        ;move onto next sprite
    bpl DSFLoop                ;do this until all sprites are done
UpdateOAMPosition
  ldx ObjectOffset           ;get enemy object offset and leave
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
  jmp IncrementSFTask2
  ; lda EventMusicBuffer      ;if event music buffer empty,
  ; beq IncrementSFTask2      ;branch to increment task

StarFlagExit2:
  rts                       ;otherwise leave
