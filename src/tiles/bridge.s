
.include "common.inc"
.include "level.inc"

.segment "LEVEL"

;--------------------------------

Bridge_High:
      lda #$06  ;start on the seventh row from top of screen
      .byte $2c   ;BIT instruction opcode

Bridge_Middle:
      lda #$07  ;start on the eighth row
      .byte $2c   ;BIT instruction opcode

Bridge_Low:
      lda #$09             ;start on the tenth row
      pha                  ;save whatever row to the stack for now
      jsr ChkLrgObjLength  ;get low nybble and save as length
      pla
      tax                  ;render bridge railing
      lda #$0b
      sta MetatileBuffer,x
      inx
      ldy #$00             ;now render the bridge itself
      lda #$63
      jmp RenderUnderPart
