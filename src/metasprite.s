
.include "common.inc"

.segment "METASPRITE"

.import MoveAllSpritesOffscreen

METASPRITE_BODY = 1

.include "metasprite.inc"

METASPRITE_LEFT_0_LO = $00
METASPRITE_LEFT_0_HI = $00
METASPRITE_RIGHT_0_LO = $00
METASPRITE_RIGHT_0_HI = $00

MetaspriteTableLeftLo:
.repeat METASPRITES_COUNT, I
  .byte .ident(.sprintf("METASPRITE_LEFT_%d_LO", I))
.endrepeat
MetaspriteTableLeftHi:
.repeat METASPRITES_COUNT, I
  .byte .ident(.sprintf("METASPRITE_LEFT_%d_HI", I))
.endrepeat

MetaspriteTableRightLo:
.repeat METASPRITES_COUNT, I
  .byte .ident(.sprintf("METASPRITE_RIGHT_%d_LO", I))
.endrepeat
MetaspriteTableRightHi:
.repeat METASPRITES_COUNT, I
  .byte .ident(.sprintf("METASPRITE_RIGHT_%d_HI", I))
.endrepeat

.export DrawAllMetasprites
.proc DrawAllMetasprites
  ; jsr MoveAllSpritesOffscreen
  ; If we are going through a pipe, we need to reserve
  ; 16 sprites (8 for each player overlay)

  ; Check to see if the leader is behind something.
  ; If they are we want to draw them second
;   lda Player_SprAttrib
;   and #(1 << 5)
;   beq :+ ; not behind the door
;     lda #32
; :
;   ; if we are in a pipe transition, reserve enough for the sprite overlay
;   ldy InPipeTransition
;   beq :+
;     clc 
;     adc #8 * 4
; :

  ; draw the player first so it doesn't ever flicker
  ldy #0
  ldx ObjectMetasprite,y
  ; unless the player is currently flickering due to damage taken
  beq :+
    jsr DrawMetasprite
  :


  lda #24 - 1 ; size of the different object update list
  sta SpriteShuffleTemp
  lda SpriteShuffleOffset
  clc
  adc #19
  cmp #24
  bcc :+
    ; implicit carry set
    sbc #24
:
ObjectLoop:
    clc
    adc #13
    cmp #24
    bcc :+
      ; implicit carry set
      sbc #24
    :
    ; skip index zero since we draw the player first always.
    beq NextLoop
      ; TODO check offscreenbits to make sure they are onscreen still
      tay
      ldx ObjectMetasprite,y
      beq NextLoop
      cpx #METASPRITES_COUNT ; todo remove this after fixing all bugs
      bcs NextLoop
        sta SpriteShuffleOffset
        jsr DrawMetasprite
        lda SpriteShuffleOffset
  NextLoop:
    dec SpriteShuffleTemp
    bpl ObjectLoop
  sta SpriteShuffleOffset

  ldx #7-1
FloateyNumberLoop:
    lda FloateyNum_Control,x     ;load control for floatey number
    beq Skip                     ;if zero, branch to leave
      phx
        jsr FloateyNumberRender
      plx
  Skip:
    dex
    bpl FloateyNumberLoop
  
  ; Clear sprites up to the offset
  lda #$f8
  ldx CurrentOAMOffset
  ClearLoop:
    sta Sprite_Y_Position,x
    inx
    inx
    inx
    inx
    bne ClearLoop
  rts
.endproc

.export DrawMetasprite
.proc DrawMetasprite
Ptr = R0
OrigOffset = R2
Atr = R3
Xlo = R4
Xhi = R5
Ylo = R6
Yhi = R7
  
  lda PlayerFacingDir,y
  lsr
  bne FacingLeft
    lda MetaspriteTableRightLo,x
    sta Ptr
    lda MetaspriteTableRightHi,x
    sta Ptr+1
    bne DrawSprite ; unconditional
  FacingLeft:
    lda MetaspriteTableLeftLo,x
    sta Ptr
    lda MetaspriteTableLeftHi,x
    sta Ptr+1
DrawSprite:

  sty OrigOffset

  lda SprObject_X_Position,y
  sec
  sbc ScreenLeft_X_Pos
  sta Xlo
  lda SprObject_PageLoc,y
  sbc ScreenLeft_PageLoc
  sta Xhi
  lda SprObject_Y_Position,y
  sta Ylo
  lda SprObject_Y_HighPos,y
  sta Yhi
  lda SprObject_SprAttrib,y
  sta Atr

  jsr MetaspriteRenderLoop

  ldy OrigOffset
  cpy #8
  bcs NotEnemyOrPlayer
    lda EnemyVerticalFlip-1,y
    beq NotVFlippedEnemy
      ldy #0
      ; reload the size. If its 8 or less then we don't need to do anything
      lda (Ptr),y
      cmp #8 + 1
      bcc NotVFlippedEnemy
        ; sprite has two columns, so flip the two columns
        lda Sprite_Tilenumber-4,x     ;load first or second row tiles
        pha                         ;and save tiles to the stack
          lda Sprite_Tilenumber-8,x
          pha
            lda Sprite_Tilenumber-12,x  ;exchange third row tiles
            sta Sprite_Tilenumber-4,x     ;with first or second row tiles
            lda Sprite_Tilenumber-16,x
            sta Sprite_Tilenumber-8,x
          pla                         ;pull first or second row tiles from stack
          sta Sprite_Tilenumber-16,x  ;and save in third row
        pla
        sta Sprite_Tilenumber-12,x
  NotVFlippedEnemy:
NotEnemyOrPlayer:

Exit:
  rts

.endproc

.proc MetaspriteRenderLoop
Ptr = R0
OrigOffset = R2
Atr = R3
Xlo = R4
Xhi = R5
Ylo = R6
Yhi = R7

  ldx CurrentOAMOffset
  ldy #0
  lda (Ptr),y
  tay
  bpl RenderLoop

; Offscreen sprites end up here

Skip4:   ; X Offscreen
    dey
Skip3:   ; Y Offscreen
    dey
Skip2:
    dey
    ; Move this sprite offscreen
    lda #$f8
    sta Sprite_Y_Position,x
    inx
    inx
    inx
    inx
    dey
    beq LoopEnded
RenderLoop:
    ; load the x position and make sure its on screen
    clc
    lda (Ptr),y
    bpl PositiveX
      adc Xlo
      sta Sprite_X_Position,x
      lda Xhi
      adc #$ff
      beq ContinueAfterX
      bne Skip4
  PositiveX:
    adc Xlo
    sta Sprite_X_Position,x
    lda Xhi
    adc #0
    bne Skip4
  ContinueAfterX:
    dey

    ; load the y position and also make sure its on screen
    clc
    lda (Ptr),y
    bpl PositiveY
      ; NegativeY
      adc Ylo
      sta Sprite_Y_Position,x
      lda Yhi
      adc #$ff
      cmp #1      ; page 1 is the "main" y position
      beq ContinueAfterY
      bne Skip3
  PositiveY:
    adc Ylo
    sta Sprite_Y_Position,x
    lda Yhi
    adc #0
    cmp #1      ; page 1 is the "main" y position
    bne Skip3
  ContinueAfterY:
    dey

    ; Mix attributes but if the NO_PALETTE bit is set, prevent
    ; the palette from changing.
    lda (Ptr),y
    bit NoPaletteBitMask
    beq AllowPaletteChange
      ; No palette change bit set, so pull the byte and
      ; mask off the palette from the attribute byte
      lda Atr
      and #%11111100
      ora (Ptr),y
      bne WritePalette ; unconditional
AllowPaletteChange:
    ora Atr
WritePalette:
    sta Sprite_Attributes,x
    dey

    ; set the tile number and move to the next sprite
    lda (Ptr),y
    sta Sprite_Tilenumber,x
    inx
    inx
    inx
    inx
    dey
    bne RenderLoop
LoopEnded:
  stx CurrentOAMOffset
  rts

NoPaletteBitMask:
  .byte (SPR_NO_PALETTE >> 8)
.endproc

; ;-------------------------------------------------------------------------------------
.proc FloateyNumberRender
Ptr = R0
OrigOffset = R2
Atr = R3
Xlo = R4
Xhi = R5
Ylo = R6
Yhi = R7

  lda FloateyNum_Y_Pos,x       ;get vertical coordinate for
  cmp #$18                     ;floatey number, if coordinate in the
  bcc SetupNumSpr              ;status bar, branch
    sbc #$01
    sta FloateyNum_Y_Pos,x       ;otherwise subtract one and store as new
SetupNumSpr:
  sta Ylo
  lda #1
  sta Yhi
  lda #0
  sta Atr
  sta Xhi
  lda FloateyNum_X_Pos,x
  sta Xlo
  ldy FloateyNum_Control,x
  ldx FloateyNumMetasprites-1,y
  lda MetaspriteTableRightLo,x
  sta Ptr
  lda MetaspriteTableRightHi,x
  sta Ptr+1
  jmp MetaspriteRenderLoop
  ; implicit rts

FloateyNumMetasprites:
  .byte METASPRITE_NUMBER_100
  .byte METASPRITE_NUMBER_200
  .byte METASPRITE_NUMBER_400
  .byte METASPRITE_NUMBER_500
  .byte METASPRITE_NUMBER_800
  .byte METASPRITE_NUMBER_1000
  .byte METASPRITE_NUMBER_2000
  .byte METASPRITE_NUMBER_4000
  .byte METASPRITE_NUMBER_5000
  .byte METASPRITE_NUMBER_8000
  .byte METASPRITE_NUMBER_1UP

.endproc

  ; lda FloateyNum_Y_Pos,x       ;get vertical coordinate

;   AllocSpr 2

; FloateyNumMetasprites:
; .endproc

;   lda FloateyNum_Control,x     ;load control for floatey number
;   beq EndFloateyNumber         ;if zero, branch to leave
;     cmp #$0b                     ;if less than $0b, branch
;     bcc ChkNumTimer
;       lda #$0b                     ;otherwise set to $0b, thus keeping
;       sta FloateyNum_Control,x     ;it in range
; ChkNumTimer:
;     tay                          ;use as Y
;     lda FloateyNum_Timer,x       ;check value here
;     bne DecNumTimer              ;if nonzero, branch ahead
;       sta FloateyNum_Control,x     ;initialize floatey number control and leave
; EndFloateyNumber:
;   rts
; DecNumTimer:
;   dec FloateyNum_Timer,x       ;decrement value here
;   cmp #$2b                     ;if not reached a certain point, branch  
;   bne ChkTallEnemy
;     cpy #$0b                     ;check offset for $0b
;     bne LoadNumTiles             ;branch ahead if not found
;       inc NumberofLives            ;give player one extra life (1-up)
;       lda #Sfx_ExtraLife
;       sta Square2SoundQueue        ;and play the 1-up sound
;   LoadNumTiles:
;     lda ScoreUpdateData,y        ;load point value here
;     lsr                          ;move high nybble to low
;     lsr
;     lsr
;     lsr
;     tax                          ;use as X offset, essentially the digit
;     lda ScoreUpdateData,y        ;load again and this time
;     and #%00001111               ;mask out the high nybble
;     sta DigitModifier,x          ;store as amount to add to the digit
;     jsr AddToScore               ;update the score accordingly
; ChkTallEnemy:
;   ; ldy CurrentOAMOffset
;   ; ldy Enemy_SprDataOffset,x    ;get OAM data offset for enemy object
;   ; lda Enemy_ID,x               ;get enemy object identifier
;   ; cmp #Spiny
;   ; beq FloateyPart              ;branch if spiny
;   ; cmp #PiranhaPlant
;   ; beq FloateyPart              ;branch if piranha plant
;   ; cmp #HammerBro
;   ; beq GetAltOffset             ;branch elsewhere if hammer bro
;   ; cmp #GreyCheepCheep
;   ; beq FloateyPart              ;branch if cheep-cheep of either color
;   ; cmp #RedCheepCheep
;   ; beq FloateyPart
;   ; cmp #TallEnemy
;   ; bcs GetAltOffset             ;branch elsewhere if enemy object => $09
;   ; lda Enemy_State,x
;   ; cmp #$02                     ;if enemy state defeated or otherwise
;   ; bcs FloateyPart              ;$02 or greater, branch beyond this part
; ; GetAltOffset:
;   ; ldx SprDataOffset_Ctrl       ;load some kind of control bit
;   ; ldy Alt_SprDataOffset,x      ;get alternate OAM data offset
;   ; ldx ObjectOffset             ;get enemy object offset again
; FloateyPart:
;   lda FloateyNum_Y_Pos,x       ;get vertical coordinate for
;   cmp #$18                     ;floatey number, if coordinate in the
;   bcc SetupNumSpr              ;status bar, branch
;     sbc #$01
;     sta FloateyNum_Y_Pos,x       ;otherwise subtract one and store as new
; SetupNumSpr:
;   lda FloateyNum_Y_Pos,x       ;get vertical coordinate
;   sbc #$08                     ;subtract eight and dump into the
;   sta Sprite_Data+4,y       ;and into first row sprites
;   sta Sprite_Data,y
;   lda FloateyNum_X_Pos,x       ;get horizontal coordinate
;   sta Sprite_X_Position,y      ;store into X coordinate of left sprite
;   clc
;   adc #$08                     ;add eight pixels and store into X
;   sta Sprite_X_Position+4,y    ;coordinate of right sprite
;   lda #$02
;   sta Sprite_Attributes,y      ;set palette control in attribute bytes
;   sta Sprite_Attributes+4,y    ;of left and right sprites
;   lda FloateyNum_Control,x
;   asl                          ;multiply our floatey number control by 2
;   tax                          ;and use as offset for look-up table
;   lda FloateyNumTileData,x
;   sta Sprite_Tilenumber,y      ;display first half of number of points
;   lda FloateyNumTileData+1,x
;   sta Sprite_Tilenumber+4,y    ;display the second half
;   ; ldx ObjectOffset             ;get enemy object offset and leave
;   rts

; ;data is used as tiles for numbers
; ;that appear when you defeat enemies
; FloateyNumTileData:
;   .byte $ff, $ff ;dummy
;   .byte FLOATEY_NUM_10, FLOATEY_NUM_0 ; "100"
;   .byte FLOATEY_NUM_20, FLOATEY_NUM_0 ; "200"
;   .byte FLOATEY_NUM_40, FLOATEY_NUM_0 ; "400"
;   .byte FLOATEY_NUM_50, FLOATEY_NUM_0 ; "500"
;   .byte FLOATEY_NUM_80, FLOATEY_NUM_0 ; "800"
;   .byte FLOATEY_NUM_10, FLOATEY_NUM_00 ; "1000"
;   .byte FLOATEY_NUM_20, FLOATEY_NUM_00 ; "2000"
;   .byte FLOATEY_NUM_40, FLOATEY_NUM_00 ; "4000"
;   .byte FLOATEY_NUM_50, FLOATEY_NUM_00 ; "5000"
;   .byte FLOATEY_NUM_80, FLOATEY_NUM_00 ; "8000"
;   .byte FLOATEY_NUM_1, FLOATEY_NUM_UP ; "1-UP"
