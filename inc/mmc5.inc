
.ifndef __MMC5_INC
__MMC5_INC = 1

.global FarCallCommon, MapperInit, FarJmpCommon

.include "memory_p.inc"

.pushseg
.zeropage
RESERVEZP safecall_a, 1
RESERVEZP safecall_x, 1
RESERVEZP safecall_y, 1
RESERVEZP safecall_ptr, 2
.segment "SHORTRAM"
RESERVE CurrentBank, 1
RESERVE ReloadCHRBank, 1
RESERVE CurrentCHRBank, 12
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
.import loc
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
.import loc
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

.endif ; __MMC5_INC
