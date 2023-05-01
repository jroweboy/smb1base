#!/usr/bin/env python3

from PIL import Image
from pilbmp2nes import pilbmp2chr

SLINGSHOT_BOX = (0, 0, 24, 32)
MARIO_BOX = (16, 0, 40, 24)
HOLSTERED_BOX = (40, 0, 64, 24)
BAND_BOX = (0, 24, 8, 32)

MARIO_WIDE_BOX = (64, 0, 96, 32)
MARIO_WIDE_HOLSTERED_BOX = (96, 0, 128, 32)

def main():
  fulldrawing = Image.open("/Users/jroweboy/dev/smb1base/chr/tileset.bmp")
  slingshot = fulldrawing.crop(SLINGSHOT_BOX)
  mario = fulldrawing.crop(MARIO_BOX)
  holstered = fulldrawing.crop(HOLSTERED_BOX)
  band = fulldrawing.crop(BAND_BOX)

  angles = 64
  direc = 1
  for i in range(0,angles):
    out = Image.new("P", (128, 32))
    out.putpalette(fulldrawing.palette)
    out.paste(slingshot, SLINGSHOT_BOX)
    out.paste(mario.rotate(direc * i * (360.0 / angles)), MARIO_BOX)
    out.paste(holstered.rotate(direc * i * (360.0 / angles)), HOLSTERED_BOX)
    out.paste(mario.resize(size=(32,32)).rotate(direc * i * (360.0 / angles)), MARIO_WIDE_BOX)
    out.paste(holstered.resize(size=(32,32)).rotate(direc * i * (360.0 / angles)), MARIO_WIDE_HOLSTERED_BOX)
    out.paste(band.rotate(direc * i * (360.0 / angles)), BAND_BOX)
    out.save(f"/Users/jroweboy/dev/smb1base/build/rotate_{i}.bmp")
    b = pilbmp2chr(out)
    with open(f"/Users/jroweboy/dev/smb1base/chr/rotate_{i}.chr", 'wb') as f:
      f.write(b''.join(b))

if __name__ == "__main__":
  main()
