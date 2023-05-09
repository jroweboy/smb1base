import math
import struct

rounding_mode = round
invert = -1
maxdraw = 32
angles = 256
def main():
  with open("/Users/jroweboy/dev/smb1base/src/sinetable.bin", "wb") as f:
    for magnitude in range(maxdraw):
      for angle in range(angles):
        f.write(struct.pack("=b",rounding_mode(math.sin(math.radians(angle * 360.0 / angles)) * magnitude * invert)))
  with open("/Users/jroweboy/dev/smb1base/src/cosinetable.bin", "wb") as f:
    for magnitude in range(maxdraw):
      for angle in range(angles):
        f.write(struct.pack("=b",rounding_mode(math.cos(math.radians(angle * 360.0 / angles)) * magnitude * invert)))

if __name__ == "__main__":
  main()
