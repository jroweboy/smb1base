; This file is for the FamiStudio Sound Engine and was generated by FamiStudio
; Project uses MMC5 expansion, you must set FAMISTUDIO_EXP_MMC5 = 1.

.if FAMISTUDIO_CFG_C_BINDINGS
.export _music_data_you_saved_the_princess=music_data_you_saved_the_princess
.endif

music_data_you_saved_the_princess:
	.byte 1
	.word @instruments
	.word @samples-4
; 00 : you saved the princess
	.word @song0ch0
	.word @song0ch1
	.word @song0ch2
	.word @song0ch3
	.word @song0ch4
	.word @song0ch5
	.word @song0ch6
	.byte .lobyte(@tempo_env_1_mid), .hibyte(@tempo_env_1_mid), 0, 0

.export music_data_you_saved_the_princess
.global FAMISTUDIO_DPCM_PTR

@instruments:
	.word @env5,@env6,@env1,@env0 ; 00 : Voice Oohs
	.word @env5,@env6,@env2,@env0 ; 01 : Electric Bass (fingered)
	.word @env5,@env6,@env8,@env0 ; 02 : Acoustic Guitar (steel) MMC5
	.word @env5,@env6,@env3,@env0 ; 03 : Orchestral Harp
	.word @env4,@env6,@env7,@env0 ; 04 : Acoustic Grand Piano

@env0:
	.byte $00,$c0,$7f,$00,$02
@env1:
	.byte $c1,$c0,$00,$01
@env2:
	.byte $c2,$c1,$00,$01
@env3:
	.byte $c2,$c0,$00,$01
@env4:
	.byte $00,$c7,$c5,$c3,$c0,$00,$04
@env5:
	.byte $00,$cf,$7f,$00,$02
@env6:
	.byte $c0,$7f,$00,$01
@env7:
	.byte $7f,$00,$00
@env8:
	.byte $c3,$c0,$00,$01

@samples:
	.byte $40+.lobyte(FAMISTUDIO_DPCM_PTR),$10,$0e,$40 ; 00 hisnare (Pitch:14)
	.byte $45+.lobyte(FAMISTUDIO_DPCM_PTR),$31,$0d,$40 ; 01 snare (Pitch:13)
	.byte $52+.lobyte(FAMISTUDIO_DPCM_PTR),$21,$0c,$40 ; 02 snare2 (Pitch:12)

@tempo_env_1_mid:
	.byte $03,$05,$80

@song0ch0:
@song0ch0loop:
	.byte $47, .lobyte(@tempo_env_1_mid), .hibyte(@tempo_env_1_mid), $f7, $80
@song0ref7:
	.byte $38, $9b, $37, $9b, $35, $9b, $35, $9b, $37, $9b, $00, $ff, $cf, $48, $d9, $2e, $9b, $38, $9b, $37, $9b, $35, $9b, $35
	.byte $b9, $37, $9b, $00, $b9, $33, $b9, $35, $9b, $2e, $9b, $48, $9d, $00, $ff, $b1, $2e, $9b, $35, $b9, $37, $9b, $38, $d7
	.byte $35, $9b, $32, $9b, $48, $9d, $33, $d7, $35, $b9, $2e, $9b, $2e, $b9, $37, $b9, $00, $ff, $93, $48, $f7
	.byte $41, $18
	.word @song0ref7
@song0ref79:
	.byte $00, $9b, $37, $9b, $33, $b9, $00, $9b, $35, $9b, $2e, $9b, $48, $9d, $00, $ff, $cf, $35, $b9, $37, $9b, $38, $d7, $35
	.byte $9b, $32, $9b, $48, $bb
@song0ref108:
	.byte $33, $9b, $35, $9b, $00, $9b, $2e, $9b, $33, $9b, $35, $9b, $36, $9b, $35, $9b, $33, $9b, $31, $9b, $00, $b9, $2e, $9b
	.byte $2f, $9b, $48, $31, $b9, $36, $b9, $35, $9b, $33, $9b, $33, $9b, $31, $9b, $33, $9b, $31, $9b, $31, $b9, $31, $9b, $00
	.byte $9b
@song0ref157:
	.byte $2e, $9b, $2f, $9b, $48, $31, $b9, $36, $b9, $38, $9b, $36, $9b, $35, $9b, $33, $9b, $33, $9b, $35, $9b, $36, $b9, $36
	.byte $9b, $00, $9b
@song0ref184:
	.byte $38, $9b, $3a, $9b, $48, $3b, $9b, $3b, $9b, $3a, $b9, $38, $b9, $36, $9b, $38, $9b, $3a, $9b, $3a, $9b, $38, $b9
@song0ref207:
	.byte $36, $b9, $33, $9b, $31, $9b, $48, $33, $9b, $36, $9b, $36, $9b, $35, $b9, $35, $9b, $37, $9b, $37, $f5, $00, $ff, $93
	.byte $48, $ff, $ff, $ff, $df
@song0ref236:
	.byte $48, $d9, $2e, $9b, $38, $9b, $37, $9b, $35, $9b, $35, $9b, $37, $9b, $00, $ff, $cf, $48, $f7, $38, $9b, $37, $9b, $35
	.byte $9b, $35, $d7, $37, $9b, $33, $b9, $35, $b9, $2e, $9b, $48, $9d, $00, $ff, $cf, $35, $b9, $37, $9b, $38, $d7, $35
@song0ref283:
	.byte $b9, $48, $32, $b9, $33, $9b, $35, $d7, $2e, $9b, $2e, $b9, $37, $b9, $00, $ff, $93
	.byte $41, $10
	.word @song0ref236
	.byte $48, $d9, $2e, $9b, $38, $9b, $37, $9b, $35, $9b, $35, $b9
	.byte $41, $17
	.word @song0ref79
	.byte $b9, $48, $32, $b9
	.byte $41, $2d
	.word @song0ref108
	.byte $b9
	.byte $41, $17
	.word @song0ref157
	.byte $b9
	.byte $41, $15
	.word @song0ref184
	.byte $9b, $36, $9b
	.byte $41, $1b
	.word @song0ref207
	.byte $48, $f7, $38, $8d, $38, $8b, $37, $9b, $35, $9b, $35, $ab, $37, $a9, $00, $ff, $b1, $48, $d9, $2e, $9b, $38, $9b, $37
	.byte $9b, $35, $9b, $35, $b9
	.byte $41, $17
	.word @song0ref79
	.byte $41, $10
	.word @song0ref283
	.byte $48, $f7, $38, $9b, $37, $9b, $35, $9b, $35, $a3, $37, $9b, $00, $ff, $c7, $48, $f7, $38, $9b, $37, $9b, $35, $9b, $35
	.byte $b9
	.byte $41, $17
	.word @song0ref79
	.byte $b9, $48, $32, $b9
	.byte $41, $2d
	.word @song0ref108
	.byte $b9
	.byte $41, $17
	.word @song0ref157
	.byte $b9, $38, $9b, $3a, $9b, $48, $3b, $9b, $3b, $9b, $3a, $9b, $38, $9b, $38, $b9, $36, $9b, $38, $9b, $3a, $9b, $3a, $9b
	.byte $38, $9b, $36, $9b
	.byte $41, $15
	.word @song0ref207
	.byte $d7
@song0ref445:
	.byte $3a, $9b, $3a, $9b, $48, $3c, $9b, $3a, $9b, $37, $9b, $33, $b9, $35, $9b, $3a, $9b, $3a, $b9, $00, $f5, $3a, $9b
	.byte $41, $0e
	.word @song0ref445
	.byte $37, $9b, $37, $b9, $00, $ff, $cf, $48, $ff, $ff, $ff, $df, $48, $ff, $ff, $ff, $df, $48, $ff, $ff, $ff, $df, $48, $ff
	.byte $ff, $ff, $df, $42
	.word @song0ch0loop
@song0ch1:
@song0ch1loop:
@song0ref502:
	.byte $ff, $ff, $ff, $df, $ff, $ff, $ff, $df, $ff, $ff, $ff, $df, $ff, $ff, $ff, $df
	.byte $41, $10
	.word @song0ref502
	.byte $ff, $ff, $ff, $df, $ff, $ff, $ff, $df, $ff, $ff, $ff, $df, $ff, $b3, $82, $1b
@song0ref537:
	.byte $b9
@song0ref538:
	.byte $0f, $b9, $00, $9b
@song0ref542:
	.byte $0f, $9b, $0c, $b9, $00, $9b, $0c, $9b, $0f, $b9, $00, $9b, $0f, $9b, $0c, $b9, $00, $9b, $0c, $9b
	.byte $41, $12
	.word @song0ref538
	.byte $18, $b9, $18, $9b, $16, $9b
	.byte $41, $18
	.word @song0ref538
	.byte $41, $18
	.word @song0ref538
@song0ref577:
	.byte $11, $b9, $00, $9b, $11, $9b, $11, $9b, $13, $9b, $14, $b9, $0a, $d7, $0a, $9b, $0a, $9b, $0c, $9b, $0e
	.byte $41, $13
	.word @song0ref537
	.byte $18, $b9, $18, $9b, $16, $9b
	.byte $41, $10
	.word @song0ref538
	.byte $80, $33, $9b
@song0ref613:
	.byte $3c, $9b, $3a, $9b, $38, $9b, $38, $b9, $00, $9b, $3a, $9b, $37, $b9, $00, $9b, $38, $9b, $33, $9b, $9d, $00, $b9, $82
	.byte $0f, $9b, $0c, $b9, $00, $9b, $0c, $9b
	.byte $41, $15
	.word @song0ref577
@song0ref648:
	.byte $9b, $0a, $9b, $0b, $ff, $cf, $0a
@song0ref655:
	.byte $9b
@song0ref656:
	.byte $12, $b9, $00, $9b, $12, $9b, $0d, $b9, $00, $9b, $0d, $9b, $0b, $b9, $00, $9b, $0b, $9b, $12, $b9, $00, $b9
	.byte $41, $14
	.word @song0ref656
@song0ref681:
	.byte $12, $9b, $0d, $9b, $0b, $b9, $0b, $b9, $0d, $b9, $0d, $b9, $12, $b9, $11, $b9, $0f, $b9, $0d, $b9, $0b, $b9, $12, $b9
	.byte $0a, $b9, $11
	.byte $41, $19
	.word @song0ref537
	.byte $41, $12
	.word @song0ref538
	.byte $18, $b9, $18, $9b, $16, $9b, $00, $ff, $ed, $ff, $ff, $ff, $df, $ff, $ff, $ff, $df
@song0ref731:
	.byte $22, $9b, $26, $9b, $2c, $9b, $26, $9b, $22, $9b, $26, $9b, $2c, $9b, $26
@song0ref746:
	.byte $9b
@song0ref747:
	.byte $22, $9b
@song0ref749:
	.byte $27, $9b, $2b, $9b, $27, $9b
@song0ref755:
	.byte $24, $9b, $27, $9b, $2b, $9b, $27, $9b, $22, $9b, $27, $9b, $2b, $9b, $27, $9b, $80, $3c, $9b, $3a, $9b, $38, $9b, $38
	.byte $a3, $3a, $93, $82
	.byte $41, $14
	.word @song0ref749
	.byte $0f, $9b, $80
	.byte $41, $14
	.word @song0ref613
	.byte $82, $0f, $b9, $00, $9b, $0f, $9b, $0c, $b9, $00, $9b, $0c, $9b
	.byte $41, $15
	.word @song0ref577
	.byte $41, $1d
	.word @song0ref648
	.byte $9b, $12
	.byte $41, $15
	.word @song0ref655
	.byte $41, $1b
	.word @song0ref681
	.byte $41, $14
	.word @song0ref537
	.byte $9b, $80, $38, $9b, $37, $9b, $37, $b9, $00, $b9, $82
	.byte $41, $0f
	.word @song0ref542
	.byte $9b, $80, $38, $9b, $3a, $9b, $3a, $b9, $00, $b9, $82, $0f, $9b, $0c, $9b, $80
@song0ref852:
	.byte $3a, $9b
@song0ref854:
	.byte $3a, $9b, $3a, $9b, $3c, $9b, $3a, $9b, $37, $9b, $33, $b9, $35, $9b, $37, $9b, $37, $b9, $00, $ff, $93
	.byte $41, $13
	.word @song0ref854
	.byte $f5
	.byte $41, $15
	.word @song0ref852
	.byte $f5, $38, $9b, $3a, $9b, $3a, $9b, $9d, $00, $f5, $38, $9b, $37, $9b, $37, $b9, $00, $ff, $cf, $42
	.word @song0ch1loop
@song0ch2:
@song0ch2loop:
	.byte $ff, $ef, $86
	.byte $41, $18
	.word @song0ref747
	.byte $41, $10
	.word @song0ref755
	.byte $41, $10
	.word @song0ref755
@song0ref917:
	.byte $24, $9b, $27, $9b, $2b, $9b, $27, $9b, $24, $9b, $29, $9b, $2c, $9b, $29, $9b, $24, $9b, $29, $9b, $2c, $9b, $29, $9b
	.byte $41, $28
	.word @song0ref731
	.byte $41, $10
	.word @song0ref755
	.byte $41, $10
	.word @song0ref755
	.byte $41, $10
	.word @song0ref755
	.byte $41, $10
	.word @song0ref755
	.byte $41, $18
	.word @song0ref917
	.byte $41, $10
	.word @song0ref731
	.byte $23, $9b, $27, $9b, $2a, $9b, $2e, $f5
@song0ref970:
	.byte $00
@song0ref971:
	.byte $9b, $2a, $9b, $2e, $9b, $2a, $9b, $00, $9b, $29, $9b, $2c, $9b, $29, $9b, $00, $9b, $27, $9b, $2f, $9b, $27, $9b, $29
	.byte $9b, $2a, $9b, $2e, $9b, $2a, $9b
	.byte $41, $20
	.word @song0ref970
@song0ref1005:
	.byte $00, $9b, $27, $9b, $2c, $9b, $27, $9b, $2c, $9b, $29, $9b, $2c, $9b, $29, $9b, $2c, $9b, $2a, $9b, $2e, $9b, $2a, $9b
	.byte $2e, $9b, $2a, $9b, $2e, $9b, $2a, $9b, $2e, $9b, $27, $9b, $2c, $9b, $27, $9b, $2c, $9b, $26, $9b, $2c, $9b, $26, $9b
	.byte $2c
	.byte $41, $19
	.word @song0ref746
	.byte $41, $10
	.word @song0ref755
	.byte $41, $10
	.word @song0ref755
	.byte $41, $10
	.word @song0ref755
	.byte $41, $10
	.word @song0ref755
	.byte $41, $10
	.word @song0ref755
	.byte $41, $10
	.word @song0ref755
	.byte $41, $18
	.word @song0ref917
	.byte $41, $28
	.word @song0ref731
	.byte $41, $10
	.word @song0ref755
	.byte $41, $10
	.word @song0ref755
	.byte $41, $10
	.word @song0ref755
	.byte $41, $10
	.word @song0ref755
	.byte $41, $18
	.word @song0ref917
	.byte $41, $10
	.word @song0ref731
@song0ref1099:
	.byte $23, $9b, $27, $9b, $2e, $9b, $2a, $9b, $2e, $9b, $2a, $9b, $2e, $9b, $2a
	.byte $41, $1f
	.word @song0ref971
	.byte $41, $20
	.word @song0ref970
	.byte $41, $31
	.word @song0ref1005
	.byte $41, $19
	.word @song0ref746
	.byte $41, $10
	.word @song0ref755
	.byte $41, $10
	.word @song0ref755
	.byte $41, $10
	.word @song0ref755
	.byte $41, $10
	.word @song0ref755
	.byte $41, $10
	.word @song0ref755
	.byte $41, $10
	.word @song0ref755
	.byte $41, $18
	.word @song0ref917
	.byte $41, $28
	.word @song0ref731
	.byte $41, $10
	.word @song0ref755
	.byte $41, $10
	.word @song0ref755
	.byte $41, $10
	.word @song0ref755
	.byte $41, $10
	.word @song0ref755
	.byte $41, $18
	.word @song0ref917
	.byte $41, $10
	.word @song0ref731
	.byte $41, $0f
	.word @song0ref1099
	.byte $41, $1f
	.word @song0ref971
	.byte $41, $20
	.word @song0ref970
	.byte $41, $31
	.word @song0ref1005
	.byte $41, $19
	.word @song0ref746
	.byte $41, $10
	.word @song0ref755
	.byte $41, $10
	.word @song0ref755
	.byte $41, $10
	.word @song0ref755
	.byte $41, $10
	.word @song0ref755
	.byte $41, $10
	.word @song0ref755
	.byte $41, $10
	.word @song0ref755
	.byte $41, $10
	.word @song0ref755
	.byte $41, $10
	.word @song0ref755
	.byte $41, $10
	.word @song0ref755
	.byte $41, $10
	.word @song0ref755
	.byte $24, $9b, $27, $9b, $2b, $9b, $27, $9b, $00, $ff, $ed, $42
	.word @song0ch2loop
@song0ch3:
@song0ch3loop:
	.byte $41, $10
	.word @song0ref502
	.byte $ff, $ef, $88
@song0ref1234:
	.byte $2e, $9b, $2e, $9b, $2e, $9b, $2e, $9b
@song0ref1242:
	.byte $2e
@song0ref1243:
	.byte $85, $2e, $85, $2e, $83, $00, $85, $2e, $9b, $2e, $9b, $2e, $9b, $2f, $9b, $2e, $9b, $2e, $9b, $2e, $9b, $2f
	.byte $41, $16
	.word @song0ref1243
	.byte $85, $2e, $85, $2e, $83, $00, $85, $2e, $9b, $2e, $9b, $2e, $9b
	.byte $41, $1e
	.word @song0ref1234
	.byte $41, $17
	.word @song0ref1242
@song0ref1287:
	.byte $85, $2e, $85, $2e, $83, $00, $85, $2e, $9b, $2e, $9b, $2e, $9b, $2e, $9b, $2e, $9b, $2e, $9b, $2e, $9b, $00, $f5
	.byte $41, $1e
	.word @song0ref1234
	.byte $41, $16
	.word @song0ref1242
	.byte $41, $17
	.word @song0ref1242
	.byte $41, $16
	.word @song0ref1243
	.byte $41, $16
	.word @song0ref1243
	.byte $41, $15
	.word @song0ref1287
	.byte $41, $0e
	.word @song0ref1242
	.byte $41, $16
	.word @song0ref1234
	.byte $2e, $9b, $2e, $9b, $2e, $9b, $2e, $9b, $2f
	.byte $41, $15
	.word @song0ref1287
	.byte $41, $0e
	.word @song0ref1242
	.byte $41, $16
	.word @song0ref1234
	.byte $41, $1e
	.word @song0ref1234
	.byte $41, $0e
	.word @song0ref1242
	.byte $2e, $9b, $2e, $9b, $2e, $9b, $2e, $9b, $2f
	.byte $41, $15
	.word @song0ref1287
	.byte $41, $17
	.word @song0ref1242
	.byte $41, $15
	.word @song0ref1287
	.byte $2f
	.byte $41, $15
	.word @song0ref1287
	.byte $41, $16
	.word @song0ref1242
	.byte $41, $16
	.word @song0ref1242
	.byte $41, $16
	.word @song0ref1242
	.byte $41, $0e
	.word @song0ref1242
	.byte $41, $1f
	.word @song0ref1234
	.byte $41, $16
	.word @song0ref1243
	.byte $41, $15
	.word @song0ref1287
	.byte $41, $16
	.word @song0ref1242
	.byte $41, $17
	.word @song0ref1242
	.byte $41, $15
	.word @song0ref1287
	.byte $2f
	.byte $41, $16
	.word @song0ref1243
	.byte $41, $15
	.word @song0ref1287
	.byte $41, $0e
	.word @song0ref1242
	.byte $41, $1e
	.word @song0ref1234
	.byte $41, $16
	.word @song0ref1242
	.byte $41, $16
	.word @song0ref1242
	.byte $41, $0e
	.word @song0ref1242
	.byte $2e, $9b, $2e, $9b, $2e, $9b, $2e, $9b, $2f
	.byte $41, $15
	.word @song0ref1287
	.byte $2f
	.byte $41, $15
	.word @song0ref1287
	.byte $41, $0e
	.word @song0ref1242
	.byte $2e, $9b, $2e, $9b, $2e, $9b, $2e, $9b, $2f
	.byte $41, $16
	.word @song0ref1243
	.byte $41, $15
	.word @song0ref1287
	.byte $2f
	.byte $41, $15
	.word @song0ref1243
	.byte $41, $0e
	.word @song0ref1242
	.byte $41, $1f
	.word @song0ref1234
	.byte $41, $15
	.word @song0ref1287
	.byte $2f
	.byte $41, $15
	.word @song0ref1287
	.byte $2f
	.byte $41, $15
	.word @song0ref1287
	.byte $41, $0e
	.word @song0ref1242
	.byte $2e, $9b, $2e, $9b, $2e, $9b, $2e, $9b, $2f
	.byte $41, $15
	.word @song0ref1243
	.byte $41, $0e
	.word @song0ref1242
	.byte $41, $1f
	.word @song0ref1234
	.byte $41, $15
	.word @song0ref1287
	.byte $41, $17
	.word @song0ref1242
	.byte $41, $15
	.word @song0ref1287
	.byte $2f
	.byte $41, $15
	.word @song0ref1287
	.byte $41, $17
	.word @song0ref1242
	.byte $41, $15
	.word @song0ref1287
	.byte $41, $0e
	.word @song0ref1242
	.byte $2e, $9b, $2e, $9b, $2e, $9b, $2e, $9b, $2f
	.byte $41, $15
	.word @song0ref1287
	.byte $2f
	.byte $41, $15
	.word @song0ref1287
	.byte $41, $16
	.word @song0ref1242
	.byte $41, $0e
	.word @song0ref1242
	.byte $41, $1e
	.word @song0ref1234
	.byte $41, $0e
	.word @song0ref1242
	.byte $41, $16
	.word @song0ref1234
	.byte $41, $1f
	.word @song0ref1234
	.byte $41, $15
	.word @song0ref1287
	.byte $2f
	.byte $41, $16
	.word @song0ref1243
	.byte $41, $16
	.word @song0ref1243
	.byte $41, $16
	.word @song0ref1243
	.byte $41, $15
	.word @song0ref1243
	.byte $41, $16
	.word @song0ref1242
	.byte $41, $0e
	.word @song0ref1242
	.byte $00, $ff, $ed, $42
	.word @song0ch3loop
@song0ch4:
@song0ch4loop:
	.byte $41, $10
	.word @song0ref502
	.byte $41, $10
	.word @song0ref502
@song0ref1599:
	.byte $03, $b7, $00, $01, $9b, $03, $9b, $03, $b7, $00, $01, $b9, $03, $ab, $01, $a9, $03, $9b, $03, $b7, $00, $01, $b9
	.byte $41, $17
	.word @song0ref1599
	.byte $41, $17
	.word @song0ref1599
	.byte $03, $ab, $01, $a9, $03, $9b, $03, $b7, $00, $01, $b9
@song0ref1639:
	.byte $03, $b7, $00, $02, $9b, $03, $9b, $03, $b7, $00
@song0ref1649:
	.byte $02, $b7, $00, $03, $b7, $00, $02, $9b, $03, $9b, $03, $b7, $00, $02, $b7, $00
	.byte $41, $15
	.word @song0ref1639
	.byte $9b, $03, $8d, $03, $8b
	.byte $41, $10
	.word @song0ref1649
	.byte $41, $1a
	.word @song0ref1639
	.byte $41, $1a
	.word @song0ref1639
	.byte $41, $1a
	.word @song0ref1639
	.byte $03, $b7, $00, $02, $9b, $03, $9b, $03, $9b, $03, $8d, $03, $8b
	.byte $41, $10
	.word @song0ref1649
	.byte $41, $1a
	.word @song0ref1639
	.byte $41, $1a
	.word @song0ref1639
	.byte $41, $1a
	.word @song0ref1639
@song0ref1710:
	.byte $03
@song0ref1711:
	.byte $9b, $03, $9b, $02, $9b, $03, $9b, $03, $9b, $03, $9b, $02, $9b, $03, $9b
	.byte $41, $10
	.word @song0ref1710
	.byte $41, $10
	.word @song0ref1710
@song0ref1732:
	.byte $03, $9b, $03, $9b, $02, $9b, $03, $9b, $03, $9b, $03, $9b, $01, $02, $99, $03, $8d, $02, $8b
	.byte $41, $10
	.word @song0ref1710
	.byte $41, $10
	.word @song0ref1710
@song0ref1757:
	.byte $03, $9b, $03, $9b, $02, $9b, $03, $9b, $02, $8d, $02, $8b, $03, $9b, $02, $9b, $03, $8d, $02, $8b
	.byte $41, $1a
	.word @song0ref1639
	.byte $41, $1a
	.word @song0ref1639
	.byte $ff, $ef, $ff, $ff, $ff, $df, $ff, $ff, $ff, $df, $ff, $ff, $ff, $df, $ff, $95, $03, $9b, $01, $02, $a9, $02, $8b
	.byte $41, $1a
	.word @song0ref1639
	.byte $41, $1a
	.word @song0ref1639
	.byte $41, $1a
	.word @song0ref1639
	.byte $41, $0e
	.word @song0ref1639
	.byte $41, $0f
	.word @song0ref1711
	.byte $41, $10
	.word @song0ref1710
	.byte $41, $10
	.word @song0ref1710
	.byte $41, $13
	.word @song0ref1732
	.byte $41, $10
	.word @song0ref1710
	.byte $41, $10
	.word @song0ref1710
	.byte $41, $14
	.word @song0ref1757
	.byte $41, $1a
	.word @song0ref1639
	.byte $41, $15
	.word @song0ref1639
	.byte $9b, $03, $8d, $03, $8b
	.byte $41, $10
	.word @song0ref1649
	.byte $41, $1a
	.word @song0ref1639
	.byte $41, $1a
	.word @song0ref1639
	.byte $41, $1a
	.word @song0ref1639
	.byte $03, $b7, $00, $02, $9b, $03, $9b, $03, $b7, $00, $ff, $ff, $ab, $42
	.word @song0ch4loop
@song0ch5:
@song0ch5loop:
	.byte $41, $10
	.word @song0ref502
	.byte $41, $10
	.word @song0ref502
	.byte $41, $0e
	.word @song0ref502
	.byte $ab, $84
@song0ref1890:
	.byte $1b, $d7, $27, $9b, $27, $9b, $27, $9b, $00, $b9, $1b, $d7, $27, $9b, $27, $9b, $22, $9b, $00, $b9
	.byte $41, $14
	.word @song0ref1890
	.byte $41, $14
	.word @song0ref1890
	.byte $41, $14
	.word @song0ref1890
@song0ref1919:
	.byte $18, $b9, $2c, $9b, $2c, $9b, $2c, $9b, $2c, $9b, $00, $b9, $1d, $b9, $1d, $9b, $1d, $9b, $1d, $9b, $1d, $9b, $00, $b9
	.byte $41, $14
	.word @song0ref1890
	.byte $41, $14
	.word @song0ref1890
	.byte $41, $14
	.word @song0ref1890
	.byte $1d, $b9, $27, $9b, $27, $9b, $27, $9b, $27, $9b, $00, $b9, $1d, $b9, $1d, $9b, $1d, $9b, $1d, $9b, $1d, $9b
@song0ref1974:
	.byte $1e, $b9, $1e, $9b, $1e
@song0ref1979:
	.byte $9b, $1e, $9b, $1e, $9b, $1e, $9b, $1e, $9b
@song0ref1988:
	.byte $25, $9b, $19, $9b, $19, $9b, $19, $9b, $1d, $9b, $1d, $9b, $1d, $9b, $1d
	.byte $41, $11
	.word @song0ref1979
	.byte $41, $0f
	.word @song0ref1988
	.byte $41, $11
	.word @song0ref1979
@song0ref2012:
	.byte $1b, $b9, $27, $9b, $27, $9b, $1d, $b9, $25, $9b, $25, $9b, $19, $b9, $25, $9b, $25, $9b, $22, $b9, $27, $9b, $27, $9b
	.byte $17, $b9, $23, $9b, $23, $9b, $16, $b9, $1d, $9b, $1d, $9b, $00, $b9
	.byte $41, $14
	.word @song0ref1890
	.byte $41, $13
	.word @song0ref1890
	.byte $ff, $ed
	.byte $41, $0e
	.word @song0ref502
	.byte $ab
	.byte $41, $14
	.word @song0ref1890
	.byte $41, $14
	.word @song0ref1890
	.byte $41, $16
	.word @song0ref1919
	.byte $41, $1d
	.word @song0ref1974
	.byte $41, $11
	.word @song0ref1979
	.byte $41, $0f
	.word @song0ref1988
	.byte $41, $11
	.word @song0ref1979
	.byte $41, $26
	.word @song0ref2012
	.byte $41, $10
	.word @song0ref1890
	.byte $27, $9b, $00, $b9
	.byte $41, $10
	.word @song0ref1890
	.byte $27, $9b, $00, $b9
	.byte $41, $14
	.word @song0ref1890
	.byte $41, $14
	.word @song0ref1890
	.byte $41, $14
	.word @song0ref1890
	.byte $41, $13
	.word @song0ref1890
	.byte $ff, $ed, $42
	.word @song0ch5loop
@song0ch6:
@song0ch6loop:
	.byte $41, $10
	.word @song0ref502
	.byte $41, $10
	.word @song0ref502
	.byte $41, $0e
	.word @song0ref502
	.byte $ab, $84
@song0ref2129:
	.byte $22, $d7, $2c, $9b, $2b, $9b, $00, $9b, $bb, $22, $d7, $2c, $9b, $2b, $9b, $27, $9b, $00, $b9
	.byte $41, $13
	.word @song0ref2129
	.byte $41, $13
	.word @song0ref2129
	.byte $41, $13
	.word @song0ref2129
@song0ref2157:
	.byte $1d, $b9, $27, $9b, $27, $9b, $27, $9b, $27, $9b, $00, $b9, $20, $b9, $20, $9b, $20, $9b, $20, $9b, $20, $9b, $00, $b9
	.byte $41, $13
	.word @song0ref2129
	.byte $41, $13
	.word @song0ref2129
	.byte $41, $13
	.word @song0ref2129
	.byte $20, $b9, $20, $9b, $20, $9b, $20, $9b, $20, $9b, $00, $b9, $20, $b9, $20, $9b, $20, $9b, $20, $9b, $20, $9b
@song0ref2212:
	.byte $23, $b9, $23, $9b, $23
@song0ref2217:
	.byte $9b, $23, $9b, $23, $9b, $23, $9b, $23, $9b
@song0ref2226:
	.byte $22, $9b, $1e, $9b, $1e, $9b, $1e, $9b, $20, $9b, $20, $9b, $20, $9b, $20
	.byte $41, $11
	.word @song0ref2217
	.byte $41, $0f
	.word @song0ref2226
	.byte $41, $12
	.word @song0ref2217
@song0ref2250:
	.byte $b9, $23, $9b, $23, $9b, $20, $b9, $20, $9b, $20, $9b, $1e, $b9, $22, $9b, $22, $9b, $27, $b9, $22, $9b, $22, $9b, $1e
	.byte $b9, $1e, $9b, $1e, $9b, $1d, $b9, $22, $9b, $22, $9b, $00, $b9
	.byte $41, $13
	.word @song0ref2129
	.byte $41, $12
	.word @song0ref2129
	.byte $ff, $ed
	.byte $41, $0e
	.word @song0ref502
	.byte $ab
	.byte $41, $13
	.word @song0ref2129
	.byte $41, $13
	.word @song0ref2129
	.byte $41, $16
	.word @song0ref2157
	.byte $41, $1d
	.word @song0ref2212
	.byte $41, $11
	.word @song0ref2217
	.byte $41, $0f
	.word @song0ref2226
	.byte $41, $12
	.word @song0ref2217
	.byte $41, $25
	.word @song0ref2250
	.byte $41, $0f
	.word @song0ref2129
	.byte $1b, $9b, $00, $b9
	.byte $41, $0f
	.word @song0ref2129
	.byte $1b, $9b, $00, $b9
	.byte $41, $13
	.word @song0ref2129
	.byte $41, $13
	.word @song0ref2129
	.byte $41, $13
	.word @song0ref2129
	.byte $41, $12
	.word @song0ref2129
	.byte $ff, $ed, $42
	.word @song0ch6loop
