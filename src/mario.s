.include "common.inc"

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
          lda AreaType                ;if not water type level, skip the rest of this
          bne BublExit
          ldx #$02                    ;otherwise load counter and use as offset
BublLoop: stx ObjectOffset            ;store offset
          jsr BubbleCheck             ;check timers and coordinates, create air bubble
          jsr RelativeBubblePosition  ;get relative coordinates
          jsr GetBubbleOffscreenBits  ;get offscreen information
          jsr DrawBubble              ;draw the air bubble
          dex
          bpl BublLoop                ;do this until all three are handled
BublExit: rts                         ;then leave

FireballXSpdData:
      .byte $40, $c0

FireballObjCore:
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
RunFB:   txa                          ;add 7 to offset to use
         clc                          ;as fireball offset for next routines
         adc #$07
         tax
         lda #$50                     ;set downward movement force here
         sta $00
         lda #$03                     ;set maximum speed here
         sta $02
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
EraseFB: lda #$00                     ;erase fireball state
         sta Fireball_State,x
NoFBall: rts                          ;leave

FireballExplosion:
      jsr RelativeFireballPosition
      jmp DrawExplosion_Fireball

BubbleCheck:
      lda PseudoRandomBitReg+1,x  ;get part of LSFR
      and #$01
      sta $07                     ;store pseudorandom bit here
      lda Bubble_Y_Position,x     ;get vertical coordinate for air bubble
      cmp #$f8                    ;if offscreen coordinate not set,
      bne MoveBubl                ;branch to move air bubble
      lda AirBubbleTimer          ;if air bubble timer not expired,
      bne ExitBubl                ;branch to leave, otherwise create new air bubble

SetupBubble:
          ldy #$00                 ;load default value here
          lda PlayerFacingDir      ;get player's facing direction
          lsr                      ;move d0 to carry
          bcc PosBubl              ;branch to use default value if facing left
          ldy #$08                 ;otherwise load alternate value here
PosBubl:  tya                      ;use value loaded as adder
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
          ldy $07                  ;get pseudorandom bit, use as offset
          lda BubbleTimerData,y    ;get data for air bubble timer
          sta AirBubbleTimer       ;set air bubble timer
MoveBubl: ldy $07                  ;get pseudorandom bit again, use as offset
          lda Bubble_YMF_Dummy,x
          sec                      ;subtract pseudorandom amount from dummy variable
          sbc Bubble_MForceData,y
          sta Bubble_YMF_Dummy,x   ;save dummy variable
          lda Bubble_Y_Position,x
          sbc #$00                 ;subtract borrow from airbubble's vertical coordinate
          cmp #$20                 ;if below the status bar,
          bcs Y_Bubl               ;branch to go ahead and use to move air bubble upwards
          lda #$f8                 ;otherwise set offscreen coordinate
Y_Bubl:   sta Bubble_Y_Position,x  ;store as new vertical coordinate for air bubble
ExitBubl: rts                      ;leave

Bubble_MForceData:
      .byte $ff, $50

BubbleTimerData:
      .byte $40, $20
.endproc

;-------------------------------------------------------------------------------------

.proc DrawPlayer_Intermediate
.import DrawPlayerLoop

  ldx #$05                       ;store data into zero page memory
PIntLoop:
    lda IntermediatePlayerData,x   ;load data to display player as he always
    sta $02,x                      ;appears on world/lives display
    dex
    bpl PIntLoop                   ;do this until all data is loaded
  ldx #$b8                       ;load offset for small standing
  ldy #$04                       ;load sprite data offset
  jsr DrawPlayerLoop             ;draw player accordingly
  lda Sprite_Attributes+36       ;get empty sprite attributes
  ora #%01000000                 ;set horizontal flip bit for bottom-right sprite
  sta Sprite_Attributes+32       ;store and leave
  rts
  IntermediatePlayerData:
        .byte $58, $01, $00, $60, $ff, $04

.endproc

;-------------------------------------------------------------------------------------
;$00-$01 - used to hold tile numbers, $00 also used to hold upper extent of animation frames
;$02 - vertical position
;$03 - facing direction, used as horizontal flip control
;$04 - attributes
;$05 - horizontal position
;$07 - number of rows to draw
;these also used in IntermediatePlayerData

.proc RenderPlayerSub
.import DrawOneSpriteRow
.export DrawPlayerLoop
  sta $07                      ;store number of rows of sprites to draw
  lda Player_Rel_XPos
  sta Player_Pos_ForScroll     ;store player's relative horizontal position
  sta $05                      ;store it here also
  lda Player_Rel_YPos
  sta $02                      ;store player's vertical position
  lda PlayerFacingDir
  sta $03                      ;store player's facing direction
  lda Player_SprAttrib
  sta $04                      ;store player's sprite attributes
  ldx PlayerGfxOffset          ;load graphics table offset
  ldy Player_SprDataOffset     ;get player's sprite data offset

DrawPlayerLoop:
  lda PlayerGraphicsTable,x    ;load player's left side
  sta $00
  lda PlayerGraphicsTable+1,x  ;now load right side
  jsr DrawOneSpriteRow
  dec $07                      ;decrement rows of sprites to draw
  bne DrawPlayerLoop           ;do this until all rows are drawn
  rts

;tiles arranged in order, 2 tiles per row, top to bottom
SwimTileRepOffset     = PlayerGraphicsTable + $9e

PlayerGraphicsTable:
;big player table
      .byte $00, $01, $02, $03, $04, $05, $06, $07 ;walking frame 1
      .byte $08, $09, $0a, $0b, $0c, $0d, $0e, $0f ;        frame 2
      .byte $10, $11, $12, $13, $14, $15, $16, $17 ;        frame 3
      .byte $18, $19, $1a, $1b, $1c, $1d, $1e, $1f ;skidding
      .byte $20, $21, $22, $23, $24, $25, $26, $27 ;jumping
      .byte $08, $09, $28, $29, $2a, $2b, $2c, $2d ;swimming frame 1
      .byte $08, $09, $0a, $0b, $0c, $30, $2c, $2d ;         frame 2
      .byte $08, $09, $0a, $0b, $2e, $2f, $2c, $2d ;         frame 3
      .byte $08, $09, $28, $29, $2a, $2b, $5c, $5d ;climbing frame 1
      .byte $08, $09, $0a, $0b, $0c, $0d, $5e, $5f ;         frame 2
      .byte $fc, $fc, $08, $09, $58, $59, $5a, $5a ;crouching
      .byte $08, $09, $28, $29, $2a, $2b, $0e, $0f ;fireball throwing

;small player table
      .byte $fc, $fc, $fc, $fc, $32, $33, $34, $35 ;walking frame 1
      .byte $fc, $fc, $fc, $fc, $36, $37, $38, $39 ;        frame 2
      .byte $fc, $fc, $fc, $fc, $3a, $37, $3b, $3c ;        frame 3
      .byte $fc, $fc, $fc, $fc, $3d, $3e, $3f, $40 ;skidding
      .byte $fc, $fc, $fc, $fc, $32, $41, $42, $43 ;jumping
      .byte $fc, $fc, $fc, $fc, $32, $33, $44, $45 ;swimming frame 1
      .byte $fc, $fc, $fc, $fc, $32, $33, $44, $47 ;         frame 2
      .byte $fc, $fc, $fc, $fc, $32, $33, $48, $49 ;         frame 3
      .byte $fc, $fc, $fc, $fc, $32, $33, $90, $91 ;climbing frame 1
      .byte $fc, $fc, $fc, $fc, $3a, $37, $92, $93 ;         frame 2
      .byte $fc, $fc, $fc, $fc, $9e, $9e, $9f, $9f ;killed

;used by both player sizes
      .byte $fc, $fc, $fc, $fc, $3a, $37, $4f, $4f ;small player standing
      .byte $fc, $fc, $00, $01, $4c, $4d, $4e, $4e ;intermediate grow frame
      .byte $00, $01, $4c, $4d, $4a, $4a, $4b, $4b ;big player standing

SwimKickTileNum:
      .byte $31, $46

.endproc

;-------------------------------------------------------------------------------------
;$00 - used to store player's vertical offscreen bits
.proc PlayerGfxHandler
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
  cmp #$00                    ;if player status normal,
  beq FindPlayerAction        ;branch and do not return
  jsr FindPlayerAction        ;otherwise jump and return
  lda FrameCounter
  and #%00000100              ;check frame counter for d2 set (8 frames every
  bne ExPGH                   ;eighth frame), and branch if set to leave
  tax                         ;initialize X to zero
  ldy Player_SprDataOffset    ;get player sprite data offset
  lda PlayerFacingDir         ;get player's facing direction
  lsr
  bcs SwimKT                  ;if player facing to the right, use current offset
  iny
  iny                         ;otherwise move to next OAM data
  iny
  iny
SwimKT:
  lda PlayerSize              ;check player's size
        beq BigKTS                  ;if big, use first tile
        lda Sprite_Tilenumber+24,y  ;check tile number of seventh/eighth sprite
        cmp SwimTileRepOffset       ;against tile number in player graphics table
        beq ExPGH                   ;if spr7/spr8 tile number = value, branch to leave
        inx                         ;otherwise increment X for second tile
BigKTS: lda SwimKickTileNum,x       ;overwrite tile number in sprite 7/8
        sta Sprite_Tilenumber+24,y  ;to animate player's feet when swimming
ExPGH:  rts                         ;then leave

FindPlayerAction:
      jsr ProcessPlayerAction       ;find proper offset to graphics table by player's actions
      jmp PlayerGfxProcessing       ;draw player, then process for fireball throwing

DoChangeSize:
      jsr HandleChangeSize          ;find proper offset to graphics table for grow/shrink
      jmp PlayerGfxProcessing       ;draw player, then process for fireball throwing

PlayerKilled:
      ldy #$0e                      ;load offset for player killed
      lda PlayerGfxTblOffsets,y     ;get offset to graphics table

PlayerGfxProcessing:
       sta PlayerGfxOffset           ;store offset to graphics table here
       lda #$04
       jsr RenderPlayerSub           ;draw player based on offset loaded
       jsr ChkForPlayerAttrib        ;set horizontal flip bits as necessary
       lda FireballThrowingTimer
       beq PlayerOffscreenChk        ;if fireball throw timer not set, skip to the end
       ldy #$00                      ;set value to initialize by default
       lda PlayerAnimTimer           ;get animation frame timer
       cmp FireballThrowingTimer     ;compare to fireball throw timer
       sty FireballThrowingTimer     ;initialize fireball throw timer
       bcs PlayerOffscreenChk        ;if animation frame timer => fireball throw timer skip to end
       sta FireballThrowingTimer     ;otherwise store animation timer into fireball throw timer
       ldy #$07                      ;load offset for throwing
       lda PlayerGfxTblOffsets,y     ;get offset to graphics table
       sta PlayerGfxOffset           ;store it for use later
       ldy #$04                      ;set to update four sprite rows by default
       lda Player_X_Speed
       ora Left_Right_Buttons        ;check for horizontal speed or left/right button press
       beq SUpdR                     ;if no speed or button press, branch using set value in Y
       dey                           ;otherwise set to update only three sprite rows
SUpdR: tya                           ;save in A for use
       jsr RenderPlayerSub           ;in sub, draw player object again

PlayerOffscreenChk:
           lda Player_OffscreenBits      ;get player's offscreen bits
           lsr
           lsr                           ;move vertical bits to low nybble
           lsr
           lsr
           sta $00                       ;store here
           ldx #$03                      ;check all four rows of player sprites
           lda Player_SprDataOffset      ;get player's sprite data offset
           clc
           adc #$18                      ;add 24 bytes to start at bottom row
           tay                           ;set as offset here
PROfsLoop: lda #$f8                      ;load offscreen Y coordinate just in case
           lsr $00                       ;shift bit into carry
           bcc NPROffscr                 ;if bit not set, skip, do not move sprites
           jsr DumpTwoSpr                ;otherwise dump offscreen Y coordinate into sprite data
NPROffscr: tya
           sec                           ;subtract eight bytes to do
           sbc #$08                      ;next row up
           tay
           dex                           ;decrement row counter
           bpl PROfsLoop                 ;do this until all sprite rows are checked
           rts                           ;then we are done!

PlayerGfxTblOffsets:
      .byte $20, $28, $c8, $18, $00, $40, $50, $58
      .byte $80, $88, $b8, $78, $60, $a0, $b0, $b8
.endproc 

.proc HandleChangeSize
         ldy PlayerAnimCtrl           ;get animation frame control
         lda FrameCounter
         and #%00000011               ;get frame counter and execute this code every
         bne GorSLog                  ;fourth frame, otherwise branch ahead
         iny                          ;increment frame control
         cpy #$0a                     ;check for preset upper extent
         bcc CSzNext                  ;if not there yet, skip ahead to use
         ldy #$00                     ;otherwise initialize both grow/shrink flag
         sty PlayerChangeSizeFlag     ;and animation frame control
CSzNext: sty PlayerAnimCtrl           ;store proper frame control
GorSLog: lda PlayerSize               ;get player's size
         bne ShrinkPlayer             ;if player small, skip ahead to next part
         lda ChangeSizeOffsetAdder,y  ;get offset adder based on frame control as offset
         ldy #$0f                     ;load offset for player growing

GetOffsetFromAnimCtrl:
        asl                        ;multiply animation frame control
        asl                        ;by eight to get proper amount
        asl                        ;to add to our offset
        adc PlayerGfxTblOffsets,y  ;add to offset to graphics table
        rts                        ;and return with result in A
ChangeSizeOffsetAdder:
        .byte $00, $01, $00, $01, $00, $01, $02, $00, $01, $02
        .byte $02, $00, $02, $00, $02, $00, $02, $00, $02, $00

.endproc
