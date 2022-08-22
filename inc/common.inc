; This amazing disassembly was created by doppelganger (doppelheathen@gmail.com) here https://www.romhacking.net/documents/344/
; Ported to CC65 by threecreepio (https://github.com/threecreepio)

.ifndef __COMMON_INC
__COMMON_INC = 1

.linecont on

.include "charmap.inc"
.include "memory.inc"
.include "constants.inc"
.include "globals.inc"
.include "mmc3.inc"

; limit the neck size to 20 segments just cause
; 19 causes softlocks in castles when small. 20 is just right
PLAYER_NECK_MAX_SIZE = (20 * 8)

.endif ;__COMMON_INC
