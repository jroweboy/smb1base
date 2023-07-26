.include "common.inc"
.include "object.inc"

; sprite_render.s
.import EnemyGfxHandler, JCoinGfxHandler, DrawHammer, FloateyNumbersRoutine
; collision.s
.import PlayerCollisionCore,InjurePlayer,EnemyToBGCollisionDet,EnemiesCollision

; player.s
.export BoundingBoxCore

; gamecore.s gamemode.s
.export EnemiesAndLoopsCore

; gamecore.s
.export MiscObjectsCore, ProcessSingleEnemy

.segment "OBJECT"

;-------------------------------------------------------------------------------------
; ProcessAllEnemies:
;   ldx #$00
; ProcELoop:
;     stx ObjectOffset           ;put incremented offset in X as enemy object offset
;     jsr EnemiesAndLoopsCore    ;process enemy objects
;     jsr FloateyNumbersRoutine  ;process floatey numbers
;     inx
;     cpx #$06                   ;do these two subroutines until the whole buffer is done
;     bne ProcELoop
;   rts

.proc ProcessSingleEnemy
  stx ObjectOffset
  jsr EnemiesAndLoopsCore
  jmp FloateyNumbersRoutine
.endproc

.proc EnemiesAndLoopsCore
  lda Enemy_Flag,x         ;check data here for MSB set
  bmi ChkBowserF           ;if MSB set in enemy flag, branch ahead of jumps
  beq ChkAreaTsk           ;if data zero, branch
    jmp RunEnemyObjectsCore  ;otherwise, jump to run enemy subroutines
ChkAreaTsk:
  lda AreaParserTaskNum    ;check number of tasks to perform
  and #$07
  cmp #$07                 ;if at a specific task, jump and leave
  beq ExitELCore
    farcall ProcLoopCommand, jmp ;otherwise, jump to process loop command/load enemies
ChkBowserF:
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
JmpEO:
  jsr JumpEngine
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
  .word RunFlagpoleShard ; $36

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
  .word EndOfEnemyInitCode ; flagpole shard

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

MiscObjectsCore:
  ldx #$08          ;set at end of misc object buffer
MiscLoop:
    stx ObjectOffset  ;store misc object offset here
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
           sta R0
           lda #$06                  ;set maximum vertical speed
           sta R2
           lsr                       ;divide by 2 and set
           sta R1                   ;as upward movement amount (apparently residual)
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
          and #%00111111             ;mask out d7
          ldy HammerEnemyOffset,x    ;get enemy object offset that spawned this hammer
          cmp #$02                   ;check hammer's state
          beq SetHSpd                ;if currently at 2, branch
          bcs SetHPos                ;if greater than 2, branch elsewhere
          txa
          clc                        ;add 13 bytes to use
          adc #$0d                   ;proper misc object
          tax                        ;return offset to X
          lda #$10
          sta R0                    ;set downward movement force
          lda #$0f
          sta R1                    ;set upward movement force (not used)
          lda #$04
          sta R2                    ;set maximum vertical speed
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
          jmp DrawHammer             ;draw the hammer

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
NoInitCode:
EndOfEnemyInitCode:
NoRunCode:
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
          sta R1                 ;store result here
          lda ScreenLeft_PageLoc
          sbc #$00                ;subtract borrow from page location of left side
          sta R0                 ;store result here
          lda ScreenRight_X_Pos   ;add 72 pixels to the right side horizontal coordinate
          adc #$48
          sta R3                 ;store result here
          lda ScreenRight_PageLoc     
          adc #$00                ;then add the carry to the page location
          sta R2                 ;and store result here
          lda Enemy_X_Position,x  ;compare horizontal coordinate of the enemy object
          cmp R1                 ;to modified horizontal left edge coordinate to get carry
          lda Enemy_PageLoc,x
          sbc R0                 ;then subtract it from the page coordinate of the enemy object
          bmi TooFar              ;if enemy object is too far left, branch to erase it
          lda Enemy_X_Position,x  ;compare horizontal coordinate of the enemy object
          cmp R3                 ;to modified horizontal right edge coordinate to get carry
          lda Enemy_PageLoc,x
          sbc R2                 ;then subtract it from the page coordinate of the enemy object
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
TooFar:   jmp EraseEnemyObject    ;erase object if necessary
ExScrnBd: rts ; TODO check this RTS can be removed                     ;leave

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

;--------------------------------

NormalXSpdData:
      .byte $f8, $f4

InitNormalEnemy:
         ldy #$01              ;load offset of 1 by default
         lda PrimaryHardMode   ;check for primary hard mode flag set
         bne GetESpd
         dey                   ;if not set, decrement offset
GetESpd: lda NormalXSpdData,y  ;get appropriate horizontal speed
SetESpd: sta Enemy_X_Speed,x   ;store as speed for enemy object
         jmp TallBBox          ;branch to set bounding box control and other data


;--------------------------------
;$00 - used to store adder for movement, also used as adder for platform
;$01 - used to store maximum value for secondary counter

MoveFlyGreenPTroopa:
        jsr XMoveCntr_GreenPTroopa ;do sub to increment primary and secondary counters
        jsr MoveWithXMCntrs        ;do sub to move green paratroopa accordingly, and horizontally
        ldy #$01                   ;set Y to move green paratroopa down
        lda FrameCounter
        and #%00000011             ;check frame counter 2 LSB for any bits set
        bne NoMGPT                 ;branch to leave if set to move up/down every fourth frame
        lda FrameCounter
        and #%01000000             ;check frame counter for d6 set
        bne YSway                  ;branch to move green paratroopa down if set
        ldy #$ff                   ;otherwise set Y to move green paratroopa up
YSway:  sty R0                    ;store adder here
        lda Enemy_Y_Position,x
        clc                        ;add or subtract from vertical position
        adc R0                    ;to give green paratroopa a wavy flight
        sta Enemy_Y_Position,x
NoMGPT: rts                        ;leave!

XMoveCntr_GreenPTroopa:
         lda #$13                    ;load preset maximum value for secondary counter

XMoveCntr_Platform:
         sta R1                     ;store value here
         lda FrameCounter
         and #%00000011              ;branch to leave if not on
         bne NoIncXM                 ;every fourth frame
         ldy XMoveSecondaryCounter,x ;get secondary counter
         lda XMovePrimaryCounter,x   ;get primary counter
         lsr
         bcs DecSeXM                 ;if d0 of primary counter set, branch elsewhere
         cpy R1                     ;compare secondary counter to preset maximum value
         beq IncPXM                  ;if equal, branch ahead of this part
         inc XMoveSecondaryCounter,x ;increment secondary counter and leave
NoIncXM: rts
IncPXM:  inc XMovePrimaryCounter,x   ;increment primary counter and leave
         rts
DecSeXM: tya                         ;put secondary counter in A
         beq IncPXM                  ;if secondary counter at zero, branch back
         dec XMoveSecondaryCounter,x ;otherwise decrement secondary counter and leave
         rts

MoveWithXMCntrs:
         lda XMoveSecondaryCounter,x  ;save secondary counter to stack
         pha
         ldy #$01                     ;set value here by default
         lda XMovePrimaryCounter,x
         and #%00000010               ;if d1 of primary counter is
         bne XMRight                  ;set, branch ahead of this part here
         lda XMoveSecondaryCounter,x
         eor #$ff                     ;otherwise change secondary
         clc                          ;counter to two's compliment
         adc #$01
         sta XMoveSecondaryCounter,x
         ldy #$02                     ;load alternate value here
XMRight: sty Enemy_MovingDir,x        ;store as moving direction
         jsr MoveEnemyHorizontally
         sta R0                      ;save value obtained from sub here
         pla                          ;get secondary counter from stack
         sta XMoveSecondaryCounter,x  ;and return to original place
         rts

;--------------------------------

InitRetainerObj:
      lda #$b8                ;set fixed vertical position for
      sta Enemy_Y_Position,x  ;princess/mushroom retainer object
      rts


;--------------------------------

InitHorizFlySwimEnemy:
      lda #$00        ;initialize horizontal speed
      jmp SetESpd



;--------------------------------

InitJumpGPTroopa:
           lda #$02                  ;set for movement to the left
           sta Enemy_MovingDir,x
           lda #$f8                  ;set horizontal speed
           sta Enemy_X_Speed,x
TallBBox2: lda #$03                  ;set specific value for bounding box control
SetBBox2:  sta Enemy_BoundBoxCtrl,x  ;set bounding box control then leave
           rts


;-------------------------------------------------------------------------------------
;$00 - used to hold one of bitmasks, or offset
;$01 - used for relative X coordinate, also used to store middle screen page location
;$02 - used for relative Y coordinate, also used to store middle screen coordinate

;this data added to relative coordinates of sprite objects
;stored in order: left edge, top edge, right edge, bottom edge
BoundBoxCtrlData:
  .byte $02, $08, $0e, $20  ; big mario
  .byte $03, $14, $0d, $20  ; small mario
  .byte $02, $14, $0e, $20  ; big mario crouching
  .byte $02, $09, $0e, $15
  .byte $00, $00, $18, $06
  .byte $00, $00, $20, $0d
  .byte $00, $00, $30, $0d
  .byte $00, $00, $08, $08
  .byte $06, $04, $0a, $08
  .byte $03, $0e, $0d, $14
  .byte $00, $02, $10, $15
  .byte $04, $04, $0c, $1c

GetFireballBoundBox:
      txa         ;add seven bytes to offset
      clc         ;to use in routines as offset for fireball
      adc #$07
      tax
      ldy #$02    ;set offset for relative coordinates
      bne FBallB  ;unconditional branch

GetMiscBoundBox:
        txa                       ;add nine bytes to offset
        clc                       ;to use in routines as offset for misc object
        adc #$09
        tax
        ldy #$06                  ;set offset for relative coordinates
FBallB: jsr BoundingBoxCore       ;get bounding box coordinates
        jmp CheckRightScreenBBox  ;jump to handle any offscreen coordinates

GetEnemyBoundBox:
      ldy #$48                 ;store bitmask here for now
      sty R0
      ldy #$44                 ;store another bitmask here for now and jump
      jmp GetMaskedOffScrBits

SmallPlatformBoundBox:
      ldy #$08                 ;store bitmask here for now
      sty R0
      ldy #$04                 ;store another bitmask here for now

GetMaskedOffScrBits:
        lda Enemy_X_Position,x      ;get enemy object position relative
        sec                         ;to the left side of the screen
        sbc ScreenLeft_X_Pos
        sta R1                     ;store here
        lda Enemy_PageLoc,x         ;subtract borrow from current page location
        sbc ScreenLeft_PageLoc      ;of left side
        bmi CMBits                  ;if enemy object is beyond left edge, branch
        ora R1
        beq CMBits                  ;if precisely at the left edge, branch
        ldy R0                     ;if to the right of left edge, use value in $00 for A
CMBits: tya                         ;otherwise use contents of Y
        and Enemy_OffscreenBits     ;preserve bitwise whatever's in here
        sta EnemyOffscrBitsMasked,x ;save masked offscreen bits here
        bne MoveBoundBoxOffscreen   ;if anything set here, branch
        jmp SetupEOffsetFBBox       ;otherwise, do something else

LargePlatformBoundBox:
      inx                        ;increment X to get the proper offset
      jsr GetXOffscreenBits      ;then jump directly to the sub for horizontal offscreen bits
      dex                        ;decrement to return to original offset
      cmp #$fe                   ;if completely offscreen, branch to put entire bounding
      bcs MoveBoundBoxOffscreen  ;box offscreen, otherwise start getting coordinates

SetupEOffsetFBBox:
      txa                        ;add 1 to offset to properly address
      clc                        ;the enemy object memory locations
      adc #$01
      tax
      ldy #$01                   ;load 1 as offset here, same reason
      jsr BoundingBoxCore        ;do a sub to get the coordinates of the bounding box
      jmp CheckRightScreenBBox   ;jump to handle offscreen coordinates of bounding box

MoveBoundBoxOffscreen:
      txa                            ;multiply offset by 4
      asl
      asl
      tay                            ;use as offset here
      lda #$ff
      sta EnemyBoundingBoxCoord,y    ;load value into four locations here and leave
      sta EnemyBoundingBoxCoord+1,y
      sta EnemyBoundingBoxCoord+2,y
      sta EnemyBoundingBoxCoord+3,y
      rts

BoundingBoxCore:
  stx R0                     ;save offset here
  lda SprObject_Rel_YPos,y    ;store object coordinates relative to screen
  sta R2                     ;vertically and horizontally, respectively
  lda SprObject_Rel_XPos,y
  sta R1
  txa                         ;multiply offset by four and save to stack
  asl
  asl
  pha
    tay                         ;use as offset for Y, X is left alone
    lda SprObj_BoundBoxCtrl,x   ;load value here to be used as offset for X
    asl                         ;multiply that by four and use as X
    asl
    tax
    lda R1                     ;add the first number in the bounding box data to the
    clc                         ;relative horizontal coordinate using enemy object offset
    adc BoundBoxCtrlData,x      ;and store somewhere using same offset * 4
    sta BoundingBox_UL_Corner,y ;store here
    lda R1
    clc
    adc BoundBoxCtrlData+2,x    ;add the third number in the bounding box data to the
    sta BoundingBox_LR_Corner,y ;relative horizontal coordinate and store
    inx                         ;increment both offsets
    iny
    lda R2                     ;add the second number to the relative vertical coordinate
    clc                         ;using incremented offset and store using the other
    adc BoundBoxCtrlData,x      ;incremented offset
    sta BoundingBox_UL_Corner,y
    lda R2
    clc
    adc BoundBoxCtrlData+2,x    ;add the fourth number to the relative vertical coordinate
    sta BoundingBox_LR_Corner,y ;and store
  pla                         ;get original offset loaded into $00 * y from stack
  tay                         ;use as Y
  ldx R0                     ;get original offset and use as X again
  rts

CheckRightScreenBBox:
       lda ScreenLeft_X_Pos       ;add 128 pixels to left side of screen
       clc                        ;and store as horizontal coordinate of middle
       adc #$80
       sta R2
       lda ScreenLeft_PageLoc     ;add carry to page location of left side of screen
       adc #$00                   ;and store as page location of middle
       sta R1
       lda SprObject_X_Position,x ;get horizontal coordinate
       cmp R2                    ;compare against middle horizontal coordinate
       lda SprObject_PageLoc,x    ;get page location
       sbc R1                    ;subtract from middle page location
       bcc CheckLeftScreenBBox    ;if object is on the left side of the screen, branch
       lda BoundingBox_DR_XPos,y  ;check right-side edge of bounding box for offscreen
       bmi NoOfs                  ;coordinates, branch if still on the screen
       lda #$ff                   ;load offscreen value here to use on one or both horizontal sides
       ldx BoundingBox_UL_XPos,y  ;check left-side edge of bounding box for offscreen
       bmi SORte                  ;coordinates, and branch if still on the screen
       sta BoundingBox_UL_XPos,y  ;store offscreen value for left side
SORte: sta BoundingBox_DR_XPos,y  ;store offscreen value for right side
NoOfs: ldx ObjectOffset           ;get object offset and leave
       rts

CheckLeftScreenBBox:
        lda BoundingBox_UL_XPos,y  ;check left-side edge of bounding box for offscreen
        bpl NoOfs2                 ;coordinates, and branch if still on the screen
        cmp #$a0                   ;check to see if left-side edge is in the middle of the
        bcc NoOfs2                 ;screen or really offscreen, and branch if still on
        lda #$00
        ldx BoundingBox_DR_XPos,y  ;check right-side edge of bounding box for offscreen
        bpl SOLft                  ;coordinates, branch if still onscreen
        sta BoundingBox_DR_XPos,y  ;store offscreen value for right side
SOLft:  sta BoundingBox_UL_XPos,y  ;store offscreen value for left side
NoOfs2: ldx ObjectOffset           ;get object offset and leave
        rts

;--------------------------------

MoveJumpingEnemy:
      jsr MoveJ_EnemyVertically  ;do a sub to impose gravity on green paratroopa
      jmp MoveEnemyHorizontally  ;jump to move enemy horizontally

.proc RunFlagpoleShard
  ldx ObjectOffset
  jmp EnemyExplodingFlagPole
.endproc
