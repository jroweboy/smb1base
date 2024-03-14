; This amazing disassembly was created by doppelganger (doppelheathen@gmail.com) here https://www.romhacking.net/documents/344/
; Ported to CC65 by threecreepio (https://github.com/threecreepio)
; Massive changes to the code by jroweboy

.ifndef __COMMON_INC
__COMMON_INC = 1

;DEBUG FLAGS
WORLD_HAX = 1

.macpack longbranch
.macpack generic

.linecont on
.feature force_range
.autoimport     +

; Global Labels named after the segment that they appear in
.macro labelled_segment s1,s2,s3,s4,s5,s6,s7,s8,s9,sa,sb,sc,sd
.ifblank s1
  ; First parameter is empty
  .exitmacro
.endif
.global .ident(.string(s1))
.ifdef _COMMON_DEFINE_SEGMENTS
.pushseg
.segment .string(s1)
.ident(.string(s1)):
.export .ident(.string(s1))
.popseg
.endif
labelled_segment s2,s3,s4,s5,s6,s7,s8,s9,sa,sb,sc,sd
.endmacro

labelled_segment OBJECT, PLAYER, MUSIC, LEVEL, CODE, COLLISION, RENDER, FIXED, TITLE, TITLE_MUSIC
labelled_segment TITLE_DPCM, LOWCODE, DECODE, MUSIC_ENGINE, METASPRITE

.include "charmap.inc"
.include "memory.inc"
.include "constants.inc"
.include "globals.inc"
.include "mmc5.inc"

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

.macro AllocSpr spr
.scope
.ident(.concat("AllocSpr_", .string(spr))):
.import CurrentOAMOffset
  lda CurrentOAMOffset
  ; cmp #$ff - (spr * 4)
  ; bcc :+
  ;   rts
  ; :
  tay
  clc
  adc #(spr * 4)
  sta CurrentOAMOffset
.endscope
.endmacro

.macro ReserveSpr spr
.scope
.ident(.concat("ReserveSpr_", .string(spr))):
.import CurrentOAMOffset
  ldy CurrentOAMOffset
  ; cpy #$ff - (spr * 4)
  ; bcc :+
  ;   rts
  ; :
.endmacro

.macro UpdateOAMPosition
  sty CurrentOAMOffset
.endscope
.endmacro


.endif ;__COMMON_INC
