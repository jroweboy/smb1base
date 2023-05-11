; This amazing disassembly was created by doppelganger (doppelheathen@gmail.com) here https://www.romhacking.net/documents/344/
; Ported to CC65 by threecreepio (https://github.com/threecreepio)

.ifndef __COMMON_INC
__COMMON_INC = 1

.linecont on
.feature force_range

.include "charmap.inc"
.include "memory.inc"
.include "constants.inc"
.include "globals.inc"
.include "mmc3.inc"

.macro ASSERT_PAGE label_
	.assert >(label_) = >*, error, "Page crossing in timing sensitive code detected! If you see this error then you need to rearrange the code so this branch doesn't cross a page"
.endmacro

.macro phx
  txa
  pha
.endmacro

.macro plx
  pla
  tax
.endmacro

.macro phy
  tya
  pha
.endmacro

.macro ply
  pla
  tay
.endmacro

.macro abssub x1, x2, store, absstore
.scope
  lda x1
  sec
  sbc x2
.ifnblank store
  sta store
.endif
  bcs skipflip
    eor #$ff
    adc #1
skipflip:
.ifnblank absstore
  sta absstore
.endif
.endscope
.endmacro

.endif ;__COMMON_INC
