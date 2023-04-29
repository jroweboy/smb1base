.include "common.inc"

.segment "MUSIC"

; Very barebones dpcm sfx support stripped out of famistudio mostly.
; I coulda just wrote this myself, but the famistudio dpcm export with banking was super handy

APU_DMC_FREQ   = $4010
APU_DMC_RAW    = $4011
APU_DMC_START  = $4012
APU_DMC_LEN    = $4013
APU_SND_CHN    = $4015

.export PlaySFX
.proc PlaySFX
  lda #%00001111 ; Stop DPCM
  sta APU_SND_CHN
  lda DPCM_OFFSET_TABLE,x ; Sample offset
  sta APU_DMC_START
  lda DPCM_LENGTH_TABLE,x ; Sample length
  sta APU_DMC_LEN
  lda #$0f ; Pitch and loop - hardcoded to $0f
  sta APU_DMC_FREQ
  lda #$40 ; Initial DMC counter - hardcoded to $40 for now
  sta APU_DMC_RAW
  ldy DPCM_BANK_TABLE,x ; Bank number
  BankPRGC y
  lda #%00011111 ; Start DMC
  sta APU_SND_CHN
  rts
.endproc

FAMISTUDIO_DPCM_PTR = $c000
BANK_NUM_OFFSET = $03

DPCM_OFFSET_TABLE:
	.byte $00+.lobyte(FAMISTUDIO_DPCM_PTR)
	.byte $00+.lobyte(FAMISTUDIO_DPCM_PTR)
	.byte $00+.lobyte(FAMISTUDIO_DPCM_PTR)
	.byte $00+.lobyte(FAMISTUDIO_DPCM_PTR)
	.byte $20+.lobyte(FAMISTUDIO_DPCM_PTR)
	.byte $00+.lobyte(FAMISTUDIO_DPCM_PTR)
	.byte $52+.lobyte(FAMISTUDIO_DPCM_PTR)
	.byte $00+.lobyte(FAMISTUDIO_DPCM_PTR)
	.byte $00+.lobyte(FAMISTUDIO_DPCM_PTR)
	.byte $25+.lobyte(FAMISTUDIO_DPCM_PTR)
	.byte $00+.lobyte(FAMISTUDIO_DPCM_PTR)
	.byte $00+.lobyte(FAMISTUDIO_DPCM_PTR)
	.byte $00+.lobyte(FAMISTUDIO_DPCM_PTR)
	.byte $29+.lobyte(FAMISTUDIO_DPCM_PTR)
	.byte $40+.lobyte(FAMISTUDIO_DPCM_PTR)
	.byte $32+.lobyte(FAMISTUDIO_DPCM_PTR)
	.byte $29+.lobyte(FAMISTUDIO_DPCM_PTR)
	.byte $65+.lobyte(FAMISTUDIO_DPCM_PTR)
	.byte $61+.lobyte(FAMISTUDIO_DPCM_PTR)
	.byte $00+.lobyte(FAMISTUDIO_DPCM_PTR)
	.byte $3c+.lobyte(FAMISTUDIO_DPCM_PTR)
	.byte $3c+.lobyte(FAMISTUDIO_DPCM_PTR)
	.byte $40+.lobyte(FAMISTUDIO_DPCM_PTR)
	.byte $40+.lobyte(FAMISTUDIO_DPCM_PTR)
	.byte $40+.lobyte(FAMISTUDIO_DPCM_PTR)

DPCM_LENGTH_TABLE:
  .byte $7f
  .byte $94
  .byte $ff
  .byte $c7
  .byte $c8
  .byte $ff
  .byte $a5
  .byte $a2
  .byte $a3
  .byte $ff
  .byte $f0
  .byte $ff
  .byte $ee
  .byte $ed
  .byte $a0
  .byte $ba
  .byte $f3
  .byte $57
  .byte $51
  .byte $ff
  .byte $ff
  .byte $f0
  .byte $f6
  .byte $ff
  .byte $ff

DPCM_BANK_TABLE:
  .byte $03 + BANK_NUM_OFFSET ; 00 sm64_mario_boing (Pitch:15)
  .byte $00 + BANK_NUM_OFFSET ; 01 sm64_mario_doh (Pitch:15)
  .byte $06 + BANK_NUM_OFFSET ; 02 sm64_mario_falli (Pitch:15)
  .byte $02 + BANK_NUM_OFFSET ; 03 sm64_mario_haha (Pitch:15)
  .byte $03 + BANK_NUM_OFFSET ; 04 sm64_mario_hello (Pitch:15)
  .byte $05 + BANK_NUM_OFFSET ; 05 sm64_mario_here_ (Pitch:15)
  .byte $03 + BANK_NUM_OFFSET ; 06 sm64_mario_hoo (Pitch:15)
  .byte $07 + BANK_NUM_OFFSET ; 07 sm64_mario_hooho (Pitch:15)
  .byte $0a + BANK_NUM_OFFSET ; 08 sm64_mario_hurt (Pitch:15)
  .byte $00 + BANK_NUM_OFFSET ; 09 sm64_mario_its_m (Pitch:15)
  .byte $04 + BANK_NUM_OFFSET ; 0a sm64_mario_lets_ (Pitch:15)
  .byte $09 + BANK_NUM_OFFSET ; 0b sm64_mario_lost_ (Pitch:15)
  .byte $08 + BANK_NUM_OFFSET ; 0c sm64_mario_mamma (Pitch:15)
  .byte $07 + BANK_NUM_OFFSET ; 0d sm64_mario_okey- (Pitch:15)
  .byte $06 + BANK_NUM_OFFSET ; 0e sm64_mario_oof (Pitch:15)
  .byte $02 + BANK_NUM_OFFSET ; 0f sm64_mario_pulli (Pitch:15)
  .byte $0a + BANK_NUM_OFFSET ; 10 sm64_mario_snore (Pitch:15)
  .byte $00 + BANK_NUM_OFFSET ; 11 sm64_mario_snore 1 (Pitch:15
  .byte $02 + BANK_NUM_OFFSET ; 12 sm64_mario_ungh (Pitch:15)
  .byte $01 + BANK_NUM_OFFSET ; 13 sm64_mario_waha (Pitch:15)
  .byte $08 + BANK_NUM_OFFSET ; 14 sm64_mario_weak (Pitch:15)
  .byte $04 + BANK_NUM_OFFSET ; 15 sm64_mario_whoa (Pitch:15)
  .byte $09 + BANK_NUM_OFFSET ; 16 sm64_mario_yahoo (Pitch:15)
  .byte $05 + BANK_NUM_OFFSET ; 17 sm64_mario_yawn (Pitch:15)
  .byte $01 + BANK_NUM_OFFSET ; 18 sm64_mario_yippe (Pitch:15)

.segment "DPCM_00"
.incbin "../audio/Untitled_bank0.dmc"
.segment "DPCM_01"
.incbin "../audio/Untitled_bank1.dmc"
.segment "DPCM_02"
.incbin "../audio/Untitled_bank2.dmc"
.segment "DPCM_03"
.incbin "../audio/Untitled_bank3.dmc"
.segment "DPCM_04"
.incbin "../audio/Untitled_bank4.dmc"
.segment "DPCM_05"
.incbin "../audio/Untitled_bank5.dmc"
.segment "DPCM_06"
.incbin "../audio/Untitled_bank6.dmc"
.segment "DPCM_07"
.incbin "../audio/Untitled_bank7.dmc"
.segment "DPCM_08"
.incbin "../audio/Untitled_bank8.dmc"
.segment "DPCM_09"
.incbin "../audio/Untitled_bank9.dmc"
.segment "DPCM_0a"
.incbin "../audio/Untitled_bank10.dmc"
