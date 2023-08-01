.include "blocks_panicuncut.inc"

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
	.byte piece00::bits_bank       ; bits_bank
	.byte piece00::slopes_bank     ; slopes_bank
	.addr piece00::bits            ; bits
	.addr piece00::slopes          ; slopes
	.byte piece00::initial_sample  ; initial_sample
	.byte piece00::length          ; length

	.byte piece01::bits_bank       ; bits_bank
	.byte piece01::slopes_bank     ; slopes_bank
	.addr piece01::bits            ; bits
	.addr piece01::slopes          ; slopes
	.byte piece01::initial_sample  ; initial_sample
	.byte piece01::length          ; length

	.byte piece02::bits_bank       ; bits_bank
	.byte piece02::slopes_bank     ; slopes_bank
	.addr piece02::bits            ; bits
	.addr piece02::slopes          ; slopes
	.byte piece02::initial_sample  ; initial_sample
	.byte piece02::length          ; length

	.byte piece03::bits_bank       ; bits_bank
	.byte piece03::slopes_bank     ; slopes_bank
	.addr piece03::bits            ; bits
	.addr piece03::slopes          ; slopes
	.byte piece03::initial_sample  ; initial_sample
	.byte piece03::length          ; length

	.byte piece04::bits_bank       ; bits_bank
	.byte piece04::slopes_bank     ; slopes_bank
	.addr piece04::bits            ; bits
	.addr piece04::slopes          ; slopes
	.byte piece04::initial_sample  ; initial_sample
	.byte piece04::length          ; length

	.byte piece05::bits_bank       ; bits_bank
	.byte piece05::slopes_bank     ; slopes_bank
	.addr piece05::bits            ; bits
	.addr piece05::slopes          ; slopes
	.byte piece05::initial_sample  ; initial_sample
	.byte piece05::length          ; length

	.byte piece06::bits_bank       ; bits_bank
	.byte piece06::slopes_bank     ; slopes_bank
	.addr piece06::bits            ; bits
	.addr piece06::slopes          ; slopes
	.byte piece06::initial_sample  ; initial_sample
	.byte piece06::length          ; length

	.byte piece07::bits_bank       ; bits_bank
	.byte piece07::slopes_bank     ; slopes_bank
	.addr piece07::bits            ; bits
	.addr piece07::slopes          ; slopes
	.byte piece07::initial_sample  ; initial_sample
	.byte piece07::length          ; length

	.byte piece08::bits_bank       ; bits_bank
	.byte piece08::slopes_bank     ; slopes_bank
	.addr piece08::bits            ; bits
	.addr piece08::slopes          ; slopes
	.byte piece08::initial_sample  ; initial_sample
	.byte piece08::length          ; length

	.byte piece09::bits_bank       ; bits_bank
	.byte piece09::slopes_bank     ; slopes_bank
	.addr piece09::bits            ; bits
	.addr piece09::slopes          ; slopes
	.byte piece09::initial_sample  ; initial_sample
	.byte piece09::length          ; length

	.byte piece0a::bits_bank       ; bits_bank
	.byte piece0a::slopes_bank     ; slopes_bank
	.addr piece0a::bits            ; bits
	.addr piece0a::slopes          ; slopes
	.byte piece0a::initial_sample  ; initial_sample
	.byte piece0a::length          ; length

	.byte piece0b::bits_bank       ; bits_bank
	.byte piece0b::slopes_bank     ; slopes_bank
	.addr piece0b::bits            ; bits
	.addr piece0b::slopes          ; slopes
	.byte piece0b::initial_sample  ; initial_sample
	.byte piece0b::length          ; length

	.byte piece0c::bits_bank       ; bits_bank
	.byte piece0c::slopes_bank     ; slopes_bank
	.addr piece0c::bits            ; bits
	.addr piece0c::slopes          ; slopes
	.byte piece0c::initial_sample  ; initial_sample
	.byte piece0c::length          ; length

	.byte piece0d::bits_bank       ; bits_bank
	.byte piece0d::slopes_bank     ; slopes_bank
	.addr piece0d::bits            ; bits
	.addr piece0d::slopes          ; slopes
	.byte piece0d::initial_sample  ; initial_sample
	.byte piece0d::length          ; length

	.byte piece0e::bits_bank       ; bits_bank
	.byte piece0e::slopes_bank     ; slopes_bank
	.addr piece0e::bits            ; bits
	.addr piece0e::slopes          ; slopes
	.byte piece0e::initial_sample  ; initial_sample
	.byte piece0e::length          ; length

	.byte piece0f::bits_bank       ; bits_bank
	.byte piece0f::slopes_bank     ; slopes_bank
	.addr piece0f::bits            ; bits
	.addr piece0f::slopes          ; slopes
	.byte piece0f::initial_sample  ; initial_sample
	.byte piece0f::length          ; length

	.byte piece10::bits_bank       ; bits_bank
	.byte piece10::slopes_bank     ; slopes_bank
	.addr piece10::bits            ; bits
	.addr piece10::slopes          ; slopes
	.byte piece10::initial_sample  ; initial_sample
	.byte piece10::length          ; length

	.byte piece11::bits_bank       ; bits_bank
	.byte piece11::slopes_bank     ; slopes_bank
	.addr piece11::bits            ; bits
	.addr piece11::slopes          ; slopes
	.byte piece11::initial_sample  ; initial_sample
	.byte piece11::length          ; length

	.byte piece12::bits_bank       ; bits_bank
	.byte piece12::slopes_bank     ; slopes_bank
	.addr piece12::bits            ; bits
	.addr piece12::slopes          ; slopes
	.byte piece12::initial_sample  ; initial_sample
	.byte piece12::length          ; length

	.byte piece13::bits_bank       ; bits_bank
	.byte piece13::slopes_bank     ; slopes_bank
	.addr piece13::bits            ; bits
	.addr piece13::slopes          ; slopes
	.byte piece13::initial_sample  ; initial_sample
	.byte piece13::length          ; length

	.byte piece14::bits_bank       ; bits_bank
	.byte piece14::slopes_bank     ; slopes_bank
	.addr piece14::bits            ; bits
	.addr piece14::slopes          ; slopes
	.byte piece14::initial_sample  ; initial_sample
	.byte piece14::length          ; length

	.byte piece15::bits_bank       ; bits_bank
	.byte piece15::slopes_bank     ; slopes_bank
	.addr piece15::bits            ; bits
	.addr piece15::slopes          ; slopes
	.byte piece15::initial_sample  ; initial_sample
	.byte piece15::length          ; length

	.byte piece16::bits_bank       ; bits_bank
	.byte piece16::slopes_bank     ; slopes_bank
	.addr piece16::bits            ; bits
	.addr piece16::slopes          ; slopes
	.byte piece16::initial_sample  ; initial_sample
	.byte piece16::length          ; length

	.byte piece17::bits_bank       ; bits_bank
	.byte piece17::slopes_bank     ; slopes_bank
	.addr piece17::bits            ; bits
	.addr piece17::slopes          ; slopes
	.byte piece17::initial_sample  ; initial_sample
	.byte piece17::length          ; length

	.byte piece18::bits_bank       ; bits_bank
	.byte piece18::slopes_bank     ; slopes_bank
	.addr piece18::bits            ; bits
	.addr piece18::slopes          ; slopes
	.byte piece18::initial_sample  ; initial_sample
	.byte piece18::length          ; length

	.byte piece19::bits_bank       ; bits_bank
	.byte piece19::slopes_bank     ; slopes_bank
	.addr piece19::bits            ; bits
	.addr piece19::slopes          ; slopes
	.byte piece19::initial_sample  ; initial_sample
	.byte piece19::length          ; length

	.byte piece1a::bits_bank       ; bits_bank
	.byte piece1a::slopes_bank     ; slopes_bank
	.addr piece1a::bits            ; bits
	.addr piece1a::slopes          ; slopes
	.byte piece1a::initial_sample  ; initial_sample
	.byte piece1a::length          ; length

	.byte piece1b::bits_bank       ; bits_bank
	.byte piece1b::slopes_bank     ; slopes_bank
	.addr piece1b::bits            ; bits
	.addr piece1b::slopes          ; slopes
	.byte piece1b::initial_sample  ; initial_sample
	.byte piece1b::length          ; length

	.byte piece1c::bits_bank       ; bits_bank
	.byte piece1c::slopes_bank     ; slopes_bank
	.addr piece1c::bits            ; bits
	.addr piece1c::slopes          ; slopes
	.byte piece1c::initial_sample  ; initial_sample
	.byte piece1c::length          ; length

	.byte piece1d::bits_bank       ; bits_bank
	.byte piece1d::slopes_bank     ; slopes_bank
	.addr piece1d::bits            ; bits
	.addr piece1d::slopes          ; slopes
	.byte piece1d::initial_sample  ; initial_sample
	.byte piece1d::length          ; length

	.byte piece1e::bits_bank       ; bits_bank
	.byte piece1e::slopes_bank     ; slopes_bank
	.addr piece1e::bits            ; bits
	.addr piece1e::slopes          ; slopes
	.byte piece1e::initial_sample  ; initial_sample
	.byte piece1e::length          ; length

	.byte piece1f::bits_bank       ; bits_bank
	.byte piece1f::slopes_bank     ; slopes_bank
	.addr piece1f::bits            ; bits
	.addr piece1f::slopes          ; slopes
	.byte piece1f::initial_sample  ; initial_sample
	.byte piece1f::length          ; length

	.byte piece20::bits_bank       ; bits_bank
	.byte piece20::slopes_bank     ; slopes_bank
	.addr piece20::bits            ; bits
	.addr piece20::slopes          ; slopes
	.byte piece20::initial_sample  ; initial_sample
	.byte piece20::length          ; length

	.byte piece21::bits_bank       ; bits_bank
	.byte piece21::slopes_bank     ; slopes_bank
	.addr piece21::bits            ; bits
	.addr piece21::slopes          ; slopes
	.byte piece21::initial_sample  ; initial_sample
	.byte piece21::length          ; length

.export num_sblk_headers
num_sblk_headers:
	.byte 34
