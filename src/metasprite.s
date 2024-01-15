
.include "common.inc"

.segment "METASPRITE"

METASPRITE_BODY = 1

.include "metasprite.inc"

METASPRITE_0_LO = $00
METASPRITE_0_HI = $00
MetaspriteTableLo:
.repeat METASPRITES_COUNT, I
  .byte .ident(.sprintf("METASPRITE_%d_LO", I))
.endrepeat
MetaspriteTableHi:
.repeat METASPRITES_COUNT, I
  .byte .ident(.sprintf("METASPRITE_%d_HI", I))
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
  sta SpriteShuffleOffset
ObjectLoop:
    lda SpriteShuffleOffset
    clc
    adc #13
    cmp #24
    bcc :+
      ; implicit carry set
      sbc #24
    :
    sta SpriteShuffleOffset
    beq :+
      jsr DrawMetasprite
    :
    dec SpriteShuffleTemp
    bpl ObjectLoop
  rts
.endproc

.proc DrawMetasprite
Ptr = R0
HFlip = R3
Attr = R4
Xlo = R5
Ylo = R6
  
; TODO check offscreenbits to make sure they are onscreen still
  tay
  ldx ObjectMetasprite,y
  cpx #0
  beq EarlyExit
  cpx #METASPRITES_COUNT ; todo remove this after fixing all bugs
  bcc Continue
EarlyExit:
    rts
Continue:
  lda MetaspriteTableLo,x
  sta Ptr
  lda MetaspriteTableHi,x
  sta Ptr+1

  lda SprObject_X_Position,y
  sec
  sbc ScreenLeft_X_Pos
  sta Xlo
  lda SprObject_Y_Position,y
  sta Ylo
  lda SprObject_SprAttrib,y
  sta Attr
  lda PlayerFacingDir,y
  sta HFlip

  ldx CurrentOAMOffset
  ldy #0
  lda (Ptr),y
  tay
RenderLoop:
    lda (Ptr),y
    ora Attr
    sta Sprite_Attributes,x
    dey
    lda (Ptr),y
    sta Sprite_Tilenumber,x
    dey
    lda (Ptr),y
    clc
    adc Ylo
    sta Sprite_Y_Position,x
    dey
    lda (Ptr),y
    clc
    adc Xlo
    sta Sprite_X_Position,x
    inx
    inx
    inx
    inx
    dey
    bne RenderLoop
  stx CurrentOAMOffset

  lda HFlip
  lsr
  beq Exit
HorizontalFlip:
    ; switch every other tile id
    lda (Ptr),y
    lsr
    lsr
    lsr
    beq Exit
    lsr
    beq OneRow
  TwoRows:
    lda Sprite_Tilenumber-16,x
    pha
      lda Sprite_Tilenumber-12,x
      sta Sprite_Tilenumber-16,x
    pla
    sta Sprite_Tilenumber-12,x
    lda Sprite_Attributes-16,x
    eor #$40
    sta Sprite_Attributes-16,x
    lda Sprite_Attributes-12,x
    eor #$40
    sta Sprite_Attributes-12,x
  OneRow:
    lda Sprite_Tilenumber-8,x
    pha
      lda Sprite_Tilenumber-4,x
      sta Sprite_Tilenumber-8,x
    pla
    sta Sprite_Tilenumber-4,x
    lda Sprite_Attributes-8,x
    eor #$40
    sta Sprite_Attributes-8,x
    lda Sprite_Attributes-4,x
    eor #$40
    sta Sprite_Attributes-4,x
Exit:
  rts
.endproc

