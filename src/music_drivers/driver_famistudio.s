.ifndef DRIVER_FAMISTUDIO_S
.define DRIVER_FAMISTUDIO_S

.macro MusicInit
  BankPRGA #.bank(music_data)
  ldx #<music_data
  ldy #>music_data
  lda #0
  jsr famistudio_init
  BankPRGA CurrentBank

.endmacro

.if ::USE_CUSTOM_ENGINE_SFX
.macro SFXInit
  BankPRGA #.bank(sfx_data)
  ldx #<sfx_data
  ldy #>sfx_data
  jsr famistudio_sfx_init
  BankPRGA CurrentBank
.endmacro
.endif

.macro CustomMusicPlay song
.if .match(song, a)
  ; Nothing
.elseif .match(song, x)
  txa
.elseif .match(song, y)
  tya
.else
  .if (.match (.left (1, {song}), #))
    lda #.right (.tcount ({song})-1, {song})
  .else
    lda song
  .endif
.endif
  jsr famistudio_music_play
.endmacro

.macro CustomMusicStop
  lda #1
  jsr famistudio_music_pause
.endmacro

.macro CustomMusicPause
  lda #1
  jsr famistudio_music_pause
.endmacro

.macro CustomMusicUnpause
  lda #0
  jsr famistudio_music_pause
.endmacro

.macro CustomMusicUpdate
  jsr famistudio_update
.endmacro

.if ::USE_CUSTOM_ENGINE_SFX

.macro CustomSFXPlay sfx, chan
.if .match(sfx, a)
  ; Nothing
.elseif .match(sfx, x)
  txa
.elseif .match(sfx, y)
  tya
.else
  .if (.match (.left (1, {sfx}), #))
    lda #.right (.tcount ({sfx})-1, {sfx})
  .else
    lda sfx
  .endif
.endif

.local Channel
.ifblank chan
Channel = 0
.else
Channel = chan
.endif

.if Channel = 0
  ldx #FAMISTUDIO_SFX_CH0
.elseif Channel = 1
  ldx #FAMISTUDIO_SFX_CH1
.elseif Channel = 2
  ldx #FAMISTUDIO_SFX_CH2
.else
  .error "Using unknown SFX channel"
.endif
  jsr famistudio_sfx_play
.endmacro

.macro CustomSFXUpdate

.endmacro

.endif ; USE_CUSTOM_ENGINE_SFX



; Setup FAMISTUDIO config stuff
; The user is expected to provide the exported values in the 

FAMISTUDIO_CFG_EXTERNAL = 1
FAMISTUDIO_CFG_DPCM_SUPPORT = 1


.define FAMISTUDIO_CA65_ZP_SEGMENT   ZEROPAGE
.define FAMISTUDIO_CA65_RAM_SEGMENT  SRAM
.define FAMISTUDIO_CA65_CODE_SEGMENT FIXED

.if USE_CUSTOM_ENGINE_SFX
FAMISTUDIO_CFG_SFX_SUPPORT = 1
FAMISTUDIO_CFG_SFX_STREAMS = 3
.endif

.proc CustomMusicLoopCallback
  lda #0
  sta EventMusicBuffer
  rts
.endproc

.include "famistudio_ca65.s"

.pushseg
.segment "MUSIC"
music_data:
.include "audio/examples/famistudio/panic_at_the_mario_disco.s"
sfx_data:
.include "audio/examples/famistudio/sfx.s"

.segment "DPCM"
.incbin "audio/examples/famistudio/panic_at_the_mario_disco.dmc"

.popseg

.endif