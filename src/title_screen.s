
.include "common.inc"
.segment "FIXED"

.export BAD_EMULATOR

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

.proc BAD_EMULATOR

  lda PPUSTATUS
  lda #0
  sta PlayPanic
  sta IRQCONTROL
  sta PPUMASK
  sta PPUCTRL

  lda #$20
  sta PPUADDR
  lda #0
  sta PPUADDR
  lda #$24
   ldx #0
   ldy #4 ; 4x256
clear_all:
   sta PPUDATA
   inx
   bne clear_all
   dey
   bne clear_all
  lda #$23
  sta PPUADDR
  ldx #$c0
  stx PPUADDR
  lda #0
clear_all2:
   sta PPUDATA
   inx
   bne clear_all2

  lda #<ErrorScreenMessage
  sta R0
  lda #>ErrorScreenMessage
  sta R1
  ldy #0
NextLine:
    lda (R0),y
    beq WaitForever
    sta PPUADDR
    iny
    lda (R0),y
    sta PPUADDR
    iny
    lda (R0),y
    tax
    iny
  NextLetter:
    lda (R0),y
    iny
    sta PPUDATA
    dex
    bne NextLetter
    iny
    tya
    clc
    adc R0
    sta R0
    lda R1
    adc #0
    sta R1
    ldy #0
    jmp NextLine
WaitForever:
  ; enable rendering and loop forever
  lda #%00001000
  sta PPUMASK
  lda #0
  sta PPUSCROLL
  sta PPUSCROLL
Loop:
  jmp Loop
  rts

ErrorScreenMessage:
  .byte $3f, $00, $04, $0f, $30, $0f, $0f, $00
L1 = * - ErrorScreenMessage
  .byte $20, $6c, L2 - L1 - 4, "E R R O R", $00
L2 = * - ErrorScreenMessage
  .byte $20, $a7, L3 - L2 - 4, "FAULTY IRQ DETECTED", $00
L3 = * - ErrorScreenMessage
  .byte $20, $e2, L4 - L3 - 4, "THIS HACK USES ADVANCED", $00
L4 = * - ErrorScreenMessage
  .byte $21, $02, L5 - L4 - 4, "NES FEATURES THAT THIS", $00
L5 = * - ErrorScreenMessage
  .byte $21, $22, L6 - L5 - 4, "EMULATOR DOES NOT SUPPORT", $00
L6 = * - ErrorScreenMessage
  .byte $21, $62, L7 - L6 - 4, "PLEASE SWITCH TO ONE OF THESE", $00
L7 = * - ErrorScreenMessage
  .byte $21, $85, L8 - L7 - 4, "MESEN 2", $00
L8 = * - ErrorScreenMessage
  .byte $21, $a5, L9 - L8 - 4, "EVERDRIVE N8 PRO", $00
L9 = * - ErrorScreenMessage
  .byte $21, $c5, La - L9 - 4, "FCEUX V2.2.6 OR NEWER", $00
La = * - ErrorScreenMessage
  .byte $22, $22, Lb - La - 4, "MAYBE OTHERS I DUNNO", $00
Lb = * - ErrorScreenMessage
  .byte $22, $42, Lc - Lb - 4, "BUT IF YOU GOT THIS SCREEN", $00
Lc = * - ErrorScreenMessage
  .byte $22, $62, Ld - Lc - 4, "THEN TRY A NEW EMULATOR", $00
Ld = * - ErrorScreenMessage
.byte $00
; Offsets:
; ; duplicate L6 so level 2 with the area transition functions properly
;   .byte $0, L0, L1, L2, L3, L4, L5, L6, L7, L8, L9, La, Lb, Lc, Ld

.endproc

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
  lda #CloudMusic
  sta AreaMusicQueue
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

