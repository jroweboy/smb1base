
.include "common.inc"
; .include "object.inc"
.include "metasprite.inc"

; screen_render.s
.import AddToScore

.export DrawSingleFireball, DrawSmallPlatform, DrawFireball
.export DrawVine, DrawLargePlatform, DrawPowerUp
.export DrawOneSpriteRow, JCoinGfxHandler, DrawHammer, DrawBrickChunks, DrawBlock
.export FlagpoleGfxHandler

.export DumpTwoSpr, FloateyNumbersRoutine

.segment "RENDER"

;-------------------------------------------------------------------------------------
;$00 - offset to vine Y coordinate adder
;$02 - offset to sprite data

VineYPosAdder:
      .byte $00, $30

DrawVine:
         sty R0                     ;save offset here
         lda Enemy_Rel_YPos         ;get relative vertical coordinate
         clc
         adc VineYPosAdder,y        ;add value using offset in Y to get value
         ldx Vine_ObjOffset,y        ;get offset to vine
      ;    ldy Enemy_SprDataOffset,x  ;get sprite data offset
      ReserveSpr 6
         sty R2                    ;store sprite data offset here
         jsr SixSpriteStackerVanilla ;stack six sprites on top of each other vertically
         lda Enemy_Rel_XPos         ;get relative horizontal coordinate
         sta Sprite_X_Position,y    ;store in first, third and fifth sprites
         sta Sprite_X_Position+8,y
         sta Sprite_X_Position+16,y
         clc
         adc #$06                   ;add six pixels to second, fourth and sixth sprites
         sta Sprite_X_Position+4,y  ;to give characteristic staggered vine shape to
         sta Sprite_X_Position+12,y ;our vertical stack of sprites
         sta Sprite_X_Position+20,y
         lda #%00100001             ;set bg priority and palette attribute bits
         sta Sprite_Attributes,y    ;set in first, third and fifth sprites
         sta Sprite_Attributes+8,y
         sta Sprite_Attributes+16,y
         ora #%01000000             ;additionally, set horizontal flip bit
         sta Sprite_Attributes+4,y  ;for second, fourth and sixth sprites
         sta Sprite_Attributes+12,y
         sta Sprite_Attributes+20,y
         ldx #$05                   ;set tiles for six sprites
VineTL:  lda #VINE_TILE_2           ;set tile number for sprite
         sta Sprite_Tilenumber,y
         iny                        ;move offset to next sprite data
         iny
         iny
         iny
         dex                        ;move onto next sprite
         bpl VineTL                 ;loop until all sprites are done
      UpdateOAMPosition
         ldy R2                     ;get original offset
         lda R0                     ;get offset to vine adding data
         bne SkpVTop                ;if offset not zero, skip this part
         lda #VINE_TILE_1
         sta Sprite_Tilenumber,y    ;set other tile number for top of vine
SkpVTop: ldx #$00                   ;start with the first sprite again
ChkFTop: lda Vine_Start_Y_Position   ;get original starting vertical coordinate
         sec
         sbc Sprite_Y_Position,y    ;subtract top-most sprite's Y coordinate
         cmp #$64                   ;if two coordinates are less than 100/$64 pixels
         bcc NextVSp                ;apart, skip this to leave sprite alone
         lda #$f8
         sta Sprite_Y_Position,y    ;otherwise move sprite offscreen
NextVSp: iny                        ;move offset to next OAM data
         iny
         iny
         iny
         inx                        ;move onto next sprite
         cpx #$06                   ;do this until all sprites are checked
         bne ChkFTop
         ldy R0                     ;return offset set earlier
         rts


FourSpriteStacker:
  sta R1
  sty R3

  ; Change the start index for the shuffle each time
  lda PlatformLastOAMOrder
  clc
  adc #3
  cmp #4
  bcc @SkipSubtract
    ; implicit carry set
    sbc #4
@SkipSubtract:
  sta PlatformLastOAMOrder

  ldx #4           ;do six sprites
@StkLp:
    ; keep platforms from disappearing with too many on screen
    lda PlatformLastOAMOrder
    clc
    adc #3
    cmp #4
    bcc @SkipSubtract1
      ; implicit carry set
      sbc #4
  @SkipSubtract1:
    sta PlatformLastOAMOrder
    asl
    asl
    clc
    adc R3
    tay
    lda R1
    sta Sprite_Data,y ;store X or Y coordinate into OAM data
    clc
    adc #$08           ;add eight pixels
    sta R1
    dex                ;do another sprite
    bne @StkLp          ;do this until all sprites are done
  ldy R2            ;get saved OAM data offset and leave
  rts

SixSpriteStackerVanilla:
  ldx #$06           ;do six sprites
@StkLp:
    sta Sprite_Data,y ;store X or Y coordinate into OAM data
    clc
    adc #$08           ;add eight pixels
    iny
    iny                ;move offset four bytes forward
    iny
    iny
    dex                ;do another sprite
    bne @StkLp          ;do this until all sprites are done
  ldy R2             ;get saved OAM data offset and leave
  rts

SixSpriteStacker:
  sta R1
  sty R3

  ; Change the start index for the shuffle each time
  lda PlatformLastOAMOrder
  clc
  adc #1
  cmp #6
  bcc @SkipSubtract
    ; implicit carry set
    sbc #6
@SkipSubtract:
  sta PlatformLastOAMOrder

  ldx #$06           ;do six sprites
@loop:
    ; keep platforms from disappearing with too many on screen
    lda PlatformLastOAMOrder
    clc
    adc #5
    cmp #6
    bcc @SkipSubtract1
      ; implicit carry set
      sbc #6
  @SkipSubtract1:
    sta PlatformLastOAMOrder
    asl
    asl
    clc
    adc R3
    tay
    lda R1
    sta Sprite_Data,y ;store X or Y coordinate into OAM data
    clc
    adc #$08           ;add eight pixels
    sta R1
    dex                ;do another sprite
    bne @loop          ;do this until all sprites are done
  ldy R2            ;get saved OAM data offset and leave
  rts
;-------------------------------------------------------------------------------------

FirstSprXPos:
      .byte $04, $00, $04, $00

FirstSprYPos:
      .byte $00, $04, $00, $04

SecondSprXPos:
      .byte $00, $08, $00, $08

SecondSprYPos:
      .byte $08, $00, $08, $00

FirstSprTilenum:
      .byte HAMMER_HEAD_1, HAMMER_HEAD_2, HAMMER_TAIL_1, HAMMER_TAIL_2

SecondSprTilenum:
      .byte HAMMER_TAIL_1, HAMMER_TAIL_2, HAMMER_HEAD_1, HAMMER_HEAD_2

HammerSprAttrib:
      .byte $03, $03, $c3, $c3

DrawHammer:
            ; ldy Misc_SprDataOffset,x    ;get misc object OAM data offset
          AllocSpr 2
            lda TimerControl
            bne ForceHPose              ;if master timer control set, skip this part
            lda Misc_State,x            ;otherwise get hammer's state
            and #%01111111              ;mask out d7
            cmp #$01                    ;check to see if set to 1 yet
            beq GetHPose                ;if so, branch
ForceHPose: ldx #$00                    ;reset offset here
            beq RenderH                 ;do unconditional branch to rendering part
GetHPose:   lda FrameCounter            ;get frame counter
            lsr                         ;move d3-d2 to d1-d0
            lsr
            and #%00000011              ;mask out all but d1-d0 (changes every four frames)
            tax                         ;use as timing offset
RenderH:    lda Misc_Rel_YPos           ;get relative vertical coordinate
            clc
            adc FirstSprYPos,x          ;add first sprite vertical adder based on offset
            sta Sprite_Y_Position,y     ;store as sprite Y coordinate for first sprite
            clc
            adc SecondSprYPos,x         ;add second sprite vertical adder based on offset
            sta Sprite_Y_Position+4,y   ;store as sprite Y coordinate for second sprite
            lda Misc_Rel_XPos           ;get relative horizontal coordinate
            clc
            adc FirstSprXPos,x          ;add first sprite horizontal adder based on offset
            sta Sprite_X_Position,y     ;store as sprite X coordinate for first sprite
            clc
            adc SecondSprXPos,x         ;add second sprite horizontal adder based on offset
            sta Sprite_X_Position+4,y   ;store as sprite X coordinate for second sprite
            lda FirstSprTilenum,x
            sta Sprite_Tilenumber,y     ;get and store tile number of first sprite
            lda SecondSprTilenum,x
            sta Sprite_Tilenumber+4,y   ;get and store tile number of second sprite
            lda HammerSprAttrib,x
            sta Sprite_Attributes,y     ;get and store attribute bytes for both
            sta Sprite_Attributes+4,y   ;note in this case they use the same data
            ldx ObjectOffset            ;get misc object offset
            lda Misc_OffscreenBits
            and #%11111100              ;check offscreen bits
            beq NoHOffscr               ;if all bits clear, leave object alone
            lda #$00
            sta Misc_State,x            ;otherwise nullify misc object state
            lda #$f8
            jmp DumpTwoSpr              ;do sub to move hammer sprites offscreen
NoHOffscr:  rts ; TODO check this RTS can be removed                         ;leave

;-------------------------------------------------------------------------------------

DrawLargePlatform:
  ldy AreaType
  cpy #$03                    ;check for castle-type level
  beq ShrinkPlatform
    ldy SecondaryHardMode       ;check for secondary hard mode flag set
    bne ShrinkPlatform          ;branch if its hardmode
      AllocSpr 6
      sty R2                      ;store here
      iny                         ;add 3 to it for offset
      iny                         ;to X coordinate
      iny
      lda Enemy_Rel_XPos          ;get horizontal relative coordinate
      jsr SixSpriteStacker        ;store X coordinates using A as base, stack horizontally
      ldy R2
      ldx ObjectOffset
      lda Enemy_Y_Position,x      ;get vertical coordinate
      jsr DumpSixSpr             ;dump into first four sprites as Y coordinate
      jmp ProcessTiles

ShrinkPlatform:
  AllocSpr 4
  sty R2                     ;store here
  iny                         ;add 3 to it for offset
  iny                         ;to X coordinate
  iny
  lda Enemy_Rel_XPos          ;get horizontal relative coordinate
  jsr FourSpriteStacker        ;store X coordinates using A as base, stack horizontally
  ldx ObjectOffset
  ldy R2
  lda Enemy_Y_Position,x      ;get vertical coordinate
  jsr DumpFourSpr             ;dump into first four sprites as Y coordinate

ProcessTiles:
  lda #PLATFORM_GIRDER ; $bc                    ;load default tile for platform (girder)
  ldx CloudTypeOverride
  beq SetPlatformTilenum      ;if cloud level override flag not set, use
  lda #PLATFORM_CLOUD ; $fe                    ;otherwise load other tile for platform (puff)

SetPlatformTilenum:
  ldx ObjectOffset            ;get enemy object buffer offset
  iny                         ;increment Y for tile offset
  jsr DumpSixSpr              ;dump tile number into all six sprites
  lda #$02                    ;set palette controls
  iny                         ;increment Y for sprite attributes
  jsr DumpSixSpr              ;dump attributes into all six sprites
  inx                         ;increment X for enemy objects
  jsr GetXOffscreenBits       ;get offscreen bits again
  sta R1                      ;check d7 of offscreen bits
  ; can't use processor flags because of loop earlier, so we need to cmp #ff
  ; which indicates that all rows are offscreen
  cmp #$ff
  bne :+
    ldy R2
    jmp MoveSixSpritesOffscreen ;otherwise branch to move all sprites offscreen
: 
  ; at least one sprite is on screen
  ldx #0
  ; New offscreen check using the same shuffle constant from earlier
  ; lda PlatformLastOAMOrder
  @loop:
    lda PlatformLastOAMOrder
    clc
    adc #5
    cmp #6
    bcc @SkipSubtract1
      ; implicit carry set
      sbc #6
  @SkipSubtract1:
    sta PlatformLastOAMOrder

    lda R1
    and InversePowerOfTwo,x
    beq :+
      ; sprite is offscreen so move it offscreen
      lda PlatformLastOAMOrder
      asl
      asl
      clc
      adc R2
      tay
      lda #$f8
      sta Sprite_Y_Position,y
    :

    inx
    cpx #6
    bne @loop


  ldx ObjectOffset
  rts

InversePowerOfTwo:
.repeat 8, I
  .byte (1 << (7-I))
.endrepeat

;-------------------------------------------------------------------------------------

DrawFloateyNumber_Coin:
  lda FrameCounter          ;get frame counter
  lsr                       ;divide by 2
  bcs @NotRsNum             ;branch if d0 not set to raise number every other frame
    dec Misc_Y_Position,x     ;otherwise, decrement vertical coordinate
@NotRsNum:
  lda Misc_Y_Position,x     ;get vertical coordinate
  jsr DumpTwoSpr            ;dump into both sprites
  lda Misc_Rel_XPos         ;get relative horizontal coordinate
  sta Sprite_X_Position,y   ;store as X coordinate for first sprite
  clc
  adc #$08                  ;add eight pixels
  sta Sprite_X_Position+4,y ;store as X coordinate for second sprite
  lda #$02
  sta Sprite_Attributes,y   ;store attribute byte in both sprites
  sta Sprite_Attributes+4,y
  lda #FLOATEY_NUM_20
  sta Sprite_Tilenumber,y   ;put tile numbers into both sprites
  lda #FLOATEY_NUM_0                  ;that resemble "200"
  sta Sprite_Tilenumber+4,y
  jmp ExJCGfx               ;then jump to leave (why not an rts here instead?)

JumpingCoinTiles:
  .byte JUMPING_COIN_TILE_1, JUMPING_COIN_TILE_2
  .byte JUMPING_COIN_TILE_3, JUMPING_COIN_TILE_4

JCoinGfxHandler:
  ;  ldy Misc_SprDataOffset,x    ;get coin/floatey number's OAM data offset
  AllocSpr 2
  lda Misc_State,x            ;get state of misc object
  cmp #$02                    ;if 2 or greater, 
  bcs DrawFloateyNumber_Coin  ;branch to draw floatey number
    lda Misc_Y_Position,x       ;store vertical coordinate as
    sta Sprite_Y_Position,y     ;Y coordinate for first sprite
    clc
    adc #$08                    ;add eight pixels
    sta Sprite_Y_Position+4,y   ;store as Y coordinate for second sprite
    lda Misc_Rel_XPos           ;get relative horizontal coordinate
    sta Sprite_X_Position,y
    sta Sprite_X_Position+4,y   ;store as X coordinate for first and second sprites
    lda FrameCounter            ;get frame counter
    lsr                         ;divide by 2 to alter every other frame
    and #%00000011              ;mask out d2-d1
    tax                         ;use as graphical offset
    lda JumpingCoinTiles,x      ;load tile number
    iny                         ;increment OAM data offset to write tile numbers
    jsr DumpTwoSpr              ;do sub to dump tile number into both sprites
    dey                         ;decrement to get old offset
    lda #$02
    sta Sprite_Attributes,y     ;set attribute byte in first sprite
    lda #$82
    sta Sprite_Attributes+4,y   ;set attribute byte with vertical flip in second sprite
    ldx ObjectOffset            ;get misc object offset
ExJCGfx: rts                         ;leave

;-------------------------------------------------------------------------------------
;$00-$01 - used to hold tiles for drawing the power-up, $00 also used to hold power-up type
;$02 - used to hold bottom row Y position
;$03 - used to hold flip control (not used here)
;$04 - used to hold sprite attributes
;$05 - used to hold X position
;$07 - counter

;tiles arranged in top left, right, bottom left, right order
PowerUpGfxTable:
  .byte MUSHROOM_TOP_LEFT, MUSHROOM_TOP_RIGHT, MUSHROOM_BOT_LEFT, MUSHROOM_BOT_RIGHT ;regular mushroom
  .byte FIREFLOWER_TOP_LEFT, FIREFLOWER_TOP_LEFT, FIREFLOWER_BOT_LEFT, FIREFLOWER_BOT_LEFT ;fire flower
  .byte STAR_TOP_LEFT, STAR_TOP_LEFT, STAR_BOT_LEFT, STAR_BOT_LEFT ;star
  .byte ONEUP_TOP_LEFT, ONEUP_TOP_RIGHT, ONEUP_BOT_LEFT, ONEUP_BOT_RIGHT ;1-up mushroom

PowerUpAttributes:
    .byte $02, $01, $02, $01

.proc DrawPowerUp
      ; ldy Enemy_SprDataOffset+5  ;get power-up's sprite data offset
  ; AllocSpr 4
  ; sty OriginalOAMOffset
  lda Enemy_Rel_YPos         ;get relative vertical coordinate
  clc
  adc #$08                   ;add eight pixels
  sta R2                     ;store result here
  lda Enemy_Rel_XPos         ;get relative horizontal coordinate
  sta R5                     ;store here
  ldx PowerUpType            ;get power-up type
  ; lda PowerUpAttributes,x    ;get attribute data for power-up type
  ; ora Enemy_SprAttrib+5      ;add background priority bit if set
  ; sta R4                     ;store attributes here
  ; txa
  ; pha                        ;save power-up type to the stack
    ; asl
    ; asl                        ;multiply by four to get proper offset
    ; tax                        ;use as X
    ; lda #$01
    ; sta R7                     ;set counter here to draw two rows of sprite object
    ; sta R3                     ;init d1 of flip control

; PUpDrawLoop:
;       lda PowerUpGfxTable,x      ;load left tile of power-up object
;       sta R0 
;       lda PowerUpGfxTable+1,x    ;load right tile
;       jsr DrawOneSpriteRow       ;branch to draw one row of our power-up object
;       dec R7                     ;decrement counter
;       bpl PUpDrawLoop           ;branch until two rows are drawn
    ; ldy Enemy_SprDataOffset+5  ;get sprite data offset again
  ;   ldy OriginalOAMOffset
  ; pla                        ;pull saved power-up type from the stack
;   beq PUpOfs                 ;if regular mushroom, branch, do not change colors or flip
;   cmp #$03
;   beq PUpOfs                 ;if 1-up mushroom, branch, do not change colors or flip
;     sta R0                     ;store power-up type here now
;     lda FrameCounter           ;get frame counter
;     lsr                        ;divide by 2 to change colors every two frames
;     and #%00000011             ;mask out all but d1 and d0 (previously d2 and d1)
;     ora Enemy_SprAttrib+5      ;add background priority bit if any set
;     sta Sprite_Attributes,y    ;set as new palette bits for top left and
;     sta Sprite_Attributes+4,y  ;top right sprites for fire flower and star
;     ldx R0 
;     dex                        ;check power-up type for fire flower
;     beq FlipPUpRightSide       ;if found, skip this part
;       sta Sprite_Attributes+8,y  ;otherwise set new palette bits  for bottom left
;       sta Sprite_Attributes+12,y ;and bottom right sprites as well for star only
; FlipPUpRightSide:
;   lda Sprite_Attributes+4,y
;   ora #%01000000             ;set horizontal flip bit for top right sprite
;   sta Sprite_Attributes+4,y
;   lda Sprite_Attributes+12,y
;   ora #%01000000             ;set horizontal flip bit for bottom right sprite
;   sta Sprite_Attributes+12,y ;note these are only done for fire flower and star power-ups
PUpOfs:
  jmp SprObjectOffscrChk     ;jump to check to see if power-up is offscreen at all, then leave
.endproc

;-------------------------------------------------------------------------------------
;$00-$01 - used in DrawEnemyObjRow to hold sprite tile numbers
;$02 - used to store Y position
;$03 - used to store moving direction, used to flip enemies horizontally
;$04 - used to store enemy's sprite attributes
;$05 - used to store X position
;$eb - used to hold sprite data offset
;$ec - used to hold either altered enemy state or special value used in gfx handler as condition
;$ed - used to hold enemy state from buffer 
;$ef - used to hold enemy code used in gfx handler (may or may not resemble Enemy_ID values)

;tiles arranged in top left, right, middle left, right, bottom left, right order
EnemyGraphicsTable:
      .byte $fc, $fc, $aa, $ab, $ac, $ad  ;buzzy beetle frame 1        $00
      .byte $fc, $fc, $ae, $af, $b0, $b1  ;             frame 2        $06
      .byte $fc, $a5, $a6, $a7, $a8, $a9  ;koopa troopa frame 1        $0c
      .byte $fc, $a0, $a1, $a2, $a3, $a4  ;             frame 2        $12
      .byte $69, $a5, $6a, $a7, $a8, $a9  ;koopa paratroopa frame 1    $18
      .byte $6b, $a0, $6c, $a2, $a3, $a4  ;                 frame 2    $1e
      .byte $fc, $fc, $96, $97, $98, $99  ;spiny frame 1               $24
      .byte $fc, $fc, $9a, $9b, $9c, $9d  ;      frame 2               $2a
      .byte $fc, $fc, $8f, $8e, $8e, $8f  ;spiny's egg frame 1         $30
      .byte $fc, $fc, $95, $94, $94, $95  ;            frame 2         $36
      .byte $fc, $fc, $dc, $dc, $df, $df  ;bloober frame 1             $3c
      .byte $dc, $dc, $dd, $dd, $de, $de  ;        frame 2             $42
      .byte $fc, $fc, $b2, $b3, $b4, $b5  ;cheep-cheep frame 1         $48
      .byte $fc, $fc, $b6, $b3, $b7, $b5  ;            frame 2         $4e
      .byte $fc, $fc, $70, $71, $72, $73  ;goomba                      $54
      .byte $fc, $fc, $6e, $6e, $6f, $6f  ;koopa shell frame 1 (upside-down)
      .byte $fc, $fc, $6d, $6d, $6f, $6f  ;            frame 2
      .byte $fc, $fc, $6f, $6f, $6e, $6e  ;koopa shell frame 1 (rightsideup)
      .byte $fc, $fc, $6f, $6f, $6d, $6d  ;            frame 2
      .byte $fc, $fc, $f4, $f4, $f5, $f5  ;buzzy beetle shell frame 1 (rightsideup)
      .byte $fc, $fc, $f4, $f4, $f5, $f5  ;                   frame 2
      .byte $fc, $fc, $f5, $f5, $f4, $f4  ;buzzy beetle shell frame 1 (upside-down)
      .byte $fc, $fc, $f5, $f5, $f4, $f4  ;                   frame 2
      .byte $fc, $fc, $fc, $fc, $ef, $ef  ;defeated goomba
      .byte $b9, $b8, $bb, $ba, $bc, $bc  ;lakitu frame 1
      .byte $fc, $fc, $bd, $bd, $bc, $bc  ;       frame 2
      .byte $7a, $7b, $da, $db, $d8, $d8  ;princess
      .byte $cd, $cd, $ce, $ce, $cf, $cf  ;mushroom retainer
      .byte $7d, $7c, $d1, $8c, $d3, $d2  ;hammer bro frame 1
      .byte $7d, $7c, $89, $88, $8b, $8a  ;           frame 2
      .byte $d5, $d4, $e3, $e2, $d3, $d2  ;           frame 3
      .byte $d5, $d4, $e3, $e2, $8b, $8a  ;           frame 4
      .byte $e5, $e5, $e6, $e6, $eb, $eb  ;piranha plant frame 1
      .byte $ec, $ec, $ed, $ed, $ee, $ee  ;              frame 2
      .byte $fc, $fc, $d0, $d0, $d7, $d7  ;podoboo
      .byte $bf, $be, $c1, $c0, $c2, $fc  ;bowser front frame 1
      .byte $c4, $c3, $c6, $c5, $c8, $c7  ;bowser rear frame 1
      .byte $bf, $be, $ca, $c9, $c2, $fc  ;       front frame 2
      .byte $c4, $c3, $c6, $c5, $cc, $cb  ;       rear frame 2
      .byte $fc, $fc, $e8, $e7, $ea, $e9  ;bullet bill
      .byte $f2, $f2, $f3, $f3, $f2, $f2  ;jumpspring frame 1
      .byte $f1, $f1, $f1, $f1, $fc, $fc  ;           frame 2
      .byte $f0, $f0, $fc, $fc, $fc, $fc  ;           frame 3

EnemyGfxTableOffsets:
      .byte $0c, $0c, $00, $0c, $0c, $a8
      ; .byte $54
      .byte METASPRITE_GOOMBA_WALKING_1
      .byte $3c
      .byte $ea, $18, $48, $48, $cc, $c0, $18, $18
      .byte $18, $90, $24, $ff, $48, $9c, $d2, $d8
      .byte $f0, $f6, $fc

EnemyAttributeData:
      .byte $01, $02, $03, $02, $01, $01, $03, $03
      .byte $03, $01, $01, $02, $02, $21, $01, $02
      .byte $01, $01, $02, $ff, $02, $02, $01, $01
      .byte $02, $02, $02

EnemyAnimTimingBMask:
      .byte $08, $18

JumpspringFrameOffsets:
      .byte $18, $19, $1a, $19, $18

EnemyGfxHandler:
      lda Enemy_Y_Position,x      ;get enemy object vertical position
      sta R2 
      lda Enemy_Rel_XPos          ;get enemy object horizontal position
      sta R5                      ;relative to screen
      ; ldy Enemy_SprDataOffset,x
    AllocSpr 6
      sty OriginalOAMOffset
      sty Local_eb                     ;get sprite data offset
      lda #$00
      sta VerticalFlipFlag        ;initialize vertical flip flag by default
      lda Enemy_MovingDir,x
      sta R3                      ;get enemy object moving direction
      lda Enemy_SprAttrib,x
      sta R4                      ;get enemy object sprite attributes
      lda Enemy_ID,x
      cmp #PiranhaPlant           ;is enemy object piranha plant?
      bne CheckForRetainerObj     ;if not, branch
      ldy PiranhaPlant_Y_Speed,x
      bmi CheckForRetainerObj     ;if piranha plant moving upwards, branch
      ldy EnemyFrameTimer,x
      beq CheckForRetainerObj     ;if timer for movement expired, branch
      rts                         ;if all conditions fail, leave

CheckForRetainerObj:
      lda Enemy_State,x           ;store enemy state
      sta Local_ed
      and #%00011111              ;nullify all but 5 LSB and use as Y
      tay
      lda Enemy_ID,x              ;check for mushroom retainer/princess object
      cmp #RetainerObject
      bne CheckForBulletBillCV    ;if not found, branch
      ldy #$00                    ;if found, nullify saved state in Y
      lda #$01                    ;set value that will not be used
      sta R3 
      lda #$15                    ;set value $15 as code for mushroom retainer/princess object

CheckForBulletBillCV:
       cmp #BulletBill_CannonVar   ;otherwise check for bullet bill object
       bne CheckForJumpspring      ;if not found, branch again
       dec R2                      ;decrement saved vertical position
       lda #$03
       ldy EnemyFrameTimer,x       ;get timer for enemy object
       beq SBBAt                   ;if expired, do not set priority bit
       ora #%00100000              ;otherwise do so
SBBAt: sta R4                      ;set new sprite attributes
       ldy #$00                    ;nullify saved enemy state both in Y and in
       sty Local_ed                     ;memory location here
       lda #$08                    ;set specific value to unconditionally branch once

CheckForJumpspring:
      cmp #JumpspringObject        ;check for jumpspring object
      bne CheckForPodoboo
      ldy #$03                     ;set enemy state -2 MSB here for jumpspring object
      ldx JumpspringAnimCtrl       ;get current frame number for jumpspring object
      lda JumpspringFrameOffsets,x ;load data using frame number as offset

CheckForPodoboo:
      sta Local_ef                 ;store saved enemy object value here
      sty Local_ec                 ;and Y here (enemy state -2 MSB if not changed)
      ldx ObjectOffset        ;get enemy object offset
      cmp #$0c                ;check for podoboo object
      bne CheckBowserGfxFlag  ;branch if not found
      lda Enemy_Y_Speed,x     ;if moving upwards, branch
      bmi CheckBowserGfxFlag
      inc VerticalFlipFlag    ;otherwise, set flag for vertical flip

CheckBowserGfxFlag:
             lda BowserGfxFlag   ;if not drawing bowser at all, skip to something else
             beq CheckForGoomba
             ldy #$16            ;if set to 1, draw bowser's front
             cmp #$01
             beq SBwsrGfxOfs
             iny                 ;otherwise draw bowser's rear
SBwsrGfxOfs: sty Local_ef

CheckForGoomba:
          ldy Local_ef               ;check value for goomba object
          cpy #Goomba
          bne CheckBowserFront  ;branch if not found
          lda Enemy_State,x
          cmp #$02              ;check for defeated state
          bcc GmbaAnim          ;if not defeated, go ahead and animate
          ldx #$04              ;if defeated, write new value here
          stx Local_ec
GmbaAnim: and #%00100000        ;check for d5 set in enemy object state 
          ora TimerControl      ;or timer disable flag set
          bne CheckBowserFront  ;if either condition true, do not animate goomba
          lda FrameCounter
          and #%00001000        ;check for every eighth frame
          bne CheckBowserFront
          
          ; lda R3 
          ; eor #%00000011        ;invert bits to flip horizontally every eight frames
          ; sta R3                ;leave alone otherwise

CheckBowserFront:
             lda EnemyAttributeData,y    ;load sprite attribute using enemy object
             ora R4                      ;as offset, and add to bits already loaded
             sta R4 
             lda EnemyGfxTableOffsets,y  ;load value based on enemy object as offset
             tax                         ;save as X
             ldy Local_ec                     ;get previously saved value
             lda BowserGfxFlag
             beq CheckForSpiny           ;if not drawing bowser object at all, skip all of this
             cmp #$01
             bne CheckBowserRear         ;if not drawing front part, branch to draw the rear part
             lda BowserBodyControls      ;check bowser's body control bits
             bpl ChkFrontSte             ;branch if d7 not set (control's bowser's mouth)      
             ldx #$de                    ;otherwise load offset for second frame
ChkFrontSte: lda Local_ed                     ;check saved enemy state
             and #%00100000              ;if bowser not defeated, do not set flag
             beq DrawBowser

FlipBowserOver:
      stx VerticalFlipFlag  ;set vertical flip flag to nonzero

DrawBowser:
      jmp DrawEnemyObject   ;draw bowser's graphics now

CheckBowserRear:
            lda BowserBodyControls  ;check bowser's body control bits
            and #$01
            beq ChkRearSte          ;branch if d0 not set (control's bowser's feet)
            ldx #$e4                ;otherwise load offset for second frame
ChkRearSte: lda Local_ed                 ;check saved enemy state
            and #%00100000          ;if bowser not defeated, do not set flag
            beq DrawBowser
            lda R2                  ;subtract 16 pixels from
            sec                     ;saved vertical coordinate
            sbc #$10
            sta R2 
            jmp FlipBowserOver      ;jump to set vertical flip flag

CheckForSpiny:
        cpx #$24               ;check if value loaded is for spiny
        bne CheckForLakitu     ;if not found, branch
        cpy #$05               ;if enemy state set to $05, do this,
        bne NotEgg             ;otherwise branch
        ldx #$30               ;set to spiny egg offset
        lda #$02
        sta R3                 ;set enemy direction to reverse sprites horizontally
        lda #$05
        sta Local_ec                ;set enemy state
NotEgg: jmp CheckForHammerBro  ;skip a big chunk of this if we found spiny but not in egg

CheckForLakitu:
        cpx #$90                  ;check value for lakitu's offset loaded
        bne CheckUpsideDownShell  ;branch if not loaded
        lda Local_ed
        and #%00100000            ;check for d5 set in enemy state
        bne NoLAFr                ;branch if set
        lda FrenzyEnemyTimer
        cmp #$10                  ;check timer to see if we've reached a certain range
        bcs NoLAFr                ;branch if not
        ldx #$96                  ;if d6 not set and timer in range, load alt frame for lakitu
NoLAFr: jmp CheckDefeatedState    ;skip this next part if we found lakitu but alt frame not needed

CheckUpsideDownShell:
      lda Local_ef                    ;check for enemy object => $04
      cmp #$04
      bcs CheckRightSideUpShell  ;branch if true
      cpy #$02
      bcc CheckRightSideUpShell  ;branch if enemy state < $02
      ldx #$5a                   ;set for upside-down koopa shell by default
      ldy Local_ef
      cpy #BuzzyBeetle           ;check for buzzy beetle object
      bne CheckRightSideUpShell
      ldx #$7e                   ;set for upside-down buzzy beetle shell if found
      inc R2                     ;increment vertical position by one pixel

CheckRightSideUpShell:
      lda Local_ec                ;check for value set here
      cmp #$04               ;if enemy state < $02, do not change to shell, if
      bne CheckForHammerBro  ;enemy state => $02 but not = $04, leave shell upside-down
      ldx #$72               ;set right-side up buzzy beetle shell by default
      inc R2                 ;increment saved vertical position by one pixel
      ldy Local_ef
      cpy #BuzzyBeetle       ;check for buzzy beetle object
      beq CheckForDefdGoomba ;branch if found
      ldx #$66               ;change to right-side up koopa shell if not found
      inc R2                 ;and increment saved vertical position again

CheckForDefdGoomba:
      cpy #Goomba            ;check for goomba object (necessary if previously
      bne CheckForHammerBro  ;failed buzzy beetle object test)
      ldx #$54               ;load for regular goomba
      lda Local_ed                ;note that this only gets performed if enemy state => $02
      and #%00100000         ;check saved enemy state for d5 set
      bne CheckForHammerBro  ;branch if set
      ldx #$8a               ;load offset for defeated goomba
      dec R2                 ;set different value and decrement saved vertical position

CheckForHammerBro:
      ldy ObjectOffset
      lda Local_ef                  ;check for hammer bro object
      cmp #HammerBro
      bne CheckForBloober      ;branch if not found
      lda Local_ed
      beq CheckToAnimateEnemy  ;branch if not in normal enemy state
      and #%00001000
      beq CheckDefeatedState   ;if d3 not set, branch further away
      ldx #$b4                 ;otherwise load offset for different frame
      bne CheckToAnimateEnemy  ;unconditional branch

CheckForBloober:
      cpx #$48                 ;check for cheep-cheep offset loaded
      beq CheckToAnimateEnemy  ;branch if found
      lda EnemyIntervalTimer,y
      cmp #$05
      bcs CheckDefeatedState   ;branch if some timer is above a certain point
      cpx #$3c                 ;check for bloober offset loaded
      bne CheckToAnimateEnemy  ;branch if not found this time
      cmp #$01
      beq CheckDefeatedState   ;branch if timer is set to certain point
      inc R2                   ;increment saved vertical coordinate three pixels
      inc R2 
      inc R2 
      jmp CheckAnimationStop   ;and do something else

CheckToAnimateEnemy:
      lda Local_ef                  ;check for specific enemy objects
      cmp #Goomba
      beq CheckDefeatedState   ;branch if goomba
      cmp #$08
      beq CheckDefeatedState   ;branch if bullet bill (note both variants use $08 here)
      cmp #Podoboo
      beq CheckDefeatedState   ;branch if podoboo
      cmp #$18                 ;branch if => $18
      bcs CheckDefeatedState
      ldy #$00    
      cmp #$15                 ;check for mushroom retainer/princess object
      bne CheckForSecondFrame  ;which uses different code here, branch if not found
      iny                      ;residual instruction
      lda WorldNumber          ;are we on world 8?
      cmp #World8
      bcs CheckDefeatedState   ;if so, leave the offset alone (use princess)
      ldx #$a2                 ;otherwise, set for mushroom retainer object instead
      lda #$03                 ;set alternate state here
      sta Local_ec
      bne CheckDefeatedState   ;unconditional branch

CheckForSecondFrame:
      lda FrameCounter            ;load frame counter
      and EnemyAnimTimingBMask,y  ;mask it (partly residual, one byte not ever used)
      bne CheckDefeatedState      ;branch if timing is off

CheckAnimationStop:
      lda Local_ed                 ;check saved enemy state
      and #%10100000          ;for d7 or d5, or check for timers stopped
      ora TimerControl
      bne CheckDefeatedState  ;if either condition true, branch
      inx
      ; txa
      ; clc
      ; adc #$06                ;add $06 to current enemy offset
      ; tax                     ;to animate various enemy objects

CheckDefeatedState:
      lda Local_ed               ;check saved enemy state
      and #%00100000        ;for d5 set
      beq DrawEnemyObject   ;branch if not set
      lda Local_ef
      cmp #$04              ;check for saved enemy object => $04
      bcc DrawEnemyObject   ;branch if less
      ldy #$01
      sty VerticalFlipFlag  ;set vertical flip flag
      dey
      sty Local_ec               ;init saved value here

DrawEnemyObject:
  ; lda Local_eb                    ;load sprite data offset
  ; ldx ObjectOffset
  txa
  ldx ObjectOffset
  sta ObjectMetasprite+1,x
  ; jsr DrawEnemyObjRow        ;draw six tiles of data
  ; jsr DrawEnemyObjRow        ;into sprite data
  ; jsr DrawEnemyObjRow
  ; ldx ObjectOffset           ;get enemy object offset
  ; ldy Enemy_SprDataOffset,x  ;get sprite data offset
  ldy Local_eb
  lda Local_ef
  cmp #$08                   ;get saved enemy object and check
  bne CheckForVerticalFlip   ;for bullet bill, branch if not found

SkipToOffScrChk:
  ; jmp SprObjectOffscrChk     ;jump if found

CheckForVerticalFlip:
;   lda VerticalFlipFlag       ;check if vertical flip flag is set here
;   beq CheckForESymmetry      ;branch if not
;   lda Sprite_Attributes,y    ;get attributes of first sprite we dealt with
;   ora #%10000000             ;set bit for vertical flip
;   iny
;   iny                        ;increment two bytes so that we store the vertical flip
;   jsr DumpSixSpr             ;in attribute bytes of enemy obj sprite data
;   dey
;   dey                        ;now go back to the Y coordinate offset
;   tya
;   tax                        ;give offset to X
;   lda Local_ef
;   cmp #HammerBro             ;check saved enemy object for hammer bro
;   beq FlipEnemyVertically
;   cmp #Lakitu                ;check saved enemy object for lakitu
;   beq FlipEnemyVertically    ;branch for hammer bro or lakitu
;   cmp #$15
;   bcs FlipEnemyVertically    ;also branch if enemy object => $15
;   txa
;   clc
;   adc #$08                   ;if not selected objects or => $15, set
;   tax                        ;offset in X for next row

; FlipEnemyVertically:
;   lda Sprite_Tilenumber,x     ;load first or second row tiles
;   pha                         ;and save tiles to the stack
;     lda Sprite_Tilenumber+4,x
;     pha
;       lda Sprite_Tilenumber+16,y  ;exchange third row tiles
;       sta Sprite_Tilenumber,x     ;with first or second row tiles
;       lda Sprite_Tilenumber+20,y
;       sta Sprite_Tilenumber+4,x
;     pla                         ;pull first or second row tiles from stack
;     sta Sprite_Tilenumber+20,y  ;and save in third row
;   pla
;   sta Sprite_Tilenumber+16,y

CheckForESymmetry:
  lda BowserGfxFlag           ;are we drawing bowser at all?
  bne SkipToOffScrChk         ;branch if so
  lda Local_ef
  ldx Local_ec                     ;get alternate enemy state
  cmp #$05                    ;check for hammer bro object
  bne ContES
  jmp SprObjectOffscrChk      ;jump if found
ContES:
  cmp #Bloober                ;check for bloober object
  beq MirrorEnemyGfx
  cmp #PiranhaPlant           ;check for piranha plant object
  beq MirrorEnemyGfx
  cmp #Podoboo                ;check for podoboo object
  beq MirrorEnemyGfx          ;branch if either of three are found
  cmp #Spiny                  ;check for spiny object
  bne ESRtnr                  ;branch closer if not found
  cpx #$05                    ;check spiny's state
  bne CheckToMirrorLakitu     ;branch if not an egg, otherwise
ESRtnr:
  cmp #$15                    ;check for princess/mushroom retainer object
  bne SpnySC
  lda #$42                    ;set horizontal flip on bottom right sprite
  sta Sprite_Attributes+20,y  ;note that palette bits were already set earlier
SpnySC:
  cpx #$02                    ;if alternate enemy state set to 1 or 0, branch
  bcc CheckToMirrorLakitu
MirrorEnemyGfx:
  lda BowserGfxFlag           ;if enemy object is bowser, skip all of this
  bne CheckToMirrorLakitu
  lda Sprite_Attributes,y     ;load attribute bits of first sprite
  and #%10100011
  sta Sprite_Attributes,y     ;save vertical flip, priority, and palette bits
  sta Sprite_Attributes+8,y   ;in left sprite column of enemy object OAM data
  sta Sprite_Attributes+16,y
  ora #%01000000              ;set horizontal flip
  cpx #$05                    ;check for state used by spiny's egg
  bne EggExc                  ;if alternate state not set to $05, branch
    ora #%10000000            ;otherwise set vertical flip
EggExc:
  sta Sprite_Attributes+4,y   ;set bits of right sprite column
  sta Sprite_Attributes+12,y  ;of enemy object sprite data
  sta Sprite_Attributes+20,y
  cpx #$04                    ;check alternate enemy state
  bne CheckToMirrorLakitu     ;branch if not $04
  lda Sprite_Attributes+8,y   ;get second row left sprite attributes
  ora #%10000000
  sta Sprite_Attributes+8,y   ;store bits with vertical flip in
  sta Sprite_Attributes+16,y  ;second and third row left sprites
  ora #%01000000
  sta Sprite_Attributes+12,y  ;store with horizontal and vertical flip in
  sta Sprite_Attributes+20,y  ;second and third row right sprites
  bne CheckToMirrorLakitu ; unconditional
; MirrorGoomba:
;   cpx #$02              ;check for defeated state
;   bcs MirrorEnemyGfx
;   ; if its not already defeated, then
;   ; Flip the top left or top right head sprite of a goomba (depending on animation frame)
;   lda $03
;   and #%00000001
;   bne @TopRight
; @TopLeft:
;     lda Sprite_Attributes+8,y
;     eor #%01000000
;     sta Sprite_Attributes+8,y
;     bne CheckToMirrorLakitu ; uncoditional
; @TopRight:
;     lda Sprite_Attributes+12,y
;     eor #%01000000
;     sta Sprite_Attributes+12,y
CheckToMirrorLakitu:
        lda Local_ef                     ;check for lakitu enemy object
        cmp #Lakitu
        bne CheckToMirrorJSpring    ;branch if not found
        lda VerticalFlipFlag
        bne NVFLak                  ;branch if vertical flip flag set
        lda Sprite_Attributes+16,y  ;save vertical flip and palette bits
        and #%10000001              ;in third row left sprite
        sta Sprite_Attributes+16,y
        lda Sprite_Attributes+20,y  ;set horizontal flip and palette bits
        ora #%01000001              ;in third row right sprite
        sta Sprite_Attributes+20,y
        ldx FrenzyEnemyTimer        ;check timer
        cpx #$10
        bcs SprObjectOffscrChk      ;branch if timer has not reached a certain range
        sta Sprite_Attributes+12,y  ;otherwise set same for second row right sprite
        and #%10000001
        sta Sprite_Attributes+8,y   ;preserve vertical flip and palette bits for left sprite
        bcc SprObjectOffscrChk      ;unconditional branch
NVFLak: lda Sprite_Attributes,y     ;get first row left sprite attributes
        and #%10000001
        sta Sprite_Attributes,y     ;save vertical flip and palette bits
        lda Sprite_Attributes+4,y   ;get first row right sprite attributes
        ora #%01000001              ;set horizontal flip and palette bits
        sta Sprite_Attributes+4,y   ;note that vertical flip is left as-is

CheckToMirrorJSpring:
      lda Local_ef                     ;check for jumpspring object (any frame)
      cmp #$18
      bcc SprObjectOffscrChk      ;branch if not jumpspring object at all
      lda #$82
      sta Sprite_Attributes+8,y   ;set vertical flip and palette bits of 
      sta Sprite_Attributes+16,y  ;second and third row left sprites
      ora #%01000000
      sta Sprite_Attributes+12,y  ;set, in addition to those, horizontal flip
      sta Sprite_Attributes+20,y  ;for second and third row right sprites

SprObjectOffscrChk:
         ldx ObjectOffset          ;get enemy buffer offset
         lda Enemy_OffscreenBits   ;check offscreen information
         lsr
         lsr                       ;shift three times to the right
         lsr                       ;which puts d2 into carry
         pha                       ;save to stack
         bcc LcChk                 ;branch if not set
         lda #$04                  ;set for right column sprites
         jsr MoveESprColOffscreen  ;and move them offscreen
LcChk:   pla                       ;get from stack
         lsr                       ;move d3 to carry
         pha                       ;save to stack
         bcc Row3C                 ;branch if not set
         lda #$00                  ;set for left column sprites,
         jsr MoveESprColOffscreen  ;move them offscreen
Row3C:   pla                       ;get from stack again
         lsr                       ;move d5 to carry this time
         lsr
         pha                       ;save to stack again
         bcc Row23C                ;branch if carry not set
         lda #$10                  ;set for third row of sprites
         jsr MoveESprRowOffscreen  ;and move them offscreen
Row23C:  pla                       ;get from stack
         lsr                       ;move d6 into carry
         pha                       ;save to stack
         bcc AllRowC
         lda #$08                  ;set for second and third rows
         jsr MoveESprRowOffscreen  ;move them offscreen
AllRowC: pla                       ;get from stack once more
         lsr                       ;move d7 into carry
         bcc ExEGHandler
         jsr MoveESprRowOffscreen  ;move all sprites offscreen (A should be 0 by now)
         lda Enemy_ID,x
         cmp #Podoboo              ;check enemy identifier for podoboo
         beq ExEGHandler           ;skip this part if found, we do not want to erase podoboo!
         lda Enemy_Y_HighPos,x     ;check high byte of vertical position
         cmp #$02                  ;if not yet past the bottom of the screen, branch
         bne ExEGHandler
         jsr EraseEnemyObject      ;what it says
ExEGHandler:
      rts

DrawEnemyObjRow:
      lda EnemyGraphicsTable,x    ;load two tiles of enemy graphics
      sta R0 
      lda EnemyGraphicsTable+1,x

DrawOneSpriteRow:
      sta R1 
      jmp DrawSpriteObject        ;draw them

MoveESprRowOffscreen:
      clc                         ;add A to enemy object OAM data offset
      adc OriginalOAMOffset
      tay                         ;use as offset
      lda #$f8
      jmp DumpTwoSpr              ;move first row of sprites offscreen

MoveESprColOffscreen:
      clc                         ;add A to enemy object OAM data offset
      adc OriginalOAMOffset
      tay                         ;use as offset
      jsr MoveColOffscreen        ;move first and second row sprites in column offscreen
      sta Sprite_Data+16,y       ;move third row sprite in column offscreen
      rts

.export EnemyGraphicsEngine
EnemyGraphicsEngine:
  lda Enemy_ID,x
  jsr JumpEngine
  ;only objects $00-$14 use this table
  .word ProcessGreenKoopa
  .word Noop
  .word ProcessBuzzyBeetle
  .word ProcessRedKoopa
  .word Noop
  .word ProcessHammerBro
  .word ProcessGoomba
  .word ProcessBlooper
  .word ProcessBulletBill
  .word Noop
  .word ProcessSwimmingCheepCheep
  .word ProcessSwimmingCheepCheep
  .word ProcessPodoboo
  .word ProcessPiranhaPlant
  .word ProcessJumpingParatrooper
  .word ProcessRedFlyingParatrooper
  .word ProcessGreebFlyingParatrooper
  .word ProcessLakitu
  .word ProcessSpiny
  .word Noop
  .word ProcessFlyingCheepCheep

Noop:
  rts

.proc ProcessGoomba
  ldx ObjectOffset
  lda Enemy_State,x
  ldy #METASPRITE_GOOMBA_WALKING_1
  cmp #$02              ;check for defeated state
  bcc GmbaAnim          ;if not defeated, go ahead and animate
    ldy #METASPRITE_GOOMBA_DEAD
    bne Exit
GmbaAnim:
  and #%00100000        ;check for d5 set in enemy object state 
  ora TimerControl      ;or timer disable flag set
  bne Exit              ;if either condition true, do not animate goomba
  lda FrameCounter
  and #%00001000        ;check for every eighth frame
  bne Exit
    ldy #METASPRITE_GOOMBA_WALKING_2
Exit:
  tya
  sta ObjectMetasprite+1,x
  rts
.endproc

ProcessGreenKoopa:
ProcessBuzzyBeetle:
ProcessRedKoopa:
ProcessHammerBro:
ProcessBlooper:
ProcessBulletBill:
ProcessSwimmingCheepCheep:
ProcessPodoboo:
ProcessPiranhaPlant:
ProcessJumpingParatrooper:
ProcessRedFlyingParatrooper:
ProcessGreebFlyingParatrooper:
ProcessLakitu:
ProcessSpiny:
ProcessFlyingCheepCheep:
  ldx ObjectOffset
  lda #0
  sta ObjectMetasprite+1,x
  rts


; GreenKoopa            = $00
; BuzzyBeetle           = $02
; RedKoopa              = $03
; HammerBro             = $05
; Goomba                = $06
; Bloober               = $07
; BulletBill_FrenzyVar  = $08
; GreyCheepCheep        = $0a
; RedCheepCheep         = $0b
; Podoboo               = $0c
; PiranhaPlant          = $0d
; GreenParatroopaJump   = $0e
; RedParatroopa         = $0f
; GreenParatroopaFly    = $10
; Lakitu                = $11
; Spiny                 = $12
; FlyCheepCheepFrenzy   = $14
; FlyingCheepCheep      = $14  .word MoveFlyingCheepCheep

;-------------------------------------------------------------------------------------
;$00-$01 - tile numbers
;$02 - relative Y position
;$03 - horizontal flip flag (not used here)
;$04 - attributes
;$05 - relative X position

DefaultBlockObjTiles:
  .byte BRICK_BUMP_TILE_1, BRICK_BUMP_TILE_1, BRICK_BUMP_TILE_2, BRICK_BUMP_TILE_2

DrawBlock:
           lda Block_Rel_YPos            ;get relative vertical coordinate of block object
           sta R2                        ;store here
           lda Block_Rel_XPos            ;get relative horizontal coordinate of block object
           sta R5                        ;store here
           lda #$03
           sta R4                        ;set attribute byte here
           lsr
           sta R3                        ;set horizontal flip bit here (will not be used)
          ;  ldy Block_SprDataOffset,x     ;get sprite data offset

        AllocSpr 4
           sty OriginalOAMOffset
           ldx #$00                      ;reset X for use as offset to tile data
DBlkLoop:  lda DefaultBlockObjTiles,x    ;get left tile number
           sta R0                        ;set here
           lda DefaultBlockObjTiles+1,x  ;get right tile number
           jsr DrawOneSpriteRow          ;do sub to write tile numbers to first row of sprites
           cpx #$04                      ;check incremented offset
           bne DBlkLoop                  ;and loop back until all four sprites are done
           ldx ObjectOffset              ;get block object offset
          ;  ldy Block_SprDataOffset,x     ;get sprite data offset
           ldy OriginalOAMOffset
           lda AreaType
           cmp #$01                      ;check for ground level type area
           beq ChkRep                    ;if found, branch to next part
           lda #BRICK_BUMP_TILE_2
           sta Sprite_Tilenumber,y       ;otherwise remove brick tiles with lines
           sta Sprite_Tilenumber+4,y     ;and replace then with lineless brick tiles
ChkRep:    lda Block_Metatile,x          ;check replacement metatile
           cmp #$c4                      ;if not used block metatile, then
           bne BlkOffscr                 ;branch ahead to use current graphics
           lda #BLOCK_USED_TILE          ;set A for used block tile
           iny                           ;increment Y to write to tile bytes
           jsr DumpFourSpr               ;do sub to dump into all four sprites
           dey                           ;return Y to original offset
           lda #$03                      ;set palette bits
           ldx AreaType
           dex                           ;check for ground level type area again
           beq SetBFlip                  ;if found, use current palette bits
           lsr                           ;otherwise set to $01
SetBFlip:  ldx ObjectOffset              ;put block object offset back in X
           sta Sprite_Attributes,y       ;store attribute byte as-is in first sprite
           ora #%01000000
           sta Sprite_Attributes+4,y     ;set horizontal flip bit for second sprite
           ora #%10000000
           sta Sprite_Attributes+12,y    ;set both flip bits for fourth sprite
           and #%10000011
           sta Sprite_Attributes+8,y     ;set vertical flip bit for third sprite
BlkOffscr: lda Block_OffscreenBits       ;get offscreen bits for block object
           pha                           ;save to stack
           and #%00000100                ;check to see if d2 in offscreen bits are set
           beq PullOfsB                  ;if not set, branch, otherwise move sprites offscreen
           lda #$f8                      ;move offscreen two OAMs
           sta Sprite_Y_Position+4,y     ;on the right side
           sta Sprite_Y_Position+12,y
PullOfsB:  pla                           ;pull offscreen bits from stack
ChkLeftCo: and #%00001000                ;check to see if d3 in offscreen bits are set
           beq ExDBlk                    ;if not set, branch, otherwise move sprites offscreen

MoveColOffscreen:
        lda #$f8                   ;move offscreen two OAMs
        sta Sprite_Y_Position,y    ;on the left side (or two rows of enemy on either side
        sta Sprite_Y_Position+8,y  ;if branched here from enemy graphics handler)
ExDBlk: rts

;-------------------------------------------------------------------------------------
;$00 - used to hold palette bits for attribute byte or relative X position

DrawBrickChunks:
         lda #$02                   ;set palette bits here
         sta R0 
         lda #$75                   ;set tile number for ball (something residual, likely)
         ldy GameEngineSubroutine
         cpy #$05                   ;if end-of-level routine running,
         beq DChunks                ;use palette and tile number assigned
         lda #$03                   ;otherwise set different palette bits
         sta R0 
         lda #$84                   ;and set tile number for brick chunks
DChunks: 
        AllocSpr 4
         iny                        ;increment to start with tile bytes in OAM
         jsr DumpFourSpr            ;do sub to dump tile number into all four sprites
         lda FrameCounter           ;get frame counter
         asl
         asl
         asl                        ;move low nybble to high
         asl
         and #$c0                   ;get what was originally d3-d2 of low nybble
         ora R0                     ;add palette bits
         iny                        ;increment offset for attribute bytes
         jsr DumpFourSpr            ;do sub to dump attribute data into all four sprites
         dey
         dey                        ;decrement offset to Y coordinate
         lda Block_Rel_YPos         ;get first block object's relative vertical coordinate
         jsr DumpTwoSpr             ;do sub to dump current Y coordinate into two sprites
         lda Block_Rel_XPos         ;get first block object's relative horizontal coordinate
         sta Sprite_X_Position,y    ;save into X coordinate of first sprite
         lda Block_Orig_XPos,x      ;get original horizontal coordinate
         sec
         sbc ScreenLeft_X_Pos       ;subtract coordinate of left side from original coordinate
         sta R0                     ;store result as relative horizontal coordinate of original
         sec
         sbc Block_Rel_XPos         ;get difference of relative positions of original - current
         adc R0                     ;add original relative position to result
         adc #$06                   ;plus 6 pixels to position second brick chunk correctly
         sta Sprite_X_Position+4,y  ;save into X coordinate of second sprite
         lda Block_Rel_YPos+1       ;get second block object's relative vertical coordinate
         sta Sprite_Y_Position+8,y
         sta Sprite_Y_Position+12,y ;dump into Y coordinates of third and fourth sprites
         lda Block_Rel_XPos+1       ;get second block object's relative horizontal coordinate
         sta Sprite_X_Position+8,y  ;save into X coordinate of third sprite
         lda R0                     ;use original relative horizontal position
         sec
         sbc Block_Rel_XPos+1       ;get difference of relative positions of original - current
         adc R0                     ;add original relative position to result
         adc #$06                   ;plus 6 pixels to position fourth brick chunk correctly
         sta Sprite_X_Position+12,y ;save into X coordinate of fourth sprite
         lda Block_OffscreenBits    ;get offscreen bits for block object
         jsr ChkLeftCo              ;do sub to move left half of sprites offscreen if necessary
         lda Block_OffscreenBits    ;get offscreen bits again
         asl                        ;shift d7 into carry
         bcc ChnkOfs                ;if d7 not set, branch to last part
         lda #$f8
         jsr DumpTwoSpr             ;otherwise move top sprites offscreen
ChnkOfs: lda R0                     ;if relative position on left side of screen,
         bpl ExBCDr                 ;go ahead and leave
         lda Sprite_X_Position,y    ;otherwise compare left-side X coordinate
         cmp Sprite_X_Position+4,y  ;to right-side X coordinate
         bcc ExBCDr                 ;branch to leave if less
         lda #$f8                   ;otherwise move right half of sprites offscreen
         sta Sprite_Y_Position+4,y
         sta Sprite_Y_Position+12,y
ExBCDr:  rts                        ;leave

;-------------------------------------------------------------------------------------

DrawFireball:
  AllocSpr 1
      lda Fireball_Rel_YPos      ;get relative vertical coordinate
      sta Sprite_Y_Position,y    ;store as sprite Y coordinate
      lda Fireball_Rel_XPos      ;get relative horizontal coordinate
      sta Sprite_X_Position,y    ;store as sprite X coordinate, then do shared code

DrawSingleFireball:
       lda FrameCounter         ;get frame counter
       lsr                      ;divide by four
       lsr
       pha                      ;save result to stack
       and #$01                 ;mask out all but last bit
       eor #FIREBALL_TILE1                 ;set either tile $64 or $65 as fireball tile
       sta Sprite_Tilenumber,y  ;thus tile changes every four frames
       pla                      ;get from stack
       lsr                      ;divide by four again
       lsr
       lda #$02                 ;load value $02 to set palette in attrib byte
       bcc FireA                ;if last bit shifted out was not set, skip this
       ora #%11000000           ;otherwise flip both ways every eight frames
FireA: sta Sprite_Attributes,y  ;store attribute byte and leave
       rts

;-------------------------------------------------------------------------------------

DrawSmallPlatform:
      ;  ldy Enemy_SprDataOffset,x   ;get OAM data offset
    AllocSpr 6
       lda #PLATFORM_GIRDER        ;load tile number for small platforms
       iny                         ;increment offset for tile numbers
       jsr DumpSixSpr              ;dump tile number into all six sprites
       iny                         ;increment offset for attributes
       lda #$02                    ;load palette controls
       jsr DumpSixSpr              ;dump attributes into all six sprites
       dey                         ;decrement for original offset
       dey
       lda Enemy_Rel_XPos          ;get relative horizontal coordinate
       sta Sprite_X_Position,y
       sta Sprite_X_Position+12,y  ;dump as X coordinate into first and fourth sprites
       clc
       adc #$08                    ;add eight pixels
       sta Sprite_X_Position+4,y   ;dump into second and fifth sprites
       sta Sprite_X_Position+16,y
       clc
       adc #$08                    ;add eight more pixels
       sta Sprite_X_Position+8,y   ;dump into third and sixth sprites
       sta Sprite_X_Position+20,y
       lda Enemy_Y_Position,x      ;get vertical coordinate
       tax
       pha                         ;save to stack
       cpx #$20                    ;if vertical coordinate below status bar,
       bcs TopSP                   ;do not mess with it
       lda #$f8                    ;otherwise move first three sprites offscreen
TopSP: jsr DumpThreeSpr            ;dump vertical coordinate into Y coordinates
       pla                         ;pull from stack
       clc
       adc #$80                    ;add 128 pixels
       tax
       cpx #$20                    ;if below status bar (taking wrap into account)
       bcs BotSP                   ;then do not change altered coordinate
       lda #$f8                    ;otherwise move last three sprites offscreen
BotSP: sta Sprite_Y_Position+12,y  ;dump vertical coordinate + 128 pixels
       sta Sprite_Y_Position+16,y  ;into Y coordinates
       sta Sprite_Y_Position+20,y
       lda Enemy_OffscreenBits     ;get offscreen bits
       pha                         ;save to stack
       and #%00001000              ;check d3
       beq SOfs
       lda #$f8                    ;if d3 was set, move first and
       sta Sprite_Y_Position,y     ;fourth sprites offscreen
       sta Sprite_Y_Position+12,y
SOfs:  pla                         ;move out and back into stack
       pha
       and #%00000100              ;check d2
       beq SOfs2
       lda #$f8                    ;if d2 was set, move second and
       sta Sprite_Y_Position+4,y   ;fifth sprites offscreen
       sta Sprite_Y_Position+16,y
SOfs2: pla                         ;get from stack
       and #%00000010              ;check d1
       beq ExSPl
       lda #$f8                    ;if d1 was set, move third and
       sta Sprite_Y_Position+8,y   ;sixth sprites offscreen
       sta Sprite_Y_Position+20,y
ExSPl: ldx ObjectOffset            ;get enemy object offset and leave
       rts

;-------------------------------------------------------------------------------------
;$00-$01 - tile numbers
;$02 - Y coordinate
;$03 - flip control
;$04 - sprite attributes
;$05 - X coordinate


.proc DrawSpriteObject
  lda R3                     ;get saved flip control bits
  lsr
  lsr                        ;move d1 into carry
  lda R0 
  bcc NoHFlip                ;if d1 not set, branch
  sta Sprite_Tilenumber+4,y  ;store first tile into second sprite
  lda R1                     ;and second into first sprite
  sta Sprite_Tilenumber,y
  lda #$40                   ;activate horizontal flip OAM attribute
  bne SetHFAt                ;and unconditionally branch
NoHFlip:
  sta Sprite_Tilenumber,y    ;store first tile into first sprite
  lda R1                     ;and second into second sprite
  sta Sprite_Tilenumber+4,y
  lda #$00                   ;clear bit for horizontal flip
SetHFAt:
  ora R4                     ;add other OAM attributes if necessary
  sta Sprite_Attributes,y    ;store sprite attributes
  sta Sprite_Attributes+4,y
  lda R2                     ;now the y coordinates
  sta Sprite_Y_Position,y    ;note because they are
  sta Sprite_Y_Position+4,y  ;side by side, they are the same
  lda R5        
  sta Sprite_X_Position,y    ;store x coordinate, then
  clc                        ;add 8 pixels and store another to
  adc #$08                   ;put them side by side
  sta Sprite_X_Position+4,y
  lda R2                     ;add eight pixels to the next y
  clc                        ;coordinate
  adc #$08
  sta R2 
  tya                        ;add eight to the offset in Y to
  clc                        ;move to the next two sprites
  adc #$08
  tay
  inx                        ;increment offset to return it to the
  inx                        ;routine that called this subroutine
  rts
.endproc


;-------------------------------------------------------------------------------------
.proc FloateyNumbersRoutine
  lda FloateyNum_Control,x     ;load control for floatey number
  beq EndFloateyNumber         ;if zero, branch to leave
  cmp #$0b                     ;if less than $0b, branch
  bcc ChkNumTimer
  lda #$0b                     ;otherwise set to $0b, thus keeping
  sta FloateyNum_Control,x     ;it in range
ChkNumTimer:
  tay                          ;use as Y
  lda FloateyNum_Timer,x       ;check value here
  bne DecNumTimer              ;if nonzero, branch ahead
  sta FloateyNum_Control,x     ;initialize floatey number control and leave
EndFloateyNumber:
  rts
DecNumTimer:
  dec FloateyNum_Timer,x       ;decrement value here
  cmp #$2b                     ;if not reached a certain point, branch  
  bne ChkTallEnemy
  cpy #$0b                     ;check offset for $0b
  bne LoadNumTiles             ;branch ahead if not found
  inc NumberofLives            ;give player one extra life (1-up)
  lda #Sfx_ExtraLife
  sta Square2SoundQueue        ;and play the 1-up sound
LoadNumTiles:
  lda ScoreUpdateData,y        ;load point value here
  lsr                          ;move high nybble to low
  lsr
  lsr
  lsr
  tax                          ;use as X offset, essentially the digit
  lda ScoreUpdateData,y        ;load again and this time
  and #%00001111               ;mask out the high nybble
  sta DigitModifier,x          ;store as amount to add to the digit
  jsr AddToScore               ;update the score accordingly
ChkTallEnemy:
AllocSpr 2
  ; ldy Enemy_SprDataOffset,x    ;get OAM data offset for enemy object
  lda Enemy_ID,x               ;get enemy object identifier
  cmp #Spiny
  beq FloateyPart              ;branch if spiny
  cmp #PiranhaPlant
  beq FloateyPart              ;branch if piranha plant
  cmp #HammerBro
  beq GetAltOffset             ;branch elsewhere if hammer bro
  cmp #GreyCheepCheep
  beq FloateyPart              ;branch if cheep-cheep of either color
  cmp #RedCheepCheep
  beq FloateyPart
  cmp #TallEnemy
  bcs GetAltOffset             ;branch elsewhere if enemy object => $09
  lda Enemy_State,x
  cmp #$02                     ;if enemy state defeated or otherwise
  bcs FloateyPart              ;$02 or greater, branch beyond this part
GetAltOffset:
  ; ldx SprDataOffset_Ctrl       ;load some kind of control bit
  ; ldy Alt_SprDataOffset,x      ;get alternate OAM data offset
  ldx ObjectOffset             ;get enemy object offset again
FloateyPart:
  lda FloateyNum_Y_Pos,x       ;get vertical coordinate for
  cmp #$18                     ;floatey number, if coordinate in the
  bcc SetupNumSpr              ;status bar, branch
  sbc #$01
  sta FloateyNum_Y_Pos,x       ;otherwise subtract one and store as new
SetupNumSpr:
  lda FloateyNum_Y_Pos,x       ;get vertical coordinate
  sbc #$08                     ;subtract eight and dump into the
  jsr DumpTwoSpr               ;left and right sprite's Y coordinates
  lda FloateyNum_X_Pos,x       ;get horizontal coordinate
  sta Sprite_X_Position,y      ;store into X coordinate of left sprite
  clc
  adc #$08                     ;add eight pixels and store into X
  sta Sprite_X_Position+4,y    ;coordinate of right sprite
  lda #$02
  sta Sprite_Attributes,y      ;set palette control in attribute bytes
  sta Sprite_Attributes+4,y    ;of left and right sprites
  lda FloateyNum_Control,x
  asl                          ;multiply our floatey number control by 2
  tax                          ;and use as offset for look-up table
  lda FloateyNumTileData,x
  sta Sprite_Tilenumber,y      ;display first half of number of points
  lda FloateyNumTileData+1,x
  sta Sprite_Tilenumber+4,y    ;display the second half
  ldx ObjectOffset             ;get enemy object offset and leave
  rts

;data is used as tiles for numbers
;that appear when you defeat enemies
FloateyNumTileData:
  .byte $ff, $ff ;dummy
  .byte FLOATEY_NUM_10, FLOATEY_NUM_0 ; "100"
  .byte FLOATEY_NUM_20, FLOATEY_NUM_0 ; "200"
  .byte FLOATEY_NUM_40, FLOATEY_NUM_0 ; "400"
  .byte FLOATEY_NUM_50, FLOATEY_NUM_0 ; "500"
  .byte FLOATEY_NUM_80, FLOATEY_NUM_0 ; "800"
  .byte FLOATEY_NUM_10, FLOATEY_NUM_00 ; "1000"
  .byte FLOATEY_NUM_20, FLOATEY_NUM_00 ; "2000"
  .byte FLOATEY_NUM_40, FLOATEY_NUM_00 ; "4000"
  .byte FLOATEY_NUM_50, FLOATEY_NUM_00 ; "5000"
  .byte FLOATEY_NUM_80, FLOATEY_NUM_00 ; "8000"
  .byte FLOATEY_NUM_1, FLOATEY_NUM_UP ; "1-UP"

;high nybble is digit number, low nybble is number to
;add to the digit of the player's score
ScoreUpdateData:
  .byte $ff ;dummy
  .byte $41, $42, $44, $45, $48
  .byte $31, $32, $34, $35, $38, $00

.endproc

;-------------------------------------------------------------------------------------
;$00-$01 - used to hold tile numbers ($01 addressed in draw floatey number part)
;$02 - used to hold Y coordinate for floatey number
;$03 - residual byte used for flip (but value set here affects nothing)
;$04 - attribute byte for floatey number
;$05 - used as X coordinate for floatey number


FlagpoleGfxHandler:
    AllocSpr 6
    sty OriginalOAMOffset
      lda Enemy_Rel_XPos             ;get relative horizontal coordinate
      sta Sprite_X_Position,y        ;store as X coordinate for first sprite
      clc
      adc #$08                       ;add eight pixels and store
      sta Sprite_X_Position+4,y      ;as X coordinate for second and third sprites
      sta Sprite_X_Position+8,y
      clc
      adc #$0c                       ;add twelve more pixels and
      sta R5                         ;store here to be used later by floatey number
      lda Enemy_Y_Position,x         ;get vertical coordinate
      jsr DumpTwoSpr                 ;and do sub to dump into first and second sprites
      adc #$08                       ;add eight pixels
      sta Sprite_Y_Position+8,y      ;and store into third sprite
      lda FlagpoleFNum_Y_Pos         ;get vertical coordinate for floatey number
      sta R2                         ;store it here
      lda #$01
      sta R3                         ;set value for flip which will not be used, and
      sta R4                         ;attribute byte for floatey number
      sta Sprite_Attributes,y        ;set attribute bytes for all three sprites
      sta Sprite_Attributes+4,y
      sta Sprite_Attributes+8,y
      lda #GOAL_FLAG_TRIANGLE
      sta Sprite_Tilenumber,y        ;put triangle shaped tile
      sta Sprite_Tilenumber+8,y      ;into first and third sprites
      lda #GOAL_FLAG_SKULL
      sta Sprite_Tilenumber+4,y      ;put skull tile into second sprite
      lda FlagpoleCollisionYPos      ;get vertical coordinate at time of collision
      beq ChkFlagOffscreen           ;if zero, branch ahead
      tya
      clc                            ;add 12 bytes to sprite data offset
      adc #$0c
      tay                            ;put back in Y
      lda FlagpoleScore              ;get offset used to award points for touching flagpole
      asl                            ;multiply by 2 to get proper offset here
      tax
      lda FlagpoleScoreNumTiles,x    ;get appropriate tile data
      sta R0 
      lda FlagpoleScoreNumTiles+1,x
      jsr DrawOneSpriteRow           ;use it to render floatey number

ChkFlagOffscreen:
      ldx ObjectOffset               ;get object offset for flag
      ldy OriginalOAMOffset
      lda Enemy_OffscreenBits        ;get offscreen bits
      and #%00001110                 ;mask out all but d3-d1
      beq ExitDumpSpr                ;if none of these bits set, branch to leave

;-------------------------------------------------------------------------------------

MoveSixSpritesOffscreen:
  lda #$f8                  ;set offscreen coordinate if jumping here

DumpSixSpr:
  sta Sprite_Data+20,y      ;dump A contents
  sta Sprite_Data+16,y      ;into third row sprites

.export DumpFourSpr
DumpFourSpr:
  sta Sprite_Data+12,y      ;into second row sprites

DumpThreeSpr:
  sta Sprite_Data+8,y

DumpTwoSpr:
  sta Sprite_Data+4,y       ;and into first row sprites
  sta Sprite_Data,y

ExitDumpSpr:
  rts

FlagpoleScoreNumTiles:
  .byte FLOATEY_NUM_50, FLOATEY_NUM_00
  .byte FLOATEY_NUM_20, FLOATEY_NUM_00
  .byte FLOATEY_NUM_80, FLOATEY_NUM_0
  .byte FLOATEY_NUM_40, FLOATEY_NUM_0
  .byte FLOATEY_NUM_10, FLOATEY_NUM_0

