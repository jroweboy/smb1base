

.ifndef __METASPRITE_INC
__METASPRITE_INC = 1

.include "metasprite_p.inc"

; Reserve Metasprite 0 as the "unused" sprite
MetaspriteReserve "NULL"

BIG_MARIO_BANK = SPRITE_BANK_0
BIG_MARIO_PALETTE = $00
BIG_MARIO_Y_OFFSET = -8
MetaspriteDefine "BIG_MARIO", "STANDING",  $00, $02, $20, $22
MetaspriteDefine "BIG_MARIO", "WALKING_1", $04, $06, $24, $26
MetaspriteDefine "BIG_MARIO", "WALKING_2", $08, $0a, $28, $2a
MetaspriteDefine "BIG_MARIO", "WALKING_3", $0c, $0e, $2c, $2e
MetaspriteDefine "BIG_MARIO", "SKIDDING",  $10, $12, $30, $32
MetaspriteDefine "BIG_MARIO", "JUMPING",   $14, $16, $34, $36

MetaspriteDefine "BIG_MARIO", "SWIMMING_1_KICK", $00, $02, $20, $22
MetaspriteDefine "BIG_MARIO", "SWIMMING_2_KICK", $04, $06, $20, $26
MetaspriteDefine "BIG_MARIO", "SWIMMING_3_KICK", $04, $06, $28, $22
MetaspriteDefine "BIG_MARIO", "SWIMMING_1_HOLD", $00, $02, $24, $22
MetaspriteDefine "BIG_MARIO", "SWIMMING_2_HOLD", $04, $06, $24, $26
MetaspriteDefine "BIG_MARIO", "SWIMMING_3_HOLD", $04, $06, $2a, $22

FIRE_MARIO_BANK = SPRITE_BANK_0
FIRE_MARIO_PALETTE = $00
FIRE_MARIO_Y_OFFSET = -8
MetaspriteDefine "FIRE_MARIO", "STANDING",  $00, $02, $2c, $2e
; MetaspriteDefine "BIG_MARIO", "FIRE_WALKING_1", $00, $02, $2c, $2e
MetaspriteDuplicate "FIRE_MARIO_STANDING", "FIRE_MARIO_WALKING_1"
MetaspriteDefine "FIRE_MARIO", "WALKING_2", $00, $02, $30, $32 ; TODO
MetaspriteDefine "FIRE_MARIO", "WALKING_3", $00, $02, $14, $16
MetaspriteDefine "FIRE_MARIO", "SKIDDING",  $00, $02, $34, $36
MetaspriteDefine "FIRE_MARIO", "JUMPING",   $00, $02, $10, $12

MetaspriteDefine "FIRE_MARIO", "SWIMMING_1_KICK", $00, $02, $20, $22
MetaspriteDefine "FIRE_MARIO", "SWIMMING_2_KICK", $04, $06, $20, $26
MetaspriteDefine "FIRE_MARIO", "SWIMMING_3_KICK", $04, $06, $28, $22
MetaspriteDefine "FIRE_MARIO", "SWIMMING_1_HOLD", $00, $02, $24, $22
MetaspriteDefine "FIRE_MARIO", "SWIMMING_2_HOLD", $04, $06, $24, $26
MetaspriteDefine "FIRE_MARIO", "SWIMMING_3_HOLD", $04, $06, $2a, $22

MetaspriteDefine "BIG_MARIO", "CROUCHING",    $18, $1a, $38, $3a
MetaspriteDefine "BIG_MARIO", "CLIMBING_1",   $00, $02, $08, $0a
MetaspriteDefine "BIG_MARIO", "CLIMBING_2",   $04, $06, $0c, $0e

SMALL_MARIO_BANK = SPRITE_BANK_0
SMALL_MARIO_PALETTE = $00
SMALL_MARIO_Y_OFFSET = 8
MetaspriteDefine "SMALL_MARIO", "STANDING",   $00, $02
MetaspriteDefine "SMALL_MARIO", "WALKING_1",  $04, $06
MetaspriteDefine "SMALL_MARIO", "WALKING_2",  $08, $0a
MetaspriteDefine "SMALL_MARIO", "WALKING_3",  $0c, $0e
MetaspriteDefine "SMALL_MARIO", "SKIDDING",   $10, $12
MetaspriteDefine "SMALL_MARIO", "JUMPING",    $14, $16

MetaspriteDefine "SMALL_MARIO", "SWIMMING_1_KICK", $20, $22
MetaspriteDefine "SMALL_MARIO", "SWIMMING_2_KICK", $28, $2a
MetaspriteDefine "SMALL_MARIO", "SWIMMING_3_KICK", $30, $32
MetaspriteDefine "SMALL_MARIO", "SWIMMING_1_HOLD", $24, $26
MetaspriteDefine "SMALL_MARIO", "SWIMMING_2_HOLD", $2c, $2e
MetaspriteDuplicate "SMALL_MARIO_SWIMMING_3_KICK", "SMALL_MARIO_SWIMMING_3_HOLD"

SMALL_FIRE_BANK = SPRITE_BANK_0
SMALL_FIRE_PALETTE = $00
SMALL_FIRE_Y_OFFSET = -8
MetaspriteDefine "SMALL_FIRE", "STANDING",   $00, $02, $20, $22
MetaspriteDefine "SMALL_FIRE", "WALKING_1",  $00, $02, $04, $06
MetaspriteDefine "SMALL_FIRE", "WALKING_2",  $00, $02, $08, $0a
MetaspriteDefine "SMALL_FIRE", "WALKING_3",  $00, $02, $0c, $0e
MetaspriteDefine "SMALL_FIRE", "SKIDDING",   $00, $02, $10, $12
MetaspriteDefine "SMALL_FIRE", "JUMPING",    $00, $02, $14, $16

MetaspriteDefine "SMALL_FIRE", "SWIMMING_1_KICK", $00, $02, $24, $26
MetaspriteDefine "SMALL_FIRE", "SWIMMING_2_KICK", $00, $02, $30, $32
MetaspriteDefine "SMALL_FIRE", "SWIMMING_3_KICK", $00, $02, $34, $36
MetaspriteDefine "SMALL_FIRE", "SWIMMING_1_HOLD", $00, $02, $28, $2a
MetaspriteDefine "SMALL_FIRE", "SWIMMING_2_HOLD", $00, $02, $2c, $2e
MetaspriteDuplicate "SMALL_FIRE_SWIMMING_3_KICK", "SMALL_FIRE_SWIMMING_3_HOLD"

MetaspriteDefine "SMALL_MARIO", "CLIMBING_1", $18, $1a
MetaspriteDefine "SMALL_MARIO", "CLIMBING_2", $38, $3a
MetaspriteDefine "SMALL_MARIO", "DEATH",      $34, $36

MetaspriteDuplicate "SMALL_MARIO_STANDING", "SMALL_MARIO_GROW_STANDING"
MetaspriteDefine "BIG_MARIO", "GROW_INTERMEDIATE", $18, $1a, $38, $3a
MetaspriteDuplicate "BIG_MARIO_STANDING", "BIG_MARIO_GROW_STANDING"

POWERUP_BANK = SPRITE_BANK_1
; POWERUP_Y_OFFSET = 8
POWERUP_1UP_PALETTE = $01
POWERUP_STAR_PALETTE = $02
POWERUP_FIREFLOWER_PALETTE = $01
POWERUP_FIREFLOWER_Y_OFFSET = -8
POWERUP_MUSHROOM_PALETTE = $02
POWERUP_1UP_NO_MIRROR = 1
POWERUP_MUSHROOM_NO_MIRROR = 1
MetaspriteDefine "POWERUP", "STAR",       $34, $36
MetaspriteDefine "POWERUP", "FIREFLOWER", $38, $38 | SPR_FLIP_H , $3a | SPR_NO_PALETTE, $3a | SPR_FLIP_H | SPR_NO_PALETTE
MetaspriteDefine "POWERUP", "MUSHROOM",   $3c, $3e
MetaspriteDefine "POWERUP", "1UP",        $3c, $3e

FIREBALL_BANK = SPRITE_BANK_0
FIREBALL_PALETTE = $02
FIREBALL_Y_OFFSET = -12
EXPLOSION_BANK = SPRITE_BANK_1
EXPLOSION_PALETTE = $02
EXPLOSION_Y_OFFSET = -12  ; Vanilla offsets the explosion by -4. The other -8 is to account for position due to 8x16 sprites?
MetaspriteDefine "FIREBALL", "FRAME_1", $3c
MetaspriteDefine "FIREBALL", "FRAME_2", $3e
MetaspriteDefine "EXPLOSION", "FRAME_1", $26, $26 | SPR_FLIP_H
MetaspriteDefine "EXPLOSION", "FRAME_2", $28, $28 | SPR_FLIP_H
MetaspriteDefine "EXPLOSION", "FRAME_3", $2a, $2a | SPR_FLIP_H
; Firebars will still draw Fireballs manually for now 

COIN_BANK = SPRITE_BANK_1
COIN_PALETTE = $02
MetaspriteDefine "COIN", "FRAME_1", $2c
MetaspriteDefine "COIN", "FRAME_2", $2e
MetaspriteDefine "COIN", "FRAME_3", $30
MetaspriteDefine "COIN", "FRAME_4", $32

MISC_BANK = SPRITE_BANK_1
MISC_PALETTE = $03
MISC_Y_OFFSET = -8
MetaspriteDefine "MISC", "BRICK", $22, $22
MetaspriteDefine "MISC", "BLOCK", $24, $24 | SPR_FLIP_H
; drawn manually for now until i figure out if i have frames for the brick
; break animation with just metasprites
; MetaspriteDefine "MISC", "CHUNK", $16
MISC_BUBBLE_PALETTE = $02
MetaspriteDefine "MISC", "BUBBLE", $18
MetaspriteDefine "MISC", "SMALL_OVERLAY", $1e, $1e
MetaspriteDefine "MISC", "LARGE_OVERLAY", $1e, $1e, $1e, $1e
MISC_STAR_FLAG_BANK = SPRITE_BANK_2
MISC_STAR_FLAG_PALETTE = $02
MetaspriteDefine "MISC", "STAR_FLAG", $3c, $3e
MISC_FLAGPOLE_FLAG_BANK = SPRITE_BANK_3
MISC_FLAGPOLE_FLAG_PALETTE = $01
MetaspriteDefine "MISC", "FLAGPOLE_FLAG", $1c, $1e

PLATFORM_BANK = SPRITE_BANK_1
PLATFORM_PALETTE = $02
MetaspriteReserve "PLATFORM_SMALL"
MetaspriteReserve "PLATFORM_SMALL_FRAME_2"
MetaspriteReserve "PLATFORM_LARGE"
MetaspriteReserve "PLATFORM_LARGE_FRAME_2"

NUMBER_BANK = SPRITE_BANK_1
NUMBER_PALETTE = $02
NUMBER_Y_OFFSET = -16

MetaspriteDefine "NUMBER", "100", $06, $10
MetaspriteDefine "NUMBER", "200", $08, $10
MetaspriteDefine "NUMBER", "400", $0a, $10
MetaspriteDefine "NUMBER", "500", $0c, $10
MetaspriteDefine "NUMBER", "800", $0e, $10
MetaspriteDefine "NUMBER", "1000", $06, $04
MetaspriteDefine "NUMBER", "2000", $08, $04
MetaspriteDefine "NUMBER", "4000", $0a, $04
MetaspriteDefine "NUMBER", "5000", $0c, $04
MetaspriteDefine "NUMBER", "8000", $0e, $04
MetaspriteDefine "NUMBER", "1UP", $12, $14


GOOMBA_BANK = SPRITE_BANK_2
GOOMBA_PALETTE = $03
GOOMBA_DEAD_Y_OFFSET = $08
MetaspriteDefine "GOOMBA", "WALKING_1", $22, $24
MetaspriteDefine "GOOMBA", "WALKING_2", $24 | SPR_FLIP_H, $22 | SPR_FLIP_H
MetaspriteDefine "GOOMBA", "DEAD", $26, $26 | SPR_FLIP_H


KOOPA_BANK = SPRITE_BANK_2
KOOPA_Y_OFFSET = -8
KOOPA_SHELL_Y_OFFSET = 2
KOOPA_SHELL_REVIVE_Y_OFFSET = 2
KOOPA_PALETTE = $00 ; Don't set a palette here so we can do red or green koopa later
MetaspriteDefine "KOOPA", "WALKING_1", $38, $12, $30, $32
MetaspriteDefine "KOOPA", "WALKING_2", $18, $16, $34, $36
MetaspriteDefine "KOOPA", "SHELL", $1a, $1a | SPR_FLIP_H
MetaspriteDefine "KOOPA", "SHELL_REVIVE", $3a, $3a | SPR_FLIP_H
MetaspriteDefine "KOOPA", "FLYING_1", $10, $12, $30, $32
MetaspriteDefine "KOOPA", "FLYING_2", $14, $16, $34, $36


PIRANHA_BANK = SPRITE_BANK_2
PIRANHA_PALETTE = $01
PIRANHA_MOUTH_OPEN_Y_OFFSET = -8
PIRANHA_MOUTH_CLOSED_Y_OFFSET = -8
MetaspriteDefine "PIRANHA", "MOUTH_OPEN", $56, $56 | SPR_FLIP_H, $76, $76 | SPR_FLIP_H
MetaspriteDefine "PIRANHA", "MOUTH_CLOSED", $58, $58 | SPR_FLIP_H, $78, $78 | SPR_FLIP_H

MetaspriteReserve "BOWSER"

.endif