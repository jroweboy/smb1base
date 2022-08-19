
.include "common.inc"
.include "object.inc"

.segment "OBJECT"

;-------------------------------------------------------------------------------------

WarpZoneObject:
  lda ScrollLock         ;check for scroll lock flag
  beq ExGTimer           ;branch if not set to leave
  lda Player_Y_Position  ;check to see if player's vertical coordinate has
  and Player_Y_HighPos   ;same bits set as in vertical high byte (why?)
  bne ExGTimer           ;if so, branch to leave
    sta ScrollLock         ;otherwise nullify scroll lock flag
    inc WarpZoneControl    ;increment warp zone flag to make warp pipes for warp zone
    jmp EraseEnemyObject   ;kill this object
; added rts here since this relied on a common rts
ExGTimer:
  rts