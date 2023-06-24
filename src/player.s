
.include "common.inc"
.include "player.inc"

; objects/object
.import BoundingBoxCore

; collision.s
.import PlayerBGCollision, FireballBGCollision, FireballEnemyCollision

; gamemode.s
.import ContinueGame

; sprite_render.s
.import DrawExplosion_Fireball, DumpTwoSpr, DrawOneSpriteRow

; screen_render.s
.import GetPlayerColors

; tiles/brick.s
.import InitBlock_XY_Pos

; objects/vine.s
.import Setup_Vine

.import BubbleCheck

.export DrawPlayer_Intermediate

; gamemode.s
.export GameRoutines, PlayerCtrlRoutine, ScrollScreen

; gamecore.s
.export ResetPalStar

.segment "PLAYER"

.import CosTable, SinTable

;-------------------------------------------------------------------------------------

.proc GameRoutines
  lda GameEngineSubroutine  ;run routine based on number (a few of these routines are   
  jsr JumpEngine            ;merely placeholders as conditions for other routines)

  .word Entrance_GameTimerSetup
  .word Vine_AutoClimb
  .word SideExitPipeEntry
  .word VerticalPipeEntry
  .word FlagpoleSlide
  .word PlayerEndLevel
  .word PlayerLoseLife
  .word PlayerEntrance
  .word PlayerCtrlRoutine
  .word PlayerChangeSize
  .word PlayerInjuryBlink
  .word PlayerDeath
  .word PlayerFireFlower
.endproc

;-------------------------------------------------------------------------------------

PlayerEntrance:
  lda AltEntranceControl    ;check for mode of alternate entry
  cmp #$02
  beq EntrMode2             ;if found, branch to enter from pipe or with vine
  lda #$00       
  ldy Player_Y_Position     ;if vertical position above a certain
  cpy #$30                  ;point, nullify controller bits and continue
  bcc AutoControlPlayer     ;with player movement code, do not return
    lda PlayerEntranceCtrl    ;check player entry bits from header
    cmp #$06
    beq ChkBehPipe            ;if set to 6 or 7, execute pipe intro code
    cmp #$07                  ;otherwise branch to normal entry
    bne PlayerRdy
ChkBehPipe:
  lda Player_SprAttrib      ;check for sprite attributes
  bne IntroEntr             ;branch if found
    lda #$01
    jmp AutoControlPlayer     ;force player to walk to the right
IntroEntr:
  jsr EnterSidePipe         ;execute sub to move player to the right
  dec ChangeAreaTimer       ;decrement timer for change of area
  bne ExitEntr              ;branch to exit if not yet expired
    inc DisableIntermediate   ;set flag to skip world and lives display
    jmp NextArea              ;jump to increment to next area and set modes
EntrMode2:
  lda JoypadOverride        ;if controller override bits set here,
  bne VineEntr              ;branch to enter with vine
  lda #$ff                  ;otherwise, set value here then execute sub
  jsr MovePlayerYAxis       ;to move player upwards
  lda Player_Y_Position     ;check to see if player is at a specific coordinate
  cmp #$91                  ;if player risen to a certain point (this requires pipes
  bcc PlayerRdy             ;to be at specific height to look/function right) branch
    rts                       ;to the last part, otherwise leave
VineEntr:
  lda Vine_Height
  cmp #$60                  ;check vine height
  bne ExitEntr              ;if vine not yet reached maximum height, branch to leave
  lda Player_Y_Position     ;get player's vertical coordinate
  cmp #$99                  ;check player's vertical coordinate against preset value
  ldy #$00                  ;load default values to be written to 
  lda #$01                  ;this value moves player to the right off the vine
  bcc OffVine               ;if vertical coordinate < preset value, use defaults
  lda #$03
  sta Player_State          ;otherwise set player state to climbing
  iny                       ;increment value in Y
  lda #$08                  ;set block in block buffer to cover hole, then 
  sta Block_Buffer_1+$b4    ;use same value to force player to climb
OffVine:
  sty DisableCollisionDet   ;set collision detection disable flag
  jsr AutoControlPlayer     ;use contents of A to move player up or right, execute sub
  lda Player_X_Position
  cmp #$48                  ;check player's horizontal position
  bcc ExitEntr              ;if not far enough to the right, branch to leave
PlayerRdy:
  lda #$08                  ;set routine to be executed by game engine next frame
  sta GameEngineSubroutine
  lda #$01                  ;set to face player to the right
  sta PlayerFacingDir
  lsr                       ;init A
  sta AltEntranceControl    ;init mode of entry
  sta DisableCollisionDet   ;init collision detection disable flag
  sta JoypadOverride        ;nullify controller override bits
ExitEntr:
  rts                       ;leave!


;-------------------------------------------------------------------------------------
;$07 - used to hold upper limit of high byte when player falls down hole

AutoControlPlayer:
  sta SavedJoypadBits         ;override controller bits with contents of A if executing here

PlayerCtrlRoutine:
  lda GameEngineSubroutine    ;check task here
  cmp #$0b                    ;if certain value is set, branch to skip controller bit loading
  beq SizeChk
    lda AreaType                ;are we in a water type area?
    bne SaveJoyp                ;if not, branch
      ldy Player_Y_HighPos
      dey                         ;if not in vertical area between
      bne DisJoyp                 ;status bar and bottom, branch
        lda Player_Y_Position
        cmp #$d0                    ;if nearing the bottom of the screen or
        bcc SaveJoyp                ;not in the vertical area between status bar or bottom,
DisJoyp:
          lda #$00                    ;disable controller bits
          sta SavedJoypadBits
SaveJoyp:
  lda SavedJoypadBits         ;otherwise store A and B buttons in $0a
  and #%11000000
  sta A_B_Buttons
  lda SavedJoypadBits         ;store left and right buttons in $0c
  and #%00000011
  sta Left_Right_Buttons
  lda SavedJoypadBits         ;store up and down buttons in $0b
  and #%00001100
  sta Up_Down_Buttons

;   and #%00000100              ;check for pressing down
;   beq SizeChk                 ;if not, branch
;     lda Player_State            ;check player's state
;     bne SizeChk                 ;if not on the ground, branch
;       ldy Left_Right_Buttons      ;check left and right
;       beq SizeChk                 ;if neither pressed, branch
;         lda #$00
;         sta Left_Right_Buttons      ;if pressing down while on the ground,
;         sta Up_Down_Buttons         ;nullify directional bits
SizeChk:
  jsr PlayerMovementSubs      ;run movement subroutines
  ldy #$01                    ;is player small?
  lda PlayerSize
  bne ChkMoveDir
    ldy #$00                    ;check for if crouching
    lda CrouchingFlag
    beq ChkMoveDir              ;if not, branch ahead
      ldy #$02                    ;if big and crouching, load y with 2
ChkMoveDir:
  sty Player_BoundBoxCtrl     ;set contents of Y as player's bounding box size control
  lda #$01                    ;set moving direction to right by default
  ldy Player_X_Speed          ;check player's horizontal speed
  beq PlayerSubs              ;if not moving at all horizontally, skip this part
    bpl SetMoveDir              ;if moving to the right, use default moving direction
      asl                         ;otherwise change to move to the left
  SetMoveDir:
    sta Player_MovingDir        ;set moving direction
PlayerSubs:
  jsr ScrollHandler           ;move the screen if necessary
  jsr GetPlayerOffscreenBits  ;get player's offscreen bits
  jsr RelativePlayerPosition  ;get coordinates relative to the screen
  ldx #$00                    ;set offset for player object
  farcall BoundingBoxCore     ;get player's bounding box coordinates
  jsr PlayerBGCollision       ;do collision detection and process
  lda Player_Y_Position
  cmp #$40                    ;check to see if player is higher than 64th pixel
  bcc PlayerHole              ;if so, branch ahead
    lda GameEngineSubroutine
    cmp #$05                    ;if running end-of-level routine, branch ahead
    beq PlayerHole
      cmp #$07                    ;if running player entrance routine, branch ahead
      beq PlayerHole
        cmp #$04                    ;if running routines $00-$03, branch ahead
        bcc PlayerHole
          lda Player_SprAttrib
          and #%11011111              ;otherwise nullify player's
          sta Player_SprAttrib        ;background priority flag
PlayerHole:
  lda Player_Y_HighPos        ;check player's vertical high byte
  cmp #$02                    ;for below the screen
  bmi ExitCtrl                ;branch to leave if not that far down
  ldx #$01
  stx ScrollLock              ;set scroll lock
  ldy #$04
  sty R7                     ;set value here
  ldx #$00                    ;use X as flag, and clear for cloud level
  ldy GameTimerExpiredFlag    ;check game timer expiration flag
  bne HoleDie                 ;if set, branch
    ldy CloudTypeOverride       ;check for cloud type override
    bne ChkHoleX                ;skip to last part if found
HoleDie:
  inx                         ;set flag in X for player death
  ldy GameEngineSubroutine
  cpy #$0b                    ;check for some other routine running
  beq ChkHoleX                ;if so, branch ahead
    ldy DeathMusicLoaded        ;check value here
    bne HoleBottom              ;if already set, branch to next part
      iny
      sty EventMusicQueue         ;otherwise play death music
      sty DeathMusicLoaded        ;and set value here
HoleBottom:
  ldy #$06
  sty R7                     ;change value here
ChkHoleX:
  cmp R7                     ;compare vertical high byte with value set here
  bmi ExitCtrl                ;if less, branch to leave
    dex                         ;otherwise decrement flag in X
    bmi CloudExit               ;if flag was clear, branch to set modes and other values
      ldy EventMusicBuffer        ;check to see if music is still playing
      bne ExitCtrl                ;branch to leave if so
        lda #$06                    ;otherwise set to run lose life routine
        sta GameEngineSubroutine    ;on next frame
ExitCtrl:
  rts                         ;leave

CloudExit:
  lda #$00
  sta JoypadOverride      ;clear controller override bits if any are set
  jsr SetEntr             ;do sub to set secondary mode
  inc AltEntranceControl  ;set mode of entry to 3
  rts

;-------------------------------------------------------------------------------------

Vine_AutoClimb:
  lda Player_Y_HighPos   ;check to see whether player reached position
  bne AutoClimb          ;above the status bar yet and if so, set modes
  lda Player_Y_Position
  cmp #$e4
  bcc SetEntr
AutoClimb:
  lda #%00001000         ;set controller bits override to up
  sta JoypadOverride
  ldy #$03               ;set player state to climbing
  sty Player_State
  jmp AutoControlPlayer
SetEntr:
  lda #$02               ;set starting position to override
  sta AltEntranceControl
  jmp ChgAreaMode        ;set modes

;-------------------------------------------------------------------------------------

VerticalPipeEntry:
  lda #$01             ;set 1 as movement amount
  jsr MovePlayerYAxis  ;do sub to move player downwards
  jsr ScrollHandler    ;do sub to scroll screen with saved force if necessary
  ldy #$00             ;load default mode of entry
  lda WarpZoneControl  ;check warp zone control variable/flag
  bne ChgAreaPipe      ;if set, branch to use mode 0
  iny
  lda AreaType         ;check for castle level type
  cmp #$03
  bne ChgAreaPipe      ;if not castle type level, use mode 1
  iny
  jmp ChgAreaPipe      ;otherwise use mode 2

MovePlayerYAxis:
  clc
  adc Player_Y_Position ;add contents of A to player position
  sta Player_Y_Position
  rts

;-------------------------------------------------------------------------------------

SideExitPipeEntry:
             jsr EnterSidePipe         ;execute sub to move player to the right
             ldy #$02
ChgAreaPipe: dec ChangeAreaTimer       ;decrement timer for change of area
             bne ExitCAPipe
             sty AltEntranceControl    ;when timer expires set mode of alternate entry
ChgAreaMode: inc DisableScreenFlag     ;set flag to disable screen output
             lda #$00
             sta OperMode_Task         ;set secondary mode of operation
             sta Sprite0HitDetectFlag  ;disable sprite 0 check
ExitCAPipe:  rts                       ;leave

EnterSidePipe:
           lda #$08               ;set player's horizontal speed
           sta Player_X_Speed
           ldy #$01               ;set controller right button by default
           lda Player_X_Position  ;mask out higher nybble of player's
           and #%00001111         ;horizontal position
           bne RightPipe
           sta Player_X_Speed     ;if lower nybble = 0, set as horizontal speed
           tay                    ;and nullify controller bit override here
RightPipe: tya                    ;use contents of Y to
           jmp AutoControlPlayer  ;execute player control routine with ctrl bits nulled
           rts ; TODO check this RTS can be removed

;-------------------------------------------------------------------------------------

;page numbers are in order from -1 to -4
HalfwayPageNybbles:
      .byte $56, $40
      .byte $65, $70
      .byte $66, $40
      .byte $66, $40
      .byte $66, $40
      .byte $66, $60
      .byte $65, $70
      .byte $00, $00

.proc PlayerLoseLife
  inc DisableScreenFlag    ;disable screen and sprite 0 check
  lda #$00
  sta Sprite0HitDetectFlag
  lda #Silence             ;silence music
  sta EventMusicQueue
  dec NumberofLives        ;take one life from player
  bpl StillInGame          ;if player still has lives, branch
  lda #$00
  sta OperMode_Task        ;initialize mode task,
  lda #MODE_GAMEOVER       ;switch to game over mode
  sta OperMode             ;and leave
  rts
StillInGame:
  lda WorldNumber          ;multiply world number by 2 and use
  asl                      ;as offset
  tax
  lda LevelNumber          ;if in area -3 or -4, increment
  and #$02                 ;offset by one byte, otherwise
  beq GetHalfway           ;leave offset alone
  inx
GetHalfway:
  ldy HalfwayPageNybbles,x ;get halfway page number with offset
  lda LevelNumber          ;check area number's LSB
  lsr
  tya                      ;if in area -2 or -4, use lower nybble
  bcs MaskHPNyb
  lsr                      ;move higher nybble to lower if area
  lsr                      ;number is -1 or -3
  lsr
  lsr
MaskHPNyb:
  and #%00001111           ;mask out all but lower nybble
  cmp ScreenLeft_PageLoc
  beq SetHalfway           ;left side of screen must be at the halfway page,
  bcc SetHalfway           ;otherwise player must start at the
  lda #$00                 ;beginning of the level
SetHalfway:
  sta HalfwayPage          ;store as halfway page for player
  jsr TransposePlayers     ;switch players around if 2-player game

  jmp ContinueGame         ;continue the game
.endproc


PlayerStarting_X_Pos:
      .byte $28, $18
      .byte $38, $28

AltYPosOffset:
      .byte $08, $00

PlayerStarting_Y_Pos:
      .byte $00, $20, $b0, $50, $00, $00, $b0, $b0
      .byte $f0

PlayerBGPriorityData:
      .byte $00, $20, $00, $00, $00, $00, $00, $00

GameTimerData:
      .byte $20 ;dummy byte, used as part of bg priority data
      .byte $04, $03, $02

Entrance_GameTimerSetup:
          lda ScreenLeft_PageLoc      ;set current page for area objects
          sta Player_PageLoc          ;as page location for player
          lda #$28                    ;store value here
          sta VerticalForceDown       ;for fractional movement downwards if necessary
          lda #$01                    ;set high byte of player position and
          sta PlayerFacingDir         ;set facing direction so that player faces right
          sta Player_Y_HighPos
          lda #$00                    ;set player state to on the ground by default
          sta Player_State
          dec Player_CollisionBits    ;initialize player's collision bits
          ldy #$00                    ;initialize halfway page
          sty HalfwayPage      
          lda AreaType                ;check area type
          bne ChkStPos                ;if water type, set swimming flag, otherwise do not set
          iny
ChkStPos: sty SwimmingFlag
          ldx PlayerEntranceCtrl      ;get starting position loaded from header
          ldy AltEntranceControl      ;check alternate mode of entry flag for 0 or 1
          beq SetStPos
          cpy #$01
          beq SetStPos
          ldx AltYPosOffset-2,y       ;if not 0 or 1, override $0710 with new offset in X
SetStPos: lda PlayerStarting_X_Pos,y  ;load appropriate horizontal position
          sta Player_X_Position       ;and vertical positions for the player, using
          lda PlayerStarting_Y_Pos,x  ;AltEntranceControl as offset for horizontal and either $0710
          sta Player_Y_Position       ;or value that overwrote $0710 as offset for vertical
          lda PlayerBGPriorityData,x
          sta Player_SprAttrib        ;set player sprite attributes using offset in X
          jsr GetPlayerColors         ;get appropriate player palette
          ldy GameTimerSetting        ;get timer control value from header
          beq ChkOverR                ;if set to zero, branch (do not use dummy byte for this)
          lda FetchNewGameTimerFlag   ;do we need to set the game timer? if not, use 
          beq ChkOverR                ;old game timer setting
          lda GameTimerData,y         ;if game timer is set and game timer flag is also set,
          sta GameTimerDisplay        ;use value of game timer control for first digit of game timer
          lda #$01
          sta GameTimerDisplay+2      ;set last digit of game timer to 1
          lsr
          sta GameTimerDisplay+1      ;set second digit of game timer
          sta FetchNewGameTimerFlag   ;clear flag for game timer reset
          sta StarInvincibleTimer     ;clear star mario timer
ChkOverR: ldy JoypadOverride          ;if controller bits not set, branch to skip this part
          beq ChkSwimE
          lda #$03                    ;set player state to climbing
          sta Player_State
          ldx #$00                    ;set offset for first slot, for block object
          jsr InitBlock_XY_Pos
          lda #$f0                    ;set vertical coordinate for block object
          sta Block_Y_Position
          ldx #$05                    ;set offset in X for last enemy object buffer slot
          ldy #$00                    ;set offset in Y for object coordinates used earlier
          farcall Setup_Vine              ;do a sub to grow vine
ChkSwimE: ldy AreaType                ;if level not water-type,
          bne SetPESub                ;skip this subroutine
          farcall SetupBubble             ;otherwise, execute sub to set up air bubbles
SetPESub: lda #$07                    ;set to run player entrance subroutine
          sta GameEngineSubroutine    ;on the next frame of game engine
          rts

;-------------------------------------------------------------------------------------

.proc DrawPlayer_Intermediate

;   ldx #$05                       ;store data into zero page memory
; PIntLoop:
;     lda IntermediatePlayerData,x   ;load data to display player as he always
;     sta R2,x                      ;appears on world/lives display
;     dex
;     bpl PIntLoop                   ;do this until all data is loaded
  lda #$58 ; y coord
  sta R2
  lda #$60
  sta Player_Rel_XPos
  ldx #0                       ;load offset for small standing
  ldy #0                       ;load sprite data offset
  jsr DrawPlayerLoop             ;draw player accordingly
  ; lda Sprite_Attributes+36       ;get empty sprite attributes
  ; ora #%01000000                 ;set horizontal flip bit for bottom-right sprite
  ; sta Sprite_Attributes+32       ;store and leave
  rts
; IntermediatePlayerData:
;   .byte $58, $01, $00, $60, $ff, $04

.endproc

RenderPlayerSub:
  lda Player_Rel_XPos
  sta Player_Pos_ForScroll     ;store player's relative horizontal position
  lda Player_Rel_YPos
  sta R2
  ldy #0
  lda PlayerSize
  bne DrawSmallPlayer
    ldx #BigMarioGraphics-SmallMarioGraphics
    ; Large Player
DrawLargePlayerLoop:
    lda PlayerGraphicsTable,x
    sta Sprite_Tilenumber,y
    inx
    lda PlayerGraphicsTable,x
    sta Sprite_Tilenumber+4,y
    inx
    lda PlayerGraphicsTable,x
    sta Sprite_Tilenumber+8,y
    inx
    lda PlayerGraphicsTable,x
    sta Sprite_Tilenumber+12,y
    inx
    lda R2
    clc
    adc #4 ; offset by +16 since always small and -4 for the new 24x24 mode
    sta Sprite_Y_Position,y
    sta Sprite_Y_Position+4,y
    sta Sprite_Y_Position+8,y
    sta Sprite_Y_Position+12,y
    lda Player_Rel_XPos
    sec
    sbc #$04
    sta Sprite_X_Position,y
    clc
    adc #$08
    sta Sprite_X_Position+4,y
    clc
    adc #$08
    sta Sprite_X_Position+8,y
    clc
    adc #$08
    sta Sprite_X_Position+12,y
    lda Player_SprAttrib
    sta Sprite_Attributes,y    ;store sprite attributes
    sta Sprite_Attributes+4,y
    sta Sprite_Attributes+8,y
    sta Sprite_Attributes+12,y
    lda R2                    ;add eight pixels to the next y
    clc                        ;coordinate
    adc #$08
    sta R2
    tya                        ;add twelve to the offset in Y to
    clc                        ;move to the next two sprites
    adc #16
    tay
    cpy #64 ; draw four rows of four sprites
    bcc DrawLargePlayerLoop
    rts
DrawSmallPlayer:
  ldx #0
DrawPlayerLoop:
  lda PlayerGraphicsTable,x
  sta Sprite_Tilenumber,y
  inx
  lda PlayerGraphicsTable,x
  sta Sprite_Tilenumber+4,y
  inx
  lda PlayerGraphicsTable,x
  sta Sprite_Tilenumber+8,y
  inx
  lda R2
  clc
  adc #12 ; offset by +16 since always small and -4 for the new 24x24 mode
  sta Sprite_Y_Position,y
  sta Sprite_Y_Position+4,y
  sta Sprite_Y_Position+8,y
  lda Player_Rel_XPos
  sec
  sbc #$04
  sta Sprite_X_Position,y
  clc
  adc #$08
  sta Sprite_X_Position+4,y
  clc
  adc #$08
  sta Sprite_X_Position+8,y
  lda Player_SprAttrib
  sta Sprite_Attributes,y    ;store sprite attributes
  sta Sprite_Attributes+4,y
  sta Sprite_Attributes+8,y
  lda R2                    ;add eight pixels to the next y
  clc                        ;coordinate
  adc #$08
  sta R2
  tya                        ;add twelve to the offset in Y to
  clc                        ;move to the next two sprites
  adc #12
  tay
  cpy #36 ; draw three rows of sprites
  bcc DrawPlayerLoop

  rts

;-------------------------------------------------------------------------------------
;$00-$01 - used to hold tile numbers, $00 also used to hold upper extent of animation frames
;$02 - vertical position
;$03 - facing direction, used as horizontal flip control
;$04 - attributes
;$05 - horizontal position
;$07 - number of rows to draw
;these also used in IntermediatePlayerData
; RenderPlayerSub:
;   sta R7                      ;store number of rows of sprites to draw
;   lda Player_Rel_XPos
;   sta Player_Pos_ForScroll     ;store player's relative horizontal position
;   sta R5                      ;store it here also
;   lda Player_Rel_YPos
;   sta R2                      ;store player's vertical position
;   lda PlayerFacingDir
;   sta R3                      ;store player's facing direction
;   lda Player_SprAttrib
;   sta R4                      ;store player's sprite attributes
;   ldx PlayerGfxOffset          ;load graphics table offset
;   ldy Player_SprDataOffset     ;get player's sprite data offset

; DrawPlayerLoop:
;     lda PlayerGraphicsTable,x    ;load player's left side
;     sta R0
;     lda PlayerGraphicsTable+1,x  ;now load right side
;     jsr DrawOneSpriteRow
;     dec R7                      ;decrement rows of sprites to draw
;     bne DrawPlayerLoop           ;do this until all rows are drawn  
;   rts

;tiles arranged in order, 2 tiles per row, top to bottom
; SwimTileRepOffset     = PlayerGraphicsTable + $9e
PlayerKilledGraphicsOffset = $00

PlayerHolsteredOffset = SmallMarioHolstered - PlayerGraphicsTable

PlayerGraphicsTable:
; Mario is now 24x24
; Good luck everybody.
SmallMarioGraphics:
; small mario sideways
.byte $02, $03, $04
.byte $12, $13, $14
.byte $22, $23, $24
SmallMarioHolstered:
; small mario holstered
.byte $05, $06, $07
.byte $15, $16, $17
.byte $25, $26, $27

; Mario is now 32x32
; hehehehe good luck punk.
BigMarioGraphics:
; big mario sideways
.byte $08, $09, $0a, $0b
.byte $18, $19, $1a, $1b
.byte $28, $29, $2a, $2b
.byte $38, $39, $3a, $3b
BigMarioHolstered:
; big mario sideways
.byte $0c, $0d, $0e, $0f
.byte $1c, $1d, $1e, $1f
.byte $2c, $2d, $2e, $2f
.byte $3c, $3d, $3e, $3f


; TODO small mario death animation rotated too

;big player table
;   .byte $00, $01, $02, $03, $04, $05, $06, $07 ;walking frame 1
;   .byte $08, $09, $0a, $0b, $0c, $0d, $0e, $0f ;        frame 2
;   .byte $10, $11, $12, $13, $14, $15, $16, $17 ;        frame 3
;   .byte $18, $19, $1a, $1b, $1c, $1d, $1e, $1f ;skidding
;   .byte $20, $21, $22, $23, $24, $25, $26, $27 ;jumping
;   .byte $08, $09, $28, $29, $2a, $2b, $2c, $2d ;swimming frame 1
;   .byte $08, $09, $0a, $0b, $0c, $30, $2c, $2d ;         frame 2
;   .byte $08, $09, $0a, $0b, $2e, $2f, $2c, $2d ;         frame 3
;   .byte $08, $09, $28, $29, $2a, $2b, $5c, $5d ;climbing frame 1
;   .byte $08, $09, $0a, $0b, $0c, $0d, $5e, $5f ;         frame 2
;   .byte $fc, $fc, $08, $09, $58, $59, $5a, $5a ;crouching
;   .byte $08, $09, $28, $29, $2a, $2b, $0e, $0f ;fireball throwing

; ;small player table
;   .byte $fc, $fc, $fc, $fc, $32, $33, $34, $35 ;walking frame 1
;   .byte $fc, $fc, $fc, $fc, $36, $37, $38, $39 ;        frame 2
;   .byte $fc, $fc, $fc, $fc, $3a, $37, $3b, $3c ;        frame 3
;   .byte $fc, $fc, $fc, $fc, $3d, $3e, $3f, $40 ;skidding
;   .byte $fc, $fc, $fc, $fc, $32, $41, $42, $43 ;jumping
;   .byte $fc, $fc, $fc, $fc, $32, $33, $44, $45 ;swimming frame 1
;   .byte $fc, $fc, $fc, $fc, $32, $33, $44, $47 ;         frame 2
;   .byte $fc, $fc, $fc, $fc, $32, $33, $48, $49 ;         frame 3
;   .byte $fc, $fc, $fc, $fc, $32, $33, $90, $91 ;climbing frame 1
;   .byte $fc, $fc, $fc, $fc, $3a, $37, $92, $93 ;         frame 2
;   .byte $fc, $fc, $fc, $fc, $9e, $9e, $9f, $9f ;killed

; ;used by both player sizes
;   .byte $fc, $fc, $fc, $fc, $3a, $37, $4f, $4f ;small player standing
;   .byte $fc, $fc, $00, $01, $4c, $4d, $4e, $4e ;intermediate grow frame
;   .byte $00, $01, $4c, $4d, $4a, $4a, $4b, $4b ;big player standing

SwimKickTileNum:
  .byte $31, $46

;-------------------------------------------------------------------------------------
;$00 - used to store player's vertical offscreen bits
PlayerGfxHandler:
  lda InjuryTimer             ;if player's injured invincibility timer
  beq CntPl                   ;not set, skip checkpoint and continue code
  lda FrameCounter
  lsr                         ;otherwise check frame counter and branch
  bcs ExPGH                   ;to leave on every other frame (when d0 is set)
CntPl:
  lda GameEngineSubroutine    ;if executing specific game engine routine,
  cmp #$0b                    ;branch ahead to some other part
  beq PlayerKilled
  lda PlayerChangeSizeFlag    ;if grow/shrink flag set
  bne DoChangeSize            ;then branch to some other code
  ldy SwimmingFlag            ;if swimming flag set, branch to
  beq FindPlayerAction        ;different part, do not return
  lda Player_State
  ; cmp #$00                    ;if player status normal,
  beq FindPlayerAction        ;branch and do not return
  jsr FindPlayerAction        ;otherwise jump and return
  ; lda FrameCounter
  ; and #%00000100              ;check frame counter for d2 set (8 frames every
  ; bne ExPGH                   ;eighth frame), and branch if set to leave
    ; tax                         ;initialize X to zero
    ; ldy Player_SprDataOffset    ;get player sprite data offset
    ; lda PlayerFacingDir         ;get player's facing direction
    ; lsr
    ; bcs SwimKT                  ;if player facing to the right, use current offset
    ;   iny
    ;   iny                         ;otherwise move to next OAM data
    ;   iny
    ;   iny
; SwimKT:
;     lda PlayerSize              ;check player's size
;     beq BigKTS                  ;if big, use first tile
;       lda Sprite_Tilenumber+24,y  ;check tile number of seventh/eighth sprite
;       cmp SwimTileRepOffset       ;against tile number in player graphics table
;       beq ExPGH                   ;if spr7/spr8 tile number = value, branch to leave
;         inx                         ;otherwise increment X for second tile
; BigKTS:
;     lda SwimKickTileNum,x       ;overwrite tile number in sprite 7/8
;     sta Sprite_Tilenumber+24,y  ;to animate player's feet when swimming
ExPGH:
  rts                         ;then leave

FindPlayerAction:
  ; jsr ProcessPlayerAction       ;find proper offset to graphics table by player's actions
  jsr ProcessPlayerAngle

  ; do something with the angle?
  lda #0
  jmp PlayerGfxProcessing       ;draw player, then process for fireball throwing

DoChangeSize:
  jsr HandleChangeSize          ;find proper offset to graphics table for grow/shrink
  jmp PlayerGfxProcessing       ;draw player, then process for fireball throwing

PlayerKilled:
  ; ldy #PlayerKilledGraphicsOffset ;load offset for player killed
  ; lda PlayerGfxTblOffsets,y     ;get offset to graphics table
  lda #PlayerKilledGraphicsOffset

PlayerGfxProcessing:
  sta PlayerGfxOffset           ;store offset to graphics table here
  ; lda #$04
  jsr RenderPlayerSub           ;draw player based on offset loaded
  ; jsr ChkForPlayerAttrib        ;set horizontal flip bits as necessary
  lda FireballThrowingTimer
  beq PlayerOffscreenChk        ;if fireball throw timer not set, skip to the end
  ldy #$00                      ;set value to initialize by default
  lda PlayerAnimTimer           ;get animation frame timer
  cmp FireballThrowingTimer     ;compare to fireball throw timer
  sty FireballThrowingTimer     ;initialize fireball throw timer
  bcs PlayerOffscreenChk        ;if animation frame timer => fireball throw timer skip to end
  sta FireballThrowingTimer     ;otherwise store animation timer into fireball throw timer
  ; ldy #$07                      ;load offset for throwing
  ; lda PlayerGfxTblOffsets,y     ;get offset to graphics table
  ; sta PlayerGfxOffset           ;store it for use later
  ; ldy #$04                      ;set to update four sprite rows by default
  ; lda Player_X_Speed
  ; ora Left_Right_Buttons        ;check for horizontal speed or left/right button press
  ; beq SUpdR                     ;if no speed or button press, branch using set value in Y
  ;   dey                         ;otherwise set to update only three sprite rows
SUpdR:
  tya                           ;save in A for use
  jsr RenderPlayerSub           ;in sub, draw player object again
PlayerOffscreenChk:
  lda Player_OffscreenBits      ;get player's offscreen bits
  lsr
  lsr                           ;move vertical bits to low nybble
  lsr
  lsr
  sta R0                       ;store here
  ; ldx #$03                      ;check all four rows of player sprites
  ldx PlayerSize
  lda PlayerSizeToOffset,x      ;get player's sprite data offset
  ; clc
  ; adc #$18                      ;add 24 bytes to start at bottom row
  ; adc #16
  tay                           ;set as offset here
  lda PlayerOffscreenRowLength,x
  sta R1
  lda PlayerSpritesPerRow,x
  tax
PROfsLoop:
    lsr R0                       ;shift bit into carry
    bcc NPROffscr                 ;if bit not set, skip, do not move sprites
    lda PlayerSize
    bne DumpOnlyThree
.import DumpFourSpr, DumpThreeSpr
      lda #$f8                      ;load offscreen Y coordinate just in case
      jsr DumpFourSpr                ;otherwise dump offscreen Y coordinate into sprite data
      bne NPROffscr
DumpOnlyThree:
      lda #$f8
      jsr DumpThreeSpr
NPROffscr:
    tya
    sec                           ;subtract eight bytes to do
    sbc R1                      ;next row up
    tay
    dex                           ;decrement row counter
    bpl PROfsLoop                 ;do this until all sprite rows are checked
  rts                             ;then we are done!
PlayerSizeToOffset:
  .byte 4*4*3, 4*3*2
PlayerOffscreenRowLength:
  .byte 4*4, 4*3
PlayerSpritesPerRow:
  .byte 4, 3
; PlayerGfxTblOffsets:
;   .byte $20, $28, $c8, $18, $00, $40, $50, $58
;   .byte $80, $88, $b8, $78, $60, $a0, $b0, $b8
SizeGraphicsOffsets:
  .byte $00, BigMarioGraphics - SmallMarioGraphics, BigMarioGraphics - SmallMarioGraphics


HandleChangeSize:
  ldy PlayerAnimCtrl           ;get animation frame control
  lda FrameCounter
  and #%00000011               ;get frame counter and execute this code every
  bne GorSLog                  ;fourth frame, otherwise branch ahead
    iny                          ;increment frame control
    cpy #$0a                     ;check for preset upper extent
    bcc CSzNext                  ;if not there yet, skip ahead to use
      ldy #$00                     ;otherwise initialize both grow/shrink flag
      sty PlayerChangeSizeFlag     ;and animation frame control
CSzNext:
    sty PlayerAnimCtrl           ;store proper frame control
GorSLog:
  lda PlayerSize               ;get player's size
  bne ShrinkPlayer             ;if player small, skip ahead to next part
    lda ChangeSizeOffsetAdder,y  ;get offset adder based on frame control as offset
    ; ldy #$0f                     ;load offset for player growing
    tay
    lda SizeGraphicsOffsets,y
; GetOffsetFromAnimCtrl:
;     asl                        ;multiply animation frame control
;     asl                        ;by eight to get proper amount
;     asl                        ;to add to our offset
;     adc PlayerGfxTblOffsets,y  ;add to offset to graphics table
    rts                        ;and return with result in A
ChangeSizeOffsetAdder:
  .byte $00, $01, $00, $01, $00, $01, $02, $00, $01, $02
  .byte $02, $00, $02, $00, $02, $00, $02, $00, $02, $00
ShrinkPlayer:
  tya                          ;add ten bytes to frame control as offset
  clc
  adc #$0a                     ;this thing apparently uses two of the swimming frames
  tax                          ;to draw the player shrinking
  ldy #$09                     ;load offset for small player swimming
  lda ChangeSizeOffsetAdder,x  ;get what would normally be offset adder
  bne ShrPlF                   ;and branch to use offset if nonzero
    ldy #$01                     ;otherwise load offset for big player swimming
ShrPlF:
  ; lda PlayerGfxTblOffsets,y    ;get offset to graphics table based on offset loaded
  rts                          ;and leave


;-------------------------------------------------------------------------------------

PlayerChangeSize:
  lda TimerControl    ;check master timer control
  cmp #$f8            ;for specific moment in time
  bne EndChgSize      ;branch if before or after that point
  jmp InitChangeSize  ;otherwise run code to get growing/shrinking going
EndChgSize:
  cmp #$c4            ;check again for another specific moment
  bne ExitChgSize     ;and branch to leave if before or after that point
  jmp DonePlayerTask  ;otherwise do sub to init timer control and set routine
ExitChgSize:
  rts ; TODO check this RTS can be removed                 ;and then leave

;-------------------------------------------------------------------------------------

PlayerInjuryBlink:
  lda TimerControl       ;check master timer control
  cmp #$f0               ;for specific moment in time
  bcs ExitBlink          ;branch if before that point
  cmp #$c8               ;check again for another specific point
  beq DonePlayerTask     ;branch if at that point, and not before or after
  jmp PlayerCtrlRoutine  ;otherwise run player control routine
ExitBlink:
  bne ExitBoth           ;do unconditional branch to leave

InitChangeSize:
  ldy PlayerChangeSizeFlag  ;if growing/shrinking flag already set
  bne ExitBoth              ;then branch to leave
  sty PlayerAnimCtrl        ;otherwise initialize player's animation frame control
  inc PlayerChangeSizeFlag  ;set growing/shrinking flag
  lda PlayerSize
  eor #$01                  ;invert player's size
  sta PlayerSize
ExitBoth:
  rts                       ;leave

;-------------------------------------------------------------------------------------
;$00 - used in CyclePlayerPalette to store current palette to cycle

PlayerDeath:
  lda TimerControl       ;check master timer control
  cmp #$f0               ;for specific moment in time
  bcs ExitTask           ;branch to leave if before that point
  jmp PlayerCtrlRoutine  ;otherwise run player control routine

DonePlayerTask:
  lda #$00
  sta TimerControl          ;initialize master timer control to continue timers
  lda #$08
  sta GameEngineSubroutine  ;set player control routine to run next frame
ExitTask:
  rts                       ;leave

CyclePlayerPalettePreload:
  lda R0
  jmp CyclePlayerPalette

PlayerFireFlower: 
  lda TimerControl       ;check master timer control
  cmp #$c0               ;for specific moment in time
  beq ResetPalFireFlower ;branch if at moment, not before or after
  lda FrameCounter       ;get frame counter
  lsr
  lsr                    ;divide by four to change every four frames

CyclePlayerPalette:
  and #$03              ;mask out all but d1-d0 (previously d3-d2)
  sta R0               ;store result here to use as palette bits
  lda Player_SprAttrib  ;get player attributes
  and #%11111100        ;save any other bits but palette bits
  ora R0               ;add palette bits
  sta Player_SprAttrib  ;store as new player attributes
  rts                   ;and leave

ResetPalFireFlower:
  jsr DonePlayerTask    ;do sub to init timer control and run player control routine

ResetPalStar:
  lda Player_SprAttrib  ;get player attributes
  and #%11111100        ;mask out palette bits to force palette 0
  sta Player_SprAttrib  ;store as new player attributes
  rts                   ;and leave

;-------------------------------------------------------------------------------------

FlagpoleSlide:
  ;  lda Enemy_ID+5           ;check special use enemy slot
  ;  cmp #FlagpoleFlagObject  ;for flagpole flag object
  ;  bne NoFPObj              ;if not found, branch to something residual
  lda FlagpoleSoundQueue   ;load flagpole sound
  sta Square1SoundQueue    ;into square 1's sfx queue
  lda #$00
  sta FlagpoleSoundQueue   ;init flagpole sound queue
  ldy Player_Y_Position
  cpy #$9e                 ;check to see if player has slid down
  bcs SlidePlayer          ;far enough, and if so, branch with no controller bits set
  lda #$04                 ;otherwise force player to climb down (to slide)
SlidePlayer:
  jmp AutoControlPlayer    ;jump to player control routine
; NoFPObj:     inc GameEngineSubroutine ;increment to next routine (this may
;              rts                      ;be residual code)

;-------------------------------------------------------------------------------------

Hidden1UpCoinAmts:
  .byte $15, $23, $16, $1b, $17, $18, $23, $63

PlayerEndLevel:
  lda #$01                  ;force player to walk to the right
  jsr AutoControlPlayer
  lda Player_Y_Position     ;check player's vertical position
  cmp #$ae
  bcc ChkStop               ;if player is not yet off the flagpole, skip this part
  lda ScrollLock            ;if scroll lock not set, branch ahead to next part
  beq ChkStop               ;because we only need to do this part once
  lda #EndOfLevelMusic
  sta EventMusicQueue       ;load win level music in event music queue
  lda #$00
  sta ScrollLock            ;turn off scroll lock to skip this part later
ChkStop:
  lda Player_CollisionBits  ;get player collision bits
  lsr                       ;check for d0 set
  bcs RdyNextA              ;if d0 set, skip to next part
  lda StarFlagTaskControl   ;if star flag task control already set,
  bne InCastle              ;go ahead with the rest of the code
  inc StarFlagTaskControl   ;otherwise set task control now (this gets ball rolling!)
InCastle:
  lda #%00100000            ;set player's background priority bit to
  sta Player_SprAttrib      ;give illusion of being inside the castle
RdyNextA:
  lda StarFlagTaskControl
  cmp #$05                  ;if star flag task control not yet set
  bne ExitNA                ;beyond last valid task number, branch to leave
  inc LevelNumber           ;increment level number used for game logic
  lda LevelNumber
  cmp #$03                  ;check to see if we have yet reached level -4
  bne NextArea              ;and skip this last part here if not
  ldy WorldNumber           ;get world number as offset
  lda CoinTallyFor1Ups      ;check third area coin tally for bonus 1-ups
  cmp Hidden1UpCoinAmts,y   ;against minimum value, if player has not collected
  bcc NextArea              ;at least this number of coins, leave flag clear
  inc Hidden1UpFlag         ;otherwise set hidden 1-up box control flag
NextArea:
  inc AreaNumber            ;increment area number used for address loader
  jsr LoadAreaPointer       ;get new level pointer
  inc FetchNewGameTimerFlag ;set flag to load new game timer
  jsr ChgAreaMode           ;do sub to set secondary mode, disable screen and sprite 0
  sta HalfwayPage           ;reset halfway page to 0 (beginning)
  lda #Silence
  sta EventMusicQueue       ;silence music and leave
ExitNA:
  rts

;-------------------------------------------------------------------------------------

PlayerMovementSubs:
;   lda #$00                  ;set A to init crouch flag by default
;   ldy PlayerSize            ;is player small?
;   bne SetCrouch             ;if so, branch
;   lda Player_State          ;check state of player
;   bne ProcMove              ;if not on the ground, branch
;   lda Up_Down_Buttons       ;load controller bits for up and down
;   and #%00000100            ;single out bit for down button
; SetCrouch:
;   sta CrouchingFlag         ;store value in crouch flag
ProcMove:
  jsr PlayerPhysicsSub      ;run sub related to jumping and swimming
  lda PlayerChangeSizeFlag  ;if growing/shrinking flag set,
  bne NoMoveSub             ;branch to leave
  lda Player_State
  cmp #$03                  ;get player state
  beq MoveSubs              ;if climbing, branch ahead, leave timer unset
  ldy #$18
  sty ClimbSideTimer        ;otherwise reset timer now
MoveSubs:
  jsr JumpEngine
.word OnGroundStateSub
.word JumpSwimSub
.word FallingSub
.word ClimbingSub
.word InSlingshotSub

NoMoveSub: rts

;-------------------------------------------------------------------------------------
;$00 - used by ClimbingSub to store high vertical adder

OnGroundStateSub:
  ; jsr GetPlayerAnimSpeed     ;do a sub to set animation frame timing
  ; lda Left_Right_Buttons
  ; beq GndMove                ;if left/right controller bits not set, skip instruction
    ; sta PlayerFacingDir        ;otherwise set new facing direction
; GndMove:
  lda #0
  jsr ImposeFriction         ;do a sub to impose friction on player's walk/run
  jsr MovePlayerHorizontally ;do another sub to move player horizontally
  sta Player_X_Scroll        ;set returned value as player's movement speed for scroll
  rts

;--------------------------------

FallingSub:
  lda VerticalForceDown
  sta VerticalForce      ;dump vertical movement force for falling into main one
  jmp JSMove
  ; jmp LRAir              ;movement force, then skip ahead to process left/right movement

;--------------------------------

JumpSwimSub:
  ldy Player_Y_Speed         ;if player's vertical speed zero
  bpl DumpFall               ;or moving downwards, branch to falling
  lda A_B_Buttons
  and #A_Button              ;check to see if A button is being pressed
  and PreviousA_B_Buttons    ;and was pressed in previous frame
  bne ProcSwim               ;if so, branch elsewhere
  lda JumpOrigin_Y_Position  ;get vertical position player jumped from
  sec
  sbc Player_Y_Position      ;subtract current from original vertical coordinate
  cmp DiffToHaltJump         ;compare to value set here to see if player is in mid-jump
  bcc ProcSwim               ;or just starting to jump, if just starting, skip ahead
DumpFall:
  lda VerticalForceDown      ;otherwise dump falling into main fractional
  sta VerticalForce
ProcSwim:
  ; lda SwimmingFlag           ;if swimming flag not set,
  ; beq LRAir                  ;branch ahead to last part
  ; jsr GetPlayerAnimSpeed     ;do a sub to get animation frame timing
  ; lda Player_Y_Position
  ; cmp #$14                   ;check vertical position against preset value
  ; bcs LRWater                ;if not yet reached a certain position, branch ahead
  lda #$18
  sta VerticalForce          ;otherwise set fractional
; LRWater:
;   lda Left_Right_Buttons     ;check left/right controller bits (check for swimming)
;   beq LRAir                  ;if not pressing any, skip
;     sta PlayerFacingDir        ;otherwise set facing direction accordingly
; LRAir:
;   lda Left_Right_Buttons     ;check left/right controller bits (check for jumping/falling)
;   beq JSMove                 ;if not pressing any, skip
;     jsr ImposeFriction         ;otherwise process horizontal movement
JSMove:
  jsr MovePlayerHorizontally ;do a sub to move player horizontally
  sta Player_X_Scroll        ;set player's speed here, to be used for scroll later
  lda GameEngineSubroutine
  cmp #$0b                   ;check for specific routine selected
  bne ExitMov1               ;branch if not set to run
    lda #$28
    sta VerticalForce          ;otherwise set fractional
ExitMov1:
  jmp MovePlayerVertically   ;jump to move player vertically, then leave

;--------------------------------

ClimbAdderLow:
  .byte $0e, $04, $fc, $f2
ClimbAdderHigh:
  .byte $00, $00, $ff, $ff

ClimbingSub:
             lda Player_YMoveForceFractional
             clc                      ;add movement force to dummy variable
             adc Player_Y_MoveForce   ;save with carry
             sta Player_YMoveForceFractional
             ldy #$00                 ;set default adder here
             lda Player_Y_Speed       ;get player's vertical speed
             bpl MoveOnVine           ;if not moving upwards, branch
             dey                      ;otherwise set adder to $ff
MoveOnVine:  sty R0                  ;store adder here
             adc Player_Y_Position    ;add carry to player's vertical position
             sta Player_Y_Position    ;and store to move player up or down
             lda Player_Y_HighPos
             adc R0                  ;add carry to player's page location
             sta Player_Y_HighPos     ;and store
             lda Left_Right_Buttons   ;compare left/right controller bits
             and Player_CollisionBits ;to collision flag
             beq InitCSTimer          ;if not set, skip to end
             ldy ClimbSideTimer       ;otherwise check timer 
             bne ExitCSub             ;if timer not expired, branch to leave
             ldy #$18
             sty ClimbSideTimer       ;otherwise set timer now
             ldx #$00                 ;set default offset here
             ldy PlayerFacingDir      ;get facing direction
             lsr                      ;move right button controller bit to carry
             bcs ClimbFD              ;if controller right pressed, branch ahead
             inx
             inx                      ;otherwise increment offset by 2 bytes
ClimbFD:     dey                      ;check to see if facing right
             beq CSetFDir             ;if so, branch, do not increment
             inx                      ;otherwise increment by 1 byte
CSetFDir:    lda Player_X_Position
             clc                      ;add or subtract from player's horizontal position
             adc ClimbAdderLow,x      ;using value here as adder and X as offset
             sta Player_X_Position
             lda Player_PageLoc       ;add or subtract carry or borrow using value here
             adc ClimbAdderHigh,x     ;from the player's page location
             sta Player_PageLoc
             lda Left_Right_Buttons   ;get left/right controller bits again
             eor #%00000011           ;invert them and store them while player
             sta PlayerFacingDir      ;is on vine to face player in opposite direction
ExitCSub:    rts                      ;then leave
InitCSTimer: sta ClimbSideTimer       ;initialize timer here
             rts

.proc InSlingshotSub
  lda #100
  sta Sprite_X_Position+62*4
  lda #100
  sta Sprite_Y_Position+62*4
  lda #$74
  sta Sprite_Tilenumber+62*4
  lda #0
  sta Sprite_Attributes+62*4
  lda SlingPull_Rel_XPos
  sta Sprite_X_Position+63*4
  lda SlingPull_Rel_YPos
  sta Sprite_Y_Position+63*4
  lda #$74
  sta Sprite_Tilenumber+63*4
  lda #0
  sta Sprite_Attributes+63*4
  rts
.endproc

;-------------------------------------------------------------------------------------
;$00 - used to store offset to friction data

JumpMForceData:
      .byte $20, $20, $1e, $28, $28, $0d, $04

FallMForceData:
      .byte $70, $70, $60, $90, $90, $0a, $09

PlayerYSpdData:
      .byte $fc, $fc, $fc, $fb, $fb, $fe, $ff

InitMForceData:
      .byte $00, $00, $00, $00, $00, $80, $00

MaxLeftXSpdData:
      .byte $d8, $e8, $f0

MaxRightXSpdData:
      .byte $28, $18, $10
      .byte $0c ;used for pipe intros

FrictionData:
      .byte $e4, $98, $d0

Climb_Y_SpeedData:
      .byte $00, $ff, $01

Climb_Y_MForceData:
      .byte $00, $20, $ff

PlayerPhysicsSub:
  lda Player_State          ;check player state
  cmp #PlayerState::Climbing
  bne CheckForSlingShot       ;if not climbing, branch
    ldy #$00
    lda Up_Down_Buttons       ;get controller bits for up/down
    and Player_CollisionBits  ;check against player's collision detection bits
    beq ProcClimb             ;if not pressing up or down, branch
      iny
      and #%00001000            ;check for pressing up
      bne ProcClimb
        iny
ProcClimb:
  ldx Climb_Y_MForceData,y  ;load value here
  stx Player_Y_MoveForce    ;store as vertical movement force
  lda #$08                  ;load default animation timing
  ldx Climb_Y_SpeedData,y   ;load some other value here
  stx Player_Y_Speed        ;store as vertical speed
  bmi SetCAnim              ;if climbing down, use default animation timing value
    lsr                       ;otherwise divide timer setting by 2
SetCAnim:
  sta PlayerAnimTimerSet    ;store animation timer setting and leave
  rts

.proc CheckForSlingShot
  ; If we are holding a slingshot, check to see if we are still holding it
  lda HoldingSlingshot
  bne SlingHold

  lda JumpspringAnimCtrl    ;if jumpspring animating, 
  bne NoSling          ;skip ahead to something else
    ; now check to see if we are starting a slingshot
    lda A_B_Buttons           ;check for A button press
    and #A_Button
    beq NoSling                ;if not, branch to something else
      and PreviousA_B_Buttons   ;if button not pressed in previous frame, branch
      beq StartSling
NoSling:
  jmp X_Physics             ;otherwise, jump to something else

StartSling:
  lda Player_State           ;check player state
  beq InitSling                 ;if on the ground, branch
    lda SwimmingFlag           ;if swimming flag not set, jump to do something else
    beq NoSling                     ;to prevent midair jumping, otherwise continue
      lda JumpSwimTimer          ;if jump/swim timer nonzero, branch
      bne InitSling
        lda Player_Y_Speed         ;check player's vertical speed
        bmi NoSling                 ;if player's vertical speed motionless or down, branch
  ; jmp X_Physics              ;if timer at zero and player still rising, do not swim
InitSling:
  ; Initialize the draw string to a fixed position in the center of the screen
  ; this is only used for calculating angle and velocity, not used for drawing
  lda #SLING_INIT_POS
  sta SlingPull_Rel_XPos
  sta SlingPull_Rel_YPos

  lda #0
  sta AngularMomentum
  sta Player_X_Speed
  sta Player_X_MoveForce
  sta Player_Y_Speed
  sta Player_Y_MoveForce

  ; and set the player to slingshot state
  lda A_B_Buttons
  sta HoldingSlingshot
  rts
.endproc

.proc SlingHold
  ;; Check to see if we let go of the a or b button
  lda A_B_Buttons
  and HoldingSlingshot
  bne StillHolding
    ; apply force and cancel out of slingshot mode.
    ; mulitply the magnitude by 1.5 to make it the x speed
    lda X_Magnitude
    bpl :+
      eor #$ff
      clc
      adc #1
      lsr
      jmp Subtract
:
      lsr
      eor #$ff
      clc
      adc #1
Subtract:
    sec
    sbc X_Magnitude
    sta Player_X_Speed
    lda Y_Magnitude
    tax
    lda Y_Sling_Speed, x
    sta Player_Y_Speed
    lda Y_Sling_Speed_Fractional, x
    sta Player_Y_MoveForce
    lda #0
    sta HoldingSlingshot
    ; Initialize the ground bounce chain to zero to signal that this is the first
    sta GroundBounceChain
    lda #SFX_Jump
    sta DpcmSampleQueue
    lda #PlayerState::Jumping
    sta Player_State
    ; Start the airtime counter to track how long mario was in the air
    ; before bouncing.
    lda #$ff
    sta AirTimeTimer
    ; set the initial angular momentum based on the facing direction
    ldy #1
    lda X_Magnitude
    bmi :+ 
      ldy #-1
    :
    sty AngularMomentum
QuickExit:
    rts
StillHolding:
  lda #PlayerState::Slingshot
  sta Player_State
  lda SavedJoypadBits
  and #%00001111
  beq QuickExit

.MACPACK generic
.MACPACK longbranch
  ; first things first check to see if we are at the maximum length, ie the
  ; current sling position is exactly the sin/cos value of the current angle
  ; (the angle before applying movement from input this frame)
  ldy PlayerAngle
  lda SinTable, y
  bpl SkipFlipY
    eor #$ff
    clc
    adc #1
SkipFlipY:
  cmp Abs_Y_Magnitude
  ble @Skip
    jmp RegularControl
@Skip: 
  lda CosTable, y
  bpl SkipFlipX
    eor #$ff
    clc
    adc #1
SkipFlipX:
  cmp Abs_X_Magnitude
  ble @Skip
    jmp RegularControl
@Skip:
  ; The player has reached the circle limit, so first try to use the movement
  ; around the circle script. In laymans terms, if we are holding left on a left
  ; quadrant, then attempt to move along the circle edge by just changing the angle
  ; and using the next position from the sin/cos table

  ; Frame counter is used to alternate speed around the cirlce to
  ; limit movement to an average of 1.5 speed per frame
  lda FrameCounter
  and #1
  asl
  asl
  asl
  sta R2

  lda PlayerAngle
  lsr
  lsr
  lsr
  lsr
  lsr
  tax
  ora R2
  tay
  lda SavedJoypadBits
  and QuadrantDisabled, x
  ; If its not, that means we need to fall back to regular movement,
  ; since we pushed a direction that falls off the normal movement edge
  jne RegularControl

  ; Now check to see if we are in the stable angle range, meaning
  ; the two angles around the edges of the quadrant.
  ; ie: if holding left, and the angle is near zero, clamp to zero.

  lda PlayerAngle
  clc
  adc #2
  sta R2
  and #%00011111
  cmp #5
  bge NotAtEdge
    ; We are at an edge, now check to see if we should clamp
    stx R3
    ; Load the angle + 2 from earlier so we can check this quadrant instead
    lda R2
    lsr
    lsr
    lsr
    lsr
    lsr
    tax
    lda SavedJoypadBits
    and #%00001111
    cmp QuadrantStableDirection,x
    bne NotTheCorrectEdge
      ; At this point, the person is holding the direction that would cause
      ; it to clamp, so lets just clamp the angle and call it done.
      lda R2
      and #%11100000
      jmp UpdatePosition
NotTheCorrectEdge:
    ; restore the original quadrant and continue processing
    ldx R3
NotAtEdge:
  ; At this point, we are pushing buttons that influences the current quadrant
  ; so now we need to figure out which buttons pressed to change the angle
  lda SavedJoypadBits
  ; and #%00001111
  and QuadrantPrimaryDirection, x
  beq OtherDirection
    lda QuadrantPrimaryDiff, y
    bne AddAngle ; unconditional
OtherDirection:
  lda QuadrantPrimaryDiff, y
  eor #$ff
  clc
  adc #1
AddAngle:
  clc
  adc PlayerAngle
UpdatePosition:
  sta PlayerAngle
  tay
  lda CosTable, y
  clc
  adc #SLING_INIT_POS
  sta SlingPull_Rel_XPos
  lda SinTable, y
  clc
  adc #SLING_INIT_POS
  sta SlingPull_Rel_YPos
Done:
  rts

; If the current input masked with this is not zero, then we fallback
; to the regular controller input scheme
QuadrantDisabled:
  .byte Down_Dir | Right_Dir
  .byte Down_Dir | Right_Dir
  .byte Down_Dir | Left_Dir
  .byte Down_Dir | Left_Dir
  .byte Up_Dir | Left_Dir
  .byte Up_Dir | Left_Dir
  .byte Up_Dir | Right_Dir
  .byte Up_Dir | Right_Dir

; When deciding to increment or decrement the player angle using the control
; scheme there always 3 different input combinations that the player can use
; to move it, two of them go in one direction, and the other goes in the opposite
; This is a table that determines the "two" direction input
QuadrantPrimaryDirection:
  .byte Up_Dir
  .byte Left_Dir
  .byte Right_Dir
  .byte Up_Dir
  .byte Down_Dir
  .byte Right_Dir
  .byte Left_Dir
  .byte Down_Dir

QuadrantPrimaryDiff:
  ; .byte 1, -1, 1, -1, 1, -1, 1, -1
  .byte 2, -2, 2, -2, 2, -2, 2, -2
  .byte 3, -3, 3, -3, 3, -3, 3, -3
  ; .byte 2, -2, 2, -2, 2, -2, 2, -2

QuadrantStableDirection:
  .byte Left_Dir
  .byte Up_Dir | Left_Dir
  .byte Up_Dir
  .byte Up_Dir | Right_Dir
  .byte Right_Dir
  .byte Down_Dir | Right_Dir
  .byte Down_Dir
  .byte Down_Dir | Left_Dir

Y_Sling_Speed:
  .incbin "slingcurve.bin"
Y_Sling_Speed_Fractional:
  .incbin "slingfractional.bin"
.endproc

; Regular movement script modifies the x/y coordinates of the sling directly
; instead of editing the angle
.proc RegularControl
  ; FrameCounter is used to make diagonal movement use 1.5px/frame instead of 2
  lda FrameCounter
  and #1
  asl
  asl
  asl
  asl
  sta R2

  lda SavedJoypadBits
  and #%00001111
  ora R2
  tay

  lda HorizontalMovementTable, y
  clc
  adc SlingPull_Rel_XPos
  sta SlingPull_Rel_XPos

  lda VerticalMovementTable, y
  clc
  adc SlingPull_Rel_YPos
  sta SlingPull_Rel_YPos

  ; find the current angle with arctan
  jsr FastAtan2
  sta PlayerAngle
  tay
  lda SinTable, y
  bpl SkipFlipY
    eor #$ff
    clc
    adc #1
SkipFlipY:
  cmp Abs_Y_Magnitude
  bcs SkipClampY
    lda SinTable, y
    clc
    adc #SLING_INIT_POS
    sta SlingPull_Rel_YPos
SkipClampY:
  lda CosTable, y
  bpl SkipFlipX
    eor #$ff
    clc
    adc #1
SkipFlipX:
  cmp Abs_X_Magnitude
  bcs SkipClampX
    lda CosTable, y
    clc
    adc #SLING_INIT_POS
    sta SlingPull_Rel_XPos
SkipClampX:
  rts

QuadrantTable:
  ;     0    R     L    R/L
  .byte $00, $01, -$01, $00
  ;     D    D/R   D/L  D/R/L
  .byte $00, $01, $00, $00
  ;     U    U/R   U/L  U/R/L
  .byte -$01, $01, -$01, $00
  ;     U/D  U/D/R U/D/L U/D/R/L
  .byte $00, $01, -$01, $00

HorizontalMovementTable:
  ;     0    R     L    R/L
  .byte $00, $02, -$02, $00
  ;     D    D/R   D/L  D/R/L
  .byte $00, $01, -$01, $00
  ;     U    U/R   U/L  U/R/L
  .byte $00, $01, -$01, $00
  ;     U/D  U/D/R U/D/L U/D/R/L
  .byte $00, $01, -$01, $00
HorizontalMovementTableAlt:
  ;     0    R     L    R/L
  .byte $00, $02, -$02, $00
  ;     D    D/R   D/L  D/R/L
  .byte $00, $02, -$02, $00
  ;     U    U/R   U/L  U/R/L
  .byte $00, $02, -$02, $00
  ;     U/D  U/D/R U/D/L U/D/R/L
  .byte $00, $02, -$02, $00
VerticalMovementTable:
  ;     0    R     L    R/L
  .byte $00, $00, $00, $00
  ;     D    D/R   D/L  D/R/L
  .byte $02, $01, $01, $02
  ;     U    U/R   U/L  U/R/L
  .byte -$02, -$01, -$01, -$02
  ;     U/D  U/D/R U/D/L U/D/R/L
  .byte $00, $00, $00, $00
VerticalMovementTableAlt:
  ;     0    R     L    R/L
  .byte $00, $00, $00, $00
  ;     D    D/R   D/L  D/R/L
  .byte $02, $02, $02, $02
  ;     U    U/R   U/L  U/R/L
  .byte -$02, -$02, -$02, -$02
  ;     U/D  U/D/R U/D/L U/D/R/L
  .byte $00, $00, $00, $00

;   lda #$20                   ;set jump/swim timer
;   sta JumpSwimTimer
;   ldy #$00                   ;initialize vertical force and dummy variable
;   sty Player_YMoveForceFractional
;   sty Player_Y_MoveForce
;   lda Player_Y_HighPos       ;get vertical high and low bytes of jump origin
;   sta JumpOrigin_Y_HighPos   ;and store them next to each other here
;   lda Player_Y_Position
;   sta JumpOrigin_Y_Position
  ; lda #$04                   ;set player state to jumping/swimming
  ; lda #PlayerState::Slingshot
  ; sta Player_State
;   lda Player_XSpeedAbsolute  ;check value related to walking/running speed
;   cmp #$09
;   bcc ChkWtr                 ;branch if below certain values, increment Y
;     iny                        ;for each amount equal or exceeded
;     cmp #$10
;     bcc ChkWtr
;       iny
;       cmp #$19
;       bcc ChkWtr
;         iny
;         cmp #$1c
;         bcc ChkWtr                 ;note that for jumping, range is 0-4 for Y
;           iny
; ChkWtr:
;   lda #$01                   ;set value here (apparently always set to 1)
;   sta DiffToHaltJump
;   lda SwimmingFlag           ;if swimming flag disabled, branch
;   beq GetYPhy
;     ldy #$05                   ;otherwise set Y to 5, range is 5-6
;     lda Whirlpool_Flag         ;if whirlpool flag not set, branch
;     beq GetYPhy
;       iny                        ;otherwise increment to 6
; GetYPhy:
;   lda JumpMForceData,y       ;store appropriate jump/swim
;   sta VerticalForce          ;data here
;   lda FallMForceData,y
;   sta VerticalForceDown
;   lda InitMForceData,y
;   sta Player_Y_MoveForce
;   lda PlayerYSpdData,y
;   sta Player_Y_Speed
;   lda SwimmingFlag           ;if swimming flag disabled, branch
;   beq PJumpSnd
;     lda #Sfx_EnemyStomp        ;load swim/goomba stomp sound into
;     sta Square1SoundQueue      ;square 1's sfx queue
;     lda Player_Y_Position
;     cmp #$14                   ;check vertical low byte of player position
;     bcs X_Physics              ;if below a certain point, branch
;       lda #$00                   ;otherwise reset player's vertical speed
;       sta Player_Y_Speed         ;and jump to something else to keep player
;       jmp X_Physics              ;from swimming above water level
; PJumpSnd:
; ;   lda #Sfx_BigJump           ;load big mario's jump sound by default
; ;   ldy PlayerSize             ;is mario big?
; ;   beq SJumpSnd
; ;   lda #Sfx_SmallJump         ;if not, load small mario's jump sound
; ; SJumpSnd:
; ;   sta Square1SoundQueue      ;store appropriate jump sound in square 1 sfx queue
.endproc

.proc X_Physics
  ldy #$00
  sty R0                    ;init value here
  ; lda Player_State           ;if mario is on the ground, branch
;   beq ProcPRun
;     lda Player_XSpeedAbsolute  ;check something that seems to be related
;     cmp #$19                   ;to mario's speed
;     bcs GetXPhy                ;if =>$19 branch here
;     bcc ChkRFast               ;if not branch elsewhere
; ProcPRun:
;   iny                        ;if mario on the ground, increment Y
;   lda AreaType               ;check area type
;   beq GetXPhy
;   ; beq ChkRFast               ;if water type, branch
;     dey                        ;decrement Y by default for non-water type area
;     lda Left_Right_Buttons     ;get left/right controller bits
;     cmp Player_MovingDir       ;check against moving direction
;     bne ChkRFast               ;if controller bits <> moving direction, skip this part
;       lda A_B_Buttons            ;check for b button pressed
;       and #B_Button
;       bne SetRTmr                ;if pressed, skip ahead to set timer
;         lda RunningTimer           ;check for running timer set
;         bne GetXPhy                ;if set, branch
; ChkRFast:
;   iny                        ;if running timer not set or level type is water, 
;   inc R0                    ;increment Y again and temp variable in memory
;   lda RunningSpeed
;   bne FastXSp                ;if running speed set here, branch
;     lda Player_XSpeedAbsolute
;     cmp #$21                   ;otherwise check player's walking/running speed
;     bcc GetXPhy                ;if less than a certain amount, branch ahead
; FastXSp:
;       inc R0                    ;if running speed set or speed => $21 increment $00
;       jmp GetXPhy                ;and jump ahead
; SetRTmr:
;   lda #$0a                   ;if b button pressed, set running timer
;   sta RunningTimer
GetXPhy:
  lda MaxLeftXSpdData,y      ;get maximum speed to the left
  sta MaximumLeftSpeed
  lda GameEngineSubroutine   ;check for specific routine running
  cmp #$07                   ;(player entrance)
  bne GetXPhy2               ;if not running, skip and use old value of Y
    ldy #$03                   ;otherwise set Y to 3
GetXPhy2:
  lda MaxRightXSpdData,y     ;get maximum speed to the right
  sta MaximumRightSpeed
  ldy R0                    ;get other value in memory
  lda FrictionData,y         ;get value using value in memory as offset
  sta FrictionAdderLow
  lda #$00
  sta FrictionAdderHigh      ;init something here
  lda PlayerFacingDir
  cmp Player_MovingDir       ;check facing direction against moving direction
  beq ExitPhy                ;if the same, branch to leave
    asl FrictionAdderLow       ;otherwise multiply friction by 2
    rol FrictionAdderHigh      ;then leave
ExitPhy:
  rts

; -------------------------------------------------------------------------------------

; PlayerAnimTmrData:
;       .byte $02, $04, $07

; GetPlayerAnimSpeed:
;   ldy #$00                   ;initialize offset in Y
;   lda Player_XSpeedAbsolute  ;check player's walking/running speed
;   cmp #$1c                   ;against preset amount
;   bcs SetRunSpd              ;if greater than a certain amount, branch ahead
;   iny                        ;otherwise increment Y
;   cmp #$0e                   ;compare against lower amount
;   bcs ChkSkid                ;if greater than this but not greater than first, skip increment
;   iny                        ;otherwise increment Y again
; ChkSkid:
;   lda SavedJoypadBits        ;get controller bits
;   and #%01111111             ;mask out A button
;   beq SetAnimSpd             ;if no other buttons pressed, branch ahead of all this
;     and #$03                   ;mask out all others except left and right
;     cmp Player_MovingDir       ;check against moving direction
;     bne ProcSkid               ;if left/right controller bits <> moving direction, branch
;       lda #$00                   ;otherwise set zero value here
; SetRunSpd:
;   sta RunningSpeed           ;store zero or running speed here
;   jmp SetAnimSpd
; ProcSkid:
;   lda Player_XSpeedAbsolute  ;check player's walking/running speed
;   cmp #$0b                   ;against one last amount
;   bcs SetAnimSpd             ;if greater than this amount, branch
;     lda PlayerFacingDir
;     sta Player_MovingDir       ;otherwise use facing direction to set moving direction
;     lda #$00
;     sta Player_X_Speed         ;nullify player's horizontal speed
;     sta Player_X_MoveForce     ;and dummy variable for player
; SetAnimSpd:
;   lda PlayerAnimTmrData,y    ;get animation timer setting using Y as offset
;   sta PlayerAnimTimerSet
;   rts
.endproc

;-------------------------------------------------------------------------------------

ImposeFriction:
  and Player_CollisionBits  ;perform AND between left/right controller bits and collision flag
  ; cmp #$00                  ;then compare to zero (this instruction is redundant)
  bne JoypFrict             ;if any bits set, branch to next part
    lda Player_X_Speed
    beq SetAbsSpd             ;if player has no horizontal speed, branch ahead to last part
    bpl RghtFrict             ;if player moving to the right, branch to slow
    bmi LeftFrict             ;otherwise logic dictates player moving left, branch to slow
JoypFrict:
  lsr                       ;put right controller bit into carry
  bcc RghtFrict             ;if left button pressed, carry = 0, thus branch
LeftFrict:
    lda Player_X_MoveForce    ;load value set here
    clc
    adc FrictionAdderLow      ;add to it another value set here
    sta Player_X_MoveForce    ;store here
    lda Player_X_Speed
    adc FrictionAdderHigh     ;add value plus carry to horizontal speed
    sta Player_X_Speed        ;set as new horizontal speed
    cmp MaximumRightSpeed     ;compare against maximum value for right movement
    bmi XSpdSign              ;if horizontal speed greater negatively, branch
      lda MaximumRightSpeed     ;otherwise set preset value as horizontal speed
      sta Player_X_Speed        ;thus slowing the player's left movement down
      jmp SetAbsSpd             ;skip to the end
RghtFrict:
    lda Player_X_MoveForce    ;load value set here
    sec
    sbc FrictionAdderLow      ;subtract from it another value set here
    sta Player_X_MoveForce    ;store here
    lda Player_X_Speed
    sbc FrictionAdderHigh     ;subtract value plus borrow from horizontal speed
    sta Player_X_Speed        ;set as new horizontal speed
    cmp MaximumLeftSpeed      ;compare against maximum value for left movement
    bpl XSpdSign              ;if horizontal speed greater positively, branch
      lda MaximumLeftSpeed      ;otherwise set preset value as horizontal speed
      sta Player_X_Speed        ;thus slowing the player's right movement down
XSpdSign:
  cmp #$00                  ;if player not moving or moving to the right,
  bpl SetAbsSpd             ;branch and leave horizontal speed value unmodified
    eor #$ff
    clc                       ;otherwise get two's compliment to get absolute
    adc #$01                  ;unsigned walking/running speed
SetAbsSpd:
  sta Player_XSpeedAbsolute ;store walking/running speed here and leave
  rts

;-------------------------------------------------------------------------------------

ScrollHandler:
  lda Player_X_Scroll       ;load value saved here
  clc
  adc Platform_X_Scroll     ;add value used by left/right platforms
  sta Player_X_Scroll       ;save as new value here to impose force on scroll
  lda ScrollLock            ;check scroll lock flag
  bne InitScrlAmt           ;skip a bunch of code here if set
  lda Player_Pos_ForScroll
  cmp #$50                  ;check player's horizontal screen position
  bcc InitScrlAmt           ;if less than 80 pixels to the right, branch
  lda SideCollisionTimer    ;if timer related to player's side collision
  bne InitScrlAmt           ;not expired, branch
  ldy Player_X_Scroll       ;get value and decrement by one
  dey                       ;if value originally set to zero or otherwise
  bmi InitScrlAmt           ;negative for left movement, branch
  iny
  cpy #$02                  ;if value $01, branch and do not decrement
  bcc ChkNearMid
  dey                       ;otherwise decrement by one
ChkNearMid:
  lda Player_Pos_ForScroll
  cmp #$70                  ;check player's horizontal screen position
  bcc ScrollScreen          ;if less than 112 pixels to the right, branch
    ldy Player_X_Scroll       ;otherwise get original value undecremented
    ; fallthrough
ScrollScreen:
  tya
  sta ScrollAmount          ;save value here
  clc
  adc ScrollThirtyTwo       ;add to value already set here
  sta ScrollThirtyTwo       ;save as new value here
  tya
  clc
  adc ScreenLeft_X_Pos      ;add to left side coordinate
  sta ScreenLeft_X_Pos      ;save as new left side coordinate
  sta HorizontalScroll      ;save here also
  lda ScreenLeft_PageLoc
  adc #$00                  ;add carry to page location for left
  sta ScreenLeft_PageLoc    ;side of the screen
  and #$01                  ;get LSB of page location
  sta R0                   ;save as temp variable for PPU register 1 mirror
  lda Mirror_PPUCTRL       ;get PPU register 1 mirror
  and #%11111110            ;save all bits except d0
  ora R0                   ;get saved bit here and save in PPU register 1
  sta Mirror_PPUCTRL       ;mirror to be used to set name table later
  jsr GetScreenPosition     ;figure out where the right side is
            ;   lda #$08
            ;   sta ScrollIntervalTimer   ;set scroll timer (residualremoved, not used elsewhere)
  jmp ChkPOffscr            ;skip this part
InitScrlAmt:
  lda #$00
  sta ScrollAmount          ;initialize value here
ChkPOffscr:
  ldx #$00                  ;set X for player offset
  jsr GetXOffscreenBits     ;get horizontal offscreen bits for player
  sta R0                   ;save them here
  ldy #$00                  ;load default offset (left side)
  asl                       ;if d7 of offscreen bits are set,
  bcs KeepOnscr             ;branch with default offset
    iny                         ;otherwise use different offset (right side)
    lda R0
    and #%00100000              ;check offscreen bits for d5 set
    beq InitPlatScrl            ;if not set, branch ahead of this part
KeepOnscr:
    lda ScreenEdge_X_Pos,y      ;get left or right side coordinate based on offset
    sec
    sbc X_SubtracterData,y      ;subtract amount based on offset
    sta Player_X_Position       ;store as player position to prevent movement further
    lda ScreenEdge_PageLoc,y    ;get left or right page location based on offset
    sbc #$00                    ;subtract borrow
    sta Player_PageLoc          ;save as player's page location
    lda Left_Right_Buttons      ;check saved controller bits
    cmp OffscrJoypadBitsData,y  ;against bits based on offset
    beq InitPlatScrl            ;if not equal, branch
      ; invert player speed here instead
      lda Player_X_Speed
      eor #$ff
      clc
      adc #1
      sta Player_X_Speed
      lda AngularMomentum
      eor #$ff
      clc
      adc #1
      sta AngularMomentum
      ; lda #$00
      ; sta Player_X_Speed          ;otherwise nullify horizontal speed of player
InitPlatScrl:
  lda #$00                    ;nullify platform force imposed on scroll
  sta Platform_X_Scroll
  rts

X_SubtracterData:
  .byte $00, $10

OffscrJoypadBitsData:
  .byte $01, $02

; ProcessPlayerAction:
;   lda Player_State      ;get player's state
;   cmp #$03
;   beq ActionClimbing    ;if climbing, branch here
;   cmp #$02
;   beq ActionFalling     ;if falling, branch here
;   cmp #$01
;   bne ProcOnGroundActs  ;if not jumping, branch here
;   lda SwimmingFlag
;   bne ActionSwimming    ;if swimming flag set, branch elsewhere
;   ldy #$06              ;load offset for crouching
;   lda CrouchingFlag     ;get crouching flag
;   bne NonAnimatedActs   ;if set, branch to get offset for graphics table
;   ldy #$00              ;otherwise load offset for jumping
;   jmp NonAnimatedActs   ;go to get offset to graphics table

; ProcOnGroundActs:
;   ldy #$06                   ;load offset for crouching
;   lda CrouchingFlag          ;get crouching flag
;   bne NonAnimatedActs        ;if set, branch to get offset for graphics table
;   ldy #$02                   ;load offset for standing
;   lda Player_X_Speed         ;check player's horizontal speed
;   ora Left_Right_Buttons     ;and left/right controller bits
;   beq NonAnimatedActs        ;if no speed or buttons pressed, use standing offset
;   lda Player_XSpeedAbsolute  ;load walking/running speed
;   cmp #$09
;   bcc ActionWalkRun          ;if less than a certain amount, branch, too slow to skid
;   lda Player_MovingDir       ;otherwise check to see if moving direction
;   and PlayerFacingDir        ;and facing direction are the same
;   bne ActionWalkRun          ;if moving direction = facing direction, branch, don't skid
;     iny                        ;otherwise increment to skid offset ($03)

; NonAnimatedActs:
;   jsr GetGfxOffsetAdder      ;do a sub here to get offset adder for graphics table
;   lda #$00
;   sta PlayerAnimCtrl         ;initialize animation frame control
;   lda PlayerGfxTblOffsets,y  ;load offset to graphics table using size as offset
;   rts

; ActionFalling:
;   ldy #$04                  ;load offset for walking/running
;   jsr GetGfxOffsetAdder     ;get offset to graphics table
;   jmp GetCurrentAnimOffset  ;execute instructions for falling state

; ActionWalkRun:
;   ldy #$04               ;load offset for walking/running
;   jsr GetGfxOffsetAdder  ;get offset to graphics table
;   jmp FourFrameExtent    ;execute instructions for normal state

; ActionClimbing:
;   ldy #$05               ;load offset for climbing
;   lda Player_Y_Speed     ;check player's vertical speed
;   beq NonAnimatedActs    ;if no speed, branch, use offset as-is
;   jsr GetGfxOffsetAdder  ;otherwise get offset for graphics table
;   jmp ThreeFrameExtent   ;then skip ahead to more code

; ActionSwimming:
;   ldy #$01               ;load offset for swimming
;   jsr GetGfxOffsetAdder
;   lda JumpSwimTimer      ;check jump/swim timer
;   ora PlayerAnimCtrl     ;and animation frame control
;   bne FourFrameExtent    ;if any one of these set, branch ahead
;   lda A_B_Buttons
;   asl                    ;check for A button pressed
;   bcs FourFrameExtent    ;branch to same place if A button pressed

; GetCurrentAnimOffset:
;   lda PlayerAnimCtrl         ;get animation frame control
;   jmp GetOffsetFromAnimCtrl  ;jump to get proper offset to graphics table

; FourFrameExtent:
;   lda #$03              ;load upper extent for frame control
;   jmp AnimationControl  ;jump to get offset and animate player object

; ThreeFrameExtent:
;   lda #$02              ;load upper extent for frame control for climbing

; AnimationControl:
;   sta R0                   ;store upper extent here
;   jsr GetCurrentAnimOffset  ;get proper offset to graphics table
;   pha                       ;save offset to stack
;     lda PlayerAnimTimer       ;load animation frame timer
;     bne ExAnimC               ;branch if not expired
;       lda PlayerAnimTimerSet    ;get animation frame timer amount
;       sta PlayerAnimTimer       ;and set timer accordingly
;       lda PlayerAnimCtrl
;       clc                       ;add one to animation frame control
;       adc #$01
;       cmp R0                   ;compare to upper extent
;       bcc SetAnimC              ;if frame control + 1 < upper extent, use as next
;         lda #$00                  ;otherwise initialize frame control
; SetAnimC:
;     sta PlayerAnimCtrl        ;store as new animation frame control
; ExAnimC:
;   pla                       ;get offset to graphics table from stack and leave
;   rts

; GetGfxOffsetAdder:
;   lda PlayerSize  ;get player's size
;   beq SzOfs       ;if player big, use current offset as-is
;   tya             ;for big player
;   clc             ;otherwise add eight bytes to offset
;   adc #$08        ;for small player
;   tay
; SzOfs:
;   rts             ;go back

; ChkForPlayerAttrib:
;   ldy Player_SprDataOffset    ;get sprite data offset
;   lda GameEngineSubroutine
;   cmp #$0b                    ;if executing specific game engine routine,
;   beq KilledAtt               ;branch to change third and fourth row OAM attributes
;   lda PlayerGfxOffset         ;get graphics table offset
;   cmp #$50
;   beq C_S_IGAtt               ;if crouch offset, either standing offset,
;   cmp #$b8                    ;or intermediate growing offset,
;   beq C_S_IGAtt               ;go ahead and execute code to change 
;   cmp #$c0                    ;fourth row OAM attributes only
;   beq C_S_IGAtt
;   cmp #$c8
;   bne ExPlyrAt                ;if none of these, branch to leave
; KilledAtt:
;   lda Sprite_Attributes+16,y
;   and #%00111111              ;mask out horizontal and vertical flip bits
;   sta Sprite_Attributes+16,y  ;for third row sprites and save
;   lda Sprite_Attributes+20,y
;   and #%00111111  
;   ora #%01000000              ;set horizontal flip bit for second
;   sta Sprite_Attributes+20,y  ;sprite in the third row
; C_S_IGAtt:
;   lda Sprite_Attributes+24,y
;   and #%00111111              ;mask out horizontal and vertical flip bits
;   sta Sprite_Attributes+24,y  ;for fourth row sprites and save
;   lda Sprite_Attributes+28,y
;   and #%00111111
;   ora #%01000000              ;set horizontal flip bit for second
;   sta Sprite_Attributes+28,y  ;sprite in the fourth row
; ExPlyrAt:
;   rts                         ;leave

; ------------------------------------------------------------
ProcessPlayerAngle:
  ; jsr FastAtan2
  lda PlayerAngle
  lsr
  lsr
  tay
  BankCHR10 y
NearbyRTS:
  rts

;-------------------------------------------------------------------------------------
;$00 - used for downward force
;$01 - used for upward force
;$02 - used for maximum vertical speed

MovePlayerVertically:
  ldx #$00                ;set X for player offset
  lda TimerControl
  bne NoJSChk             ;if master timer control set, branch ahead
  lda JumpspringAnimCtrl  ;otherwise check to see if jumpspring is animating
  bne NearbyRTS             ;branch to leave if so
NoJSChk:
  lda VerticalForce       ;dump vertical force 
  sta R0
  lda #$04                ;set maximum vertical speed here
  jmp ImposeGravitySprObj ;then jump to move player vertically



;; Calculate the angle, in a 256-degree circle, between two points.
;; The trick is to use logarithmic division to get the y/x ratio and
;; integrate the power function into the atan table. Some branching is
;; avoided by using a table to adjust for the octants.
;; In otherwords nothing new or particularily clever but nevertheless
;; quite useful.
;;
;; by Johan Forslöf (doynax)
.proc FastAtan2
octant = R3        ;; temporary zeropage variable
x2 = Player_Rel_XPos
y2 = Player_Rel_YPos
x1 = SlingPull_Rel_XPos
y1 = SlingPull_Rel_YPos
  ; abssub x1, x2, X_Magnitude, Abs_X_Magnitude
  lda SlingPull_Rel_XPos
  sec
  sbc #SLING_INIT_POS
  sta X_Magnitude
  bcs @X
    eor #$ff
    adc #1
@X:
  sta Abs_X_Magnitude
  tax
  rol octant

  ; abssub y1, y2, Y_Magnitude, Abs_Y_Magnitude
  lda SlingPull_Rel_YPos
  sec
  sbc #SLING_INIT_POS
  sta Y_Magnitude
  bcs @Y
    eor #$ff
    adc #1
@Y:
  sta Abs_Y_Magnitude
  tay
  rol octant

  lda log2_tab,x
  ; sec
  sbc log2_tab,y
  bcc skipflip
    eor #$ff
    ; clc
    ; adc #1
skipflip:
  tax
  lda octant
  rol
  and #%111
  tay

  lda atan_tab,x
  eor octant_adjust,y
  rts

octant_adjust:
  .byte %00111111		;; x+,y+,|x|>|y|
  .byte %00000000		;; x+,y+,|x|<|y|
  .byte %11000000		;; x+,y-,|x|>|y|
  .byte %11111111		;; x+,y-,|x|<|y|
  .byte %01000000		;; x-,y+,|x|>|y|
  .byte %01111111		;; x-,y+,|x|<|y|
  .byte %10111111		;; x-,y-,|x|>|y|
  .byte %10000000		;; x-,y-,|x|<|y|

;;;;;;;; atan(2^(x/32))*128/pi ;;;;;;;;
atan_tab:
  .byte $00,$00,$00,$00,$00,$00,$00,$00
  .byte $00,$00,$00,$00,$00,$00,$00,$00
  .byte $00,$00,$00,$00,$00,$00,$00,$00
  .byte $00,$00,$00,$00,$00,$00,$00,$00
  .byte $00,$00,$00,$00,$00,$00,$00,$00
  .byte $00,$00,$00,$00,$00,$00,$00,$00
  .byte $00,$00,$00,$00,$00,$00,$00,$00
  .byte $00,$00,$00,$00,$00,$00,$00,$00
  .byte $00,$00,$00,$00,$00,$00,$00,$00
  .byte $00,$00,$00,$00,$00,$00,$00,$00
  .byte $00,$00,$00,$00,$00,$01,$01,$01
  .byte $01,$01,$01,$01,$01,$01,$01,$01
  .byte $01,$01,$01,$01,$01,$01,$01,$01
  .byte $01,$01,$01,$01,$01,$01,$01,$01
  .byte $01,$01,$01,$01,$01,$02,$02,$02
  .byte $02,$02,$02,$02,$02,$02,$02,$02
  .byte $02,$02,$02,$02,$02,$02,$02,$02
  .byte $03,$03,$03,$03,$03,$03,$03,$03
  .byte $03,$03,$03,$03,$03,$04,$04,$04
  .byte $04,$04,$04,$04,$04,$04,$04,$04
  .byte $05,$05,$05,$05,$05,$05,$05,$05
  .byte $06,$06,$06,$06,$06,$06,$06,$06
  .byte $07,$07,$07,$07,$07,$07,$08,$08
  .byte $08,$08,$08,$08,$09,$09,$09,$09
  .byte $09,$0a,$0a,$0a,$0a,$0b,$0b,$0b
  .byte $0b,$0c,$0c,$0c,$0c,$0d,$0d,$0d
  .byte $0d,$0e,$0e,$0e,$0e,$0f,$0f,$0f
  .byte $10,$10,$10,$11,$11,$11,$12,$12
  .byte $12,$13,$13,$13,$14,$14,$15,$15
  .byte $15,$16,$16,$17,$17,$17,$18,$18
  .byte $19,$19,$19,$1a,$1a,$1b,$1b,$1c
  .byte $1c,$1c,$1d,$1d,$1e,$1e,$1f,$1f

;;;;;;;; log2(x)*32 ;;;;;;;;
log2_tab:
  .byte $00,$00,$20,$32,$40,$4a,$52,$59
  .byte $60,$65,$6a,$6e,$72,$76,$79,$7d
  .byte $80,$82,$85,$87,$8a,$8c,$8e,$90
  .byte $92,$94,$96,$98,$99,$9b,$9d,$9e
  .byte $a0,$a1,$a2,$a4,$a5,$a6,$a7,$a9
  .byte $aa,$ab,$ac,$ad,$ae,$af,$b0,$b1
  .byte $b2,$b3,$b4,$b5,$b6,$b7,$b8,$b9
  .byte $b9,$ba,$bb,$bc,$bd,$bd,$be,$bf
  .byte $c0,$c0,$c1,$c2,$c2,$c3,$c4,$c4
  .byte $c5,$c6,$c6,$c7,$c7,$c8,$c9,$c9
  .byte $ca,$ca,$cb,$cc,$cc,$cd,$cd,$ce
  .byte $ce,$cf,$cf,$d0,$d0,$d1,$d1,$d2
  .byte $d2,$d3,$d3,$d4,$d4,$d5,$d5,$d5
  .byte $d6,$d6,$d7,$d7,$d8,$d8,$d9,$d9
  .byte $d9,$da,$da,$db,$db,$db,$dc,$dc
  .byte $dd,$dd,$dd,$de,$de,$de,$df,$df
  .byte $df,$e0,$e0,$e1,$e1,$e1,$e2,$e2
  .byte $e2,$e3,$e3,$e3,$e4,$e4,$e4,$e5
  .byte $e5,$e5,$e6,$e6,$e6,$e7,$e7,$e7
  .byte $e7,$e8,$e8,$e8,$e9,$e9,$e9,$ea
  .byte $ea,$ea,$ea,$eb,$eb,$eb,$ec,$ec
  .byte $ec,$ec,$ed,$ed,$ed,$ed,$ee,$ee
  .byte $ee,$ee,$ef,$ef,$ef,$ef,$f0,$f0
  .byte $f0,$f1,$f1,$f1,$f1,$f1,$f2,$f2
  .byte $f2,$f2,$f3,$f3,$f3,$f3,$f4,$f4
  .byte $f4,$f4,$f5,$f5,$f5,$f5,$f5,$f6
  .byte $f6,$f6,$f6,$f7,$f7,$f7,$f7,$f7
  .byte $f8,$f8,$f8,$f8,$f9,$f9,$f9,$f9
  .byte $f9,$fa,$fa,$fa,$fa,$fa,$fb,$fb
  .byte $fb,$fb,$fb,$fc,$fc,$fc,$fc,$fc
  .byte $fd,$fd,$fd,$fd,$fd,$fd,$fe,$fe
  .byte $fe,$fe,$fe,$ff,$ff,$ff,$ff,$ff
.endproc


; https://github.com/TobyLobster/sqrt_test/blob/main/sqrt/sqrt9.a
; ***************************************************************************************
;
; sqrt
;
; Heavily based on http://www.txbobsc.com/aal/1986/aal8611.html#a1
; but reformatted and tweaked to improve performance by TobyLobster
; Average runtime is 39.84 cycles, worst case 129 cycles.
;
; On Entry:
;   X is high byte of number to SQRT (aka 'arghi')
;   A is low byte of number to SQRT  (aka 'arglo')
;
; On Exit:
;   Y is result
;
; ***************************************************************************************
; .proc FastSqrt

; argsav     =  R2                 ; 2 bytes
; arglo      =  R4                 ; 1 byte

;     cpx #$2c                        ; value already 'normalised' (i.e. large enough)?
;     bcs atleast11264                ; ...yes

;     ; $0000 to $2bff (11264 cases)
; under11264:
;     stx argsav+1                    ; save arghi
;     cpx #0                          ; is arghi zero?
;     beq under256                    ; ...yes

;     ; $01ff to $2bff (10752 cases)
;     ; we want to bring the input value into the range of our root table ($2c00-$ffff).
;     ;
;     ; each time around the next loop we multiply the input by 4 (double shift), which
;     ; doubles the result. So we keep track of the number of times we shift twice in Y
;     ; (aka shift_count) so we can scale down the result later.
;     ;
;     ; The loop has been unrolled for speed.
;     sta argsav                      ; save arglo for shifting
;     sta arglo                       ; save arglo for later compare
;     txa                             ; arghi to a
;     ldy #1                          ; Y = shift_count = 1

;     asl arglo                       ; }
;     rol                             ; }
;     asl arglo                       ; } shift arglo until >=$2c
;     rol                             ; }
;     cmp #$2c                        ; }
;     bcs normalised                  ; }

;     asl arglo                       ; }
;     rol                             ; }
;     asl arglo                       ; } shift arglo until >=$2c
;     rol                             ; }
;     iny                             ; } Y = shift_count = 2
;     cmp #$2c                        ; }
;     bcs normalised                  ; }

;     asl arglo                       ; }
;     rol                             ; }
;     asl arglo                       ; } shift arglo until >=$2c
;     rol                             ; }
;     iny                             ; } Y = shift_count = 3

;     ; a=normalised arg, y=shift_count
; normalised:
;     tax                             ; use norm-arg for index
;     lda root,x                      ; get root from table
; back:
;     lsr                             ; halve the root shift_count times
;     dey                             ;
;     bne back                           ;

;     ; check our result against actual square from square_low/high as it could be one out
;     tay                             ; use shifted root for index now
;     lda argsav                      ; get arglo
;     cmp SquareTableLo,y                ;
;     bcc forward                          ; ...speeds up average by 0.7 cycle
;     lda argsav+1                    ;
;     sbc SquareTableHi,y               ;
;     bcc forward                           ;
;     iny                             ;
; forward:
;     rts                             ;

; atleast11264:
;     ; $2c00 to $ffff (54272 cases)
;     cpx #$ff                        ; check for arghi = $ff
;     beq over65280                   ; ...yes, special case

;     ; if the number is big enough, then we can look up the root in a table indexed by the
;     ; high byte (with the proviso that it may be out by one). We then check against the
;     ; actual squares tables and adjust up by one as needed.
;     ldy root,x                      ; get root, use as index
;     cmp SquareTableLo,y                ;
;     bcc return1                     ; ...speeds up average by 0.8 cycle
;     txa                             ; arghi
;     sbc SquareTableHi,y               ;
;     bcc return1                     ;
;     iny                             ; adjust result by one
; return1:
;     rts                             ;

;     ; $0000 to $00ff (256 cases)
; under256:
;     tay                             ; is arglo also zero?
;     beq return1                     ; ...yes, sqrt=0

;     ; $0001 to $00ff (255 cases)
;     ; As above, we want to bring the input value into the range of our root table ($2c00-$ffff).
;     ;
;     ; each time around the next loop we multiply the input by 4 (double shifted), which
;     ; doubles the result. So we keep track of the number of times we double shifted in Y
;     ; (aka shift_count) so we can scale down the result later.
;     ;
;     ; The loop has been unrolled for speed.
;     ;
;     ; By using arglo (the low byte) as the high byte we have double shifted four
;     ; times already. Hence shift_count starts at four.
;     ;
;     sta argsav                      ; save arglo for later compare
;     ldy #4                          ; start shift_count = 4
;     cmp #$2c                        ; normalised yet?
;     bcs normalised                  ; ...yes, get root now
;     asl                             ;
;     asl                             ;
;     iny                             ; count the shift
;     cmp #$2c                        ; normalised yet?
;     bcs normalised                  ; ...yes, get root now
;     asl                             ;
;     asl                             ;
;     iny                             ; count the shift
;     cmp #$2c                        ; normalised yet?
;     bcs normalised                  ; ...yes, get root now
;     asl                             ;
;     asl                             ;
;     iny                             ; count the shift
;     bne normalised                  ; ALWAYS branch

;     ; $ff00 to $ffff (256 cases)
; over65280:
;     ldy #$ff                        ;
;     rts                             ;

; ; align tables to $2c offset from a page, so no page crossings occur
; ; !align $ff, $2c

; ; --------------------------------
; ; square root of n, for n=11264 to 65280 step 256
; root_table:
;   .byte                     $6a, $6b, $6c, $6d
;   .byte $6e, $70, $71, $72, $73, $74, $75, $76
;   .byte $77, $78, $79, $7a, $7b, $7c, $7d, $7e
;   .byte $80, $80, $81, $82, $83, $84, $85, $86
;   .byte $87, $88, $89, $8a, $8b, $8c, $8d, $8e
;   .byte $8f, $90, $90, $91, $92, $93, $94, $95
;   .byte $96, $96, $97, $98, $99, $9a, $9b, $9b
;   .byte $9c, $9d, $9e, $9f, $a0, $a0, $a1, $a2
;   .byte $a3, $a3, $a4, $a5, $a6, $a7, $a7, $a8
;   .byte $a9, $aa, $aa, $ab, $ac, $ad, $ad, $ae
;   .byte $af, $b0, $b0, $b1, $b2, $b2, $b3, $b4
;   .byte $b5, $b5, $b6, $b7, $b7, $b8, $b9, $b9
;   .byte $ba, $bb, $bb, $bc, $bd, $bd, $be, $bf
;   .byte $c0, $c0, $c1, $c1, $c2, $c3, $c3, $c4
;   .byte $c5, $c5, $c6, $c7, $c7, $c8, $c9, $c9
;   .byte $ca, $cb, $cb, $cc, $cc, $cd, $ce, $ce
;   .byte $cf, $d0, $d0, $d1, $d1, $d2, $d3, $d3
;   .byte $d4, $d4, $d5, $d6, $d6, $d7, $d7, $d8
;   .byte $d9, $d9, $da, $da, $db, $db, $dc, $dd
;   .byte $dd, $de, $de, $df, $e0, $e0, $e1, $e1
;   .byte $e2, $e2, $e3, $e3, $e4, $e5, $e5, $e6
;   .byte $e6, $e7, $e7, $e8, $e8, $e9, $ea, $ea
;   .byte $eb, $eb, $ec, $ec, $ed, $ed, $ee, $ee
;   .byte $ef, $f0, $f0, $f1, $f1, $f2, $f2, $f3
;   .byte $f3, $f4, $f4, $f5, $f5, $f6, $f6, $f7
;   .byte $f7, $f8, $f8, $f9, $f9, $fa, $fa, $fb
;   .byte $fb, $fc, $fc, $fd, $fd, $fe, $fe, $ff

; root = root_table-$2c    ; set up so $6a is first square root
; .endproc

; SquareTableLo:
; .repeat 256,I
;   .lobytes I * I
; .endrepeat

; SquareTableHi:
; .repeat 256,I
;   .hibytes I * I
; .endrepeat

; mult22.a
; from Niels Möller: https://www.lysator.liu.se/~nisse/misc/6502-mul.html
; slightly tweaked for speed
;
; 8 bit x 8 bit unsigned multiply, 16 bit result
; Average cycles: 76.48
; 563 bytes

; In: Factors in A and X
; Out: High byte in A, low byte in result_low
; .proc FastMult8x8

; min         = R2
; result_low  = R3
; temp3       = R4
;     sta min
;     cpx min
;     bcc swap
;     txa
; continue:
;     sbc min
;     tay
;     ; at this point:
;     ;   Y = max(inputs) - min(inputs);
;     ;   X = max(inputs);
;     lda SquareTableLo,x
;     sbc SquareTableLo,y
;     sta result_low
;     lda SquareTableHi,x
;     sbc SquareTableHi,y
;     sta temp3
;     clc
;     ldx min
;     lda result_low
;     adc SquareTableLo,x
;     sta result_low
;     lda temp3
;     adc SquareTableHi,x
;     ror
;     ror result_low
;     rts

; swap:
;     stx min
;     tax
;     sec
;     bcs continue            ; ALWAYS branch
; .endproc

; .proc LongDivide
; divisor = R1      ;R2 used for hi-byte
; dividend = R3	    ;R4 used for hi-byte
; remainder = R5	  ;R6 used for hi-byte
; result = dividend ;save memory by reusing divident to store the result

;   lda #0	        ;preset remainder to 0
;   sta remainder
;   sta remainder+1
;   ldx #16	        ;repeat for each bit: ...

; divloop:
;   asl dividend	;dividend lb & hb*2, msb -> Carry
;   rol dividend+1	
;   rol remainder	;remainder lb & hb * 2 + msb from carry
;   rol remainder+1
;   lda remainder
;   sec
;   sbc divisor	;substract divisor to see if it fits in
;   tay	        ;lb result -> Y, for we may need it later
;   lda remainder+1
;   sbc divisor+1
;   bcc skip	;if carry=0 then divisor didn't fit in yet

;   sta remainder+1	;else save substraction result as new remainder,
;   sty remainder	
;   inc result	;and INCrement result cause divisor fit in 1 times
; skip:
;   dex
;   bne divloop	
;   rts
; .endproc

; .proc FastSinCos
; .import CosTable, SinTable
;   ldx CosTable, y
;   lda SinTable, y
;   tay
;   rts
; .endproc

; .pushseg
; .segment "LOWCODE"
; .proc FastSinCos
; .import SinTable, CosTable
; pointer = R0
; ; currentbank = R2
; ;; in values
; ; angle = Y
; ; mag = A
; ;; out values
; ; cos = X
; ; sin = Y
;   ; multiply the address by 255 to get the correct offset in the lookup
;   ; aka "just use the magnitude as the lookup high byte"
;   sta pointer+1
;   lda #0
;   sta pointer
; ; .repeat 5
; ;   asl pointer
; ;   rol pointer+1
; ; .endrepeat
;   BankPRGA #.lobyte(.bank(CosTable))
;   lda #.hibyte(CosTable)
;   clc
;   adc pointer+1
;   sta pointer+1
;   lda (pointer),y 
;   tax
;   BankPRGA #.lobyte(.bank(SinTable))
;   ; lda #.hibyte(SinTable - CosTable)
;   ; clc
;   ; adc pointer+1
;   ; sta pointer+1
;   lda (pointer),y
;   tay
;   ; TODO do we need to call this from anywhere else?
;   BankPRGA #.lobyte(.bank(SlingHold))
;   rts
; .endproc
; .popseg