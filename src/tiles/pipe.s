.include "common.inc"
.include "level.inc"

.import InitPiranhaPlant

.segment "LEVEL"

;--------------------------------

WaterPipe:
      jsr GetLrgObjAttrib     ;get row and lower nybble
      ; ldy AreaObjectLength,x  ;get length (residual code, water pipe is 1 col thick)
      ldx R7                  ;get row
      lda #$6b
      sta MetatileBuffer,x    ;draw something here and below it
      lda #$6c
      sta MetatileBuffer+1,x
      rts

;--------------------------------
;$05 - used to store length of vertical shaft in RenderSidewaysPipe
;$06 - used to store leftover horizontal length in RenderSidewaysPipe
; and vertical length in VerticalPipe and GetPipeHeight

IntroPipe:
               ldy #$03                 ;check if length set, if not set, set it
               jsr ChkLrgObjFixedLength
               ldy #$0a                 ;set fixed value and render the sideways part
               jsr RenderSidewaysPipe
               bcs NoBlankP             ;if carry flag set, not time to draw vertical pipe part
               ldx #$06                 ;blank everything above the vertical pipe part
VPipeSectLoop: lda #$00                 ;all the way to the top of the screen
               sta MetatileBuffer,x     ;because otherwise it will look like exit pipe
               dex
               bpl VPipeSectLoop
               lda VerticalPipeData,y   ;draw the end of the vertical pipe part
               sta MetatileBuffer+7
NoBlankP:      rts

SidePipeShaftData:
      .byte $15, $14  ;used to control whether or not vertical pipe shaft
      .byte $00, $00  ;is drawn, and if so, controls the metatile number
SidePipeTopPart:
      .byte $15, $1e  ;top part of sideways part of pipe
      .byte $1d, $1c
SidePipeBottomPart: 
      .byte $15, $21  ;bottom part of sideways part of pipe
      .byte $20, $1f

ExitPipe:
      ldy #$03                 ;check if length set, if not set, set it
      jsr ChkLrgObjFixedLength
      jsr GetLrgObjAttrib      ;get vertical length, then plow on through RenderSidewaysPipe

RenderSidewaysPipe:
              dey                       ;decrement twice to make room for shaft at bottom
              dey                       ;and store here for now as vertical length
              sty R5 
              ldy AreaObjectLength,x    ;get length left over and store here
              sty R6 
              ldx R5                    ;get vertical length plus one, use as buffer offset
              inx
              lda SidePipeShaftData,y   ;check for value $00 based on horizontal offset
              cmp #$00
              beq DrawSidePart          ;if found, do not draw the vertical pipe shaft
              ldx #$00
              ldy R5                    ;init buffer offset and get vertical length
              jsr RenderUnderPart       ;and render vertical shaft using tile number in A
              clc                       ;clear carry flag to be used by IntroPipe
DrawSidePart: ldy R6                    ;render side pipe part at the bottom
              lda SidePipeTopPart,y
              sta MetatileBuffer,x      ;note that the pipe parts are stored
              lda SidePipeBottomPart,y  ;backwards horizontally
              sta MetatileBuffer+1,x
              rts

VerticalPipeData:
      .byte $11, $10 ;used by pipes that lead somewhere
      .byte $15, $14
      .byte $13, $12 ;used by decoration pipes
      .byte $15, $14

VerticalPipe:
          jsr GetPipeHeight
          lda R0                   ;check to see if value was nullified earlier
          beq WarpPipe             ;(if d3, the usage control bit of second byte, was set)
          iny
          iny
          iny
          iny                      ;add four if usage control bit was not set
WarpPipe: tya                      ;save value in stack
          pha
          lda AreaNumber
          ora WorldNumber          ;if at world 1-1, do not add piranha plant ever
          beq DrawPipe
          ldy AreaObjectLength,x   ;if on second column of pipe, branch
          beq DrawPipe             ;(because we only need to do this once)
          jsr FindEmptyEnemySlot   ;check for an empty moving data buffer space
          bcs DrawPipe             ;if not found, too many enemies, thus skip
          jsr GetAreaObjXPosition  ;get horizontal pixel coordinate
          clc
          adc #$08                 ;add eight to put the piranha plant in the center
          sta Enemy_X_Position,x   ;store as enemy's horizontal coordinate
          lda CurrentPageLoc       ;add carry to current page number
          adc #$00
          sta Enemy_PageLoc,x      ;store as enemy's page coordinate
          lda #$01
          sta Enemy_Y_HighPos,x
          sta Enemy_Flag,x         ;activate enemy flag
          jsr GetAreaObjYPosition  ;get piranha plant's vertical coordinate and store here
          sta Enemy_Y_Position,x
          lda #PiranhaPlant        ;write piranha plant's value into buffer
          sta Enemy_ID,x
          farcall InitPiranhaPlant
DrawPipe: pla                      ;get value saved earlier and use as Y
          tay
          ldx R7                   ;get buffer offset
          lda VerticalPipeData,y   ;draw the appropriate pipe with the Y we loaded earlier
          sta MetatileBuffer,x     ;render the top of the pipe
          inx
          lda VerticalPipeData+2,y ;render the rest of the pipe
          ldy R6                   ;subtract one from length and render the part underneath
          dey
          jmp RenderUnderPart
      
GetPipeHeight:
      ldy #$01       ;check for length loaded, if not, load
      jsr ChkLrgObjFixedLength ;pipe length of 2 (horizontal)
      jsr GetLrgObjAttrib
      tya            ;get saved lower nybble as height
      and #$07       ;save only the three lower bits as
      sta R6         ;vertical length, then load Y with
      ldy AreaObjectLength,x    ;length left over
      rts

FindEmptyEnemySlot:
              ldx #$00          ;start at first enemy slot
EmptyChkLoop: clc               ;clear carry flag by default
              lda Enemy_Flag,x  ;check enemy buffer for nonzero
              beq ExitEmptyChk  ;if zero, leave
              inx
              cpx #$05          ;if nonzero, check next value
              bne EmptyChkLoop
ExitEmptyChk: rts               ;if all values nonzero, carry flag is set
