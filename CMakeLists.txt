cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME smb1base)

project(${PROJECT_NAME} ASM C)

add_executable(${PROJECT_NAME}
  
  chr/bg_castle.chr
  chr/bg_ground.chr
  chr/bg_underground.chr
  chr/bg_water.chr
  chr/sprites_castle.chr
  chr/sprites_mario.chr
  chr/sprites_misc.chr
  chr/sprites_overworld.chr
  chr/sprites_underground.chr
  chr/sprites_water.chr

  inc/charmap.inc
  inc/common.inc
  inc/constants.inc
  inc/globals.inc
  inc/level.inc
  inc/memory.inc
  inc/metasprite.inc
  inc/metasprite_custom.inc
  inc/metasprite_p.inc
  inc/mmc3.inc
  inc/nes2header.inc
  inc/object.inc

  src/chr.s
  src/collision.s
  src/common.s
  src/gamecore.s
  src/gamemode.s
  src/player.s
  src/mmc3.s
  src/memory.s
  src/metasprite.s
  src/music.s
  src/nesheader.s
  src/reset.s
  src/screen_render.s
  src/sprite_render.s

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
)

target_include_directories(${PROJECT_NAME} PUBLIC inc/)

include(GenerateLinkerScript)

generate_ca65_linker_cmd(TARGET "${PROJECT_NAME}" CONFIG src/nes.cfg)
