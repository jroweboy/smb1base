
.include "common.inc"
.segment "FIXED"

.export TitleScreenIrq
.proc TitleScreenIrq
  pha
  phx
  phy
    sta IRQDISABLE
    ldx IrqNextScanline
    lda #95
    sta IRQLATCH
    sta IRQRELOAD
    sta IRQENABLE

    ldy NextBankValuesHi,x
    lda NextBankValuesLo,x
    tax
    ; delay 32 cycles
    NOP
    TYA
    LDY #5
    DEY
    BNE *-1
    TAY
    inc IrqNextScanline
    BankCHR0 x
    BankCHR8 y
  ply
  plx
  pla
  rti

NextBankValuesLo:
  .byte $4c, $50
NextBankValuesHi:
  .byte $4e, $50
; IrqReloadValues:
;   .byte $40, $ff
.endproc

.segment "TITLE"
.export DrawTitleScreenInternal
.proc DrawTitleScreenInternal
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

  ; Setup the correct bank
  BankCHR0 #$48
  BankCHR8 #$4a
  ; c/e
  ; 50/52
  ; Title screen sprites starts at 54 i think
  BankCHR10 #$52
  BankCHR14 #$53
  BankCHR18 #$54
  BankCHR1C #$55
.define BHOP_MAGIC_STRING "7BHP"
  ; check to see if our sound driver is already in SRAM
  lda BhopValidation
  cmp #.strat(BHOP_MAGIC_STRING,0)
  bne FailedValidation
  lda BhopValidation+1
  cmp #.strat(BHOP_MAGIC_STRING,1)
  bne FailedValidation
  lda BhopValidation+2
  cmp #.strat(BHOP_MAGIC_STRING,2)
  bne FailedValidation
  lda BhopValidation+3
  cmp #.strat(BHOP_MAGIC_STRING,3)
  bne FailedValidation
    jmp Finish

.import __MUSIC_DRIVER_LOAD__, __MUSIC_DRIVER_RUN__, __MUSIC_DRIVER_SIZE__
FailedValidation:
  ; enable writing to sram
  ; lda #%10000000
  ; sta RAM_PROTECT

LOAD = IrqR0
RUN = IrqR2
SIZE = IrqR4

  lda #<__MUSIC_DRIVER_LOAD__
  sta LOAD
  lda #>__MUSIC_DRIVER_LOAD__
  sta LOAD+1
  lda #<__MUSIC_DRIVER_RUN__
  sta RUN
  lda #>__MUSIC_DRIVER_RUN__
  sta RUN+1
  lda #<__MUSIC_DRIVER_SIZE__
  sta SIZE
  ldx #>__MUSIC_DRIVER_SIZE__
  beq LessThan256
  stx SIZE+1
  ldy #0
  :
      lda (LOAD),y
      sta (RUN),y
      iny
      bne :-
    inc LOAD+1
    inc RUN+1
    dex
    bne :-
LessThan256:
  ldx SIZE
  beq DoneCopying
  :
    lda (LOAD),y
    sta (RUN),y
    iny
    dex
    bne :-
DoneCopying:
  lda #.strat(BHOP_MAGIC_STRING,0)
  sta BhopValidation
  lda #.strat(BHOP_MAGIC_STRING,1)
  sta BhopValidation+1
  lda #.strat(BHOP_MAGIC_STRING,2)
  sta BhopValidation+2
  lda #.strat(BHOP_MAGIC_STRING,3)
  sta BhopValidation+3
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
  inc OperMode_Task
  
  rts

NametableData:
  .incbin "../chr/title/4kb.nam"
PaletteData:
  .incbin "../chr/title/4kb_palette.dat"
SpriteData:
  .incbin "../chr/title/4kb.oam"
.endproc


.segment "MUSIC_DRIVER"
.include "bhop/bhop.inc"

.proc banked_init
  BankPRGA #.lobyte(.bank(TITLE_MUSIC))
  BankPRGC #.lobyte(.bank(TITLE_DPCM))

  lda #0
  jsr bhop_init
  
  lda #1
  sta BhopInitalized

  BankPRGA #.lobyte(.bank(TITLE))

  rts
.endproc

.segment "TITLE_MUSIC"

bhop_music_data:
  .include "../audio/angry_title.asm"
.export bhop_music_data

