; New NESlib RLE decompressor, improvements
; as suggested by PinoBatch, implemented by Nesrocks. 
; NESlib Originally written by Shiru. 
; comments are a mix of Nesrocks and FrankenGraphics.

; overall, NESlib RLE compression is fine
; for single-screen and vertically scrolling games,
; or games of smaller scope.

; Larger games, and games using horizontal scrolling,
; should consider metatile/dictionary compression
; instead.

; it might also be ok for some games to not use 
; nametable compression at all; this depends on 
; your ROM constraints and your ambition.



; Util constants
; "temp" should refer to temp memory in the zero page. 

_RLE_LOW = temp
_RLE_HIGH = temp+1
_RLE_TAG = temp+2 ; this value marks the end of the sequence
_RLE_BYTE = temp+3

; Decompresses a stream compressed with Shiru RLE to video memory.
; The first byte in the stream is the run marker.
; After that, any byte other than the run marker is copied literally to the output.
; A run marker followed by 0 ends the stream.
; a run marker followed by 1 means itself (uncompressed);
; A run marker followed by a byte with value 2-255 writes
; that many copies of the most recently written byte to the stream.

; PARAMETERS: registers XA should contain address of
; data to unpack 

load_nam_rle:
  ; Set up
  tay
  stx <_RLE_HIGH
  lda #0
  sta <_RLE_LOW
  ; Read byte that does not appear in data, used to signal a run
  lda (_RLE_LOW),y
  sta <_RLE_TAG
  iny
  bne @tag_nowrap
    inc <_RLE_HIGH
  @tag_nowrap:
@decodeloop:
  ; Read a byte from the stream
  lda (_RLE_LOW),y
  iny
  bne @main_nowrap
    inc <_RLE_HIGH
  @main_nowrap:
 ; If it doesn't match the run marker, output it
  cmp <_RLE_TAG
  beq @is_rle
  @is_literal:
    sta _PPUDATA_2007
    sta <_RLE_BYTE
    bne @decodeloop
  @is_rle:
  ; We just saw a run marker.  Load the length, stopping if zero
  lda (_RLE_LOW),y
  beq @done
  iny
  bne @len_nowrap
    inc <_RLE_HIGH
  @len_nowrap:
  ; The run marker followed by $01 means itself
  cmp #$01
  bcs @is_run
    lda <_RLE_TAG
    bcc @is_literal
  @is_run:
  tax  ; X = length of run
  ; Output the most recent byte X times
  lda <_RLE_BYTE
  @runloop:
    sta _PPUDATA_2007
    dex
    bne @runloop
  beq @decodeloop
@done:
  rts