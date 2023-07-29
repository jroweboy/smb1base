
.global PlayerEnemyCollision
.global GetEnemyBoundBox
.global SetBBox, SetBBox2
.global EraseEnemyObject
.global EnemyToBGCollisionDet
.global EnemiesCollision
.global OffscreenBoundsCheck
.global SmallBBox
.global DuplicateEnemyObj
.global XMoveCntr_Platform
.global MoveWithXMCntrs
.global CheckpointEnemyID
.global TallBBox2
.global SmallPlatformBoundBox, LargePlatformBoundBox

; Object run code
.global RunNormalEnemies
.global RunBowserFlame
.global RunFireworks
.global NoRunCode
.global RunFirebarObj
.global RunLargePlatform
.global RunSmallPlatform
.global RunBowser
.global PowerUpObjHandler
.global VineObjectHandler
.global RunStarFlagObj
.global JumpspringHandler
.global NoRunCode
.global WarpZoneObject
.global RunRetainerObj

; Object load table
.global InitNormalEnemy
.global InitRedKoopa
.global NoInitCode
.global InitHammerBro
.global InitGoomba
.global InitBloober
.global InitBulletBill
.global InitCheepCheep
.global InitPodoboo
.global InitPiranhaPlant
.global InitJumpGPTroopa
.global InitRedPTroopa

.global InitHorizFlySwimEnemy
.global InitLakitu
.global InitEnemyFrenzy
.global EndFrenzy
.global InitShortFirebar
.global InitLongFirebar

.global InitBalPlatform
.global InitVertPlatform
.global LargeLiftUp
.global LargeLiftDown
.global InitHoriPlatform
.global InitDropPlatform
.global PlatLiftUp
.global PlatLiftDown
.global InitBowser
.global PwrUpJmp
.global Setup_Vine
.global InitRetainerObj
.global EndOfEnemyInitCode

; Object movement code
.global MoveNormalEnemy
.global ProcHammerBro
.global MoveBloober
.global MoveBulletBill
.global NoMoveCode
.global MoveSwimmingCheepCheep
.global MovePodoboo
.global MovePiranhaPlant
.global MoveJumpingEnemy
.global ProcMoveRedPTroopa
.global MoveFlyGreenPTroopa
.global MoveLakitu
.global NoMoveCode
.global MoveFlyingCheepCheep
