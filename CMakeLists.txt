cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME panic_at_the_mario_disco)

project(${PROJECT_NAME} ASM C)

set(CLEAN ${PROJECT_NAME}_clean)
set(UNCUT ${PROJECT_NAME}_uncut)

set(SRCS

  chr/arranged_bg.chr
  chr/arranged_sprites.chr
  chr/alternate_disco_floor.chr
  chr/peach_spritesheet.chr

  chr/fixed_number_0_3.chr
  chr/fixed_number_4_7.chr
  chr/fixed_number_8_9.chr
  chr/title_screen_bg_new.chr

  inc/charmap.inc
  inc/common.inc
  inc/constants.inc
  inc/globals.inc
  inc/level.inc
  inc/memory.inc
  # inc/mmc3.inc
  inc/vrc7.inc
  inc/nes2header.inc
  inc/object.inc

  # src/bhop.s
  src/chr.s
  src/collision.s
  src/common.s
  # src/dpcm.s
  src/follower.s
  src/gamecore.s
  src/gamemode.s
  src/player.s
  # src/mmc3.s
  src/vrc7.s
  src/memory.s
  src/custom_music.s
  # src/music.s
  src/nesheader.s
  src/reset.s
  src/screen_render.s
  src/sprite_render.s
  src/title_screen.s
  # src/trigtable.s

  src/ssdpcm/buffer.s
  src/ssdpcm/decode_ss2_async_fullunroll.s
  # src/ssdpcm/mapper_funcs.s
  src/ssdpcm/playback_irq.s
  src/ssdpcm/superblock_load.s
  src/ssdpcm/ram_locations_common.s

  # src/bhop/bhop_internal.inc
  # src/bhop/bhop.inc
  # src/bhop/commands.asm
  # src/bhop/config.inc
  # src/bhop/effects.asm
  # src/bhop/longbranch.inc
  # src/bhop/midi_lut.inc
  # src/bhop/vibrato_lut.inc
  # src/bhop/word_util.inc
  # src/bhop/zsaw.asm
  # src/bhop/zsaw.inc

  src/objects/blooper.s
  src/objects/bowser.s
  src/objects/cannon.s
  src/objects/cheepcheep.s
  src/objects/fire_bubble.s
  src/objects/fireball_bubble.s
  src/objects/firebar.s
  src/objects/fireworks.s
  src/objects/frenzy.s
  src/objects/goomba.s
  src/objects/hammer_bro.s
  src/objects/jump_spring.s
  src/objects/koopa.s
  src/objects/lakitu.s
  src/objects/platform.s
  src/objects/podoboo.s
  src/objects/object.s
  src/objects/piranha_plant.s
  src/objects/powerup.s
  src/objects/vine.s
  src/objects/warpzone.s

  src/tiles/area_style.s
  src/tiles/brick.s
  src/tiles/brick_column.s
  src/tiles/bridge.s
  src/tiles/castle.s
  src/tiles/coin_row.s
  src/tiles/flagpole.s
  src/tiles/hole_water.s
  src/tiles/jump_spring.s
  src/tiles/level.s
  src/tiles/pipe.s
  src/tiles/pulley.s
  src/tiles/rope.s
  src/tiles/question_block.s
  src/tiles/staircase.s

  src/nes.cfg
  # chr/lanky_mario.chr
)

add_executable(${CLEAN} ${SRCS})
add_executable(${UNCUT} ${SRCS})

target_sources(${CLEAN} PUBLIC
  audio/paniccut/superblocks_paniccut.s
  audio/paniccut/blocks_paniccut.inc
)
target_compile_options(${CLEAN} PUBLIC -D PRODUCE_CLEAN_VERSION=1)

target_sources(${UNCUT} PUBLIC
  audio/panicuncut/superblocks_panicuncut.s
  audio/panicuncut/blocks_panicuncut.inc
)

target_include_directories(${CLEAN} PUBLIC audio/ inc/ gen/)
target_include_directories(${UNCUT} PUBLIC audio/ inc/ gen/)

include(GenerateLinkerScript)

generate_ca65_linker_cmd(TARGET ${CLEAN} CONFIG src/nes.cfg)
generate_ca65_linker_cmd(TARGET ${UNCUT} CONFIG src/nes.cfg)
