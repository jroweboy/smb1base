import math
import struct


maxmag = 52

rounding_mode = round

invert = -1
maxspeed = 8

def sling_curve():
  with open("/Users/jroweboy/dev/smb1base/src/slingcurve.bin", "wb") as sling:
    with open("/Users/jroweboy/dev/smb1base/src/slingfractional.bin", "wb") as fract:
      for mag in range(maxmag):
        sling.write(struct.pack("=b",rounding_mode(math.sin(math.radians(mag * 90.0 / maxmag)) * maxspeed * invert)))
        fractional = math.floor((math.sin(math.radians(mag * 90.0 / maxmag) * maxspeed) % 1) * 255.0)
        # print(fractional)
        fract.write(struct.pack("=B", fractional))


maxangle = 16

def angular_momentum():
  with open("/Users/jroweboy/dev/smb1base/src/rotation_speed.bin", "wb") as speed:
    for i in range(0,256):
      as_signed = -1 * (256 - i) if i > 128 else i
      val = as_signed * maxangle / 128.0
      speed.write(struct.pack("=b",rounding_mode(val)))


rounding_mode = round
invert = -1
maxdraw = 48
angles = 256

def fulltable():
  with open("/Users/jroweboy/dev/smb1base/src/sinetable.bin", "wb") as f:
    for magnitude in range(maxdraw):
      for angle in range(angles):
        f.write(struct.pack("=b",rounding_mode(math.sin(math.radians(angle * 360.0 / angles)) * magnitude * invert)))
  with open("/Users/jroweboy/dev/smb1base/src/cosinetable.bin", "wb") as f:
    for magnitude in range(maxdraw):
      for angle in range(angles):
        f.write(struct.pack("=b",rounding_mode(math.cos(math.radians(angle * 360.0 / angles)) * magnitude * invert)))

angles = 256
def maxtable():
  with open("/Users/jroweboy/dev/smb1base/src/sinetable.bin", "wb") as f:
    for angle in range(angles):
      f.write(struct.pack("=b",rounding_mode(math.sin(math.radians(angle * 360.0 / angles)) * maxdraw * invert)))
  with open("/Users/jroweboy/dev/smb1base/src/cosinetable.bin", "wb") as f:
    for angle in range(angles):
      f.write(struct.pack("=b",rounding_mode(math.cos(math.radians(angle * 360.0 / angles)) * maxdraw * invert)))

if __name__ == "__main__":
  # fulltable()
  sling_curve()
  angular_momentum()
  maxtable()
