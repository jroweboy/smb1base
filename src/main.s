
.include "common.inc"

;-------------------------------------------------------------------------------------
;INTERRUPT VECTORS

.segment "VECTORS"
    .word (NonMaskableInterrupt)
    .word (Start)
    .word ($fff0)  ;unused

.segment "CODE"

.proc Start
.import InitializeMemory, MoveAllSpritesOffscreen, InitializeNameTables, WritePPUReg1
  sei                          ;pretty standard 6502 type init here
  cld
  lda #%00010000               ;init PPU control register 1 
  sta PPU_CTRL_REG1
  ldx #$ff                     ;reset stack pointer
  txs
: lda PPU_STATUS               ;wait two frames
  bpl :-
: lda PPU_STATUS
  bpl :-
  ldy #ColdBootOffset          ;load default cold boot pointer
  ldx #$05                     ;this is where we check for a warm boot
WBootCheck:
  lda TopScoreDisplay,x        ;check each score digit in the top score
  cmp #10                      ;to see if we have a valid digit
  bcs ColdBoot                 ;if not, give up and proceed with cold boot
  dex                      
  bpl WBootCheck
  lda WarmBootValidation       ;second checkpoint, check to see if 
  cmp #$a5                     ;another location has a specific value
  bne ColdBoot   
  ldy #WarmBootOffset          ;if passed both, load warm boot pointer
ColdBoot:
  jsr InitializeMemory         ;clear memory using pointer in Y
  sta SND_DELTA_REG+1          ;reset delta counter load register
  sta OperMode                 ;reset primary mode of operation
  lda #$a5                     ;set warm boot flag
  sta WarmBootValidation     
  sta PseudoRandomBitReg       ;set seed for pseudorandom register
  lda #%00001111
  sta SND_MASTERCTRL_REG       ;enable all sound channels except dmc
  lda #%00000110
  sta PPU_CTRL_REG2            ;turn off clipping for OAM and background
  jsr MoveAllSpritesOffscreen
  jsr InitializeNameTables     ;initialize both name tables
  inc DisableScreenFlag        ;set flag to disable screen output
  lda Mirror_PPU_CTRL_REG1
  ora #%10000000               ;enable NMIs
  jsr WritePPUReg1
EndlessLoop:
  jmp EndlessLoop              ;endless loop, need I say more?
.endproc

.proc NonMaskableInterrupt

.import OperModeExecutionTree, SpriteShuffler, MoveSpritesOffscreen, UpdateTopScore
.import InitScroll, UpdateScreen, SoundEngine, ReadJoypads, PauseRoutine

  lda Mirror_PPU_CTRL_REG1  ;disable NMIs in mirror reg
  and #%01111111            ;save all other bits
  sta Mirror_PPU_CTRL_REG1
  and #%01111110            ;alter name table address to be $2800
  sta PPU_CTRL_REG1         ;(essentially $2000) but save other bits
  lda Mirror_PPU_CTRL_REG2  ;disable OAM and background display by default
  and #%11100110
  ldy DisableScreenFlag     ;get screen disable flag
  bne ScreenOff             ;if set, used bits as-is
    lda Mirror_PPU_CTRL_REG2  ;otherwise reenable bits and save them
    ora #%00011110
ScreenOff:
  sta Mirror_PPU_CTRL_REG2  ;save bits for later but not in register at the moment
  and #%11100111            ;disable screen for now
  sta PPU_CTRL_REG2
  ldx PPU_STATUS            ;reset flip-flop and reset scroll registers to zero
  lda #$00
  jsr InitScroll
  sta PPU_SPR_ADDR          ;reset spr-ram address register
  lda #$02                  ;perform spr-ram DMA access on $0200-$02ff
  sta SPR_DMA
  ldx VRAM_Buffer_AddrCtrl  ;load control for pointer to buffer contents
  lda VRAM_AddrTable_Low,x  ;set indirect at $00 to pointer
  sta $00
  lda VRAM_AddrTable_High,x
  sta $01
  jsr UpdateScreen          ;update screen with buffer contents
  ldy #$00
  ldx VRAM_Buffer_AddrCtrl  ;check for usage of $0341
  cpx #$06
  bne InitBuffer
    iny                       ;get offset based on usage
InitBuffer:
  ldx VRAM_Buffer_Offset,y
  lda #$00                  ;clear buffer header at last location
  sta VRAM_Buffer1_Offset,x        
  sta VRAM_Buffer1,x
  sta VRAM_Buffer_AddrCtrl  ;reinit address control to $0301
  lda Mirror_PPU_CTRL_REG2  ;copy mirror of $2001 to register
  sta PPU_CTRL_REG2
  jsr SoundEngine           ;play sound
  jsr ReadJoypads           ;read joypads
  jsr PauseRoutine          ;handle pause
  jsr UpdateTopScore
  lda GamePauseStatus       ;check for pause status
  lsr
  bcs PauseSkip
  lda TimerControl          ;if master timer control not set, decrement
  beq DecTimers             ;all frame and interval timers
    dec TimerControl
  bne NoDecTimers
  DecTimers:
    ldx #$14                  ;load end offset for end of frame timers
    dec IntervalTimerControl  ;decrement interval timer control,
    bpl DecTimersLoop         ;if not expired, only frame timers will decrement
    lda #$14
    sta IntervalTimerControl  ;if control for interval timers expired,
    ldx #$23                  ;interval timers will decrement along with frame timers
  DecTimersLoop:
      lda Timers,x              ;check current timer
      beq SkipExpTimer          ;if current timer expired, branch to skip,
        dec Timers,x              ;otherwise decrement the current timer
    SkipExpTimer:
      dex                       ;move onto next timer
      bpl DecTimersLoop         ;do this until all timers are dealt with
NoDecTimers:
  inc FrameCounter          ;increment frame counter
PauseSkip:
  ldx #$00
  ldy #$07
  lda PseudoRandomBitReg    ;get first memory location of LSFR bytes
  and #%00000010            ;mask out all but d1
  sta $00                   ;save here
  lda PseudoRandomBitReg+1  ;get second memory location
  and #%00000010            ;mask out all but d1
  eor $00                   ;perform exclusive-OR on d1 from first and second bytes
  clc                       ;if neither or both are set, carry will be clear
  beq RotPRandomBit
  sec                       ;if one or the other is set, carry will be set
RotPRandomBit:
    ror PseudoRandomBitReg,x  ;rotate carry into d7, and rotate last bit into carry
    inx                       ;increment to next byte
    dey                       ;decrement for loop
    bne RotPRandomBit
  lda Sprite0HitDetectFlag  ;check for flag here
  beq SkipSprite0
Sprite0Clr:
    lda PPU_STATUS            ;wait for sprite 0 flag to clear, which will
    and #%01000000            ;not happen until vblank has ended
    bne Sprite0Clr
  lda GamePauseStatus       ;if in pause mode, do not bother with sprites at all
  lsr
  bcs Sprite0Hit
  jsr MoveSpritesOffscreen
  jsr SpriteShuffler
Sprite0Hit:
    lda PPU_STATUS            ;do sprite #0 hit detection
    and #%01000000
    beq Sprite0Hit
  ldy #$14                  ;small delay, to wait until we hit horizontal blank time
HBlankDelay:
    dey
    bne HBlankDelay
SkipSprite0:
  lda HorizontalScroll      ;set scroll registers from variables
  sta PPU_SCROLL_REG
  lda VerticalScroll
  sta PPU_SCROLL_REG
  lda Mirror_PPU_CTRL_REG1  ;load saved mirror of $2000
  pha
    sta PPU_CTRL_REG1
    lda GamePauseStatus       ;if in pause mode, do not perform operation mode stuff
    lsr
    bcs SkipMainOper
    jsr OperModeExecutionTree ;otherwise do one of many, many possible subroutines
SkipMainOper:
    lda PPU_STATUS            ;reset flip-flop
  pla
  ora #%10000000            ;reactivate NMIs
  sta PPU_CTRL_REG1
  rti                       ;we are done until the next frame!

;-------------------------------------------------------------------------------------
;$00 - vram buffer address table low, also used for pseudorandom bit
;$01 - vram buffer address table high

.define VRAM_AddrTable \
      VRAM_Buffer1, WaterPaletteData, GroundPaletteData, \
      UndergroundPaletteData, CastlePaletteData, VRAM_Buffer1_Offset, \
      VRAM_Buffer2, VRAM_Buffer2, BowserPaletteData, \
      DaySnowPaletteData, NightSnowPaletteData, MushroomPaletteData, \
      MarioThanksMessage, LuigiThanksMessage, MushroomRetainerSaved, \
      PrincessSaved1, PrincessSaved2, WorldSelectMessage1, \
      WorldSelectMessage2

VRAM_AddrTable_Low: .lobytes VRAM_AddrTable
VRAM_AddrTable_High: .hibytes VRAM_AddrTable

VRAM_Buffer_Offset:
      .byte <VRAM_Buffer1_Offset, <VRAM_Buffer2_Offset

;-------------------------------------------------------------------------------------
;VRAM BUFFER DATA FOR LOCATIONS IN PRG-ROM

WaterPaletteData:
  .byte $3f, $00, $20
  .byte $0f, $15, $12, $25  
  .byte $0f, $3a, $1a, $0f
  .byte $0f, $30, $12, $0f
  .byte $0f, $27, $12, $0f
  .byte $22, $16, $27, $18
  .byte $0f, $10, $30, $27
  .byte $0f, $16, $30, $27
  .byte $0f, $0f, $30, $10
  .byte $00

GroundPaletteData:
  .byte $3f, $00, $20
  .byte $0f, $29, $1a, $0f
  .byte $0f, $36, $17, $0f
  .byte $0f, $30, $21, $0f
  .byte $0f, $27, $17, $0f
  .byte $0f, $16, $27, $18
  .byte $0f, $1a, $30, $27
  .byte $0f, $16, $30, $27
  .byte $0f, $0f, $36, $17
  .byte $00

UndergroundPaletteData:
  .byte $3f, $00, $20
  .byte $0f, $29, $1a, $09
  .byte $0f, $3c, $1c, $0f
  .byte $0f, $30, $21, $1c
  .byte $0f, $27, $17, $1c
  .byte $0f, $16, $27, $18
  .byte $0f, $1c, $36, $17
  .byte $0f, $16, $30, $27
  .byte $0f, $0c, $3c, $1c
  .byte $00

CastlePaletteData:
  .byte $3f, $00, $20
  .byte $0f, $30, $10, $00
  .byte $0f, $30, $10, $00
  .byte $0f, $30, $16, $00
  .byte $0f, $27, $17, $00
  .byte $0f, $16, $27, $18
  .byte $0f, $1c, $36, $17
  .byte $0f, $16, $30, $27
  .byte $0f, $00, $30, $10
  .byte $00

DaySnowPaletteData:
  .byte $3f, $00, $04
  .byte $22, $30, $00, $10
  .byte $00

NightSnowPaletteData:
  .byte $3f, $00, $04
  .byte $0f, $30, $00, $10
  .byte $00

MushroomPaletteData:
  .byte $3f, $00, $04
  .byte $22, $27, $16, $0f
  .byte $00

BowserPaletteData:
  .byte $3f, $14, $04
  .byte $0f, $1a, $30, $27
  .byte $00

MarioThanksMessage:
  .byte $25, $48, $10
  .byte "THANK YOU MARIO!"
  .byte $00

LuigiThanksMessage:
  .byte $25, $48, $10
  .byte "THANK YOU LUIGI!"
  .byte $00

MushroomRetainerSaved:
  .byte $25, $c5, $16
  .byte "BUT OUR PRINCESS IS IN"
  .byte $26, $05, $0f
  .byte "ANOTHER CASTLE!"
  .byte $00

PrincessSaved1:
  .byte $25, $a7, $13
  .byte "YOUR QUEST IS OVER."
  .byte $00

PrincessSaved2:
  .byte $25, $e3, $1b
  .byte "WE PRESENT YOU A NEW QUEST."
  .byte $00

WorldSelectMessage1:
  .byte $26, $4a, $0d
  .byte "PUSH BUTTON B"
  .byte $00

WorldSelectMessage2:
  .byte $26, $88, $11
  .byte "TO SELECT A WORLD"
  .byte $00

.endproc
