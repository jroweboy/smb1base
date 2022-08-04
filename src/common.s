
.include "common.inc"

;-------------------------------------------------------------------------------------

.proc GetScreenPosition
.export GetScreenPosition

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
.export LoadAreaPointer
LoadAreaPointer:
  jsr FindAreaPointer  ;find it and store it here
  sta AreaPointer
GetAreaType:
  and #%01100000       ;mask out all but d6 and d5
  asl
  rol
  rol
  rol                  ;make %0xx00000 into %000000xx
  sta AreaType         ;save 2 MSB as area type
  rts

.proc FindAreaPointer
  .import WorldAddrOffsets, AreaAddrOffsets

  ldy WorldNumber        ;load offset from world variable
  lda WorldAddrOffsets,y
  clc                    ;add area number used to find data
  adc AreaNumber
  tay
  lda AreaAddrOffsets,y  ;from there we have our area pointer
  rts
.endproc


.proc GetAreaDataAddrs
.import EnemyAddrHOffsets, EnemyDataAddrLow, EnemyDataAddrHigh, AreaDataHOffsets, AreaDataAddrLow, AreaDataAddrHigh
.export GetAreaDataAddrs

  lda AreaPointer          ;use 2 MSB for Y
  jsr GetAreaType
  tay
  lda AreaPointer          ;mask out all but 5 LSB
  and #%00011111
  sta AreaAddrsLOffset     ;save as low offset
  lda EnemyAddrHOffsets,y  ;load base value with 2 altered MSB,
  clc                      ;then add base value to 5 LSB, result
  adc AreaAddrsLOffset     ;becomes offset for level data
  tay
  lda EnemyDataAddrLow,y   ;use offset to load pointer
  sta EnemyDataLow
  lda EnemyDataAddrHigh,y
  sta EnemyDataHigh
  ldy AreaType             ;use area type as offset
  lda AreaDataHOffsets,y   ;do the same thing but with different base value
  clc
  adc AreaAddrsLOffset        
  tay
  lda AreaDataAddrLow,y    ;use this offset to load another pointer
  sta AreaDataLow
  lda AreaDataAddrHigh,y
  sta AreaDataHigh
  ldy #$00                 ;load first byte of header
  lda (AreaData),y     
  pha                      ;save it to the stack for now
  and #%00000111           ;save 3 LSB for foreground scenery or bg color control
  cmp #$04
  bcc StoreFore
  sta BackgroundColorCtrl  ;if 4 or greater, save value here as bg color control
  lda #$00
StoreFore:
  sta ForegroundScenery    ;if less, save value here as foreground scenery
  pla                      ;pull byte from stack and push it back
  pha
  and #%00111000           ;save player entrance control bits
  lsr                      ;shift bits over to LSBs
  lsr
  lsr
  sta PlayerEntranceCtrl       ;save value here as player entrance control
  pla                      ;pull byte again but do not push it back
  and #%11000000           ;save 2 MSB for game timer setting
  clc
  rol                      ;rotate bits over to LSBs
  rol
  rol
  sta GameTimerSetting     ;save value here as game timer setting
  iny
  lda (AreaData),y         ;load second byte of header
  pha                      ;save to stack
  and #%00001111           ;mask out all but lower nybble
  sta TerrainControl
  pla                      ;pull and push byte to copy it to A
  pha
  and #%00110000           ;save 2 MSB for background scenery type
  lsr
  lsr                      ;shift bits to LSBs
  lsr
  lsr
  sta BackgroundScenery    ;save as background scenery
  pla           
  and #%11000000
  clc
  rol                      ;rotate bits over to LSBs
  rol
  rol
  cmp #%00000011           ;if set to 3, store here
  bne StoreStyle           ;and nullify other value
  sta CloudTypeOverride    ;otherwise store value in other place
  lda #$00
StoreStyle:
  sta AreaStyle
  lda AreaDataLow          ;increment area data address by 2 bytes
  clc
  adc #$02
  sta AreaDataLow
  lda AreaDataHigh
  adc #$00
  sta AreaDataHigh
  rts
.endproc


;-------------------------------------------------------------------------------------
;$00 - used in adding to get proper offset

.proc RelativePlayerPosition
.import RelWOfs

  ldx #$00      ;set offsets for relative cooordinates
  ldy #$00      ;routine to correspond to player object
  jmp RelWOfs   ;get the coordinates
.endproc

.proc RelativeBubblePosition
.import RelWOfs

  ldy #$01                ;set for air bubble offsets
  jsr GetProperObjOffset  ;modify X to get proper air bubble offset
  ldy #$03
  jmp RelWOfs             ;get the coordinates
.endproc

.proc RelativeFireballPosition
.export RelWOfs

  ldy #$00                    ;set for fireball offsets
  jsr GetProperObjOffset      ;modify X to get proper fireball offset
  ldy #$02
RelWOfs:
  jsr GetObjRelativePosition  ;get the coordinates
  ldx ObjectOffset            ;return original offset
  rts                         ;leave
.endproc

.proc RelativeMiscPosition
.import RelWOfs

  ldy #$02                ;set for misc object offsets
  jsr GetProperObjOffset  ;modify X to get proper misc object offset
  ldy #$06
  jmp RelWOfs             ;get the coordinates
.endproc

.proc RelativeEnemyPosition
.import VariableObjOfsRelPos

  lda #$01                     ;get coordinates of enemy object 
  ldy #$01                     ;relative to the screen
  jmp VariableObjOfsRelPos
.endproc

.proc RelativeBlockPosition
.export VariableObjOfsRelPos

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
.endproc

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
.import GetOffScreenBitsSet
  ldx #$00                 ;set offsets for player-specific variables
  ldy #$00                 ;and get offscreen information about player
  jmp GetOffScreenBitsSet
.endproc

.proc GetFireballOffscreenBits
.import GetOffScreenBitsSet
  ldy #$00                 ;set for fireball offsets
  jsr GetProperObjOffset   ;modify X to get proper fireball offset
  ldy #$02                 ;set other offset for fireball's offscreen bits
  jmp GetOffScreenBitsSet  ;and get offscreen information about fireball
.endproc

.proc GetBubbleOffscreenBits
.import GetOffScreenBitsSet
  ldy #$01                 ;set for air bubble offsets
  jsr GetProperObjOffset   ;modify X to get proper air bubble offset
  ldy #$03                 ;set other offset for airbubble's offscreen bits
  jmp GetOffScreenBitsSet  ;and get offscreen information about air bubble
.endproc

.proc GetMiscOffscreenBits
.import GetOffScreenBitsSet
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
.import SetOffscrBitsOffset

  lda #$01                 ;set A to add 1 byte in order to get enemy offset
  ldy #$01                 ;set Y to put offscreen bits in Enemy_OffscreenBits
  jmp SetOffscrBitsOffset
.endproc

.proc GetBlockOffscreenBits
.export SetOffscrBitsOffset, GetOffScreenBitsSet
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
.endproc

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
ExXOfsBS: rts

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
.export TransposePlayers

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


;-------------------------------------------------------------------------------------
;$00 - used for downward force
;$01 - used for upward force
;$07 - used as adder for vertical position

.proc ImposeGravity
.export ImposeGravity

  pha                          ;push value to stack
    lda SprObject_YMF_Dummy,x
    clc                          ;add value in movement force to contents of dummy variable
    adc SprObject_Y_MoveForce,x
    sta SprObject_YMF_Dummy,x
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
.export PlayerEnemyDiff
  lda Enemy_X_Position,x  ;get distance between enemy object's
  sec                     ;horizontal coordinate and the player's
  sbc Player_X_Position   ;horizontal coordinate
  sta $00                 ;and store here
  lda Enemy_PageLoc,x
  sbc Player_PageLoc      ;subtract borrow, then leave
  rts
.endproc

.export Bitmasks
Bitmasks:
      .byte %00000001, %00000010, %00000100, %00001000, %00010000, %00100000, %01000000, %10000000
