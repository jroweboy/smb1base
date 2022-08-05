
.include "common.inc"
.include "object.inc"

;--------------------------------

InitGoomba:
      jsr InitNormalEnemy  ;set appropriate horizontal speed
      jmp SmallBBox        ;set $09 as bounding box control, set other values
