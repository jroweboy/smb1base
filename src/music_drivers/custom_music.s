
.segment "FIXED"


.pushseg
.segment "SHORTRAM"
RESERVE EventMusicBuffer, 1
RESERVE AreaMusicBuffer, 1
RESERVE Square1SoundBuffer, 1
RESERVE Square2SoundBuffer, 1
RESERVE TriangleSoundBuffer, 1 ; Newly added
RESERVE NoiseSoundBuffer, 1
; RESERVE SND_MASTERCTRL_REG_Mirror, 1
RESERVE ReloadChannel, 2

.segment "BSS"

RESERVE PauseModeFlag, 1

.popseg


.if ::USE_VANILLA_MUSIC
.include "vanilla_music.s"
.endif

.if ::USE_VANILLA_SFX
.include "vanilla_sfx.s"
.endif

.if ::USE_FAMISTUDIO_MUSIC
.include "driver_famistudio.s"
.endif

.if .not ::USE_VANILLA_MUSIC
.macro MusicPlayback
  BankPRGA #.bank(music_data)
  jsr CustomMusicEngine
  BankPRGA CurrentBank
.endmacro
.endif

.if .not ::USE_VANILLA_SFX
.macro SFXPlayback
  BankPRGA #.bank(music_data)
  jsr CustomSfxEngine
  BankPRGA CurrentBank
.endmacro
.endif

.proc AudioInit
  lda #0
  sta PauseModeFlag
  sta EventMusicBuffer
  sta AreaMusicBuffer
  sta Square1SoundBuffer
  sta Square2SoundBuffer
  sta NoiseSoundBuffer
  MusicInit
  SFXInit
  rts
.endproc

.proc AudioUpdate
  MusicPlayback
  SFXPlayback
.if ::USE_VANILLA_SFX && (!::USE_VANILLA_MUSIC)

  CustomMusicMixAudio
.endif
  rts
.endproc

.proc AudioClear
.if .not ::USE_VANILLA_MUSIC
  CustomMusicStop
.endif
  
.if ::USE_VANILLA_MUSIC
  ldy #MusicMemoryEnd - MusicMemoryStart
:   sta MusicMemoryStart,y     ;clear out memory used
    dey                   ;by the sound engines
    bpl :-
.endif
.if ::USE_VANILLA_SFX
  ldy #SFXMemoryEnd - SFXMemoryStart
:   sta SFXMemoryStart,y     ;clear out memory used
    dey                   ;by the sound engines
    bpl :-
.endif
  rts
.endproc

.if .not USE_VANILLA_MUSIC

; Data used to convert from the SMB1 queue to a regular driver

BrickBreak =  0
FireBreath =  1
Coin =        2
SpawnPup =    3
Vine =        4
Blast =       5
GrowUp =      6
OneUp =       7
BigJump =     8
Bump =        9
Stomp_Swim =  10
EnemySmack =  11
Injure_Pipe = 12
Fireball =    13
Flagpole =    14
SmallJump =   15
TimerTick =   16
BowserFall =  17
Pause =       18


Overworld = $00 ; : Overworld
Underworld = $01 ; : Underworld
Waterworld = $02 ; : Waterworld
Castleworld = $03 ; : Castleworld
Cloud = $04 ; : Cloud
EnterPipe = $05 ; : Enter in a pipe
Starman = $06 ; : Starman
Death = $07 ; : Death
GameOver = $08 ; : Game Over
SavedPrincess = $09 ; : You saved the princess
InAnotherCastle = $0a ; : In an other castle
Victory = $0b ; : Victory
HurryUp = $0c ; : Hurry up
Intermediate = $0d ; : Intermediate
SilenceTrack = -1


AreaMusicLUT:
  .byte SilenceTrack, Starman, EnterPipe, Cloud, SilenceTrack, Underworld, Waterworld, Overworld
EventMusicLUT:
  .byte SilenceTrack, Castleworld, Victory, Waterworld, InAnotherCastle, SavedPrincess, Intermediate, Death

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
  .byte Injure_Pipe     
  .byte EnemySmack 
  .byte Stomp_Swim 
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

; LeadingBitLookup:
; .repeat 256,I
; HI_BIT .set 8
; .repeat 8, J
; .if HI_BIT = 8 .and (I & (1 << (7 - J))) <> 0
; HI_BIT .set J
; .endif
; .endrepeat
;   .byte HI_BIT
; .endrepeat

.proc FindMostSigBit
  txa
  ldy #$ff
  sec
  :
    iny
    rol
    bcc :-
  rts
.endproc


.proc CustomMusicEngine
  lda PauseModeFlag         ;is sound already in pause mode?
  bne InPause
    lda PauseSoundQueue       ;if not, check pause sfx queue
    beq RunSoundSubroutines   ;if queue is empty, skip pause mode routine
InPause:

    ; Check if the Pause is just starting to change
    lda GamePauseTimer
    cmp #$2b
    bne SkipToUpdate
    lda PauseSoundQueue       ;check pause queue
    cmp #2
    beq UnPause
    lda #Pause
    sta PauseModeFlag         ;pause mode to interrupt game sounds
    
    ; CustomMusicPause Macro
    CustomMusicPause

    lda #0
    sta PauseSoundQueue

    jmp SkipToUpdate
UnPause:

  ; CustomMusicUnpause Macro
  CustomMusicUnpause

RunSoundSubroutines:
  lda EventMusicQueue
  ora AreaMusicQueue
  beq SkipToUpdate
    ldx AreaMusicQueue
    beq SkipAreaProcessing
      lda #0
      sta AreaMusicQueue
      stx AreaMusicBuffer
      jsr FindMostSigBit
      lda AreaMusicLUT, y
SkipAreaProcessing:
    ldx EventMusicQueue
    beq PlayNewSong
      ; Music from EventMusicQueue will override the AreaMusicQueue
      stx EventMusicBuffer
      lda #0
      sta EventMusicQueue
      jsr FindMostSigBit
      lda EventMusicLUT, y
PlayNewSong:
  cmp #SilenceTrack
  beq StopMusic
    ; Macro CustomMusicStop a
    CustomMusicPlay a
    jmp SkipToUpdate
StopMusic:
  ; Macro CustomMusicStop
  CustomMusicStop

  jmp SkipAreaProcessing

SkipToUpdate:

  ; Macro CustomMusicUpdate
  CustomMusicUpdate
  
  rts
.endproc

.if USE_CUSTOM_ENGINE_SFX
.proc CustomSfxEngine
  lda PauseModeFlag         ;is sound already in pause mode?
  bne InPause
    lda PauseSoundQueue       ;if not, check pause sfx queue
    beq RunSoundSubroutines   ;if queue is empty, skip pause mode routine
InPause:
    lda GamePauseTimer
    cmp #$2b
    jne SkipToUpdate
    lda PauseSoundQueue       ;check pause queue
    cmp #2
    beq UnPause
    lda #Pause
    sta PauseModeFlag         ;pause mode to interrupt game sounds
  
    ; CustomSFXPlay #Pause Macro
    CustomSFXPlay #Pause

    jmp SkipToUpdate
UnPause:
  lda #0
  sta PauseModeFlag
  sta PauseSoundQueue

  ; CustomSFXPlay #Pause Macro
  CustomSFXPlay #Pause
RunSoundSubroutines:
  ; Now check for sound effects
  ldx Square2SoundQueue
  beq :+
    jsr FindMostSigBit
    lda Sq2SfxTable,y
    ; CustomSFXPlay a, 0 Macro
    CustomSFXPlay a, 0
    ; ldx NextSFXChannel
    ; jsr famistudio_sfx_play
    ; jsr BumpSFXChannel
    lda #0
    sta Square2SoundQueue
  :
  ldx Square1SoundQueue
  beq :+
    jsr FindMostSigBit
    lda Sq1SfxTable,y
    ; CustomSFXPlay a, 1 Macro
    CustomSFXPlay a, 1
    ; ldx NextSFXChannel
    ; jsr famistudio_sfx_play
    ; jsr BumpSFXChannel
    lda #0
    sta Square1SoundQueue
  :
  ldx NoiseSoundQueue
  beq :+
    jsr FindMostSigBit
    lda NoiseSfxTable,y
    ; CustomSFXPlay a, 2 Macro
    CustomSFXPlay a, 2
    ; ldx NextSFXChannel
    ; jsr famistudio_sfx_play
    ; jsr BumpSFXChannel
    lda #0
    sta NoiseSoundQueue
  :
  
SkipToUpdate:

  CustomSFXUpdate
  
  rts
.endproc
.endif ; USE_CUSTOM_ENGINE_SFX

.endif

