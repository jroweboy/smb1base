
.segment "CHR"

; Switch sprites and bg
.incbin "../Super Mario Bros. (World).nes", $9010, $1000
.incbin "../Super Mario Bros. (World).nes", $8010, $1000

.repeat 48,I
.incbin .sprintf("../chr/rotate_%d.chr", I)
.endrepeat
