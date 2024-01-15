

.ifndef __METASPRITE_INC
__METASPRITE_INC = 1

PATTERN_TABLE_1 = $01
SPRITE_BANK_0 = $00
SPRITE_BANK_1 = $40
SPRITE_BANK_2 = $80
SPRITE_BANK_3 = $c0

; Custom values to signify that this sprite should be flipped in the metasprite
SPR_FLIP_H = %01000000 << 8
SPR_FLIP_V = %10000000 << 8

METASPRITES_COUNT .set 1
.macro MetaspriteDefine Object, Animation, Spr1, Spr2, Spr3, Spr4
.local Bank
.local Palette
.local YOffset

.if .defined(.ident( .sprintf("%s_%s_BANK", Object, Animation) ))
	Bank = .ident( .sprintf("%s_%s_BANK", Object, Animation) )
.elseif .defined(.ident( .sprintf("%s_BANK", Object) ))
	Bank = .ident( .sprintf("%s_BANK", Object) )
.else
	.error .sprintf("Could not define Metasprite without the bank. Please define either %s_BANK or %s_%s_BANK", Object, Object, Animation)
.endif
.if .defined(.ident( .sprintf("%s_%s_PALETTE", Object, Animation) ))
	Palette = .ident( .sprintf("%s_%s_PALETTE", Object, Animation) )
.elseif .defined(.ident( .sprintf("%s_PALETTE", Object) ))
	Palette = .ident( .sprintf("%s_PALETTE", Object) )
.else
	.error .sprintf("Could not define Metasprite without the palette. Please define either %s_PALETTE or %s_%s_PALETTE", Object, Object, Animation)
.endif

.if .defined(.ident( .sprintf("%s_%s_Y_OFFSET", Object, Animation) ))
	YOffset = <.ident( .sprintf("%s_%s_Y_OFFSET", Object, Animation) )
.elseif .defined(.ident( .sprintf("%s_Y_OFFSET", Object) ))
	YOffset = <.ident( .sprintf("%s_Y_OFFSET", Object) )
.else
	YOffset = 0
.endif

.ifdef METASPRITE_BODY
.ident( .sprintf("MetaspriteData_%s_%s", Object, Animation) ):
  .byte   (.paramcount - 2) * 4
.ident( .sprintf("%s_%s_%s", Object, Animation, "TOP_LEFT") ) = <Spr1
	.byte   0,8+YOffset,<Spr1 | Bank | PATTERN_TABLE_1, Palette | >Spr1
.ifnblank Spr2
.ident( .sprintf("%s_%s_%s", Object, Animation, "TOP_RIGHT") ) = <Spr2
	.byte   8,8+YOffset,<Spr2 | Bank | PATTERN_TABLE_1, Palette | >Spr2
.endif
.ifnblank Spr3
.ident( .sprintf("%s_%s_%s", Object, Animation, "BOT_LEFT") ) = <Spr3
	.byte   0,24+YOffset,<Spr3 | Bank | PATTERN_TABLE_1, Palette | >Spr3
.endif
.ifnblank Spr4
.ident( .sprintf("%s_%s_%s", Object, Animation, "BOT_RIGHT") ) = <Spr4
	.byte   8,24+YOffset,<Spr4 | Bank | PATTERN_TABLE_1, Palette | >Spr4
.endif
.ident( .sprintf("METASPRITE_%d_LO",  METASPRITES_COUNT) ) = .lobyte(.ident( .sprintf("MetaspriteData_%s_%s", Object, Animation) ))
.ident( .sprintf("METASPRITE_%d_HI",  METASPRITES_COUNT) ) = .hibyte(.ident( .sprintf("MetaspriteData_%s_%s", Object, Animation) ))
.endif

.ident( .sprintf("METASPRITE_%s_%s", Object, Animation) ) = METASPRITES_COUNT
.out .sprintf("METASPRITE_%s_%s = %d", Object, Animation, METASPRITES_COUNT)
METASPRITES_COUNT .set METASPRITES_COUNT + 1
.endmacro

.macro MetaspriteDuplicate Mspr, Name
.ifdef METASPRITE_BODY
.ident( .sprintf("METASPRITE_%d_LO",  METASPRITES_COUNT) ) = .lobyte(.ident(Mspr))
.ident( .sprintf("METASPRITE_%d_HI",  METASPRITES_COUNT) ) = .hibyte(.ident(Mspr))
.endif

.ident( Name ) = METASPRITES_COUNT
METASPRITES_COUNT .set METASPRITES_COUNT + 1
.endmacro

BIG_MARIO_BANK = SPRITE_BANK_0
BIG_MARIO_PALETTE = $00
BIG_MARIO_Y_OFFSET = -8
MetaspriteDefine "BIG_MARIO", "STANDING",  $00, $02, $20, $22
MetaspriteDefine "BIG_MARIO", "WALKING_1", $04, $06, $24, $26
MetaspriteDefine "BIG_MARIO", "WALKING_2", $08, $0a, $28, $2a
MetaspriteDefine "BIG_MARIO", "WALKING_3", $0c, $0e, $2c, $2e
MetaspriteDefine "BIG_MARIO", "SKIDDING",  $10, $12, $30, $32
MetaspriteDefine "BIG_MARIO", "JUMPING",   $10, $12, $30, $32
MetaspriteDefine "BIG_MARIO", "CROUCHING", $18, $1a, $38, $3a

MetaspriteDefine "BIG_MARIO", "SWIMMING_1_KICK", $00, $02, $20, $22
MetaspriteDefine "BIG_MARIO", "SWIMMING_1_HOLD", $00, $02, $24, $22
MetaspriteDefine "BIG_MARIO", "SWIMMING_2_KICK", $04, $06, $20, $26
MetaspriteDefine "BIG_MARIO", "SWIMMING_2_HOLD", $04, $06, $24, $26
MetaspriteDefine "BIG_MARIO", "SWIMMING_3_KICK", $04, $06, $28, $22
MetaspriteDefine "BIG_MARIO", "SWIMMING_3_HOLD", $04, $06, $2a, $22

MetaspriteDefine "BIG_MARIO", "FIRE_STANDING",  $00, $02, $20, $22
; MetaspriteDefine "BIG_MARIO", "FIRE_WALKING_1", $04, $06, $24, $26
; MetaspriteDefine "BIG_MARIO", "FIRE_WALKING_2", $08, $0a, $28, $2a
; MetaspriteDefine "BIG_MARIO", "FIRE_WALKING_3", $0c, $0e, $2c, $2e
; MetaspriteDefine "BIG_MARIO", "FIRE_SKIDDING",  $10, $12, $30, $32
; MetaspriteDefine "BIG_MARIO", "FIRE_JUMPING",   $14, $16, $34, $36
MetaspriteDefine "BIG_MARIO", "CLIMBING_1",   $00, $02, $08, $0a
MetaspriteDefine "BIG_MARIO", "CLIMBING_2",   $04, $06, $0c, $0e

SMALL_MARIO_BANK = SPRITE_BANK_0
SMALL_MARIO_PALETTE = $00
SMALL_MARIO_Y_OFFSET = 9
MetaspriteDefine "SMALL_MARIO", "STANDING",   $00, $02
MetaspriteDefine "SMALL_MARIO", "WALKING_1",  $04, $06
MetaspriteDefine "SMALL_MARIO", "WALKING_2",  $08, $0a
MetaspriteDefine "SMALL_MARIO", "WALKING_3",  $0c, $0e
MetaspriteDefine "SMALL_MARIO", "SKIDDING",   $10, $12
MetaspriteDefine "SMALL_MARIO", "JUMPING",    $14, $16
MetaspriteDefine "SMALL_MARIO", "CLIMBING_1", $18, $1a

MetaspriteDefine "SMALL_MARIO", "SWIMMING_1_KICK", $20, $22
MetaspriteDefine "SMALL_MARIO", "SWIMMING_1_HOLD", $24, $26
MetaspriteDefine "SMALL_MARIO", "SWIMMING_2_KICK", $28, $2a
MetaspriteDefine "SMALL_MARIO", "SWIMMING_2_HOLD", $2c, $2e
MetaspriteDefine "SMALL_MARIO", "SWIMMING_3_KICK", $30, $32
MetaspriteDuplicate "METASPRITE_SMALL_MARIO_SWIMMING_3_KICK", "SMALL_MARIO_SWIMMING_3_HOLD"
MetaspriteDefine "SMALL_MARIO", "DEATH",           $34, $36
MetaspriteDefine "SMALL_MARIO", "CLIMBING_2",      $38, $3a

POWERUP_BANK = SPRITE_BANK_1
POWERUP_Y_OFFSET = 8
POWERUP_1UP_PALETTE = $01
POWERUP_STAR_PALETTE = $02
POWERUP_FIREFLOWER_PALETTE = $01
POWERUP_MUSHROOM_PALETTE = $02
MetaspriteDefine "POWERUP", "STAR",       $34, $36
MetaspriteDefine "POWERUP", "FIREFLOWER", $38, $3a
MetaspriteDefine "POWERUP", "MUSHROOM",   $3c, $3e
MetaspriteDefine "POWERUP", "1UP",        $3c, $3e

EXPLOSION_BANK = SPRITE_BANK_1
EXPLOSION_PALETTE = $03
MetaspriteDefine "EXPLOSION", "FRAME_1", $26, $26 | SPR_FLIP_H
MetaspriteDefine "EXPLOSION", "FRAME_2", $28, $28 | SPR_FLIP_H
MetaspriteDefine "EXPLOSION", "FRAME_3", $2a, $2a | SPR_FLIP_H

COIN_BANK = SPRITE_BANK_1
COIN_PALETTE = $03
MetaspriteDefine "COIN", "FRAME_1", $2c
MetaspriteDefine "COIN", "FRAME_2", $2e
MetaspriteDefine "COIN", "FRAME_3", $30
MetaspriteDefine "COIN", "FRAME_4", $32

MISC_BANK = SPRITE_BANK_1
MISC_PALETTE = $03
MISC_BUBBLE_PALETTE = $02
MetaspriteDefine "MISC", "BRICK", $22, $22
MetaspriteDefine "MISC", "BLOCK", $24, $24
MetaspriteDefine "MISC", "CHUNK", $16
MetaspriteDefine "MISC", "BUBBLE", $18


GOOMBA_BANK = SPRITE_BANK_2
GOOMBA_PALETTE = $03
MetaspriteDefine "GOOMBA", "WALKING_1", $22, $24
MetaspriteDefine "GOOMBA", "WALKING_2", $24 | SPR_FLIP_H, $22 | SPR_FLIP_H

.endif