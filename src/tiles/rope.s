
.include "common.inc"
.include "level.inc"

.segment "LEVEL"

;--------------------------------

EndlessRope:
      ldx #$00       ;render rope from the top to the bottom of screen
      ldy #$0f
      jmp DrawRope

BalancePlatRope:
          txa                 ;save object buffer offset for now
          pha
          ldx #$01            ;blank out all from second row to the bottom
          ldy #$0f            ;with blank used for balance platform rope
          lda #$44
          jsr RenderUnderPart
          pla                 ;get back object buffer offset
          tax
          jsr GetLrgObjAttrib ;get vertical length from lower nybble
          ldx #$01
DrawRope: lda #$40            ;render the actual rope
          jmp RenderUnderPart
