
.include "common.inc"
.include "level.inc"

.segment "LEVEL"

;--------------------------------

; FlagBalls_Residual:
;       jsr GetLrgObjAttrib  ;get low nybble from object byte
;       ldx #$02             ;render flag balls on third row from top
;       lda #$6d             ;of screen downwards based on low nybble
;       jmp RenderUnderPart

;--------------------------------

FlagpoleObject:
  lda #$24                 ;render flagpole ball on top
  sta MetatileBuffer
  ldx #$01                 ;now render the flagpole shaft
  ldy #$08
  lda #$25
  jsr RenderUnderPart
  lda #$61                 ;render solid block at the bottom
  sta MetatileBuffer+10
  jsr GetAreaObjXPosition
  sec                      ;get pixel coordinate of where the flagpole is,
  sbc #$08                 ;subtract eight pixels and use as horizontal
  sta Enemy_X_Position+5   ;coordinate for the flag
  lda CurrentPageLoc
  sbc #$00                 ;subtract borrow from page location and use as
  sta Enemy_PageLoc+5      ;page location for the flag
  lda #$30
  sta Enemy_Y_Position+5   ;set vertical coordinate for flag
  lda #1
  sta Enemy_Y_HighPos+5
  lda #$b0 + 8 ; jroweboy offset the flag score by 8 px to account for 8x16 difference
  sta FlagpoleFNum_Y_Pos   ;set initial vertical coordinate for flagpole's floatey number
  lda #FlagpoleFlagObject
  sta Enemy_ID+5           ;set flag identifier, note that identifier and coordinates
  inc Enemy_Flag+5         ;use last space in enemy object buffer
  rts
