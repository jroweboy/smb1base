; This file is for the FamiStudio Sound Engine and was generated by FamiStudio
; Project uses MMC5 expansion, you must set FAMISTUDIO_EXP_MMC5 = 1.

.if FAMISTUDIO_CFG_C_BINDINGS
.export _music_data_hurry_up=music_data_hurry_up
.endif

music_data_hurry_up:
	.byte 1
	.word @instruments
	.word @samples-4
; 00 : hurry up
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

@env0:
	.byte $00,$c0,$7f,$00,$02
@env1:
	.byte $00,$cf,$7f,$00,$02
@env2:
	.byte $c0,$7f,$00,$01
@env3:
	.byte $7f,$00,$00

@samples:
	.byte $5b+.lobyte(FAMISTUDIO_DPCM_PTR),$75,$05,$40 ; 00 y2mate.com - Don (Pitch:5)

@tempo_env_1_mid:
	.byte $03,$05,$80

@song0ch0:
	.byte $ff, $ff, $97
@song0ch0loop:
	.byte $47, .lobyte(@tempo_env_1_mid), .hibyte(@tempo_env_1_mid), $93, $42
	.word @song0ch0loop
@song0ch1:
	.byte $ff, $ff, $97
@song0ch1loop:
	.byte $93, $42
	.word @song0ch1loop
@song0ch2:
	.byte $ff, $ff, $97
@song0ch2loop:
	.byte $93, $42
	.word @song0ch2loop
@song0ch3:
	.byte $ff, $ff, $97
@song0ch3loop:
	.byte $93, $42
	.word @song0ch3loop
@song0ch4:
	.byte $01, $ff, $ff, $95
@song0ch4loop:
	.byte $00, $91, $42
	.word @song0ch4loop
@song0ch5:
	.byte $ff, $ff, $97
@song0ch5loop:
	.byte $93, $42
	.word @song0ch5loop
@song0ch6:
	.byte $ff, $ff, $97
@song0ch6loop:
	.byte $93, $42
	.word @song0ch6loop
