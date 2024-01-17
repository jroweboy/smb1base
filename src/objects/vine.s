
.include "common.inc"
.include "object.inc"

; sprite_render.s
.import DrawVine

; collision.s
.import BlockBufferCollision

.segment "OBJECT"

;-------------------------------------------------------------------------------------

Setup_Vine:
        lda #VineObject          ;load identifier for vine object
        sta Enemy_ID,x           ;store in buffer
        lda #$01
        sta Enemy_Flag,x         ;set flag for enemy object buffer
        lda Block_PageLoc,y
        sta Enemy_PageLoc,x      ;copy page location from previous object
        lda Block_X_Position,y
        sta Enemy_X_Position,x   ;copy horizontal coordinate from previous object
        lda Block_Y_Position,y
        sta Enemy_Y_Position,x   ;copy vertical coordinate from previous object
        ldy Vine_FlagOffset       ;load vine flag/offset to next available vine slot
        bne NextVO               ;if set at all, don't bother to store vertical
        sta Vine_Start_Y_Position ;otherwise store vertical coordinate here
NextVO: txa                      ;store object offset to next available vine slot
        sta Vine_ObjOffset,y      ;using vine flag as offset
        inc Vine_FlagOffset       ;increment vine flag offset
        lda #Sfx_GrowVine
        sta Square2SoundQueue    ;load vine grow sound
        rts

;-------------------------------------------------------------------------------------
;$06-$07 - used as address to block buffer data
;$02 - used as vertical high nybble of block buffer offset

VineHeightData:
      .byte $30, $60

VineObjectHandler:
           cpx #$05                  ;check enemy offset for special use slot
           bne ExitVH                ;if not in last slot, branch to leave
           ldy Vine_FlagOffset
           dey                       ;decrement vine flag in Y, use as offset
           lda Vine_Height
           cmp VineHeightData,y      ;if vine has reached certain height,
           beq RunVSubs              ;branch ahead to skip this part
           lda FrameCounter          ;get frame counter
           lsr                       ;shift d1 into carry
           lsr
           bcc RunVSubs              ;if d1 not set (2 frames every 4) skip this part
           lda Enemy_Y_Position+5
           sbc #$01                  ;subtract vertical position of vine
           sta Enemy_Y_Position+5    ;one pixel every frame it's time
           inc Vine_Height            ;increment vine height
RunVSubs:  lda Vine_Height            ;if vine still very small,
           cmp #$08                  ;branch to leave
           bcc ExitVH
           jsr RelativeEnemyPosition ;get relative coordinates of vine,
           jsr GetEnemyOffscreenBits ;and any offscreen bits
           ldy #$00                  ;initialize offset used in draw vine sub
VDrawLoop: jsr DrawVine              ;draw vine
           iny                       ;increment offset
           cpy Vine_FlagOffset        ;if offset in Y and offset here
           bne VDrawLoop             ;do not yet match, loop back to draw more vine
           lda Enemy_OffscreenBits
           and #%00001100            ;mask offscreen bits
           beq WrCMTile              ;if none of the saved offscreen bits set, skip ahead
           dey                       ;otherwise decrement Y to get proper offset again
KillVine:  ldx Vine_ObjOffset,y       ;get enemy object offset for this vine object
           jsr EraseEnemyObject      ;kill this vine object
           dey                       ;decrement Y
           bpl KillVine              ;if any vine objects left, loop back to kill it
           sta Vine_FlagOffset        ;initialize vine flag/offset
           sta Vine_Height            ;initialize vine height
WrCMTile:  lda Vine_Height            ;check vine height
           cmp #$20                  ;if vine small (less than 32 pixels tall)
           bcc ExitVH                ;then branch ahead to leave
           ldx #$06                  ;set offset in X to last enemy slot
           lda #$01                  ;set A to obtain horizontal in $04, but we don't care
           ldy #$1b                  ;set Y to offset to get block at ($04, $10) of coordinates
           jsr BlockBufferCollision  ;do a sub to get block buffer address set, return contents
           ldy R2 
           cpy #$d0                  ;if vertical high nybble offset beyond extent of
           bcs ExitVH                ;current block buffer, branch to leave, do not write
           lda (R6) ,y               ;otherwise check contents of block buffer at 
           bne ExitVH                ;current offset, if not empty, branch to leave
           lda #$26
           sta (R6) ,y               ;otherwise, write climbing metatile to block buffer
ExitVH:    ldx ObjectOffset          ;get enemy object offset and leave
           rts
