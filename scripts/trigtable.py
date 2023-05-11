import math
import struct

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
  maxtable()
