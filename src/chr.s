
.segment "CHR"

; .incbin "../Super Mario Bros. (World).nes", $8010

; .incbin "../chr/rearranged_background.chr"
; .incbin "../chr/rearranged_sprites.chr"


.ifdef MAPPER_MMC5

; Water = 0
; Ground = 1
; UnderGround = 2
; Castle = 3
.incbin "../chr/mmc5/bg_water.chr"
.incbin "../chr/mmc5/bg_ground.chr"
.incbin "../chr/mmc5/bg_underground.chr"
.incbin "../chr/mmc5/bg_castle.chr"

.incbin "../chr/mmc5/sprites_mario.chr"
.incbin "../chr/mmc5/sprites_misc.chr"
.incbin "../chr/mmc5/sprites_enemies.chr"
.endif

.ifdef MAPPER_MMC3

.incbin "../chr/mmc5/bg_water.chr"
.incbin "../chr/mmc5/bg_ground.chr"
.incbin "../chr/mmc5/bg_underground.chr"
.incbin "../chr/mmc5/bg_castle.chr"

.incbin "../chr/mmc3/sprites_mario.chr"
.incbin "../chr/mmc3/sprites_misc.chr"

.incbin "../chr/mmc3/sprites_water.chr"
.incbin "../chr/mmc3/sprites_overworld.chr"
.incbin "../chr/mmc3/sprites_underground.chr"
.incbin "../chr/mmc3/sprites_castle.chr"

.endif

