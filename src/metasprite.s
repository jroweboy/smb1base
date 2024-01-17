
.include "common.inc"

.segment "METASPRITE"

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
  lda #0
  sta PlayerOAMOffset
  sta CurrentOAMOffset

  ; draw the player first so it doesn't ever flicker
  ldy #0
  ldx ObjectMetasprite,y
  jsr DrawMetasprite

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
  rts

.endproc

.export DrawMetasprite
.proc DrawMetasprite
Ptr = R0
Tmp = R2
Atr = R3
Xlo = R4
; Xhi = R5
Ylo = R6
; Yhi = R7
  
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

  lda SprObject_Rel_XPos,y
  sta Xlo
  lda SprObject_Rel_YPos,y
  sta Ylo
  lda SprObject_SprAttrib,y
  sta Atr
  ldx CurrentOAMOffset
  ldy #0
  lda (Ptr),y
  tay
  bpl RenderLoop

Skip3:
    dey
Skip2:
    dey
    dey
    inx
    inx
    inx
    inx
RenderLoop:
    lda (Ptr),y
    dey
    clc
    adc Xlo
    sta Sprite_X_Position,x

    lda (Ptr),y
    dey
    clc
    adc Ylo
    sta Sprite_Y_Position,x

    lda (Ptr),y
    dey
    ora Atr
    sta Sprite_Attributes,x

    lda (Ptr),y
    sta Sprite_Tilenumber,x
    inx
    inx
    inx
    inx
    dey
    bne RenderLoop
  stx CurrentOAMOffset
Exit:
  rts

.endproc
