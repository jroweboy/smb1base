
.include "common.inc"

.segment "FIXED"


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
    sta CurrentBank
    sta BANK_DATA
    jsr TargetAddrJmp
    lda #7 | PRG_FIXED_8
    sta BankShadow
    sta BANK_SELECT
  pla
  sta CurrentBank
  sta BANK_DATA
  rts
