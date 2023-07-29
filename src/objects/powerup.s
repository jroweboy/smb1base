
.include "common.inc"
.include "object.inc"

; collision.s
.import EnemyJump, EnemyToBGCollisionDet, PlayerEnemyCollision
; screen_render.s
.import DrawPowerUp

.segment "OBJECT"

;-------------------------------------------------------------------------------------

PowerUpObjHandler:
  ; ldx #$05                   ;set object offset for last slot in enemy object buffer
  ; stx ObjectOffset
  lda Enemy_State,x          ;check power-up object's state
  beq ExitPUp                ;if not set, branch to leave
  asl                        ;shift to check if d7 was set in object state
  bcc GrowThePowerUp         ;if not set, branch ahead to skip this part
  lda TimerControl           ;if master timer control set,
  bne RunPUSubs              ;branch ahead to enemy object routines
  lda Enemy_PowerupType,x    ;check power-up type
  ; cmp #PowerMushroom
  beq ShroomM                ;if normal mushroom, branch ahead to move it
;  cmp #$03
  cmp #OneupMushroom
  beq ShroomM                ;if 1-up mushroom, branch ahead to move it
;  cmp #$02
  cmp #PowerStar
  bne RunPUSubs              ;if not star, branch elsewhere to skip movement
    jsr MoveJumpingEnemy       ;otherwise impose gravity on star power-up and make it jump
    jsr EnemyJump              ;note that green paratroopa shares the same code here 
    jmp RunPUSubs              ;then jump to other power-up subroutines
ShroomM:
  jsr MoveNormalEnemy        ;do sub to make mushrooms move
  jsr EnemyToBGCollisionDet  ;deal with collisions
  jmp RunPUSubs              ;run the other subroutines

GrowThePowerUp:
  lda FrameCounter           ;get frame counter
  and #$03                   ;mask out all but 2 LSB
  bne ChkPUSte               ;if any bits set here, branch
  dec Enemy_Y_Position,x     ;otherwise decrement vertical coordinate slowly
  lda Enemy_State,x          ;load power-up object state
  inc Enemy_State,x          ;increment state for next frame (to make power-up rise)
  cmp #$11                   ;if power-up object state not yet past 16th pixel,
  bcc ChkPUSte               ;branch ahead to last part here
  lda #$10
  sta Enemy_X_Speed,x        ;otherwise set horizontal speed
  lda #%10000000
  sta Enemy_State,x          ;and then set d7 in power-up object's state
  asl                        ;shift once to init A
  sta Enemy_SprAttrib,x      ;initialize background priority bit set here
  rol                        ;rotate A to set right moving direction
  sta Enemy_MovingDir,x      ;set moving direction
ChkPUSte:
  lda Enemy_State,x          ;check power-up object's state
  cmp #$06                   ;for if power-up has risen enough
  bcc ExitPUp                ;if not, don't even bother running these routines
RunPUSubs:
  jsr RelativeEnemyPosition  ;get coordinates relative to screen
  jsr GetEnemyOffscreenBits  ;get offscreen bits
  jsr GetEnemyBoundBox       ;get bounding box coordinates
  jsr DrawPowerUp            ;draw the power-up object
  jsr PlayerEnemyCollision   ;check for collision with player
  jmp OffscreenBoundsCheck   ;check to see if it went offscreen
ExitPUp:
  rts ; TODO check this RTS can be removed                        ;and we're done
