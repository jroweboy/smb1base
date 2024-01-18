

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

.macro MetaspriteDefineBody Object, Animation, Direction, Bank, Palette, YOffset, Spr1, Spr2, Spr3, Spr4
.local X1, X2, Y1, Y2, Size
.local Tile1, Tile2, Tile3, Tile4
.local Attr1, Attr2, Attr3, Attr4
X1 = 0
X2 = 8
Y1 = 8+YOffset
Y2 = 24+YOffset
Size .set 1

Tile1 .set ((<Spr1) | Bank | PATTERN_TABLE_1)
.ifnblank Spr2
Size .set 2
Tile2 .set ((<Spr2) | Bank | PATTERN_TABLE_1)
.endif
.ifnblank Spr3
Size .set 3
Tile3 .set ((<Spr3) | Bank | PATTERN_TABLE_1)
.endif
.ifnblank Spr4
Size .set 4
Tile4 .set ((<Spr4) | Bank | PATTERN_TABLE_1)
.endif
Attr1 .set (Palette | ((>Spr1) >> 8))
.ifnblank Spr2
Attr2 .set (Palette | ((>Spr2) >> 8))
.endif
.ifnblank Spr3
Attr3 .set (Palette | ((>Spr3) >> 8))
.endif
.ifnblank Spr4
Attr4 .set (Palette | ((>Spr4) >> 8))
.endif

.if .xmatch(Direction, "LEFT")
; .out .sprintf("Switching tile and attr %s %s %s", Object, Animation, Direction)
.ifnblank Spr2
  Tmp .set Tile1
  Tile1 .set Tile2
  Tile2 .set Tmp
  Tmp .set Attr1
  Attr1 .set Attr2
  Attr2 .set Tmp
  Attr1 .set Attr1 ^ $40
  Attr2 .set Attr2 ^ $40
.endif
.ifnblank Spr4  
  Tmp .set Tile3
  Tile3 .set Tile4
  Tile4 .set Tmp
  Tmp .set Attr3
  Attr3 .set Attr4
  Attr4 .set Tmp
  Attr3 .set Attr3 ^ $40
  Attr4 .set Attr4 ^ $40
.endif
.endif

.ident( .sprintf("MetaspriteData_%s_%s_%s", Object, Animation, Direction) ):
  .byte   (Size * 4)
  .byte   Tile1, Attr1, Y1, X1
.ifnblank Spr2
  .byte   Tile2, Attr2, Y1, X2
.endif
.ifnblank Spr3
  .byte   Tile3, Attr3, Y2, X1
.endif
.ifnblank Spr4
  .byte   Tile4, Attr4, Y2, X2
.endif
.endmacro

METASPRITES_COUNT .set 1
.macro MetaspriteDefine Object, Animation, Spr1, Spr2, Spr3, Spr4
.local Bank, Palette, YOffset, Mirror

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

.if .defined(.ident( .sprintf("%s_%s_NO_MIRROR", Object, Animation) ))
  Mirror = 0
.elseif .defined(.ident( .sprintf("%s_NO_MIRROR", Object) ))
  Mirror = 0
.else
  Mirror = 1
.endif

.ifdef METASPRITE_BODY

.if Mirror = 1
MetaspriteDefineBody Object, Animation, "LEFT", Bank, Palette, YOffset, Spr1, Spr2, Spr3, Spr4
.endif
MetaspriteDefineBody Object, Animation, "RIGHT", Bank, Palette, YOffset, Spr1, Spr2, Spr3, Spr4

.out .sprintf("METASPRITE_%s_%s = $%02x", Object, Animation, METASPRITES_COUNT)
.if Mirror = 1
.ident( .sprintf("METASPRITE_LEFT_%d_LO",  METASPRITES_COUNT) ) = .lobyte(.ident( .sprintf("MetaspriteData_%s_%s_LEFT", Object, Animation) ))
.ident( .sprintf("METASPRITE_LEFT_%d_HI",  METASPRITES_COUNT) ) = .hibyte(.ident( .sprintf("MetaspriteData_%s_%s_LEFT", Object, Animation) ))
.else
.ident( .sprintf("METASPRITE_LEFT_%d_LO",  METASPRITES_COUNT) ) = .lobyte(.ident( .sprintf("MetaspriteData_%s_%s_RIGHT", Object, Animation) ))
.ident( .sprintf("METASPRITE_LEFT_%d_HI",  METASPRITES_COUNT) ) = .hibyte(.ident( .sprintf("MetaspriteData_%s_%s_RIGHT", Object, Animation) ))
.endif

.ident( .sprintf("METASPRITE_RIGHT_%d_LO",  METASPRITES_COUNT) ) = .lobyte(.ident( .sprintf("MetaspriteData_%s_%s_RIGHT", Object, Animation) ))
.ident( .sprintf("METASPRITE_RIGHT_%d_HI",  METASPRITES_COUNT) ) = .hibyte(.ident( .sprintf("MetaspriteData_%s_%s_RIGHT", Object, Animation) ))
.endif

.ident( .sprintf("METASPRITE_%s_%s", Object, Animation) ) = METASPRITES_COUNT
METASPRITES_COUNT .set METASPRITES_COUNT + 1
.endmacro

.macro MetaspriteDuplicate Mspr, Name
.ifdef METASPRITE_BODY
.ident( .sprintf("METASPRITE_LEFT_%d_LO",  METASPRITES_COUNT) ) = .lobyte(.ident(Mspr))
.ident( .sprintf("METASPRITE_LEFT_%d_HI",  METASPRITES_COUNT) ) = .hibyte(.ident(Mspr))
.ident( .sprintf("METASPRITE_RIGHT_%d_LO",  METASPRITES_COUNT) ) = .lobyte(.ident(Mspr))
.ident( .sprintf("METASPRITE_RIGHT_%d_HI",  METASPRITES_COUNT) ) = .hibyte(.ident(Mspr))
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
MetaspriteDefine "BIG_MARIO", "JUMPING",   $14, $16, $34, $36
MetaspriteDefine "BIG_MARIO", "CROUCHING", $18, $1a, $38, $3a

MetaspriteDefine "BIG_MARIO", "SWIMMING_1_KICK", $00, $02, $20, $22
MetaspriteDefine "BIG_MARIO", "SWIMMING_1_HOLD", $00, $02, $24, $22
MetaspriteDefine "BIG_MARIO", "SWIMMING_2_KICK", $04, $06, $20, $26
MetaspriteDefine "BIG_MARIO", "SWIMMING_2_HOLD", $04, $06, $24, $26
MetaspriteDefine "BIG_MARIO", "SWIMMING_3_KICK", $04, $06, $28, $22
MetaspriteDefine "BIG_MARIO", "SWIMMING_3_HOLD", $04, $06, $2a, $22

MetaspriteDefine "BIG_MARIO", "FIRE_STANDING",  $00, $02, $20, $22
MetaspriteDefine "BIG_MARIO", "FIRE_WALKING_1", $04, $06, $24, $26 ; TODO
MetaspriteDefine "BIG_MARIO", "FIRE_WALKING_2", $08, $0a, $28, $2a ; TODO
MetaspriteDefine "BIG_MARIO", "FIRE_WALKING_3", $0c, $0e, $2c, $2e ; TODO
MetaspriteDefine "BIG_MARIO", "FIRE_SKIDDING",  $10, $12, $30, $32 ; TODO
MetaspriteDefine "BIG_MARIO", "FIRE_JUMPING",   $14, $16, $34, $36 ; TODO
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
; POWERUP_Y_OFFSET = 8
POWERUP_1UP_PALETTE = $01
POWERUP_STAR_PALETTE = $02
POWERUP_FIREFLOWER_PALETTE = $01
POWERUP_MUSHROOM_PALETTE = $02
POWERUP_1UP_NO_MIRROR = 1
POWERUP_MUSHROOM_NO_MIRROR = 1
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
MISC_Y_OFFSET = -8
MetaspriteDefine "MISC", "BRICK", $22, $22
MetaspriteDefine "MISC", "BLOCK", $24, $24 | SPR_FLIP_H
; drawn manually
; MetaspriteDefine "MISC", "CHUNK", $16
MetaspriteDefine "MISC", "BUBBLE", $18


GOOMBA_BANK = SPRITE_BANK_2
GOOMBA_PALETTE = $03
GOOMBA_DEAD_Y_OFFSET = $08
MetaspriteDefine "GOOMBA", "WALKING_1", $22, $24
MetaspriteDefine "GOOMBA", "WALKING_2", $24 | SPR_FLIP_H, $22 | SPR_FLIP_H
MetaspriteDefine "GOOMBA", "DEAD", $26 | SPR_FLIP_V, $26 | SPR_FLIP_H | SPR_FLIP_V

.endif