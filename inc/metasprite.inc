

.ifndef __METASPRITE_INC
__METASPRITE_INC = 1

.include "metasprite_p.inc"

; Reserve Metasprite 0 as the "unused" sprite
MetaspriteReserve "NULL"

BIG_MARIO_BANK = SPRITE_BANK_0
BIG_MARIO_PALETTE = $00
BIG_MARIO_Y_OFFSET = -8
MetaspriteBox "BIG_MARIO", "STANDING",  $00, $02, $20, $22
MetaspriteBox "BIG_MARIO", "WALKING_1", $04, $06, $24, $26
MetaspriteBox "BIG_MARIO", "WALKING_2", $08, $0a, $28, $2a
MetaspriteBox "BIG_MARIO", "WALKING_3", $0c, $0e, $2c, $2e
MetaspriteBox "BIG_MARIO", "SKIDDING",  $10, $12, $30, $32
MetaspriteBox "BIG_MARIO", "JUMPING",   $14, $16, $34, $36

MetaspriteBox "BIG_MARIO", "SWIMMING_1_KICK", $00, $02, $20, $22
MetaspriteBox "BIG_MARIO", "SWIMMING_2_KICK", $04, $06, $20, $26
MetaspriteBox "BIG_MARIO", "SWIMMING_3_KICK", $04, $06, $28, $22
MetaspriteBox "BIG_MARIO", "SWIMMING_1_HOLD", $00, $02, $24, $22
MetaspriteBox "BIG_MARIO", "SWIMMING_2_HOLD", $04, $06, $24, $26
MetaspriteBox "BIG_MARIO", "SWIMMING_3_HOLD", $04, $06, $2a, $22

FIRE_MARIO_BANK = SPRITE_BANK_0
FIRE_MARIO_PALETTE = $00
FIRE_MARIO_Y_OFFSET = -8
MetaspriteBox "FIRE_MARIO", "STANDING",  $00, $02, $2c, $2e
; MetaspriteBox "BIG_MARIO", "FIRE_WALKING_1", $00, $02, $2c, $2e
MetaspriteDuplicate "FIRE_MARIO_STANDING", "FIRE_MARIO_WALKING_1"
MetaspriteBox "FIRE_MARIO", "WALKING_2", $00, $02, $30, $32 ; TODO
MetaspriteBox "FIRE_MARIO", "WALKING_3", $00, $02, $14, $16
MetaspriteBox "FIRE_MARIO", "SKIDDING",  $00, $02, $34, $36
MetaspriteBox "FIRE_MARIO", "JUMPING",   $00, $02, $10, $12

MetaspriteBox "FIRE_MARIO", "SWIMMING_1_KICK", $00, $02, $20, $22
MetaspriteBox "FIRE_MARIO", "SWIMMING_2_KICK", $04, $06, $20, $26
MetaspriteBox "FIRE_MARIO", "SWIMMING_3_KICK", $04, $06, $28, $22
MetaspriteBox "FIRE_MARIO", "SWIMMING_1_HOLD", $00, $02, $24, $22
MetaspriteBox "FIRE_MARIO", "SWIMMING_2_HOLD", $04, $06, $24, $26
MetaspriteBox "FIRE_MARIO", "SWIMMING_3_HOLD", $04, $06, $2a, $22

MetaspriteBox "BIG_MARIO", "CROUCHING",    $18, $1a, $38, $3a
MetaspriteBox "BIG_MARIO", "CLIMBING_1",   $00, $02, $08, $0a
MetaspriteBox "BIG_MARIO", "CLIMBING_2",   $04, $06, $0c, $0e

SMALL_MARIO_BANK = SPRITE_BANK_0
SMALL_MARIO_PALETTE = $00
SMALL_MARIO_Y_OFFSET = 8
MetaspriteBox "SMALL_MARIO", "STANDING",   $00, $02
MetaspriteBox "SMALL_MARIO", "WALKING_1",  $04, $06
MetaspriteBox "SMALL_MARIO", "WALKING_2",  $08, $0a
MetaspriteBox "SMALL_MARIO", "WALKING_3",  $0c, $0e
MetaspriteBox "SMALL_MARIO", "SKIDDING",   $10, $12
MetaspriteBox "SMALL_MARIO", "JUMPING",    $14, $16

MetaspriteBox "SMALL_MARIO", "SWIMMING_1_KICK", $20, $22
MetaspriteBox "SMALL_MARIO", "SWIMMING_2_KICK", $28, $2a
MetaspriteBox "SMALL_MARIO", "SWIMMING_3_KICK", $30, $32
MetaspriteBox "SMALL_MARIO", "SWIMMING_1_HOLD", $24, $26
MetaspriteBox "SMALL_MARIO", "SWIMMING_2_HOLD", $2c, $2e
MetaspriteDuplicate "SMALL_MARIO_SWIMMING_3_KICK", "SMALL_MARIO_SWIMMING_3_HOLD"

SMALL_FIRE_BANK = SPRITE_BANK_0
SMALL_FIRE_PALETTE = $00
SMALL_FIRE_Y_OFFSET = -8
MetaspriteBox "SMALL_FIRE", "STANDING",   $00, $02, $20, $22
MetaspriteBox "SMALL_FIRE", "WALKING_1",  $00, $02, $04, $06
MetaspriteBox "SMALL_FIRE", "WALKING_2",  $00, $02, $08, $0a
MetaspriteBox "SMALL_FIRE", "WALKING_3",  $00, $02, $0c, $0e
MetaspriteBox "SMALL_FIRE", "SKIDDING",   $00, $02, $10, $12
MetaspriteBox "SMALL_FIRE", "JUMPING",    $00, $02, $14, $16

MetaspriteBox "SMALL_FIRE", "SWIMMING_1_KICK", $00, $02, $24, $26
MetaspriteBox "SMALL_FIRE", "SWIMMING_2_KICK", $00, $02, $30, $32
MetaspriteBox "SMALL_FIRE", "SWIMMING_3_KICK", $00, $02, $34, $36
MetaspriteBox "SMALL_FIRE", "SWIMMING_1_HOLD", $00, $02, $28, $2a
MetaspriteBox "SMALL_FIRE", "SWIMMING_2_HOLD", $00, $02, $2c, $2e
MetaspriteDuplicate "SMALL_FIRE_SWIMMING_3_KICK", "SMALL_FIRE_SWIMMING_3_HOLD"

MetaspriteBox "SMALL_MARIO", "CLIMBING_1", $18, $1a
MetaspriteBox "SMALL_MARIO", "CLIMBING_2", $38, $3a
MetaspriteBox "SMALL_MARIO", "DEATH",      $34, $36

MetaspriteDuplicate "SMALL_MARIO_STANDING", "SMALL_MARIO_GROW_STANDING"
MetaspriteBox "BIG_MARIO", "GROW_INTERMEDIATE", $18, $1a, $38, $3a
MetaspriteDuplicate "BIG_MARIO_STANDING", "BIG_MARIO_GROW_STANDING"

POWERUP_BANK = SPRITE_BANK_1
; POWERUP_Y_OFFSET = 8
POWERUP_1UP_PALETTE = $01
POWERUP_STAR_PALETTE = $00
POWERUP_FIREFLOWER_PALETTE = $00
POWERUP_FIREFLOWER_Y_OFFSET = -8
POWERUP_MUSHROOM_PALETTE = $02
POWERUP_1UP_NO_MIRROR = 1
POWERUP_MUSHROOM_NO_MIRROR = 1
MetaspriteBox "POWERUP", "STAR",       $34, $36
; Force the lower two parts of the fireflower to be palette two by using the override
MetaspriteBox "POWERUP", "FIREFLOWER", $38, $38 | SPR_FLIP_H , $3a | SPR_PALETTE_1, $3a | SPR_FLIP_H | SPR_PALETTE_1
MetaspriteBox "POWERUP", "MUSHROOM",   $3c, $3e
MetaspriteBox "POWERUP", "1UP",        $3c, $3e

FIREBALL_BANK = SPRITE_BANK_0
FIREBALL_PALETTE = $02
FIREBALL_Y_OFFSET = -12
EXPLOSION_BANK = SPRITE_BANK_1
EXPLOSION_PALETTE = $02
EXPLOSION_Y_OFFSET = -12  ; Vanilla offsets the explosion by -4. The other -8 is to account for position due to 8x16 sprites?
MetaspriteBox "FIREBALL", "FRAME_1", $3c
MetaspriteBox "FIREBALL", "FRAME_2", $3e
MetaspriteBox "EXPLOSION", "FRAME_1", $26, $26 | SPR_FLIP_H
MetaspriteBox "EXPLOSION", "FRAME_2", $28, $28 | SPR_FLIP_H
MetaspriteBox "EXPLOSION", "FRAME_3", $2a, $2a | SPR_FLIP_H
; Firebars will still draw Fireballs manually for now 

COIN_BANK = SPRITE_BANK_1
COIN_PALETTE = $02
MetaspriteBox "COIN", "FRAME_1", $2c
MetaspriteBox "COIN", "FRAME_2", $2e
MetaspriteBox "COIN", "FRAME_3", $30
MetaspriteBox "COIN", "FRAME_4", $32

MISC_BANK = SPRITE_BANK_1
MISC_PALETTE = $03
MISC_Y_OFFSET = -8

; The brick and block sprite palettes are set by the code
MISC_BRICK_GROUND_PALETTE = $00
MISC_BRICK_OTHER_PALETTE = $00 ; The brick palette is set by the code
MISC_BLOCK_PALETTE = $00 ; The block palette is set by the code
MetaspriteBox "MISC", "BRICK_OTHER", $20, $20
MetaspriteBox "MISC", "BRICK_GROUND", $22, $22
MetaspriteBox "MISC", "BLOCK", $24, $24 | SPR_FLIP_H
; drawn manually for now until i figure out if i have frames for the brick
; break animation with just metasprites
; MetaspriteBox "MISC", "CHUNK", $16
MISC_BUBBLE_PALETTE = $02
MetaspriteBox "MISC", "BUBBLE", $18
MetaspriteBox "MISC", "SMALL_OVERLAY", $1e, $1e
MetaspriteBox "MISC", "LARGE_OVERLAY", $1e, $1e, $1e, $1e
MISC_STAR_FLAG_BANK = SPRITE_BANK_2
MISC_STAR_FLAG_PALETTE = $02
MetaspriteBox "MISC", "STAR_FLAG", $3c, $3e
MISC_FLAGPOLE_FLAG_BANK = SPRITE_BANK_3
MISC_FLAGPOLE_FLAG_PALETTE = $01
MetaspriteBox "MISC", "FLAGPOLE_FLAG", $32, $34

PLATFORM_BANK = SPRITE_BANK_1
PLATFORM_PALETTE = $02
MetaspriteReserve "PLATFORM_GIRDER_SMALL"
MetaspriteReserve "PLATFORM_GIRDER_SMALL_FRAME_2"
MetaspriteReserve "PLATFORM_GIRDER_LARGE"
MetaspriteReserve "PLATFORM_GIRDER_LARGE_FRAME_2"
MetaspriteReserve "PLATFORM_CLOUD_SMALL"
MetaspriteReserve "PLATFORM_CLOUD_SMALL_FRAME_2"
MetaspriteReserve "PLATFORM_CLOUD_LARGE"
MetaspriteReserve "PLATFORM_CLOUD_LARGE_FRAME_2"

NUMBER_BANK = SPRITE_BANK_1
NUMBER_PALETTE = $02
NUMBER_Y_OFFSET = -16

MetaspriteBox "NUMBER", "100", $06, $10
MetaspriteBox "NUMBER", "200", $08, $10
MetaspriteBox "NUMBER", "400", $0a, $10
MetaspriteBox "NUMBER", "500", $0c, $10
MetaspriteBox "NUMBER", "800", $0e, $10
MetaspriteBox "NUMBER", "1000", $06, $04
MetaspriteBox "NUMBER", "2000", $08, $04
MetaspriteBox "NUMBER", "4000", $0a, $04
MetaspriteBox "NUMBER", "5000", $0c, $04
MetaspriteBox "NUMBER", "8000", $0e, $04
MetaspriteBox "NUMBER", "1UP", $12, $14


GOOMBA_BANK = SPRITE_BANK_2
GOOMBA_PALETTE = $03
GOOMBA_DEAD_Y_OFFSET = $08
MetaspriteBox "GOOMBA", "WALKING_1", $22, $24
MetaspriteBox "GOOMBA", "WALKING_2", $24 | SPR_FLIP_H, $22 | SPR_FLIP_H
MetaspriteBox "GOOMBA", "DEAD", $26, $26 | SPR_FLIP_H


KOOPA_BANK = SPRITE_BANK_2
KOOPA_Y_OFFSET = -8
KOOPA_SHELL_Y_OFFSET = 2
KOOPA_SHELL_REVIVE_Y_OFFSET = 2
KOOPA_PALETTE = $00 ; Don't set a palette here so we can do red or green koopa later
MetaspriteBox "KOOPA", "WALKING_1", $38, $12, $30, $32
MetaspriteBox "KOOPA", "WALKING_2", $18, $16, $34, $36
MetaspriteBox "KOOPA", "SHELL", $1a, $1a | SPR_FLIP_H
MetaspriteBox "KOOPA", "SHELL_REVIVE", $3a, $3a | SPR_FLIP_H
MetaspriteBox "KOOPA", "FLYING_1", $10, $12, $30, $32
MetaspriteBox "KOOPA", "FLYING_2", $14, $16, $34, $36


PIRANHA_BANK = SPRITE_BANK_3
PIRANHA_PALETTE = $01
PIRANHA_Y_OFFSET = -8
MetaspriteBox "PIRANHA", "MOUTH_OPEN", $1c, $1c | SPR_FLIP_H, $3c, $3c | SPR_FLIP_H
MetaspriteBox "PIRANHA", "MOUTH_CLOSED", $1e, $1e | SPR_FLIP_H, $3e, $3e | SPR_FLIP_H

BULLET_BANK = SPRITE_BANK_2
BULLET_PALETTE = $03
MetaspriteBox "BULLET", "BILL", $1c, $1e

PODOBOO_BANK = SPRITE_BANK_2
PODOBOO_PALETTE = $02
MetaspriteBox "PODOBOO", "UP", $0e, $0e | SPR_FLIP_H


BUZZY_BEETLE_BANK = SPRITE_BANK_2
BUZZY_BEETLE_PALETTE = $03
BUZZY_BEETLE_SHELL_Y_OFFSET = 2
MetaspriteBox "BUZZY_BEETLE", "WALKING_1", $00, $02
MetaspriteBox "BUZZY_BEETLE", "WALKING_2", $04, $06
MetaspriteBox "BUZZY_BEETLE", "SHELL", $20, $20 | SPR_FLIP_H


CHEEP_CHEEP_BANK = SPRITE_BANK_2
CHEEP_CHEEP_PALETTE = $00
MetaspriteBox "CHEEP_CHEEP", "SWIM_1", $2c, $2e
MetaspriteBox "CHEEP_CHEEP", "SWIM_2", $2a, $2e


BLOOPER_BANK = SPRITE_BANK_3
BLOOPER_PALETTE = $03
BLOOPER_Y_OFFSET = -8
BLOOPER_SWIM_2_Y_OFFSET = (-8 + 3) ; original code adds 3 px to the y position
MetaspriteBox "BLOOPER", "SWIM_1", $1a, $1a | SPR_FLIP_H, $3a, $3a | SPR_FLIP_H
MetaspriteBox "BLOOPER", "SWIM_2", $18, $18 | SPR_FLIP_H

HAMMER_BANK = SPRITE_BANK_1
HAMMER_FRAME_1_X_OFFSET = 4
HAMMER_FRAME_1_Y_OFFSET = -8
HAMMER_FRAME_2_X_OFFSET = 0
HAMMER_FRAME_2_Y_OFFSET = -8
HAMMER_PALETTE = $03
MetaspriteBox "HAMMER", "FRAME_1", $1e
MetaspriteBox "HAMMER", "FRAME_2", $00, $02

HAMMER_BRO_BANK = SPRITE_BANK_3
HAMMER_BRO_PALETTE = $01
HAMMER_BRO_Y_OFFSET = -8
MetaspriteBox "HAMMER_BRO", "WALK_1", $00, $02, $20, $22
MetaspriteBox "HAMMER_BRO", "WALK_2", $08, $0a, $24, $26
MetaspriteBox "HAMMER_BRO", "THROW_1", $04, $06, $20, $22
MetaspriteBox "HAMMER_BRO", "THROW_2", $04, $06, $24, $26

BOWSER_BANK = SPRITE_BANK_3
BOWSER_PALETTE = $01
BOWSER_Y_OFFSET = -8
MetaspriteBox "BOWSER", "FRONT_MOUTH_OPEN", $10, $12, $30
MetaspriteBox "BOWSER", "FRONT_MOUTH_CLOSED", $14, $16, $30
MetaspriteBox "BOWSER", "REAR_WALK_1", $0c, $0e, $28, $2a
MetaspriteBox "BOWSER", "REAR_WALK_2", $0c, $0e, $2c, $2e

MetaspriteReserve "BOWSER_FLAME"

LAKITU_BANK = SPRITE_BANK_3
LAKITU_PALETTE = $01
LAKITU_NORMAL_Y_OFFSET = -16
LAKITU_THROWING_Y_OFFSET = 0
MetaspriteBox "LAKITU", "NORMAL", $0c, $0e, $2c, $2e
MetaspriteBox "LAKITU", "THROWING", $30, $30 | SPR_FLIP_H


SPINY_BANK = SPRITE_BANK_2
SPINY_PALETTE = $02
SPINY_EGG_1_BANK = SPRITE_BANK_3
SPINY_EGG_2_BANK = SPRITE_BANK_3
MetaspriteBox "SPINY", "WALK_1", $08, $0a
MetaspriteBox "SPINY", "WALK_2", $0c, $0e
MetaspriteBox "SPINY", "EGG_1", $10, $10 | SPR_FLIP_H | SPR_FLIP_V
MetaspriteBox "SPINY", "EGG_2", $12, $12 | SPR_FLIP_H | SPR_FLIP_V


JUMPSPRING_BANK = SPRITE_BANK_3
JUMPSPRING_PALETTE = $02
JUMPSPRING_Y_OFFSET = -8
MetaspriteBox "JUMPSPRING", "FRAME_1", $16, $16 | SPR_FLIP_H , $36, $36 | SPR_FLIP_H
MetaspriteBox "JUMPSPRING", "FRAME_2", $38, $38 | SPR_FLIP_H
MetaspriteBox "JUMPSPRING", "FRAME_3", $14, $14


PEACH_BANK = SPRITE_BANK_2
PEACH_PALETTE = $02
PEACH_Y_OFFSET = -8
MetaspriteBox "PEACH", "STANDING", $08, $0a, $28, $28 | SPR_FLIP_H
TOAD_BANK = SPRITE_BANK_2
TOAD_PALETTE = $02
TOAD_Y_OFFSET = -8
MetaspriteBox "TOAD", "STANDING", $3c, $3c | SPR_FLIP_H, $3e, $3e | SPR_FLIP_H


.endif