
.include "common.inc"
.include "level.inc"

.import RenderAttributeTables, RenderAreaGraphics
.import GetScreenPosition
.import WriteGameText ;

.export AreaParserTaskHandler, GetAreaDataAddrs, ProcLoopCommand, AreaParserTaskLoop

.segment "LEVEL"


;-------------------------------------------------------------------------------------
.export LoadAreaPointer
.proc LoadAreaPointer
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
.endproc

.proc GetAreaDataAddrs
  lda AreaPointer          ;use 2 MSB for Y
  jsr LoadAreaPointer::GetAreaType
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
ReloadHeader:
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


AreaParserTaskLoop:
    jsr AreaParserTaskHandler ;render column set of current area
    lda AreaParserTaskNum     ;check number of tasks
    bne AreaParserTaskLoop              ;if tasks still not all done, do another one
SkipATRender:
  rts

AreaParserTaskHandler:
  ldy AreaParserTaskNum     ;check number of tasks here
  bne DoAPTasks             ;if already set, go ahead
    ldy #$08
    sty AreaParserTaskNum     ;otherwise, set eight by default
DoAPTasks:
  dey
  tya
  jsr AreaParserTasks
  dec AreaParserTaskNum     ;if all tasks not complete do not
  bne SkipATRender          ;render attribute table yet
    jmp RenderAttributeTables

AreaParserTasks:
  jsr JumpEngine
  .word IncrementColumnPos
  .word RenderAreaGraphics
  .word RenderAreaGraphics
  .word AreaParserCore
  .word IncrementColumnPos
  .word RenderAreaGraphics
  .word RenderAreaGraphics
  .word AreaParserCore

;-------------------------------------------------------------------------------------

.proc IncrementColumnPos
  inc CurrentColumnPos     ;increment column where we're at
  lda CurrentColumnPos
  and #%00001111           ;mask out higher nybble
  bne NoColWrap
  sta CurrentColumnPos     ;if no bits left set, wrap back to zero (0-f)
  inc CurrentPageLoc       ;and increment page number where we're at
NoColWrap:
  inc BlockBufferColumnPos ;increment column offset where we're at
  lda BlockBufferColumnPos
  and #%00011111           ;mask out all but 5 LSB (0-1f)
  sta BlockBufferColumnPos ;and save
  rts
.endproc

BSceneDataOffsets:
  .byte $00, $30, $60

BackSceneryData:
  .byte $83, $00, $00, $11, $12, $12, $13, $00 ;clouds
  .byte $00, $51, $52, $53, $00, $00, $00, $00
  .byte $00, $00, $01, $02, $02, $03, $00, $00
  .byte $00, $00, $00, $00, $81, $82, $83, $00
  .byte $00, $00, $00, $51, $52, $53, $41, $42
  .byte $43, $00, $00, $00, $00, $00, $81, $82

  .byte $97, $87, $88, $89, $99, $00, $00, $00 ;mountains and bushes
  .byte $11, $12, $13, $94, $95, $95, $95, $96
  .byte $97, $98, $99, $01, $02, $03, $00, $94
  .byte $95, $96, $00, $11, $12, $12, $12, $13
  .byte $00, $00, $00, $00, $01, $02, $02, $03
  .byte $00, $94, $95, $95, $96, $00, $00, $00

  .byte $11, $12, $12, $13, $00, $00, $00, $00 ;trees and fences
  .byte $00, $00, $00, $9c, $00, $8b, $aa, $aa
  .byte $aa, $aa, $11, $12, $13, $8b, $00, $9c
  .byte $9c, $00, $00, $01, $02, $03, $11, $12
  .byte $12, $13, $00, $00, $00, $00, $aa, $aa
  .byte $9c, $aa, $00, $8b, $00, $01, $02, $03

BackSceneryMetatiles:
  .byte $80, $83, $00 ;cloud left
  .byte $81, $84, $00 ;cloud middle
  .byte $82, $85, $00 ;cloud right
  .byte BUSH_TOPLEFT_METATILE, BUSH_LEFT_METATILE, $00 ;bush left
  .byte BUSH_TOPMIDDLE_METATILE, $03, $00 ;bush middle
  .byte $00, BUSH_RIGHT_METATILE, $00 ;bush right
  .byte $00,                        MOUNTAIN_MIDLEFT_METATILE,  MOUNTAIN_MIDMID_METATILE ;mountain left
  .byte MOUNTAIN_MIDTOP_METATILE,   MOUNTAIN_MIDMID_METATILE,   MOUNTAIN_BOTMID_METATILE ;mountain middle
  .byte MOUNTAIN_TOPRIGHT_METATILE, MOUNTAIN_MIDRIGHT_METATILE, MOUNTAIN_BOTRIGHT_METATILE ;mountain right
  .byte $4d, $00, $00 ;fence
  .byte $0d, $0f, $4e ;tall tree
  .byte $0e, $4e, $4e ;short tree

FSceneDataOffsets:
  .byte $00, $0d, $1a

ForeSceneryData:
  .byte $86, $87, $87, $87, $87, $87, $87   ;in water
  .byte $87, $87, $87, $87, $69, $69

  .byte $00, $00, $00, $00, $00, $45, $47   ;wall
  .byte $47, $47, $47, $47, $00, $00

  .byte $00, $00, $00, $00, $00, $00, $00   ;over water
  .byte $00, $00, $00, $00, $86, $87

TerrainMetatiles:
  .byte $69, CRACKED_BRICK_METATILE, $52, $62

TerrainRenderBits:
  .byte %00000000, %00000000 ;no ceiling or floor
  .byte %00000000, %00011000 ;no ceiling, floor 2
  .byte %00000001, %00011000 ;ceiling 1, floor 2
  .byte %00000111, %00011000 ;ceiling 3, floor 2
  .byte %00001111, %00011000 ;ceiling 4, floor 2
  .byte %11111111, %00011000 ;ceiling 8, floor 2
  .byte %00000001, %00011111 ;ceiling 1, floor 5
  .byte %00000111, %00011111 ;ceiling 3, floor 5
  .byte %00001111, %00011111 ;ceiling 4, floor 5
  .byte %10000001, %00011111 ;ceiling 1, floor 6
  .byte %00000001, %00000000 ;ceiling 1, no floor
  .byte %10001111, %00011111 ;ceiling 4, floor 6
  .byte %11110001, %00011111 ;ceiling 1, floor 9
  .byte %11111001, %00011000 ;ceiling 1, middle 5, floor 2
  .byte %11110001, %00011000 ;ceiling 1, middle 4, floor 2
  .byte %11111111, %00011111 ;completely solid top to bottom

AreaParserCore:
      lda BackloadingFlag       ;check to see if we are starting right of start
      beq RenderSceneryTerrain  ;if not, go ahead and render background, foreground and terrain
      jsr ProcessAreaData       ;otherwise skip ahead and load level data


;-------------------------------------------------------------------------------------
;$00 - used as counter, store for low nybble for background, ceiling byte for terrain
;$01 - used to store floor byte for terrain
;$07 - used to store terrain metatile
;$06-$07 - used to store block buffer address

.import GetBlockBufferAddr, Bitmasks
RenderSceneryTerrain:
          ldx #$0c
          lda #$00
ClrMTBuf: sta MetatileBuffer,x       ;clear out metatile buffer
          dex
          bpl ClrMTBuf
          ldy BackgroundScenery      ;do we need to render the background scenery?
          beq RendFore               ;if not, skip to check the foreground
          lda CurrentPageLoc         ;otherwise check for every third page
ThirdP:   cmp #$03
          bmi RendBack               ;if less than three we're there
          sec
          sbc #$03                   ;if 3 or more, subtract 3 and 
          bpl ThirdP                 ;do an unconditional branch
RendBack: asl                        ;move results to higher nybble
          asl
          asl
          asl
          adc BSceneDataOffsets-1,y  ;add to it offset loaded from here
          adc CurrentColumnPos       ;add to the result our current column position
          tax
          lda BackSceneryData,x      ;load data from sum of offsets
          beq RendFore               ;if zero, no scenery for that part
          pha
          and #$0f                   ;save to stack and clear high nybble
          sec
          sbc #$01                   ;subtract one (because low nybble is $01-$0c)
          sta R0                    ;save low nybble
          asl                        ;multiply by three (shift to left and add result to old one)
          adc R0                    ;note that since d7 was nulled, the carry flag is always clear
          tax                        ;save as offset for background scenery metatile data
          pla                        ;get high nybble from stack, move low
          lsr
          lsr
          lsr
          lsr
          tay                        ;use as second offset (used to determine height)
          lda #$03                   ;use previously saved memory location for counter
          sta R0
SceLoop1: lda BackSceneryMetatiles,x ;load metatile data from offset of (lsb - 1) * 3
          sta MetatileBuffer,y       ;store into buffer from offset of (msb / 16)
          inx
          iny
          cpy #$0b                   ;if at this location, leave loop
          beq RendFore
          dec R0                    ;decrement until counter expires, barring exception
          bne SceLoop1
RendFore: ldx ForegroundScenery      ;check for foreground data needed or not
          beq RendTerr               ;if not, skip this part
          ldy FSceneDataOffsets-1,x  ;load offset from location offset by header value, then
          ldx #$00                   ;reinit X
SceLoop2: lda ForeSceneryData,y      ;load data until counter expires
          beq NoFore                 ;do not store if zero found
          sta MetatileBuffer,x
NoFore:   iny
          inx
          cpx #$0d                   ;store up to end of metatile buffer
          bne SceLoop2
RendTerr: ldy AreaType               ;check world type for water level
          bne TerMTile               ;if not water level, skip this part
          lda WorldNumber            ;check world number, if not world number eight
          cmp #World8                ;then skip this part
          bne TerMTile
          lda #$62                   ;if set as water level and world number eight,
          jmp StoreMT                ;use castle wall metatile as terrain type
TerMTile: lda TerrainMetatiles,y     ;otherwise get appropriate metatile for area type
          ldy CloudTypeOverride      ;check for cloud type override
          beq StoreMT                ;if not set, keep value otherwise
            lda #CLOUD_METATILE      ;use cloud block terrain
StoreMT:  sta R7                    ;store value here
          ldx #$00                   ;initialize X, use as metatile buffer offset
          lda TerrainControl         ;use yet another value from the header
          asl                        ;multiply by 2 and use as yet another offset
          tay
TerrLoop: lda TerrainRenderBits,y    ;get one of the terrain rendering bit data
          sta R0
          iny                        ;increment Y and use as offset next time around
          sty R1
          lda CloudTypeOverride      ;skip if value here is zero
          beq NoCloud2
          cpx #$00                   ;otherwise, check if we're doing the ceiling byte
          beq NoCloud2
          lda R0                    ;if not, mask out all but d3
          and #%00001000
          sta R0
NoCloud2: ldy #$00                   ;start at beginning of bitmasks
TerrBChk: lda Bitmasks,y             ;load bitmask, then perform AND on contents of first byte
          bit R0
          beq NextTBit               ;if not set, skip this part (do not write terrain to buffer)
          lda R7
          sta MetatileBuffer,x       ;load terrain type metatile number and store into buffer here
          cmp #CRACKED_BRICK_METATILE
          bcc NotFloor
          ; and its on the floor (to rule out the axe)
          cpx #$0b
          bne :+
          ; check to see if we are okay to paint over the metatile above
          ; and its on the floor (to rule out the axe)
          lda MetatileBuffer-1,x
          bne NotFloor
            lda #CRACKED_BRICK_NONSOLID
            sta MetatileBuffer-1,x
        :
          cpx #$0c
          bcc NotFloor ; and its on the floor (to rule out the axe)
          inc MetatileBuffer,x
          ; jmp NotFlooor
NotFloor:
NextTBit: inx                        ;continue until end of buffer
          cpx #$0d
          beq RendBBuf               ;if we're at the end, break out of this loop
          lda AreaType               ;check world type for underground area
          cmp #$02
          bne EndUChk                ;if not underground, skip this part
          cpx #$0b
          bcc EndUChk                ;if we're at the bottom of the screen, override
          lda #CRACKED_BRICK_METATILE
          sta R7
EndUChk:  iny                        ;increment bitmasks offset in Y
          cpy #$08
          bne TerrBChk               ;if not all bits checked, loop back    
          ldy R1
          bne TerrLoop               ;unconditional branch, use Y to load next byte
RendBBuf: jsr ProcessAreaData        ;do the area data loading routine now
          lda BlockBufferColumnPos
          jsr GetBlockBufferAddr     ;get block buffer address from where we're at
          ldx #$00
          ldy #$00                   ;init index regs and start at beginning of smaller buffer
ChkMTLow: sty R0
          lda MetatileBuffer,x       ;load stored metatile number
          and #%11000000             ;mask out all but 2 MSB
          asl
          rol                        ;make %xx000000 into %000000xx
          rol
          tay                        ;use as offset in Y
          lda MetatileBuffer,x       ;reload original unmasked value here
          cmp BlockBuffLowBounds,y   ;check for certain values depending on bits set
          bcs StrBlock               ;if equal or greater, branch
          lda #$00                   ;if less, init value before storing
StrBlock: ldy R0                    ;get offset for block buffer
          sta ($06),y                ;store value into block buffer
          tya
          clc                        ;add 16 (move down one row) to offset
          adc #$10
          tay
          inx                        ;increment column value
          cpx #$0d
          bcc ChkMTLow               ;continue until we pass last row, then leave
          rts

;numbers lower than these with the same attribute bits
;will not be stored in the block buffer
BlockBuffLowBounds:
  .byte $10, $51, CLOUD_METATILE, $c0

;-------------------------------------------------------------------------------------
;$00 - used to store area object identifier
;$07 - used as adder to find proper area object code

; .proc AreaDataExpired
;   lda FirstTimeAreaReset
;   bne :+
;     inc FirstTimeAreaReset
; :
;   jmp RdyDecode
; .endproc

ProcessAreaData:
            ldx #$02                 ;start at the end of area object buffer
ProcADLoop: stx ObjectOffset
            lda #$00                 ;reset flag
            sta BehindAreaParserFlag
            ldy AreaDataOffset       ;get offset of area data pointer
            lda (AreaData),y         ;get first byte of area object
            cmp #$fd                 ;if end-of-area, skip all this crap
            beq RdyDecode
Continue:
            lda AreaObjectLength,x   ;check area object buffer flag
            bpl RdyDecode            ;if buffer not negative, branch, otherwise
            iny
            lda (AreaData),y         ;get second byte of area object
            asl                      ;check for page select bit (d7), branch if not set
            bcc Chk1Row13
            lda AreaObjectPageSel    ;check page select
            bne Chk1Row13
            inc AreaObjectPageSel    ;if not already set, set it now
            inc AreaObjectPageLoc    ;and increment page location
Chk1Row13:  dey
            lda (AreaData),y         ;reread first byte of level object
            and #$0f                 ;mask out high nybble
            cmp #$0d                 ;row 13?
            bne Chk1Row14
            iny                      ;if so, reread second byte of level object
            lda (AreaData),y
            dey                      ;decrement to get ready to read first byte
            and #%01000000           ;check for d6 set (if not, object is page control)
            bne CheckRear
            lda AreaObjectPageSel    ;if page select is set, do not reread
            bne CheckRear
            iny                      ;if d6 not set, reread second byte
            lda (AreaData),y
            and #%00011111           ;mask out all but 5 LSB and store in page control
            sta AreaObjectPageLoc
            inc AreaObjectPageSel    ;increment page select
            jmp NextAObj
Chk1Row14:  cmp #$0e                 ;row 14?
            bne CheckRear
            lda BackloadingFlag      ;check flag for saved page number and branch if set
            bne RdyDecode            ;to render the object (otherwise bg might not look right)
CheckRear:  lda AreaObjectPageLoc    ;check to see if current page of level object is
            cmp CurrentPageLoc       ;behind current page of renderer
            bcc SetBehind            ;if so branch
RdyDecode:  jsr DecodeAreaData       ;do sub and do not turn on flag
            jmp ChkLength
SetBehind:  inc BehindAreaParserFlag ;turn on flag if object is behind renderer
NextAObj:   jsr IncAreaObjOffset     ;increment buffer offset and move on
ChkLength:  ldx ObjectOffset         ;get buffer offset
            lda AreaObjectLength,x   ;check object length for anything stored here
            bmi ProcLoopb            ;if not, branch to handle loopback
            dec AreaObjectLength,x   ;otherwise decrement length or get rid of it
ProcLoopb:  dex                      ;decrement buffer offset
            bpl ProcADLoop           ;and loopback unless exceeded buffer
            lda BehindAreaParserFlag ;check for flag set if objects were behind renderer
            bne ProcessAreaData      ;branch if true to load more level data, otherwise
            lda BackloadingFlag      ;check for flag set if starting right of page $00
            bne ProcessAreaData      ;branch if true to load more level data, otherwise leave
EndAParse:  rts

IncAreaObjOffset:
      inc AreaDataOffset    ;increment offset of level pointer
      inc AreaDataOffset
      lda #$00              ;reset page select
      sta AreaObjectPageSel
      rts

DecodeAreaData:
          lda AreaObjectLength,x     ;check current buffer flag
          bmi Chk1stB
          ldy AreaObjOffsetBuffer,x  ;if not, get offset from buffer
Chk1stB:  ldx #$10                   ;load offset of 16 for special row 15
          lda (AreaData),y           ;get first byte of level object again
          cmp #$fd
          beq EndAParse              ;if end of level, leave this routine
          and #$0f                   ;otherwise, mask out low nybble
          cmp #$0f                   ;row 15?
          beq ChkRow14               ;if so, keep the offset of 16
          ldx #$08                   ;otherwise load offset of 8 for special row 12
          cmp #$0c                   ;row 12?
          beq ChkRow14               ;if so, keep the offset value of 8
          ldx #$00                   ;otherwise nullify value by default
ChkRow14: stx R7                    ;store whatever value we just loaded here
          ldx ObjectOffset           ;get object offset again
          cmp #$0e                   ;row 14?
          bne ChkRow13
          lda #$00                   ;if so, load offset with $00
          sta R7
          lda #$2e                   ;and load A with another value
          bne NormObj                ;unconditional branch
ChkRow13: cmp #$0d                   ;row 13?
          bne ChkSRows
          lda #$22                   ;if so, load offset with 34
          sta R7
          iny                        ;get next byte
          lda (AreaData),y
          and #%01000000             ;mask out all but d6 (page control obj bit)
          beq LeavePar               ;if d6 clear, branch to leave (we handled this earlier)
          lda (AreaData),y           ;otherwise, get byte again
          and #%01111111             ;mask out d7
          cmp #$4b                   ;check for loop command in low nybble
          bne Mask2MSB               ;(plus d6 set for object other than page control)
          inc LoopCommand            ;if loop command, set loop command flag
Mask2MSB: and #%00111111             ;mask out d7 and d6
          jmp NormObj                ;and jump
ChkSRows: cmp #$0c                   ;row 12-15?
          bcs SpecObj
          iny                        ;if not, get second byte of level object
          lda (AreaData),y
          and #%01110000             ;mask out all but d6-d4
          bne LrgObj                 ;if any bits set, branch to handle large object
          lda #$16
          sta R7                    ;otherwise set offset of 24 for small object
          lda (AreaData),y           ;reload second byte of level object
          and #%00001111             ;mask out higher nybble and jump
          jmp NormObj
LrgObj:   sta R0                    ;store value here (branch for large objects)
          cmp #$70                   ;check for vertical pipe object
          bne NotWPipe
          lda (AreaData),y           ;if not, reload second byte
          and #%00001000             ;mask out all but d3 (usage control bit)
          beq NotWPipe               ;if d3 clear, branch to get original value
          lda #$00                   ;otherwise, nullify value for warp pipe
          sta R0
NotWPipe: lda R0                    ;get value and jump ahead
          jmp MoveAOId
SpecObj:  iny                        ;branch here for rows 12-15
          lda (AreaData),y
          and #%01110000             ;get next byte and mask out all but d6-d4
MoveAOId: lsr                        ;move d6-d4 to lower nybble
          lsr
          lsr
          lsr
NormObj:  sta R0                    ;store value here (branch for small objects and rows 13 and 14)
          lda AreaObjectLength,x     ;is there something stored here already?
          bpl RunAObj                ;if so, branch to do its particular sub
          lda AreaObjectPageLoc      ;otherwise check to see if the object we've loaded is on the
          cmp CurrentPageLoc         ;same page as the renderer, and if so, branch
          beq InitRear
          ldy AreaDataOffset         ;if not, get old offset of level pointer
          lda (AreaData),y           ;and reload first byte
          and #%00001111
          cmp #$0e                   ;row 14?
          bne LeavePar
          lda BackloadingFlag        ;if so, check backloading flag
          bne StrAObj                ;if set, branch to render object, else leave
LeavePar: rts
InitRear: lda BackloadingFlag        ;check backloading flag to see if it's been initialized
          beq BackColC               ;branch to column-wise check
          lda #$00                   ;if not, initialize both backloading and 
          sta BackloadingFlag        ;behind-renderer flags and leave
          sta BehindAreaParserFlag
          sta ObjectOffset
LoopCmdE: rts
BackColC: ldy AreaDataOffset         ;get first byte again
          lda (AreaData),y
          and #%11110000             ;mask out low nybble and move high to low
          lsr
          lsr
          lsr
          lsr
          cmp CurrentColumnPos       ;is this where we're at?
          bne LeavePar               ;if not, branch to leave
StrAObj:  lda AreaDataOffset         ;if so, load area obj offset and store in buffer
          sta AreaObjOffsetBuffer,x
          jsr IncAreaObjOffset       ;do sub to increment to next object data
RunAObj:  lda R0                    ;get stored value and add offset to it
          clc                        ;then use the jump engine with current contents of A
          adc R7
          jsr JumpEngine

;large objects (rows $00-$0b or 00-11, d6-d4 set)
      .word VerticalPipe         ;used by warp pipes
      .word AreaStyleObject
      .word RowOfBricks
      .word RowOfSolidBlocks
      .word RowOfCoins
      .word ColumnOfBricks
      .word ColumnOfSolidBlocks
      .word VerticalPipe         ;used by decoration pipes

;objects for special row $0c or 12
      .word Hole_Empty
      .word PulleyRopeObject
      .word Bridge_High
      .word Bridge_Middle
      .word Bridge_Low
      .word Hole_Water
      .word QuestionBlockRow_High
      .word QuestionBlockRow_Low

;objects for special row $0f or 15
      .word EndlessRope
      .word BalancePlatRope
      .word CastleObject
      .word StaircaseObject
      .word ExitPipe
      .word FlagBalls_Residual

;small objects (rows $00-$0b or 00-11, d6-d4 all clear)
      .word QuestionBlock     ;power-up
      .word QuestionBlock     ;coin
      .word QuestionBlock     ;hidden, coin
      .word Hidden1UpBlock    ;hidden, 1-up
      .word BrickWithItem     ;brick, power-up
      .word BrickWithItem     ;brick, vine
      .word BrickWithItem     ;brick, star
      .word BrickWithCoins    ;brick, coins
      .word BrickWithItem     ;brick, 1-up
      .word WaterPipe
      .word EmptyBlock
      .word Jumpspring

;objects for special row $0d or 13 (d6 set)
      .word IntroPipe
      .word FlagpoleObject
      .word AxeObj
      .word ChainObj
      .word CastleBridgeObj
      .word ScrollLockObject_Warp
      .word ScrollLockObject
      .word ScrollLockObject
      .word AreaFrenzy            ;flying cheep-cheeps 
      .word AreaFrenzy            ;bullet bills or swimming cheep-cheeps
      .word AreaFrenzy            ;stop frenzy
      .word LoopCmdE

;object for special row $0e or 14
      .word AlterAreaAttributes

;-------------------------------------------------------------------------------------
;(these apply to all area object subroutines in this section unless otherwise stated)
;$00 - used to store offset used to find object code
;$07 - starts with adder from area parser, used to store row offset

AlterAreaAttributes:
         ldy AreaObjOffsetBuffer,x ;load offset for level object data saved in buffer
         iny                       ;load second byte
         lda (AreaData),y
         pha                       ;save in stack for now
         and #%01000000
         bne Alter2                ;branch if d6 is set
         pla
         pha                       ;pull and push offset to copy to A
         and #%00001111            ;mask out high nybble and store as
         sta TerrainControl        ;new terrain height type bits
         pla
         and #%00110000            ;pull and mask out all but d5 and d4
         lsr                       ;move bits to lower nybble and store
         lsr                       ;as new background scenery bits
         lsr
         lsr
         sta BackgroundScenery     ;then leave
         rts
Alter2:  pla
         and #%00000111            ;mask out all but 3 LSB
         cmp #$04                  ;if four or greater, set color control bits
         bcc SetFore               ;and nullify foreground scenery bits
         sta BackgroundColorCtrl
         lda #$00
SetFore: sta ForegroundScenery     ;otherwise set new foreground scenery bits
         rts


;--------------------------------
ScrollLockObject_Warp:
         ldx #$04            ;load value of 4 for game text routine as default
         lda WorldNumber     ;warp zone (4-3-2), then check world number
         beq WarpNum
         inx                 ;if world number > 1, increment for next warp zone (5)
         ldy AreaType        ;check area type
         dey
         bne WarpNum         ;if ground area type, increment for last warp zone
         inx                 ;(8-7-6) and move on
WarpNum: txa
         sta WarpZoneControl ;store number here to be used by warp zone routine
      ;    jsr WriteGameText   ;print text and warp zone numbers
         lda #PiranhaPlant
         jsr KillEnemies     ;load identifier for piranha plants and do sub

ScrollLockObject:
      lda ScrollLock      ;invert scroll lock to turn it on
      eor #%00000001
      sta ScrollLock
      rts


;--------------------------------

FrenzyIDData:
      .byte FlyCheepCheepFrenzy, BBill_CCheep_Frenzy, Stop_Frenzy

AreaFrenzy:  ldx R0               ;use area object identifier bit as offset
             lda FrenzyIDData-8,x  ;note that it starts at 8, thus weird address here
             ldy #$05
FreCompLoop: dey                   ;check regular slots of enemy object buffer
             bmi ExitAFrenzy       ;if all slots checked and enemy object not found, branch to store
             cmp Enemy_ID,y    ;check for enemy object in buffer versus frenzy object
             bne FreCompLoop
             lda #$00              ;if enemy object already present, nullify queue and leave
ExitAFrenzy: sta EnemyFrenzyQueue  ;store enemy into frenzy queue
             rts



;--------------------------------

ChkLrgObjLength:
        jsr GetLrgObjAttrib     ;get row location and size (length if branched to from here)

ChkLrgObjFixedLength:
        lda AreaObjectLength,x  ;check for set length counter
        clc                     ;clear carry flag for not just starting
        bpl LenSet              ;if counter not set, load it, otherwise leave alone
        tya                     ;save length into length counter
        sta AreaObjectLength,x
        sec                     ;set carry flag if just starting
LenSet: rts


GetLrgObjAttrib:
      ldy AreaObjOffsetBuffer,x ;get offset saved from area obj decoding routine
      lda (AreaData),y          ;get first byte of level object
      and #%00001111
      sta R7                   ;save row location
      iny
      lda (AreaData),y          ;get next byte, save lower nybble (length or height)
      and #%00001111            ;as Y, then leave
      tay
      rts

;--------------------------------

GetAreaObjXPosition:
      lda CurrentColumnPos    ;multiply current offset where we're at by 16
      asl                     ;to obtain horizontal pixel coordinate
      asl
      asl
      asl
      rts

;--------------------------------

GetAreaObjYPosition:
      lda R7  ;multiply value by 16
      asl
      asl      ;this will give us the proper vertical pixel coordinate
      asl
      asl
      clc
      adc #32  ;add 32 pixels for the status bar
      rts

;--------------------------------------------
; Enemy loading code

;--------------------------------

;loop command data
LoopCmdWorldNumber:
      .byte $03, $03, $06, $06, $06, $06, $06, $06, $07, $07, $07

LoopCmdPageNumber:
      .byte $05, $09, $04, $05, $06, $08, $09, $0a, $06, $0b, $10

LoopCmdYPosition:
      .byte $40, $b0, $b0, $80, $40, $40, $80, $40, $f0, $f0, $f0

ExecGameLoopback:
      lda Player_PageLoc        ;send player back four pages
      sec
      sbc #$04
      sta Player_PageLoc
      lda CurrentPageLoc        ;send current page back four pages
      sec
      sbc #$04
      sta CurrentPageLoc
      lda ScreenLeft_PageLoc    ;subtract four from page location
      sec                       ;of screen's left border
      sbc #$04
      sta ScreenLeft_PageLoc
      lda ScreenRight_PageLoc   ;do the same for the page location
      sec                       ;of screen's right border
      sbc #$04
      sta ScreenRight_PageLoc
      lda AreaObjectPageLoc     ;subtract four from page control
      sec                       ;for area objects
      sbc #$04
      sta AreaObjectPageLoc
      lda #$00                  ;initialize page select for both
      sta EnemyObjectPageSel    ;area and enemy objects
      sta AreaObjectPageSel
      sta EnemyDataOffset       ;initialize enemy object data offset
      sta EnemyObjectPageLoc    ;and enemy object page control
      lda AreaDataOfsLoopback,y ;adjust area object offset based on
      sta AreaDataOffset        ;which loop command we encountered
      rts

;-------------------------------------------------------------------------------------

AreaDataOfsLoopback:
      .byte $12, $36, $0e, $0e, $0e, $32, $32, $32, $0a, $26, $40

ProcLoopCommand:
          lda LoopCommand           ;check if loop command was found
          beq ChkEnemyFrenzy
          lda CurrentColumnPos      ;check to see if we're still on the first page
          bne ChkEnemyFrenzy        ;if not, do not loop yet
          ldy #$0b                  ;start at the end of each set of loop data
FindLoop: dey
          bmi ChkEnemyFrenzy        ;if all data is checked and not match, do not loop
          lda WorldNumber           ;check to see if one of the world numbers
          cmp LoopCmdWorldNumber,y  ;matches our current world number
          bne FindLoop
          lda CurrentPageLoc        ;check to see if one of the page numbers
          cmp LoopCmdPageNumber,y   ;matches the page we're currently on
          bne FindLoop
          lda Player_Y_Position     ;check to see if the player is at the correct position
          cmp LoopCmdYPosition,y    ;if not, branch to check for world 7
          bne WrongChk
          lda Player_State          ;check to see if the player is
          cmp #$00                  ;on solid ground (i.e. not jumping or falling)
          bne WrongChk              ;if not, player fails to pass loop, and loopback
          lda WorldNumber           ;are we in world 7? (check performed on correct
          cmp #World7               ;vertical position and on solid ground)
          bne InitMLp               ;if not, initialize flags used there, otherwise
          inc MultiLoopCorrectCntr  ;increment counter for correct progression
IncMLoop: inc MultiLoopPassCntr     ;increment master multi-part counter
          lda MultiLoopPassCntr     ;have we done all three parts?
          cmp #$03
          bne InitLCmd              ;if not, skip this part
          lda MultiLoopCorrectCntr  ;if so, have we done them all correctly?
          cmp #$03
          beq InitMLp               ;if so, branch past unnecessary check here
          bne DoLpBack              ;unconditional branch if previous branch fails
WrongChk: lda WorldNumber           ;are we in world 7? (check performed on
          cmp #World7               ;incorrect vertical position or not on solid ground)
          beq IncMLoop
DoLpBack: jsr ExecGameLoopback      ;if player is not in right place, loop back
          farcall KillAllEnemies
InitMLp:  lda #$00                  ;initialize counters used for multi-part loop commands
          sta MultiLoopPassCntr
          sta MultiLoopCorrectCntr
InitLCmd: lda #$00                  ;initialize loop command flag
          sta LoopCommand
          ; fallthrough
;--------------------------------

ChkEnemyFrenzy:
  lda EnemyFrenzyQueue  ;check for enemy object in frenzy queue
  beq ProcessEnemyData  ;if not, skip this part
  sta Enemy_ID,x        ;store as enemy object identifier here
  lda #$01
  sta Enemy_Flag,x      ;activate enemy object flag
  lda #$00
  sta Enemy_State,x     ;initialize state and frenzy queue
  sta EnemyFrenzyQueue
  jmp InitEnemyObject   ;and then jump to deal with this enemy

;--------------------------------
;$06 - used to hold page location of extended right boundary
;$07 - used to hold high nybble of position of extended right boundary

ProcessEnemyData:
  ldy EnemyDataOffset      ;get offset of enemy object data
  lda (EnemyData),y        ;load first byte
  cmp #$ff                 ;check for EOD terminator
  bne CheckEndofBuffer
  jmp CheckFrenzyBuffer    ;if found, jump to check frenzy buffer, otherwise

CheckEndofBuffer:
  and #%00001111           ;check for special row $0e
  cmp #$0e
  beq CheckRightBounds     ;if found, branch, otherwise
  ; Changed to allow enemies into slot 6 (used to be reserved for powerups)
  cpx #$06                 ;check for end of buffer
  bcc CheckRightBounds     ;if not at end of buffer, branch
  iny
  lda (EnemyData),y        ;check for specific value here
  and #%00111111           ;not sure what this was intended for, exactly
  cmp #$2e                 ;this part is quite possibly residual code
  beq CheckRightBounds     ;but it has the effect of keeping enemies out of
  rts                      ;the sixth slot

CheckRightBounds:
  lda ScreenRight_X_Pos    ;add 48 to pixel coordinate of right boundary
  clc
  adc #$30
  and #%11110000           ;store high nybble
  sta R7
  lda ScreenRight_PageLoc  ;add carry to page location of right boundary
  adc #$00
  sta R6                  ;store page location + carry
  ldy EnemyDataOffset
  iny
  lda (EnemyData),y        ;if MSB of enemy object is clear, branch to check for row $0f
  asl
  bcc CheckPageCtrlRow
  lda EnemyObjectPageSel   ;if page select already set, do not set again
  bne CheckPageCtrlRow
  inc EnemyObjectPageSel   ;otherwise, if MSB is set, set page select 
  inc EnemyObjectPageLoc   ;and increment page control

CheckPageCtrlRow:
  dey
  lda (EnemyData),y        ;reread first byte
  and #$0f
  cmp #$0f                 ;check for special row $0f
  bne PositionEnemyObj     ;if not found, branch to position enemy object
  lda EnemyObjectPageSel   ;if page select set,
  bne PositionEnemyObj     ;branch without reading second byte
  iny
  lda (EnemyData),y        ;otherwise, get second byte, mask out 2 MSB
  and #%00111111
  sta EnemyObjectPageLoc   ;store as page control for enemy object data
  inc EnemyDataOffset      ;increment enemy object data offset 2 bytes
  inc EnemyDataOffset
  inc EnemyObjectPageSel   ;set page select for enemy object data and 
  jmp ProcLoopCommand      ;jump back to process loop commands again

PositionEnemyObj:
  lda EnemyObjectPageLoc   ;store page control as page location
  sta Enemy_PageLoc,x      ;for enemy object
  lda (EnemyData),y        ;get first byte of enemy object
  and #%11110000
  sta Enemy_X_Position,x   ;store column position
  cmp ScreenRight_X_Pos    ;check column position against right boundary
  lda Enemy_PageLoc,x      ;without subtracting, then subtract borrow
  sbc ScreenRight_PageLoc  ;from page location
  bcs CheckRightExtBounds  ;if enemy object beyond or at boundary, branch
  lda (EnemyData),y
  and #%00001111           ;check for special row $0e
  cmp #$0e                 ;if found, jump elsewhere
  jeq ParseRow0e
  jmp CheckThreeBytes      ;if not found, unconditional jump

CheckRightExtBounds:
  lda R7                  ;check right boundary + 48 against
  cmp Enemy_X_Position,x   ;column position without subtracting,
  lda R6                  ;then subtract borrow from page control temp
  sbc Enemy_PageLoc,x      ;plus carry
  bcc CheckFrenzyBuffer    ;if enemy object beyond extended boundary, branch
  lda #$01                 ;store value in vertical high byte
  sta Enemy_Y_HighPos,x
  lda (EnemyData),y        ;get first byte again
  asl                      ;multiply by four to get the vertical
  asl                      ;coordinate
  asl
  asl
  sta Enemy_Y_Position,x
  cmp #$e0                 ;do one last check for special row $0e
  beq ParseRow0e           ;(necessary if branched to $c1cb)
  iny
  lda (EnemyData),y        ;get second byte of object
  and #%01000000           ;check to see if hard mode bit is set
  beq CheckForEnemyGroup   ;if not, branch to check for group enemy objects
  lda SecondaryHardMode    ;if set, check to see if secondary hard mode flag
  jeq Inc2B                ;is on, and if not, branch to skip this object completely

CheckForEnemyGroup:
  lda (EnemyData),y      ;get second byte and mask out 2 MSB
  and #%00111111
  cmp #$37               ;check for value below $37
  bcc BuzzyBeetleMutate
  cmp #$3f               ;if $37 or greater, check for value
  bcc DoGroup            ;below $3f, branch if below $3f

BuzzyBeetleMutate:
  cmp #Goomba          ;if below $37, check for goomba
  bne StrID            ;value ($3f or more always fails)
  ldy PrimaryHardMode  ;check if primary hard mode flag is set
  beq StrID            ;and if so, change goomba to buzzy beetle
    lda #BuzzyBeetle
StrID:
  sta Enemy_ID,x       ;store enemy object number into buffer
  lda #$01
  sta Enemy_Flag,x     ;set flag for enemy in buffer
  jsr InitEnemyObject
  lda Enemy_Flag,x     ;check to see if flag is set
  bne Inc2B            ;if not, leave, otherwise branch
ExitStrID:
  rts

CheckFrenzyBuffer:
  ; check first for the custom frenzy-esque disco lakitu
  lda LakituActionBuffer
  bne DiscoLakituSpawnItem
  lda EnemyFrenzyBuffer    ;if enemy object stored in frenzy buffer
  bne StrFre               ;then branch ahead to store in enemy object buffer
  lda Vine_FlagOffset       ;otherwise check vine flag offset
  cmp #$01
  bne ExitStrID               ;if other value <> 1, leave
  lda #VineObject          ;otherwise put vine in enemy identifier
StrFre:
  sta Enemy_ID,x           ;store contents of frenzy buffer into enemy identifier value
InitEnemyObject:
  lda #$00                 ;initialize enemy state
  sta Enemy_State,x
  farcall CheckpointEnemyID, jmp    ;jump ahead to run jump engine and subroutines

DoGroup:
  jmp HandleGroupEnemies   ;handle enemy group objects

ParseRow0e:
  iny                      ;increment Y to load third byte of object
  iny
  lda (EnemyData),y
  lsr                      ;move 3 MSB to the bottom, effectively
  lsr                      ;making %xxx00000 into %00000xxx
  lsr
  lsr
  lsr
  cmp WorldNumber          ;is it the same world number as we're on?
  bne NotUse               ;if not, do not use (this allows multiple uses
  dey                      ;of the same area, like the underground bonus areas)
  lda (EnemyData),y        ;otherwise, get second byte and use as offset
  sta AreaPointer          ;to addresses for level and enemy object data
  iny
  lda (EnemyData),y        ;get third byte again, and this time mask out
  and #%00011111           ;the 3 MSB from before, save as page number to be
  sta EntrancePage         ;used upon entry to area, if area is entered
NotUse:
  jmp Inc3B

CheckThreeBytes:
  ldy EnemyDataOffset      ;load current offset for enemy object data
  lda (EnemyData),y        ;get first byte
  and #%00001111           ;check for special row $0e
  cmp #$0e
  bne Inc2B
Inc3B:
  inc EnemyDataOffset      ;if row = $0e, increment three bytes
Inc2B:
  inc EnemyDataOffset      ;otherwise increment two bytes
  inc EnemyDataOffset
  lda #$00                 ;init page select for enemy objects
  sta EnemyObjectPageSel
  ldx ObjectOffset         ;reload current offset in enemy buffers
  rts                      ;and leave


.proc DiscoLakituSpawnItem
  ; cmp #PowerUpObject
  ; bne LoadEnemy
;     ; enemy object
;     lda #0
;     jmp Exit
    ; lda LakituObjectBuffer
    ; sta Enemy_PowerupType, x
    ; lda #PowerUpObject
    ; sta LakituObjectBuffer
  
; LoadEnemy:
;   .import SetupPowerUp
;   lda LakituObjectBuffer
;   sta PowerUpType
; Exit:

  ; lda LakituObjectBuffer
  ; sta Enemy_PowerupType, x
  ; ; just 
  ; lda #PowerUpObject
  ; sta Enemy_ID,x           ;store contents of frenzy buffer into enemy identifier value
  ; lda #$00
  ; sta Enemy_State,x
  ; sty LakituActionBuffer

  ldy #0
  farcall CreateObject, jmp
  ; lda #Spiny
  ; farcall CheckpointEnemyID, jmp    ;jump ahead to run jump engine and subroutines
.endproc

;--------------------------------
;$00 - used to store Y position of group enemies
;$01 - used to store enemy ID
;$02 - used to store page location of right side of screen
;$03 - used to store X position of right side of screen

HandleGroupEnemies:
        ldy #$00                  ;load value for green koopa troopa
        sec
        sbc #$37                  ;subtract $37 from second byte read
        pha                       ;save result in stack for now
        cmp #$04                  ;was byte in $3b-$3e range?
        bcs SnglID                ;if so, branch
        pha                       ;save another copy to stack
        ldy #Goomba               ;load value for goomba enemy
        lda PrimaryHardMode       ;if primary hard mode flag not set,
        beq PullID                ;branch, otherwise change to value
        ldy #BuzzyBeetle          ;for buzzy beetle
PullID: pla                       ;get second copy from stack
SnglID: sty R1                   ;save enemy id here
        ldy #$b0                  ;load default y coordinate
        and #$02                  ;check to see if d1 was set
        beq SetYGp                ;if so, move y coordinate up,
        ldy #$70                  ;otherwise branch and use default
SetYGp: sty R0                   ;save y coordinate here
        lda ScreenRight_PageLoc   ;get page number of right edge of screen
        sta R2                   ;save here
        lda ScreenRight_X_Pos     ;get pixel coordinate of right edge
        sta R3                   ;save here
        ldy #$02                  ;load two enemies by default
        pla                       ;get first copy from stack
        lsr                       ;check to see if d0 was set
        bcc CntGrp                ;if not, use default value
        iny                       ;otherwise increment to three enemies
CntGrp: sty NumberofGroupEnemies  ;save number of enemies here
GrLoop: ldx #$ff                  ;start at beginning of enemy buffers
GSltLp: inx                       ;increment and branch if past
        cpx #$05                  ;end of buffers
        bcs NextED
        lda Enemy_Flag,x          ;check to see if enemy is already
        bne GSltLp                ;stored in buffer, and branch if so
        lda R1
        sta Enemy_ID,x            ;store enemy object identifier
        lda R2
        sta Enemy_PageLoc,x       ;store page location for enemy object
        lda R3
        sta Enemy_X_Position,x    ;store x coordinate for enemy object
        clc
        adc #$18                  ;add 24 pixels for next enemy
        sta R3
        lda R2                   ;add carry to page location for
        adc #$00                  ;next enemy
        sta R2
        lda R0                   ;store y coordinate for enemy object
        sta Enemy_Y_Position,x
        lda #$01                  ;activate flag for buffer, and
        sta Enemy_Y_HighPos,x     ;put enemy within the screen vertically
        sta Enemy_Flag,x
        farcall CheckpointEnemyID     ;process each enemy object separately
        dec NumberofGroupEnemies  ;do this until we run out of enemy objects
        bne GrLoop
NextED: jmp Inc2B                 ;jump to increment data offset and leave


;-------------------------------------------------------------------------------------
;GAME LEVELS DATA
; .define db byte
.include "./custom_levels.s"
