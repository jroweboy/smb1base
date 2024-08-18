
.segment "FIXED"

.pushseg
.segment "SHORTRAM"
RESERVE MusicLooped, 1
.popseg

.macro MusicInit
  DriverMusicInit
  lda #0
  sta MusicLooped
.endmacro

.macro MusicClear
  DriverMusicStop
.endmacro

.macro MusicPlayback
  BankPRGA #.bank(music_data)
  CustomMusicEngine
.endmacro

.if .not ::USE_VANILLA_SFX
.macro SFXPlayback
  BankPRGA #.bank(sfx_data)
  CustomSfxEngine
.endmacro
.endif


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
  .byte SilenceTrack, Starman, EnterPipe, Cloud, Castleworld, Underworld, Waterworld, Overworld
EventMusicLUT:
  .byte SilenceTrack, HurryUp, Victory, Waterworld, InAnotherCastle, SavedPrincess, GameOver, Death

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


.macro CustomMusicEngine
.scope
  lda PauseModeFlag         ;is sound already in pause mode?
  bne InPause
    lda PauseSoundQueue       ;if not, check pause sfx queue
    beq RunSoundSubroutines   ;if queue is empty, skip pause mode routine
InPause:

  ; Check if the Pause is just starting to change
  lda GamePauseTimer
  cmp #$2b
  jne SkipToUpdate
    lda PauseSoundQueue       ;check pause queue
    cmp #2
    beq UnPause
      lda #Pause
      sta PauseModeFlag         ;pause mode to interrupt game sounds
      
      ; DriverMusicPause Macro
      DriverMusicPause

      lda #0
      sta PauseSoundQueue
      jmp SkipToUpdate
UnPause:
  ; DriverMusicUnpause Macro
  DriverMusicUnpause
  jmp RunSoundSubroutines

MusicLoopBack:
  lda #0
  sta EventMusicBuffer
  ldx AreaMusicBuffer_Alt
  stx AreaMusicBuffer
  jmp FindAreaMusic

RunSoundSubroutines:
  ; First check if the music has just looped
  lda MusicLooped
  beq NotTRO
    lda #0
    sta MusicLooped
    lda EventMusicBuffer     ;check secondary buffer for time running out music
    cmp #TimeRunningOutMusic
    bne NotTRO
      ; It was a hurry up time warning, so restart the alt music
      lda AreaMusicBuffer_Alt  ;load previously saved contents of primary buffer
      bne MusicLoopBack        ;and start playing the song again if there is one
NotTRO:
  lda EventMusicQueue
  ora AreaMusicQueue
  beq SkipToUpdate
    ldx AreaMusicQueue
    beq SkipAreaProcessing
      lda #0
      sta AreaMusicQueue
      stx AreaMusicBuffer
FindAreaMusic:
      jsr FindMostSigBit
      lda AreaMusicLUT, y
SkipAreaProcessing:
    ldx EventMusicQueue
    beq PlayNewSong
      lda AreaMusicBuffer
      sta AreaMusicBuffer_Alt   ;save current area music buffer to be re-obtained later
      ; Music from EventMusicQueue will override the AreaMusicQueue
      stx EventMusicBuffer
      lda #0
      sta AreaMusicBuffer
      sta EventMusicQueue
      jsr FindMostSigBit
      lda EventMusicLUT, y
PlayNewSong:
  cmp #SilenceTrack
  beq StopMusic
    ; pha 
    ;   ; Macro DriverMusicStop
    ;   DriverMusicStop

    ;   ; Macro DriverMusicUpdate
    ;   DriverMusicUpdate
    ; pla
    ; Macro DriverMusicPlay a
    DriverMusicPlay a
    jmp SkipToUpdate
StopMusic:
  ; Macro DriverMusicStop
  DriverMusicStop

;   jmp SkipAreaProcessing

SkipToUpdate:

  ; Macro DriverMusicUpdate
  DriverMusicUpdate

.endscope
.endmacro

.if USE_CUSTOM_ENGINE_SFX
.macro CustomSfxEngine
.scope
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
  
    ; DriverSFXPlay #Pause Macro
    DriverSFXPlay #Pause

    jmp SkipToUpdate
UnPause:
  lda #0
  sta PauseModeFlag
  sta PauseSoundQueue

  ; DriverSFXPlay #Pause Macro
  DriverSFXPlay #Pause
RunSoundSubroutines:
  ; Now check for sound effects
  ldx Square2SoundQueue
  beq :+
    jsr FindMostSigBit
    lda Sq2SfxTable,y
    ; DriverSFXPlay a, 0 Macro
    DriverSFXPlay a, 0
    lda #0
    sta Square2SoundQueue
  :
  ldx Square1SoundQueue
  beq :+
    jsr FindMostSigBit
    lda Sq1SfxTable,y
    ; DriverSFXPlay a, 1 Macro
    DriverSFXPlay a, 1
    lda #0
    sta Square1SoundQueue
  :
  ldx NoiseSoundQueue
  beq :+
    jsr FindMostSigBit
    lda NoiseSfxTable,y
    ; DriverSFXPlay a, 2 Macro
    DriverSFXPlay a, 2
    lda #0
    sta NoiseSoundQueue
  :
  
SkipToUpdate:
  DriverSFXUpdate

.endscope
.endmacro
.endif ; USE_CUSTOM_ENGINE_SFX
