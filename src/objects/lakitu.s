
.include "common.inc"
.include "object.inc"

.segment "WRAM"

ID:            .res 2
XSpeed:        .res 2
YSpeed:        .res 2
XMoveForce:    .res 2
XPos:          .res 2
XPage:         .res 2
LastItem:      .res 2
ItemTimer:     .res 2
State:         .res 2
MoveDirection: .res 2
MoveSpeed:     .res 2

.segment "OBJECT"

.export HandleDiscoLakitu

; Add new Disco Lakitu

HandleDiscoLakitu:
  

DrawDiscoLakitu:
  AllocSpr (9 + 2)

  rts

LakituDiffAdj:
  .byte $15, $30, $40

MoveDiscoLakitu:
  lda State          ;if lakitu's enemy state not set at all,
  beq Fr12S                  ;go ahead and continue with code
  lda #$00
  sta MoveDirection     ;otherwise initialize moving direction to move to left
  lda #$10
  bne SetLSpd                ;load horizontal speed and do unconditional branch
Fr12S:
  lda #Spiny
  sta EnemyFrenzyBuffer      ;set spiny identifier in frenzy buffer
  ldy #$02
LdLDa:
  lda LakituDiffAdj,y        ;load values
    sta R1,y                   ;store in zero page
    dey
    bpl LdLDa                  ;do this until all values are stired
  jsr PlayerLakituDiff       ;execute sub to set speed and create spinys
SetLSpd:
  sta MoveSpeed              ;set movement speed returned from sub
  ldy #$01                   ;set moving direction to right by default
  lda MoveDirection
  and #$01                   ;get LSB of moving direction
  bne SetLMov                ;if set, branch to the end to use moving direction
    lda MoveSpeed
    eor #$ff                   ;get two's compliment of moving speed
    clc
    adc #$01
    sta MoveSpeed              ;store as new moving speed
    iny                        ;increment moving direction to left
SetLMov:
  sty MoveDirection      ;store moving direction
  
MoveObjectHorizontally:
  lda XSpeed                  ;get currently saved value (horizontal
  asl                         ;speed, secondary counter, whatever)
  asl                         ;and move low nybble to high
  asl
  asl
  sta R1                     ;store result here
  lda XSpeed                 ;get saved value again
  lsr                        ;move high nybble to low
  lsr
  lsr
  lsr
  cmp #$08                    ;if < 8, branch, do not change
  bcc SaveXSpd
    ora #%11110000              ;otherwise alter high nybble
SaveXSpd:
  sta R0                     ;save result here
  ldy #$00                    ;load default Y value here
  cmp #$00                    ;if result positive, leave Y alone
  bpl UseAdder
    dey                         ;otherwise decrement Y
UseAdder:
  sty R2                     ;save Y here
  lda XMoveForce ;get whatever number's here
  clc
  adc R1                     ;add low nybble moved to high
  sta XMoveForce ;store result here
  lda #$00                    ;init A
  rol                         ;rotate carry into d0
  pha                         ;push onto stack
    ror                         ;rotate d0 back onto carry
    lda XPos
    adc R0                     ;add carry plus saved value (high nybble moved to low
    sta XPos  ;plus R0 if necessary) to object's horizontal position
    lda XPage
    adc R2                     ;add carry plus other saved value to the
    sta XPage     ;object's page location and save
  pla
  clc                         ;pull old carry from stack and add
  adc R0                     ;to high nybble moved to low
ExXMove:
  rts                         ;and leave

PlayerLakituDiff:
  ldy #$00                   ;set Y for default value         
  lda XPos,x                ;get distance between enemy object's
  sec                     ;horizontal coordinate and the player's
  sbc Player_X_Position   ;horizontal coordinate
  sta R0                 ;and store here
  lda XPage,x
  sbc Player_PageLoc      ;subtract borrow, then leave
  bpl ChkLakDif              ;branch if enemy is to the right of the player
    iny                        ;increment Y for left of player
    lda R0
    eor #$ff                   ;get two's compliment of low byte of horizontal difference
    clc
    adc #$01                   ;store two's compliment as horizontal difference
    sta R0
ChkLakDif:
  lda R0                    ;get low byte of horizontal difference
  cmp #$3c                   ;if within a certain distance of player, branch
  bcc ChkPSpeed
    lda #$3c                   ;otherwise set maximum distance
    sta R0
    tya                        ;compare contents of Y, now in A
    cmp MoveDirection,x  ;to what is being used as horizontal movement direction
    beq ChkPSpeed              ;if moving toward the player, branch, do not alter
      lda MoveDirection,x  ;if moving to the left beyond maximum distance,
      beq SetLMovD               ;branch and alter without delay
        dec MoveSpeed,x      ;decrement horizontal speed
        lda MoveSpeed,x      ;if horizontal speed not yet at zero, branch to leave
        bne ExMoveLak
SetLMovD:
      tya                        ;set horizontal direction depending on horizontal
      sta MoveDirection,x  ;difference between enemy and player if necessary
ChkPSpeed:
      lda R0
      and #%00111100             ;mask out all but four bits in the middle
      lsr                        ;divide masked difference by four
      lsr
      sta R0                    ;store as new value
      ldy #$00                   ;init offset
      lda Player_X_Speed
      beq SubDifAdj              ;if player not moving horizontally, branch
      lda ScrollAmount
      beq SubDifAdj              ;if scroll speed not set, branch to same place
      iny                        ;otherwise increment offset
      lda Player_X_Speed
      cmp #$19                   ;if player not running, branch
      bcc ChkSpinyO
      lda ScrollAmount
      cmp #$02                   ;if scroll speed below a certain amount, branch
      bcc ChkSpinyO              ;to same place
      iny                        ;otherwise increment once more
ChkSpinyO:
      lda ID,x             ;check for spiny object
      cmp #Spiny
      bne ChkEmySpd              ;branch if not found
      lda Player_X_Speed         ;if player not moving, skip this part
      bne SubDifAdj
ChkEmySpd:
      lda YSpeed,x        ;check vertical speed
      bne SubDifAdj              ;branch if nonzero
      ldy #$00                   ;otherwise reinit offset
SubDifAdj:
        lda R1,y                ;get one of three saved values from earlier
        ldy R0                    ;get saved horizontal difference
:
          sec                        ;subtract one for each pixel of horizontal difference
          sbc #$01                   ;from one of three saved values
          dey
          bpl :-              ;branch until all pixels are subtracted, to adjust difference
ExMoveLak:
  rts                        ;leave!!!
