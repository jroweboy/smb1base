
.ifndef __GLOBALS_INC
__GLOBALS_INC = 1

; 
.global JumpEngine

; global imports found in common.s
.global GetEnemyOffscreenBits
.global GetScreenPosition
.global LoadAreaPointer
.global GetAreaType
.global GetXOffscreenBits
.global EnemyGfxHandler
.global RelativeEnemyPosition
.global ImposeGravityBlock
.global ImposeGravity
.global PlayerEnemyDiff
.global Bitmasks
.global RelWOfs
.global GetProperObjOffset
.global MoveObjectHorizontally
.global MoveD_EnemyVertically
.global MoveEnemyHorizontally
.global GetOffScreenBitsSet
.global RelativeFireballPosition
.global GetFireballBoundBox
.global GetPlayerOffscreenBits
.global RelativePlayerPosition
.global RelativeBlockPosition
.global GetBlockOffscreenBits
.global InitVStf
.global GetMiscOffscreenBits
.global KillEnemies
.global MoveFallingPlatform, MoveDropPlatform, MoveJ_EnemyVertically
.global TransposePlayers, MovePlayerHorizontally, ImposeGravitySprObj
.global RedPTroopaGrav, MoveEnemySlowVert, SetXMoveAmt
.global EraseEnemyObject
.global MovePlatformUp, MovePlatformDown

.global WorldAddrOffsets
.global AreaAddrOffsets

.endif ; __GLOBALS_INC
