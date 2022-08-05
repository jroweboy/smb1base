
.include "common.inc"
.include "object.inc"

;--------------------------------

InitPiranhaPlant:
      lda #$01                     ;set initial speed
      sta PiranhaPlant_Y_Speed,x
      lsr
      sta Enemy_State,x            ;initialize enemy state and what would normally
      sta PiranhaPlant_MoveFlag,x  ;be used as vertical speed, but not in this case
      lda Enemy_Y_Position,x
      sta PiranhaPlantDownYPos,x   ;save original vertical coordinate here
      sec
      sbc #$18
      sta PiranhaPlantUpYPos,x     ;save original vertical coordinate - 24 pixels here
      lda #$09
      jmp SetBBox2                 ;set specific value for bounding box control
