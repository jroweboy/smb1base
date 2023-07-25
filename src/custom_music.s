.include "common.inc"

.segment "BSS"

.segment "MUSIC_ENGINE"

FAMISTUDIO_CFG_EXTERNAL = 1

FAMISTUDIO_USE_VOLUME_TRACK = 1
FAMISTUDIO_USE_FAMITRACKER_TEMPO = 1
FAMISTUDIO_CFG_SFX_SUPPORT = 1

FAMISTUDIO_USE_RELEASE_NOTES = 0
FAMISTUDIO_USE_VOLUME_SLIDES = 0
FAMISTUDIO_USE_PITCH_TRACK = 0
FAMISTUDIO_USE_SLIDE_NOTES = 0
FAMISTUDIO_USE_VIBRATO = 0
FAMISTUDIO_USE_ARPEGGIO = 0
FAMISTUDIO_USE_DUTYCYCLE_EFFECT = 0
FAMISTUDIO_USE_DELTA_COUNTER = 0
FAMISTUDIO_USE_DPCM_BANKSWITCHING = 0
FAMISTUDIO_USE_DPCM_EXTENDED_RANGE = 0
FAMISTUDIO_USE_PHASE_RESET = 0

OVERWORLD_MUSIC = $07
UNDERWORLD_MUSIC = $09
CLOUD_TITLE_MUSIC = $01

.export CustomSoundInit
CustomSoundInit:

  BankPRGA #.bank(music_data)
  ldx #<music_data
  ldy #>music_data
  lda #0
  jsr famistudio_init

  ldx #<sfx_data
  ldy #>sfx_data
  jsr famistudio_sfx_init

  lda #CLOUD_TITLE_MUSIC
  jsr famistudio_music_play

  rts


.export CustomSoundEngine
CustomSoundEngine:
  lda AreaMusicQueue
  beq SkipAreaProcessing

SkipAreaProcessing:

  BankPRGA #.bank(music_data)
  jsr famistudio_update
  BankPRGA CurrentBank
  
  lda #$00               ;clear the music queues
  sta AreaMusicQueue
  sta EventMusicQueue
  rts

.define FAMISTUDIO_CA65_ZP_SEGMENT   ZEROPAGE
.define FAMISTUDIO_CA65_RAM_SEGMENT  WRAM
.define FAMISTUDIO_CA65_CODE_SEGMENT MUSIC_ENGINE
.include "famistudio_ca65.s"

.segment "MUSIC_DATA"
music_data:
  .include "disco_mario.s"
sfx_data:
  .include "disco_mario_sfx.s"

.segment "DPCM_00"
  .incbin "../audio/disco_mario.dmc"

