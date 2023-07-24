
.include "common.inc"
.segment "FIXED"

.export FIRST_SCANLINE_IRQ
FIRST_SCANLINE_IRQ = (15 * 8 + 7) - 8
SECOND_SCANLINE_IRQ = 95 - 16 ; subtract 16 to account for the latest

; .export TitleScreenIrq
; .proc TitleScreenIrq
;   pha
;   phx
;   phy
;     sta IRQDISABLE
;     ldx IrqNextScanline
;     lda #SECOND_SCANLINE_IRQ
;     sta IRQLATCH
;     sta IRQRELOAD
;     sta IRQENABLE

;     ldy NextBankValuesHi,x
;     lda NextBankValuesLo,x
;     tax
;     ; delay 32 cycles
;     NOP
;     TYA
;     LDY #5
;     DEY
;     BNE *-1
;     TAY
;     inc IrqNextScanline
;     BankCHR0 x
;     BankCHR8 y
;   ply
;   plx
;   pla
;   rti

; NextBankValuesLo:
;   .byte $4c, $50
; NextBankValuesHi:
;   .byte $4e, $50
; ; IrqReloadValues:
; ;   .byte $40, $ff
; .endproc

.segment "TITLE"
.export DrawTitleScreenInternal
.proc DrawTitleScreenInternal

  ; Clear out the vram buffer contents.
  lda #0
  tay
ClearVRLoop: sta VRAM_Buffer1-1,y      ;clear buffer at $0300-$03ff
  iny
  bne ClearVRLoop

  lda #$ff
  sta NmiDisable
  ; wait for NMI so we can disable rendering and start writing the data
  : lda NmiSkipped
    beq :-
  ; set nametable to 0 and increment to horizontal
  lda Mirror_PPUCTRL
  and #%11111000
  sta PPUCTRL
  ; disable rendering sprites and background while we draw everything
  lda Mirror_PPUMASK
  and #%11100111
  sta PPUMASK
  lda PPUSTATUS
  
  ; Load $400 bytes from NametableData into the first nametable (including attributes)
  lda #$20
  sta PPUADDR
  lda #$00
  sta PPUADDR
  ldx #0
.repeat 4, I
.scope
  :
    lda NametableData + (I * $100),x
    sta PPUDATA
    inx
    bne :-
.endscope
.endrepeat
  ; copy $100 bytes into the sprite buffer
  :
    lda SpriteData,x
    sta Sprite_Data,x
    inx
    bne :-

  ; lastly copy the palette data
  lda #$3f
  sta PPUADDR
  lda #$00
  sta PPUADDR
  :
    lda PaletteData,x
    sta PPUDATA
    inx
    cpx #$20
    bne :-

  ; fill the last bit of sprites with clouds
  jsr DrawClouds

  ; Setup the correct bank for the background
  ; BankCHR0 #$48
  ; BankCHR8 #$4a
  ; ; c/e
  ; ; 50/52
  ; ; Title screen sprites starts at 52 i think
  ; BankCHR10 #$52
  ; BankCHR14 #$53
  ; BankCHR18 #$54
  ; ; BankCHR1C #$55
  ; ; specifically bank in the sprite for cloud from the BG
  ; BankCHR1C #$40

; .define BHOP_MAGIC_STRING "8BHP"
;   ; check to see if our sound driver is already in SRAM
;   lda BhopValidation
;   cmp #.strat(BHOP_MAGIC_STRING,0)
;   bne FailedValidation
;   lda BhopValidation+1
;   cmp #.strat(BHOP_MAGIC_STRING,1)
;   bne FailedValidation
;   lda BhopValidation+2
;   cmp #.strat(BHOP_MAGIC_STRING,2)
;   bne FailedValidation
;   lda BhopValidation+3
;   cmp #.strat(BHOP_MAGIC_STRING,3)
;   bne FailedValidation
    jmp Finish

; .import __MUSIC_DRIVER_LOAD__, __MUSIC_DRIVER_RUN__, __MUSIC_DRIVER_SIZE__
FailedValidation:
  ; enable writing to sram
  ; lda #%10000000
  ; sta RAM_PROTECT

; LOAD = IrqR0
; RUN = M0
; SIZE = M2

  ; lda #<__MUSIC_DRIVER_LOAD__
  ; sta LOAD
  ; lda #>__MUSIC_DRIVER_LOAD__
  ; sta LOAD+1
  ; lda #<__MUSIC_DRIVER_RUN__
  ; sta RUN
  ; lda #>__MUSIC_DRIVER_RUN__
  ; sta RUN+1
  ; lda #<__MUSIC_DRIVER_SIZE__
  ; sta SIZE
  ; ldx #>__MUSIC_DRIVER_SIZE__
;   beq LessThan256
;   stx SIZE+1
;   ldy #0
;   :
;       lda (LOAD),y
;       sta (RUN),y
;       iny
;       bne :-
;     inc LOAD+1
;     inc RUN+1
;     dex
;     bne :-
; LessThan256:
;   ldx SIZE
;   beq DoneCopying
;   :
;     lda (LOAD),y
;     sta (RUN),y
;     iny
;     dex
;     bne :-
; DoneCopying:
;   lda #.strat(BHOP_MAGIC_STRING,0)
;   sta BhopValidation
;   lda #.strat(BHOP_MAGIC_STRING,1)
;   sta BhopValidation+1
;   lda #.strat(BHOP_MAGIC_STRING,2)
;   sta BhopValidation+2
;   lda #.strat(BHOP_MAGIC_STRING,3)
;   sta BhopValidation+3
  ; disable writing to sram
  ; lda #%1000000
  ; sta RAM_PROTECT
Finish:
  jsr banked_init
  ; this seems to prevent drawing something else?
  lda #5
  sta VRAM_Buffer_AddrCtrl
  ; skip moving sprites offscreen
  lda #0
  sta Sprite0HitDetectFlag

  ; re-enable NMI and setup the next task
  lda #0
  sta NmiDisable
  ; start the timers for clouds as well
  sta TimerControl
  inc OperMode_Task
  
  rts

NametableData:
  .incbin "../chr/title/latest.nam"
PaletteData:
  .incbin "../chr/title/latest_palette.dat"
SpriteData:
  .incbin "../chr/title/latest.oam"

.endproc

CLOUD_BASE_INDEX = 34
CLOUD_1_INDEX = CLOUD_BASE_INDEX
CLOUD_2_INDEX = CLOUD_BASE_INDEX + 10
CLOUD_3_INDEX = CLOUD_BASE_INDEX + 20
CLOUD_1_SPRITE = CLOUD_1_INDEX * 4
CLOUD_2_SPRITE = CLOUD_2_INDEX * 4
CLOUD_3_SPRITE = CLOUD_3_INDEX * 4
CLOUD_METASPRITE_SIZE = 10 * 4
CLOUD_LAST_SPRITE = CLOUD_METASPRITE_SIZE - 4

.proc MoveClouds
.export MoveClouds
  ; cloud 1: once every 3 frame rules
  ; cloud 2: random 1 to 4 frame rules
  ; cloud 3: random 1 to 2 frame rules
CloudTimer = SideCollisionTimer ; this is a framerule timer thats not used on the title screen

  ; check to see if the timers have expired (the first frame its run they are all expired)
  lda CloudTimer
  bne CloudTwo
    ; reload the CloudTimer
    lda #65
    sta CloudTimer
    ldx #CLOUD_3_SPRITE + CLOUD_LAST_SPRITE
    jsr MoveCloud
CloudTwo:
  lda CloudTimer+1
  bne CloudThree
    lda PseudoRandomBitReg
    and #%00001111
    clc
    adc #55
    sta CloudTimer + 1
    ldx #CLOUD_2_SPRITE + CLOUD_LAST_SPRITE
    jsr MoveCloud
CloudThree:
  lda CloudTimer+2
  bne Exit
    lda PseudoRandomBitReg+1
    and #%00001111
    clc
    adc #55
    sta CloudTimer + 2
    ldx #CLOUD_1_SPRITE + CLOUD_LAST_SPRITE
    jmp MoveCloud
Exit:
  rts

MoveCloud:
  ldy #10 - 1 ; number of cloud sprites
  @Loop:
    dec Sprite_X_Position, x
    dex
    dex
    dex
    dex
    dey
    bpl @Loop
  rts

.endproc

.export DrawClouds
.proc DrawClouds
  ldy #CLOUD_1_SPRITE + CLOUD_LAST_SPRITE
  ldx #9
  :
    lda CloudTile, x
    sta Sprite_Tilenumber, y
    sta Sprite_Tilenumber + CLOUD_METASPRITE_SIZE, y
    sta Sprite_Tilenumber + CLOUD_METASPRITE_SIZE * 2, y
CLOUD_PALETTE = $02
BACKGROUND_PRIORITY = (1 << 5)
    lda #CLOUD_PALETTE | BACKGROUND_PRIORITY
    sta Sprite_Attributes, y
    sta Sprite_Attributes + CLOUD_METASPRITE_SIZE, y
    sta Sprite_Attributes + CLOUD_METASPRITE_SIZE * 2, y

    lda CloudSpriteXOrigin
    clc
    adc CloudDataXOffset, x
    sta Sprite_X_Position, y
    lda CloudSpriteXOrigin + 1
    clc
    adc CloudDataXOffset, x
    sta Sprite_X_Position + CLOUD_METASPRITE_SIZE, y
    lda CloudSpriteXOrigin + 2
    clc
    adc CloudDataXOffset, x
    sta Sprite_X_Position + CLOUD_METASPRITE_SIZE * 2, y

    lda CloudSpriteYOrigin
    clc
    adc CloudDataYOffset, x
    sta Sprite_Y_Position, y
    lda CloudSpriteYOrigin + 1
    clc
    adc CloudDataYOffset, x
    sta Sprite_Y_Position + CLOUD_METASPRITE_SIZE, y
    lda CloudSpriteYOrigin + 2
    clc
    adc CloudDataYOffset, x
    sta Sprite_Y_Position + CLOUD_METASPRITE_SIZE * 2, y

    dey
    dey
    dey
    dey
    dex
    bpl :-
  rts
CloudDataYOffset:
  .byte      $00, $00
  .byte $08, $08, $08, $08
  .byte $10, $10, $10, $10
CloudTile:
  .byte      $f6, $f7
  .byte $f5, $e5, $e5, $f8
  .byte $f9, $fa, $fb, $fc
CloudDataXOffset:
  .byte      $08, $10
  .byte $00, $08, $10, $18
  .byte $00, $08, $10, $18
CloudSpriteYOrigin:
  .byte $23, $16, $09
CloudSpriteXOrigin:
  .byte $16, $d0, $72
.endproc


; .segment "MUSIC_DRIVER"
; .include "bhop/bhop.inc"

.proc banked_init
  ; BankPRGA #.lobyte(.bank(TITLE_MUSIC))
  ; BankPRGC #.lobyte(.bank(TITLE_DPCM))

;   lda #0
;   jsr bhop_init
  
;   lda #1
;   sta BhopInitalized

;   BankPRGA #.lobyte(.bank(TITLE))

  rts
.endproc

; .segment "TITLE_MUSIC"

; bhop_music_data:
;   .include "../audio/angry_title.asm"
; .export bhop_music_data

