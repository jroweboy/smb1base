.include "common.inc"
.include "metasprite.inc"

.import DrawFireball, FireballBGCollision

; collision.s
.import HandleEnemyFBallCol

.segment "OBJECT"

;-------------------------------------------------------------------------------------
;$00 - used to store downward movement force in FireballObjCore
;$02 - used to store maximum vertical speed in FireballObjCore
;$07 - used to store pseudorandom bit in BubbleCheck

.proc ProcFireball_Bubble
.export ProcFireball_Bubble
  lda PlayerStatus           ;check player's status
  cmp #$02
  bcc ProcAirBubbles         ;if not fiery, branch
  lda A_B_Buttons
  and #B_Button              ;check for b button pressed
  beq ProcFireballs          ;branch if not pressed
  and PreviousA_B_Buttons
  bne ProcFireballs          ;if button pressed in previous frame, branch
  lda FireballCounter        ;load fireball counter
  and #%00000001             ;get LSB and use as offset for buffer
  tax
  lda Fireball_State,x       ;load fireball state
  bne ProcFireballs          ;if not inactive, branch
  ldy Player_Y_HighPos       ;if player too high or too low, branch
  dey
  bne ProcFireballs
  lda CrouchingFlag          ;if player crouching, branch
  bne ProcFireballs
  lda Player_State           ;if player's state = climbing, branch
  cmp #$03
  beq ProcFireballs
  lda #Sfx_Fireball          ;play fireball sound effect
  sta Square1SoundQueue
  lda #$02                   ;load state
  sta Fireball_State,x
  ldy PlayerAnimTimerSet     ;copy animation frame timer setting
  sty FireballThrowingTimer  ;into fireball throwing timer
  dey
  sty PlayerAnimTimer        ;decrement and store in player's animation timer
  inc FireballCounter        ;increment fireball counter

ProcFireballs:
  ldx #$00
  jsr FireballObjCore  ;process first fireball object
  ldx #$01
  jsr FireballObjCore  ;process second fireball object, then do air bubbles

ProcAirBubbles:
  ; lda AreaType                ;if not water type level, skip the rest of this
  ; bne BublExit
  lda SwimmingFlag
  beq NotSwimming
    ldx #$02                    ;otherwise load counter and use as offset
  AirBubbleLoop:
      stx ObjectOffset            ;store offset
      jsr BubbleCheck             ;check timers and coordinates, create air bubble
      ldy Player_Y_HighPos        ;if player's vertical high position
      dey                         ;not within screen, skip all of this
      bne SkipBubble
        ldy #METASPRITE_MISC_BUBBLE
        lda Bubble_Y_Position,x 
        cmp #$f0
        bcc :+
          ; Clear the metasprite if the bubble is offscreen
          ldy #0
        :
        tya
        sta BubbleMetasprite,x
    SkipBubble:
      dex
      bpl AirBubbleLoop                ;do this until all three are handled
  ; Fall through and check if the fireball needs to be cleared because we took damage
  ; rts
NotSwimming:
  lda PlayerStatus           ;check player's status
  cmp #$02
  bcs Exit
    ; Not fiery state anymore, so kill fireball
    lda #0
    sta FireballMetasprite
    sta FireballMetasprite+1
Exit:
  rts                         ;then leave
.endproc

FireballExplosion:
  jsr RelativeFireballPosition
  jmp DrawExplosion_Fireball

.proc FireballObjCore
  stx ObjectOffset             ;store offset as current object
  lda Fireball_State,x         ;check for d7 = 1
  asl
  bcs FireballExplosion        ;if so, branch to get relative coordinates and draw explosion
  ldy Fireball_State,x         ;if fireball inactive, branch to leave
  beq NoFBall
  dey                          ;if fireball state set to 1, skip this part and just run it
  beq RunFB
  lda Player_X_Position        ;get player's horizontal position
  adc #$04                     ;add four pixels and store as fireball's horizontal position
  sta Fireball_X_Position,x
  lda Player_PageLoc           ;get player's page location
  adc #$00                     ;add carry and store as fireball's page location
  sta Fireball_PageLoc,x
  lda Player_Y_Position        ;get player's vertical position and store
  sta Fireball_Y_Position,x
  lda #$01                     ;set high byte of vertical position
  sta Fireball_Y_HighPos,x
  ldy PlayerFacingDir          ;get player's facing direction
  dey                          ;decrement to use as offset here
  lda FireballXSpdData,y       ;set horizontal speed of fireball accordingly
  sta Fireball_X_Speed,x
  lda #$04                     ;set vertical speed of fireball
  sta Fireball_Y_Speed,x
  lda #$07
  sta Fireball_BoundBoxCtrl,x  ;set bounding box size control for fireball
  dec Fireball_State,x         ;decrement state to 1 to skip this part from now on
RunFB:
  txa                          ;add 7 to offset to use
  clc                          ;as fireball offset for next routines
  adc #$07
  tax
  lda #$50                     ;set downward movement force here
  sta R0
  lda #$03                     ;set maximum speed here
  sta R2
  lda #$00
  jsr ImposeGravity            ;do sub here to impose gravity on fireball and move vertically
  jsr MoveObjectHorizontally   ;do another sub to move it horizontally
  ldx ObjectOffset             ;return fireball offset to X
  jsr RelativeFireballPosition ;get relative coordinates
  jsr GetFireballOffscreenBits ;get offscreen information
  jsr GetFireballBoundBox      ;get bounding box coordinates
  jsr FireballBGCollision      ;do fireball to background collision detection
  lda FBall_OffscreenBits      ;get fireball offscreen bits
  and #%11001100               ;mask out certain bits
  bne EraseFB                  ;if any bits still set, branch to kill fireball
  jsr FireballEnemyCollision   ;do fireball to enemy collision detection and deal with collisions
  jmp DrawFireball             ;draw fireball appropriately and leave
EraseFB:
  lda #$00                     ;erase fireball state
  sta Fireball_State,x
  sta FireballMetasprite,x
NoFBall:
  rts                          ;leave

FireballXSpdData:
	.byte $40, $c0
.endproc

BubbleCheck:
  lda PseudoRandomBitReg+1,x  ;get part of LSFR
  and #$01
  sta R7                     ;store pseudorandom bit here
  lda Bubble_Y_Position,x     ;get vertical coordinate for air bubble
  cmp #$f8                    ;if offscreen coordinate not set,
  bne MoveBubl                ;branch to move air bubble
    lda AirBubbleTimer          ;if air bubble timer not expired,
    bne ExitBubl                ;branch to leave, otherwise create new air bubble
SetupBubble:
.export SetupBubble
  ldy #$00                 ;load default value here
  lda PlayerFacingDir      ;get player's facing direction
  lsr                      ;move d0 to carry
  bcc :+              ;branch to use default value if facing left
    ldy #$08                 ;otherwise load alternate value here
: tya                      ;use value loaded as adder
  adc Player_X_Position    ;add to player's horizontal position
  sta Bubble_X_Position,x  ;save as horizontal position for airbubble
  lda Player_PageLoc
  adc #$00                 ;add carry to player's page location
  sta Bubble_PageLoc,x     ;save as page location for airbubble
  lda Player_Y_Position
  clc                      ;add eight pixels to player's vertical position
  adc #$08
  sta Bubble_Y_Position,x  ;save as vertical position for air bubble
  lda #$01
  sta Bubble_Y_HighPos,x   ;set vertical high byte for air bubble
  ldy R7                  ;get pseudorandom bit, use as offset
  lda BubbleTimerData,y    ;get data for air bubble timer
  sta AirBubbleTimer       ;set air bubble timer
MoveBubl:
  ldy R7                  ;get pseudorandom bit again, use as offset
  lda Bubble_YMoveForceFractional,x
  sec                      ;subtract pseudorandom amount from dummy variable
  sbc Bubble_MForceData,y
  sta Bubble_YMoveForceFractional,x   ;save dummy variable
  lda Bubble_Y_Position,x
  sbc #$00                 ;subtract borrow from airbubble's vertical coordinate
  cmp #$20                 ;if below the status bar,
  bcs :+               ;branch to go ahead and use to move air bubble upwards
    lda #$f8                 ;otherwise set offscreen coordinate
: sta Bubble_Y_Position,x  ;store as new vertical coordinate for air bubble
ExitBubl: rts                      ;leave

Bubble_MForceData:
  .byte $ff, $50

BubbleTimerData:
  .byte $40, $20

;------------------------sw-------------------------------------------------------------

.proc GetFireballOffscreenBits
  ldy #$00                 ;set for fireball offsets
  jsr GetProperObjOffset   ;modify X to get proper fireball offset
  ldy #$02                 ;set other offset for fireball's offscreen bits
  jmp GetOffScreenBitsSet  ;and get offscreen information about fireball
.endproc


;-------------------------------------------------------------------------------------
.export ExplosionTiles
ExplosionTiles:
  .byte METASPRITE_EXPLOSION_FRAME_1
  .byte METASPRITE_EXPLOSION_FRAME_2
  .byte METASPRITE_EXPLOSION_FRAME_3

DrawExplosion_Fireball:
  ; ldy Alt_SprDataOffset,x  ;get OAM data offset of alternate sort for fireball's explosion
  lda Fireball_State,x     ;load fireball state
  inc Fireball_State,x     ;increment state for next frame
  lsr                      ;divide by 2
  and #%00000111           ;mask out all but d3-d1
  cmp #$03                 ;check to see if time to kill fireball
  bcs KillFireBall         ;branch if so, otherwise continue to draw explosion
  ;fallthrough
  tay                         ;use whatever's in A for offset
  lda ExplosionTiles,y        ;get tile number using offset
  sta FireballMetasprite,x
  ; prevent rotation of the fireball from bleeding into the explosion
  lda #0
  sta Fireball_SprAttrib,x
  rts                         ;we are done

KillFireBall:
  lda #$00                    ;clear fireball state to kill it
  sta Fireball_State,x
  sta FireballMetasprite,x
  rts


.segment "OBJECT"
;-------------------------------------------------------------------------------------
;$01 - enemy buffer offset

.proc FireballEnemyCollision
.import SprObjectCollisionCore
  lda Fireball_State,x  ;check to see if fireball state is set at all
  beq ExitFBallEnemy    ;branch to leave if not
  asl
  bcs ExitFBallEnemy    ;branch to leave also if d7 in state is set
  lda FrameCounter
  lsr                   ;get LSB of frame counter
  bcs ExitFBallEnemy    ;branch to leave if set (do routine every other frame)
  txa
  asl                   ;multiply fireball offset by four
  asl
  clc
  adc #$1c              ;then add $1c or 28 bytes to it
  tay                   ;to use fireball's bounding box coordinates

  ; Check the extra enemy slot as well since we let enemies spawn there now
  ldx #$05

FireballEnemyCDLoop:
  stx R1                     ;store enemy object offset here
  tya
  pha                         ;push fireball offset to the stack
    lda Enemy_State,x
    and #%00100000              ;check to see if d5 is set in enemy state
    bne NoFToECol               ;if so, skip to next enemy slot
    lda Enemy_Flag,x            ;check to see if buffer flag is set
    beq NoFToECol               ;if not, skip to next enemy slot
    lda Enemy_ID,x              ;check enemy identifier
    cmp #$24
    bcc GoombaDie               ;if < $24, branch to check further
    cmp #$2b
    bcc NoFToECol               ;if in range $24-$2a, skip to next enemy slot
GoombaDie:
  cmp #Goomba                 ;check for goomba identifier
  bne NotGoomba               ;if not found, continue with code
    lda Enemy_State,x           ;otherwise check for defeated state
    cmp #$02                    ;if stomped or otherwise defeated,
    bcs NoFToECol               ;skip to next enemy slot
NotGoomba:
  lda EnemyOffscrBitsMasked,x ;if any masked offscreen bits set,
  bne NoFToECol               ;skip to next enemy slot
    txa
    asl                         ;otherwise multiply enemy offset by four
    asl
    clc
    adc #$04                    ;add 4 bytes to it
    tax                         ;to use enemy's bounding box coordinates
    jsr SprObjectCollisionCore  ;do fireball-to-enemy collision detection
    ldx ObjectOffset            ;return fireball's original offset
    bcc NoFToECol               ;if carry clear, no collision, thus do next enemy slot
      lda #%10000000
      sta Fireball_State,x        ;set d7 in enemy state
      ldx R1                     ;get enemy offset
      jsr HandleEnemyFBallCol     ;jump to handle fireball to enemy collision
NoFToECol:
  pla                         ;pull fireball offset from stack
  tay                         ;put it in Y
  ldx R1                     ;get enemy object offset
  dex                         ;decrement it
  bpl FireballEnemyCDLoop     ;loop back until collision detection done on all enemies

ExitFBallEnemy:
  ldx ObjectOffset                 ;get original fireball offset and leave
  rts
.endproc