
.ifndef __METASPRITE_P_INC
__METASPRITE_P_INC = 1

METASPRITES_COUNT .set 0

PATTERN_TABLE_0 = $00
PATTERN_TABLE_1 = $01
SPRITE_BANK_0 = $00 | PATTERN_TABLE_0
SPRITE_BANK_1 = $40 | PATTERN_TABLE_0
SPRITE_BANK_2 = $80 | PATTERN_TABLE_0
SPRITE_BANK_3 = $c0 | PATTERN_TABLE_0
SPRITE_BANK_4 = $00 | PATTERN_TABLE_1
SPRITE_BANK_5 = $40 | PATTERN_TABLE_1
SPRITE_BANK_6 = $80 | PATTERN_TABLE_1
SPRITE_BANK_7 = $c0 | PATTERN_TABLE_1

MSPR_VERTICAL_FLIP = %10000000
MSPR_OFFSET_MASK = %00111111
.define MetaspriteOffset(b) 0+((b) & MSPR_OFFSET_MASK)

; Custom values to signify that this sprite should be flipped in the metasprite
SPR_FLIP_H = %01000000 << 8
SPR_FLIP_V = %10000000 << 8
; Can be used to force a sprite palette. If NO_PALETTE is set, then the palette value
; from the object in the game will not be applied, and this value will be forced instead
SPR_NO_PALETTE = %00100000 << 8
SPR_PALETTE_0  = SPR_NO_PALETTE | %00000000 << 8
SPR_PALETTE_1  = SPR_NO_PALETTE | %00000001 << 8
SPR_PALETTE_2  = SPR_NO_PALETTE | %00000010 << 8
SPR_PALETTE_3  = SPR_NO_PALETTE | %00000011 << 8


.define MetaspriteVramOffset(b, vram) (((b) & %00111111) | vram)

.macro MetaspriteBoxBody Object, Animation, Direction, VramOffset, Palette, XOffset, YOffset, Spr1, Spr2, Spr3, Spr4
.local X1, X2, Y1, Y2, Size
.local Tile1, Tile2, Tile3, Tile4
.local Attr1, Attr2, Attr3, Attr4
.local DrawSprite1, DrawSprite2, DrawSprite3, DrawSprite4

X1 = 0+XOffset
X2 = 8+XOffset
Y1 = 8+YOffset
Y2 = 24+YOffset
Size .set 1

DrawSprite2 .set 0
DrawSprite3 .set 0
DrawSprite4 .set 0

DrawSprite1 .set 1
Tile1 .set MetaspriteVramOffset(<Spr1, VramOffset)

.ifnblank Spr2
DrawSprite2 .set 1
Size .set 2
Tile2 .set MetaspriteVramOffset(<Spr2, VramOffset)
.endif
.ifnblank Spr3
DrawSprite3 .set 1
Size .set 3
Tile3 .set MetaspriteVramOffset(<Spr3, VramOffset)
.endif
.ifnblank Spr4
DrawSprite4 .set 1
Size .set 4
Tile4 .set MetaspriteVramOffset(<Spr4, VramOffset)
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
.if .blank(Spr4) .and(.not .blank(Spr3))
  DrawSprite4 .set 1
  DrawSprite3 .set 0
  Attr4 .set Attr3 ^ $40
  Tile4 .set Tile3
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
.if DrawSprite1 <> 0
  .byte   Tile1, Attr1, Y1, X1
.endif
.if DrawSprite2 <> 0
  .byte   Tile2, Attr2, Y1, X2
.endif
.if DrawSprite3 <> 0
  .byte   Tile3, Attr3, Y2, X1
.endif
.if DrawSprite4 <> 0
  .byte   Tile4, Attr4, Y2, X2
.endif
.endmacro

.macro MetaspriteBox Object, Animation, Spr1, Spr2, Spr3, Spr4
.local VramOffset, Bank, Palette, XOffset, YOffset, Mirror

.if .defined(.ident( .sprintf("%s_%s_VRAM_OFFSET", Object, Animation) ))
  VramOffset = .ident( .sprintf("%s_%s_VRAM_OFFSET", Object, Animation) )
.elseif .defined(.ident( .sprintf("%s_VRAM_OFFSET", Object) ))
  VramOffset = .ident( .sprintf("%s_VRAM_OFFSET", Object) )
.else
  VramOffset = 0
.endif
.if .defined(.ident( .sprintf("%s_%s_BANK", Object, Animation) ))
  Bank = .ident( .sprintf("%s_%s_BANK", Object, Animation) )
.elseif .defined(.ident( .sprintf("%s_BANK", Object) ))
  Bank = .ident( .sprintf("%s_BANK", Object) )
.else
  Bank = $ff
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

.if .defined(.ident( .sprintf("%s_%s_X_OFFSET", Object, Animation) ))
  XOffset = <.ident( .sprintf("%s_%s_X_OFFSET", Object, Animation) )
.elseif .defined(.ident( .sprintf("%s_X_OFFSET", Object) ))
  XOffset = <.ident( .sprintf("%s_X_OFFSET", Object) )
.else
  XOffset = 0
.endif

.if .defined(.ident( .sprintf("%s_%s_NO_MIRROR", Object, Animation) ))
  Mirror = 0
.elseif .defined(.ident( .sprintf("%s_NO_MIRROR", Object) ))
  Mirror = 0
.else
  Mirror = 1
.endif

.ident( .sprintf("METASPRITE_%d_BANK",  METASPRITES_COUNT) ) = Bank

.ifdef METASPRITE_BODY

.if Mirror = 1
MetaspriteBoxBody Object, Animation, "LEFT", VramOffset, Palette, XOffset, YOffset, Spr1, Spr2, Spr3, Spr4
.endif
MetaspriteBoxBody Object, Animation, "RIGHT", VramOffset, Palette, XOffset, YOffset, Spr1, Spr2, Spr3, Spr4

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

.macro MetaspriteDuplicate Name, Mspr
.Local LL, LH, RL, RH, Id, Bank
Id = .ident( .concat("METASPRITE_", Mspr) )
Bank = .ident( .sprintf("METASPRITE_%d_BANK", Id) )
.ident( .sprintf("METASPRITE_%d_BANK",  METASPRITES_COUNT) ) = Bank

.ifdef METASPRITE_BODY
.out .sprintf("METASPRITE_%s = $%02x (duplicate of %s)", Mspr, METASPRITES_COUNT, Name)

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

.endif
