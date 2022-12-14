
.include "common.inc"

.segment "LOWCODE"

;-------------------------------------------------------------------------------------
;$04 - address low to jump address
;$05 - address high to jump address
;$06 - jump address low
;$07 - jump address high
.proc JumpEngine
  asl          ;shift bit from contents of A
  tay
  pla          ;pull saved return address from stack
  sta $04      ;save to indirect
  pla
  sta $05
  iny
  lda ($04),y  ;load pointer from indirect
  sta $06      ;note that if an RTS is performed in next routine
  iny          ;it will return to the execution before the sub
  lda ($04),y  ;that called this routine
  sta $07
  jmp ($0006)  ;jump to the address we loaded
.endproc

;-------------------------------------------------------------------------------------

.proc GetScreenPosition

  lda ScreenLeft_X_Pos    ;get coordinate of screen's left boundary
  clc
  adc #$ff                ;add 255 pixels
  sta ScreenRight_X_Pos   ;store as coordinate of screen's right boundary
  lda ScreenLeft_PageLoc  ;get page number where left boundary is
  adc #$00                ;add carry from before
  sta ScreenRight_PageLoc ;store as page number where right boundary is
  rts
.endproc

;-------------------------------------------------------------------------------------
LoadAreaPointer:
  ; jroweboy inlined FindAreaPointer
  ldy WorldNumber        ;load offset from world variable
  lda WorldAddrOffsets,y
  clc                    ;add area number used to find data
  adc AreaNumber
  tay
  lda AreaAddrOffsets,y  ;from there we have our area pointer
  sta AreaPointer
GetAreaType:
  and #%01100000       ;mask out all but d6 and d5
  asl
  rol
  rol
  rol                  ;make %0xx00000 into %000000xx
  sta AreaType         ;save 2 MSB as area type
  rts

;-------------------------------------------------------------------------------------
;$00 - used in adding to get proper offset

.proc RelativePlayerPosition

  ldx #$00      ;set offsets for relative cooordinates
  ldy #$00      ;routine to correspond to player object
  jmp RelWOfs   ;get the coordinates
.endproc


RelativeFireballPosition:
  ldy #$00                    ;set for fireball offsets
  jsr GetProperObjOffset      ;modify X to get proper fireball offset
  ldy #$02
RelWOfs:
  jsr GetObjRelativePosition  ;get the coordinates
  ldx ObjectOffset            ;return original offset
  rts                         ;leave

.proc RelativeEnemyPosition
  lda #$01                     ;get coordinates of enemy object 
  ldy #$01                     ;relative to the screen
  jmp VariableObjOfsRelPos
.endproc

RelativeBlockPosition:
  lda #$09                     ;get coordinates of one block object
  ldy #$04                     ;relative to the screen
  jsr VariableObjOfsRelPos
  inx                          ;adjust offset for other block object if any
  inx
  lda #$09
  iny                          ;adjust other and get coordinates for other one
  ; fall through
VariableObjOfsRelPos:
  stx $00                     ;store value to add to A here
  clc
  adc $00                     ;add A to value stored
  tax                         ;use as enemy offset
  jsr GetObjRelativePosition
  ldx ObjectOffset            ;reload old object offset and leave
  rts

.proc GetObjRelativePosition
  lda SprObject_Y_Position,x  ;load vertical coordinate low
  sta SprObject_Rel_YPos,y    ;store here
  lda SprObject_X_Position,x  ;load horizontal coordinate
  sec                         ;subtract left edge coordinate
  sbc ScreenLeft_X_Pos
  sta SprObject_Rel_XPos,y    ;store result here
  rts
.endproc

;-------------------------------------------------------------------------------------
;$00 - used as temp variable to hold offscreen bits

.proc GetPlayerOffscreenBits
  ldx #$00                 ;set offsets for player-specific variables
  ldy #$00                 ;and get offscreen information about player
  jmp GetOffScreenBitsSet
.endproc


.proc GetMiscOffscreenBits
  ldy #$02                 ;set for misc object offsets
  jsr GetProperObjOffset   ;modify X to get proper misc object offset
  ldy #$06                 ;set other offset for misc object's offscreen bits
  jmp GetOffScreenBitsSet  ;and get offscreen information about misc object
.endproc

.proc GetProperObjOffset
  txa                  ;move offset to A
  clc
  adc ObjOffsetData,y  ;add amount of bytes to offset depending on setting in Y
  tax                  ;put back in X and leave
  rts
ObjOffsetData:
  .byte $07, $16, $0d
.endproc

.proc GetEnemyOffscreenBits
  lda #$01                 ;set A to add 1 byte in order to get enemy offset
  ldy #$01                 ;set Y to put offscreen bits in Enemy_OffscreenBits
  jmp SetOffscrBitsOffset
.endproc

GetBlockOffscreenBits:
  lda #$09       ;set A to add 9 bytes in order to get block obj offset
  ldy #$04       ;set Y to put offscreen bits in Block_OffscreenBits
  ; fallthrough
SetOffscrBitsOffset:
  stx $00
  clc           ;add contents of X to A to get
  adc $00       ;appropriate offset, then give back to X
  tax
  ; fallthrough
GetOffScreenBitsSet:
  tya                         ;save offscreen bits offset to stack for now
  pha
    jsr RunOffscrBitsSubs
    asl                         ;move low nybble to high nybble
    asl
    asl
    asl
    ora $00                     ;mask together with previously saved low nybble
    sta $00                     ;store both here
  pla                         ;get offscreen bits offset from stack
  tay
  lda $00                     ;get value here and store elsewhere
  sta SprObject_OffscrBits,y
  ldx ObjectOffset
  rts

.proc RunOffscrBitsSubs
  jsr GetXOffscreenBits  ;do subroutine here
  lsr                    ;move high nybble to low
  lsr
  lsr
  lsr
  sta $00                ;store here
  jmp GetYOffscreenBits
.endproc

;--------------------------------
;(these apply to these three subsections)
;$04 - used to store offset to sprite object data
;$05 - used as adder in DividePDiff
;$06 - used to store constant used to compare to pixel difference in $07
;$07 - used to store pixel difference between X positions of object and screen edges

.proc GetXOffscreenBits
  stx $04                     ;save position in buffer to here
  ldy #$01                    ;start with right side of screen
XOfsLoop:
    lda ScreenEdge_X_Pos,y      ;get pixel coordinate of edge
    sec                         ;get difference between pixel coordinate of edge
    sbc SprObject_X_Position,x  ;and pixel coordinate of object position
    sta $07                     ;store here
    lda ScreenEdge_PageLoc,y    ;get page location of edge
    sbc SprObject_PageLoc,x     ;subtract page location of object position from it
    ldx DefaultXOnscreenOfs,y   ;load offset value here
    cmp #$00
    bmi XLdBData                ;if beyond right edge or in front of left edge, branch
      ldx DefaultXOnscreenOfs+1,y ;if not, load alternate offset value here
      cmp #$01      
      bpl XLdBData                ;if one page or more to the left of either edge, branch
        lda #$38                    ;if no branching, load value here and store
        sta $06
        lda #$08                    ;load some other value and execute subroutine
        jsr DividePDiff
XLdBData:
    lda XOffscreenBitsData,x    ;get bits here
    ldx $04                     ;reobtain position in buffer
    cmp #$00                    ;if bits not zero, branch to leave
    bne ExXOfsBS
    dey                         ;otherwise, do left side of screen now
    bpl XOfsLoop                ;branch if not already done with left side
ExXOfsBS:
  rts

XOffscreenBitsData:
  .byte $7f, $3f, $1f, $0f, $07, $03, $01, $00
  .byte $80, $c0, $e0, $f0, $f8, $fc, $fe, $ff

DefaultXOnscreenOfs:
  .byte $07, $0f, $07
.endproc

;--------------------------------


.proc GetYOffscreenBits
  stx $04                      ;save position in buffer to here
  ldy #$01                     ;start with top of screen
YOfsLoop:
  lda HighPosUnitData,y        ;load coordinate for edge of vertical unit
  sec
  sbc SprObject_Y_Position,x   ;subtract from vertical coordinate of object
  sta $07                      ;store here
  lda #$01                     ;subtract one from vertical high byte of object
  sbc SprObject_Y_HighPos,x
  ldx DefaultYOnscreenOfs,y    ;load offset value here
  cmp #$00
  bmi YLdBData                 ;if under top of the screen or beyond bottom, branch
  ldx DefaultYOnscreenOfs+1,y  ;if not, load alternate offset value here
  cmp #$01
  bpl YLdBData                 ;if one vertical unit or more above the screen, branch
  lda #$20                     ;if no branching, load value here and store
  sta $06
  lda #$04                     ;load some other value and execute subroutine
  jsr DividePDiff
YLdBData:
  lda YOffscreenBitsData,x     ;get offscreen data bits using offset
  ldx $04                      ;reobtain position in buffer
  cmp #$00
  bne ExYOfsBS                 ;if bits not zero, branch to leave
  dey                          ;otherwise, do bottom of the screen now
  bpl YOfsLoop
ExYOfsBS: rts

YOffscreenBitsData:
  .byte $00, $08, $0c, $0e
  .byte $0f, $07, $03, $01
  .byte $00

DefaultYOnscreenOfs:
  .byte $04, $00, $04

HighPosUnitData:
  .byte $ff, $00

.endproc

;--------------------------------

.proc DividePDiff
.export DividePDiff

  sta $05       ;store current value in A here
  lda $07       ;get pixel difference
  cmp $06       ;compare to preset value
  bcs ExDivPD   ;if pixel difference >= preset value, branch
  lsr           ;divide by eight to get tile difference
  lsr
  lsr
  and #$07      ;mask out all but 3 LSB
  cpy #$01      ;right side of the screen or top?
  bcs SetOscrO  ;if so, branch, use difference / 8 as offset
  adc $05       ;if not, add value to difference / 8
SetOscrO:
  tax           ;use as offset
ExDivPD:
  rts           ;leave
.endproc

.proc TransposePlayers
  sec                       ;set carry flag by default to end game
  lda NumberOfPlayers       ;if only a 1 player game, leave
  beq ExTrans
  lda OffScr_NumberofLives  ;does offscreen player have any lives left?
  bmi ExTrans               ;branch if not
  lda CurrentPlayer         ;invert bit to update
  eor #%00000001            ;which player is on the screen
  sta CurrentPlayer
  ldx #$06
TransLoop:
  lda OnscreenPlayerInfo,x    ;transpose the information
  pha                         ;of the onscreen player
    lda OffscreenPlayerInfo,x   ;with that of the offscreen player
    sta OnscreenPlayerInfo,x
  pla
  sta OffscreenPlayerInfo,x
  dex
  bpl TransLoop
  clc            ;clear carry flag to get game going
ExTrans:
  rts
.endproc

;--------------------------------

MaxSpdBlockData:
      .byte $06, $08

ImposeGravityBlock:
      ldy #$01       ;set offset for maximum speed
      lda #$50       ;set movement amount here
      sta $00
      lda MaxSpdBlockData,y    ;get maximum speed

ImposeGravitySprObj:
      sta $02            ;set maximum speed here
      lda #$00           ;set value to move downwards
      jmp ImposeGravity  ;jump to the code that actually moves it

;-------------------------------------------------------------------------------------
;$00 - used for downward force
;$01 - used for upward force
;$07 - used as adder for vertical position

.proc ImposeGravity

  pha                          ;push value to stack
    lda Player_YMoveForceFractional,x
    clc                          ;add value in movement force to contents of dummy variable
    adc SprObject_Y_MoveForce,x
    sta Player_YMoveForceFractional,x
    ldy #$00                     ;set Y to zero by default
    lda SprObject_Y_Speed,x      ;get current vertical speed
    bpl AlterYP                  ;if currently moving downwards, do not decrement Y
    dey                          ;otherwise decrement Y
AlterYP:
    sty $07                      ;store Y here
    adc SprObject_Y_Position,x   ;add vertical position to vertical speed plus carry
    sta SprObject_Y_Position,x   ;store as new vertical position
    lda SprObject_Y_HighPos,x
    adc $07                      ;add carry plus contents of $07 to vertical high byte
    sta SprObject_Y_HighPos,x    ;store as new vertical high byte
    lda SprObject_Y_MoveForce,x
    clc
    adc $00                      ;add downward movement amount to contents of $0433
    sta SprObject_Y_MoveForce,x
    lda SprObject_Y_Speed,x      ;add carry to vertical speed and store
    adc #$00
    sta SprObject_Y_Speed,x
    cmp $02                      ;compare to maximum speed
    bmi ChkUpM                   ;if less than preset value, skip this part
    lda SprObject_Y_MoveForce,x
    cmp #$80                     ;if less positively than preset maximum, skip this part
    bcc ChkUpM
    lda $02
    sta SprObject_Y_Speed,x      ;keep vertical speed within maximum value
    lda #$00
    sta SprObject_Y_MoveForce,x  ;clear fractional
ChkUpM:
  pla                          ;get value from stack
  beq ExVMove                  ;if set to zero, branch to leave
  lda $02
  eor #%11111111               ;otherwise get two's compliment of maximum speed
  tay
  iny
  sty $07                      ;store two's compliment here
  lda SprObject_Y_MoveForce,x
  sec                          ;subtract upward movement amount from contents
  sbc $01                      ;of movement force, note that $01 is twice as large as $00,
  sta SprObject_Y_MoveForce,x  ;thus it effectively undoes add we did earlier
  lda SprObject_Y_Speed,x
  sbc #$00                     ;subtract borrow from vertical speed and store
  sta SprObject_Y_Speed,x
  cmp $07                      ;compare vertical speed to two's compliment
  bpl ExVMove                  ;if less negatively than preset maximum, skip this part
  lda SprObject_Y_MoveForce,x
  cmp #$80                     ;check if fractional part is above certain amount,
  bcs ExVMove                  ;and if so, branch to leave
  lda $07
  sta SprObject_Y_Speed,x      ;keep vertical speed within maximum value
  lda #$ff
  sta SprObject_Y_MoveForce,x  ;clear fractional
ExVMove:
  rts                          ;leave!
.endproc

;--------------------------------
;$00 - used to hold horizontal difference between player and enemy

.proc PlayerEnemyDiff
  lda Enemy_X_Position,x  ;get distance between enemy object's
  sec                     ;horizontal coordinate and the player's
  sbc Player_X_Position   ;horizontal coordinate
  sta $00                 ;and store here
  lda Enemy_PageLoc,x
  sbc Player_PageLoc      ;subtract borrow, then leave
  rts
.endproc

Bitmasks:
  .byte %00000001, %00000010, %00000100, %00001000, %00010000, %00100000, %01000000, %10000000

;-------------------------------------------------------------------------------------
;$00 - used to store high nybble of horizontal speed as adder
;$01 - used to store low nybble of horizontal speed
;$02 - used to store adder to page location

MoveEnemyHorizontally:
      inx                         ;increment offset for enemy offset
      jsr MoveObjectHorizontally  ;position object horizontally according to
      ldx ObjectOffset            ;counters, return with saved value in A,
      rts                         ;put enemy offset back in X and leave

MovePlayerHorizontally:
      lda JumpspringAnimCtrl  ;if jumpspring currently animating,
      bne ExXMove             ;branch to leave
      tax                     ;otherwise set zero for offset to use player's stuff

MoveObjectHorizontally:
          lda SprObject_X_Speed,x     ;get currently saved value (horizontal
          asl                         ;speed, secondary counter, whatever)
          asl                         ;and move low nybble to high
          asl
          asl
          sta $01                     ;store result here
          lda SprObject_X_Speed,x     ;get saved value again
          lsr                         ;move high nybble to low
          lsr
          lsr
          lsr
          cmp #$08                    ;if < 8, branch, do not change
          bcc SaveXSpd
          ora #%11110000              ;otherwise alter high nybble
SaveXSpd: sta $00                     ;save result here
          ldy #$00                    ;load default Y value here
          cmp #$00                    ;if result positive, leave Y alone
          bpl UseAdder
          dey                         ;otherwise decrement Y
UseAdder: sty $02                     ;save Y here
          lda SprObject_X_MoveForce,x ;get whatever number's here
          clc
          adc $01                     ;add low nybble moved to high
          sta SprObject_X_MoveForce,x ;store result here
          lda #$00                    ;init A
          rol                         ;rotate carry into d0
          pha                         ;push onto stack
          ror                         ;rotate d0 back onto carry
          lda SprObject_X_Position,x
          adc $00                     ;add carry plus saved value (high nybble moved to low
          sta SprObject_X_Position,x  ;plus $f0 if necessary) to object's horizontal position
          lda SprObject_PageLoc,x
          adc $02                     ;add carry plus other saved value to the
          sta SprObject_PageLoc,x     ;object's page location and save
          pla
          clc                         ;pull old carry from stack and add
          adc $00                     ;to high nybble moved to low
ExXMove:  rts                         ;and leave

;--------------------------------

MoveD_EnemyVertically:
      ldy #$3d           ;set quick movement amount downwards
      lda Enemy_State,x  ;then check enemy state
      cmp #$05           ;if not set to unique state for spiny's egg, go ahead
      bne ContVMove      ;and use, otherwise set different movement amount, continue on

MoveFallingPlatform:
           ldy #$20       ;set movement amount
ContVMove: jmp SetHiMax   ;jump to skip the rest of this

;--------------------------------

MoveDropPlatform:
      ldy #$7f      ;set movement amount for drop platform
      bne SetMdMax  ;skip ahead of other value set here

MoveEnemySlowVert:
          ldy #$0f         ;set movement amount for bowser/other objects
SetMdMax: lda #$02         ;set maximum speed in A
          bne SetXMoveAmt  ;unconditional branch

;--------------------------------

MoveJ_EnemyVertically:
             ldy #$1c                ;set movement amount for podoboo/other objects
SetHiMax:    lda #$03                ;set maximum speed in A
SetXMoveAmt: sty $00                 ;set movement amount here
             inx                     ;increment X for enemy offset
             jsr ImposeGravitySprObj ;do a sub to move enemy object downwards
             ldx ObjectOffset        ;get enemy object buffer offset and leave
             rts

;--------------------------------

MovePlatformDown:
  lda #$00    ;save value to stack (if branching here, execute next
  .byte $2c     ;part as BIT instruction)
MovePlatformUp:
  lda #$01        ;save value to stack
  pha
    ldy Enemy_ID,x  ;get enemy object identifier
    inx             ;increment offset for enemy object
    lda #$05        ;load default value here
SetDplSpd:
    sta $00         ;save downward movement amount here
    lda #$0a        ;save upward movement amount here
    sta $01
    lda #$03        ;save maximum vertical speed here
    sta $02
  pla             ;get value from stack
  tay             ;use as Y, then move onto code shared by red koopa

RedPTroopaGrav:
  jsr ImposeGravity  ;do a sub to move object gradually
  ldx ObjectOffset   ;get enemy object offset and leave
  rts

;------------------------------------------------
; Moved from object handler since this is common code
InitVStf:
  lda #$00                    ;initialize vertical speed
  sta Enemy_Y_Speed,x         ;and movement force
  sta Enemy_Y_MoveForce,x
  rts

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

WorldAddrOffsets:
      .byte World1Areas-AreaAddrOffsets, World2Areas-AreaAddrOffsets
      .byte World3Areas-AreaAddrOffsets, World4Areas-AreaAddrOffsets
      .byte World5Areas-AreaAddrOffsets, World6Areas-AreaAddrOffsets
      .byte World7Areas-AreaAddrOffsets, World8Areas-AreaAddrOffsets

AreaAddrOffsets:
World1Areas: .byte $25, $29, $c0, $26, $60
World2Areas: .byte $28, $29, $01, $27, $62
World3Areas: .byte $24, $35, $20, $63
World4Areas: .byte $22, $29, $41, $2c, $61
World5Areas: .byte $2a, $31, $26, $62
World6Areas: .byte $2e, $23, $2d, $60
World7Areas: .byte $33, $29, $01, $27, $64
World8Areas: .byte $30, $32, $21, $65


