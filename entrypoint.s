
; Single entrypoint that includes files to make building this easier


.macpack longbranch
.macpack generic

.linecont on
.feature force_range


.include "options.s"

.include "inc/common.inc"

.ifdef MAPPER_MMC5
.include "src/mmc5.s"
.endif
.ifdef MAPPER_MMC3
.include "src/mmc3.s"
.endif

.include "src/metasprite_engine.s"
.include "src/metasprite.s"


.segment "CHR"

; Water = 0
; Ground = 1
; UnderGround = 2
; Castle = 3

.ifdef MAPPER_MMC5
.incbin "chr/mmc5/bg_water.chr"
.incbin "chr/mmc5/bg_ground.chr"
.incbin "chr/mmc5/bg_underground.chr"
.incbin "chr/mmc5/bg_castle.chr"

.incbin "chr/mmc5/sprites_mario.chr"
.incbin "chr/mmc5/sprites_misc.chr"
.incbin "chr/mmc5/sprites_enemies.chr"
.endif

.ifdef MAPPER_MMC3

.incbin "chr/mmc5/bg_water.chr"
.incbin "chr/mmc5/bg_ground.chr"
.incbin "chr/mmc5/bg_underground.chr"
.incbin "chr/mmc5/bg_castle.chr"

.incbin "chr/mmc3/sprites_mario.chr"
.incbin "chr/mmc3/sprites_misc.chr"

.incbin "chr/mmc3/sprites_water.chr"
.incbin "chr/mmc3/sprites_overworld.chr"
.incbin "chr/mmc3/sprites_underground.chr"
.incbin "chr/mmc3/sprites_castle.chr"

.endif

