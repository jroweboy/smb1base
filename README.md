## SMB1Base

My personal SMB1 base code for making ROM hacks. Some of the features of this base are as follows:

- CA65 with every last part of the code split into individual modules for compliation.
- All of the code is split into banks of 8kb with only 16kb of fixed space required.
  - This frees up the $c000 region to be used for banked DPCM
- Minor optimizations for the original game, removing residual code and cleaning up some small wtfs 
- CMake and VSCode integration
  - Code is built using Ninja with a custom CMake Toolchain and errors are displayed inline in VSCode

