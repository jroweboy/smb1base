.include "common.inc"
.macpack longbranch

; objects/hammer_bros.s
.import SetHJ

; objects/object.s
.import EraseEnemyObject

; screen_render.s
.import GiveOneCoin, RemoveCoin_Axe, HandlePipeEntry, DrawPowerUp
.import DestroyBlockMetatile, GetPlayerColors

; tiles/brick.s
.import BlockBumpedChk, InitBlock_XY_Pos, BrickShatter, BumpBlock

.export PlayerBGCollision, FireballBGCollision, PlayerEnemyCollision
.export EnemyToBGCollisionDet, SprObjectCollisionCore, HandleEnemyFBallCol
.export EnemyJump, SetupFloateyNumber, EnemiesCollision, InjurePlayer

; platform.s
.export SmallPlatformCollision, CheckPlayerVertical
.export PlayerCollisionCore, ProcLPlatCollisions, GetEnemyBoundBoxOfsArg

; vine.s
.export BlockBufferCollision

; gamecore.s
.export ForceInjury

.export BBChk_E

.segment "COLLISION"

;-------------------------------------------------------------------------------------
;$06 - second object's offset
;$07 - counter

PlayerCollisionCore:
      ldx #$00     ;initialize X to use player's bounding box for comparison

SprObjectCollisionCore:
      sty R6      ;save contents of Y here
      lda #$01
      sta R7      ;save value 1 here as counter, compare horizontal coordinates first

CollisionCoreLoop:
      lda BoundingBox_UL_Corner,y  ;compare left/top coordinates
      cmp BoundingBox_UL_Corner,x  ;of first and second objects' bounding boxes
      bcs FirstBoxGreater          ;if first left/top => second, branch
      cmp BoundingBox_LR_Corner,x  ;otherwise compare to right/bottom of second
      bcc SecondBoxVerticalChk     ;if first left/top < second right/bottom, branch elsewhere
      beq CollisionFound           ;if somehow equal, collision, thus branch
      lda BoundingBox_LR_Corner,y  ;if somehow greater, check to see if bottom of
      cmp BoundingBox_UL_Corner,y  ;first object's bounding box is greater than its top
      bcc CollisionFound           ;if somehow less, vertical wrap collision, thus branch
      cmp BoundingBox_UL_Corner,x  ;otherwise compare bottom of first bounding box to the top
      bcs CollisionFound           ;of second box, and if equal or greater, collision, thus branch
      ldy R6                      ;otherwise return with carry clear and Y = $0006
      rts                          ;note horizontal wrapping never occurs

SecondBoxVerticalChk:
      lda BoundingBox_LR_Corner,x  ;check to see if the vertical bottom of the box
      cmp BoundingBox_UL_Corner,x  ;is greater than the vertical top
      bcc CollisionFound           ;if somehow less, vertical wrap collision, thus branch
      lda BoundingBox_LR_Corner,y  ;otherwise compare horizontal right or vertical bottom
      cmp BoundingBox_UL_Corner,x  ;of first box with horizontal left or vertical top of second box
      bcs CollisionFound           ;if equal or greater, collision, thus branch
      ldy R6                      ;otherwise return with carry clear and Y = $0006
      rts

FirstBoxGreater:
  cmp BoundingBox_UL_Corner,x  ;compare first and second box horizontal left/vertical top again
  beq CollisionFound           ;if first coordinate = second, collision, thus branch
  cmp BoundingBox_LR_Corner,x  ;if not, compare with second object right or bottom edge
  bcc CollisionFound           ;if left/top of first less than or equal to right/bottom of second
  beq CollisionFound           ;then collision, thus branch
  cmp BoundingBox_LR_Corner,y  ;otherwise check to see if top of first box is greater than bottom
  bcc NoCollisionFound         ;if less than or equal, no collision, branch to end
  beq NoCollisionFound
  lda BoundingBox_LR_Corner,y  ;otherwise compare bottom of first to top of second
  cmp BoundingBox_UL_Corner,x  ;if bottom of first is greater than top of second, vertical wrap
  bcs CollisionFound           ;collision, and branch, otherwise, proceed onwards here

NoCollisionFound:
  clc          ;clear carry, then load value set earlier, then leave
  ldy R6      ;like previous ones, if horizontal coordinates do not collide, we do
  rts          ;not bother checking vertical ones, because what's the point?

CollisionFound:
  inx                    ;increment offsets on both objects to check
  iny                    ;the vertical coordinates
  dec R7                ;decrement counter to reflect this
  bpl CollisionCoreLoop  ;if counter not expired, branch to loop
  sec                    ;otherwise we already did both sets, therefore collision, so set carry
  ldy R6                ;load original value set here earlier, then leave
  rts

;-------------------------------------------------------------------------------------
;$02 - modified y coordinate
;$03 - stores metatile involved in block buffer collisions
;$04 - comes in with offset to block buffer adder data, goes out with low nybble x/y coordinate
;$05 - modified x coordinate
;$06-$07 - block buffer address

BlockBufferChk_FBall:
  ldy #$1a                  ;set offset for block buffer adder data
  txa
  clc
  adc #$07                  ;add seven bytes to use
  tax
  lda #$00                  ;set A to return vertical coordinate
BBChk_E:
  jsr BlockBufferCollision  ;do collision detection subroutine for sprite object
  ldx ObjectOffset          ;get object offset
  cmp #$00                  ;check to see if object bumped into anything
  rts

BlockBufferAdderData:
  .byte (BlockBuffer_Swimming_X_Adder - BlockBuffer_X_Adder)
  .byte (BlockBuffer_Big_X_Adder      - BlockBuffer_X_Adder)
  .byte (BlockBuffer_Small_X_Adder    - BlockBuffer_X_Adder)

; misc objects use hardcoded offsets
MISC_BLOCK_BUFFER_START = $16

BlockBuffer_X_Adder:
; Added to the sprite position to get the location to check for tile collision
;     head, foot l, r, side 1 2    3,   4
BlockBuffer_Swimming_X_Adder:
  .byte $08, $03, $0c, $02, $02, $0d, $0d ; swimming
BlockBuffer_Big_X_Adder:
  .byte $08, $03, $0c, $02, $02, $0d, $0d ; big
BlockBuffer_Small_X_Adder:
  .byte $08, $03, $0c, $02, $02, $0d, $0d ; small
BlockBuffer_Misc_X_Adder:
  .byte $08, $00, $10, $04, $14, $04, $04 ; misc

BlockBuffer_Y_Adder:
  .byte $04, $20, $20, $08, $18, $08, $18 ; swimming
  .byte $02, $20, $20, $08, $18, $08, $18 ; big
  .byte $12, $20, $20, $18, $18, $18, $18 ; small
  .byte $18, $14, $14, $06, $06, $08, $10 ; misc

BlockBufferColli_Feet:
  iny            ;if branched here, increment to next set of adders
BlockBufferColli_Head:
  lda #$00       ;set flag to return vertical coordinate
  beq BlockBufferPlayerCollision ; Unconditional
BlockBufferColli_Side:
  lda #$01       ;set flag to return horizontal coordinate
BlockBufferPlayerCollision:
  ldx #$00       ;set offset for player object
BlockBufferCollision:
  pha                         ;save contents of A to stack
    sty R4                     ;save contents of Y here
    lda BlockBuffer_X_Adder,y   ;add horizontal coordinate
    clc                         ;of object to value obtained using Y as offset
    adc SprObject_X_Position,x
    sta R5                     ;store here
    lda SprObject_PageLoc,x
    adc #$00                    ;add carry to page location
    and #$01                    ;get LSB, mask out all other bits
    lsr                         ;move to carry
    ora R5                     ;get stored value
    ror                         ;rotate carry to MSB of A
    lsr                         ;and effectively move high nybble to
    lsr                         ;lower, LSB which became MSB will be
    lsr                         ;d4 at this point
    jsr GetBlockBufferAddr      ;get address of block buffer into R6, R7
    ldy R4                     ;get old contents of Y
    lda SprObject_Y_Position,x  ;get vertical coordinate of object
    clc
    adc BlockBuffer_Y_Adder,y   ;add it to value obtained using Y as offset
    and #%11110000              ;mask out low nybble
    sec
    sbc #$20                    ;subtract 32 pixels for the status bar
    bcc HeadWrapped
    sta R2                     ;store result here
    tay                         ;use as offset for block buffer
    lda ($06),y                 ;check current content of block buffer
    jmp NoHeadWrap
HeadWrapped:
    lda #0
NoHeadWrap:
    sta R3                     ;and store here
    ldy R4                     ;get old contents of Y again
  pla                         ;pull A from stack
  bne RetXC                   ;if A = 1, branch
  lda SprObject_Y_Position,x  ;if A = 0, load vertical coordinate
  jmp RetYC                   ;and jump
RetXC:
  lda SprObject_X_Position,x  ;otherwise load horizontal coordinate
RetYC:
  and #%00001111              ;and mask out high nybble
  sta R4                     ;store masked out result here
  lda R3                     ;get saved content of block buffer
  rts                         ;and leave

;-------------------------------------------------------------------------------------
;$06-$07 - used to store block buffer address used as indirect

BlockBufferAddr:
.lobytes Block_Buffer_1, Block_Buffer_2
.hibytes Block_Buffer_1, Block_Buffer_2

.export GetBlockBufferAddr
GetBlockBufferAddr:
  pha                      ;take value of A, save
    lsr                      ;move high nybble to low
    lsr
    lsr
    lsr
    tay                      ;use nybble as pointer to high byte
    lda BlockBufferAddr+2,y  ;of indirect here
    sta R7
  pla
  and #%00001111           ;pull from stack, mask out high nybble
  clc
  adc BlockBufferAddr,y    ;add to low byte
  sta R6                  ;store here and leave
  rts


;-------------------------------------------------------------------------------------

FireballBGCollision:
  lda Fireball_Y_Position,x   ;check fireball's vertical coordinate
  cmp #$18
  bcc ClearBounceFlag         ;if within the status bar area of the screen, branch ahead
  jsr BlockBufferChk_FBall    ;do fireball to background collision detection on bottom of it
  beq ClearBounceFlag         ;if nothing underneath fireball, branch
  jsr ChkForNonSolids         ;check for non-solid metatiles
  beq ClearBounceFlag         ;branch if any found
  lda Fireball_Y_Speed,x      ;if fireball's vertical speed set to move upwards,
  bmi InitFireballExplode     ;branch to set exploding bit in fireball's state
  lda FireballBouncingFlag,x  ;if bouncing flag already set,
  bne InitFireballExplode     ;branch to set exploding bit in fireball's state
  ; attempt to bounce the fireball back to the original height
  lda InitialFireballYSpeed,x
  bmi :+
    eor #$ff
    clc
    adc #1
:
  cmp #$fd
  bcc :+
    lda #$fd
:
  sta Fireball_Y_Speed,x      ;otherwise set vertical speed to move upwards (give it bounce)
  lda #$01
  sta FireballBouncingFlag,x  ;set bouncing flag
  lda Fireball_Y_Position,x
  and #$f8                    ;modify vertical coordinate to land it properly
  sta Fireball_Y_Position,x   ;store as new vertical coordinate
  rts                         ;leave

ClearBounceFlag:
  lda #$00
  sta FireballBouncingFlag,x  ;clear bouncing flag by default
  rts                         ;leave

InitFireballExplode:
  lda #$80
  sta Fireball_State,x        ;set exploding flag in fireball's state
  lda #Sfx_Bump
  sta Square1SoundQueue       ;load bump sound
  rts                         ;leave

ChkForNonSolids:
  cmp #$26       ;blank metatile used for vines?
  beq NSFnd
  cmp #$c2       ;regular coin?
  beq NSFnd
  cmp #$c3       ;underwater coin?
  beq NSFnd
  cmp #$5f       ;hidden coin block?
  beq NSFnd
  cmp #$60       ;hidden 1-up block?
NSFnd:
  rts

;--------------------------------

KickedShellXSpdData:
  .byte $30, $d0

DemotedKoopaXSpdData:
  .byte $08, $f8

PlayerEnemyCollision:
  lda FrameCounter            ;check counter for d0 set
  lsr
  bcs NoPECol                   ;if set, branch to leave
  jsr CheckPlayerVertical     ;if player object is completely offscreen or
  bcs NoPECol                 ;if down past 224th pixel row, branch to leave
  lda EnemyOffscrBitsMasked,x ;if current enemy is offscreen by any amount,
  bne NoPECol                 ;go ahead and branch to leave
  lda GameEngineSubroutine
  cmp #$08                    ;if not set to run player control routine
  bne NoPECol                 ;on next frame, branch to leave
  lda Enemy_State,x
  and #%00100000              ;if enemy state has d5 set, branch to leave
  bne NoPECol
  jsr GetEnemyBoundBoxOfs     ;get bounding box offset for current enemy object
  jsr PlayerCollisionCore     ;do collision detection on player vs. enemy
  ldx ObjectOffset            ;get enemy object buffer offset
  bcs CheckForPUpCollision    ;if collision, branch past this part here
  lda Enemy_CollisionBits,x
  and #%11111110              ;otherwise, clear d0 of current enemy object's
  sta Enemy_CollisionBits,x   ;collision bit
NoPECol:
  rts

CheckForPUpCollision:
       ldy Enemy_ID,x
       cpy #PowerUpObject            ;check for power-up object
       bne EColl                     ;if not found, branch to next part
       jmp HandlePowerUpCollision    ;otherwise, unconditional jump backwards
EColl: lda StarInvincibleTimer       ;if star mario invincibility timer expired,
       beq HandlePECollisions        ;perform task here, otherwise kill enemy like
       jmp ShellOrBlockDefeat        ;hit with a shell, or from beneath

KickedShellPtsData:
      .byte $0a, $06, $04

HandlePECollisions:
       lda Enemy_CollisionBits,x    ;check enemy collision bits for d0 set
       and #%00000001               ;or for being offscreen at all
       ora EnemyOffscrBitsMasked,x
       bne ExPEC                    ;branch to leave if either is true
       lda #$01
       ora Enemy_CollisionBits,x    ;otherwise set d0 now
       sta Enemy_CollisionBits,x
       cpy #Spiny                   ;branch if spiny
       beq ChkForPlayerInjury
       cpy #PiranhaPlant            ;branch if piranha plant
       beq InjurePlayer
       cpy #Podoboo                 ;branch if podoboo
       beq InjurePlayer
       cpy #BulletBill_CannonVar    ;branch if bullet bill
       beq ChkForPlayerInjury
       cpy #$15                     ;branch if object => $15
       bcs InjurePlayer
       lda AreaType                 ;branch if water type level
       beq InjurePlayer
       lda Enemy_State,x            ;branch if d7 of enemy state was set
       asl
       bcs ChkForPlayerInjury
       lda Enemy_State,x            ;mask out all but 3 LSB of enemy state
       and #%00000111
       cmp #$02                     ;branch if enemy is in normal or falling state
       bcc ChkForPlayerInjury
       lda Enemy_ID,x               ;branch to leave if goomba in defeated state
       cmp #Goomba
       beq ExPEC
       lda #Sfx_EnemySmack          ;play smack enemy sound
       sta Square1SoundQueue
       lda Enemy_State,x            ;set d7 in enemy state, thus become moving shell
       ora #%10000000
       sta Enemy_State,x
       jsr EnemyFacePlayer          ;set moving direction and get offset
       lda KickedShellXSpdData,y    ;load and set horizontal speed data with offset
       sta Enemy_X_Speed,x
       lda #$03                     ;add three to whatever the stomp counter contains
       clc                          ;to give points for kicking the shell
       adc StompChainCounter
       ldy EnemyIntervalTimer,x     ;check shell enemy's timer
       cpy #$03                     ;if above a certain point, branch using the points
       bcs KSPts                    ;data obtained from the stomp counter + 3
       lda KickedShellPtsData,y     ;otherwise, set points based on proximity to timer expiration
KSPts: jmp SetupFloateyNumber       ;set values for floatey number now
ExPEC: rts ; TODO check this RTS can be removed                          ;leave!!!

ChkForPlayerInjury:
          lda Player_Y_Speed     ;check player's vertical speed
          bmi ChkInj             ;perform procedure below if player moving upwards
          bne EnemyStomped       ;or not at all, and branch elsewhere if moving downwards
ChkInj:   lda Enemy_ID,x         ;branch if enemy object < $07
          cmp #Bloober
          bcc ChkETmrs
          lda Player_Y_Position  ;add 12 pixels to player's vertical position
          clc
          adc #$0c
          cmp Enemy_Y_Position,x ;compare modified player's position to enemy's position
          bcc EnemyStomped       ;branch if this player's position above (less than) enemy's
ChkETmrs: lda StompTimer         ;check stomp timer
          bne EnemyStomped       ;branch if set
          lda InjuryTimer        ;check to see if injured invincibility timer still
          bne ExInjColRoutines   ;counting down, and branch elsewhere to leave if so
          lda Player_Rel_XPos
          cmp Enemy_Rel_XPos     ;if player's relative position to the left of enemy's
          bcc TInjE              ;relative position, branch here
          jmp ChkEnemyFaceRight  ;otherwise do a jump here
TInjE:    lda Enemy_MovingDir,x  ;if enemy moving towards the left,
          cmp #$01               ;branch, otherwise do a jump here
          bne InjurePlayer       ;to turn the enemy around
          jmp LInj

InjurePlayer:
      lda InjuryTimer          ;check again to see if injured invincibility timer is
      bne ExInjColRoutines     ;at zero, and branch to leave if so

ForceInjury:
          ldx PlayerStatus          ;check player's status
          beq KillPlayer            ;branch if small
          sta PlayerStatus          ;otherwise set player's status to small
          lda #$08
          sta InjuryTimer           ;set injured invincibility timer
          asl
          sta Square1SoundQueue     ;play pipedown/injury sound
          jsr GetPlayerColors       ;change player's palette if necessary
          lda #$0a                  ;set subroutine to run on next frame
SetKRout: ldy #$01                  ;set new player state
SetPRout: sta GameEngineSubroutine  ;load new value to run subroutine on next frame
          sty Player_State          ;store new player state
          ldy #$ff
          sty TimerControl          ;set master timer control flag to halt timers
          iny
          sty ScrollAmount          ;initialize scroll speed

ExInjColRoutines:
      ldx ObjectOffset              ;get enemy offset and leave
      rts

KillPlayer:
      stx Player_X_Speed   ;halt player's horizontal movement by initializing speed
      inx
      stx EventMusicQueue  ;set event music queue to death music
      lda #$fc
      sta Player_Y_Speed   ;set new vertical speed
      lda #$0b             ;set subroutine to run on next frame
      bne SetKRout         ;branch to set player's state and other things

StompedEnemyPtsData:
      .byte $02, $06, $05, $06

EnemyStomped:
      lda Enemy_ID,x             ;check for spiny, branch to hurt player
      cmp #Spiny                 ;if found
      beq InjurePlayer
      lda #Sfx_EnemyStomp        ;otherwise play stomp/swim sound
      sta Square1SoundQueue
      lda Enemy_ID,x
      ldy #$00                   ;initialize points data offset for stomped enemies
      cmp #FlyingCheepCheep      ;branch for cheep-cheep
      beq EnemyStompedPts
      cmp #BulletBill_FrenzyVar  ;branch for either bullet bill object
      beq EnemyStompedPts
      cmp #BulletBill_CannonVar
      beq EnemyStompedPts
      cmp #Podoboo               ;branch for podoboo (this branch is logically impossible
      beq EnemyStompedPts        ;for cpu to take due to earlier checking of podoboo)
      iny                        ;increment points data offset
      cmp #HammerBro             ;branch for hammer bro
      beq EnemyStompedPts
      iny                        ;increment points data offset
      cmp #Lakitu                ;branch for lakitu
      beq EnemyStompedPts
      iny                        ;increment points data offset
      cmp #Bloober               ;branch if NOT bloober
      bne ChkForDemoteKoopa

EnemyStompedPts:
  lda StompedEnemyPtsData,y  ;load points data using offset in Y
  jsr SetupFloateyNumber     ;run sub to set floatey number controls
  lda Enemy_MovingDir,x
  pha                        ;save enemy movement direction to stack
    jsr SetStun                ;run sub to kill enemy
  pla
  sta Enemy_MovingDir,x      ;return enemy movement direction from stack
  lda #%00100000
  sta Enemy_State,x          ;set d5 in enemy state
  jsr InitVStf               ;nullify vertical speed, physics-related thing,
  sta Enemy_X_Speed,x        ;and horizontal speed
  lda #$fd                   ;set player's vertical speed, to give bounce
  sta Player_Y_Speed
  ; reset the air timer so we don't bounce too high when hitting the ground
  ; after bouncing on an enemy.
  ; sta AirTimeTimer
  rts

ChkForDemoteKoopa:
      cmp #$09                   ;branch elsewhere if enemy object < $09
      bcc HandleStompedShellE
      and #%00000001             ;demote koopa paratroopas to ordinary troopas
      sta Enemy_ID,x
      ldy #$00                   ;return enemy to normal state
      sty Enemy_State,x
      lda #$03                   ;award 400 points to the player
      jsr SetupFloateyNumber
      jsr InitVStf               ;nullify physics-related thing and vertical speed
      jsr EnemyFacePlayer        ;turn enemy around if necessary
      lda DemotedKoopaXSpdData,y
      sta Enemy_X_Speed,x        ;set appropriate moving speed based on direction
      jmp SBnce                  ;then move onto something else

RevivalRateData:
      .byte $10, $0b

HandleStompedShellE:
       lda #$04                   ;set defeated state for enemy
       sta Enemy_State,x
       inc StompChainCounter      ;increment the stomp counter
       lda StompChainCounter      ;add whatever is in the stomp counter
       clc                        ;to whatever is in the stomp timer
       adc StompTimer
       jsr SetupFloateyNumber     ;award points accordingly
       inc StompTimer             ;increment stomp timer of some sort
       ldy PrimaryHardMode        ;check primary hard mode flag
       lda RevivalRateData,y      ;load timer setting according to flag
       sta EnemyIntervalTimer,x   ;set as enemy timer to revive stomped enemy
SBnce: lda #$fc                   ;set player's vertical speed for bounce
       sta Player_Y_Speed         ;and then leave!!!
       rts

ChkEnemyFaceRight:
       lda Enemy_MovingDir,x ;check to see if enemy is moving to the right
       cmp #$01
       bne LInj              ;if not, branch
       jmp InjurePlayer      ;otherwise go back to hurt player
LInj:  jsr EnemyTurnAround   ;turn the enemy around, if necessary
       jmp InjurePlayer      ;go back to hurt player


EnemyFacePlayer:
       ldy #$01               ;set to move right by default
       jsr PlayerEnemyDiff    ;get horizontal difference between player and enemy
       bpl SFcRt              ;if enemy is to the right of player, do not increment
       iny                    ;otherwise, increment to set to move to the left
SFcRt: sty Enemy_MovingDir,x  ;set moving direction here
       dey                    ;then decrement to use as a proper offset
       rts

SetupFloateyNumber:
       sta FloateyNum_Control,x ;set number of points control for floatey numbers
       lda #$30
       sta FloateyNum_Timer,x   ;set timer for floatey numbers
       lda Enemy_Y_Position,x
       sta FloateyNum_Y_Pos,x   ;set vertical coordinate
       lda Enemy_Rel_XPos
       sta FloateyNum_X_Pos,x   ;set horizontal coordinate and leave
ExSFN: rts


;-------------------------------------------------------------------------------------
;$01 - used to hold enemy offset for second enemy

SetBitsMask:
      .byte %10000000, %01000000, %00100000, %00010000, %00001000, %00000100, %00000010

ClearBitsMask:
      .byte %01111111, %10111111, %11011111, %11101111, %11110111, %11111011, %11111101

EnemiesCollision:
        lda FrameCounter            ;check counter for d0 set
        lsr
        bcc ExSFN                   ;if d0 not set, leave
        lda AreaType
        beq ExSFN                   ;if water area type, leave
        lda Enemy_ID,x
        cmp #$15                    ;if enemy object => $15, branch to leave
        bcs ExitECRoutine
        cmp #Lakitu                 ;if lakitu, branch to leave
        beq ExitECRoutine
        cmp #PiranhaPlant           ;if piranha plant, branch to leave
        beq ExitECRoutine
        lda EnemyOffscrBitsMasked,x ;if masked offscreen bits nonzero, branch to leave
        bne ExitECRoutine
        jsr GetEnemyBoundBoxOfs     ;otherwise, do sub, get appropriate bounding box offset for
        dex                         ;first enemy we're going to compare, then decrement for second
        bmi ExitECRoutine           ;branch to leave if there are no other enemies
ECLoop: stx R1                     ;save enemy object buffer offset for second enemy here
        tya                         ;save first enemy's bounding box offset to stack
        pha
        lda Enemy_Flag,x            ;check enemy object enable flag
        beq ReadyNextEnemy          ;branch if flag not set
        lda Enemy_ID,x
        cmp #$15                    ;check for enemy object => $15
        bcs ReadyNextEnemy          ;branch if true
        cmp #Lakitu
        beq ReadyNextEnemy          ;branch if enemy object is lakitu
        cmp #PiranhaPlant
        beq ReadyNextEnemy          ;branch if enemy object is piranha plant
        lda EnemyOffscrBitsMasked,x
        bne ReadyNextEnemy          ;branch if masked offscreen bits set
        txa                         ;get second enemy object's bounding box offset
        asl                         ;multiply by four, then add four
        asl
        clc
        adc #$04
        tax                         ;use as new contents of X
        jsr SprObjectCollisionCore  ;do collision detection using the two enemies here
        ldx ObjectOffset            ;use first enemy offset for X
        ldy R1                     ;use second enemy offset for Y
        bcc NoEnemyCollision        ;if carry clear, no collision, branch ahead of this
        lda Enemy_State,x
        ora Enemy_State,y           ;check both enemy states for d7 set
        and #%10000000
        bne YesEC                   ;branch if at least one of them is set
        lda Enemy_CollisionBits,y   ;load first enemy's collision-related bits
        and SetBitsMask,x           ;check to see if bit connected to second enemy is
        bne ReadyNextEnemy          ;already set, and move onto next enemy slot if set
        lda Enemy_CollisionBits,y
        ora SetBitsMask,x           ;if the bit is not set, set it now
        sta Enemy_CollisionBits,y
YesEC:  jsr ProcEnemyCollisions     ;react according to the nature of collision
        jmp ReadyNextEnemy          ;move onto next enemy slot

NoEnemyCollision:
      lda Enemy_CollisionBits,y     ;load first enemy's collision-related bits
      and ClearBitsMask,x           ;clear bit connected to second enemy
      sta Enemy_CollisionBits,y     ;then move onto next enemy slot

ReadyNextEnemy:
      pla              ;get first enemy's bounding box offset from the stack
      tay              ;use as Y again
      ldx R1          ;get and decrement second enemy's object buffer offset
      dex
      bpl ECLoop       ;loop until all enemy slots have been checked

ExitECRoutine:
      ldx ObjectOffset ;get enemy object buffer offset
      rts              ;leave

ProcEnemyCollisions:
      lda Enemy_State,y        ;check both enemy states for d5 set
      ora Enemy_State,x
      and #%00100000           ;if d5 is set in either state, or both, branch
      bne ExitProcessEColl     ;to leave and do nothing else at this point
      lda Enemy_State,x
      cmp #$06                 ;if second enemy state < $06, branch elsewhere
      bcc ProcSecondEnemyColl
      lda Enemy_ID,x           ;check second enemy identifier for hammer bro
      cmp #HammerBro           ;if hammer bro found in alt state, branch to leave
      beq ExitProcessEColl
      lda Enemy_State,y        ;check first enemy state for d7 set
      asl
      bcc ShellCollisions      ;branch if d7 is clear
      lda #$06
      jsr SetupFloateyNumber   ;award 1000 points for killing enemy
      jsr ShellOrBlockDefeat   ;then kill enemy, then load
      ldy R1                  ;original offset of second enemy

ShellCollisions:
      tya                      ;move Y to X
      tax
      jsr ShellOrBlockDefeat   ;kill second enemy
      ldx ObjectOffset
      lda ShellChainCounter,x  ;get chain counter for shell
      clc
      adc #$04                 ;add four to get appropriate point offset
      ldx R1
      jsr SetupFloateyNumber   ;award appropriate number of points for second enemy
      ldx ObjectOffset         ;load original offset of first enemy
      inc ShellChainCounter,x  ;increment chain counter for additional enemies

ExitProcessEColl:
      rts                      ;leave!!!

ProcSecondEnemyColl:
      lda Enemy_State,y        ;if first enemy state < $06, branch elsewhere
      cmp #$06
      bcc MoveEOfs
      lda Enemy_ID,y           ;check first enemy identifier for hammer bro
      cmp #HammerBro           ;if hammer bro found in alt state, branch to leave
      beq ExitProcessEColl
      jsr ShellOrBlockDefeat   ;otherwise, kill first enemy
      ldy R1
      lda ShellChainCounter,y  ;get chain counter for shell
      clc
      adc #$04                 ;add four to get appropriate point offset
      ldx ObjectOffset
      jsr SetupFloateyNumber   ;award appropriate number of points for first enemy
      ldx R1                  ;load original offset of second enemy
      inc ShellChainCounter,x  ;increment chain counter for additional enemies
      rts                      ;leave!!!

MoveEOfs:
      tya                      ;move Y ($01) to X
      tax
      jsr EnemyTurnAround      ;do the sub here using value from R1
      ldx ObjectOffset         ;then do it again using value from ObjectOffset

EnemyTurnAround:
       lda Enemy_ID,x           ;check for specific enemies
       cmp #PiranhaPlant
       beq ExTA                 ;if piranha plant, leave
       cmp #Lakitu
       beq ExTA                 ;if lakitu, leave
       cmp #HammerBro
       beq ExTA                 ;if hammer bro, leave
       cmp #Spiny
       beq RXSpd                ;if spiny, turn it around
       cmp #GreenParatroopaJump
       beq RXSpd                ;if green paratroopa, turn it around
       cmp #$07
       bcs ExTA                 ;if any OTHER enemy object => $07, leave
RXSpd: lda Enemy_X_Speed,x      ;load horizontal speed
       eor #$ff                 ;get two's compliment for horizontal speed
       tay
       iny
       sty Enemy_X_Speed,x      ;store as new horizontal speed
       lda Enemy_MovingDir,x
       eor #%00000011           ;invert moving direction and store, then leave
       sta Enemy_MovingDir,x    ;thus effectively turning the enemy around
ExTA:  rts                      ;leave!!!

;--------------------------------
;$00 - counter for bounding boxes

SmallPlatformCollision:
      lda TimerControl             ;if master timer control set,
      bne ExSPC                    ;branch to leave
      sta PlatformCollisionFlag,x  ;otherwise initialize collision flag
      jsr CheckPlayerVertical      ;do a sub to see if player is below a certain point
      bcs ExSPC                    ;or entirely offscreen, and branch to leave if true
      lda #$02
      sta R0                      ;load counter here for 2 bounding boxes

ChkSmallPlatLoop:
      ldx ObjectOffset           ;get enemy object offset
      jsr GetEnemyBoundBoxOfs    ;get bounding box offset in Y
      and #%00000010             ;if d1 of offscreen lower nybble bits was set
      bne ExSPC                  ;then branch to leave
      lda BoundingBox_UL_YPos,y  ;check top of platform's bounding box for being
      cmp #$20                   ;above a specific point
      bcc MoveBoundBox           ;if so, branch, don't do collision detection
      jsr PlayerCollisionCore    ;otherwise, perform player-to-platform collision detection
      bcs ProcSPlatCollisions    ;skip ahead if collision

MoveBoundBox:
       lda BoundingBox_UL_YPos,y  ;move bounding box vertical coordinates
       clc                        ;128 pixels downwards
       adc #$80
       sta BoundingBox_UL_YPos,y
       lda BoundingBox_DR_YPos,y
       clc
       adc #$80
       sta BoundingBox_DR_YPos,y
       dec R0                    ;decrement counter we set earlier
       bne ChkSmallPlatLoop       ;loop back until both bounding boxes are checked
ExSPC: ldx ObjectOffset           ;get enemy object buffer offset, then leave
       rts

;--------------------------------

ProcSPlatCollisions:
      ldx ObjectOffset             ;return enemy object buffer offset to X, then continue

ProcLPlatCollisions:
      lda BoundingBox_DR_YPos,y    ;get difference by subtracting the top
      sec                          ;of the player's bounding box from the bottom
      sbc BoundingBox_UL_YPos      ;of the platform's bounding box
      cmp #$04                     ;if difference too large or negative,
      bcs ChkForTopCollision       ;branch, do not alter vertical speed of player
      lda Player_Y_Speed           ;check to see if player's vertical speed is moving down
      bpl ChkForTopCollision       ;if so, don't mess with it
      lda #$01                     ;otherwise, set vertical
      sta Player_Y_Speed           ;speed of player to kill jump

ChkForTopCollision:
      lda BoundingBox_DR_YPos      ;get difference by subtracting the top
      sec                          ;of the platform's bounding box from the bottom
      sbc BoundingBox_UL_YPos,y    ;of the player's bounding box
      cmp #$06
      bcs PlatformSideCollisions   ;if difference not close enough, skip all of this
      lda Player_Y_Speed
      bmi PlatformSideCollisions   ;if player's vertical speed moving upwards, skip this
      lda R0                      ;get saved bounding box counter from earlier
      ldy Enemy_ID,x
      cpy #$2b                     ;if either of the two small platform objects are found,
      beq SetCollisionFlag         ;regardless of which one, branch to use bounding box counter
      cpy #$2c                     ;as contents of collision flag
      beq SetCollisionFlag
      txa                          ;otherwise use enemy object buffer offset

SetCollisionFlag:
      ldx ObjectOffset             ;get enemy object buffer offset
      sta PlatformCollisionFlag,x  ;save either bounding box counter or enemy offset here
      lda #$00
      sta Player_State             ;set player state to normal then leave
      rts

PlatformSideCollisions:
         lda #$01                   ;set value here to indicate possible horizontal
         sta R0                    ;collision on left side of platform
         lda BoundingBox_DR_XPos    ;get difference by subtracting platform's left edge
         sec                        ;from player's right edge
         sbc BoundingBox_UL_XPos,y
         cmp #$08                   ;if difference close enough, skip all of this
         bcc SideC
         inc R0                    ;otherwise increment value set here for right side collision
         lda BoundingBox_DR_XPos,y  ;get difference by subtracting player's left edge
         clc                        ;from platform's right edge
         sbc BoundingBox_UL_XPos
         cmp #$09                   ;if difference not close enough, skip subroutine
         bcs NoSideC                ;and instead branch to leave (no collision)
         ; keep the carry clear so StopPlayerMove goes straight to ImpedeMove
SideC:   jsr StopPlayerMove       ;deal with horizontal collision
NoSideC: ldx ObjectOffset           ;return with enemy object buffer offset
         rts

BowserIdentities:
      .byte Goomba, GreenKoopa, BuzzyBeetle, Spiny, Lakitu, Bloober, HammerBro, Bowser
HandleEnemyFBallCol:
      jsr RelativeEnemyPosition  ;get relative coordinate of enemy
      ldx R1                    ;get current enemy object offset
      lda Enemy_Flag,x           ;check buffer flag for d7 set
      bpl ChkBuzzyBeetle         ;branch if not set to continue
      and #%00001111             ;otherwise mask out high nybble and
      tax                        ;use low nybble as enemy offset
      lda Enemy_ID,x
      cmp #Bowser                ;check enemy identifier for bowser
      beq HurtBowser             ;branch if found
      ldx R1                    ;otherwise retrieve current enemy offset

ChkBuzzyBeetle:
      lda Enemy_ID,x
      cmp #BuzzyBeetle           ;check for buzzy beetle
      beq ExHCF                  ;branch if found to leave (buzzy beetles fireproof)
      cmp #Bowser                ;check for bowser one more time (necessary if d7 of flag was clear)
      bne ChkOtherEnemies        ;if not found, branch to check other enemies

HurtBowser:
          dec BowserHitPoints        ;decrement bowser's hit points
          bne ExHCF                  ;if bowser still has hit points, branch to leave
          jsr InitVStf               ;otherwise do sub to init vertical speed and movement force
          sta Enemy_X_Speed,x        ;initialize horizontal speed
          sta EnemyFrenzyBuffer      ;init enemy frenzy buffer
          lda #$fe
          sta Enemy_Y_Speed,x        ;set vertical speed to make defeated bowser jump a little
          ldy WorldNumber            ;use world number as offset
          lda BowserIdentities,y     ;get enemy identifier to replace bowser with
          sta Enemy_ID,x             ;set as new enemy identifier
          lda #$20                   ;set A to use starting value for state
          cpy #$03                   ;check to see if using offset of 3 or more
          bcs SetDBSte               ;branch if so
          ora #$03                   ;otherwise add 3 to enemy state
SetDBSte: sta Enemy_State,x          ;set defeated enemy state
          lda #Sfx_BowserFall
          sta Square2SoundQueue      ;load bowser defeat sound
          ldx R1                    ;get enemy offset
          lda #$09                   ;award 5000 points to player for defeating bowser
          bne EnemySmackScore        ;unconditional branch to award points

ChkOtherEnemies:
      cmp #BulletBill_FrenzyVar
      beq ExHCF                 ;branch to leave if bullet bill (frenzy variant) 
      cmp #Podoboo       
      beq ExHCF                 ;branch to leave if podoboo
      cmp #$15       
      bcs ExHCF                 ;branch to leave if identifier => $15

ShellOrBlockDefeat:
      lda Enemy_ID,x            ;check for piranha plant
      cmp #PiranhaPlant
      bne StnE                  ;branch if not found
      lda Enemy_Y_Position,x
      adc #$18                  ;add 24 pixels to enemy object's vertical position
      sta Enemy_Y_Position,x
StnE: jsr ChkToStunEnemies      ;do yet another sub
      lda Enemy_State,x
      and #%00011111            ;mask out 2 MSB of enemy object's state
      ora #%00100000            ;set d5 to defeat enemy and save as new state
      sta Enemy_State,x
      lda #$02                  ;award 200 points by default
      ldy Enemy_ID,x            ;check for hammer bro
      cpy #HammerBro
      bne GoombaPoints          ;branch if not found
      lda #$06                  ;award 1000 points for hammer bro

GoombaPoints:
      cpy #Goomba               ;check for goomba
      bne EnemySmackScore       ;branch if not found
      lda #$01                  ;award 100 points for goomba

EnemySmackScore:
       jsr SetupFloateyNumber   ;update necessary score variables
       lda #Sfx_EnemySmack      ;play smack enemy sound
       sta Square1SoundQueue
ExHCF: rts                      ;and now let's leave

;-------------------------------------------------------------------------------------

HandlePowerUpCollision:
      jsr EraseEnemyObject    ;erase the power-up object
      lda #$06
      jsr SetupFloateyNumber  ;award 1000 points to player by default
      lda #Sfx_PowerUpGrab
      sta Square2SoundQueue   ;play the power-up sound
      lda PowerUpType         ;check power-up type
      cmp #$02
      bcc Shroom_Flower_PUp   ;if mushroom or fire flower, branch
      cmp #$03
      beq SetFor1Up           ;if 1-up mushroom, branch
      lda #$23                ;otherwise set star mario invincibility
      sta StarInvincibleTimer ;timer, and load the star mario music
      lda #StarPowerMusic     ;into the area music queue, then leave
      sta AreaMusicQueue
NearbyRTS:
      rts

Shroom_Flower_PUp:
      lda PlayerStatus    ;if player status = small, branch
      beq UpToSuper
      cmp #$01            ;if player status not super, leave
      bne NearbyRTS
      ldx ObjectOffset    ;get enemy offset, not necessary
      lda #$02            ;set player status to fiery
      sta PlayerStatus
      jsr GetPlayerColors ;run sub to change colors of player
      ldx ObjectOffset    ;get enemy offset again, and again not necessary
      lda #$0c            ;set value to be used by subroutine tree (fiery)
      jmp UpToFiery       ;jump to set values accordingly

SetFor1Up:
      lda #$0b                 ;change 1000 points into 1-up instead
      sta FloateyNum_Control,x ;and then leave
      rts

UpToSuper:
       lda #$01         ;set player status to super
       sta PlayerStatus
      ;  lda #$09         ;set value to be used by subroutine tree (super)
      lda #$0c            ;set value to be used by subroutine tree (fiery)

UpToFiery:
       ldy #$00         ;set value to be used as new player state
       jmp SetPRout     ;set values to stop certain things in motion

;-------------------------------------------------------------------------------------

CheckPlayerVertical:
       lda Player_OffscreenBits  ;if player object is completely offscreen
       cmp #$f0                  ;vertically, leave this routine
       bcs ExCPV
       ldy Player_Y_HighPos      ;if player high vertical byte is not
       dey                       ;within the screen, leave this routine
       bne ExCPV
       lda Player_Y_Position     ;if on the screen, check to see how far down
       cmp #$d0                  ;the player is vertically
ExCPV: rts

;-------------------------------------------------------------------------------------

GetEnemyBoundBoxOfs:
      lda ObjectOffset         ;get enemy object buffer offset

GetEnemyBoundBoxOfsArg:
      asl                      ;multiply A by four, then add four
      asl                      ;to skip player's bounding box
      clc
      adc #$04
      tay                      ;send to Y
      lda Enemy_OffscreenBits  ;get offscreen bits for enemy object
      and #%00001111           ;save low nybble
      cmp #%00001111           ;check for all bits set
      rts

;-------------------------------------------------------------------------------------
;$00-$01 - used to hold many values, essentially temp variables
;$04 - holds lower nybble of vertical coordinate from block buffer routine
;$eb - used to hold block buffer adder

PlayerBGUpperExtent:
  .byte $20, $10

PlayerBGCollision:
  lda DisableCollisionDet   ;if collision detection disabled flag set,
  bne ExPBGCol              ;branch to leave
  lda GameEngineSubroutine
  cmp #$0b                  ;if running routine #11 or $0b
  beq ExPBGCol              ;branch to leave
  cmp #$04
  bcc ExPBGCol              ;if running routines $00-$03 branch to leave
  lda #$01                  ;load default player state for swimming
  ldy SwimmingFlag          ;if swimming flag set,
  bne SetPSte               ;branch ahead to set default state
  lda Player_State          ;if player in normal state,
  beq SetFallS              ;branch to set default state for falling
  cmp #$03
  bne ChkOnScr              ;if in any other state besides climbing, skip to next part
SetFallS:
  lda #$02                  ;load default player state for falling
SetPSte:
  sta Player_State          ;set whatever player state is appropriate
ChkOnScr:
  lda Player_Y_HighPos
  cmp #$01                  ;check player's vertical high byte for still on the screen
  bne ExPBGCol              ;branch to leave if not
  lda #$ff
  sta Player_CollisionBits  ;initialize player's collision flag
  lda Player_Y_Position
  cmp #$cf                  ;check player's vertical coordinate
  bcc ChkCollSize           ;if not too close to the bottom of screen, continue
ExPBGCol:
  rts                       ;otherwise leave

ChkCollSize:
  ldy #$02                    ;load default offset
  lda CrouchingFlag
  bne GBBAdr                  ;if player crouching, skip ahead
    lda PlayerSize
    bne GBBAdr                  ;if player small, skip ahead
      dey                         ;otherwise decrement offset for big player not crouching
      lda SwimmingFlag
      bne GBBAdr                  ;if swimming flag set, skip ahead
        dey                         ;otherwise decrement offset
GBBAdr:
  lda BlockBufferAdderData,y  ;get value using offset
  sta Local_eb                     ;store value here
  tay                         ;put value into Y, as offset for block buffer routine
  ldx PlayerSize              ;get player's size as offset
  lda CrouchingFlag
  beq HeadChk                 ;if player not crouching, branch ahead
    inx                         ;otherwise increment size as offset
HeadChk:
  lda Player_Y_Position       ;get player's vertical coordinate
  cmp PlayerBGUpperExtent,x   ;compare with upper extent value based on offset
  bcc DoFootCheck             ;if player is too high, skip this part
    jsr BlockBufferColli_Head   ;do player-to-bg collision detection on top of
    beq DoFootCheck             ;player, and branch if nothing above player's head
      jsr CheckForCoinMTiles      ;check to see if player touched coin with their head
      bcs AwardTouchedCoin        ;if so, branch to some other part of code
        ldy Player_Y_Speed          ;check player's vertical speed
        bpl DoFootCheck             ;if player not moving upwards, branch elsewhere
        ldy R4                     ;check lower nybble of vertical coordinate returned
        cpy #$04                    ;from collision detection routine
        bcc DoFootCheck             ;if low nybble < 4, branch
          jsr CheckForSolidMTiles     ;check to see what player's head bumped on
          bcs SolidOrClimb            ;if player collided with solid metatile, branch
          ldy AreaType                ;otherwise check area type
          beq NYSpd                   ;if water level, branch ahead
          ldy BlockBounceTimer        ;if block bounce timer not expired,
          bne NYSpd                   ;branch ahead, do not process collision
          jsr PlayerHeadCollision     ;otherwise do a sub to process collision
          jmp DoFootCheck             ;jump ahead to skip these other parts

SolidOrClimb:
  cmp #$26               ;if climbing metatile,
  beq NYSpd              ;branch ahead and do not play sound
    lda #Sfx_Bump
    sta Square1SoundQueue  ;otherwise load bump sound
NYSpd:
  lda #$01               ;set player's vertical speed to nullify
  sta Player_Y_Speed     ;jump or swim

DoFootCheck:
  ldy Local_eb                    ;get block buffer adder offset
  lda Player_Y_Position
  cmp #$cf                   ;check to see how low player is
  jcs DoPlayerSideCheck      ;if player is too far down on screen, skip all of this
    jsr BlockBufferColli_Feet  ;do player-to-bg collision detection on bottom left of player
    ; jsr QuickBrickShatterWhenBig
    jsr CheckForCoinMTiles     ;check to see if player touched coin with their left foot
    bcs AwardTouchedCoin       ;if so, branch to some other part of code
      pha                        ;save bottom left metatile to stack
        jsr BlockBufferColli_Feet  ;do player-to-bg collision detection on bottom right of player
        ; jsr QuickBrickShatterWhenBig
        sta R0                    ;save bottom right metatile here
      pla
      sta R1                    ;pull bottom left metatile and save here
      bne ChkFootMTile           ;if anything here, skip this part
        lda R0                    ;otherwise check for anything in bottom right metatile
        jeq DoPlayerSideCheck      ;and skip ahead if not
        jsr CheckForCoinMTiles     ;check to see if player touched coin with their right foot
        bcc ChkFootMTile           ;if not, skip unconditional jump and continue code
AwardTouchedCoin:
  jmp HandleCoinMetatile     ;follow the code to erase coin and award to player 1 coin
  ;implicit rts

ChkFootMTile:
  jsr CheckForClimbMTiles    ;check to see if player landed on climbable metatiles
  bcs DoPlayerSideCheck      ;if so, branch
    ldy Player_Y_Speed         ;check player's vertical speed
    bmi DoPlayerSideCheck      ;if player moving upwards, branch
    cmp #$c5
    bne ContChk                ;if player did not touch axe, skip ahead
      jmp HandleAxeMetatile      ;otherwise jump to set modes of operation
ContChk:
  jsr ChkInvisibleMTiles     ;do sub to check for hidden coin or 1-up blocks
  beq DoPlayerSideCheck      ;if either found, branch
    ldy JumpspringAnimCtrl     ;if jumpspring animating right now,
    bne InitSteP               ;branch ahead
      ldy R4                    ;check lower nybble of vertical coordinate returned
      cpy #$05                   ;from collision detection routine
      bcc LandPlyr               ;if lower nybble < 5, branch
        ; pha ; save the current metatile for checking if we are breaking a brick
          lda Player_MovingDir
          sta R0                    ;use player's moving direction as temp variable
        ; pla
        ; also keep the carry set to signify that we want to check for bricks
        jmp StopPlayerMove       ;jump to impede player's movement in that direction
LandPlyr:
  jsr ChkForLandJumpSpring   ;do sub to check for jumpspring metatiles and deal with it
  lda #$f0
  and Player_Y_Position      ;mask out lower nybble of player's vertical position
  sta Player_Y_Position      ;and store as new vertical position to land player properly
  jsr HandlePipeEntry        ;do sub to process potential pipe entry
  lda #$00
  sta Player_Y_Speed         ;initialize vertical speed and fractional
  sta Player_Y_MoveForce     ;movement force to stop player's vertical movement
  sta StompChainCounter      ;initialize enemy stomp counter
InitSteP:
  lda #$00
  sta Player_State           ;set player's state to normal
  
  ; fallthrough

DoPlayerSideCheck:
  ldy Local_eb       ;get block buffer adder offset
  iny
  iny           ;increment offset 2 bytes to use adders for side collisions
  lda #2
  sta R0
SideCheckLoop:
  iny                       ;move onto the next one
  sty Local_eb                   ;store it
  lda Player_Y_Position
  cmp #$20                  ;check player's vertical position
  bcc BHalf                 ;if player is in status bar area, branch ahead to skip this part
  cmp #$e4
  bcs ExSCH                 ;branch to leave if player is too far down
  jsr BlockBufferColli_Side ;do player-to-bg collision detection on one half of player
  beq BHalf                 ;branch ahead if nothing found
  cmp #$1c                  ;otherwise check for pipe metatiles
  beq BHalf                 ;if collided with sideways pipe (top), branch ahead
  cmp #$6b
  beq BHalf                 ;if collided with water pipe (top), branch ahead
    jsr CheckForClimbMTiles   ;do sub to see if player bumped into anything climbable
    bcc CheckSideMTiles       ;if not, branch to alternate section of code
BHalf:
  ldy Local_eb                   ;load block adder offset
  iny                       ;increment it
  lda Player_Y_Position     ;get player's vertical position
  cmp #$08
  bcc ExSCH                 ;if too high, branch to leave
  cmp #$d0
  bcs ExSCH                 ;if too low, branch to leave
    jsr BlockBufferColli_Side ;do player-to-bg collision detection on other half of player
    bne CheckSideMTiles       ;if something found, branch
      dec R0                   ;otherwise decrement counter
      bne SideCheckLoop         ;run code until both sides of player are checked
ExSCH:
  rts                       ;leave

CheckSideMTiles:
  ; jsr QuickBrickShatterWhenBig
  jsr ChkInvisibleMTiles     ;check for hidden or coin 1-up blocks
  beq ExSCH                  ;branch to leave if either found
    jsr CheckForClimbMTiles    ;check for climbable metatiles
    bcc ContSChk               ;if not found, skip and continue with code
      jmp HandleClimbing         ;otherwise jump to handle climbing
ContSChk:
  jsr CheckForCoinMTiles     ;check to see if player touched coin
  bcs HandleCoinMetatile     ;if so, execute code to erase coin and award to player 1 coin
    jsr ChkJumpspringMetatiles ;check for jumpspring metatiles
    bcc ChkPBtm                ;if not found, branch ahead to continue cude
      lda JumpspringAnimCtrl     ;otherwise check jumpspring animation control
      bne ExSCH                  ;branch to leave if set
        jmp StopPlayerMove         ;otherwise jump to impede player's movement
ChkPBtm:
  ldy Player_State           ;get player's state
  cpy #$00                   ;check for player's state set to normal
  bne StopPlayerMove         ;if not, branch to impede player's movement
    ldy PlayerFacingDir        ;get player's facing direction
    dey
    bne StopPlayerMove         ;if facing left, branch to impede movement
    cmp #$6c                   ;otherwise check for pipe metatiles
    beq PipeDwnS               ;if collided with sideways pipe (bottom), branch
    cmp #$1f                   ;if collided with water pipe (bottom), continue
    bne StopPlayerMove         ;otherwise branch to impede player's movement
PipeDwnS:
  lda Player_SprAttrib       ;check player's attributes
  bne PlyrPipe               ;if already set, branch, do not play sound again
    ldy #Sfx_PipeDown_Injury
    sty Square1SoundQueue      ;otherwise load pipedown/injury sound
PlyrPipe:
  ora #%00100000
  sta Player_SprAttrib       ;set background priority bit in player attributes
  lda Player_X_Position
  and #%00001111             ;get lower nybble of player's horizontal coordinate
  beq ChkGERtn               ;if at zero, branch ahead to skip this part
    ldy #$00                   ;set default offset for timer setting data
    lda ScreenLeft_PageLoc     ;load page location for left side of screen
    beq SetCATmr               ;if at page zero, use default offset
      iny                        ;otherwise increment offset
SetCATmr:
  lda AreaChangeTimerData,y  ;set timer for change of area as appropriate
  sta ChangeAreaTimer
ChkGERtn:
  lda GameEngineSubroutine   ;get number of game engine routine running
  cmp #$07
  beq ExCSM                  ;if running player entrance routine or
  cmp #$08                   ;player control routine, go ahead and branch to leave
  bne ExCSM
  lda #$02
  sta GameEngineSubroutine   ;otherwise set sideways pipe entry routine to run
ExCSM:
  rts                        ;and leave

; BounceForceData:
;   .byte $06, $06, $05, $05
;   .byte $05, $05, $04, $04
;   .byte $04, $03, $03, $03
;   .byte $02, $02, $01, $00

;--------------------------------
;$02 - high nybble of vertical coordinate from block buffer
;$04 - low nybble of horizontal coordinate from block buffer
;$06-$07 - block buffer address

StopPlayerMove:
; if the carry is set then we know this object isn't a brick
;   bcc @Exit
; ; brick metatile is 51 and 52
; ; so if we bounce into a brick when we are big then we don't want to
; ; impede the player movement
;   ldx PlayerSize
;   bne @Exit
;   cmp #$54 ; check if we are touching the ground
;   beq @Exit
;   cmp #$50 ; this range is the power up and brick range
;   bcc @Exit
;   cmp #$61
;   bcs @Exit
;     rts
; @Exit:
  jmp ImpedePlayerMove      ;stop player's movement
      
AreaChangeTimerData:
  .byte $a0, $34

HandleCoinMetatile:
  jsr ErACM             ;do sub to erase coin metatile from block buffer
  inc CoinTallyFor1Ups  ;increment coin tally used for 1-up blocks
  jmp GiveOneCoin       ;update coin amount and tally on the screen

HandleAxeMetatile:
  lda #$00
  sta OperMode_Task   ;reset secondary mode
  lda #$02
  sta OperMode        ;set primary mode to autoctrl mode
  lda #$18
  sta Player_X_Speed  ;set horizontal speed and continue to erase axe metatile
ErACM:
  ldy R2             ;load vertical high nybble offset for block buffer
  lda #$00            ;load blank metatile
  sta ($06),y         ;store to remove old contents from block buffer
  jmp RemoveCoin_Axe  ;update the screen accordingly


; ScaledRotationSpeed:
;   .incbin "rotation_speed.bin"

;--------------------------------

SolidMTileUpperExt:
  .byte $10, $61, $88, $c4

.export CheckForSolidMTiles
CheckForSolidMTiles:
  jsr GetMTileAttrib        ;find appropriate offset based on metatile's 2 MSB
  cmp SolidMTileUpperExt,x  ;compare current metatile with solid metatiles
  rts

ClimbMTileUpperExt:
  .byte $24, $6d, $8b, $c6

CheckForClimbMTiles:
  jsr GetMTileAttrib        ;find appropriate offset based on metatile's 2 MSB
  cmp ClimbMTileUpperExt,x  ;compare current metatile with climbable metatiles
  rts

CheckForCoinMTiles:
  cmp #$c2              ;check for regular coin
  beq CoinSd            ;branch if found
  cmp #$c3              ;check for underwater coin
  beq CoinSd            ;branch if found
  clc                   ;otherwise clear carry and leave
  rts
CoinSd:
  lda #Sfx_CoinGrab
  sta Square2SoundQueue ;load coin grab sound and leave
  rts

GetMTileAttrib:
  tay            ;save metatile value into Y
  and #%11000000 ;mask out all but 2 MSB
  asl
  rol            ;shift and rotate d7-d6 to d1-d0
  rol
  tax            ;use as offset for metatile data
  tya            ;get original metatile value back
ExEBG:
  rts            ;leave


;-------------------------------------------------------------------------------------
;$06-$07 - address from block buffer routine

EnemyBGCStateData:
      .byte $01, $01, $02, $02, $02, $05

EnemyBGCXSpdData:
      .byte $10, $f0

EnemyToBGCollisionDet:
      lda Enemy_State,x        ;check enemy state for d6 set
      and #%00100000
      bne ExEBG                ;if set, branch to leave
      jsr SubtEnemyYPos        ;otherwise, do a subroutine here
      bcc ExEBG                ;if enemy vertical coord + 62 < 68, branch to leave
      ldy Enemy_ID,x
      cpy #Spiny               ;if enemy object is not spiny, branch elsewhere
      bne DoIDCheckBGColl
      lda Enemy_Y_Position,x
      cmp #$25                 ;if enemy vertical coordinate < 36 branch to leave
      bcc ExEBG

DoIDCheckBGColl:
       cpy #GreenParatroopaJump ;check for some other enemy object
       bne HBChk                ;branch if not found
       jmp EnemyJump            ;otherwise jump elsewhere
HBChk: cpy #HammerBro           ;check for hammer bro
       bne CInvu                ;branch if not found
       jmp HammerBroBGColl      ;otherwise jump elsewhere
CInvu: cpy #Spiny               ;if enemy object is spiny, branch
       beq YesIn
       cpy #PowerUpObject       ;if special power-up object, branch
       beq YesIn
       cpy #$07                 ;if enemy object =>$07, branch to leave
       bcs ExEBGChk
YesIn: jsr ChkUnderEnemy        ;if enemy object < $07, or = $12 or $2e, do this sub
       bne HandleEToBGCollision ;if block underneath enemy, branch

NoEToBGCollision:
       jmp ChkForRedKoopa       ;otherwise skip and do something else

;--------------------------------
;$02 - vertical coordinate from block buffer routine

HandleEToBGCollision:
      jsr ChkForNonSolids       ;if something is underneath enemy, find out what
      beq NoEToBGCollision      ;if blank $26, coins, or hidden blocks, jump, enemy falls through
      cmp #$23
      bne LandEnemyProperly     ;check for blank metatile $23 and branch if not found
      ldy R2                   ;get vertical coordinate used to find block
      lda #$00                  ;store default blank metatile in that spot so we won't
      sta ($06),y               ;trigger this routine accidentally again
      lda Enemy_ID,x
      cmp #$15                  ;if enemy object => $15, branch ahead
      bcs ChkToStunEnemies
      cmp #Goomba               ;if enemy object not goomba, branch ahead of this routine
      bne GiveOEPoints
      jsr KillEnemyAboveBlock   ;if enemy object IS goomba, do this sub

GiveOEPoints:
      lda #$01                  ;award 100 points for hitting block beneath enemy
      jsr SetupFloateyNumber

ChkToStunEnemies:
          cmp #$09                   ;perform many comparisons on enemy object identifier
          bcc SetStun      
          cmp #$11                   ;if the enemy object identifier is equal to the values
          bcs SetStun                ;$09, $0e, $0f or $10, it will be modified, and not
          cmp #$0a                   ;modified if not any of those values, note that piranha plant will
          bcc Demote                 ;always fail this test because A will still have vertical
          cmp #PiranhaPlant          ;coordinate from previous addition, also these comparisons
          bcc SetStun                ;are only necessary if branching from $d7a1
Demote:   and #%00000001             ;erase all but LSB, essentially turning enemy object
          sta Enemy_ID,x             ;into green or red koopa troopa to demote them
SetStun:  lda Enemy_State,x          ;load enemy state
          and #%11110000             ;save high nybble
          ora #%00000010
          sta Enemy_State,x          ;set d1 of enemy state
          dec Enemy_Y_Position,x
          dec Enemy_Y_Position,x     ;subtract two pixels from enemy's vertical position
          lda Enemy_ID,x
          cmp #Bloober               ;check for bloober object
          beq SetWYSpd
          lda #$fd                   ;set default vertical speed
          ldy AreaType
          bne SetNotW                ;if area type not water, set as speed, otherwise
SetWYSpd: lda #$ff                   ;change the vertical speed
SetNotW:  sta Enemy_Y_Speed,x        ;set vertical speed now
          ldy #$01
          jsr PlayerEnemyDiff        ;get horizontal difference between player and enemy object
          bpl ChkBBill               ;branch if enemy is to the right of player
          iny                        ;increment Y if not
ChkBBill: lda Enemy_ID,x      
          cmp #BulletBill_CannonVar  ;check for bullet bill (cannon variant)
          beq NoCDirF
          cmp #BulletBill_FrenzyVar  ;check for bullet bill (frenzy variant)
          beq NoCDirF                ;branch if either found, direction does not change
          sty Enemy_MovingDir,x      ;store as moving direction
NoCDirF:  dey                        ;decrement and use as offset
          lda EnemyBGCXSpdData,y     ;get proper horizontal speed
          sta Enemy_X_Speed,x        ;and store, then leave
ExEBGChk: rts

;--------------------------------
;$04 - low nybble of vertical coordinate from block buffer routine

LandEnemyProperly:
       lda R4                 ;check lower nybble of vertical coordinate saved earlier
       sec
       sbc #$08                ;subtract eight pixels
       cmp #$05                ;used to determine whether enemy landed from falling
       bcs ChkForRedKoopa      ;branch if lower nybble in range of $0d-$0f before subtract
       lda Enemy_State,x      
       and #%01000000          ;branch if d6 in enemy state is set
       bne LandEnemyInitState
       lda Enemy_State,x
       asl                     ;branch if d7 in enemy state is not set
       bcc ChkLandedEnemyState
SChkA: jmp DoEnemySideCheck    ;if lower nybble < $0d, d7 set but d6 not set, jump here

ChkLandedEnemyState:
           lda Enemy_State,x         ;if enemy in normal state, branch back to jump here
           beq SChkA
           cmp #$05                  ;if in state used by spiny's egg
           beq ProcEnemyDirection    ;then branch elsewhere
           cmp #$03                  ;if already in state used by koopas and buzzy beetles
           bcs ExSteChk              ;or in higher numbered state, branch to leave
           lda Enemy_State,x         ;load enemy state again (why?)
           cmp #$02                  ;if not in $02 state (used by koopas and buzzy beetles)
           bne ProcEnemyDirection    ;then branch elsewhere
           lda #$10                  ;load default timer here
           ldy Enemy_ID,x            ;check enemy identifier for spiny
           cpy #Spiny
           bne SetForStn             ;branch if not found
           lda #$00                  ;set timer for $00 if spiny
SetForStn: sta EnemyIntervalTimer,x  ;set timer here
           lda #$03                  ;set state here, apparently used to render
           sta Enemy_State,x         ;upside-down koopas and buzzy beetles
           jmp EnemyLanding          ;then land it properly
ExSteChk:  rts ; TODO check this RTS can be removed                       ;then leave

ProcEnemyDirection:
         lda Enemy_ID,x            ;check enemy identifier for goomba
         cmp #Goomba               ;branch if found
         beq LandEnemyInitState
         cmp #Spiny                ;check for spiny
         bne InvtD                 ;branch if not found
         lda #$01
         sta Enemy_MovingDir,x     ;send enemy moving to the right by default
         lda #$08
         sta Enemy_X_Speed,x       ;set horizontal speed accordingly
         lda FrameCounter
         and #%00000111            ;if timed appropriately, spiny will skip over
         beq LandEnemyInitState    ;trying to face the player
InvtD:   ldy #$01                  ;load 1 for enemy to face the left (inverted here)
         jsr PlayerEnemyDiff       ;get horizontal difference between player and enemy
         bpl CNwCDir               ;if enemy to the right of player, branch
         iny                       ;if to the left, increment by one for enemy to face right (inverted)
CNwCDir: tya
         cmp Enemy_MovingDir,x     ;compare direction in A with current direction in memory
         bne LandEnemyInitState
         jsr ChkForBump_HammerBroJ ;if equal, not facing in correct dir, do sub to turn around

LandEnemyInitState:
      jsr EnemyLanding       ;land enemy properly
      lda Enemy_State,x
      and #%10000000         ;if d7 of enemy state is set, branch
      bne NMovShellFallBit
      lda #$00               ;otherwise initialize enemy state and leave
      sta Enemy_State,x      ;note this will also turn spiny's egg into spiny
      rts

NMovShellFallBit:
      lda Enemy_State,x   ;nullify d6 of enemy state, save other bits
      and #%10111111      ;and store, then leave
      sta Enemy_State,x
      rts

;--------------------------------

ChkForRedKoopa:
             lda Enemy_ID,x            ;check for red koopa troopa $03
             cmp #RedKoopa
             bne Chk2MSBSt             ;branch if not found
             lda Enemy_State,x
             beq ChkForBump_HammerBroJ ;if enemy found and in normal state, branch
Chk2MSBSt:   lda Enemy_State,x         ;save enemy state into Y
             tay
             asl                       ;check for d7 set
             bcc GetSteFromD           ;branch if not set
             lda Enemy_State,x
             ora #%01000000            ;set d6
             jmp SetD6Ste              ;jump ahead of this part
GetSteFromD: lda EnemyBGCStateData,y   ;load new enemy state with old as offset
SetD6Ste:    sta Enemy_State,x         ;set as new state

;--------------------------------
;$00 - used to store bitmask (not used but initialized here)
;$eb - used in DoEnemySideCheck as counter and to compare moving directions

DoEnemySideCheck:
          lda Enemy_Y_Position,x     ;if enemy within status bar, branch to leave
          cmp #$20                   ;because there's nothing there that impedes movement
          bcc ExESdeC
          ldy #$16                   ;start by finding block to the left of enemy ($00,$14)
          lda #$02                   ;set value here in what is also used as
          sta Local_eb                    ;OAM data offset
SdeCLoop: lda Local_eb                    ;check value
          cmp Enemy_MovingDir,x      ;compare value against moving direction
          bne NextSdeC               ;branch if different and do not seek block there
          lda #$01                   ;set flag in A for save horizontal coordinate 
          ; jroweboy(inlined BlockBufferChk_Enemy)
          inx
          jsr BBChk_E   ;find block to left or right of enemy object
          beq NextSdeC               ;if nothing found, branch
          jsr ChkForNonSolids        ;check for non-solid blocks
          bne ChkForBump_HammerBroJ  ;branch if not found
NextSdeC: dec Local_eb               ;move to the next direction
          iny
          cpy #$18                   ;increment Y, loop only if Y < $18, thus we check
          bcc SdeCLoop               ;enemy ($00, $14) and ($10, $14) pixel coordinates
ExESdeC:  rts

ChkForBump_HammerBroJ: 
  cpx #$05               ;check if we're on the special use slot
  beq NoBump             ;and if so, branch ahead and do not play sound
  lda Enemy_State,x      ;if enemy state d7 not set, branch
  asl                    ;ahead and do not play sound
  bcc NoBump
    lda #Sfx_Bump          ;otherwise, play bump sound
    sta Square1SoundQueue  ;sound will never be played if branching from ChkForRedKoopa
NoBump:
  lda Enemy_ID,x         ;check for hammer bro
  cmp #$05
  bne InvEnemyDir        ;branch if not found
    lda #$00
    sta R0                ;initialize value here for bitmask  
    ldy #$fa               ;load default vertical speed for jumping
    jmp SetHJ              ;jump to code that makes hammer bro jump

InvEnemyDir:
  jmp RXSpd     ;jump to turn the enemy around


ImpedePlayerMove:
  lda #$00                  ;initialize value here
  ldy Player_X_Speed        ;get player's horizontal speed
  ldx R0                   ;check value set earlier for
  dex                       ;left side collision
  bne RImpd                 ;if right side collision, skip this part
  inx                       ;return value to X
  cpy #$00                  ;if player moving to the left,
  bmi ExIPM                 ;branch to invert bit and leave
    lda #$ff                  ;otherwise load A with value to be used later
    bne NXSpd                 ;and jump to affect movement
RImpd:
    ldx #$02                  ;return $02 to X
    cpy #$01                  ;if player moving to the right,
    bpl ExIPM                 ;branch to invert bit and leave
    lda #$01                  ;otherwise load A with value to be used here
NXSpd:
    ldy #$10
    sty SideCollisionTimer    ;set timer of some sort
    ldy #$00
    sty Player_X_Speed        ;nullify player's horizontal speed
    cmp #$00                  ;if value set in A not set to $ff,
    bpl PlatF                 ;branch ahead, do not decrement Y
      dey                       ;otherwise decrement Y now
PlatF: 
    sty R0                   ;store Y as high bits of horizontal adder
    clc
    adc Player_X_Position     ;add contents of A to player's horizontal
    sta Player_X_Position     ;position to move player left or right
    lda Player_PageLoc
    adc R0                   ;add high bits and carry to
    sta Player_PageLoc        ;page location if necessary
ExIPM:
  txa                       ;invert contents of X
  eor #$ff
  and Player_CollisionBits  ;mask out bit that was set here
  sta Player_CollisionBits  ;store to clear bit
  rts

;--------------------------------
;$02 - high nybble of vertical coordinate from block buffer
;$04 - low nybble of horizontal coordinate from block buffer
;$06-$07 - block buffer address

; .proc BreakFlagPole
;   ; Step 1: Replace flagpole with sprites
;   lda VRAM_Buffer1_Offset
;   beq CanDraw
;     lda #0
;     sta NmiDisable
;     :
;       cmp NmiDisable
;       beq :-
; CanDraw:
;   lda #5 - 1
;   sta M0
;   lda R2
;   and #%00001111
;   sta R2
;   :
;     jsr DestroyBlockMetatile
;     lda R2
;     clc
;     adc #%00010000
;     sta R2
;     dec M0
;     bpl :-

;   ; add the palette write as well
;   ldx VRAM_Buffer1_Offset
;   lda #$3f
;   sta VRAM_Buffer1+0,x
;   lda #$1e
;   sta VRAM_Buffer1+1,x
;   lda #$01
;   sta VRAM_Buffer1+2,x
;   lda #$29
;   sta VRAM_Buffer1+3,x
;   lda #$00                 ;now the null terminator
;   sta VRAM_Buffer1+4,x
;   txa
;   clc
;   adc #5
;   sta VRAM_Buffer1_Offset

;   ; wait for the second NMI since we can't delete fast enough :P
;   ; without doing more coding
;   lda #0
;   sta NmiDisable
;   :
;     cmp NmiDisable
;     beq :-

;   lda #5 - 1
;   sta M0
;   :
;     jsr DestroyBlockMetatile
;     lda R2
;     clc
;     adc #%00010000
;     sta R2
;     dec M0
;     bpl :-

;   ; add sprite flagpole in the enemy slots
;   ; flagpole flag is fixed in slot #5
;   ; use offsets 0 - 4 to leave the slot for the flag alone
;   ldx #5 - 1
;   :
;     ; don't over write the starflag object!
;     lda Enemy_ID, x
;     cmp #StarFlagObject
;     beq @Skip

;     lda #FlagpoleShatterObject
;     sta Enemy_ID, x
;     sta Enemy_Flag, x

;     lda FlagPoleTile, x
;     sta Enemy_State, x

;     lda Player_X_Position
;     clc
;     adc #8
;     sta Enemy_X_Position, x
;     lda FlagPoleYPosTop, x
;     sta Enemy_Y_Position, x

;     lda PseudoRandomBitReg,x
;     and #%00000111
;     tay
;     adc FlagPoleXSpeed,y
;     sta Enemy_X_Speed,x

;     lda PseudoRandomBitReg,x
;     and #%00000111
;     tay
;     adc FlagPoleYSpeed,y
;     sta Enemy_Y_Speed,x
; @Skip:
;     dex
;     bpl :-

;   ; add the misc flagpole sprites
;   ldx #9 - 1
;   :
;     lda FlagPoleTile, x
;     sta Misc_State, x

;     lda Player_X_Position
;     clc
;     adc #8
;     sta Misc_X_Position, x

;     lda FlagPoleYPosBot, x
;     sta Misc_Y_Position, x

;     lda PseudoRandomBitReg,x
;     and #%00000111
;     tay
;     adc FlagPoleXSpeed,y
;     sta Misc_X_Speed,x
    
;     lda PseudoRandomBitReg,x
;     and #%00000111
;     tay
;     adc FlagPoleYSpeed,y
;     sta Misc_Y_Speed,x

;     tya
;     and #%00000011
;     clc
;     adc #3
;     sta EnemyIntervalTimer, x
;     dex
;     bpl :-

;   ; don't let the code above set the injury timer
;   lda #0
;   sta InjuryTimer

;   ; force the player to rotate to make the chunks rotate too
;   lda #10
;   sta AngularMomentum
;   rts

; FlagPoleTile:
;   .byte $80 | $33, $80 | $34, $80 | $35
;   .byte $80 | $33, $80 | $34, $80 | $35
;   .byte $80 | $33, $80 | $34, $80 | $35
; FlagPoleYPosTop:
; .repeat 5, I
;   .byte $30 + (I * 8)
; .endrepeat
; FlagPoleYPosBot:
; .repeat 9, I
;   .byte $30 + (I * 8) + (5 * 8)
; .endrepeat
; X_ADD = $8
; FlagPoleXSpeed:
;   .byte $8 + X_ADD, $14 + X_ADD, $3 - X_ADD, -$12 - X_ADD
;   .byte $0 + X_ADD, -$20 - X_ADD, $20 + X_ADD, -$6 - X_ADD
; FlagPoleYSpeed:
;   .byte $1, $2, $0, $2, $3, $1, $4, $0
; .endproc

ClimbXPosAdder:
      .byte $f9, $07

ClimbPLocAdder:
      .byte $ff, $00

FlagpoleYPosData:
      .byte $18, $22, $50, $68, $90

HandleClimbing:
      ldy R4            ;check low nybble of horizontal coordinate returned from
      cpy #$06           ;collision detection routine against certain values, this
      bcc ExHC           ;makes actual physical part of vine or flagpole thinner
      cpy #$0a           ;than 16 pixels
      bcc ChkForFlagpole
ExHC: rts                ;leave if too far left or too far right

ChkForFlagpole:
      cmp #$24               ;check climbing metatiles
      beq FlagpoleCollision  ;branch if flagpole ball found
      cmp #$25
      bne VineCollision      ;branch to alternate code if flagpole shaft not found

FlagpoleCollision:
      ; lda GameEngineSubroutine
      ; cmp #$05                  ;check for end-of-level routine running
      ; beq PutPlayerOnVine       ;if running, branch to end of climbing code
      ; lda #$01
      ; sta PlayerFacingDir       ;set player's facing direction to right
      ; inc ScrollLock            ;set scroll lock flag
      ; lda GameEngineSubroutine
      ; cmp #$04                  ;check for flagpole slide routine running
      ; beq RunFR                 ;if running, branch to end of flagpole code here
      ; lda #BulletBill_CannonVar ;load identifier for bullet bills (cannon variant)
      ; jsr KillEnemies           ;get rid of them
      ; lda #Silence
      ; sta EventMusicQueue       ;silence music
      ; lsr
      ; sta FlagpoleSoundQueue    ;load flagpole sound into flagpole sound queue
      ; ldx #$04                  ;start at end of vertical coordinate data
      ; lda Player_Y_Position
      ; sta FlagpoleCollisionYPos ;store player's vertical coordinate here to be used later
  lda GameEngineSubroutine
  cmp #5
  beq PutPlayerOnVine
    lda #$01
    sta PlayerFacingDir       ;set player's facing direction to right
    inc ScrollLock            ;set scroll lock flag
    lda GameEngineSubroutine
    cmp #$04                  ;check for flagpole slide routine running
    beq RunFR                 ;if running, branch to end of flagpole code here

    ; first time hitting the flagpole
    ; so lets 
    ; jsr BreakFlagPole
    ; inc ScrollLock
    ; lda #BulletBill_CannonVar
    ; jsr KillEnemies
    lda #Silence
    sta EventMusicQueue
    lsr
    sta FlagpoleSoundQueue
    ldx #$04                  ;start at end of vertical coordinate data
    lda Player_Y_Position
    sta FlagpoleCollisionYPos ;store player's vertical coordinate here to be used later

ChkFlagpoleYPosLoop:
       cmp FlagpoleYPosData,x    ;compare with current vertical coordinate data
       bcs MtchF                 ;if player's => current, branch to use current offset
       dex                       ;otherwise decrement offset to use 
       bne ChkFlagpoleYPosLoop   ;do this until all data is checked (use last one if all checked)
MtchF: stx FlagpoleScore         ;store offset here to be used later
RunFR: lda #$04
       sta GameEngineSubroutine  ;set value to run flagpole slide routine
       jmp PutPlayerOnVine       ;jump to end of climbing code

VineCollision:
  cmp #$26                  ;check for climbing metatile used on vines
  bne PutPlayerOnVine
  lda Player_Y_Position     ;check player's vertical coordinate
  cmp #$20                  ;for being in status bar area
  bcs PutPlayerOnVine       ;branch if not that far up
  lda #$01
  sta GameEngineSubroutine  ;otherwise set to run autoclimb routine next frame

PutPlayerOnVine:
  lda #$03                ;set player state to climbing
  sta Player_State
  lda #$00                ;nullify player's horizontal speed
  sta Player_X_Speed      ;and fractional horizontal movement force
  sta Player_X_MoveForce
  lda Player_X_Position   ;get player's horizontal coordinate
  sec
  sbc ScreenLeft_X_Pos    ;subtract from left side horizontal coordinate
  cmp #$10
  bcs SetVXPl             ;if 16 or more pixels difference, do not alter facing direction
  lda #$02
  sta PlayerFacingDir     ;otherwise force player to face left
SetVXPl:
  ldy PlayerFacingDir     ;get current facing direction, use as offset
  lda R6                 ;get low byte of block buffer address
  asl
  asl                     ;move low nybble to high
  asl
  asl
  clc
  adc ClimbXPosAdder-1,y  ;add pixels depending on facing direction
  sta Player_X_Position   ;store as player's horizontal coordinate
  lda R6                 ;get low byte of block buffer address again
  bne ExPVne              ;if not zero, branch
  lda ScreenRight_PageLoc ;load page location of right side of screen
  clc
  adc ClimbPLocAdder-1,y  ;add depending on facing location
  sta Player_PageLoc      ;store as player's page location
ExPVne:
  rts                     ;finally, we're done!

;--------------------------------

ChkInvisibleMTiles:
         cmp #$5f       ;check for hidden coin block
         beq ExCInvT    ;branch to leave if found
         cmp #$60       ;check for hidden 1-up block
ExCInvT: rts            ;leave with zero flag set if either found

;--------------------------------
;$00-$01 - used to hold bottom right and bottom left metatiles (in that order)
;$00 - used as flag by ImpedePlayerMove to restrict specific movement

ChkForLandJumpSpring:
        jsr ChkJumpspringMetatiles  ;do sub to check if player landed on jumpspring
        bcc ExCJSp                  ;if carry not set, jumpspring not found, therefore leave
        lda #$70
        sta VerticalForce           ;otherwise set vertical movement force for player
        lda #$f9
        sta JumpspringForce         ;set default jumpspring force
        lda #$03
        sta JumpspringTimer         ;set jumpspring timer to be used later
        lsr
        sta JumpspringAnimCtrl      ;set jumpspring animation control to start animating
ExCJSp: rts                         ;and leave

ChkJumpspringMetatiles:
         cmp #$67      ;check for top jumpspring metatile
         beq JSFnd     ;branch to set carry if found
         cmp #$68      ;check for bottom jumpspring metatile
         clc           ;clear carry flag
         bne NoJSFnd   ;branch to use cleared carry if not found
JSFnd:   sec           ;set carry if found
NoJSFnd: rts           ;leave

;-------------------------------------------------------------------------------------
;These apply to all routines in this section unless otherwise noted:
;$00 - used to store metatile from block buffer routine
;$02 - used to store vertical high nybble offset from block buffer routine
;$05 - used to store metatile stored in A at beginning of PlayerHeadCollision
;$06-$07 - used as block buffer address indirect

BlockYPosAdderData:
      .byte $04, $12

PlayerHeadCollision:
           pha                      ;store metatile number to stack
           lda #$11                 ;load unbreakable block object state by default
           ldx SprDataOffset_Ctrl   ;load offset control bit here
           ldy PlayerSize           ;check player's size
           bne DBlockSte            ;if small, branch
           lda #$12                 ;otherwise load breakable block object state
DBlockSte: sta Block_State,x        ;store into block object buffer
           jsr DestroyBlockMetatile ;store blank metatile in vram buffer to write to name table
           ldx SprDataOffset_Ctrl   ;load offset control bit
           lda R2                  ;get vertical high nybble offset used in block buffer routine
           sta Block_Orig_YPos,x    ;set as vertical coordinate for block object
           tay
           lda R6                  ;get low byte of block buffer address used in same routine
           sta Block_BBuf_Low,x     ;save as offset here to be used later
           lda ($06),y              ;get contents of block buffer at old address at $06, $07
           jsr BlockBumpedChk       ;do a sub to check which block player bumped head on
           sta R0                  ;store metatile here
           ldy PlayerSize           ;check player's size
           bne ChkBrick             ;if small, use metatile itself as contents of A
           tya                      ;otherwise init A (note: big = 0)
ChkBrick:  bcc PutMTileB            ;if no match was found in previous sub, skip ahead
           ldy #$11                 ;otherwise load unbreakable state into block object buffer
           sty Block_State,x        ;note this applies to both player sizes
           lda #$c4                 ;load empty block metatile into A for now
           ldy R0                  ;get metatile from before
           cpy #$58                 ;is it brick with coins (with line)?
           beq StartBTmr            ;if so, branch
           cpy #$5d                 ;is it brick with coins (without line)?
           bne PutMTileB            ;if not, branch ahead to store empty block metatile
StartBTmr: lda BrickCoinTimerFlag   ;check brick coin timer flag
           bne ContBTmr             ;if set, timer expired or counting down, thus branch
           lda #$0b
           sta BrickCoinTimer       ;if not set, set brick coin timer
           inc BrickCoinTimerFlag   ;and set flag linked to it
ContBTmr:  lda BrickCoinTimer       ;check brick coin timer
           bne PutOldMT             ;if not yet expired, branch to use current metatile
           ldy #$c4                 ;otherwise use empty block metatile
PutOldMT:  tya                      ;put metatile into A
PutMTileB: sta Block_Metatile,x     ;store whatever metatile be appropriate here
           jsr InitBlock_XY_Pos     ;get block object horizontal coordinates saved
           ldy R2                  ;get vertical high nybble offset
           lda #$23
           sta ($06),y              ;write blank metatile $23 to block buffer
           lda #$10
           sta BlockBounceTimer     ;set block bounce timer
           pla                      ;pull original metatile from stack
           sta R5                  ;and save here
           ldy #$00                 ;set default offset
           lda CrouchingFlag        ;is player crouching?
           bne SmallBP              ;if so, branch to increment offset
           lda PlayerSize           ;is player big?
           beq BigBP                ;if so, branch to use default offset
SmallBP:   iny                      ;increment for small or big and crouching
BigBP:     lda Player_Y_Position    ;get player's vertical coordinate
           clc
           adc BlockYPosAdderData,y ;add value determined by size
           and #$f0                 ;mask out low nybble to get 16-pixel correspondence
           sta Block_Y_Position,x   ;save as vertical coordinate for block object
           ldy Block_State,x        ;get block object state
           cpy #$11
           beq Unbreak              ;if set to value loaded for unbreakable, branch
           jsr BrickShatter         ;execute code for breakable brick
           jmp InvOBit              ;skip subroutine to do last part of code here
Unbreak:   jsr BumpBlock            ;execute code for unbreakable brick or question block
InvOBit:   lda SprDataOffset_Ctrl   ;invert control bit used by block objects
           eor #$01                 ;and floatey numbers
           sta SprDataOffset_Ctrl
           rts                      ;leave!

; QuickBrickShatterWhenBig:
;   ldx PlayerSize
;   beq @PlayerBig
; @Exit:
;     rts
; @PlayerBig:
; ; brick metatile is 51 and 52
;   cmp #$51
;   bcc @Exit
;   cmp #$53 
;   bcs @Exit
;   pha
;     phy
;       jsr PlayerHeadCollision     ;otherwise do a sub to process collision
;     ply
;   pla
;   rts

;--------------------------------

EnemyLanding:
      jsr InitVStf            ;do something here to vertical speed and something else
      lda Enemy_Y_Position,x
      and #%11110000          ;save high nybble of vertical coordinate, and
      ora #%00001000          ;set d3, then store, probably used to set enemy object
      sta Enemy_Y_Position,x  ;neatly on whatever it's landing on
      rts

SubtEnemyYPos:
      lda Enemy_Y_Position,x  ;add 62 pixels to enemy object's
      clc                     ;vertical coordinate
      adc #$3e
      cmp #$44                ;compare against a certain range
      rts                     ;and leave with flags set for conditional branch

EnemyJump:
        jsr SubtEnemyYPos     ;do a sub here
        bcc DoSide            ;if enemy vertical coord + 62 < 68, branch to leave
        lda Enemy_Y_Speed,x
        clc                   ;add two to vertical speed
        adc #$02
        cmp #$03              ;if green paratroopa not falling, branch ahead
        bcc DoSide
        jsr ChkUnderEnemy     ;otherwise, check to see if green paratroopa is 
        beq DoSide            ;standing on anything, then branch to same place if not
        jsr ChkForNonSolids   ;check for non-solid blocks
        beq DoSide            ;branch if found
        jsr EnemyLanding      ;change vertical coordinate and speed
        lda #$fd
        sta Enemy_Y_Speed,x   ;make the paratroopa jump again
DoSide: jmp DoEnemySideCheck  ;check for horizontal blockage, then leave

;--------------------------------

HammerBroBGColl:
      jsr ChkUnderEnemy    ;check to see if hammer bro is standing on anything
      beq NoUnderHammerBro      
      cmp #$23             ;check for blank metatile $23 and branch if not found
      bne UnderHammerBro

KillEnemyAboveBlock:
      jsr ShellOrBlockDefeat  ;do this sub to kill enemy
      lda #$fc                ;alter vertical speed of enemy and leave
      sta Enemy_Y_Speed,x
      rts

UnderHammerBro:
      lda EnemyFrameTimer,x ;check timer used by hammer bro
      bne NoUnderHammerBro  ;branch if not expired
      lda Enemy_State,x
      and #%10001000        ;save d7 and d3 from enemy state, nullify other bits
      sta Enemy_State,x     ;and store
      jsr EnemyLanding      ;modify vertical coordinate, speed and something else
      jmp DoEnemySideCheck  ;then check for horizontal blockage and leave

NoUnderHammerBro:
      lda Enemy_State,x  ;if hammer bro is not standing on anything, set d0
      ora #$01           ;in the enemy state to indicate jumping or falling, then leave
      sta Enemy_State,x
      rts

ChkUnderEnemy:
      lda #$00                  ;set flag in A for save vertical coordinate
      ldy #$15                  ;set Y to check the bottom middle (8,18) of enemy object
      inx ; jroweboy(inlined BlockBufferChk_Enemy)
      jmp BBChk_E  ;hop to it!

