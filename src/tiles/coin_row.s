
.include "common.inc"
.include "level.inc"

.segment "LEVEL"

;--------------------------------

CoinMetatileData:
      .byte $c3, $c2, $c2, $c2

RowOfCoins:
      ldy AreaType            ;get area type
      lda CoinMetatileData,y  ;load appropriate coin metatile
      jmp GetRow
