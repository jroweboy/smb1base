## SMB1Base

My personal SMB1 base code for making ROM hacks. Some of the features of this base are as follows:

- CA65 with every last part of the code split into individual modules for compliation.
- All of the code is split into banks of 8kb with only 16kb of fixed space required.
  - This frees up the $c000 region to be used for banked DPCM
- Minor optimizations for the original game, removing residual code and cleaning up some small wtfs 
- CMake and VSCode integration
  - Code is built using Ninja with a custom CMake Toolchain and errors are displayed inline in VSCode

## Branch information

Panic At The Mario Disco

----

Travel through 5 groovy levels as both Peach and Mario in the search for the ultimate Discothèque!

Experience a brand new enemy, the disco Lakitu, a hip cat who can either drop helpful power ups or dangerous foes. 

A new P-Wing power up will let Mario and Peach swim through the skies!

----

This hack was made in 10 days during the SMB Jam 2023.

----

Version 1.1 is now live! Keeping with the spirit of this being a quickly thrown together hack, the new update was coded in a single day of crunch plus a second day of overtime ;)

*NEW* 

- Mario and Peach now have unique abilities.
  - Mario is invincible while rising in his jump and Peach has her traditional float abillity.
  - Press select at any time to switch between Mario and Peach
- Fire Power is not lost on damage, only on death.
- Level 5 is slightly reworked to accommodate their individual abilities better

*Bug Fixes*

- Floatey text for 1 ups properly working again
- Add missing halfway points for levels
- Fix fast death sometimes happening for regular levels

