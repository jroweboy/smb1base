
.include "common.inc"

.segment "METASPRITE"

.import MoveAllSpritesOffscreen

METASPRITE_BODY = 1

.include "metasprite.inc"
.include "metasprite_custom.inc"

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

LoopCount = M0

  ; draw the player first so it doesn't ever flicker
  ldy #0
  ldx ObjectMetasprite,y
  ; unless the player is currently flickering due to damage taken
  beq ClearOAMLoop
    ; put the player in slot 0 always
    lda CurrentOAMOffset
    pha
      lda PlayerOAMOffset
      sta CurrentOAMOffset
      jsr DrawMetasprite
    pla
    sta CurrentOAMOffset
  ClearOAMLoop:
    ; Now clear out the 
    ; X is the old OAM offset
    cpx #4*4
    beq DoneDrawingPlayer
    lda #$f8
    ClearLoop:
      sta Sprite_Y_Position,x
      inx
      inx
      inx
      inx
      cpx #4*4
      bne ClearLoop
DoneDrawingPlayer:

  lda #24 - 1 ; size of the different object update list
  sta LoopCount
  lda SpriteShuffleOffset
  clc
  adc #19
  cmp #24
  bcc :+
    ; implicit carry set
    sbc #24
:
  sta SpriteShuffleOffset
  lda FrameCounter
  lsr
  lda SpriteShuffleOffset
  bcc ObjectLoopNegative

ObjectLoopPositive:
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
    dec LoopCount
    bpl ObjectLoopPositive
  jmp HandleFloateyNumbers

ObjectLoopNegative:
    sec
    sbc #13
    bcs :+
      ; implicit carry clear
      adc #24
    :
    ; skip index zero since we draw the player first always.
    beq NextLoop2
      ; TODO check offscreenbits to make sure they are onscreen still
      tay
      ldx ObjectMetasprite,y
      beq NextLoop2
      cpx #METASPRITES_COUNT ; todo remove this after fixing all bugs
      bcs NextLoop2
        sta SpriteShuffleOffset
        jsr DrawMetasprite
        lda SpriteShuffleOffset
  NextLoop2:
    dec LoopCount
    bpl ObjectLoopNegative

HandleFloateyNumbers:
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
  :
    sta Sprite_Y_Position,x
    inx
    inx
    inx
    inx
    bne :-
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

  cpy #7
  bcs NotFlipped
  lda ObjectVerticalFlip,y
  beq NotFlipped
    lda #OAM_FLIP_V
    .byte $2c
NotFlipped:
  lda #0
  ora SprObject_SprAttrib,y
  sta Atr

  jsr MetaspriteRenderLoop

  ldy OrigOffset
  cpy #7
  bcs NotEnemy
    lda ObjectVerticalFlip,y
    beq NotVFlippedEnemy
    bmi BowserFrontFlip
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
NotEnemy:

Exit:
  rts
BowserFrontFlip:
  ; Custom flip code for bowser's front since he is weird.
  lda Sprite_Y_Position-8,x
  clc
  adc #32
  bcs Offscreen
    sta Sprite_Y_Position-8,x
    sta Sprite_Y_Position-4,x
Offscreen:
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
