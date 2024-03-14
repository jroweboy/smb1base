
.include "common.inc"


.import PlayerEndWorld, PrcNextA
.import MoveAllSpritesOffscreen, InitializeNameTables, WritePPUReg1
.import OperModeExecutionTree, UpdateTopScore
.import InitScroll, UpdateScreen, SoundEngine
.import FarCallInit

;-------------------------------------------------------------------------------------
;INTERRUPT VECTORS

.segment "VECTORS"
    .word (NonMaskableInterrupt)
    .word (Start)
    .word (IrqScrollSplit)  ;unused

.segment "FIXED"

.proc IrqScrollSplit
  pha
  phx
    MAPPER_IRQ_ACK
    ; 
    lda IrqPPUCTRL
    and #%00000001
    asl
    asl
    sta PPUADDR

    ; Y position to $2005.
    lda #32
    sta PPUSCROLL

    ; Prepare for the 2 later writes:
    ; We reuse new_x to hold (Y & $F8) << 2.
    and #%11111000
    asl
    asl
    ldx IrqNewScroll
    sta IrqNewScroll

    ; stall for 39 cpu cycles
    ; lda #$4a ;hides 'LSR A'
    ; bne *-1
    ; stall for 14 CPU cycles
    php
    plp
    php
    plp

    ; ((Y & $F8) << 2) | (X >> 3) in A for $2006 later.
    txa
    lsr
    lsr
    lsr
    ora IrqNewScroll


    ; The last two PPU writes must happen during hblank:
    stx PPUSCROLL
    sta PPUADDR

    ; Restore new_x.
    stx IrqNewScroll
  plx
  pla
  rti
.endproc

;-------------------------------------------------------------------------------------

.proc Start
  sei                          ;pretty standard 6502 type init here
  cld
  lda #%00010000               ;init PPU control register 1 
  sta PPUCTRL
  ldx #$ff                     ;reset stack pointer
  stx APU_FRAMECOUNTER         ; disable APU framecounter IRQ
  txs
: lda PPUSTATUS               ;wait two frames
  bpl :-
: lda PPUSTATUS
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
  ; for debugging, clear the sprite tile numbers so its easier to see in sprite viewers
  ldx #0
  lda #0
  :
    sta Sprite_Attributes,x
    sta Sprite_Tilenumber,x
    inx
    inx
    inx
    inx
    bne :-

  ; do mapper specific init
  jsr MapperInit
FinializeMarioInit:
  cli
  lda #$a5                     ;set warm boot flag
  sta WarmBootValidation     
  sta PseudoRandomBitReg       ;set seed for pseudorandom register
  lda #%00001111
  sta SND_MASTERCTRL_REG       ;enable all sound channels except dmc
  lda #%00000110
  sta PPUMASK            ;turn off clipping for OAM and background
  jsr MoveAllSpritesOffscreen
  jsr InitializeNameTables     ;initialize both name tables
  inc DisableScreenFlag        ;set flag to disable screen output
  lda Mirror_PPUCTRL
  ora #%10100000               ;enable NMIs and 8x16 sprites
  sta PPUCTRL              ;write contents of A to PPU register 1
  sta Mirror_PPUCTRL       ;and its mirror
  ; do a jsr to the main loop so we can profile it separately
  jsr IdleLoop
.endproc

.proc IdleLoop
  lda NmiDisable
  beq IdleLoop
; Detect if the last frame lagged and skip immediately to the next frame if we did so we don't
; slow down if we lag.
GoToNextFrameImmediately:
    lda NmiSkipped
    pha
      jsr GameLoop
    pla
    cmp NmiSkipped
    bne GoToNextFrameImmediately
  jmp IdleLoop
.endproc

.proc GameLoop
  lda GamePauseStatus       ;if in pause mode, do not perform operation mode stuff
  lsr
  bcs :+
    ; lda Mirror_PPUMASK
    ; ora #%00100000
    ; sta PPUMASK
    jsr OperModeExecutionTree ;otherwise do one of many, many possible subroutines
:
  ; lda Mirror_PPUMASK
  ; and #%11011111
  ; sta PPUMASK
  lda #0
  sta NmiDisable
  rts
.endproc

.proc NonMaskableInterrupt
  pha
  phx
  phy
  lda NmiDisable
  bpl ContinueNMI
    inc NmiSkipped
    ; lag frame, prevent the graphics from going bunk by still running
    ; the irq. also run audio to keep it sounding like we didn't lag
    SetScanlineIRQ #$1f
    lda Mirror_PPUCTRL
    and #%11111110            ;alter name table address to be $2800
    sta PPUCTRL              ;(essentially $2000) but save other bits
    lda #0
    sta PPUSCROLL
    sta PPUSCROLL
    lda CurrentBank
    pha
      BankPRGA #.bank(MUSIC)
      jsr SoundEngine
    ;   lda #7 | PRG_FIXED_8
    ;   sta BANK_SELECT
    ; pla
    ; sta BANK_DATA
    ; lda BankShadow
    ; sta BANK_SELECT
    pla
    BankPRGA a

    ply
    plx
    pla
    rti
ContinueNMI:
  ; jroweboy disable NMI with a soft disable instead of turning off the NMI source from PPU
  dec NmiDisable
  
  ; jroweboy switch the nametable back to nmt0 and force NMI to be enabled
  lda Mirror_PPUCTRL
  and #%11111110            ;alter name table address to be $2800
  sta PPUCTRL              ;(essentially $2000) but save other bits
  lda Mirror_PPUMASK       ;disable OAM and background display by default
  and #%11100110
  ldy DisableScreenFlag     ;get screen disable flag
  bne ScreenOff             ;if set, used bits as-is
    lda Mirror_PPUMASK     ;otherwise reenable bits and save them
    ora #%00011110
ScreenOff:
  sta Mirror_PPUMASK       ;save bits for later but not in register at the moment
  and #%11100111            ;disable screen for now
  sta PPUMASK
  ldx PPUSTATUS            ;reset flip-flop and reset scroll registers to zero
  lda #$00
  sta PPUSCROLL
  sta PPUSCROLL
  sta OAMADDR          ;reset spr-ram address register

  ldx VRAM_Buffer_AddrCtrl  ;load control for pointer to buffer contents
  lda VRAM_AddrTable_Low,x  ;set indirect at $00 to pointer
  sta NmiR0
  lda VRAM_AddrTable_High,x
  sta NmiR1
  jsr UpdateScreen          ;update screen with buffer contents

  jsr OAMandReadJoypad
  lda ReloadCHRBank
  beq :+
    .repeat 12, I
      lda CurrentCHRBank + I
      sta MMC5_CHR_BANK_BASE + I
    .endrepeat
  ;   ldx #PRG_FIXED_8
  ; .repeat 6, I
  ;   stx BANK_SELECT
  ;   lda CurrentCHRBank + I
  ;   sta BANK_DATA
  ; .if I <> 5
  ;   inx
  ; .endif
  ; .endrepeat
    ldx #0
    stx ReloadCHRBank
  :

  
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
  lda Mirror_PPUMASK       ;copy mirror of $2001 to register
  sta PPUMASK
  

  lda HorizontalScroll
  sta IrqNewScroll
  ; lda Mirror_PPUCTRL
  ; sta IrqScrollBit
  lda Sprite0HitDetectFlag  ;check for flag here
  beq SkipSprite0
  
    SetScanlineIRQ #$1f
    ; cli just in case NMI runs late
    cli
    
    ; lda GamePauseStatus       ;if in pause mode, do not bother with sprites at all
    ; lsr
    ; bcs SkipSprite0
    ;   jsr MoveAllSpritesOffscreen
SkipSprite0:
  lda Mirror_PPUCTRL
  sta IrqPPUCTRL
  ; and also reset the flags for the HUD
  and #%11111100
  sta PPUCTRL

  ; play sound
  ; lda CurrentBank
  ; pha
  ;   BankPRGA #.bank(MUSIC)
  ;   jsr SoundEngine
  ;   lda #7 | PRG_FIXED_8
  ;   sta BANK_SELECT
  ; pla
  ; sta BANK_DATA

  BankPRGA #.bank(MUSIC)
  jsr SoundEngine
  BankPRGA CurrentBank
  
.ifdef WORLD_HAX
	dec DebugCooldown
	bpl :++
	inc DebugCooldown
	lda SavedJoypadBits
	and #Select_Button
	beq :++
  lda #10
	sta DebugCooldown
  lda SavedJoypadBits
	and #B_Button
  beq :+
  ; TODO don't farcall in nmi
  farcall PrcNextA
  jmp :++
  :
	jsr PlayerEndWorld
	:
.endif

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
      ldx #FRAME_TIMER_COUNT    ;load end offset for end of frame timers
      dec IntervalTimerControl  ;decrement interval timer control,
      bpl DecTimersLoop         ;if not expired, only frame timers will decrement
      lda #$14
      sta IntervalTimerControl  ;if control for interval timers expired,
      ldx #ALL_TIMER_COUNT      ;interval timers will decrement along with frame timers
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
  sta NmiR0                   ;save here
  lda PseudoRandomBitReg+1  ;get second memory location
  and #%00000010            ;mask out all but d1
  eor NmiR0                   ;perform exclusive-OR on d1 from first and second bytes
  clc                       ;if neither or both are set, carry will be clear
  beq RotPRandomBit
  sec                       ;if one or the other is set, carry will be set
RotPRandomBit:
    ror PseudoRandomBitReg,x  ;rotate carry into d7, and rotate last bit into carry
    inx                       ;increment to next byte
    dey                       ;decrement for loop
    bne RotPRandomBit
SkipMainOper:

  ; lda BankShadow
  ; sta BANK_SELECT
  ply
  plx
  pla
  rti                       ;we are done until the next frame!

;-------------------------------------------------------------------------------------
;$00 - vram buffer address table low, also used for pseudorandom bit
;$01 - vram buffer address table high

.define VRAM_AddrTable \
      VRAM_Buffer1, \
      WaterPaletteData, \
      GroundPaletteData, \
      UndergroundPaletteData, \
      CastlePaletteData, \
      VRAM_Buffer1_Offset, \
      VRAM_Buffer2, \
      VRAM_Buffer2, \
      BowserPaletteData, \
      DaySnowPaletteData, \
      NightSnowPaletteData, \
      MushroomPaletteData, \
      MarioThanksMessage, \
      LuigiThanksMessage, \
      MushroomRetainerSaved, \
      PrincessSaved1, \
      PrincessSaved2, \
      WorldSelectMessage1, \
      WorldSelectMessage2

VRAM_AddrTable_Low: .lobytes VRAM_AddrTable
VRAM_AddrTable_High: .hibytes VRAM_AddrTable

VRAM_Buffer_Offset:
  .byte VRAM_Buffer1_PtrOffset, VRAM_Buffer2_PtrOffset

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

;-------------------------------------------------------------------------------------

;$06 - RAM address low
;$07 - RAM address high
InitializeMemoryRAMLo = $06
InitializeMemoryRAMHi = $07
.export InitializeMemory, InitializeMemoryRAMLo, InitializeMemoryRAMHi
.proc InitializeMemory
  ldx #$07          ;set initial high byte to $0700-$07ff
  lda #$00          ;set initial low byte to start of page (at $00 of page)
  sta InitializeMemoryRAMLo
InitPageLoop:
    stx InitializeMemoryRAMHi
InitByteLoop:
      cpx #2            ; jroweboy: don't clear sprite ram
      beq SkipByte
      cpx #$01          ;check to see if we're on the stack ($0100-$01ff)
      bne InitByte      ;if not, go ahead anyway
      cpy #<StackClear  ;otherwise, check to see if we're at $0160-$01ff
      bcs SkipByte      ;if so, skip write
InitByte:
      sta (InitializeMemoryRAMLo),y       ;otherwise, initialize byte with current low byte in Y
SkipByte:
      dey
      cpy #$ff          ;do this until all bytes in page have been erased
      bne InitByteLoop
    dex               ;go onto the next page
    bpl InitPageLoop  ;do this until all pages of memory have been erased
  rts
.endproc


;-------------------------------------------------------------------------------------
.proc PauseRoutine
               lda OperMode           ;are we in victory mode?
               cmp #MODE_VICTORY  ;if so, go ahead
               beq ChkPauseTimer
               cmp #MODE_GAMEPLAY     ;are we in game mode?
               bne ExitPause          ;if not, leave
               lda OperMode_Task      ;if we are in game mode, are we running game engine?
               cmp #$03
               bne ExitPause          ;if not, leave
ChkPauseTimer: lda GamePauseTimer     ;check if pause timer is still counting down
               beq ChkStart
               dec GamePauseTimer     ;if so, decrement and leave
               rts
ChkStart:      lda SavedJoypad1Bits   ;check to see if start is pressed
               and #Start_Button      ;on controller 1
               beq ClrPauseTimer
               lda GamePauseStatus    ;check to see if timer flag is set
               and #%10000000         ;and if so, do not reset timer
               bne ExitPause
               lda #$2b               ;set pause timer
               sta GamePauseTimer
               lda GamePauseStatus
               tay
               iny                    ;set pause sfx queue for next pause mode
               sty PauseSoundQueue
               eor #%00000001         ;invert d0 and set d7
               ora #%10000000
               bne SetPause           ;unconditional branch
ClrPauseTimer: lda GamePauseStatus    ;clear timer flag if timer is at zero and start button
               and #%01111111         ;is not pressed
SetPause:      sta GamePauseStatus
ExitPause:     rts
.endproc


;;;;;;;;----------------------------------------
.segment "OAMALIGNED"
.proc OAMandReadJoypad
  lda #OAM
  sta OAM_DMA          ; ------ OAM DMA ------
  ldx #1             ; get put          <- strobe code must take an odd number of cycles total
  stx SavedJoypad1Bits ; get put get
  stx JOYPAD_PORT1   ; put get put get
  dex                ; put get
  stx JOYPAD_PORT1   ; put get put get
read_loop:
  lda JOYPAD_PORT2   ; put get put GET  <- loop code must take an even number of cycles total
  and #3             ; put get
  cmp #1             ; put get
  rol SavedJoypad2Bits, x ; put get put get put get (X = 0; waste 1 cycle and 0 bytes for alignment)
  lda JOYPAD_PORT1   ; put get put GET
  and #3             ; put get
  cmp #1             ; put get
  rol SavedJoypad1Bits ; put get put get put
  bcc read_loop      ; get put [get]    <- this branch must not be allowed to cross a page
ASSERT_PAGE read_loop
  rts
.endproc
