
.include "common.inc"
.include "object.inc"

HammerBroJumpLData:
      .byte $20, $37

HammerBroJumpCode:
       lda Enemy_State,x           ;get hammer bro's enemy state
       and #%00000111              ;mask out all but 3 LSB
       cmp #$01                    ;check for d0 set (for jumping)
       beq MoveHammerBroXDir       ;if set, branch ahead to moving code
       lda #$00                    ;load default value here
       sta $00                     ;save into temp variable for now
       ldy #$fa                    ;set default vertical speed
       lda Enemy_Y_Position,x      ;check hammer bro's vertical coordinate
       bmi SetHJ                   ;if on the bottom half of the screen, use current speed
       ldy #$fd                    ;otherwise set alternate vertical speed
       cmp #$70                    ;check to see if hammer bro is above the middle of screen
       inc $00                     ;increment preset value to $01
       bcc SetHJ                   ;if above the middle of the screen, use current speed and $01
       dec $00                     ;otherwise return value to $00
       lda PseudoRandomBitReg+1,x  ;get part of LSFR, mask out all but LSB
       and #$01
       bne SetHJ                   ;if d0 of LSFR set, branch and use current speed and $00
       ldy #$fa                    ;otherwise reset to default vertical speed
SetHJ: sty Enemy_Y_Speed,x         ;set vertical speed for jumping
       lda Enemy_State,x           ;set d0 in enemy state for jumping
       ora #$01
       sta Enemy_State,x
       lda $00                     ;load preset value here to use as bitmask
       and PseudoRandomBitReg+2,x  ;and do bit-wise comparison with part of LSFR
       tay                         ;then use as offset
       lda SecondaryHardMode       ;check secondary hard mode flag
       bne HJump
       tay                         ;if secondary hard mode flag clear, set offset to 0
HJump: lda HammerBroJumpLData,y    ;get jump length timer data using offset from before
       sta EnemyFrameTimer,x       ;save in enemy timer
       lda PseudoRandomBitReg+1,x
       ora #%11000000              ;get contents of part of LSFR, set d7 and d6, then
       sta HammerBroJumpTimer,x    ;store in jump timer

MoveHammerBroXDir:
         ldy #$fc                  ;move hammer bro a little to the left
         lda FrameCounter
         and #%01000000            ;change hammer bro's direction every 64 frames
         bne Shimmy
         ldy #$04                  ;if d6 set in counter, move him a little to the right
Shimmy:  sty Enemy_X_Speed,x       ;store horizontal speed
         ldy #$01                  ;set to face right by default
         jsr PlayerEnemyDiff       ;get horizontal difference between player and hammer bro
         bmi SetShim               ;if enemy to the left of player, skip this part
         iny                       ;set to face left
         lda EnemyIntervalTimer,x  ;check walking timer
         bne SetShim               ;if not yet expired, skip to set moving direction
         lda #$f8
         sta Enemy_X_Speed,x       ;otherwise, make the hammer bro walk left towards player
SetShim: sty Enemy_MovingDir,x     ;set moving direction

MoveNormalEnemy:
       ldy #$00                   ;init Y to leave horizontal movement as-is 
       lda Enemy_State,x
       and #%01000000             ;check enemy state for d6 set, if set skip
       bne FallE                  ;to move enemy vertically, then horizontally if necessary
       lda Enemy_State,x
       asl                        ;check enemy state for d7 set
       bcs SteadM                 ;if set, branch to move enemy horizontally
       lda Enemy_State,x
       and #%00100000             ;check enemy state for d5 set
       bne MoveDefeatedEnemy      ;if set, branch to move defeated enemy object
       lda Enemy_State,x
       and #%00000111             ;check d2-d0 of enemy state for any set bits
       beq SteadM                 ;if enemy in normal state, branch to move enemy horizontally
       cmp #$05
       beq FallE                  ;if enemy in state used by spiny's egg, go ahead here
       cmp #$03
       bcs ReviveStunned          ;if enemy in states $03 or $04, skip ahead to yet another part
FallE: jsr MoveD_EnemyVertically  ;do a sub here to move enemy downwards
       ldy #$00
       lda Enemy_State,x          ;check for enemy state $02
       cmp #$02
       beq MEHor                  ;if found, branch to move enemy horizontally
       and #%01000000             ;check for d6 set
       beq SteadM                 ;if not set, branch to something else
       lda Enemy_ID,x
       cmp #PowerUpObject         ;check for power-up object
       beq SteadM
       bne SlowM                  ;if any other object where d6 set, jump to set Y
MEHor: jmp MoveEnemyHorizontally  ;jump here to move enemy horizontally for <> $2e and d6 set

SlowM:  ldy #$01                  ;if branched here, increment Y to slow horizontal movement
SteadM: lda Enemy_X_Speed,x       ;get current horizontal speed
        pha                       ;save to stack
        bpl AddHS                 ;if not moving or moving right, skip, leave Y alone
        iny
        iny                       ;otherwise increment Y to next data
AddHS:  clc
        adc XSpeedAdderData,y     ;add value here to slow enemy down if necessary
        sta Enemy_X_Speed,x       ;save as horizontal speed temporarily
        jsr MoveEnemyHorizontally ;then do a sub to move horizontally
        pla
        sta Enemy_X_Speed,x       ;get old horizontal speed from stack and return to
        rts                       ;original memory location, then leave
