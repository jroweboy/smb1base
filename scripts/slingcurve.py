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

if __name__ == "__main__":
  sling_curve()
