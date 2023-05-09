

.segment "COS_TABLE"

.export CosTable
CosTable:
.incbin "cosinetable.bin"

.segment "SIN_TABLE"
.export SinTable
SinTable:
.incbin "sinetable.bin"
