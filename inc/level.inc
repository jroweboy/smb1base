; common definitions for levels

.global GetLrgObjAttrib
.global ChkLrgObjLength
.global ChkLrgObjFixedLength
.global RenderUnderPart
.global GetAreaObjXPosition, GetAreaObjYPosition
.global FindEmptyEnemySlot
.global GetRow

;large objects (rows $00-$0b or 00-11, d6-d4 set)
.global VerticalPipe, AreaStyleObject, RowOfBricks, RowOfSolidBlocks
.global RowOfCoins, ColumnOfBricks, ColumnOfSolidBlocks

;objects for special row $0c or 12
.global Hole_Empty, PulleyRopeObject, Bridge_High, Bridge_Middle
.global Bridge_Low, Hole_Water, QuestionBlockRow_High, QuestionBlockRow_Low

;objects for special row $0f or 15
.global EndlessRope, BalancePlatRope, CastleObject, StaircaseObject
.global ExitPipe, FlagpoleObject

;small objects (rows $00-$0b or 00-11, d6-d4 all clear)
.global QuestionBlock, Hidden1UpBlock, BrickWithCoins, BrickWithItem
.global WaterPipe, EmptyBlock, Jumpspring

;objects for special row $0d or 13 (d6 set)
.global IntroPipe, FlagpoleObject, AxeObj, ChainObj, CastleBridgeObj
.global ScrollLockObject_Warp, ScrollLockObject, ScrollLockObject, AreaFrenzy, LoopCmdE
