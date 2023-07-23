
.include "common.inc"

.segment "FIXED"

; x = bank to switch to
.proc FarCallCommon
  sta NextBank
  lda CurrentBank
  pha
    lda NextBank
    BankPRGA a
    jsr TargetAddrJmp
  pla
  BankPRGA a
  sta CurrentBank
  rts
.endproc
