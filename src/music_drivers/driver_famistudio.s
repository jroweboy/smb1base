.ifndef DRIVER_FAMISTUDIO_S
.define DRIVER_FAMISTUDIO_S

.macro MusicInit
  BankPRGA #.bank(music_data)
  ldx #<music_data
  ldy #>music_data
  lda #0
  jsr famistudio_init
  BankPRGA CurrentBank
.if ::USE_VANILLA_SFX
  lda #1
  sta ReloadChannel+0
  sta ReloadChannel+1
.endif
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

; ======================================================================================================================
; SMBStudio:
; Mix Famistudio output with the vanilla sfx output

.if USE_VANILLA_SFX

.macro CustomMusicMixAudio
.scope
  ; if bit 7 (paused) then don't mix any audio
  lda famistudio_song_speed
  bpl :+
    rts
  :
  ; load the data from the famistudio output buffer
  lda Square1SoundBuffer
  beq @music_pulse1_upd
    lda #1
    sta ReloadChannel+0
    jmp @no_pulse1_upd
    ; lda SfxOutputBuffer      ; Pulse 1 volume
    ; sta FAMISTUDIO_APU_PL1_VOL
    ; lda SfxOutputBuffer+1    ; Pulse 1 Sweep
    ; sta FAMISTUDIO_APU_PL1_SWEEP
    ; lda SfxOutputBuffer+2    ; Pulse 1 period LSB
    ; sta FAMISTUDIO_APU_PL1_LO
    ; lda SfxOutputBuffer+3    ; Pulse 1 period MSB, only applied when changed
    ; cmp famistudio_pulse1_prev
    ; beq @no_pulse1_upd
    ;   sta famistudio_pulse1_prev
    ;   sta FAMISTUDIO_APU_PL1_HI
    ;   jmp @no_pulse1_upd ; unconditional
    ; .if ::FAMISTUDIO_CFG_SMOOTH_VIBRATO
    ;     famistudio_smooth_vibrato SfxOutputBuffer+2, famistudio_pulse1_prev, FAMISTUDIO_APU_PL1_HI, FAMISTUDIO_APU_PL1_LO, FAMISTUDIO_APU_PL1_SWEEP
    ; .else
    ;     cmp famistudio_pulse1_prev
    ;     beq @no_pulse1_upd
    ;     sta famistudio_pulse1_prev
    ;     sta FAMISTUDIO_APU_PL1_HI
    ; .endif
    ; bne @no_pulse1_upd ; unconditional

@music_pulse1_upd:
    lda famistudio_output_buf      ; Pulse 1 volume
    sta FAMISTUDIO_APU_PL1_VOL
    lda #$08
    sta FAMISTUDIO_APU_PL1_SWEEP
    lda famistudio_output_buf+1    ; Pulse 1 period LSB
    sta FAMISTUDIO_APU_PL1_LO
    lda famistudio_output_buf+2    ; Pulse 1 period MSB, only applied when changed
    ldx ReloadChannel+0
    bne @force_update1
      cmp famistudio_pulse1_prev
      beq @no_pulse1_upd
@force_update1:
        sta famistudio_pulse1_prev
        sta FAMISTUDIO_APU_PL1_HI
        lda #0
        sta ReloadChannel+0
    ; .if ::FAMISTUDIO_CFG_SMOOTH_VIBRATO
    ;     famistudio_smooth_vibrato famistudio_output_buf+1, famistudio_pulse1_prev, FAMISTUDIO_APU_PL1_HI, FAMISTUDIO_APU_PL1_LO, FAMISTUDIO_APU_PL1_SWEEP
    ; .else
    ;     cmp famistudio_pulse1_prev
    ;     beq @no_pulse1_upd
    ;     sta famistudio_pulse1_prev
    ;     sta FAMISTUDIO_APU_PL1_HI
    ; .endif

@no_pulse1_upd:
  lda Square2SoundBuffer
  beq @music_pulse2_upd
    lda #1
    sta ReloadChannel+1
    jmp @no_pulse2_upd
    ; lda SfxOutputBuffer+4    ; Pulse 2 volume
    ; sta FAMISTUDIO_APU_PL2_VOL
    ; lda SfxOutputBuffer+5    ; Pulse 1 Sweep
    ; sta FAMISTUDIO_APU_PL2_SWEEP
    ; lda SfxOutputBuffer+6    ; Pulse 2 period LSB
    ; sta FAMISTUDIO_APU_PL2_LO
    ; lda SfxOutputBuffer+7    ; Pulse 2 period MSB, only applied when changed

    ; .if ::FAMISTUDIO_CFG_SMOOTH_VIBRATO
    ;     famistudio_smooth_vibrato SfxOutputBuffer+4, famistudio_pulse2_prev, FAMISTUDIO_APU_PL2_HI, FAMISTUDIO_APU_PL2_LO, FAMISTUDIO_APU_PL2_SWEEP
    ; .else
    ;     cmp famistudio_pulse2_prev
    ;     beq @no_pulse2_upd
    ;     sta famistudio_pulse2_prev
    ;     sta FAMISTUDIO_APU_PL2_HI
    ; .endif

    ; bne @no_pulse2_upd ; unconditional

@music_pulse2_upd:
    lda famistudio_output_buf+3    ; Pulse 2 volume
    sta FAMISTUDIO_APU_PL2_VOL
    lda #$08    ; Pulse 2 Sweep
    sta FAMISTUDIO_APU_PL2_SWEEP
    lda famistudio_output_buf+4    ; Pulse 2 period LSB
    sta FAMISTUDIO_APU_PL2_LO
    lda famistudio_output_buf+5    ; Pulse 2 period MSB, only applied when changed
    ldx ReloadChannel+1
    bne @force_update2
      cmp famistudio_pulse2_prev
      beq @no_pulse2_upd
@force_update2:
        sta famistudio_pulse2_prev
        sta FAMISTUDIO_APU_PL2_HI
        lda #0
        sta ReloadChannel+1

    ; .if ::FAMISTUDIO_CFG_SMOOTH_VIBRATO
    ;     famistudio_smooth_vibrato famistudio_output_buf+4, famistudio_pulse2_prev, FAMISTUDIO_APU_PL2_HI, FAMISTUDIO_APU_PL2_LO, FAMISTUDIO_APU_PL2_SWEEP
    ; .else
    ;     cmp famistudio_pulse2_prev
    ;     beq @no_pulse2_upd
    ;     sta famistudio_pulse2_prev
    ;     sta FAMISTUDIO_APU_PL2_HI
    ; .endif

@no_pulse2_upd:
; No triangle used in vanilla sfx
    lda famistudio_output_buf+6    ; Triangle volume (plays or not)
    sta FAMISTUDIO_APU_TRI_LINEAR
    lda famistudio_output_buf+7    ; Triangle period LSB
    sta FAMISTUDIO_APU_TRI_LO
    lda famistudio_output_buf+8    ; Triangle period MSB
    sta FAMISTUDIO_APU_TRI_HI

  lda NoiseSoundBuffer
  beq @music_noise_upd
    ; lda SfxOutputBuffer+9    ; Noise volume
    ; sta FAMISTUDIO_APU_NOISE_VOL
    ; lda SfxOutputBuffer+10   ; Noise period
    ; sta FAMISTUDIO_APU_NOISE_LO
    jmp @no_noise_upd
@music_noise_upd:
    lda famistudio_output_buf+9    ; Noise volume
    sta FAMISTUDIO_APU_NOISE_VOL
    lda famistudio_output_buf+10   ; Noise period
    sta FAMISTUDIO_APU_NOISE_LO

@no_noise_upd:
.endscope
.endmacro

.endif
