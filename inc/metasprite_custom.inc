

.ifndef __METASPRITE_CUSTOM_INC
__METASPRITE_CUSTOM_INC = 1

.ifdef METASPRITE_BODY

.include "common.inc"
.include "metasprite.inc"

MetaspriteData "METASPRITE_NULL", $0000

;;;;;;;;;;;
; Girder Platform

Y_OFFSET .set 0
X_OFFSET .set 0
PALETTE  .set 2

MetaspriteData "METASPRITE_PLATFORM_GIRDER_SMALL", MetaspritePlatformGirderSmall
MetaspriteData "METASPRITE_PLATFORM_GIRDER_SMALL_FRAME_2", MetaspritePlatformGirderSmallFrame2

MetaspritePlatformGirderSmall:
  .byte  4 * 4
.repeat 4, I
  .byte  MetaspriteVramOffset{$1a}, {SPRITE_BANK_1}, PALETTE,  0 + Y_OFFSET,  0 + X_OFFSET + (I * 8)
.endrepeat

MetaspritePlatformGirderSmallFrame2:
  .byte  4 * 4
.repeat 4, I
  .byte  MetaspriteVramOffset{$1a}, {SPRITE_BANK_1}, PALETTE,  0 + Y_OFFSET,  0 + X_OFFSET + (((I + 2) .mod 4) * 8)
.endrepeat

MetaspriteData "METASPRITE_PLATFORM_GIRDER_LARGE", MetaspritePlatformGirderLarge
MetaspriteData "METASPRITE_PLATFORM_GIRDER_LARGE_FRAME_2", MetaspritePlatformGirderLargeFrame2

MetaspritePlatformGirderLarge:
  .byte  6 * 4
.repeat 6, I
  .byte  MetaspriteVramOffset{$1a}, {SPRITE_BANK_1}, PALETTE,  0 + Y_OFFSET,  0 + X_OFFSET + (I * 8)
.endrepeat

MetaspritePlatformGirderLargeFrame2:
  .byte  6 * 4
.repeat 6, I
  .byte  MetaspriteVramOffset{$1a}, {SPRITE_BANK_1}, PALETTE,  0 + Y_OFFSET,  0 + X_OFFSET + (((I + 3) .mod 6) * 8)
.endrepeat

;;;;;;;;;;;
; Cloud Platform

Y_OFFSET .set 0
X_OFFSET .set 0
PALETTE  .set 2

MetaspriteData "METASPRITE_PLATFORM_CLOUD_SMALL", MetaspritePlatformCloudSmall
MetaspriteData "METASPRITE_PLATFORM_CLOUD_SMALL_FRAME_2", MetaspritePlatformCloudSmallFrame2

MetaspritePlatformCloudSmall:
  .byte  4 * 4
.repeat 4, I
  .byte  MetaspriteVramOffset{$1c}, {SPRITE_BANK_1}, PALETTE,  0 + Y_OFFSET,  0 + X_OFFSET + (I * 8)
.endrepeat

MetaspritePlatformCloudSmallFrame2:
  .byte  4 * 4
.repeat 4, I
  .byte  MetaspriteVramOffset{$1c}, {SPRITE_BANK_1}, PALETTE,  0 + Y_OFFSET,  0 + X_OFFSET + (((I + 2) .mod 4) * 8)
.endrepeat

MetaspriteData "METASPRITE_PLATFORM_CLOUD_LARGE", MetaspritePlatformCloudLarge
MetaspriteData "METASPRITE_PLATFORM_CLOUD_LARGE_FRAME_2", MetaspritePlatformCloudLargeFrame2

MetaspritePlatformCloudLarge:
  .byte  6 * 4
.repeat 6, I
  .byte  MetaspriteVramOffset{$1c}, {SPRITE_BANK_1}, PALETTE,  0 + Y_OFFSET,  0 + X_OFFSET + (I * 8)
.endrepeat

MetaspritePlatformCloudLargeFrame2:
  .byte  6 * 4
.repeat 6, I
  .byte  MetaspriteVramOffset{$1c}, {SPRITE_BANK_1}, PALETTE,  0 + Y_OFFSET,  0 + X_OFFSET + (((I + 3) .mod 6) * 8)
.endrepeat


Y_OFFSET .set -4
X_OFFSET .set 0
PALETTE  .set 2
MetaspriteData "METASPRITE_BOWSER_FLAME", MetaspriteBowserFlame, MetaspriteBowserFlame
MetaspriteBowserFlame:
  .byte 3 * 4
  .byte  MetaspriteVramOffset{$74}, {SPRITE_BANK_4}, PALETTE,   0 + Y_OFFSET,  0 + X_OFFSET
  .byte  MetaspriteVramOffset{$76}, {SPRITE_BANK_4}, PALETTE,   0 + Y_OFFSET,  8 + X_OFFSET
  .byte  MetaspriteVramOffset{$78}, {SPRITE_BANK_4}, PALETTE,   0 + Y_OFFSET, 16 + X_OFFSET

.endif ; METASPRITE_BODY

.endif ; METASPRITE_CUSTOM_INC
