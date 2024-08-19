
.segment "FIXED"


.pushseg
.segment "SHORTRAM"
TargetAddrJmp: .res 4 ; 3 ; increased to 4 to jsr/rts for debugging
TargetAddress := TargetAddrJmp + 1
CurrentBank: .res 1
BankShadow: .res 1
ReloadCHRBank: .res 1
CurrentCHRBank: .res 6
AreaChrBank := CurrentCHRBank
PlayerChrBank := CurrentCHRBank + 2
EnemyChrBank := CurrentCHRBank + 4
.popseg

; MMC3 registers / banking
PRG_FIXED_8 = (1 << 6)
BANK_SELECT = $8000
BANK_DATA   = $8001
NMT_MIRROR  = $a000
RAM_PROTECT = $a001
IRQLATCH    = $c000
IRQRELOAD   = $c001
IRQDISABLE  = $e000
IRQENABLE   = $e001

.macro SwitchAreaCHR
    ldx #PRG_FIXED_8
  .repeat 6, I
    stx BANK_SELECT
    lda CurrentCHRBank + I
    sta BANK_DATA
  .if I <> 5
    inx
  .endif
  .endrepeat
.endmacro

.macro MAPPER_IRQ_ACK
  sta IRQDISABLE
.endmacro

.macro BankCHR0 bank
  _BANK_INNER bank, 0
.endmacro

.macro BankCHR8 bank
  _BANK_INNER bank, 1
.endmacro

.macro BankCHR10 bank
  _BANK_INNER bank, 2
.endmacro

.macro BankCHR14 bank
  _BANK_INNER bank, 3
.endmacro

.macro BankCHR18 bank
  _BANK_INNER bank, 4
.endmacro

.macro BankCHR1C bank
  _BANK_INNER bank, 5
.endmacro

.macro BankPRGA bank
  _BANK_INNER bank, 7
.endmacro

.macro BankPRGC bank
  _BANK_INNER bank, 6
.endmacro

.macro _BANK_INNER bank, select
.scope
  ; set fixed $8000 bank bit
.if .match(bank, a)
  pha
  lda #select | PRG_FIXED_8
  sta BANK_SELECT
.else
  lda #select | PRG_FIXED_8
  sta BANK_SELECT
.endif
  .if .match(bank, a)
    pla
    sta BANK_DATA
  .elseif .match(bank, x)
    stx BANK_DATA
  .elseif .match(bank, y)
    sty BANK_DATA
  .else
    .if (.match (.left (1, {arg}), #))
      lda #bank
    .else
      lda bank
    .endif
    sta BANK_DATA
  .endif
.endscope
.endmacro

.macro farcall loc, usejmp
.scope
.assert .bank(loc) <> .bank(*), error, "Attempting to farcall to the same bank!"
.assert .bank(loc) <> .bank(LOWCODE), error, "Attempting to farcall to the low bank!"
.assert .bank(loc) <> .bank(FIXED), error, "Attempting to farcall to the fixed bank!"
  lda #<loc
  sta TargetAddress
  lda #>loc
  sta TargetAddress+1
  lda #.lobyte(.bank(loc))
  sta NextBank
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
    lda #7 | PRG_FIXED_8
    sta BankShadow
    sta BANK_SELECT
    lda #.bank(function)
    sta BANK_DATA
    sta CurrentBank
.endmacro

.macro endfar
    lda #7 | PRG_FIXED_8
    sta BankShadow
    sta BANK_SELECT
  pla
  sta BANK_DATA
  sta CurrentBank
.endscope
.endmacro


.macro SetScanlineIRQ line
  .if .match(line, x)
    stx IRQLATCH
    stx IRQRELOAD
    stx IRQENABLE
  .elseif .match(line, y)
    sty IRQLATCH
    sty IRQRELOAD
    sty IRQENABLE
  .else
    .if (.match (.left (1, {arg}), #))
      lda #line
    .else
      lda line
    .endif
    sta IRQLATCH
    sta IRQRELOAD
    sta IRQENABLE
  .endif
.endmacro


BankInitValues:
  .byte CHR_BG_GROUND, CHR_BG_GROUND+2, CHR_SMALLMARIO, CHR_MISC, CHR_SPR_GROUND, CHR_SPR_GROUND+1

MapperInit:
  ; setup the jmp instruction for the FarBank Target
  jsr FarCallInit
  ldx #5
  CHRBankInitLoop:
    txa
    ora PRG_FIXED_8
    sta BANK_SELECT
    lda BankInitValues,x
    sta CurrentCHRBank,x
    sta BANK_DATA
    dex
    bpl CHRBankInitLoop

  ; Now set the initial A bank
  BankPRGA #0
  lda #0
  sta CurrentBank
  sta NmiSkipped
  lda #7 | PRG_FIXED_8
  sta BankShadow

  ; disable scanline counter, and IRQ
  lda #0
  sta NMT_MIRROR
  sta IRQDISABLE
  ; enable on board WRAM
  lda #%10000000
  sta RAM_PROTECT

; Profiler friendly version of the farcall that does jsr rts instead
; Switch this back before release.
FarCallInit:
  lda #$20 ; #$4c
  sta TargetAddrJmp
  lda #$60
  sta TargetAddress+2
  rts

; FarCallInit:
;   lda #$4c
;   sta TargetAddrJmp
;   rts

; x = bank to switch to
FarCallCommon:
  lda CurrentBank
  pha
    lda #7 | PRG_FIXED_8
    sta BankShadow
    sta BANK_SELECT
    lda NextBank
    sta BANK_DATA
    sta CurrentBank
    jsr TargetAddrJmp
    lda #7 | PRG_FIXED_8
    sta BankShadow
    sta BANK_SELECT
  pla
  sta BANK_DATA
  sta CurrentBank
  rts
