#!/usr/bin/env python3

from PIL import Image
from pilbmp2nes import pilbmp2chr

SLINGSHOT_BOX = (0, 0, 24, 32)
MARIO_BOX = (16, 0, 40, 24)
HOLSTERED_BOX = (40, 0, 64, 24)
BAND_BOX = (0, 24, 8, 32)
FLAG1_BOX = (24, 24, 32, 32)
FLAG2_BOX = (32, 24, 40, 32)
FLAG3_BOX = (40, 24, 48, 32)

MARIO_WIDE_BOX = (64, 0, 96, 32)
MARIO_WIDE_HOLSTERED_BOX = (96, 0, 128, 32)

def main():
  fulldrawing = Image.open("/Users/jroweboy/dev/smb1base/chr/tileset.bmp")
  slingshot = fulldrawing.crop(SLINGSHOT_BOX)
  mario = fulldrawing.crop(MARIO_BOX)
  holstered = fulldrawing.crop(HOLSTERED_BOX)
  band = fulldrawing.crop(BAND_BOX)
  flag1 = fulldrawing.crop(FLAG1_BOX)
  flag2 = fulldrawing.crop(FLAG2_BOX)
  flag3 = fulldrawing.crop(FLAG3_BOX)

  angles = 64
  direc = -1
  for i in range(0,angles):
    angle = direc * i * (360.0 / angles)
    out = Image.new("P", (128, 32))
    out.putpalette(fulldrawing.palette)
    out.paste(slingshot, SLINGSHOT_BOX)
    out.paste(mario.rotate(angle), MARIO_BOX)
    out.paste(holstered.rotate(angle), HOLSTERED_BOX)
    out.paste(mario.resize(size=(32,32)).rotate(angle), MARIO_WIDE_BOX)
    out.paste(holstered.resize(size=(32,32)).rotate(angle), MARIO_WIDE_HOLSTERED_BOX)
    out.paste(band.rotate(angle), BAND_BOX)
    out.paste(flag1.rotate(angle), FLAG1_BOX)
    out.paste(flag2.rotate(angle), FLAG2_BOX)
    out.paste(flag3.rotate(angle), FLAG3_BOX)
    out.save(f"/Users/jroweboy/dev/smb1base/build/rotate_{i}.bmp")
    b = pilbmp2chr(out)
    with open(f"/Users/jroweboy/dev/smb1base/chr/rotate/rotate_{i}.chr", 'wb') as f:
      f.write(b''.join(b))

if __name__ == "__main__":
  main()
