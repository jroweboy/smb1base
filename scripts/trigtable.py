import math
import struct

invert = -1
def main():
  with open("/Users/jroweboy/dev/smb1base/src/sinetable.bin", "wb") as f:
    for i in range(64):
      for j in range(64):
        f.write(struct.pack("=b",round(math.sin(math.radians(j * 360.0 / 64.0)) * i * invert)))
  with open("/Users/jroweboy/dev/smb1base/src/cosinetable.bin", "wb") as f:
    for i in range(64):
      for j in range(64):
        f.write(struct.pack("=b",round(math.cos(math.radians(j * 360.0 / 64.0)) * i * invert)))

if __name__ == "__main__":
  main()
