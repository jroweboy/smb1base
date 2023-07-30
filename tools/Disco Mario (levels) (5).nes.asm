;GAME LEVELS DATA

WorldAddrOffsets:
      .byte World1Areas-AreaAddrOffsets, World2Areas-AreaAddrOffsets
      .byte World3Areas-AreaAddrOffsets, World4Areas-AreaAddrOffsets
      .byte World5Areas-AreaAddrOffsets, World6Areas-AreaAddrOffsets
      .byte World7Areas-AreaAddrOffsets, World8Areas-AreaAddrOffsets

AreaAddrOffsets:
World1Areas: .byte $25, $29, $c0, $26, $28
World2Areas: .byte $29, $01, $27, $62, $24
World3Areas: .byte $24, $35, $20, $63, $22
World4Areas: .byte $22, $29, $41, $2c, $61
World5Areas: .byte $2a, $31, $26, $62, $2e
World6Areas: .byte $2e, $23, $2d, $60, $33
World7Areas: .byte $33, $29, $01, $27, $64
World8Areas: .byte $30, $32, $21, $65, $1f

EnemyAddrHOffsets:
      .byte EnemyDataAddrLow_WaterStart - EnemyDataAddrLow          ; Water
      .byte EnemyDataAddrLow_GroundStart - EnemyDataAddrLow         ; Ground
      .byte EnemyDataAddrLow_UndergroundStart - EnemyDataAddrLow    ; Underground
      .byte EnemyDataAddrLow_CastleStart - EnemyDataAddrLow         ; castle

EnemyDataAddrLow:
      ; Water
      EnemyDataAddrLow_WaterStart:
      .byte <E_WaterArea1, <E_WaterArea2, <E_WaterArea3
      ; Ground
      EnemyDataAddrLow_GroundStart:
      .byte <E_GroundArea1, <E_GroundArea2, <E_GroundArea3, <E_GroundArea4, <E_GroundArea5, <E_GroundArea6
      .byte <E_GroundArea7, <E_GroundArea8, <E_GroundArea9, <E_GroundArea10, <E_GroundArea11, <E_GroundArea12
      .byte <E_GroundArea13, <E_GroundArea14, <E_GroundArea15, <E_GroundArea16, <E_GroundArea17, <E_GroundArea18
      .byte <E_GroundArea19, <E_GroundArea20, <E_GroundArea21, <E_GroundArea22
      ; Underground
      EnemyDataAddrLow_UndergroundStart:
      .byte <E_UndergroundArea1, <E_UndergroundArea2, <E_UndergroundArea3
      ; Castle
      EnemyDataAddrLow_CastleStart:
      .byte <E_CastleArea1, <E_CastleArea2, <E_CastleArea3, <E_CastleArea4, <E_CastleArea5, <E_CastleArea6

EnemyDataAddrHigh:
      ; Water
      .byte >E_WaterArea1, >E_WaterArea2, >E_WaterArea3
      ; Ground
      .byte >E_GroundArea1, >E_GroundArea2, >E_GroundArea3, >E_GroundArea4, >E_GroundArea5, >E_GroundArea6
      .byte >E_GroundArea7, >E_GroundArea8, >E_GroundArea9, >E_GroundArea10, >E_GroundArea11, >E_GroundArea12
      .byte >E_GroundArea13, >E_GroundArea14, >E_GroundArea15, >E_GroundArea16, >E_GroundArea17, >E_GroundArea18
      .byte >E_GroundArea19, >E_GroundArea20, >E_GroundArea21, >E_GroundArea22
      ; Underground
      .byte >E_UndergroundArea1, >E_UndergroundArea2, >E_UndergroundArea3
      ; Castle
      .byte >E_CastleArea1, >E_CastleArea2, >E_CastleArea3, >E_CastleArea4, >E_CastleArea5, >E_CastleArea6

AreaDataHOffsets:
      .byte AreaDataAddrLow_WaterStart - AreaDataAddrLow          ; Water
      .byte AreaDataAddrLow_GroundStart - AreaDataAddrLow         ; Ground
      .byte AreaDataAddrLow_UndergroundStart - AreaDataAddrLow    ; Underground
      .byte AreaDataAddrLow_CastleStart - AreaDataAddrLow         ; castle

AreaDataAddrLow:
      ; Water
      AreaDataAddrLow_WaterStart:
      .byte <L_WaterArea1, <L_WaterArea2, <L_WaterArea3
      ; Ground
      AreaDataAddrLow_GroundStart:
      .byte <L_GroundArea1, <L_GroundArea2, <L_GroundArea3, <L_GroundArea4, <L_GroundArea5, <L_GroundArea6
      .byte <L_GroundArea7, <L_GroundArea8, <L_GroundArea9, <L_GroundArea10, <L_GroundArea11, <L_GroundArea12
      .byte <L_GroundArea13, <L_GroundArea14, <L_GroundArea15, <L_GroundArea16, <L_GroundArea17, <L_GroundArea18
      .byte <L_GroundArea19, <L_GroundArea20, <L_GroundArea21, <L_GroundArea22
      ; Underground
      AreaDataAddrLow_UndergroundStart:
      .byte <L_UndergroundArea1, <L_UndergroundArea2, <L_UndergroundArea3
      ; Castle
      AreaDataAddrLow_CastleStart:
      .byte <L_CastleArea1, <L_CastleArea2, <L_CastleArea3, <L_CastleArea4, <L_CastleArea5, <L_CastleArea6

AreaDataAddrHigh:
      ; Water
      .byte >L_WaterArea1, >L_WaterArea2, >L_WaterArea3
      ; Ground
      .byte >L_GroundArea1, >L_GroundArea2, >L_GroundArea3, >L_GroundArea4, >L_GroundArea5, >L_GroundArea6
      .byte >L_GroundArea7, >L_GroundArea8, >L_GroundArea9, >L_GroundArea10, >L_GroundArea11, >L_GroundArea12
      .byte >L_GroundArea13, >L_GroundArea14, >L_GroundArea15, >L_GroundArea16, >L_GroundArea17, >L_GroundArea18
      .byte >L_GroundArea19, >L_GroundArea20, >L_GroundArea21, >L_GroundArea22
      ; Underground
      .byte >L_UndergroundArea1, >L_UndergroundArea2, >L_UndergroundArea3
      ; Castle
      .byte >L_CastleArea1, >L_CastleArea2, >L_CastleArea3, >L_CastleArea4, >L_CastleArea5, >L_CastleArea6

E_WaterArea1:
      .byte $3b, $87, $66, $27, $cc, $27, $ee, $31, $87, $ee
      .byte $23, $a7, $3b, $87, $db, $07, $ff

E_WaterArea2:
      .byte $0f, $01, $2e, $25, $2b, $2e, $25, $4b, $4e, $25
      .byte $cb, $6b, $07, $97, $47, $e9, $87, $47, $c7, $7a
      .byte $07, $d6, $c7, $78, $07, $38, $87, $ab, $47, $e3
      .byte $07, $9b, $87, $0f, $09, $68, $47, $db, $c7, $3b
      .byte $c7, $ff

E_WaterArea3:
      .byte $47, $9b, $cb, $07, $fa, $1d, $86, $9b, $3a, $87
      .byte $56, $07, $88, $1b, $07, $9d, $2e, $65, $f0, $ff

E_GroundArea1:
      .byte $a5, $86, $e4, $28, $18, $a8, $45, $83, $69, $03
      .byte $c6, $29, $9b, $83, $16, $a4, $88, $24, $e9, $28
      .byte $05, $a8, $7b, $28, $24, $8f, $c8, $03, $e8, $03
      .byte $46, $a8, $85, $24, $c8, $24, $ff

E_GroundArea2:
      .byte $eb, $8e, $0f, $03, $17, $05, $aa, $05, $db, $8e
      .byte $0f, $07, $57, $05, $9b, $80, $2b, $85, $fb, $05
      .byte $0f, $0b, $1b, $05, $9b, $05, $ff

E_GroundArea3:
      .byte $2e, $c2, $66, $e2, $11, $0f, $07, $02, $11, $0f
      .byte $0c, $12, $11, $ff

E_GroundArea4:
      .byte $0e, $c2, $a8, $bb, $8e, $6b, $82, $de, $00, $a0
      .byte $33, $86, $43, $06, $3e, $b4, $a0, $0f, $07, $7e
      .byte $42, $a6, $83, $02, $0f, $0a, $3b, $02, $cb, $37
      .byte $0f, $0c, $e3, $0e, $ff

E_GroundArea5:
      .byte $9b, $8e, $ca, $0e, $ee, $42, $44, $5b, $86, $80
      .byte $b8, $1b, $80, $50, $ba, $10, $b7, $5b, $00, $17
      .byte $85, $4b, $05, $fe, $34, $40, $b7, $86, $c6, $06
      .byte $5b, $80, $83, $00, $d0, $38, $5b, $8e, $8a, $0e
      .byte $a6, $00, $bb, $0e, $c5, $80, $f3, $00, $ff

E_GroundArea6:
      .byte $1e, $c2, $00, $aa, $3a, $8a, $ba, $42, $bd, $7d
      .byte $25, $f3, $03, $96, $86, $19, $86, $17, $86, $5a
      .byte $24, $9d, $24, $5a, $bb, $7b, $b8, $f7, $03, $d8
      .byte $90, $0d, $a8, $4a, $b7, $d4, $06, $fe, $2f, $00
      .byte $6d, $ea, $ff

E_GroundArea7:
      .byte $7b, $a8, $77, $8e, $58, $a4, $8d, $24, $81, $43
      .byte $cb, $85, $2b, $d7, $35, $06, $9a, $83, $3e, $c1
      .byte $40, $c5, $0f, $34, $a8, $b4, $28, $8b, $90, $cb
      .byte $85, $f2, $ce, $ff

E_GroundArea8:
      .byte $0f, $02, $78, $40, $48, $ce, $f8, $c3, $f8, $c3
      .byte $0f, $07, $7b, $43, $c6, $d0, $0f, $8a, $c8, $50
      .byte $ff

E_GroundArea9:
      .byte $87, $86, $ea, $03, $1b, $80, $4c, $b7, $67, $03
      .byte $fe, $2b, $60, $2b, $b7, $2b, $00, $7b, $38, $ab
      .byte $b8, $77, $86, $fe, $42, $20, $49, $86, $f8, $06
      .byte $9b, $80, $7b, $8e, $5b, $b7, $68, $0e, $18, $8e
      .byte $9b, $00, $ff

E_GroundArea10:
      .byte $ff

E_GroundArea11:
      .byte $0b, $80, $60, $38, $10, $b8, $c0, $3b, $db, $8e
      .byte $40, $b8, $f0, $38, $7b, $8e, $a0, $b8, $c0, $b8
      .byte $fb, $00, $a0, $b8, $30, $bb, $ee, $42, $88, $0f
      .byte $0b, $2b, $0e, $67, $0e, $ff

E_GroundArea12:
      .byte $0a, $aa, $0e, $28, $64, $0e, $31, $88, $ff

E_GroundArea13:
      .byte $c7, $83, $d7, $03, $42, $8f, $7a, $03, $05, $a4
      .byte $78, $24, $a6, $25, $e4, $25, $4b, $83, $e3, $03
      .byte $05, $a4, $89, $24, $b5, $24, $09, $a4, $65, $24
      .byte $c9, $24, $0f, $08, $85, $25, $ff

E_GroundArea14:
      .byte $cd, $a5, $b5, $a8, $07, $a8, $76, $28, $cc, $25
      .byte $65, $a4, $a9, $24, $e5, $24, $19, $a4, $0f, $07
      .byte $95, $28, $e6, $24, $19, $a4, $d7, $29, $16, $a9
      .byte $58, $29, $97, $29, $ff

E_GroundArea15:
      .byte $0f, $02, $a3, $11, $0f, $07, $02, $11, $ff

E_GroundArea16:
      .byte $ff

E_GroundArea17:
      .byte $2b, $82, $ab, $38, $1b, $b8, $eb, $3b, $db, $80
      .byte $8b, $b8, $1b, $82, $fb, $b8, $7b, $80, $fb, $3c
      .byte $5b, $bc, $7b, $b8, $1b, $8e, $cb, $0e, $1b, $8e
      .byte $0f, $0d, $2b, $3b, $bb, $b8, $eb, $82, $4b, $b8
      .byte $bb, $38, $3b, $b7, $bb, $02, $0f, $13, $1b, $00
      .byte $cb, $80, $6b, $bc, $ff

E_GroundArea18:
      .byte $7b, $80, $ae, $00, $80, $8b, $8e, $e8, $05, $f9
      .byte $86, $17, $86, $16, $85, $4e, $2b, $80, $ab, $8e
      .byte $87, $85, $c3, $05, $8b, $82, $9b, $02, $ab, $02
      .byte $bb, $86, $cb, $06, $d3, $03, $3b, $8e, $6b, $0e
      .byte $a7, $8e, $ff

E_GroundArea19:
      .byte $29, $8e, $52, $11, $83, $0e, $0f, $03, $9b, $0e
      .byte $2b, $8e, $5b, $0e, $cb, $8e, $fb, $0e, $fb, $82
      .byte $9b, $82, $bb, $02, $fe, $42, $e8, $bb, $8e, $0f
      .byte $0a, $ab, $0e, $cb, $0e, $f9, $0e, $88, $86, $a6
      .byte $06, $db, $02, $b6, $8e, $ff

E_GroundArea20:
      .byte $ab, $ce, $de, $42, $c0, $cb, $ce, $5b, $8e, $1b
      .byte $ce, $4b, $85, $67, $45, $0f, $07, $2b, $00, $e9
      .byte $68, $7b, $85, $97, $05, $0f, $0a, $92, $02, $ff

E_GroundArea21:
      .byte $0a, $aa, $0e, $40, $02, $1e, $23, $aa, $ff

E_GroundArea22:
      .byte $1b, $80, $4b, $bc, $0f, $04, $2b, $00, $ab, $38
      .byte $eb, $00, $cb, $8e, $fb, $80, $ab, $b8, $6b, $80
      .byte $fb, $3c, $9b, $bb, $5b, $bc, $fb, $00, $6b, $b8
      .byte $fb, $38, $ff

E_UndergroundArea1:
      .byte $07, $8e, $39, $29, $be, $34, $20, $d5, $03, $f9
      .byte $46, $5d, $a8, $2a, $86, $54, $0e, $ee, $42, $22
      .byte $2d, $a8, $c3, $03, $f0, $39, $87, $80, $5a, $b9
      .byte $f7, $85, $78, $80, $b9, $29, $39, $a9, $87, $43
      .byte $b9, $29, $ee, $23, $2c, $a9, $82, $ff

E_UndergroundArea2:
      .byte $0f, $02, $0e, $26, $47, $c6, $e7, $db, $80, $3b
      .byte $82, $8b, $02, $70, $bb, $25, $a7, $2c, $27, $b2
      .byte $26, $b9, $26, $9b, $80, $a8, $82, $b5, $27, $bc
      .byte $27, $b0, $bb, $87, $b4, $ff

E_UndergroundArea3:
      .byte $1e, $a5, $0a, $2e, $28, $27, $2e, $33, $c7, $0f
      .byte $03, $1e, $40, $07, $2e, $30, $e7, $0f, $05, $1e
      .byte $24, $44, $0f, $07, $1e, $22, $6a, $2e, $23, $ab
      .byte $0f, $09, $1e, $41, $68, $1e, $2a, $8a, $2e, $23
      .byte $a2, $2e, $32, $ea, $ff

E_CastleArea1:
      .byte $76, $dd, $bb, $4c, $ea, $1d, $1b, $cc, $56, $5d
      .byte $16, $9d, $c6, $1d, $36, $9d, $c9, $1d, $04, $db
      .byte $49, $1d, $84, $1b, $c9, $5d, $88, $95, $0f, $08
      .byte $30, $4c, $78, $2d, $a6, $28, $90, $b5, $ff

E_CastleArea2:
      .byte $0f, $03, $56, $1b, $c9, $1b, $0f, $07, $36, $1b
      .byte $aa, $1b, $48, $95, $0f, $0a, $2a, $1b, $5b, $0c
      .byte $78, $2d, $90, $b5, $ff

E_CastleArea3:
      .byte $0b, $8c, $4b, $4c, $77, $5f, $eb, $0c, $bd, $db
      .byte $19, $9d, $75, $1d, $7d, $5b, $d9, $1d, $3d, $dd
      .byte $99, $1d, $26, $9d, $5a, $2b, $8a, $2c, $ca, $1b
      .byte $20, $95, $7b, $5c, $db, $4c, $1b, $cc, $3b, $cc
      .byte $78, $2d, $a6, $28, $90, $b5, $ff

E_CastleArea4:
      .byte $0b, $8c, $3b, $1d, $8b, $1d, $ab, $0c, $db, $1d
      .byte $0f, $03, $65, $1d, $6b, $1b, $05, $9d, $0b, $1b
      .byte $05, $9b, $0b, $1d, $8b, $0c, $1b, $8c, $70, $15
      .byte $7b, $0c, $db, $0c, $0f, $08, $78, $2d, $a6, $28
      .byte $90, $b5, $ff

E_CastleArea5:
      .byte $27, $a9, $4b, $0c, $68, $29, $0f, $06, $77, $1b
      .byte $0f, $0b, $60, $15, $4b, $8c, $78, $2d, $90, $b5
      .byte $ff

E_CastleArea6:
      .byte $0f, $03, $8e, $65, $e1, $bb, $38, $6d, $a8, $3e
      .byte $e5, $e7, $0f, $08, $0b, $02, $2b, $02, $5e, $65
      .byte $e1, $bb, $0e, $db, $0e, $bb, $8e, $db, $0e, $fe
      .byte $65, $ec, $0f, $0d, $4e, $65, $e1, $0f, $0e, $4e
      .byte $02, $e0, $0f, $10, $fe, $e5, $e1, $1b, $85, $7b
      .byte $0c, $5b, $95, $78, $2d, $90, $b5, $ff

L_WaterArea1:
      .byte $41, $01, $b4, $34, $c8, $52, $f2, $51, $47, $d3
      .byte $6c, $03, $65, $49, $9e, $07, $be, $01, $cc, $03
      .byte $fe, $07, $0d, $c9, $1e, $01, $6c, $01, $62, $35
      .byte $63, $53, $8a, $41, $ac, $01, $b3, $53, $e9, $51
      .byte $26, $c3, $27, $33, $63, $43, $64, $33, $ba, $60
      .byte $c9, $61, $ce, $0b, $e5, $09, $ee, $0f, $7d, $ca
      .byte $7d, $47, $fd

L_WaterArea2:
      .byte $41, $01, $b8, $52, $ea, $41, $27, $b2, $b3, $42
      .byte $16, $d4, $08, $e2, $16, $64, $2c, $04, $76, $64
      .byte $88, $62, $de, $07, $fe, $01, $0d, $c9, $23, $32
      .byte $31, $51, $98, $52, $0d, $c9, $59, $42, $63, $53
      .byte $67, $31, $14, $c2, $36, $31, $87, $53, $17, $e3
      .byte $29, $61, $30, $62, $3c, $08, $42, $37, $59, $40
      .byte $6a, $42, $99, $40, $c9, $61, $d7, $63, $39, $d1
      .byte $58, $52, $c3, $67, $d3, $31, $dc, $06, $f7, $42
      .byte $fa, $42, $23, $b1, $43, $67, $c3, $34, $c7, $34
      .byte $d1, $51, $43, $b3, $47, $33, $9a, $30, $a9, $61
      .byte $b8, $62, $be, $0b, $d5, $09, $de, $0f, $0d, $ca
      .byte $7d, $47, $fd

L_WaterArea3:
      .byte $49, $0f, $1e, $01, $39, $73, $5e, $07, $ae, $0b
      .byte $1e, $82, $6e, $88, $9e, $02, $0d, $04, $2e, $0b
      .byte $45, $09, $4e, $0f, $ed, $47, $fd

L_GroundArea1:
      .byte $94, $11, $0f, $26, $fe, $10, $28, $94, $65, $15
      .byte $eb, $12, $4a, $96, $b7, $13, $e9, $19, $f5, $15
      .byte $11, $80, $71, $13, $15, $92, $1b, $1f, $24, $40
      .byte $55, $12, $64, $40, $95, $12, $a4, $40, $d2, $12
      .byte $e1, $40, $13, $c0, $2c, $17, $2f, $12, $49, $13
      .byte $83, $40, $9f, $14, $a3, $40, $17, $92, $83, $13
      .byte $92, $41, $b9, $14, $c5, $12, $c8, $40, $d4, $40
      .byte $4b, $92, $78, $1b, $9c, $94, $9f, $11, $df, $14
      .byte $fe, $11, $7d, $c1, $9e, $42, $cf, $20, $fd

L_GroundArea2:
      .byte $90, $b1, $0f, $26, $7e, $c2, $fe, $40, $2e, $c0
      .byte $4e, $c0, $0d, $0a, $5e, $40, $88, $72, $be, $42
      .byte $e7, $87, $2e, $c0, $65, $41, $be, $42, $ef, $20
      .byte $fe, $31, $fd

L_GroundArea3:
      .byte $52, $21, $0f, $20, $6e, $40, $58, $f2, $93, $01
      .byte $97, $00, $0c, $81, $97, $40, $a6, $41, $c7, $40
      .byte $0d, $04, $03, $01, $07, $01, $23, $01, $27, $01
      .byte $ec, $03, $ac, $f3, $c3, $03, $78, $e2, $94, $43
      .byte $47, $f3, $74, $43, $47, $fb, $74, $43, $2c, $f1
      .byte $4c, $63, $47, $00, $57, $21, $5c, $01, $7c, $72
      .byte $39, $f1, $ec, $02, $4c, $81, $d8, $62, $ec, $01
      .byte $0d, $0d, $0f, $38, $c7, $07, $ed, $4a, $1d, $c1
      .byte $5f, $26, $fd

L_GroundArea4:
      .byte $50, $31, $0f, $26, $a7, $22, $37, $fb, $73, $20
      .byte $83, $07, $93, $20, $c7, $73, $06, $b1, $37, $a0
      .byte $47, $04, $e5, $71, $e7, $31, $33, $a4, $39, $71
      .byte $a9, $71, $d3, $23, $08, $f2, $13, $05, $27, $02
      .byte $49, $71, $75, $75, $e8, $72, $67, $f3, $99, $71
      .byte $e7, $20, $f4, $72, $f7, $31, $17, $a0, $33, $20
      .byte $39, $71, $73, $28, $bc, $05, $39, $f1, $79, $71
      .byte $a6, $21, $c3, $06, $d3, $20, $dc, $00, $fc, $00
      .byte $07, $a2, $13, $21, $5f, $32, $8c, $00, $98, $7a
      .byte $c7, $63, $d9, $61, $03, $a2, $07, $22, $74, $72
      .byte $77, $31, $e7, $73, $39, $f1, $58, $72, $77, $73
      .byte $d8, $72, $29, $f3, $6f, $33, $a6, $64, $b5, $65
      .byte $c4, $66, $d3, $67, $e3, $67, $7d, $c1, $cf, $26
      .byte $fd

L_GroundArea5:
      .byte $52, $31, $0f, $20, $6e, $66, $07, $81, $36, $01
      .byte $66, $00, $a7, $22, $08, $f2, $67, $7b, $dc, $02
      .byte $98, $f2, $d7, $20, $39, $f1, $9f, $33, $dc, $27
      .byte $dc, $57, $23, $83, $57, $63, $6c, $51, $87, $63
      .byte $99, $61, $a3, $06, $b3, $21, $77, $f3, $f3, $21
      .byte $f7, $2a, $13, $81, $23, $22, $53, $00, $63, $22
      .byte $e9, $0b, $0c, $83, $13, $21, $16, $22, $33, $05
      .byte $8f, $35, $ec, $01, $63, $a0, $67, $20, $73, $01
      .byte $77, $01, $83, $20, $87, $20, $b3, $20, $b7, $20
      .byte $c3, $01, $c7, $00, $d3, $20, $d7, $20, $67, $a0
      .byte $77, $07, $87, $22, $e8, $62, $f5, $65, $1c, $82
      .byte $7f, $38, $8d, $c1, $cf, $26, $fd

L_GroundArea6:
      .byte $53, $21, $4f, $b3, $7e, $10, $87, $3f, $ac, $1a
      .byte $7e, $a1, $87, $63, $98, $62, $a9, $61, $ba, $30
      .byte $17, $a4, $37, $01, $6e, $10, $bb, $17, $c3, $18
      .byte $0b, $95, $69, $1c, $76, $19, $c1, $03, $47, $91
      .byte $77, $11, $a7, $11, $e7, $16, $13, $86, $80, $65
      .byte $c0, $62, $c5, $67, $ce, $31, $0c, $a2, $15, $40
      .byte $3e, $31, $47, $02, $5e, $22, $60, $62, $66, $64
      .byte $f8, $31, $f6, $71, $17, $82, $27, $02, $37, $31
      .byte $34, $7a, $39, $41, $57, $02, $67, $02, $76, $71
      .byte $78, $31, $e8, $01, $f7, $63, $7e, $90, $70, $67
      .byte $84, $32, $ab, $12, $b9, $0b, $b4, $50, $c4, $32
      .byte $f4, $12, $2e, $a1, $39, $73, $7c, $1b, $a8, $28
      .byte $e8, $01, $67, $f3, $86, $74, $a5, $75, $c4, $76
      .byte $e3, $77, $8d, $c1, $ef, $26, $9d, $c6, $09, $89
      .byte $0e, $2f, $ed, $42, $fd

L_GroundArea7:
      .byte $94, $61, $0f, $26, $c8, $24, $d7, $22, $e6, $06
      .byte $28, $82, $3e, $10, $46, $66, $66, $22, $76, $04
      .byte $a6, $6f, $bc, $2f, $dc, $1a, $b6, $e6, $fe, $21
      .byte $56, $d7, $5e, $30, $81, $56, $97, $22, $b7, $55
      .byte $be, $21, $17, $94, $35, $40, $75, $16, $15, $94
      .byte $93, $11, $9c, $00, $ae, $31, $cc, $00, $c3, $11
      .byte $d8, $40, $fe, $21, $24, $9a, $5a, $35, $2e, $b1
      .byte $39, $73, $66, $40, $87, $7b, $a7, $02, $b4, $03
      .byte $b7, $02, $d1, $69, $ec, $01, $fe, $16, $00, $cf
      .byte $45, $76, $a5, $76, $fe, $21, $6c, $83, $27, $92
      .byte $4e, $31, $77, $40, $7e, $21, $98, $40, $95, $12
      .byte $c6, $03, $d7, $02, $ee, $31, $f4, $76, $3c, $83
      .byte $98, $40, $9f, $33, $a6, $40, $dc, $00, $e4, $66
      .byte $f3, $67, $03, $e7, $9d, $41, $ce, $42, $ff, $23
      .byte $ed, $c2, $fd

L_GroundArea8:
      .byte $90, $11, $0f, $26, $6e, $10, $8b, $17, $af, $32
      .byte $d8, $62, $e8, $62, $fc, $3f, $ad, $c8, $0c, $be
      .byte $f8, $64, $0c, $bf, $b3, $40, $f8, $64, $48, $e4
      .byte $5c, $39, $83, $40, $92, $41, $b3, $40, $f8, $64
      .byte $48, $e4, $5c, $39, $f8, $64, $13, $c2, $37, $65
      .byte $4c, $24, $63, $00, $97, $65, $c3, $42, $0b, $97
      .byte $ac, $32, $f8, $64, $0c, $be, $53, $45, $9d, $48
      .byte $f8, $64, $2a, $e2, $3c, $47, $56, $43, $ba, $62
      .byte $f8, $64, $0c, $b7, $88, $64, $bc, $31, $d4, $45
      .byte $fc, $31, $3c, $b1, $78, $64, $8c, $38, $0b, $9c
      .byte $1a, $33, $18, $61, $28, $61, $39, $60, $5d, $4a
      .byte $ee, $11, $0f, $b8, $1d, $c1, $3e, $42, $6f, $20
      .byte $fd

L_GroundArea9:
      .byte $52, $a1, $0f, $23, $5e, $40, $c7, $23, $07, $84
      .byte $17, $29, $be, $31, $c7, $02, $c4, $03, $e3, $13
      .byte $e7, $60, $ea, $60, $47, $e3, $58, $62, $69, $61
      .byte $7a, $60, $9f, $33, $97, $40, $a6, $40, $e7, $73
      .byte $67, $e3, $9c, $01, $47, $a0, $53, $06, $63, $22
      .byte $a4, $04, $a7, $73, $b2, $01, $c4, $40, $15, $c4
      .byte $13, $21, $1c, $70, $33, $05, $43, $21, $5c, $70
      .byte $90, $63, $cc, $03, $04, $c6, $07, $46, $77, $7b
      .byte $93, $44, $9c, $01, $e3, $73, $e7, $31, $f0, $01
      .byte $39, $f1, $36, $04, $7c, $00, $a7, $73, $d3, $04
      .byte $e8, $72, $e3, $22, $26, $f4, $69, $60, $83, $27
      .byte $a8, $40, $ac, $04, $37, $93, $7e, $00, $8c, $01
      .byte $a8, $3d, $d5, $60, $05, $e0, $02, $01, $35, $30
      .byte $b5, $60, $be, $31, $08, $f2, $55, $24, $93, $22
      .byte $97, $22, $c9, $0b, $e1, $69, $f1, $69, $8d, $c1
      .byte $cf, $26, $ed, $c2, $f1, $69, $fd

L_GroundArea10:
      .byte $38, $11, $0f, $26, $cd, $40, $3d, $c7, $fd

L_GroundArea11:
      .byte $95, $b1, $0f, $26, $0d, $02, $c8, $72, $1c, $81
      .byte $38, $72, $0d, $05, $97, $34, $98, $62, $a3, $20
      .byte $b3, $06, $c3, $20, $cc, $03, $f9, $91, $2c, $81
      .byte $48, $62, $0d, $09, $37, $63, $47, $03, $57, $21
      .byte $8c, $02, $c5, $79, $c7, $31, $f9, $11, $39, $f1
      .byte $a9, $11, $6f, $b4, $d3, $65, $e3, $65, $7d, $c1
      .byte $bf, $26, $fd

L_GroundArea12:
      .byte $00, $c1, $4c, $00, $f4, $4f, $0d, $02, $02, $42
      .byte $43, $4f, $52, $c2, $de, $00, $5a, $c2, $4d, $c7
      .byte $fd

L_GroundArea13:
      .byte $90, $51, $0f, $26, $ee, $10, $0b, $94, $33, $14
      .byte $42, $42, $77, $16, $86, $44, $02, $92, $4a, $16
      .byte $69, $42, $73, $14, $b0, $00, $c7, $12, $05, $c0
      .byte $1c, $17, $1f, $11, $36, $12, $8f, $14, $91, $40
      .byte $1b, $94, $35, $12, $34, $42, $60, $42, $61, $12
      .byte $87, $12, $96, $40, $a3, $14, $1c, $98, $1f, $11
      .byte $47, $12, $9f, $15, $cc, $15, $cf, $11, $05, $c0
      .byte $1f, $15, $39, $12, $7c, $16, $7f, $11, $82, $40
      .byte $98, $12, $df, $15, $16, $c4, $17, $14, $54, $12
      .byte $9b, $16, $28, $94, $ce, $01, $3d, $c1, $5e, $42
      .byte $8f, $20, $fd

L_GroundArea14:
      .byte $97, $11, $0f, $26, $fe, $10, $2b, $92, $57, $12
      .byte $c0, $41, $5b, $92, $19, $93, $17, $94, $7c, $14
      .byte $7f, $11, $93, $41, $bf, $15, $fc, $13, $ff, $11
      .byte $2f, $95, $50, $42, $51, $12, $58, $14, $a6, $12
      .byte $db, $12, $1b, $93, $46, $43, $7b, $12, $8d, $49
      .byte $b7, $14, $1b, $94, $49, $0b, $bb, $12, $fc, $13
      .byte $ff, $12, $03, $c1, $2f, $15, $43, $12, $4b, $13
      .byte $77, $13, $9d, $4a, $15, $c1, $a1, $41, $c3, $12
      .byte $fe, $01, $7d, $c1, $9e, $42, $cf, $20, $fd

L_GroundArea15:
      .byte $52, $21, $0f, $20, $6e, $44, $0c, $f1, $4c, $01
      .byte $aa, $35, $d9, $34, $ee, $20, $08, $b3, $37, $32
      .byte $43, $04, $4e, $21, $53, $20, $9c, $81, $e7, $42
      .byte $5f, $b3, $97, $63, $ac, $02, $c5, $41, $49, $e0
      .byte $58, $61, $76, $64, $85, $65, $94, $66, $a4, $22
      .byte $a6, $03, $c8, $22, $68, $f2, $96, $42, $13, $82
      .byte $17, $02, $af, $34, $f6, $21, $26, $80, $2a, $24
      .byte $36, $01, $8c, $00, $ff, $35, $4e, $a0, $55, $21
      .byte $77, $20, $87, $07, $89, $22, $ae, $21, $4c, $82
      .byte $9f, $34, $ec, $01, $03, $e7, $13, $67, $8d, $4a
      .byte $ad, $41, $0f, $a6, $fd

L_GroundArea16:
      .byte $10, $51, $4c, $01, $c7, $12, $c6, $42, $03, $92
      .byte $02, $42, $29, $12, $63, $12, $62, $42, $69, $14
      .byte $a5, $12, $a4, $42, $e2, $14, $e1, $44, $f8, $16
      .byte $37, $c1, $8f, $38, $02, $bb, $28, $7a, $68, $7a
      .byte $a8, $7a, $e0, $6a, $f0, $6a, $6d, $c5, $fd

L_GroundArea17:
      .byte $92, $31, $0f, $20, $6e, $40, $0d, $02, $37, $73
      .byte $ec, $00, $0c, $80, $3c, $00, $6c, $00, $9c, $00
      .byte $06, $c0, $c7, $73, $06, $83, $28, $72, $96, $40
      .byte $e7, $73, $26, $c0, $87, $7b, $d2, $41, $39, $f1
      .byte $c8, $f2, $97, $e3, $a3, $23, $e7, $02, $e3, $07
      .byte $f3, $22, $37, $e3, $9c, $00, $bc, $00, $ec, $00
      .byte $0c, $80, $3c, $00, $86, $21, $a6, $06, $b6, $24
      .byte $5c, $80, $7c, $00, $9c, $00, $29, $e1, $dc, $05
      .byte $f6, $41, $dc, $80, $e8, $72, $0c, $81, $27, $73
      .byte $4c, $01, $66, $74, $0d, $11, $3f, $35, $b6, $41
      .byte $2c, $82, $36, $40, $7c, $02, $86, $40, $f9, $61
      .byte $39, $e1, $ac, $04, $c6, $41, $0c, $83, $16, $41
      .byte $88, $f2, $39, $f1, $7c, $00, $89, $61, $9c, $00
      .byte $a7, $63, $bc, $00, $c5, $65, $dc, $00, $e3, $67
      .byte $f3, $67, $8d, $c1, $cf, $26, $fd

L_GroundArea18:
      .byte $55, $b1, $0f, $26, $cf, $33, $07, $b2, $15, $11
      .byte $52, $42, $99, $0b, $d3, $24, $d6, $42, $d7, $25
      .byte $23, $84, $cf, $33, $07, $e3, $19, $61, $78, $7a
      .byte $ef, $33, $2c, $81, $46, $64, $55, $65, $65, $65
      .byte $ec, $74, $47, $82, $53, $05, $63, $21, $62, $41
      .byte $96, $22, $9a, $41, $cc, $03, $b9, $91, $39, $f1
      .byte $63, $26, $67, $27, $d3, $06, $fc, $01, $18, $e2
      .byte $d9, $07, $e9, $04, $0c, $86, $37, $22, $93, $24
      .byte $87, $84, $ac, $02, $c2, $41, $c3, $23, $d9, $71
      .byte $fc, $01, $7f, $b1, $9c, $00, $a7, $63, $b6, $64
      .byte $cc, $00, $d4, $66, $e3, $67, $f3, $67, $8d, $c1
      .byte $cf, $26, $fd

L_GroundArea19:
      .byte $50, $b1, $0f, $26, $fc, $00, $1f, $b3, $5c, $00
      .byte $65, $65, $74, $66, $83, $67, $4c, $80, $b3, $20
      .byte $c9, $0b, $c3, $08, $d3, $2f, $88, $80, $d3, $2e
      .byte $ed, $4a, $d7, $a1, $ec, $01, $4c, $80, $59, $11
      .byte $d8, $11, $da, $10, $37, $a0, $47, $04, $99, $11
      .byte $e7, $21, $3a, $90, $67, $20, $76, $10, $77, $60
      .byte $87, $07, $d8, $12, $39, $f1, $ac, $00, $e9, $71
      .byte $0c, $80, $2c, $00, $4c, $05, $c7, $7b, $39, $f1
      .byte $ec, $00, $f9, $11, $0c, $82, $6f, $34, $f8, $11
      .byte $fa, $10, $7f, $b2, $ac, $00, $b6, $64, $cc, $01
      .byte $e3, $67, $f3, $67, $8d, $c1, $cf, $26, $fd

L_GroundArea20:
      .byte $52, $b1, $0f, $20, $6e, $45, $39, $91, $b3, $04
      .byte $c3, $21, $c8, $11, $ca, $10, $49, $91, $7c, $73
      .byte $e8, $12, $88, $91, $8a, $10, $e7, $21, $05, $91
      .byte $07, $30, $17, $07, $27, $20, $49, $11, $9c, $01
      .byte $c8, $72, $23, $a6, $27, $26, $d3, $03, $d8, $7a
      .byte $89, $91, $d8, $72, $39, $f1, $a9, $11, $09, $f1
      .byte $63, $24, $67, $24, $d8, $62, $28, $91, $2a, $10
      .byte $56, $21, $70, $04, $79, $0b, $8c, $00, $94, $21
      .byte $9f, $35, $2f, $b8, $3d, $c1, $7f, $26, $fd

L_GroundArea21:
      .byte $05, $c1, $4c, $00, $f4, $4f, $0d, $02, $06, $20
      .byte $24, $4f, $35, $a0, $36, $20, $53, $46, $d5, $20
      .byte $d6, $20, $34, $a1, $73, $49, $74, $20, $94, $20
      .byte $b4, $20, $d4, $20, $f4, $20, $2e, $80, $59, $42
      .byte $4d, $c7, $fd

L_GroundArea22:
      .byte $96, $31, $0f, $26, $0d, $03, $1a, $60, $c4, $00
      .byte $c8, $62, $b9, $e1, $d3, $06, $d7, $07, $f9, $61
      .byte $0c, $81, $4e, $b1, $8e, $b1, $bc, $01, $e4, $50
      .byte $e9, $61, $0c, $81, $0d, $0a, $84, $43, $98, $72
      .byte $0d, $0c, $0f, $38, $1d, $c1, $5f, $26, $fd

L_UndergroundArea1:
      .byte $4b, $0f, $0e, $21, $60, $22, $67, $53, $77, $04
      .byte $87, $27, $8e, $0a, $07, $a2, $2e, $0a, $5e, $21
      .byte $60, $57, $99, $71, $a6, $25, $b5, $50, $c5, $07
      .byte $d5, $05, $e5, $51, $f9, $71, $2e, $90, $36, $34
      .byte $7e, $1e, $ca, $71, $1a, $f1, $5e, $22, $6a, $70
      .byte $64, $23, $a4, $04, $a5, $52, $a7, $23, $c6, $40
      .byte $c3, $02, $de, $15, $ee, $00, $7e, $91, $a7, $25
      .byte $a1, $40, $b4, $22, $c3, $04, $e0, $40, $1e, $90
      .byte $47, $55, $5c, $24, $a7, $55, $07, $d5, $1c, $24
      .byte $67, $55, $aa, $54, $ba, $73, $da, $52, $e3, $33
      .byte $f1, $79, $2a, $d2, $25, $02, $3a, $73, $36, $02
      .byte $5a, $54, $7e, $21, $8b, $62, $d7, $21, $d7, $52
      .byte $e4, $22, $fc, $60, $f7, $06, $07, $a1, $17, $52
      .byte $5f, $33, $97, $63, $9e, $00, $f7, $32, $77, $b2
      .byte $ee, $01, $f8, $64, $fe, $21, $9f, $ca, $ce, $0f
      .byte $0d, $c7, $ae, $01, $c7, $14, $ed, $42, $3d, $c6
      .byte $fe, $0f, $fd

L_UndergroundArea2:
      .byte $48, $0f, $0e, $01, $5e, $31, $6c, $04, $b7, $40
      .byte $f8, $40, $0c, $84, $4e, $34, $6e, $24, $67, $40
      .byte $85, $40, $8c, $70, $a6, $40, $b8, $02, $c7, $02
      .byte $d6, $20, $e6, $07, $f6, $07, $fe, $0f, $ff, $45
      .byte $ee, $80, $fe, $80, $4e, $82, $66, $02, $6e, $01
      .byte $73, $05, $75, $02, $86, $02, $88, $72, $a5, $02
      .byte $a3, $21, $c7, $20, $d7, $07, $e4, $76, $07, $a0
      .byte $17, $06, $48, $7a, $76, $20, $98, $72, $79, $e1
      .byte $88, $62, $9c, $01, $b7, $73, $dc, $01, $f8, $62
      .byte $fe, $01, $08, $e2, $0e, $00, $6e, $02, $73, $20
      .byte $77, $23, $83, $04, $93, $20, $ae, $00, $fe, $0a
      .byte $0e, $82, $39, $71, $a8, $72, $e7, $73, $0c, $81
      .byte $8f, $32, $ae, $00, $fe, $04, $04, $d1, $17, $04
      .byte $26, $49, $27, $29, $df, $33, $fe, $02, $44, $f6
      .byte $7c, $01, $8e, $06, $8f, $47, $ee, $0f, $4d, $c7
      .byte $0e, $82, $68, $7a, $ae, $01, $de, $0f, $6d, $c5
      .byte $fd

L_UndergroundArea3:
      .byte $48, $01, $0e, $01, $00, $5a, $3e, $06, $45, $46
      .byte $47, $46, $53, $44, $ae, $01, $df, $4a, $4d, $c7
      .byte $0e, $81, $00, $5a, $2e, $04, $37, $28, $3a, $48
      .byte $46, $47, $c7, $07, $ce, $0f, $df, $4a, $4d, $c7
      .byte $0e, $81, $00, $5a, $33, $53, $43, $51, $46, $40
      .byte $47, $50, $53, $04, $55, $40, $56, $50, $62, $43
      .byte $64, $40, $65, $50, $71, $41, $73, $51, $83, $51
      .byte $94, $40, $95, $50, $a3, $50, $a5, $40, $a6, $50
      .byte $b3, $51, $b6, $40, $b7, $50, $c3, $53, $df, $4a
      .byte $4d, $c7, $0e, $81, $00, $5a, $2e, $02, $36, $47
      .byte $37, $52, $3a, $49, $47, $25, $a7, $52, $d7, $04
      .byte $df, $4a, $4d, $c7, $0e, $81, $00, $5a, $3e, $02
      .byte $44, $51, $53, $44, $54, $44, $55, $24, $a1, $54
      .byte $ae, $01, $b4, $21, $df, $4a, $e5, $07, $4d, $c7
      .byte $fd

L_CastleArea1:
      .byte $9b, $07, $05, $32, $06, $33, $07, $34, $ce, $03
      .byte $dc, $51, $ee, $07, $73, $e0, $74, $0a, $7e, $06
      .byte $9e, $0a, $ce, $06, $e4, $00, $e8, $0a, $fe, $0a
      .byte $2e, $89, $4e, $0b, $54, $0a, $14, $8a, $c4, $0a
      .byte $34, $8a, $7e, $06, $c7, $0a, $01, $e0, $02, $0a
      .byte $47, $0a, $81, $60, $82, $0a, $c7, $0a, $0e, $87
      .byte $7e, $02, $a7, $02, $b3, $02, $d7, $02, $e3, $02
      .byte $07, $82, $13, $02, $3e, $06, $7e, $02, $ae, $07
      .byte $fe, $0a, $0d, $c4, $cd, $43, $ce, $09, $de, $0b
      .byte $dd, $42, $fe, $02, $5d, $c7, $fd

L_CastleArea2:
      .byte $5b, $07, $05, $32, $06, $33, $07, $34, $5e, $0a
      .byte $68, $64, $98, $64, $a8, $64, $ce, $06, $fe, $02
      .byte $0d, $01, $1e, $0e, $7e, $02, $94, $63, $b4, $63
      .byte $d4, $63, $f4, $63, $14, $e3, $2e, $0e, $5e, $02
      .byte $64, $35, $88, $72, $be, $0e, $0d, $04, $ae, $02
      .byte $ce, $08, $cd, $4b, $fe, $02, $0d, $05, $68, $31
      .byte $7e, $0a, $96, $31, $a9, $63, $a8, $33, $d5, $30
      .byte $ee, $02, $e6, $62, $f4, $61, $04, $b1, $08, $3f
      .byte $44, $33, $94, $63, $a4, $31, $e4, $31, $04, $bf
      .byte $08, $3f, $04, $bf, $08, $3f, $cd, $4b, $03, $e4
      .byte $0e, $03, $2e, $01, $7e, $06, $be, $02, $de, $06
      .byte $fe, $0a, $0d, $c4, $cd, $43, $ce, $09, $de, $0b
      .byte $dd, $42, $fe, $02, $5d, $c7, $fd

L_CastleArea3:
      .byte $9b, $07, $05, $32, $06, $33, $07, $34, $fe, $00
      .byte $27, $b1, $65, $32, $75, $0a, $71, $00, $b7, $31
      .byte $08, $e4, $18, $64, $1e, $04, $57, $3b, $bb, $0a
      .byte $17, $8a, $27, $3a, $73, $0a, $7b, $0a, $d7, $0a
      .byte $e7, $3a, $3b, $8a, $97, $0a, $fe, $08, $24, $8a
      .byte $2e, $00, $3e, $40, $38, $64, $6f, $00, $9f, $00
      .byte $be, $43, $c8, $0a, $c9, $63, $ce, $07, $fe, $07
      .byte $2e, $81, $66, $42, $6a, $42, $79, $0a, $be, $00
      .byte $c8, $64, $f8, $64, $08, $e4, $2e, $07, $7e, $03
      .byte $9e, $07, $be, $03, $de, $07, $fe, $0a, $03, $a5
      .byte $0d, $44, $cd, $43, $ce, $09, $dd, $42, $de, $0b
      .byte $fe, $02, $5d, $c7, $fd

L_CastleArea4:
      .byte $9b, $07, $05, $32, $06, $33, $07, $34, $fe, $06
      .byte $0c, $81, $39, $0a, $5c, $01, $89, $0a, $ac, $01
      .byte $d9, $0a, $fc, $01, $2e, $83, $a7, $01, $b7, $00
      .byte $c7, $01, $de, $0a, $fe, $02, $4e, $83, $5a, $32
      .byte $63, $0a, $69, $0a, $7e, $02, $ee, $03, $fa, $32
      .byte $03, $8a, $09, $0a, $1e, $02, $ee, $03, $fa, $32
      .byte $03, $8a, $09, $0a, $14, $42, $1e, $02, $7e, $0a
      .byte $9e, $07, $fe, $0a, $2e, $86, $5e, $0a, $8e, $06
      .byte $be, $0a, $ee, $07, $3e, $83, $5e, $07, $fe, $0a
      .byte $0d, $c4, $41, $52, $51, $52, $cd, $43, $ce, $09
      .byte $de, $0b, $dd, $42, $fe, $02, $5d, $c7, $fd

L_CastleArea5:
      .byte $5b, $07, $05, $32, $06, $33, $07, $34, $fe, $0a
      .byte $ae, $86, $be, $07, $fe, $02, $0d, $02, $27, $32
      .byte $5e, $0e, $1e, $82, $68, $3c, $74, $3a, $7d, $4b
      .byte $5e, $8e, $7d, $4b, $7e, $82, $84, $62, $94, $61
      .byte $a4, $31, $bd, $4b, $ce, $06, $fe, $02, $0d, $06
      .byte $34, $31, $3e, $0a, $64, $32, $75, $0a, $7b, $61
      .byte $a4, $33, $ae, $02, $de, $0e, $3e, $82, $64, $32
      .byte $78, $32, $b4, $36, $c8, $36, $dd, $4b, $44, $b2
      .byte $58, $32, $94, $63, $a4, $3e, $ba, $30, $c9, $61
      .byte $ce, $06, $dd, $4b, $ce, $86, $dd, $4b, $fe, $02
      .byte $2e, $86, $5e, $02, $7e, $06, $fe, $02, $1e, $86
      .byte $3e, $02, $5e, $06, $7e, $02, $9e, $06, $fe, $0a
      .byte $0d, $c4, $cd, $43, $ce, $09, $de, $0b, $dd, $42
      .byte $fe, $02, $5d, $c7, $fd

L_CastleArea6:
      .byte $5b, $06, $05, $32, $06, $33, $07, $34, $5e, $0a
      .byte $ae, $02, $0d, $01, $39, $73, $0d, $03, $39, $7b
      .byte $4d, $4b, $de, $06, $1e, $8a, $ae, $06, $c4, $33
      .byte $16, $fe, $a5, $77, $fe, $02, $fe, $82, $0d, $07
      .byte $39, $73, $a8, $74, $ed, $4b, $49, $fb, $e8, $74
      .byte $fe, $0a, $2e, $82, $67, $02, $84, $7a, $87, $31
      .byte $0d, $0b, $fe, $02, $0d, $0c, $39, $73, $5e, $06
      .byte $c6, $76, $45, $ff, $be, $0a, $dd, $48, $fe, $06
      .byte $3d, $cb, $46, $7e, $ad, $4a, $fe, $82, $39, $f3
      .byte $a9, $7b, $4e, $8a, $9e, $07, $fe, $0a, $0d, $c4
      .byte $cd, $43, $ce, $09, $de, $0b, $dd, $42, $fe, $02
      .byte $5d, $c7, $fd


