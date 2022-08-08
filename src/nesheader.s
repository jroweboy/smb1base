.include "nes2header.inc"

nes2mapper 0
nes2prg $8000
nes2chr $2000
nes2mirror 'V'
nes2tv 'N'
nes2end

CreateBanks $8000, $8000, 0

CreateSegment CODE, 0
CreateSegment VECTORS, 0, $fffa
