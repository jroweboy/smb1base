.include "common.inc"

; .segment "CHR_ROTATE"


; .repeat MARIO_ROTATION_ANGLE_MAX,I
; .incbin .sprintf("../chr/rotate/rotate_%d.chr", I)
; .endrepeat


.segment "CHR_ORIGINAL"
; Switch sprites and bg
; .incbin "../Super Mario Bros. (World).nes", $9010, $1000
; .incbin "../Super Mario Bros. (World).nes", $8010, $1000

.incbin "../chr/arranged_bg.chr"
.incbin "../chr/arranged_sprites.chr"

; include the new title screen graphics
.segment "CHR_TITLE_BG_0"
.incbin "../chr/title/latest_bg_0.chr"
.segment "CHR_TITLE_BG_1"
.incbin "../chr/title/latest_bg_1.chr"
.segment "CHR_TITLE_BG_2"
.incbin "../chr/title/latest_bg_2.chr"

.segment "CHR_TITLE_SPRITE"
.incbin "../chr/title/latest_spr.chr"
