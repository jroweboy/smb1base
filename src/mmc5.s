

_MEMORY_DEFINE_MEMORY .set 1
.include "mmc5.inc"
_MEMORY_DEFINE_MEMORY .set 0

.include "common.inc"

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
  BankPRG8 #.bank(LOWCODE)
  BankPRGE #.bank(FIXED)

  ; now setup the CHR banks
  ldx #0
  :
    lda CHRBankInitValues,x
    sta CurrentCHRBank,x
    sta MMC5_CHR_BANK_BASE,x
    inx
    cmp #12
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
