

.pushseg

.segment "ZEROPAGE"
safecall_ptr: .res 2

.segment "SHORTRAM"
safecall_a: .res 1
safecall_x: .res 1
safecall_y: .res 1
CurrentBank: .res 1
ReloadCHRBank: .res 1
CurrentCHRBank: .res 12
AreaChrBank := CurrentCHRBank + 8
PlayerChrBank := CurrentCHRBank + 0
EnemyChrBank := CurrentCHRBank + 2

.popseg


IRQ_STATUS = $5204

.macro MAPPER_IRQ_ACK
  bit IRQ_STATUS
.endmacro

MMC5_PRG_ROM = %10000000

MMC5_PRG_BANK_BASE = $5113
MMC5_PRG_BANK_6 = MMC5_PRG_BANK_BASE + 0
MMC5_PRG_BANK_8 = MMC5_PRG_BANK_BASE + 1
MMC5_PRG_BANK_A = MMC5_PRG_BANK_BASE + 2
MMC5_PRG_BANK_C = MMC5_PRG_BANK_BASE + 3
MMC5_PRG_BANK_E = MMC5_PRG_BANK_BASE + 4

MMC5_CHR_BANK_BASE = $5120
MMC5_CHR_SPR_BANK_00 = MMC5_CHR_BANK_BASE + 0
MMC5_CHR_SPR_BANK_04 = MMC5_CHR_BANK_BASE + 1
MMC5_CHR_SPR_BANK_08 = MMC5_CHR_BANK_BASE + 2
MMC5_CHR_SPR_BANK_0C = MMC5_CHR_BANK_BASE + 3
MMC5_CHR_SPR_BANK_10 = MMC5_CHR_BANK_BASE + 4
MMC5_CHR_SPR_BANK_14 = MMC5_CHR_BANK_BASE + 5
MMC5_CHR_SPR_BANK_18 = MMC5_CHR_BANK_BASE + 6
MMC5_CHR_SPR_BANK_1C = MMC5_CHR_BANK_BASE + 7
MMC5_CHR_BG_BANK_00 = MMC5_CHR_BANK_BASE + 8
MMC5_CHR_BG_BANK_04 = MMC5_CHR_BANK_BASE + 9
MMC5_CHR_BG_BANK_08 = MMC5_CHR_BANK_BASE + 10
MMC5_CHR_BG_BANK_0C = MMC5_CHR_BANK_BASE + 11


.macro BankSprCHR00 bank
  _BANK_INNER bank, MMC5_CHR_SPR_BANK_00
.endmacro

.macro BankSprCHR04 bank
  _BANK_INNER bank, MMC5_CHR_SPR_BANK_04
.endmacro

.macro BankSprCHR08 bank
  _BANK_INNER bank, MMC5_CHR_SPR_BANK_08
.endmacro

.macro BankSprCHR0C bank
  _BANK_INNER bank, MMC5_CHR_SPR_BANK_0C
.endmacro

.macro BankSprCHR10 bank
  _BANK_INNER bank, MMC5_CHR_SPR_BANK_10
.endmacro

.macro BankSprCHR14 bank
  _BANK_INNER bank, MMC5_CHR_SPR_BANK_14
.endmacro

.macro BankSprCHR18 bank
  _BANK_INNER bank, MMC5_CHR_SPR_BANK_18
.endmacro

.macro BankSprCHR1C bank
  _BANK_INNER bank, MMC5_CHR_SPR_BANK_1C
.endmacro

.macro BankBgCHR00 bank
  _BANK_INNER bank, MMC5_CHR_BG_BANK_00
.endmacro

.macro BankBgCHR04 bank
  _BANK_INNER bank, MMC5_CHR_BG_BANK_04
.endmacro

.macro BankBgCHR08 bank
  _BANK_INNER bank, MMC5_CHR_BG_BANK_08
.endmacro

.macro BankBgCHR0C bank
  _BANK_INNER bank, MMC5_CHR_BG_BANK_0C
.endmacro

.macro BankPRG6 bank
  _BANK_INNER bank, MMC5_PRG_BANK_6
.endmacro

.macro BankPRG8 bank
  _BANK_INNER bank, MMC5_PRG_BANK_8
.endmacro

.macro BankPRGA bank
  _BANK_INNER bank, MMC5_PRG_BANK_A
.endmacro

.macro BankPRGC bank
  _BANK_INNER bank, MMC5_PRG_BANK_C
.endmacro

.macro BankPRGE bank
  _BANK_INNER bank, MMC5_PRG_BANK_E
.endmacro


.macro _BANK_INNER bank, ADDRESS
.scope
  .if .match(bank, a)
    sta ADDRESS
  .elseif .match(bank, x)
    stx ADDRESS
  .elseif .match(bank, y)
    sty ADDRESS
  .else
    .if (.match (.left (1, {bank}), #))
      lda #.right (.tcount ({bank})-1, {bank}) | MMC5_PRG_ROM ; todo handle ram banking
    .else
      lda bank
    .endif
    sta ADDRESS
  .endif
.endscope
.endmacro

.macro SetScanlineIRQ val
.if (.match (.left (1, {val}), #))
  lda #.right (.tcount ({val})-1, {val})
.else
  lda val
.endif
  sta $5203
  lda #$80
  sta $5204
.endmacro

.macro farcall loc, usejmp
.scope
.assert .bank(loc) <> .bank(*), error, "Attempting to farcall to the same bank!"
.assert .bank(loc) <> .bank(LOWCODE), error, "Attempting to farcall to the low bank!"
.assert .bank(loc) <> .bank(FIXED), error, "Attempting to farcall to the fixed bank!"

.ifblank usejmp
  jsr FarCallCommon
.else
  jsr FarJmpCommon
.endif
.byte .hibyte(loc-1)
.byte .lobyte(loc-1)
.byte .bank(loc) | MMC5_PRG_ROM

.endscope
.endmacro

.macro far loc
.scope
.assert .bank(loc) <> .bank(*), error, "Attempting to farcall to the same bank!"
.assert .bank(loc) <> .bank(LOWCODE), error, "Attempting to farcall to the low bank!"
.assert .bank(loc) <> .bank(FIXED), error, "Attempting to farcall to the fixed bank!"

  sta safecall_a  ; 3  -  3

  lda CurrentBank
  pha
    lda #.bank(loc) | MMC5_PRG_ROM
    sta CurrentBank
    sta MMC5_PRG_BANK_A
    lda safecall_a

.endmacro

.macro endfar
    sta safecall_a
    pla
  sta CurrentBank
  sta MMC5_PRG_BANK_A
  lda safecall_a
.endscope
.endmacro

.macro SwitchAreaCHR
  .repeat 12, I
    lda CurrentCHRBank + I
    sta MMC5_CHR_BANK_BASE + I
  .endrepeat
.endmacro

.segment "FIXED"

MapperInit:
  ; Set PRG mode to 4 8kb banks
  lda #3
  sta $5100
  ; set CHR mode to 4 1kb banks
  sta $5101
  ; allow writing PRG RAM
  lda #2
  sta $5102
  lda #1
  sta $5103
  ; setup vertical mirroring
  lda #$44
  sta $5105

  ; Setup default banks
  lda #$80
  sta CurrentBank
  BankPRGA #0
  BankPRGC #.bank(DPCM)
  BankPRG8 #.bank(LOWCODE)
  BankPRGE #.bank(FIXED)

  ; now setup the CHR banks
  ; To prevent graphics glitches, they must always be written in this order
  ldx #0
  :
    lda CHRBankInitValues,x
    sta CurrentCHRBank,x
    sta MMC5_CHR_BANK_BASE,x
    inx
    cpx #12
    bcc :-
  rts
CHRBankInitValues:
; SPR bank init values
  .byte CHR_SMALLMARIO, CHR_MISC, CHR_SPRITES, CHR_SPRITES+1
  .byte CHR_SPRITES+2, CHR_SPRITES+3, CHR_SPRITES+4, CHR_SPRITES+5

; BG bank init values
  .byte CHR_BG_GROUND, CHR_BG_GROUND+1, CHR_BG_GROUND+2, CHR_BG_GROUND+3

FarCallCommon:
  sta safecall_a  ; 3  -  3
  stx safecall_x  ; 3  -  6
  sty safecall_y  ; 3  -  9

  ; copy stack ptr to x
  tsx             ; 2  -  11
  ; Read the return address and write it to our ptr
  lda $100 + 1, x ; 4  -  15
  sta safecall_ptr ; 3 -  18
  ; and also add 3 to it so that we can skip over the 3 bytes
  ; that we use to store the data
  clc              ; 2 -  20
  adc #3           ; 2 -  22
  sta $100 + 1, x  ; 4 -  27
  ; Now read the high byte of the return address and 
  lda $100 + 2, x  ; 4 -  31
  sta safecall_ptr+1 ; 3 - 34
  adc #0           ; 2 - 36
  sta $100 + 2, x  ; 4 - 40

  lda CurrentBank
  pha
    jsr @DoFarCall

    ; Now restore the previous bank
    sta safecall_a

  ; Pull what page our bank used to be in and switch back
  pla
  sta CurrentBank
  sta MMC5_PRG_BANK_A

  ; Load A
  lda safecall_a
  rts

@DoFarCall:
  ; read the high byte of the destination and write it to the stack
  ldy #1
  lda (safecall_ptr),y
  pha
  iny
  ; and the low byte
  lda (safecall_ptr),y
  pha
  iny
  ; and the bank byte
  lda (safecall_ptr),y
  sta CurrentBank
  sta MMC5_PRG_BANK_A
  lda safecall_a
  ldx safecall_x
  ldy safecall_y
  ; return to jmp to the target address
  rts

FarJmpCommon:
  sta safecall_a  ; 3  -  3
  stx safecall_x  ; 3  -  6
  sty safecall_y  ; 3  -  9

  pla
  sta safecall_ptr ; 3 -  18
  ; Now read the high byte of the return address and 
  pla
  sta safecall_ptr+1 ; 3 - 34

  lda CurrentBank
  pha
    jsr @DoFarCall

    ; Now restore the previous bank
    sta safecall_a

  ; Pull what page our bank used to be in and switch back
  pla
  sta CurrentBank
  sta MMC5_PRG_BANK_A

  ; Load A
  lda safecall_a
  rts

@DoFarCall:
  ; read the high byte of the destination and write it to the stack
  ldy #1
  lda (safecall_ptr),y
  pha
  iny
  ; and the low byte
  lda (safecall_ptr),y
  pha
  iny
  ; and the bank byte
  lda (safecall_ptr),y
  sta CurrentBank
  sta MMC5_PRG_BANK_A

  lda safecall_a
  ldx safecall_x
  ldy safecall_y
  ; return to jmp to the target address
  rts
