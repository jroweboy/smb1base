
.segment "FIXED"


.pushseg
.segment "SHORTRAM"
RESERVE AreaMusicBuffer, 1
RESERVE Square1SoundBuffer, 1
RESERVE Square2SoundBuffer, 1
RESERVE NoiseSoundBuffer, 1

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
  jsr CustomMusicEngine
.endmacro
.endif

.if .not ::USE_VANILLA_SFX
.macro SFXPlayback
  jsr CustomSfxEngine
.endmacro
.endif


.proc InitAudio
  MusicInit
  SFXInit
  rts
.endproc

.proc ProcessAudio
  MusicPlayback
  SFXPlayback
  rts
.endproc

.if .not USE_VANILLA_MUSIC

; Data used to convert from the SMB1 queue to a regular driver

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


Castleworld = $00 ; : Castleworld
Cloud = $01 ; : Cloud
Death = $02 ; : Death
EnterPipe = $03 ; : Enter in a pipe
GameOver = $04 ; : Game Over
HurryUp = $05 ; : Hurry up
InAnotherCastle = $06 ; : In an other castle
Intermediate = $07 ; : Intermediate
Overworld = $08 ; : Overworld
Starman = $09 ; : Starman
Underworld = $0a ; : Underworld
Victory = $0b ; : Victory
Waterworld = $0c ; : Waterworld
SavedPrincess = $0d ; : You saved the princess
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
  ldy #8
  sec
  :
    dey
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

    ; SFXPlay #Pause Macro
    CustomSFXPlay #Pause

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

.endif

