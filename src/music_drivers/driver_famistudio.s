.ifndef DRIVER_FAMISTUDIO_S
.define DRIVER_FAMISTUDIO_S

.pushseg
.segment "SHORTRAM"
RESERVE ReloadChannel, 2

.popseg


.macro DriverMusicInit
  ; Normally we would init the song here, but to support banked music data, we split the songs into their own
  ; files, and each of those need to be initialized independently.
  BankPRGA #.bank(music_overworld_data_start)
  ldx #<music_overworld_data_start
  ldy #>music_overworld_data_start
  lda #0
  jsr famistudio_init
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
.endmacro
.endif

.macro DriverMusicPlay song
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
  ; Load the data pointer into x/y and bank switch to the correct A bank
  tax
  ldy music_data_hi_table,x
  lda music_data_bank_table,x
  BankPRGA a
  sta MusicBank
  lda music_data_lo_table,x
  tax
  ; Use NTSC playback speed and then reinit + start the song
  lda #0
  jsr famistudio_init
  lda #0
  jsr famistudio_music_play
.endmacro

.macro DriverMusicStop
  jsr famistudio_music_stop
.endmacro

.macro DriverMusicPause
  lda #1
  jsr famistudio_music_pause
.endmacro

.macro DriverMusicUnpause
  lda #0
  jsr famistudio_music_pause
.endmacro

.macro DriverMusicUpdate
  BankPRGA MusicBank
  jsr famistudio_update
.endmacro

.macro DriverSpeedUpAudio
  ; TODO figure this out if we want to support it
.endmacro

.if ::USE_CUSTOM_ENGINE_SFX

.macro DriverSFXPlay sfx, chan
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

.macro DriverSFXUpdate

.endmacro

.endif ; USE_CUSTOM_ENGINE_SFX



; Setup FAMISTUDIO config stuff
; The user is expected to provide the exported values in the 

FAMISTUDIO_CFG_EXTERNAL = 1

.define FAMISTUDIO_CA65_ZP_SEGMENT   MUSIC_ZEROPAGE_OVERLAY
.define FAMISTUDIO_CA65_RAM_SEGMENT  SRAM
.define FAMISTUDIO_CA65_CODE_SEGMENT FIXED

.if ::USE_CUSTOM_ENGINE_SFX
FAMISTUDIO_CFG_SFX_SUPPORT = 1
FAMISTUDIO_CFG_SFX_STREAMS = 3
.endif

.include "famistudio_ca65.s"

; Custom table listing song pointers and what bank they are in

.define MUSIC_LIST \
  music_overworld_data_start, \
  music_underworld_data_start, \
  music_waterworld_data_start, \
  music_castleworld_data_start, \
  music_cloud_data_start, \
  music_enterpipe_data_start, \
  music_starman_data_start, \
  music_death_data_start, \
  music_gameover_data_start, \
  music_savedprincess_data_start, \
  music_anothercastle_data_start, \
  music_victory_data_start, \
  music_hurryup_data_start, \
  music_intermediate_data_start

music_data_lo_table:
  .lobytes MUSIC_LIST

music_data_hi_table:
  .hibytes MUSIC_LIST

music_data_bank_table:
.ifdef MMC5_PRG_ROM
.define BANK_ADJUST $80
.else
.define BANK_ADJUST $00
.endif
  .byte .bank(music_overworld_data_start) | BANK_ADJUST
  .byte .bank(music_underworld_data_start) | BANK_ADJUST
  .byte .bank(music_waterworld_data_start) | BANK_ADJUST
  .byte .bank(music_castleworld_data_start) | BANK_ADJUST
  .byte .bank(music_cloud_data_start) | BANK_ADJUST
  .byte .bank(music_enterpipe_data_start) | BANK_ADJUST
  .byte .bank(music_starman_data_start) | BANK_ADJUST
  .byte .bank(music_death_data_start) | BANK_ADJUST
  .byte .bank(music_gameover_data_start) | BANK_ADJUST
  .byte .bank(music_savedprincess_data_start) | BANK_ADJUST
  .byte .bank(music_anothercastle_data_start) | BANK_ADJUST
  .byte .bank(music_victory_data_start) | BANK_ADJUST
  .byte .bank(music_hurryup_data_start) | BANK_ADJUST
  .byte .bank(music_intermediate_data_start) | BANK_ADJUST


.pushseg
.segment "MUSIC_OVERWORLD"
music_overworld_data_start:
.include "audio/examples/famistudio/disco_mario_overworld.s"

.segment "MUSIC_UNDERWORLD"
music_underworld_data_start:
.include "audio/examples/famistudio/disco_mario_underworld.s"

.segment "MUSIC_WATERWORLD"
music_waterworld_data_start:
.include "audio/examples/famistudio/disco_mario_waterworld.s"

.segment "MUSIC_CASTLEWORLD"
music_castleworld_data_start:
.include "audio/examples/famistudio/disco_mario_castleworld.s"

.segment "MUSIC_CLOUD"
music_cloud_data_start:
.include "audio/examples/famistudio/disco_mario_cloud.s"

.segment "MUSIC_ENTERPIPE"
music_enterpipe_data_start:
.include "audio/examples/famistudio/disco_mario_enter_in_a_pipe.s"

.segment "MUSIC_STARMAN"
music_starman_data_start:
.include "audio/examples/famistudio/disco_mario_starman.s"

.segment "MUSIC_DEATH"
music_death_data_start:
.include "audio/examples/famistudio/disco_mario_death.s"

.segment "MUSIC_GAMEOVER"
music_gameover_data_start:
.include "audio/examples/famistudio/disco_mario_game_over.s"

.segment "MUSIC_SAVEDPRINCESS"
music_savedprincess_data_start:
.include "audio/examples/famistudio/disco_mario_you_saved_the_princess.s"

.segment "MUSIC_ANOTHERCASTLE"
music_anothercastle_data_start:
.include "audio/examples/famistudio/disco_mario_in_an_other_castle.s"

.segment "MUSIC_VICTORY"
music_victory_data_start:
.include "audio/examples/famistudio/disco_mario_victory.s"

.segment "MUSIC_HURRYUP"
music_hurryup_data_start:
.include "audio/examples/famistudio/disco_mario_hurry_up.s"

.segment "MUSIC_INTERMEDIATE"
music_intermediate_data_start:
.include "audio/examples/famistudio/disco_mario_new_song.s"

.if ::USE_CUSTOM_ENGINE_SFX
sfx_data:
.include "audio/examples/famistudio/panic_at_the_mario_disco_sfx.s"
.endif

.segment "DPCM_BANK0"
.incbin "audio/examples/famistudio/disco_mario.dmc"

.popseg

.endif

; ======================================================================================================================
; SMBStudio:
; Mix Famistudio output with the vanilla sfx output

.if USE_VANILLA_SFX

.macro DriverMusicMixAudio
.scope
  ; if bit 7 (paused) then don't mix any audio
  ; lda famistudio_song_speed
  ; bpl :+
  ;   rts
  ; :
  ; load the data from the famistudio output buffer
  lda Square1SoundBuffer
  beq @music_pulse1_upd
    lda #1
    sta ReloadChannel+0
    jmp @no_pulse1_upd

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

@no_pulse1_upd:
  lda Square2SoundBuffer
  beq @music_pulse2_upd
    lda #1
    sta ReloadChannel+1
    jmp @no_pulse2_upd

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
@no_pulse2_upd:
    ; No triangle used in vanilla sfx
    lda famistudio_output_buf+6    ; Triangle volume (plays or not)
    sta FAMISTUDIO_APU_TRI_LINEAR
    lda famistudio_output_buf+7    ; Triangle period LSB
    sta FAMISTUDIO_APU_TRI_LO
    lda famistudio_output_buf+8    ; Triangle period MSB
    sta FAMISTUDIO_APU_TRI_HI

  lda NoiseSoundBuffer
  bne @no_noise_upd
@music_noise_upd:
    lda famistudio_output_buf+9    ; Noise volume
    sta FAMISTUDIO_APU_NOISE_VOL
    lda famistudio_output_buf+10   ; Noise period
    sta FAMISTUDIO_APU_NOISE_LO

@no_noise_upd:
.endscope
.endmacro

.endif
