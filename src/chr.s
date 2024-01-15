
.segment "CHR"

; .incbin "../Super Mario Bros. (World).nes", $8010

.incbin "../chr/rearranged_background.chr"
; .incbin "../chr/rearranged_sprites.chr"

.global CHR_BIGMARIO, CHR_MARIOACTION
CHR_BIGMARIO = $04
CHR_MARIOACTION = $05
CHR_SMALLMARIO = $06
CHR_FIXED = $07
.incbin "../chr/sprites_mario.chr"

CHR_OVERWORLD_SPRITES = $08
.incbin "../chr/sprites_overworld.chr"

CHR_UNDERGROUND_SPRITES = $0a
.incbin "../chr/sprites_underground.chr"

CHR_CASTLE_SPRITES = $0c
.incbin "../chr/sprites_castle.chr"
