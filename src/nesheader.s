.include "nes2header.inc"

nes2mapper 4 ; mmc3
; nes2mapper 5 ; mmc5
; nes2prg $24000
nes2prg $20000
nes2chr $12000
nes2mirror 'V'
nes2tv 'N'
nes2end

; CreateBanks $8000, $8000, 0

; CreateSegment CODE, 0
; CreateSegment VECTORS, 0, $fffa
