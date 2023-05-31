import math
import struct
import pathlib

basepath = pathlib.Path.cwd().joinpath("src/")

rounding_mode = round


def sling_curve():
  maxmag = 52
  invert = -1
  maxspeed = 8
  with open(basepath.joinpath("slingcurve.bin"), "wb") as sling:
    with open(basepath.joinpath("slingfractional.bin"), "wb") as fract:
      for mag in range(maxmag):
        sling.write(struct.pack("=b",rounding_mode(math.sin(math.radians(mag * 90.0 / maxmag)) * maxspeed * invert)))
        fractional = math.floor((math.sin(math.radians(mag * 90.0 / maxmag) * maxspeed) % 1) * 255.0)
        # print(fractional)
        fract.write(struct.pack("=B", fractional))


def angular_momentum():
  maxanglespeed = 32
  with open(basepath.joinpath("rotation_speed.bin"), "wb") as speed:
    for i in range(0,256):
      as_signed = -1 * (256 - i) if i > 128 else i
      val = as_signed * maxanglespeed / 128.0
      speed.write(struct.pack("=b",rounding_mode(val)))


def fireball_speed():
  max_x_speed = 0x40
  max_y_speed = 0x04
  angles = 64
  with open(basepath.joinpath("fireball_x_speed.bin"), "wb") as x:
    with open(basepath.joinpath("fireball_y_speed.bin"), "wb") as y:
      for angle in range(0,angles):
        # as_signed = -1 * (256 - i) if i > 128 else i
        # val = as_signed * maxanglespeed / 128.0
        x_speed = max_x_speed * math.cos(math.radians(angle * 360.0 / angles))
        y_speed = max_y_speed * math.sin(math.radians(angle * 360.0 / angles))
        print(f"{x_speed} {y_speed}")
        x.write(struct.pack("=b",rounding_mode(x_speed)))
        y.write(struct.pack("=b",rounding_mode(y_speed)))


def fulltable():
  rounding_mode = round
  invert = -1
  maxdraw = 48
  angles = 256
  with open(basepath.joinpath("sinetable.bin"), "wb") as f:
    for magnitude in range(maxdraw):
      for angle in range(angles):
        f.write(struct.pack("=b",rounding_mode(math.sin(math.radians(angle * 360.0 / angles)) * magnitude * invert)))
  with open(basepath.joinpath("cosinetable.bin"), "wb") as f:
    for magnitude in range(maxdraw):
      for angle in range(angles):
        f.write(struct.pack("=b",rounding_mode(math.cos(math.radians(angle * 360.0 / angles)) * magnitude * invert)))

def maxtable():
  invert = -1
  maxdraw = 48
  angles = 256
  with open(basepath.joinpath("sinetable.bin"), "wb") as f:
    for angle in range(angles):
      f.write(struct.pack("=b",rounding_mode(math.sin(math.radians(angle * 360.0 / angles)) * maxdraw * invert)))
  with open(basepath.joinpath("cosinetable.bin"), "wb") as f:
    for angle in range(angles):
      f.write(struct.pack("=b",rounding_mode(math.cos(math.radians(angle * 360.0 / angles)) * maxdraw * invert)))

if __name__ == "__main__":
  # fulltable()
  sling_curve()
  angular_momentum()
  maxtable()
  fireball_speed()
