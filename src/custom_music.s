.include "common.inc"

.segment "BSS"

.segment "MUSIC_ENGINE"

FAMISTUDIO_CFG_EXTERNAL = 1

FAMISTUDIO_USE_VOLUME_TRACK = 1
FAMISTUDIO_USE_FAMITRACKER_TEMPO = 1
FAMISTUDIO_CFG_SFX_SUPPORT = 1
FAMISTUDIO_CFG_SFX_STREAMS = 2
FAMISTUDIO_USE_RELEASE_NOTES = 1

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

  ; lda #CLOUD_TITLE_MUSIC
  lda #Cloud
  jsr famistudio_music_play

  rts

Castleworld = $00 ; : Castleworld
Cloud = $01 ; : Cloud
Death = $02 ; : Death
EnterPipe = $03 ; : Enter in a pipe
GameOver = $04 ; : Game Over
HurryUp = $05 ; : Hurry up
InAnotherCastle = $06 ; : In an other castle
Overworld = $07 ; : Overworld
Starman = $08 ; : Starman
Underworld = $09 ; : Underworld
Victory = $0a ; : Victory
Waterworld = $0b ; : Waterworld
SavedPrincess = $0c ; : You saved the princess
SilenceTrack = -1

AreaMusicLUT:
  .byte SilenceTrack, Starman, EnterPipe, Cloud, Castleworld, Underworld, Waterworld, Overworld
EventMusicLUT:
  .byte SilenceTrack, HurryUp, Victory, SilenceTrack, InAnotherCastle, SavedPrincess, GameOver, Death

OneUp =       0
BigJump =     1
Blast =       2
BowserFall =  3
BrickBreak =  4
Bump =        5
Coin =        6
EnemySmack =  7
EnemyStomp =  8
FireBreath =  9
Fireball =    10
Flagpole =    11
GrowUp =      12
SpawnPup =    13
Injure =      14
Pause =       15
SmallJump =   16
TimerTick =   17
Vine =        18

Sq2SfxTable:
  .byte BowserFall
  .byte OneUp     
  .byte GrowUp    
  .byte TimerTick 
  .byte Blast     
  .byte Vine      
  .byte SpawnPup  
  .byte Coin      

Sq1SfxTable:
  .byte SmallJump  
  .byte Flagpole   
  .byte Fireball   
  .byte Injure     
  .byte EnemySmack 
  .byte EnemyStomp 
  .byte Bump       
  .byte BigJump    

NoiseSfxTable:
  .byte $00        
  .byte $00        
  .byte $00        
  .byte $00        
  .byte $00        
  .byte Pause      
  .byte FireBreath 
  .byte BrickBreak 

.export CustomSoundEngine
CustomSoundEngine:
  BankPRGA #.bank(music_data)
  lda EventMusicQueue
  ora AreaMusicQueue
  beq SkipMusicProcessing
    ldx AreaMusicQueue
    beq SkipAreaProcessing
      lda #0
      sta AreaMusicQueue
      stx AreaMusicBuffer
      ldy CountLeadingZeroLookup,x
      lda AreaMusicLUT, y
SkipAreaProcessing:
    ldx EventMusicQueue
    beq PlayNewSong
      ; Music from EventMusicQueue will override the AreaMusicQueue
      stx EventMusicBuffer
      lda #0
      sta EventMusicQueue
      ldy CountLeadingZeroLookup,x
      lda EventMusicLUT, y
PlayNewSong:
  cmp #SilenceTrack
  beq StopMusic
    jsr famistudio_music_play
    jmp SkipMusicProcessing
StopMusic:
  jsr famistudio_music_stop
SkipMusicProcessing:

  ; Now check for sound effects
  ldx Square2SoundQueue
  beq :+
    ldy CountLeadingZeroLookup,x
    lda Sq2SfxTable,y
    ldx #FAMISTUDIO_SFX_CH0
    jsr famistudio_sfx_play
    lda #0
    sta Square2SoundQueue
  :
  ldx Square1SoundQueue
  beq :+
    ldy CountLeadingZeroLookup,x
    lda Sq1SfxTable,y
    ldx #FAMISTUDIO_SFX_CH1
    jsr famistudio_sfx_play
    lda #0
    sta Square1SoundQueue
  :
  ldx NoiseSoundQueue
  beq :+
    ldy CountLeadingZeroLookup,x
    lda NoiseSfxTable,y
    ldx #FAMISTUDIO_SFX_CH0
    jsr famistudio_sfx_play
    lda #0
    sta NoiseSoundQueue
  :

  jsr famistudio_update
  BankPRGA CurrentBank
  
  ; lda #$00               ;clear the music queues
  ; sta AreaMusicQueue
  ; sta EventMusicQueue
  rts

; When the music driver is completes playback (and before it loops)
; we create a custom callback that will run to clear out the queue
; and set song playing to 0
.export CustomMusicLoopCallback
CustomMusicLoopCallback:
	lda EventMusicBuffer
	cmp #TimeRunningOutMusic
	bne :+
    lda AreaMusicBuffer
    sta AreaMusicQueue
:
  lda #0
  sta EventMusicBuffer
	rts

.define FAMISTUDIO_CA65_ZP_SEGMENT   ZEROPAGE
.define FAMISTUDIO_CA65_RAM_SEGMENT  WRAM
.define FAMISTUDIO_CA65_CODE_SEGMENT MUSIC_ENGINE
.include "famistudio_ca65.s"

; BitCountLookup:
; .repeat 256,I
; BIT_COUNT .set 0
; .repeat 8, J
; BIT_COUNT .set BIT_COUNT + ((I & (1 << J)) >> (1 >> J))
; .endrepeat
;   .byte BIT_COUNT
; .endrepeat

CountLeadingZeroLookup:
.repeat 256,I
HI_BIT .set 8
.repeat 8, J
.if HI_BIT = 8 .and (I & (1 << (7 - J))) <> 0
HI_BIT .set J
.endif
.endrepeat
  .byte HI_BIT
.endrepeat

.segment "MUSIC_DATA"
music_data:
  .include "disco_mario.s"
sfx_data:
  .include "disco_mario_sfx.s"

.segment "DPCM_00"
  .incbin "../audio/disco_mario.dmc"

