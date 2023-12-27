;GAME LEVELS DATA

WorldAddrOffsets:
      .db World1Areas-AreaAddrOffsets, World2Areas-AreaAddrOffsets
      .db World3Areas-AreaAddrOffsets, World4Areas-AreaAddrOffsets
      .db World5Areas-AreaAddrOffsets, World6Areas-AreaAddrOffsets
      .db World7Areas-AreaAddrOffsets, World8Areas-AreaAddrOffsets

AreaAddrOffsets:
World1Areas: .db $25, $29, $c0, $26, $28, $2f, $60
World2Areas: .db $01, $27, $62, $24, $34, $20, $63
World3Areas: .db $24, $34, $20, $63, $22, $29, $41
World4Areas: .db $22, $29, $41, $2b, $61, $2a, $30
World5Areas: .db $2a, $30, $26, $62, $2d, $23, $2c
World6Areas: .db $2d, $23, $2c, $60, $32, $29, $01
World7Areas: .db $32, $29, $01, $27, $64, $2f, $31
World8Areas: .db $2f, $31, $21, $65, $1f, $06, $1c

EnemyAddrHOffsets:
      .db EnemyDataAddrLow_WaterStart - EnemyDataAddrLow          ; Water
      .db EnemyDataAddrLow_GroundStart - EnemyDataAddrLow         ; Ground
      .db EnemyDataAddrLow_UndergroundStart - EnemyDataAddrLow    ; Underground
      .db EnemyDataAddrLow_CastleStart - EnemyDataAddrLow         ; castle

EnemyDataAddrLow:
      ; Water
      EnemyDataAddrLow_WaterStart:
      .db <E_WaterArea1, <E_WaterArea2, <E_WaterArea3
      ; Ground
      EnemyDataAddrLow_GroundStart:
      .db <E_GroundArea1, <E_GroundArea2, <E_GroundArea3, <E_GroundArea4, <E_GroundArea5, <E_GroundArea6
      .db <E_GroundArea7, <E_GroundArea8, <E_GroundArea9, <E_GroundArea10, <E_GroundArea11, <E_GroundArea12
      .db <E_GroundArea13, <E_GroundArea14, <E_GroundArea15, <E_GroundArea16, <E_GroundArea17, <E_GroundArea18
      .db <E_GroundArea19, <E_GroundArea20, <E_GroundArea21
      ; Underground
      EnemyDataAddrLow_UndergroundStart:
      .db <E_UndergroundArea1, <E_UndergroundArea2, <E_UndergroundArea3
      ; Castle
      EnemyDataAddrLow_CastleStart:
      .db <E_CastleArea1, <E_CastleArea2, <E_CastleArea3, <E_CastleArea4, <E_CastleArea5, <E_CastleArea6

EnemyDataAddrHigh:
      ; Water
      .db >E_WaterArea1, >E_WaterArea2, >E_WaterArea3
      ; Ground
      .db >E_GroundArea1, >E_GroundArea2, >E_GroundArea3, >E_GroundArea4, >E_GroundArea5, >E_GroundArea6
      .db >E_GroundArea7, >E_GroundArea8, >E_GroundArea9, >E_GroundArea10, >E_GroundArea11, >E_GroundArea12
      .db >E_GroundArea13, >E_GroundArea14, >E_GroundArea15, >E_GroundArea16, >E_GroundArea17, >E_GroundArea18
      .db >E_GroundArea19, >E_GroundArea20, >E_GroundArea21
      ; Underground
      .db >E_UndergroundArea1, >E_UndergroundArea2, >E_UndergroundArea3
      ; Castle
      .db >E_CastleArea1, >E_CastleArea2, >E_CastleArea3, >E_CastleArea4, >E_CastleArea5, >E_CastleArea6

AreaDataHOffsets:
      .db AreaDataAddrLow_WaterStart - AreaDataAddrLow          ; Water
      .db AreaDataAddrLow_GroundStart - AreaDataAddrLow         ; Ground
      .db AreaDataAddrLow_UndergroundStart - AreaDataAddrLow    ; Underground
      .db AreaDataAddrLow_CastleStart - AreaDataAddrLow         ; castle

AreaDataAddrLow:
      ; Water
      AreaDataAddrLow_WaterStart:
      .db <L_WaterArea1, <L_WaterArea2, <L_WaterArea3
      ; Ground
      AreaDataAddrLow_GroundStart:
      .db <L_GroundArea1, <L_GroundArea2, <L_GroundArea3, <L_GroundArea4, <L_GroundArea5, <L_GroundArea6
      .db <L_GroundArea7, <L_GroundArea8, <L_GroundArea9, <L_GroundArea10, <L_GroundArea11, <L_GroundArea12
      .db <L_GroundArea13, <L_GroundArea14, <L_GroundArea15, <L_GroundArea16, <L_GroundArea17, <L_GroundArea18
      .db <L_GroundArea19, <L_GroundArea20, <L_GroundArea21
      ; Underground
      AreaDataAddrLow_UndergroundStart:
      .db <L_UndergroundArea1, <L_UndergroundArea2, <L_UndergroundArea3
      ; Castle
      AreaDataAddrLow_CastleStart:
      .db <L_CastleArea1, <L_CastleArea2, <L_CastleArea3, <L_CastleArea4, <L_CastleArea5, <L_CastleArea6

AreaDataAddrHigh:
      ; Water
      .db >L_WaterArea1, >L_WaterArea2, >L_WaterArea3
      ; Ground
      .db >L_GroundArea1, >L_GroundArea2, >L_GroundArea3, >L_GroundArea4, >L_GroundArea5, >L_GroundArea6
      .db >L_GroundArea7, >L_GroundArea8, >L_GroundArea9, >L_GroundArea10, >L_GroundArea11, >L_GroundArea12
      .db >L_GroundArea13, >L_GroundArea14, >L_GroundArea15, >L_GroundArea16, >L_GroundArea17, >L_GroundArea18
      .db >L_GroundArea19, >L_GroundArea20, >L_GroundArea21
      ; Underground
      .db >L_UndergroundArea1, >L_UndergroundArea2, >L_UndergroundArea3
      ; Castle
      .db >L_CastleArea1, >L_CastleArea2, >L_CastleArea3, >L_CastleArea4, >L_CastleArea5, >L_CastleArea6

E_WaterArea1:
      .db $3b, $87, $66, $27, $96, $15, $cc, $27, $ee, $30
      .db $87, $ee, $23, $a7, $f5, $1d, $3b, $87, $5a, $1d
      .db $75, $5d, $88, $5b, $96, $1d, $db, $07, $ff

E_WaterArea2:
      .db $0f, $01, $2e, $25, $2b, $2e, $25, $4b, $4e, $25
      .db $cb, $b3, $03, $7b, $c6, $e9, $4e, $47, $ce, $7b
      .db $02, $78, $8a, $d6, $4a, $38, $8a, $a9, $4b, $e3
      .db $0b, $9b, $8b, $0f, $09, $68, $4b, $db, $c7, $3b
      .db $c7, $ff

E_WaterArea3:
      .db $47, $9b, $63, $00, $76, $46, $cb, $07, $de, $42
      .db $00, $fa, $1d, $86, $9b, $3a, $87, $56, $07, $88
      .db $1b, $07, $9d, $2e, $65, $f0, $ff

E_GroundArea1:
      .db $a5, $86, $e4, $28, $18, $a8, $45, $83, $69, $03
      .db $c6, $29, $9b, $83, $16, $a4, $88, $24, $e9, $28
      .db $05, $a8, $7b, $28, $24, $8f, $c8, $03, $e8, $03
      .db $46, $a8, $85, $24, $c8, $24, $ff

E_GroundArea2:
      .db $eb, $8e, $0f, $03, $17, $05, $aa, $05, $db, $8e
      .db $0f, $07, $57, $05, $9b, $80, $2b, $85, $fb, $05
      .db $0f, $0b, $1b, $05, $9b, $05, $ff

E_GroundArea3:
      .db $2e, $c2, $66, $e2, $11, $0f, $07, $02, $11, $0f
      .db $0c, $12, $11, $ff

E_GroundArea4:
      .db $0e, $c2, $a8, $bb, $8e, $6b, $82, $de, $00, $a0
      .db $33, $86, $43, $06, $3e, $b3, $a0, $0f, $07, $7e
      .db $42, $a6, $83, $02, $0f, $0a, $3b, $02, $cb, $37
      .db $0f, $0c, $e3, $0e, $ff

E_GroundArea5:
      .db $9b, $8e, $ca, $0e, $ee, $42, $44, $5b, $86, $80
      .db $b8, $1b, $80, $50, $ba, $10, $b7, $5b, $00, $17
      .db $85, $4b, $05, $fe, $33, $40, $b7, $86, $c6, $06
      .db $5b, $80, $83, $00, $d0, $38, $5b, $8e, $8a, $0e
      .db $a6, $00, $bb, $0e, $c5, $80, $f3, $00, $ff

E_GroundArea6:
      .db $1e, $c2, $00, $aa, $39, $47, $86, $42, $bd, $7d
      .db $25, $f3, $03, $96, $86, $19, $86, $17, $86, $5a
      .db $24, $9d, $24, $5a, $bb, $76, $b7, $f7, $03, $d8
      .db $90, $0d, $a8, $4a, $b7, $86, $06, $fe, $2e, $00
      .db $6d, $ea, $ff

E_GroundArea7:
      .db $7b, $a8, $37, $ce, $59, $a4, $8d, $24, $81, $03
      .db $b5, $85, $2b, $d7, $35, $06, $9a, $83, $3e, $c2
      .db $06, $c5, $0f, $64, $a8, $b4, $68, $8b, $90, $db
      .db $85, $f2, $ce, $ff

E_GroundArea8:
      .db $0f, $02, $78, $40, $48, $ce, $f8, $c3, $f8, $c3
      .db $0f, $07, $7b, $43, $c6, $d0, $0f, $8a, $c8, $50
      .db $ff

E_GroundArea9:
      .db $05, $86, $ea, $03, $47, $80, $5b, $86, $8a, $50
      .db $78, $c6, $49, $c6, $4e, $c2, $08, $58, $02, $4b
      .db $c6, $9b, $82, $7b, $ce, $06, $94, $c8, $0e, $75
      .db $ce, $62, $94, $f2, $40, $ff

E_GroundArea10:
      .db $ff

E_GroundArea11:
      .db $0b, $80, $60, $38, $10, $b8, $c0, $3b, $db, $8e
      .db $40, $b8, $f0, $38, $7b, $8e, $a0, $b8, $c0, $b8
      .db $fb, $00, $a0, $b8, $30, $bb, $ee, $42, $88, $0f
      .db $0b, $2b, $0e, $67, $0e, $ff

E_GroundArea12:
      .db $c7, $83, $d7, $03, $42, $8f, $7a, $03, $05, $a4
      .db $78, $24, $a6, $25, $e4, $25, $4b, $83, $e3, $03
      .db $05, $a4, $89, $24, $b5, $24, $09, $a4, $65, $24
      .db $c9, $24, $0f, $08, $85, $25, $ff

E_GroundArea13:
      .db $cd, $a5, $b5, $a8, $07, $a8, $76, $28, $cc, $25
      .db $65, $a4, $a9, $24, $e5, $24, $19, $a4, $0f, $07
      .db $95, $28, $e6, $24, $19, $a4, $d7, $29, $16, $a9
      .db $58, $29, $97, $29, $ff

E_GroundArea14:
      .db $0f, $02, $a3, $11, $0f, $07, $02, $11, $ff

E_GroundArea15:
      .db $ff

E_GroundArea16:
      .db $8b, $b8, $1b, $82, $fb, $b8, $7b, $80, $fb, $3c
      .db $5b, $bc, $7b, $b8, $1b, $8e, $cb, $0e, $1b, $8e
      .db $0f, $0d, $2b, $3b, $3a, $37, $4b, $38, $bb, $38
      .db $bb, $38, $bb, $02, $eb, $02, $0f, $13, $1b, $00
      .db $cb, $80, $6b, $bc, $ff

E_GroundArea17:
      .db $7b, $80, $ae, $00, $80, $8b, $8e, $e8, $05, $f9
      .db $86, $17, $86, $16, $85, $4e, $35, $80, $ab, $8e
      .db $87, $85, $c3, $05, $8b, $82, $9b, $02, $ab, $02
      .db $bb, $86, $cb, $06, $d3, $03, $3b, $8e, $6b, $0e
      .db $a7, $8e, $ff

E_GroundArea18:
      .db $29, $8e, $52, $11, $83, $0e, $0f, $03, $9b, $0e
      .db $2b, $8e, $5b, $0e, $cb, $8e, $fb, $0e, $fb, $82
      .db $9b, $82, $bb, $02, $fe, $42, $e8, $bb, $8e, $0f
      .db $0a, $ab, $0e, $cb, $0e, $f9, $0e, $88, $86, $a6
      .db $06, $db, $02, $b6, $8e, $ff

E_GroundArea19:
      .db $ab, $ce, $de, $42, $c0, $cb, $ce, $5b, $8e, $1b
      .db $ce, $4b, $85, $67, $45, $0f, $07, $2b, $00, $e9
      .db $68, $7b, $85, $97, $05, $0f, $0a, $92, $02, $ff

E_GroundArea20:
      .db $0a, $aa, $0e, $40, $02, $1e, $23, $aa, $ff

E_GroundArea21:
      .db $1b, $80, $4b, $bc, $0f, $04, $2b, $00, $ab, $38
      .db $eb, $00, $cb, $8e, $fb, $80, $ab, $b8, $6b, $80
      .db $fb, $3c, $9b, $bb, $5b, $bc, $fb, $00, $6b, $b8
      .db $fb, $38, $ff

E_UndergroundArea1:
      .db $37, $90, $39, $29, $be, $33, $00, $d5, $03, $f9
      .db $46, $5d, $a8, $2b, $c6, $54, $0e, $2d, $a8, $c3
      .db $03, $e7, $06, $87, $80, $4e, $c2, $02, $5a, $06
      .db $e7, $c5, $07, $85, $78, $00, $b9, $29, $b6, $14
      .db $39, $a9, $b9, $29, $1e, $a3, $0c, $a9, $02, $ff

E_UndergroundArea2:
      .db $0f, $02, $0e, $26, $07, $c6, $e7, $2b, $82, $ab
      .db $38, $db, $00, $3b, $82, $8b, $02, $1a, $b8, $3b
      .db $00, $70, $3b, $eb, $3b, $25, $a7, $2c, $27, $b2
      .db $26, $b9, $26, $9b, $80, $a8, $82, $b5, $27, $bc
      .db $27, $b0, $bb, $87, $b4, $ff

E_UndergroundArea3:
      .db $1e, $a5, $0a, $2e, $28, $27, $2e, $32, $c7, $0f
      .db $03, $1e, $40, $07, $2e, $2f, $e7, $0f, $05, $1e
      .db $24, $44, $0f, $07, $1e, $26, $07, $2e, $23, $ab
      .db $0f, $09, $1e, $28, $05, $1e, $2a, $8a, $2e, $23
      .db $a2, $2e, $31, $ea, $ff

E_CastleArea1:
      .db $d9, $8c, $ba, $8c, $66, $cc, $78, $82, $68, $9d
      .db $d7, $dc, $24, $c2, $f6, $1c, $ea, $90, $94, $c2
      .db $58, $dc, $05, $c5, $59, $05, $43, $dd, $19, $85
      .db $68, $ad, $90, $b5, $ff

E_CastleArea2:
      .db $0f, $03, $56, $1b, $c9, $1b, $0f, $07, $36, $1b
      .db $aa, $1b, $48, $95, $0f, $0a, $2a, $1b, $5b, $0c
      .db $78, $2d, $90, $b5, $ff

E_CastleArea3:
      .db $0b, $8c, $4b, $4c, $77, $5f, $bd, $db, $7d, $db
      .db $3d, $dd, $26, $9d, $57, $2b, $8a, $2c, $a8, $1b
      .db $20, $95, $7b, $5c, $db, $4c, $1b, $cc, $3b, $cc
      .db $78, $2d, $a6, $28, $90, $b5, $ff

E_CastleArea4:
      .db $0b, $8c, $3b, $1d, $8b, $1d, $ab, $0c, $db, $1d
      .db $0f, $03, $65, $1d, $6b, $1b, $05, $9d, $0b, $1b
      .db $05, $9b, $0b, $1d, $8b, $0c, $1b, $8c, $70, $15
      .db $7b, $0c, $db, $0c, $0f, $08, $78, $2d, $a6, $28
      .db $90, $b5, $ff

E_CastleArea5:
      .db $27, $a9, $4b, $0c, $68, $29, $0f, $06, $77, $1b
      .db $0f, $0b, $60, $15, $4b, $8c, $78, $2d, $90, $b5
      .db $ff

E_CastleArea6:
      .db $0f, $03, $8e, $65, $e1, $bb, $38, $6d, $a8, $3e
      .db $e5, $e7, $0f, $08, $0b, $02, $2b, $02, $5e, $65
      .db $e1, $bb, $0e, $db, $0e, $bb, $8e, $db, $0e, $fe
      .db $65, $ec, $0f, $0d, $4e, $65, $e1, $0f, $0e, $4e
      .db $02, $e0, $0f, $10, $fe, $e5, $e1, $1b, $85, $7b
      .db $0c, $5b, $95, $78, $2d, $90, $b5, $ff

L_WaterArea1:
      .db $41, $01, $b4, $34, $c8, $52, $f2, $51, $18, $8a
      .db $47, $53, $43, $0a, $6c, $03, $65, $49, $73, $03
      .db $98, $40, $9e, $07, $be, $01, $c8, $40, $c7, $40
      .db $cc, $03, $fe, $07, $0d, $c9, $1e, $01, $6c, $01
      .db $62, $35, $63, $53, $80, $30, $8a, $41, $ac, $01
      .db $b3, $53, $d8, $0a, $e9, $51, $26, $c3, $27, $33
      .db $63, $43, $64, $33, $ba, $60, $c9, $61, $ce, $0b
      .db $e5, $09, $ee, $0f, $7d, $ca, $7d, $47, $fd

L_WaterArea2:
      .db $43, $01, $0f, $23, $4e, $42, $9f, $21, $ee, $3a
      .db $ee, $41, $33, $d4, $47, $20, $45, $50, $43, $50
      .db $63, $54, $73, $50, $83, $54, $a4, $53, $a3, $21
      .db $b7, $20, $c4, $52, $6d, $c6, $b6, $51, $c5, $63
      .db $c4, $40, $0b, $8d, $7f, $22, $c3, $67, $c0, $62
      .db $dc, $06, $d0, $6f, $e0, $6c, $f0, $6c, $00, $ec
      .db $88, $52, $c3, $34, $c7, $34, $d1, $51, $43, $b3
      .db $47, $33, $9a, $30, $a9, $61, $b8, $62, $be, $0b
      .db $d5, $09, $de, $0f, $0d, $ca, $7d, $47, $fd

L_WaterArea3:
      .db $49, $0f, $1e, $01, $39, $73, $5e, $07, $ae, $0b
      .db $1e, $82, $aa, $71, $6e, $88, $9e, $02, $0d, $04
      .db $2e, $0b, $45, $09, $4e, $0f, $ed, $47, $fd

L_GroundArea1:
      .db $94, $11, $0f, $26, $fe, $10, $28, $94, $65, $15
      .db $eb, $12, $4a, $96, $b7, $13, $e9, $19, $f5, $15
      .db $11, $80, $71, $13, $15, $92, $1b, $1f, $24, $40
      .db $55, $12, $64, $40, $95, $12, $a4, $40, $d2, $12
      .db $e1, $40, $13, $c0, $2c, $17, $2f, $12, $49, $13
      .db $83, $40, $9f, $14, $a3, $40, $17, $92, $83, $13
      .db $92, $41, $b9, $14, $c5, $12, $c8, $40, $d4, $40
      .db $4b, $92, $78, $1b, $9c, $94, $9f, $11, $df, $14
      .db $fe, $11, $7d, $c1, $9e, $42, $cf, $20, $fd

L_GroundArea2:
      .db $90, $b1, $0f, $26, $7e, $c2, $2b, $c0, $4e, $c0
      .db $0d, $0a, $5e, $40, $88, $72, $be, $42, $e7, $87
      .db $2e, $c0, $65, $41, $be, $42, $ef, $20, $fe, $31
      .db $fd

L_GroundArea3:
      .db $52, $21, $0f, $20, $6e, $40, $58, $f2, $93, $01
      .db $97, $00, $0c, $81, $97, $40, $a6, $41, $c7, $40
      .db $0d, $04, $03, $01, $07, $01, $23, $01, $27, $01
      .db $ec, $03, $ac, $f3, $c3, $03, $78, $e2, $94, $43
      .db $47, $f3, $74, $43, $47, $fb, $74, $43, $2c, $f1
      .db $4c, $63, $47, $00, $57, $21, $5c, $01, $7c, $72
      .db $39, $f1, $ec, $02, $4c, $81, $d8, $62, $ec, $01
      .db $0d, $0d, $0f, $38, $c7, $07, $ed, $4a, $1d, $c1
      .db $5f, $26, $fd

L_GroundArea4:
      .db $50, $31, $0f, $26, $a7, $22, $37, $fb, $73, $20
      .db $83, $07, $93, $20, $c7, $73, $06, $b1, $37, $a0
      .db $47, $04, $e5, $71, $e7, $31, $33, $a4, $39, $71
      .db $a9, $71, $d3, $23, $08, $f2, $13, $05, $27, $02
      .db $49, $71, $75, $75, $e8, $72, $67, $f3, $99, $71
      .db $e7, $20, $f4, $72, $f7, $31, $17, $a0, $33, $20
      .db $39, $71, $73, $28, $bc, $05, $39, $f1, $79, $71
      .db $a6, $21, $c3, $06, $d3, $20, $dc, $00, $fc, $00
      .db $07, $a2, $13, $21, $5f, $32, $8c, $00, $98, $7a
      .db $c7, $63, $d9, $61, $03, $a2, $07, $22, $74, $72
      .db $77, $31, $39, $f1, $58, $72, $d8, $72, $00, $ea
      .db $10, $3a, $39, $73, $7f, $37, $e3, $67, $f3, $67
      .db $7d, $c1, $cf, $26, $c8, $81, $de, $20, $f0, $6c
      .db $fd

L_GroundArea5:
      .db $52, $31, $0f, $20, $6e, $66, $07, $81, $36, $01
      .db $66, $00, $a7, $22, $08, $f2, $67, $7b, $dc, $02
      .db $98, $f2, $d7, $20, $39, $f1, $9f, $33, $dc, $27
      .db $dc, $57, $23, $83, $57, $63, $6c, $51, $87, $63
      .db $99, $61, $a3, $06, $b3, $21, $77, $f3, $f3, $21
      .db $f7, $2a, $13, $81, $23, $22, $53, $00, $63, $22
      .db $e9, $0b, $0c, $83, $13, $21, $16, $22, $33, $05
      .db $8f, $35, $ec, $01, $63, $a0, $67, $20, $73, $01
      .db $77, $01, $83, $20, $87, $20, $b3, $20, $b7, $20
      .db $c3, $01, $c7, $00, $d3, $20, $d7, $20, $67, $a0
      .db $77, $07, $87, $22, $e8, $62, $f5, $65, $1c, $82
      .db $7f, $38, $8d, $c1, $cf, $26, $fd

L_GroundArea6:
      .db $53, $21, $4f, $b3, $7e, $10, $87, $3f, $ac, $1a
      .db $7e, $a1, $87, $63, $98, $62, $a9, $61, $ba, $30
      .db $17, $a4, $37, $01, $6e, $10, $bb, $17, $c3, $18
      .db $0b, $95, $69, $1c, $76, $19, $47, $91, $77, $11
      .db $a7, $11, $e7, $16, $13, $83, $80, $65, $c0, $62
      .db $c5, $67, $ce, $31, $0c, $a2, $15, $40, $3e, $31
      .db $47, $02, $5e, $22, $60, $62, $66, $64, $f8, $31
      .db $f6, $71, $17, $82, $27, $02, $37, $31, $34, $7a
      .db $39, $41, $57, $02, $67, $02, $76, $71, $78, $31
      .db $e8, $07, $f7, $63, $08, $a0, $7e, $10, $70, $67
      .db $84, $32, $ab, $12, $b9, $0b, $b4, $50, $c4, $32
      .db $f4, $12, $2e, $a1, $39, $73, $7c, $1b, $a8, $28
      .db $e8, $01, $67, $f3, $86, $74, $a5, $75, $c4, $76
      .db $e3, $77, $8d, $c1, $ef, $26, $ea, $c1, $09, $89
      .db $0e, $2f, $8e, $40, $fd

L_GroundArea7:
      .db $94, $61, $0f, $26, $c8, $24, $d7, $22, $e6, $06
      .db $28, $82, $3e, $10, $46, $66, $66, $22, $76, $04
      .db $a6, $6f, $bc, $2f, $dc, $1a, $b6, $e6, $fe, $11
      .db $0f, $b4, $4e, $30, $81, $53, $94, $22, $be, $21
      .db $b5, $57, $17, $94, $35, $40, $75, $16, $15, $94
      .db $74, $12, $9c, $01, $ae, $31, $cc, $01, $c7, $40
      .db $d1, $12, $fe, $21, $24, $9a, $5a, $35, $2e, $b1
      .db $39, $73, $66, $40, $87, $7b, $a7, $02, $b4, $03
      .db $b7, $02, $d1, $69, $ec, $01, $fe, $16, $00, $cf
      .db $45, $76, $a5, $76, $fe, $21, $6c, $83, $27, $92
      .db $4e, $31, $77, $40, $7e, $21, $98, $40, $95, $12
      .db $c6, $03, $d7, $02, $ee, $31, $f4, $76, $3c, $83
      .db $98, $40, $9f, $33, $a6, $40, $dc, $00, $e4, $66
      .db $f3, $67, $03, $e7, $9d, $41, $ce, $42, $ff, $23
      .db $ee, $bf, $fd

L_GroundArea8:
      .db $90, $11, $0f, $26, $6e, $10, $8b, $17, $af, $32
      .db $d8, $62, $e8, $62, $fc, $3f, $ad, $c8, $0c, $be
      .db $f8, $64, $0c, $bf, $f8, $64, $48, $e4, $5c, $39
      .db $f8, $64, $48, $e4, $5c, $39, $f8, $64, $13, $c2
      .db $37, $65, $4c, $24, $63, $00, $97, $65, $c3, $42
      .db $0b, $97, $ac, $32, $f8, $64, $0c, $be, $53, $45
      .db $9d, $48, $f8, $64, $2a, $e2, $3c, $47, $56, $43
      .db $ba, $62, $f8, $64, $0c, $b7, $88, $64, $bc, $31
      .db $d4, $45, $fc, $31, $3c, $b1, $78, $64, $8c, $38
      .db $0b, $9c, $1a, $33, $18, $61, $28, $61, $39, $60
      .db $5d, $4a, $ee, $11, $0f, $b8, $1d, $c1, $3e, $42
      .db $6f, $20, $fd

L_GroundArea9:
      .db $57, $a1, $0f, $23, $4e, $42, $6e, $43, $05, $d2
      .db $17, $25, $77, $04, $87, $22, $be, $31, $c4, $03
      .db $c7, $02, $e7, $60, $ea, $60, $e3, $13, $47, $e3
      .db $58, $62, $69, $61, $7a, $60, $9f, $33, $97, $40
      .db $fe, $21, $6c, $84, $bb, $62, $f2, $03, $f9, $0b
      .db $2e, $81, $38, $64, $4c, $55, $4c, $35, $a8, $64
      .db $ae, $11, $29, $e1, $39, $71, $59, $63, $5e, $10
      .db $a9, $63, $ae, $21, $b9, $71, $d9, $61, $58, $b0
      .db $76, $30, $8e, $10, $8b, $61, $94, $30, $b2, $30
      .db $ee, $31, $eb, $61, $05, $b3, $12, $7a, $5d, $48
      .db $67, $40, $8c, $02, $88, $40, $16, $e2, $19, $32
      .db $24, $14, $32, $66, $45, $22, $75, $01, $88, $02
      .db $85, $22, $98, $40, $a6, $54, $c7, $40, $dc, $01
      .db $f7, $02, $15, $f5, $4c, $14, $9f, $33, $ce, $00
      .db $dc, $25, $8c, $b4, $ce, $31, $d8, $62, $e9, $61
      .db $fa, $30, $57, $82, $75, $15, $da, $32, $d9, $32
      .db $e7, $0b, $fe, $00, $12, $be, $2d, $c8, $48, $3e
      .db $a6, $41, $75, $be, $c2, $44, $be, $91, $cf, $37
      .db $43, $e7, $4d, $4a, $cd, $41, $ee, $42, $1f, $a0
      .db $6e, $a0, $f1, $69, $fd

L_GroundArea10:
      .db $38, $11, $0f, $26, $cd, $40, $3d, $c7, $fd

L_GroundArea11:
      .db $95, $b1, $0f, $26, $0d, $02, $c8, $72, $1c, $81
      .db $38, $72, $0d, $05, $97, $34, $98, $62, $a3, $20
      .db $b3, $06, $c3, $20, $cc, $03, $f9, $91, $2c, $81
      .db $48, $62, $0d, $09, $37, $63, $47, $03, $57, $21
      .db $8c, $02, $c5, $79, $c7, $31, $f9, $11, $39, $f1
      .db $a9, $11, $6f, $b4, $d3, $65, $e3, $65, $7d, $c1
      .db $bf, $26, $fd

L_GroundArea12:
      .db $90, $51, $0f, $26, $ee, $10, $0b, $94, $33, $14
      .db $b2, $08, $02, $92, $69, $42, $b0, $00, $1c, $97
      .db $1f, $11, $8f, $14, $75, $94, $1c, $98, $1f, $11
      .db $47, $12, $9f, $15, $cc, $15, $cf, $11, $05, $c0
      .db $1f, $15, $39, $12, $7c, $16, $7f, $11, $82, $40
      .db $98, $12, $df, $15, $16, $c4, $17, $14, $54, $12
      .db $9b, $16, $28, $94, $ce, $01, $3d, $c1, $5e, $42
      .db $8f, $20, $fd

L_GroundArea13:
      .db $97, $11, $0f, $26, $fe, $10, $2b, $92, $57, $12
      .db $c0, $41, $5b, $92, $19, $93, $17, $94, $7c, $14
      .db $7f, $11, $93, $41, $bf, $15, $fc, $13, $ff, $11
      .db $2f, $95, $50, $42, $51, $12, $58, $14, $a6, $12
      .db $db, $12, $1b, $93, $46, $43, $7b, $12, $8d, $49
      .db $b7, $14, $1b, $94, $49, $0b, $bb, $12, $fc, $13
      .db $ff, $12, $03, $c1, $2f, $15, $43, $12, $4b, $13
      .db $77, $13, $9d, $4a, $15, $c1, $a1, $41, $c3, $12
      .db $fe, $01, $7d, $c1, $9e, $42, $cf, $20, $fd

L_GroundArea14:
      .db $52, $21, $0f, $20, $6e, $44, $0c, $f1, $4c, $01
      .db $aa, $35, $d9, $34, $ee, $20, $08, $b3, $37, $32
      .db $43, $04, $4e, $21, $53, $20, $9c, $81, $e7, $42
      .db $5f, $b3, $97, $63, $ac, $02, $c5, $41, $49, $e0
      .db $58, $61, $76, $64, $85, $65, $94, $66, $a4, $22
      .db $a6, $03, $c8, $22, $68, $f2, $96, $42, $13, $82
      .db $17, $02, $af, $34, $f6, $21, $26, $80, $2a, $24
      .db $36, $01, $8c, $00, $ff, $35, $4e, $a0, $55, $21
      .db $77, $20, $87, $07, $89, $22, $ae, $21, $4c, $82
      .db $9f, $34, $ec, $01, $03, $e7, $13, $67, $8d, $4a
      .db $ad, $41, $0f, $a6, $fd

L_GroundArea15:
      .db $11, $51, $00, $3f, $4c, $01, $c7, $12, $04, $92
      .db $00, $3f, $43, $14, $42, $44, $99, $14, $a8, $42
      .db $08, $94, $00, $3f, $17, $42, $55, $12, $54, $42
      .db $8f, $38, $03, $bb, $00, $3f, $28, $7a, $68, $7a
      .db $a8, $7a, $f0, $6a, $00, $be, $6d, $45, $fd

L_GroundArea16:
      .db $92, $51, $0f, $23, $5e, $46, $8e, $42, $bf, $20
      .db $0d, $c6, $0d, $01, $bc, $00, $dc, $00, $fc, $00
      .db $06, $c0, $c7, $73, $06, $83, $28, $72, $96, $40
      .db $e7, $73, $26, $c0, $87, $7b, $d2, $41, $39, $f1
      .db $c8, $f2, $97, $e3, $a3, $23, $e7, $02, $e3, $07
      .db $f3, $22, $37, $e3, $9c, $00, $bc, $00, $ec, $00
      .db $0c, $80, $3c, $00, $86, $21, $a6, $06, $b6, $24
      .db $5c, $80, $7c, $00, $9c, $00, $0c, $81, $0d, $11
      .db $36, $40, $39, $61, $39, $71, $5d, $02, $5f, $47
      .db $9c, $00, $a7, $63, $bc, $00, $b6, $41, $c6, $41
      .db $c5, $65, $dc, $00, $e3, $67, $f9, $61, $f3, $67
      .db $fd

L_GroundArea17:
      .db $55, $b1, $0f, $26, $cf, $33, $07, $b2, $15, $11
      .db $52, $42, $99, $0b, $d3, $24, $d6, $42, $d7, $25
      .db $23, $84, $cf, $33, $07, $e3, $19, $61, $78, $7a
      .db $ef, $33, $2c, $81, $46, $64, $55, $65, $65, $65
      .db $ec, $74, $47, $82, $53, $05, $63, $21, $62, $41
      .db $96, $22, $9a, $41, $cc, $03, $b9, $91, $39, $f1
      .db $63, $26, $67, $27, $d3, $06, $fc, $01, $18, $e2
      .db $d9, $07, $e9, $04, $0c, $86, $37, $22, $93, $24
      .db $87, $84, $ac, $02, $c2, $41, $c3, $23, $d9, $71
      .db $fc, $01, $7f, $b1, $9c, $00, $a7, $63, $b6, $64
      .db $cc, $00, $d4, $66, $e3, $67, $f3, $67, $8d, $c1
      .db $cf, $26, $fd

L_GroundArea18:
      .db $50, $b1, $0f, $26, $fc, $00, $1f, $b3, $5c, $00
      .db $65, $65, $74, $66, $83, $67, $4c, $80, $b3, $20
      .db $c9, $0b, $c3, $08, $d3, $2f, $88, $80, $d3, $2e
      .db $ed, $4a, $d7, $a1, $ec, $01, $4c, $80, $59, $11
      .db $d8, $11, $da, $10, $37, $a0, $47, $04, $99, $11
      .db $e7, $21, $3a, $90, $67, $20, $76, $10, $77, $60
      .db $87, $07, $d8, $12, $39, $f1, $ac, $00, $e9, $71
      .db $0c, $80, $2c, $00, $4c, $05, $c7, $7b, $39, $f1
      .db $ec, $00, $f9, $11, $0c, $82, $6f, $34, $f8, $11
      .db $fa, $10, $7f, $b2, $ac, $00, $b6, $64, $cc, $01
      .db $e3, $67, $f3, $67, $8d, $c1, $cf, $26, $fd

L_GroundArea19:
      .db $52, $b1, $0f, $20, $6e, $45, $39, $91, $b3, $04
      .db $c3, $21, $c8, $11, $ca, $10, $49, $91, $7c, $73
      .db $e8, $12, $88, $91, $8a, $10, $e7, $21, $05, $91
      .db $07, $30, $17, $07, $27, $20, $49, $11, $9c, $01
      .db $c8, $72, $23, $a6, $27, $26, $d3, $03, $d8, $7a
      .db $89, $91, $d8, $72, $39, $f1, $a9, $11, $09, $f1
      .db $63, $24, $67, $24, $d8, $62, $28, $91, $2a, $10
      .db $56, $21, $70, $04, $79, $0b, $8c, $00, $94, $21
      .db $9f, $35, $2f, $b8, $3d, $c1, $7f, $26, $fd

L_GroundArea20:
      .db $05, $c1, $4c, $00, $f6, $4f, $0d, $02, $15, $2e
      .db $24, $4c, $35, $a1, $36, $21, $53, $47, $d5, $21
      .db $d6, $21, $34, $a1, $73, $48, $74, $20, $94, $20
      .db $b4, $20, $d4, $20, $f4, $20, $2e, $80, $59, $42
      .db $4d, $c7, $fd

L_GroundArea21:
      .db $96, $31, $0f, $26, $0d, $03, $1a, $60, $c4, $00
      .db $c8, $62, $b9, $e1, $d3, $06, $d7, $07, $f9, $61
      .db $0c, $81, $4e, $b1, $8e, $b1, $bc, $01, $e4, $50
      .db $e9, $61, $0c, $81, $0d, $0a, $84, $43, $98, $72
      .db $0d, $0c, $0f, $38, $1d, $c1, $5f, $26, $fd

L_UndergroundArea1:
      .db $4b, $0f, $0e, $21, $60, $24, $67, $53, $77, $04
      .db $87, $25, $ae, $0a, $07, $a2, $2e, $0a, $5e, $21
      .db $60, $57, $99, $71, $a6, $25, $b5, $50, $c5, $07
      .db $d5, $21, $f9, $71, $2e, $90, $36, $34, $60, $3f
      .db $7e, $1e, $ca, $71, $5e, $a2, $6a, $70, $64, $23
      .db $a4, $04, $a5, $52, $a7, $23, $c6, $40, $c3, $02
      .db $de, $15, $ee, $00, $6e, $91, $a7, $25, $a1, $40
      .db $b4, $22, $c3, $07, $e0, $40, $2e, $90, $57, $55
      .db $6c, $24, $b7, $55, $0a, $d5, $1c, $44, $6a, $55
      .db $a9, $54, $b9, $73, $d9, $53, $e3, $33, $f1, $79
      .db $29, $d3, $24, $02, $39, $73, $35, $02, $59, $55
      .db $7e, $21, $8b, $62, $d7, $21, $d7, $52, $e4, $22
      .db $fc, $60, $f7, $06, $07, $a1, $17, $52, $5f, $33
      .db $97, $63, $9e, $00, $f7, $32, $77, $b2, $ee, $01
      .db $f8, $64, $fe, $21, $9f, $ca, $ce, $0f, $0d, $c7
      .db $ae, $01, $c7, $14, $ec, $42, $3d, $c6, $fe, $0f
      .db $fd

L_UndergroundArea2:
      .db $48, $0f, $0c, $03, $0e, $01, $16, $41, $29, $61
      .db $27, $73, $2d, $41, $3f, $35, $4c, $01, $5e, $31
      .db $66, $74, $6c, $04, $7c, $02, $7c, $00, $89, $61
      .db $86, $40, $88, $72, $ac, $04, $b7, $40, $dc, $00
      .db $dc, $05, $e8, $72, $f6, $41, $f8, $40, $0c, $84
      .db $4e, $34, $6e, $24, $67, $40, $85, $40, $8c, $70
      .db $a6, $40, $b8, $02, $c7, $02, $d6, $20, $e6, $07
      .db $f6, $07, $fe, $0f, $ff, $45, $ee, $80, $fe, $80
      .db $4e, $82, $66, $02, $6e, $01, $73, $05, $75, $02
      .db $86, $02, $88, $72, $a5, $02, $a3, $21, $c7, $20
      .db $d7, $07, $e4, $76, $07, $a0, $17, $06, $48, $7a
      .db $76, $20, $98, $72, $79, $e1, $88, $62, $9c, $01
      .db $b7, $73, $dc, $01, $f8, $62, $fe, $01, $08, $e2
      .db $0e, $00, $6e, $02, $73, $20, $77, $23, $83, $04
      .db $93, $20, $ae, $00, $fe, $0a, $0e, $82, $39, $71
      .db $a8, $72, $e7, $73, $0c, $81, $8f, $32, $ae, $00
      .db $fe, $04, $04, $d1, $17, $04, $26, $49, $27, $29
      .db $df, $33, $fe, $02, $44, $f6, $7c, $01, $8e, $06
      .db $8f, $47, $ee, $0f, $4d, $c7, $0e, $82, $68, $7a
      .db $ae, $01, $de, $0f, $6d, $c5, $fd

L_UndergroundArea3:
      .db $48, $01, $0e, $01, $00, $5a, $17, $02, $27, $02
      .db $3e, $0e, $37, $02, $58, $43, $52, $43, $69, $41
      .db $9e, $01, $a5, $01, $b7, $02, $c8, $02, $c4, $34
      .db $df, $43, $4d, $c7, $0e, $81, $00, $5a, $2e, $32
      .db $35, $21, $31, $53, $55, $01, $65, $25, $68, $02
      .db $78, $40, $88, $02, $98, $40, $a8, $02, $c5, $07
      .db $c8, $02, $d5, $22, $d7, $40, $df, $44, $ee, $0f
      .db $fc, $00, $4d, $c7, $0e, $81, $00, $5a, $47, $50
      .db $a3, $50, $b3, $51, $c3, $53, $df, $4a, $4d, $c7
      .db $0e, $81, $00, $6a, $2e, $01, $35, $15, $44, $43
      .db $7a, $62, $98, $01, $a3, $16, $ba, $62, $ef, $42
      .db $00, $b1, $4d, $47, $0e, $81, $00, $5a, $3e, $02
      .db $46, $51, $47, $25, $56, $44, $73, $01, $a1, $56
      .db $ae, $01, $c8, $07, $d8, $23, $df, $47, $4d, $c7
      .db $fd

L_CastleArea1:
      .db $93, $08, $0e, $22, $5a, $30, $6c, $52, $9a, $30
      .db $be, $43, $ce, $07, $d3, $3c, $2c, $81, $9e, $23
      .db $ac, $56, $b8, $11, $e8, $11, $0e, $87, $7e, $0a
      .db $98, $11, $b3, $01, $b4, $50, $c8, $11, $ee, $16
      .db $1e, $9a, $48, $11, $63, $51, $78, $11, $ae, $28
      .db $0e, $a4, $44, $64, $7e, $1b, $66, $8a, $fe, $22
      .db $27, $82, $24, $30, $4e, $2e, $fa, $08, $f1, $08
      .db $7e, $b1, $84, $36, $80, $3a, $ae, $30, $f4, $0a
      .db $fa, $6b, $0c, $d6, $18, $11, $30, $63, $47, $11
      .db $6e, $31, $70, $38, $77, $66, $c4, $04, $c7, $50
      .db $d4, $64, $e4, $31, $fe, $3e, $64, $8a, $ce, $21
      .db $f7, $02, $07, $82, $17, $08, $27, $02, $37, $02
      .db $56, $64, $ae, $20, $d9, $1a, $f5, $16, $21, $e6
      .db $9e, $31, $f8, $02, $06, $82, $14, $02, $22, $02
      .db $27, $63, $3c, $50, $42, $68, $41, $08, $19, $84
      .db $16, $20, $ae, $08, $d4, $08, $fe, $2a, $0d, $c4
      .db $37, $50, $43, $24, $61, $54, $7e, $1a, $97, $50
      .db $cd, $43, $ce, $0b, $dd, $42, $fe, $02, $5d, $c7
      .db $fd

L_CastleArea2:
      .db $5b, $07, $05, $32, $06, $33, $07, $34, $5e, $0a
      .db $68, $64, $98, $64, $a8, $64, $ce, $06, $fe, $02
      .db $0d, $01, $1e, $0e, $7e, $02, $94, $63, $b4, $63
      .db $d4, $63, $f4, $63, $14, $e3, $2e, $0e, $5e, $02
      .db $64, $35, $88, $72, $be, $0e, $0d, $04, $ae, $02
      .db $ce, $08, $cd, $4b, $fe, $02, $0d, $05, $68, $31
      .db $7e, $0a, $96, $31, $a9, $63, $a8, $33, $d5, $30
      .db $ee, $02, $e6, $62, $f4, $61, $04, $b1, $08, $3f
      .db $44, $33, $94, $63, $a4, $31, $e4, $31, $04, $bf
      .db $08, $3f, $04, $bf, $08, $3f, $cd, $4b, $03, $e4
      .db $0e, $03, $2e, $01, $7e, $06, $be, $02, $de, $06
      .db $fe, $0a, $0d, $c4, $cd, $43, $ce, $09, $de, $0b
      .db $dd, $42, $fe, $02, $5d, $c7, $fd

L_CastleArea3:
      .db $9b, $07, $07, $34, $1e, $80, $1e, $84, $9e, $88
      .db $fe, $87, $2e, $81, $be, $00, $2e, $87, $b5, $0a
      .db $0d, $c4, $cd, $43, $ce, $09, $dd, $42, $de, $0b
      .db $fe, $02, $5d, $c7, $fd

L_CastleArea4:
      .db $9b, $07, $05, $32, $06, $33, $07, $34, $fe, $06
      .db $0c, $81, $39, $0a, $5c, $01, $89, $0a, $ac, $01
      .db $d9, $0a, $fc, $01, $2e, $83, $a7, $01, $b7, $00
      .db $c7, $01, $de, $0a, $fe, $02, $4e, $83, $5a, $32
      .db $63, $0a, $69, $0a, $7e, $02, $ee, $03, $fa, $32
      .db $03, $8a, $09, $0a, $1e, $02, $ee, $03, $fa, $32
      .db $03, $8a, $09, $0a, $14, $42, $1e, $02, $7e, $0a
      .db $9e, $07, $fe, $0a, $2e, $86, $5e, $0a, $8e, $06
      .db $be, $0a, $ee, $07, $3e, $83, $5e, $07, $fe, $0a
      .db $0d, $c4, $41, $52, $51, $52, $cd, $43, $ce, $09
      .db $de, $0b, $dd, $42, $fe, $02, $5d, $c7, $fd

L_CastleArea5:
      .db $5b, $07, $05, $32, $06, $33, $07, $34, $fe, $0a
      .db $ae, $86, $be, $07, $fe, $02, $0d, $02, $27, $32
      .db $5e, $0e, $1e, $82, $68, $3c, $74, $3a, $7d, $4b
      .db $5e, $8e, $7d, $4b, $7e, $82, $84, $62, $94, $61
      .db $a4, $31, $bd, $4b, $ce, $06, $fe, $02, $0d, $06
      .db $34, $31, $3e, $0a, $64, $32, $75, $0a, $7b, $61
      .db $a4, $33, $ae, $02, $de, $0e, $3e, $82, $64, $32
      .db $78, $32, $b4, $36, $c8, $36, $dd, $4b, $44, $b2
      .db $58, $32, $94, $63, $a4, $3e, $ba, $30, $c9, $61
      .db $ce, $06, $dd, $4b, $ce, $86, $dd, $4b, $fe, $02
      .db $2e, $86, $5e, $02, $7e, $06, $fe, $02, $1e, $86
      .db $3e, $02, $5e, $06, $7e, $02, $9e, $06, $fe, $0a
      .db $0d, $c4, $cd, $43, $ce, $09, $de, $0b, $dd, $42
      .db $fe, $02, $5d, $c7, $fd

L_CastleArea6:
      .db $5b, $06, $05, $32, $06, $33, $07, $34, $5e, $0a
      .db $ae, $02, $0d, $01, $39, $73, $0d, $03, $39, $7b
      .db $4d, $4b, $de, $06, $1e, $8a, $ae, $06, $16, $fe
      .db $a5, $77, $fe, $02, $fe, $82, $0d, $07, $39, $73
      .db $a8, $74, $ed, $4b, $49, $fb, $e8, $74, $fe, $0a
      .db $2e, $82, $67, $02, $84, $7a, $87, $31, $0d, $0b
      .db $fe, $02, $0d, $0c, $39, $73, $5e, $06, $c6, $76
      .db $45, $ff, $be, $0a, $dd, $48, $fe, $06, $3d, $cb
      .db $46, $7e, $ad, $4a, $fe, $82, $39, $f3, $a9, $7b
      .db $4e, $8a, $9e, $07, $fe, $0a, $0d, $c4, $cd, $43
      .db $ce, $09, $de, $0b, $dd, $42, $fe, $02, $5d, $c7
      .db $fd

