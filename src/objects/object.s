.include "common.inc"
.include "object.inc"

; sprite_render.s
.import EnemyGfxHandler, JCoinGfxHandler, DrawHammer
; collision.s
.import PlayerCollisionCore,InjurePlayer

;-------------------------------------------------------------------------------------

.proc EnemiesAndLoopsCore
  lda Enemy_Flag,x         ;check data here for MSB set
  pha                      ;save in stack
    asl
    bcs ChkBowserF           ;if MSB set in enemy flag, branch ahead of jumps
  pla                      ;get from stack
  beq ChkAreaTsk           ;if data zero, branch
  jmp RunEnemyObjectsCore  ;otherwise, jump to run enemy subroutines
ChkAreaTsk:
  lda AreaParserTaskNum    ;check number of tasks to perform
  and #$07
  cmp #$07                 ;if at a specific task, jump and leave
  beq ExitELCore
  jmp ProcLoopCommand      ;otherwise, jump to process loop command/load enemies
ChkBowserF:
  pla                      ;get data from stack
  and #%00001111           ;mask out high nybble
  tay
  lda Enemy_Flag,y         ;use as pointer and load same place with different offset
  bne ExitELCore
  sta Enemy_Flag,x         ;if second enemy flag not set, also clear first one
ExitELCore:
  rts
.endproc

;-------------------------------------------------------------------------------------

RunEnemyObjectsCore:
       ldx ObjectOffset  ;get offset for enemy object buffer
       lda #$00          ;load value 0 for jump engine by default
       ldy Enemy_ID,x
       cpy #$15          ;if enemy object < $15, use default value
       bcc JmpEO
       tya               ;otherwise subtract $14 from the value and use
       sbc #$14          ;as value for jump engine
JmpEO: jsr JumpEngine
      
      .word RunNormalEnemies  ;for objects $00-$14

      .word RunBowserFlame    ;for objects $15-$1f
      .word RunFireworks
      .word NoRunCode
      .word NoRunCode
      .word NoRunCode
      .word NoRunCode
      .word RunFirebarObj
      .word RunFirebarObj
      .word RunFirebarObj
      .word RunFirebarObj
      .word RunFirebarObj

      .word RunFirebarObj     ;for objects $20-$2f
      .word RunFirebarObj
      .word RunFirebarObj
      .word NoRunCode
      .word RunLargePlatform
      .word RunLargePlatform
      .word RunLargePlatform
      .word RunLargePlatform
      .word RunLargePlatform
      .word RunLargePlatform
      .word RunLargePlatform
      .word RunSmallPlatform
      .word RunSmallPlatform
      .word RunBowser
      .word PowerUpObjHandler
      .word VineObjectHandler

      .word NoRunCode         ;for objects $30-$35
      .word RunStarFlagObj
      .word JumpspringHandler
      .word NoRunCode
      .word WarpZoneObject
      .word RunRetainerObj

;--------------------------------

;loop command data
LoopCmdWorldNumber:
      .byte $03, $03, $06, $06, $06, $06, $06, $06, $07, $07, $07

LoopCmdPageNumber:
      .byte $05, $09, $04, $05, $06, $08, $09, $0a, $06, $0b, $10

LoopCmdYPosition:
      .byte $40, $b0, $b0, $80, $40, $40, $80, $40, $f0, $f0, $f0

ExecGameLoopback:
      lda Player_PageLoc        ;send player back four pages
      sec
      sbc #$04
      sta Player_PageLoc
      lda CurrentPageLoc        ;send current page back four pages
      sec
      sbc #$04
      sta CurrentPageLoc
      lda ScreenLeft_PageLoc    ;subtract four from page location
      sec                       ;of screen's left border
      sbc #$04
      sta ScreenLeft_PageLoc
      lda ScreenRight_PageLoc   ;do the same for the page location
      sec                       ;of screen's right border
      sbc #$04
      sta ScreenRight_PageLoc
      lda AreaObjectPageLoc     ;subtract four from page control
      sec                       ;for area objects
      sbc #$04
      sta AreaObjectPageLoc
      lda #$00                  ;initialize page select for both
      sta EnemyObjectPageSel    ;area and enemy objects
      sta AreaObjectPageSel
      sta EnemyDataOffset       ;initialize enemy object data offset
      sta EnemyObjectPageLoc    ;and enemy object page control
      lda AreaDataOfsLoopback,y ;adjust area object offset based on
      sta AreaDataOffset        ;which loop command we encountered
      rts

;-------------------------------------------------------------------------------------

AreaDataOfsLoopback:
      .byte $12, $36, $0e, $0e, $0e, $32, $32, $32, $0a, $26, $40



.import KillAllEnemies
ProcLoopCommand:
          lda LoopCommand           ;check if loop command was found
          beq ChkEnemyFrenzy
          lda CurrentColumnPos      ;check to see if we're still on the first page
          bne ChkEnemyFrenzy        ;if not, do not loop yet
          ldy #$0b                  ;start at the end of each set of loop data
FindLoop: dey
          bmi ChkEnemyFrenzy        ;if all data is checked and not match, do not loop
          lda WorldNumber           ;check to see if one of the world numbers
          cmp LoopCmdWorldNumber,y  ;matches our current world number
          bne FindLoop
          lda CurrentPageLoc        ;check to see if one of the page numbers
          cmp LoopCmdPageNumber,y   ;matches the page we're currently on
          bne FindLoop
          lda Player_Y_Position     ;check to see if the player is at the correct position
          cmp LoopCmdYPosition,y    ;if not, branch to check for world 7
          bne WrongChk
          lda Player_State          ;check to see if the player is
          cmp #$00                  ;on solid ground (i.e. not jumping or falling)
          bne WrongChk              ;if not, player fails to pass loop, and loopback
          lda WorldNumber           ;are we in world 7? (check performed on correct
          cmp #World7               ;vertical position and on solid ground)
          bne InitMLp               ;if not, initialize flags used there, otherwise
          inc MultiLoopCorrectCntr  ;increment counter for correct progression
IncMLoop: inc MultiLoopPassCntr     ;increment master multi-part counter
          lda MultiLoopPassCntr     ;have we done all three parts?
          cmp #$03
          bne InitLCmd              ;if not, skip this part
          lda MultiLoopCorrectCntr  ;if so, have we done them all correctly?
          cmp #$03
          beq InitMLp               ;if so, branch past unnecessary check here
          bne DoLpBack              ;unconditional branch if previous branch fails
WrongChk: lda WorldNumber           ;are we in world 7? (check performed on
          cmp #World7               ;incorrect vertical position or not on solid ground)
          beq IncMLoop
DoLpBack: jsr ExecGameLoopback      ;if player is not in right place, loop back
          jsr KillAllEnemies
InitMLp:  lda #$00                  ;initialize counters used for multi-part loop commands
          sta MultiLoopPassCntr
          sta MultiLoopCorrectCntr
InitLCmd: lda #$00                  ;initialize loop command flag
          sta LoopCommand
          ; fallthrough
;--------------------------------

ChkEnemyFrenzy:
      lda EnemyFrenzyQueue  ;check for enemy object in frenzy queue
      beq ProcessEnemyData  ;if not, skip this part
      sta Enemy_ID,x        ;store as enemy object identifier here
      lda #$01
      sta Enemy_Flag,x      ;activate enemy object flag
      lda #$00
      sta Enemy_State,x     ;initialize state and frenzy queue
      sta EnemyFrenzyQueue
      jmp InitEnemyObject   ;and then jump to deal with this enemy

;--------------------------------
;$06 - used to hold page location of extended right boundary
;$07 - used to hold high nybble of position of extended right boundary

ProcessEnemyData:
        ldy EnemyDataOffset      ;get offset of enemy object data
        lda (EnemyData),y        ;load first byte
        cmp #$ff                 ;check for EOD terminator
        bne CheckEndofBuffer
        jmp CheckFrenzyBuffer    ;if found, jump to check frenzy buffer, otherwise

CheckEndofBuffer:
        and #%00001111           ;check for special row $0e
        cmp #$0e
        beq CheckRightBounds     ;if found, branch, otherwise
        cpx #$05                 ;check for end of buffer
        bcc CheckRightBounds     ;if not at end of buffer, branch
        iny
        lda (EnemyData),y        ;check for specific value here
        and #%00111111           ;not sure what this was intended for, exactly
        cmp #$2e                 ;this part is quite possibly residual code
        beq CheckRightBounds     ;but it has the effect of keeping enemies out of
        rts                      ;the sixth slot

CheckRightBounds:
        lda ScreenRight_X_Pos    ;add 48 to pixel coordinate of right boundary
        clc
        adc #$30
        and #%11110000           ;store high nybble
        sta $07
        lda ScreenRight_PageLoc  ;add carry to page location of right boundary
        adc #$00
        sta $06                  ;store page location + carry
        ldy EnemyDataOffset
        iny
        lda (EnemyData),y        ;if MSB of enemy object is clear, branch to check for row $0f
        asl
        bcc CheckPageCtrlRow
        lda EnemyObjectPageSel   ;if page select already set, do not set again
        bne CheckPageCtrlRow
        inc EnemyObjectPageSel   ;otherwise, if MSB is set, set page select 
        inc EnemyObjectPageLoc   ;and increment page control

CheckPageCtrlRow:
        dey
        lda (EnemyData),y        ;reread first byte
        and #$0f
        cmp #$0f                 ;check for special row $0f
        bne PositionEnemyObj     ;if not found, branch to position enemy object
        lda EnemyObjectPageSel   ;if page select set,
        bne PositionEnemyObj     ;branch without reading second byte
        iny
        lda (EnemyData),y        ;otherwise, get second byte, mask out 2 MSB
        and #%00111111
        sta EnemyObjectPageLoc   ;store as page control for enemy object data
        inc EnemyDataOffset      ;increment enemy object data offset 2 bytes
        inc EnemyDataOffset
        inc EnemyObjectPageSel   ;set page select for enemy object data and 
        jmp ProcLoopCommand      ;jump back to process loop commands again

PositionEnemyObj:
        lda EnemyObjectPageLoc   ;store page control as page location
        sta Enemy_PageLoc,x      ;for enemy object
        lda (EnemyData),y        ;get first byte of enemy object
        and #%11110000
        sta Enemy_X_Position,x   ;store column position
        cmp ScreenRight_X_Pos    ;check column position against right boundary
        lda Enemy_PageLoc,x      ;without subtracting, then subtract borrow
        sbc ScreenRight_PageLoc  ;from page location
        bcs CheckRightExtBounds  ;if enemy object beyond or at boundary, branch
        lda (EnemyData),y
        and #%00001111           ;check for special row $0e
        cmp #$0e                 ;if found, jump elsewhere
        beq ParseRow0e
        jmp CheckThreeBytes      ;if not found, unconditional jump

CheckRightExtBounds:
        lda $07                  ;check right boundary + 48 against
        cmp Enemy_X_Position,x   ;column position without subtracting,
        lda $06                  ;then subtract borrow from page control temp
        sbc Enemy_PageLoc,x      ;plus carry
        bcc CheckFrenzyBuffer    ;if enemy object beyond extended boundary, branch
        lda #$01                 ;store value in vertical high byte
        sta Enemy_Y_HighPos,x
        lda (EnemyData),y        ;get first byte again
        asl                      ;multiply by four to get the vertical
        asl                      ;coordinate
        asl
        asl
        sta Enemy_Y_Position,x
        cmp #$e0                 ;do one last check for special row $0e
        beq ParseRow0e           ;(necessary if branched to $c1cb)
        iny
        lda (EnemyData),y        ;get second byte of object
        and #%01000000           ;check to see if hard mode bit is set
        beq CheckForEnemyGroup   ;if not, branch to check for group enemy objects
        lda SecondaryHardMode    ;if set, check to see if secondary hard mode flag
        beq Inc2B                ;is on, and if not, branch to skip this object completely

CheckForEnemyGroup:
        lda (EnemyData),y      ;get second byte and mask out 2 MSB
        and #%00111111
        cmp #$37               ;check for value below $37
        bcc BuzzyBeetleMutate
        cmp #$3f               ;if $37 or greater, check for value
        bcc DoGroup            ;below $3f, branch if below $3f

BuzzyBeetleMutate:
        cmp #Goomba          ;if below $37, check for goomba
        bne StrID            ;value ($3f or more always fails)
        ldy PrimaryHardMode  ;check if primary hard mode flag is set
        beq StrID            ;and if so, change goomba to buzzy beetle
        lda #BuzzyBeetle
StrID:  sta Enemy_ID,x       ;store enemy object number into buffer
        lda #$01
        sta Enemy_Flag,x     ;set flag for enemy in buffer
        jsr InitEnemyObject
        lda Enemy_Flag,x     ;check to see if flag is set
        bne Inc2B            ;if not, leave, otherwise branch
        rts

CheckFrenzyBuffer:
        lda EnemyFrenzyBuffer    ;if enemy object stored in frenzy buffer
        bne StrFre               ;then branch ahead to store in enemy object buffer
        lda VineFlagOffset       ;otherwise check vine flag offset
        cmp #$01
        bne ExEPar               ;if other value <> 1, leave
        lda #VineObject          ;otherwise put vine in enemy identifier
StrFre: sta Enemy_ID,x           ;store contents of frenzy buffer into enemy identifier value

InitEnemyObject:
        lda #$00                 ;initialize enemy state
        sta Enemy_State,x
        jsr CheckpointEnemyID    ;jump ahead to run jump engine and subroutines
ExEPar: rts                      ;then leave

DoGroup:
        jmp HandleGroupEnemies   ;handle enemy group objects

ParseRow0e:
        iny                      ;increment Y to load third byte of object
        iny
        lda (EnemyData),y
        lsr                      ;move 3 MSB to the bottom, effectively
        lsr                      ;making %xxx00000 into %00000xxx
        lsr
        lsr
        lsr
        cmp WorldNumber          ;is it the same world number as we're on?
        bne NotUse               ;if not, do not use (this allows multiple uses
        dey                      ;of the same area, like the underground bonus areas)
        lda (EnemyData),y        ;otherwise, get second byte and use as offset
        sta AreaPointer          ;to addresses for level and enemy object data
        iny
        lda (EnemyData),y        ;get third byte again, and this time mask out
        and #%00011111           ;the 3 MSB from before, save as page number to be
        sta EntrancePage         ;used upon entry to area, if area is entered
NotUse: jmp Inc3B

CheckThreeBytes:
        ldy EnemyDataOffset      ;load current offset for enemy object data
        lda (EnemyData),y        ;get first byte
        and #%00001111           ;check for special row $0e
        cmp #$0e
        bne Inc2B
Inc3B:  inc EnemyDataOffset      ;if row = $0e, increment three bytes
Inc2B:  inc EnemyDataOffset      ;otherwise increment two bytes
        inc EnemyDataOffset
        lda #$00                 ;init page select for enemy objects
        sta EnemyObjectPageSel
        ldx ObjectOffset         ;reload current offset in enemy buffers
        rts                      ;and leave

CheckpointEnemyID:
        lda Enemy_ID,x
        cmp #$15                     ;check enemy object identifier for $15 or greater
        bcs InitEnemyRoutines        ;and branch straight to the jump engine if found
        tay                          ;save identifier in Y register for now
        lda Enemy_Y_Position,x
        adc #$08                     ;add eight pixels to what will eventually be the
        sta Enemy_Y_Position,x       ;enemy object's vertical coordinate ($00-$14 only)
        lda #$01
        sta EnemyOffscrBitsMasked,x  ;set offscreen masked bit
        tya                          ;get identifier back and use as offset for jump engine

InitEnemyRoutines:
  jsr JumpEngine

;jump engine table for newly loaded enemy objects
  .word InitNormalEnemy  ;for objects $00-$0f
  .word InitNormalEnemy
  .word InitNormalEnemy
  .word InitRedKoopa
  .word NoInitCode
  .word InitHammerBro
  .word InitGoomba
  .word InitBloober
  .word InitBulletBill
  .word NoInitCode
  .word InitCheepCheep
  .word InitCheepCheep
  .word InitPodoboo
  .word InitPiranhaPlant
  .word InitJumpGPTroopa
  .word InitRedPTroopa

  .word InitHorizFlySwimEnemy  ;for objects $10-$1f
  .word InitLakitu
  .word InitEnemyFrenzy
  .word NoInitCode
  .word InitEnemyFrenzy
  .word InitEnemyFrenzy
  .word InitEnemyFrenzy
  .word InitEnemyFrenzy
  .word EndFrenzy
  .word NoInitCode
  .word NoInitCode
  .word InitShortFirebar
  .word InitShortFirebar
  .word InitShortFirebar
  .word InitShortFirebar
  .word InitLongFirebar

  .word NoInitCode ;for objects $20-$2f
  .word NoInitCode
  .word NoInitCode
  .word NoInitCode
  .word InitBalPlatform
  .word InitVertPlatform
  .word LargeLiftUp
  .word LargeLiftDown
  .word InitHoriPlatform
  .word InitDropPlatform
  .word InitHoriPlatform
  .word PlatLiftUp
  .word PlatLiftDown
  .word InitBowser
  .word PwrUpJmp   ;possibly dummy value
  .word Setup_Vine

  .word NoInitCode ;for objects $30-$36
  .word NoInitCode
  .word NoInitCode
  .word NoInitCode
  .word NoInitCode
  .word InitRetainerObj
  .word EndOfEnemyInitCode

;--------------------------------
;$00 - used to store Y position of group enemies
;$01 - used to store enemy ID
;$02 - used to store page location of right side of screen
;$03 - used to store X position of right side of screen

HandleGroupEnemies:
        ldy #$00                  ;load value for green koopa troopa
        sec
        sbc #$37                  ;subtract $37 from second byte read
        pha                       ;save result in stack for now
        cmp #$04                  ;was byte in $3b-$3e range?
        bcs SnglID                ;if so, branch
        pha                       ;save another copy to stack
        ldy #Goomba               ;load value for goomba enemy
        lda PrimaryHardMode       ;if primary hard mode flag not set,
        beq PullID                ;branch, otherwise change to value
        ldy #BuzzyBeetle          ;for buzzy beetle
PullID: pla                       ;get second copy from stack
SnglID: sty $01                   ;save enemy id here
        ldy #$b0                  ;load default y coordinate
        and #$02                  ;check to see if d1 was set
        beq SetYGp                ;if so, move y coordinate up,
        ldy #$70                  ;otherwise branch and use default
SetYGp: sty $00                   ;save y coordinate here
        lda ScreenRight_PageLoc   ;get page number of right edge of screen
        sta $02                   ;save here
        lda ScreenRight_X_Pos     ;get pixel coordinate of right edge
        sta $03                   ;save here
        ldy #$02                  ;load two enemies by default
        pla                       ;get first copy from stack
        lsr                       ;check to see if d0 was set
        bcc CntGrp                ;if not, use default value
        iny                       ;otherwise increment to three enemies
CntGrp: sty NumberofGroupEnemies  ;save number of enemies here
GrLoop: ldx #$ff                  ;start at beginning of enemy buffers
GSltLp: inx                       ;increment and branch if past
        cpx #$05                  ;end of buffers
        bcs NextED
        lda Enemy_Flag,x          ;check to see if enemy is already
        bne GSltLp                ;stored in buffer, and branch if so
        lda $01
        sta Enemy_ID,x            ;store enemy object identifier
        lda $02
        sta Enemy_PageLoc,x       ;store page location for enemy object
        lda $03
        sta Enemy_X_Position,x    ;store x coordinate for enemy object
        clc
        adc #$18                  ;add 24 pixels for next enemy
        sta $03
        lda $02                   ;add carry to page location for
        adc #$00                  ;next enemy
        sta $02
        lda $00                   ;store y coordinate for enemy object
        sta Enemy_Y_Position,x
        lda #$01                  ;activate flag for buffer, and
        sta Enemy_Y_HighPos,x     ;put enemy within the screen vertically
        sta Enemy_Flag,x
        jsr CheckpointEnemyID     ;process each enemy object separately
        dec NumberofGroupEnemies  ;do this until we run out of enemy objects
        bne GrLoop
NextED: jmp Inc2B                 ;jump to increment data offset and leave


;--------------------------------

InitBloober:
  lda #$00               ;initialize horizontal speed
  sta BlooperMoveSpeed,x
SmallBBox:
  lda #$09               ;set specific bounding box size control
  bne SetBBox            ;unconditional branch

;--------------------------------

InitRedPTroopa:
  ldy #$30                    ;load central position adder for 48 pixels down
  lda Enemy_Y_Position,x      ;set vertical coordinate into location to
  sta RedPTroopaOrigXPos,x    ;be used as original vertical coordinate
  bpl GetCent                 ;if vertical coordinate < $80
  ldy #$e0                    ;if => $80, load position adder for 32 pixels up
GetCent:
  tya                         ;send central position adder to A
  adc Enemy_Y_Position,x      ;add to current vertical coordinate
  sta RedPTroopaCenterYPos,x  ;store as central vertical coordinate
TallBBox:
  lda #$03                    ;set specific bounding box size control
SetBBox:
  sta Enemy_BoundBoxCtrl,x    ;set bounding box control here
  lda #$02                    ;set moving direction for left
  sta Enemy_MovingDir,x
  jmp InitVStf ; jroweboy: Added this jmp to common code

;-------------------------------------------------------------------------------------

MiscObjectsCore:
          ldx #$08          ;set at end of misc object buffer
MiscLoop: stx ObjectOffset  ;store misc object offset here
          lda Misc_State,x  ;check misc object state
          beq MiscLoopBack  ;branch to check next slot
          asl               ;otherwise shift d7 into carry
          bcc ProcJumpCoin  ;if d7 not set, jumping coin, thus skip to rest of code here
          jsr ProcHammerObj ;otherwise go to process hammer,
          jmp MiscLoopBack  ;then check next slot

;--------------------------------
;$00 - used to set downward force
;$01 - used to set upward force (residual)
;$02 - used to set maximum speed

ProcJumpCoin:
           ldy Misc_State,x          ;check misc object state
           dey                       ;decrement to see if it's set to 1
           beq JCoinRun              ;if so, branch to handle jumping coin
           inc Misc_State,x          ;otherwise increment state to either start off or as timer
           lda Misc_X_Position,x     ;get horizontal coordinate for misc object
           clc                       ;whether its jumping coin (state 0 only) or floatey number
           adc ScrollAmount          ;add current scroll speed
           sta Misc_X_Position,x     ;store as new horizontal coordinate
           lda Misc_PageLoc,x        ;get page location
           adc #$00                  ;add carry
           sta Misc_PageLoc,x        ;store as new page location
           lda Misc_State,x
           cmp #$30                  ;check state of object for preset value
           bne RunJCSubs             ;if not yet reached, branch to subroutines
           lda #$00
           sta Misc_State,x          ;otherwise nullify object state
           jmp MiscLoopBack          ;and move onto next slot
JCoinRun:  txa             
           clc                       ;add 13 bytes to offset for next subroutine
           adc #$0d
           tax
           lda #$50                  ;set downward movement amount
           sta $00
           lda #$06                  ;set maximum vertical speed
           sta $02
           lsr                       ;divide by 2 and set
           sta $01                   ;as upward movement amount (apparently residual)
           lda #$00                  ;set A to impose gravity on jumping coin
           jsr ImposeGravity         ;do sub to move coin vertically and impose gravity on it
           ldx ObjectOffset          ;get original misc object offset
           lda Misc_Y_Speed,x        ;check vertical speed
           cmp #$05
           bne RunJCSubs             ;if not moving downward fast enough, keep state as-is
           inc Misc_State,x          ;otherwise increment state to change to floatey number
RunJCSubs: jsr RelativeMiscPosition  ;get relative coordinates
           jsr GetMiscOffscreenBits  ;get offscreen information
           jsr GetMiscBoundBox       ;get bounding box coordinates (why?)
           jsr JCoinGfxHandler       ;draw the coin or floatey number

MiscLoopBack: 
           dex                       ;decrement misc object offset
           bpl MiscLoop              ;loop back until all misc objects handled
           rts                       ;then leave


;--------------------------------
;$00 - used to set downward force
;$01 - used to set upward force (residual)
;$02 - used to set maximum speed
.export ProcHammerObj
ProcHammerObj:
          lda TimerControl           ;if master timer control set
          bne RunHSubs               ;skip all of this code and go to last subs at the end
          lda Misc_State,x           ;otherwise get hammer's state
          and #%01111111             ;mask out d7
          ldy HammerEnemyOffset,x    ;get enemy object offset that spawned this hammer
          cmp #$02                   ;check hammer's state
          beq SetHSpd                ;if currently at 2, branch
          bcs SetHPos                ;if greater than 2, branch elsewhere
          txa
          clc                        ;add 13 bytes to use
          adc #$0d                   ;proper misc object
          tax                        ;return offset to X
          lda #$10
          sta $00                    ;set downward movement force
          lda #$0f
          sta $01                    ;set upward movement force (not used)
          lda #$04
          sta $02                    ;set maximum vertical speed
          lda #$00                   ;set A to impose gravity on hammer
          jsr ImposeGravity          ;do sub to impose gravity on hammer and move vertically
          jsr MoveObjectHorizontally ;do sub to move it horizontally
          ldx ObjectOffset           ;get original misc object offset
          jmp RunAllH                ;branch to essential subroutines
SetHSpd:  lda #$fe
          sta Misc_Y_Speed,x         ;set hammer's vertical speed
          lda Enemy_State,y          ;get enemy object state
          and #%11110111             ;mask out d3
          sta Enemy_State,y          ;store new state
          ldx Enemy_MovingDir,y      ;get enemy's moving direction
          dex                        ;decrement to use as offset
          lda HammerXSpdData,x       ;get proper speed to use based on moving direction
          ldx ObjectOffset           ;reobtain hammer's buffer offset
          sta Misc_X_Speed,x         ;set hammer's horizontal speed
SetHPos:  dec Misc_State,x           ;decrement hammer's state
          lda Enemy_X_Position,y     ;get enemy's horizontal position
          clc
          adc #$02                   ;set position 2 pixels to the right
          sta Misc_X_Position,x      ;store as hammer's horizontal position
          lda Enemy_PageLoc,y        ;get enemy's page location
          adc #$00                   ;add carry
          sta Misc_PageLoc,x         ;store as hammer's page location
          lda Enemy_Y_Position,y     ;get enemy's vertical position
          sec
          sbc #$0a                   ;move position 10 pixels upward
          sta Misc_Y_Position,x      ;store as hammer's vertical position
          lda #$01
          sta Misc_Y_HighPos,x       ;set hammer's vertical high byte
          bne RunHSubs               ;unconditional branch to skip first routine
RunAllH:  jsr PlayerHammerCollision  ;handle collisions
RunHSubs: jsr GetMiscOffscreenBits   ;get offscreen information
          jsr RelativeMiscPosition   ;get relative coordinates
          jsr GetMiscBoundBox        ;get bounding box coordinates
          jsr DrawHammer             ;draw the hammer
          rts                        ;and we are done here


;-------------------------------------------------------------------------------------

.export SpawnHammerObj
SpawnHammerObj:
  lda PseudoRandomBitReg+1 ;get pseudorandom bits from
  and #%00000111           ;second part of LSFR
  bne SetMOfs              ;if any bits are set, branch and use as offset
  lda PseudoRandomBitReg+1
  and #%00001000           ;get d3 from same part of LSFR
SetMOfs:
  tay                      ;use either d3 or d2-d0 for offset here
  lda Misc_State,y         ;if any values loaded in
  bne NoHammer             ;$2a-$32 where offset is then leave with carry clear
  ldx HammerEnemyOfsData,y ;get offset of enemy slot to check using Y as offset
  lda Enemy_Flag,x         ;check enemy buffer flag at offset
  bne NoHammer             ;if buffer flag set, branch to leave with carry clear
  ldx ObjectOffset         ;get original enemy object offset
  txa
  sta HammerEnemyOffset,y  ;save here
  lda #$90
  sta Misc_State,y         ;save hammer's state here
  lda #$07
  sta Misc_BoundBoxCtrl,y  ;set something else entirely, here
  sec                      ;return with carry set
  rts
NoHammer:
  ldx ObjectOffset         ;get original enemy object offset
  clc                      ;return with carry clear
  rts

HammerEnemyOfsData:
      .byte $04, $04, $04, $05, $05, $05
      .byte $06, $06, $06

HammerXSpdData:
      .byte $10, $f0


;--------------------------------
.export DuplicateEnemyObj
.proc DuplicateEnemyObj
  ldy #$ff                ;start at beginning of enemy slots
FSLoop:
  iny                     ;increment one slot
  lda Enemy_Flag,y        ;check enemy buffer flag for empty slot
  bne FSLoop              ;if set, branch and keep checking
  sty DuplicateObj_Offset ;otherwise set offset here
  txa                     ;transfer original enemy buffer offset
  ora #%10000000          ;store with d7 set as flag in new enemy
  sta Enemy_Flag,y        ;slot as well as enemy offset
  lda Enemy_PageLoc,x
  sta Enemy_PageLoc,y     ;copy page location and horizontal coordinates
  lda Enemy_X_Position,x  ;from original enemy to new enemy
  sta Enemy_X_Position,y
  lda #$01
  sta Enemy_Flag,x        ;set flag as normal for original enemy
  sta Enemy_Y_HighPos,y   ;set high vertical byte for new enemy
  lda Enemy_Y_Position,x
  sta Enemy_Y_Position,y  ;copy vertical coordinate from original to new
FlmEx:
  rts                     ;and then leave
.endproc

;--------------------------------
.proc RunRetainerObj
.import GetEnemyOffscreenBits, RelativeEnemyPosition, EnemyGfxHandler
  jsr GetEnemyOffscreenBits
  jsr RelativeEnemyPosition
  jmp EnemyGfxHandler
.endproc


;--------------------------------
;$00 - used to store enemy identifier in KillEnemies
KillEnemies:
  sta $00           ;store identifier here
  lda #$00
  ldx #$04          ;check for identifier in enemy object buffer
KillELoop:
  ldy Enemy_ID,x
  cpy $00           ;if not found, branch
  bne NoKillE
  sta Enemy_Flag,x  ;if found, deactivate enemy object flag
NoKillE:
  dex               ;do this until all slots are checked
  bpl KillELoop
  rts


;-------------------------------------------------------------------------------------

EraseEnemyObject:
      lda #$00                 ;clear all enemy object variables
      sta Enemy_Flag,x
      sta Enemy_ID,x
      sta Enemy_State,x
      sta FloateyNum_Control,x
      sta EnemyIntervalTimer,x
      sta ShellChainCounter,x
      sta Enemy_SprAttrib,x
      sta EnemyFrameTimer,x
      rts


;--------------------------------

RunNormalEnemies:
          lda #$00                  ;init sprite attributes
          sta Enemy_SprAttrib,x
          jsr GetEnemyOffscreenBits
          jsr RelativeEnemyPosition
          jsr EnemyGfxHandler
          jsr GetEnemyBoundBox
          jsr EnemyToBGCollisionDet
          jsr EnemiesCollision
          jsr PlayerEnemyCollision
          ldy TimerControl          ;if master timer control set, skip to last routine
          bne SkipMove
          jsr EnemyMovementSubs
SkipMove: jmp OffscreenBoundsCheck

EnemyMovementSubs:
      lda Enemy_ID,x
      jsr JumpEngine

      .word MoveNormalEnemy      ;only objects $00-$14 use this table
      .word MoveNormalEnemy
      .word MoveNormalEnemy
      .word MoveNormalEnemy
      .word MoveNormalEnemy
      .word ProcHammerBro
      .word MoveNormalEnemy
      .word MoveBloober
      .word MoveBulletBill
      .word NoMoveCode
      .word MoveSwimmingCheepCheep
      .word MoveSwimmingCheepCheep
      .word MovePodoboo
      .word MovePiranhaPlant
      .word MoveJumpingEnemy
      .word ProcMoveRedPTroopa
      .word MoveFlyGreenPTroopa
      .word MoveLakitu
      .word MoveNormalEnemy
      .word NoMoveCode   ;dummy
      .word MoveFlyingCheepCheep

;--------------------------------

NoMoveCode:
      rts


;-------------------------------------------------------------------------------------
;$00 - page location of extended left boundary
;$01 - extended left boundary position
;$02 - page location of extended right boundary
;$03 - extended right boundary position

OffscreenBoundsCheck:
          lda Enemy_ID,x          ;check for cheep-cheep object
          cmp #FlyingCheepCheep   ;branch to leave if found
          beq ExScrnBd
          lda ScreenLeft_X_Pos    ;get horizontal coordinate for left side of screen
          ldy Enemy_ID,x
          cpy #HammerBro          ;check for hammer bro object
          beq LimitB
          cpy #PiranhaPlant       ;check for piranha plant object
          bne ExtendLB            ;these two will be erased sooner than others if too far left
LimitB:   adc #$38                ;add 56 pixels to coordinate if hammer bro or piranha plant
ExtendLB: sbc #$48                ;subtract 72 pixels regardless of enemy object
          sta $01                 ;store result here
          lda ScreenLeft_PageLoc
          sbc #$00                ;subtract borrow from page location of left side
          sta $00                 ;store result here
          lda ScreenRight_X_Pos   ;add 72 pixels to the right side horizontal coordinate
          adc #$48
          sta $03                 ;store result here
          lda ScreenRight_PageLoc     
          adc #$00                ;then add the carry to the page location
          sta $02                 ;and store result here
          lda Enemy_X_Position,x  ;compare horizontal coordinate of the enemy object
          cmp $01                 ;to modified horizontal left edge coordinate to get carry
          lda Enemy_PageLoc,x
          sbc $00                 ;then subtract it from the page coordinate of the enemy object
          bmi TooFar              ;if enemy object is too far left, branch to erase it
          lda Enemy_X_Position,x  ;compare horizontal coordinate of the enemy object
          cmp $03                 ;to modified horizontal right edge coordinate to get carry
          lda Enemy_PageLoc,x
          sbc $02                 ;then subtract it from the page coordinate of the enemy object
          bmi ExScrnBd            ;if enemy object is on the screen, leave, do not erase enemy
          lda Enemy_State,x       ;if at this point, enemy is offscreen to the right, so check
          cmp #HammerBro          ;if in state used by spiny's egg, do not erase
          beq ExScrnBd
          cpy #PiranhaPlant       ;if piranha plant, do not erase
          beq ExScrnBd
          cpy #FlagpoleFlagObject ;if flagpole flag, do not erase
          beq ExScrnBd
          cpy #StarFlagObject     ;if star flag, do not erase
          beq ExScrnBd
          cpy #JumpspringObject   ;if jumpspring, do not erase
          beq ExScrnBd            ;erase all others too far to the right
TooFar:   jsr EraseEnemyObject    ;erase object if necessary
ExScrnBd: rts                     ;leave

.proc RelativeMiscPosition
  ldy #$02                ;set for misc object offsets
  jsr GetProperObjOffset  ;modify X to get proper misc object offset
  ldy #$06
  jmp RelWOfs             ;get the coordinates
.endproc


;-------------------------------------------------------------------------------------

PlayerHammerCollision:
        lda FrameCounter          ;get frame counter
        lsr                       ;shift d0 into carry
        bcc ExPHC                 ;branch to leave if d0 not set to execute every other frame
        lda TimerControl          ;if either master timer control
        ora Misc_OffscreenBits    ;or any offscreen bits for hammer are set,
        bne ExPHC                 ;branch to leave
        txa
        asl                       ;multiply misc object offset by four
        asl
        clc
        adc #$24                  ;add 36 or $24 bytes to get proper offset
        tay                       ;for misc object bounding box coordinates
        jsr PlayerCollisionCore   ;do player-to-hammer collision detection
        ldx ObjectOffset          ;get misc object offset
        bcc ClHCol                ;if no collision, then branch
        lda Misc_Collision_Flag,x ;otherwise read collision flag
        bne ExPHC                 ;if collision flag already set, branch to leave
        lda #$01
        sta Misc_Collision_Flag,x ;otherwise set collision flag now
        lda Misc_X_Speed,x
        eor #$ff                  ;get two's compliment of
        clc                       ;hammer's horizontal speed
        adc #$01
        sta Misc_X_Speed,x        ;set to send hammer flying the opposite direction
        lda StarInvincibleTimer   ;if star mario invincibility timer set,
        bne ExPHC                 ;branch to leave
        jmp InjurePlayer          ;otherwise jump to hurt player, do not return
ClHCol: lda #$00                  ;clear collision flag
        sta Misc_Collision_Flag,x
ExPHC:  rts
