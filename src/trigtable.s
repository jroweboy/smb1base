

; .segment "COS_TABLE"

; .export CosTable
; CosTable:
; .incbin "cosinetable.bin"

; .segment "SIN_TABLE"
; .export SinTable
; SinTable:
; .incbin "sinetable.bin"

.segment "PLAYER"

.export CosTable
CosTable:
.incbin "cosinetable.bin"

.export SinTable
SinTable:
.incbin "sinetable.bin"
