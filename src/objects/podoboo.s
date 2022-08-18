
.include "common.inc"
.include "object.inc"

.segment "OBJECT"

;--------------------------------

InitPodoboo:
      lda #$02                  ;set enemy position to below
      sta Enemy_Y_HighPos,x     ;the bottom of the screen
      sta Enemy_Y_Position,x
      lsr
      sta EnemyIntervalTimer,x  ;set timer for enemy
      lsr
      sta Enemy_State,x         ;initialize enemy state, then jump to use
      jmp SmallBBox             ;$09 as bounding box size and set other things

;-------------------------------------------------------------------------------------

MovePodoboo:
      lda EnemyIntervalTimer,x   ;check enemy timer
      bne PdbM                   ;branch to move enemy if not expired
      jsr InitPodoboo            ;otherwise set up podoboo again
      lda PseudoRandomBitReg+1,x ;get part of LSFR
      ora #%10000000             ;set d7
      sta Enemy_Y_MoveForce,x    ;store as movement force
      and #%00001111             ;mask out high nybble
      ora #$06                   ;set for at least six intervals
      sta EnemyIntervalTimer,x   ;store as new enemy timer
      lda #$f9
      sta Enemy_Y_Speed,x        ;set vertical speed to move podoboo upwards
PdbM: jmp MoveJ_EnemyVertically  ;branch to impose gravity on podoboo
