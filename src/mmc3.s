
.include "common.inc"

.segment "FIXED"


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
