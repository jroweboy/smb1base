.ifndef __VRC7_INC
__VRC7_INC = 1


.global CurrentBank, NextBank, BankShadow
.global TargetAddrJmp, TargetAddress
.global FarCallCommon

; MMC3 registers / banking
; PRG_FIXED_8 = (1 << 6)
; BANK_SELECT_8 = $8000
; BANK_SELECT_A = $8010
; BANK_SELECT_C = $9000
; BANK_DATA   = $8001
NMT_MIRROR  = $e000
; RAM_PROTECT = $a001
IRQLATCH    = $e010
IRQCONTROL  = $f000
IRQACK      = $f010
; IRQRELOAD   = $c001
; IRQDISABLE  = $e000
; IRQENABLE   = $e001

.macro BankCHR0 bank
  _BANK_INNER bank, $a000
.endmacro

.macro BankCHR4 bank
  _BANK_INNER bank, $a010
.endmacro

.macro BankCHR8 bank
  _BANK_INNER bank, $b000
.endmacro

.macro BankCHRC bank
  _BANK_INNER bank, $b010
.endmacro

.macro BankCHR10 bank
  _BANK_INNER bank, $c000
.endmacro

.macro BankCHR14 bank
  _BANK_INNER bank, $c010
.endmacro

.macro BankCHR18 bank
  _BANK_INNER bank, $d000
.endmacro

.macro BankCHR1C bank
  _BANK_INNER bank, $d010
.endmacro

.macro BankPRG8 bank
  _BANK_INNER bank, $8000
.endmacro

.macro BankPRGA bank
  _BANK_INNER bank, $8010
.endmacro

.macro BankPRGC bank
  _BANK_INNER bank, $9000
.endmacro

.macro _BANK_INNER bank, select
.scope
  .if .match(bank, a)
    sta select
  .elseif .match(bank, x)
    stx select
  .elseif .match(bank, y)
    sty select
  .else
    .if (.match (.left (1, {arg}), #))
      lda #bank
    .else
      lda bank
    .endif
    sta select
  .endif
.endscope
.endmacro

.macro farcall loc, usejmp
.scope
.import loc
.assert .bank(loc) <> .bank(*), error, "Attempting to farcall to the same bank!"
.assert .bank(loc) <> .bank(LOWCODE), error, "Attempting to farcall to the low bank!"
.assert .bank(loc) <> .bank(FIXED), error, "Attempting to farcall to the fixed bank!"
  lda #<loc
  sta TargetAddress
  lda #>loc
  sta TargetAddress+1
  lda #.lobyte(.bank(loc))
.ifblank usejmp
  jsr FarCallCommon
.else
  jmp FarCallCommon
.endif
.endscope
.endmacro

.macro far function
.scope
.ident(.concat("farblock_", .string(function))):
  lda CurrentBank
  pha
    BankPRGA #.bank(function)
.endmacro

.macro endfar
  pla
  BankPRGA a
.endscope
.endmacro
.endif

