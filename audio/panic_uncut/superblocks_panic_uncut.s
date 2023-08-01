.include "blocks_panic_uncut.inc"

.segment "HDRS"

;.export sblk_header_s
;.struct sblk_header_s
;	bits_bank       .byte
;	slopes_bank     .byte
;	bits            .addr
;	slopes          .addr
;	initial_sample  .byte
;	length          .byte
;.endstruct

.export sblk_table
sblk_table:
.export num_sblk_headers
num_sblk_headers:
	.byte 0
