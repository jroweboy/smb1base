
.ifndef __METASPRITE_P_INC
__METASPRITE_P_INC = 1

METASPRITES_COUNT .set 0

PATTERN_TABLE_1 = $01
SPRITE_BANK_0 = $00
SPRITE_BANK_1 = $40
SPRITE_BANK_2 = $80
SPRITE_BANK_3 = $c0

; Custom values to signify that this sprite should be flipped in the metasprite
SPR_FLIP_H = %01000000 << 8
SPR_FLIP_V = %10000000 << 8
SPR_NO_PALETTE = %00100000 << 8

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

MetaspriteReserve .sprintf("%s_%s",Object, Animation) 
.endmacro

.macro MetaspriteDuplicate Mspr, Name
.ifdef METASPRITE_BODY
.Local LL, LH, RL, RH, Id
Id = .ident( .concat("METASPRITE_", Mspr) )

LL = .ident(.sprintf("METASPRITE_LEFT_%d_LO", Id))
LH = .ident(.sprintf("METASPRITE_LEFT_%d_HI", Id))
RL = .ident(.sprintf("METASPRITE_RIGHT_%d_LO", Id))
RH = .ident(.sprintf("METASPRITE_RIGHT_%d_HI", Id))

.ident( .sprintf("METASPRITE_LEFT_%d_LO",  METASPRITES_COUNT) ) = LL
.ident( .sprintf("METASPRITE_LEFT_%d_HI",  METASPRITES_COUNT) ) = LH
.ident( .sprintf("METASPRITE_RIGHT_%d_LO",  METASPRITES_COUNT) ) = RL
.ident( .sprintf("METASPRITE_RIGHT_%d_HI",  METASPRITES_COUNT) ) = RH
.endif

MetaspriteReserve Name
.endmacro

;;;;
; Reserves a spot in the metasprite table for this named constant.
; This is used internally when making a new metasprite, but can also
; be used to make your own metasprite. Just reserve a slot with the name
; of the metasprite and 
.macro MetaspriteReserve Name
.ident( .sprintf("METASPRITE_%s", Name) ) = METASPRITES_COUNT
METASPRITES_COUNT .set METASPRITES_COUNT + 1
.endmacro

.macro MetaspriteData Name, Left, Right
.Local Id

Id = .ident(Name)

.ident( .sprintf("METASPRITE_LEFT_%d_LO",  Id) ) = .lobyte(Left)
.ident( .sprintf("METASPRITE_LEFT_%d_HI",  Id) ) = .hibyte(Left)
.ifnblank Right
.ident( .sprintf("METASPRITE_RIGHT_%d_LO", Id) ) = .lobyte(Right)
.ident( .sprintf("METASPRITE_RIGHT_%d_HI", Id) ) = .hibyte(Right)
.else
.ident( .sprintf("METASPRITE_RIGHT_%d_LO", Id) ) = .lobyte(Left)
.ident( .sprintf("METASPRITE_RIGHT_%d_HI", Id) ) = .hibyte(Left)
.endif

.endmacro

.define MetaspriteBank(b, bank) (((b) & %00111111) | bank | 1)

.endif