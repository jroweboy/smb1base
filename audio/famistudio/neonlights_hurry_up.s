; This file is for the FamiStudio Sound Engine and was generated by FamiStudio
; Project uses MMC5 expansion, you must set FAMISTUDIO_EXP_MMC5 = 1.
; Project has DPCM bank-switching enabled in the project settings, you must set FAMISTUDIO_USE_DPCM_BANKSWITCHING = 1 and implement bank switching.

.if FAMISTUDIO_CFG_C_BINDINGS
.export _music_data_hurry_up=music_data_hurry_up
.endif

music_data_hurry_up:
	.byte 1
	.word @instruments
	.word @samples-5
; 00 : Hurry up
	.word @song0ch0
	.word @song0ch1
	.word @song0ch2
	.word @song0ch3
	.word @song0ch4
	.word @song0ch5
	.word @song0ch6
	.byte .lobyte(@tempo_env_1_mid), .hibyte(@tempo_env_1_mid), 0, 0

.export music_data_hurry_up
.global FAMISTUDIO_DPCM_PTR

@instruments:
	.word @env5,@env3,@env6,@env0 ; 00 : Long instrumental
	.word @env1,@env3,@env4,@env0 ; 01 : Triangle
	.word @env2,@env3,@env4,@env0 ; 02 : 12.5% Pulse

@env0:
	.byte $00,$c0,$7f,$00,$02
@env1:
	.byte $00,$cf,$07,$c0,$00,$03
@env2:
	.byte $00,$cf,$7f,$00,$02
@env3:
	.byte $c0,$7f,$00,$01
@env4:
	.byte $7f,$00,$00
@env5:
	.byte $00,$c5,$c3,$c4,$c5,$c5,$c6,$10,$c5,$05,$c4,$05,$c3,$02,$c2,$c2,$c1,$c0,$00,$11
@env6:
	.byte $c0,$c2,$00,$01

@samples:

@tempo_env_1_mid:
	.byte $03,$05,$80

@song0ch0:
	.byte $80, $11, $83, $00, $83, $21, $89, $00, $89, $21, $89, $21, $89, $00, $89, $12, $89, $22, $89, $00, $89, $22, $89, $22
	.byte $89, $00, $89, $13, $89, $23, $89, $00, $89, $23, $89, $23, $89, $00, $89, $24, $89, $00, $89, $24, $dd
@song0ch0loop:
	.byte $47, .lobyte(@tempo_env_1_mid), .hibyte(@tempo_env_1_mid), $00, $89, $42
	.word @song0ch0loop
@song0ch1:
	.byte $80, $1d, $89, $27, $95, $27, $89, $27, $95, $1e, $89, $28, $95, $28, $89, $28, $95, $1f, $89, $29, $95, $29, $89, $29
	.byte $95, $2a, $95, $2a, $dd
@song0ch1loop:
	.byte $00, $89, $42
	.word @song0ch1loop
@song0ch2:
	.byte $82, $24, $89, $30, $95, $30, $89, $30, $95, $25, $89, $31, $95, $31, $89, $31, $95, $26, $89, $32, $95, $32, $89, $32
	.byte $95, $20, $95, $84, $20, $dd
@song0ch2loop:
	.byte $00, $89, $42
	.word @song0ch2loop
@song0ch3:
	.byte $ff, $ff, $cf
@song0ch3loop:
	.byte $8b, $42
	.word @song0ch3loop
@song0ch4:
	.byte $ff, $ff, $cf
@song0ch4loop:
	.byte $8b, $42
	.word @song0ch4loop
@song0ch5:
	.byte $ff, $ff, $cf
@song0ch5loop:
	.byte $8b, $42
	.word @song0ch5loop
@song0ch6:
	.byte $ff, $ff, $cf
@song0ch6loop:
	.byte $8b, $42
	.word @song0ch6loop
