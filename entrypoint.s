
; Single entrypoint that includes files to make building this easier


.macpack longbranch
.macpack generic

.linecont on
.feature force_range


.include "options.s"


.include "inc/nes2header.inc"

;--------------------------------
; NES2.0 header

.if MAPPER_MMC5
nes2mapper 5 ; mmc5
.elseif MAPPER_MMC3
nes2mapper 3 ; mmc3
.endif
nes2prg $10000
nes2chr $10000
nes2bram $2000
nes2mirror 'V'
nes2tv 'N'
nes2end



_COMMON_DEFINE_SEGMENTS = 1
.include "inc/common.inc"

.include "inc/charmap.inc"
.include "inc/constants.inc"
.include "src/memory.s"

.if MAPPER_MMC5
.include "src/mmc5.s"
.endif
.if MAPPER_MMC3
.include "src/mmc3.s"
.endif

.include "src/metasprite_engine.s"
.include "src/metasprite.s"

.include "src/common.s"
.include "src/collision.s"
.include "src/level_tiles.s"
.include "src/reset.s"
.include "src/main.s"

.include "src/player.s"
.include "src/object.s"
.include "src/screen_render.s"
.include "src/sprite_render.s"


.segment "AUDIO"

.include "src/music.s"


.segment "CHR"

; Water = 0
; Ground = 1
; UnderGround = 2
; Castle = 3

.if MAPPER_MMC5
.incbin "chr/mmc5/bg_water.chr"
.incbin "chr/mmc5/bg_ground.chr"
.incbin "chr/mmc5/bg_underground.chr"
.incbin "chr/mmc5/bg_castle.chr"

.incbin "chr/mmc5/sprites_mario.chr"
.incbin "chr/mmc5/sprites_misc.chr"
.incbin "chr/mmc5/sprites_enemies.chr"

.else

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

.pushseg
.segment "TITLE"
TitleScreenData:
.incbin "chr/raw/titlescreen.bin"
.popseg