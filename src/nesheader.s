.include "nes2header.inc"

; nes2mapper 4 ; mmc3
nes2mapper 85, 1 ; vrc7
; nes2mapper 5 ; mmc5
nes2prg $80000
; nes2prg $40000
; nes2prg $2c000
; nes2prg $24000
; nes2prg $20000
nes2chr $4000
; nes2chr $20000
nes2mirror 'V'
nes2tv 'N'
nes2bram $2000
nes2end

; CreateBanks $8000, $8000, 0

; CreateSegment CODE, 0
; CreateSegment VECTORS, 0, $fffa
