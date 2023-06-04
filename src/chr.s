.include "common.inc"

.segment "CHR"


.repeat MARIO_ROTATION_ANGLE_MAX,I
.incbin .sprintf("../chr/rotate/rotate_%d.chr", I)
.endrepeat

; Switch sprites and bg
.incbin "../Super Mario Bros. (World).nes", $9010, $1000
.incbin "../Super Mario Bros. (World).nes", $8010, $1000

; include the new title screen graphics
.align $ff
.incbin "../chr/title/4kb_bg_0.chr"
.incbin "../chr/title/4kb_bg_1.chr"
.incbin "../chr/title/4kb_bg_2.chr"

