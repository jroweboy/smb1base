.include "common.inc"

; .segment "CHR_ROTATE" 


; .repeat MARIO_ROTATION_ANGLE_MAX,I
; .incbin .sprintf("../chr/rotate/rotate_%d.chr", I)
; .endrepeat


; .segment "CHR_IN_PRG1"
.segment "CHR_ORIGINAL"
; Switch sprites and bg
; .incbin "../Super Mario Bros. (World).nes", $9010, $1000
; .incbin "../Super Mario Bros. (World).nes", $8010, $1000
; .export CHR1
; CHR1:
.incbin "../chr/arranged_bg.chr"
.incbin "../chr/arranged_sprites.chr"

; .segment "CHR_IN_PRG2"
; .export CHR2
; CHR2:
.incbin "../chr/peach_spritesheet.chr"
.incbin "../chr/alternate_disco_floor.chr"
.incbin "../chr/title_screen_bg_new.chr"

.segment "CHR_NUMBERS"

.incbin "../chr/fixed_number_0_3.chr"
.incbin "../chr/fixed_number_4_7.chr"
.incbin "../chr/fixed_number_8_9.chr"

; include the new title screen graphics
; .segment "CHR_TITLE_BG_0"
; .incbin "../chr/title/latest_bg_0.chr"
; .segment "CHR_TITLE_BG_1"
; .incbin "../chr/title/latest_bg_1.chr"
; .segment "CHR_TITLE_BG_2"
; .incbin "../chr/title/latest_bg_2.chr"

; .segment "CHR_TITLE_SPRITE"
; .incbin "../chr/title/latest_spr.chr"
