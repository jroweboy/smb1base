
.include "common.inc"
.include "level.inc"

.segment "LEVEL"

;--------------------------------

QuestionBlockRow_High:
      lda #$03    ;start on the fourth row
      .byte $2c     ;BIT instruction opcode

QuestionBlockRow_Low:
      lda #$07             ;start on the eighth row
      pha                  ;save whatever row to the stack for now
      jsr ChkLrgObjLength  ;get low nybble and save as length
      pla
      tax                  ;render question boxes with coins
      lda #$c0
      sta MetatileBuffer,x
      rts
