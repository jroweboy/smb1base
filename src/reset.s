
.include "common.inc"
.include "smc.inc"

.import MoveAllSpritesOffscreen, InitializeNameTables, WritePPUReg1
.import OperModeExecutionTree, MoveSpritesOffscreen, UpdateTopScore
.import InitScroll, UpdateScreen, SoundEngine
.import TitleScreenIrq

;-------------------------------------------------------------------------------------
;INTERRUPT VECTORS
.import irq_sample_selfmod
.segment "VECTORS"
  .word (NonMaskableInterrupt)
  .word (Start)
  .word (irq_sample_selfmod)

.segment "FIXED"

;-------------------------------------------------------------------------------------
; .proc IrqStatusBar
;   pha
;     sta IRQDISABLE
;     lda IrqNewScroll  ; Combine bits 7-3 of new X with 2-0 of old X
;     eor IrqOldScroll
;     and #%11111000
;     eor IrqOldScroll
;     sta PPUSCROLL  ; Write old fine X and new coarse X
;     bit PPUSTATUS  ; Clear first/second write toggle
;     lda IrqNewScroll
;     sta IrqOldScroll
;     ; stall here if needed. Right now this is on a blank line so its unimportant to deal with.
;     sta PPUSCROLL  ; Write entire new X
;     bit PPUSTATUS  ; Clear first/second write toggle
;     ; Write nametable to PPUCTRL as well
;     lda IrqPPUCTRL
;     sta PPUCTRL
;   pla
;   rti
; .endproc

.proc load_playback_code
.import __PLAYBACK_CODE_LOAD__, __PLAYBACK_CODE_RUN__, __PLAYBACK_CODE_SIZE__
	ldx #<__PLAYBACK_CODE_SIZE__
	@loop:
		dex
		lda __PLAYBACK_CODE_LOAD__, x
		sta __PLAYBACK_CODE_RUN__, x
		cpx #00
		bne @loop
	rts

.assert __PLAYBACK_CODE_SIZE__ <= 256, error, "playback code is bigger than 256 bytes"
.endproc

.proc load_test_irq_code
.import __PLAYBACK_CODE_RUN__
  ldx #5
loop:
    lda TestIRQ, x
    sta __PLAYBACK_CODE_RUN__, x
    dex
    bpl loop
  rts
TestIRQ:
  .byte $8d, $10, $f0, $e6, tmp_irq_a, $40
.endproc

.export StartAudioIRQ
.proc StartAudioIRQ
  jsr CancelAudioIRQ
  
  lda #1
  sta PlayPanic
  sei
  jsr load_playback_code       ; copy playback code into RAM
  cli

  ; configure VRC7 IRQ
  lda #%00000111
	sta IRQCONTROL
; 127 clock cycles per sample = ~14093 Hz
; 162 clock cycles = ~ 11025 Hz
RELOAD_RATE = 162
	lda #(256 - RELOAD_RATE)
	sta irq_latch_value
	sta IRQLATCH
	sta IRQACK
  rts
.endproc

.export CancelAudioIRQ
.proc CancelAudioIRQ
  lda #0
  sta PlayPanic
  sta IRQCONTROL
  rts
.endproc

.export StartSwimmingIRQ
.proc StartSwimmingIRQ

.endproc

;;;;;;;;;;;;;;;;;;;;;;;;
; Delays X*256 clocks + overhead
; Clobbers X,Y. Preserves A. Relocatable.
; Time: X*256+16 clocks (including JSR)
;;;;;;;;;;;;;;;;;;;;;;;;
delay_256x_16_clocks:
	cpx #0
	bne delay_256x_11_clocks_
	rts
delay_256x_11_clocks_:
	;5 cycles done. Must consume 256 cycles; 251 cycles remain.
        pha                      ;3
        tya                      ;2
         ldy #46                 ;2
@l:      dey                     ;2*46
         bne @l                  ;3*46
         nop                     ;2-1
         nop                     ;2
        tay                      ;2
        pla                      ;4
	dex                      ;2
	jmp delay_256x_16_clocks ;3

.proc Start
  sei                          ;pretty standard 6502 type init here
  cld
  ; lda #%00010000               ;init PPU control register 1 
  lda #0
  sta IRQCONTROL

  lda #%00001000
  sta PPUCTRL
  ldx #$ff                     ;reset stack pointer
  txs
: lda PPUSTATUS               ;wait two frames
  bpl :-
: lda PPUSTATUS
  bpl :-
  ldy #ColdBootOffset          ;load default cold boot pointer
  ldx #$05                     ;this is where we check for a warm boot
WBootCheck:
  ; lda TopScoreDisplay,x        ;check each score digit in the top score
  ; cmp #10                      ;to see if we have a valid digit
  ; bcs ColdBoot                 ;if not, give up and proceed with cold boot
  ; dex                      
  ; bpl WBootCheck
  lda WarmBootValidation       ;second checkpoint, check to see if 
  cmp #$a5                     ;another location has a specific value
  bne ColdBoot
  ldy #<WarmBootOffset          ;if passed both, load warm boot pointer
ColdBoot:
  jsr InitializeMemory         ;clear memory using pointer in Y
  sta SND_DELTA_REG+1          ;reset delta counter load register
  sta OperMode                 ;reset primary mode of operation

  ; Clear the ssdpcm memory only on reset
  ldx #last_sample - idx_superblock - 1
  lda #0
  :
    sta idx_superblock,x
    dex
    bpl :-

.import load_next_superblock, decode_async, sblk_table
BankPRG8 #.bank(DECODE)
BankPRGC #.bank(sblk_table)    ; not needed since this is in fixed rom
	jsr load_next_superblock     ; load first superblock
	jsr decode_async         ; pre-fill the buffer with some samples
	jsr decode_async
	jsr decode_async
BankPRG8 #.bank(LOWCODE)

VRC7Init:
  ; setup the jmp instruction for the FarBank Target
  lda #$4c
  sta TargetAddrJmp
  ldx #0
  BankCHR0 x
  inx
  BankCHR4 x
  inx
  BankCHR8 x
  inx
  BankCHRC x
  inx
  BankCHR10 x
  inx
  BankCHR14 x
  inx
  BankCHR18 x
  inx
  BankCHR1C x


HORIZONTAL_MIRRORING = (1 << 0)
DISABLE_VRC7_CHANNELS = (1 << 6)
WRAM_ENABLE = (1 << 7)
  lda #WRAM_ENABLE
  sta NMT_MIRROR
  ; sta IRQENABLE
  lda #$ff
  sta APU_FRAMECOUNTER ; disable frame counter

  jsr StartAudioIRQ

; do a quick check that the IRQ works.
  lda #0
  sta tmp_irq_a
  jsr load_test_irq_code

  ; re-enable interrupts so the scanline irq can run
  cli

  ; check that it runs at 162 times in a frame
  ldx #116
  jsr delay_256x_16_clocks

  lda tmp_irq_a
  cmp #162
  bcs :+
BAD_EMULATOR:
    jmp BAD_EMULATOR
:

; set up the banks before starting anything else 

  BankPRG8 #.bank(MUSIC_ENGINE)
  ; initialize famistudio
.import CustomSoundInit
  
  jsr CustomSoundInit

  ; Now set the initial 8 bank
  BankPRG8 #.bank(LOWCODE)

  ; Now set the initial 8 bank
  BankPRGA #.bank(OBJECT)
  sta CurrentBank

FinializeMarioInit:
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
  ora #%10000000               ;enable NMIs
  jsr WritePPUReg1

  ; do a jsr to the main loop so we can profile it separately
  jsr IdleLoop
.endproc
.proc IdleLoop
  lda NmiDisable
  beq IdleLoop
  jsr GameLoop
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

; .export GraphicsBankInitValues
; GraphicsBankInitValues:
;   .byte $40, $42, $44, $45, $46, $47

.import PlayPanic

SMC_Import idx_smc_pcm_playback
.import fill_buffer
.proc NonMaskableInterrupt
  pha
  phx
  phy
  ; inc StatTimerLo
  ; bne @CheckIfNMIEnabled
  ;   inc StatTimerMd
  ;   bne @CheckIfNMIEnabled
  ;     inc StatTimerHi
@CheckIfNMIEnabled:
  bit NmiDisable
  bpl ContinueNMI
  lda PlayPanic
  beq :+
  BankPRG8 #.bank(DECODE)
    jsr fill_buffer
  BankPRG8 #.bank(LOWCODE)
:
    inc NmiSkipped
    ply
    plx
    pla
    rti
ContinueNMI:
  lda SwimmingFlag
  beq :+
    ; reinit the scanline irq
  :
  lda PlayPanic
  beq :+
    cli
  :

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
  jsr InitScroll
  ldx VRAM_Buffer_AddrCtrl  ;load control for pointer to buffer contents
  lda VRAM_AddrTable_Low,x  ;set indirect at $00 to pointer
  sta IrqR0
  lda VRAM_AddrTable_High,x
  sta IrqR1
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
  lda Mirror_PPUMASK       ;copy mirror of $2001 to register
  sta PPUMASK
  
  ; lda Sprite0HitDetectFlag  ;check for flag here
  ; beq :+
  ; jroweboy - set up Irq scroll split
  ; lda HorizontalScroll
  ; sta IrqNewScroll
  
	lda HorizontalScroll	;set scroll registers from variables
	sta PPUSCROLL
	lda VerticalScroll
	sta PPUSCROLL
  lda Mirror_PPUCTRL
  sta PPUCTRL

;   lda SwitchToMainIRQ
;   beq NoChangeToIRQ
;   bmi UseMainIRQ
;     ; Change back to title screen IRQ handler
;     lda #<TitleScreenIrq
;     sta IrqPointer
;     lda #>TitleScreenIrq
;     sta IrqPointer+1
;     lda #0
;     sta SwitchToMainIRQ
;     jmp NoChangeToIRQ
; UseMainIRQ:
;     ; Reload initial graphics and IRQ handler when switching to main gameplay
;     .import TitleScreenIrq
;     lda #<IrqStatusBar
;     sta IrqPointer
;     lda #>IrqStatusBar
;     sta IrqPointer+1

;     ldx #5
;     :
;       txa
;       ora PRG_FIXED_8
;       sta BANK_SELECT
;       lda GraphicsBankInitValues,x
;       sta BANK_DATA
;       dex
;       bpl :-
;     ; also switch the first sprite bank to the player sprite
;     BankCHR10 #0
;     lda #0
;     sta SwitchToMainIRQ
; NoChangeToIRQ:

  ; Setup the scanline to launch the IRQ at. This has to happen before the first scanline of the frame.
  ; lda OperMode
  ; bne :+
  ;   ; We are in the title screen, so initialize the IRQ
  ;   BankCHR0 #$48
  ;   BankCHR8 #$4a
  ;   lda #0
  ;   sta IrqNextScanline
  ;   ; start the TitleScreenIRQ
  ;   .import FIRST_SCANLINE_IRQ
  ;   lda #FIRST_SCANLINE_IRQ
  ;   sta IRQLATCH
  ;   sta IRQRELOAD
  ;   sta IRQENABLE
  ;   bne :++ ; unconditional
  ; :
  ;   ; otherwise we want to use the main IRQ
  ;   lda #32 - 1 ; do the split 32 scanlines from the top of the screen. -1 to give us time to set the new scroll
  ;   sta IRQLATCH
  ;   sta IRQRELOAD
  ;   sta IRQENABLE
  ; :


  ; lda #0
  ; sta OAMADDR               ;reset spr-ram address register
  ; jsr OAMandReadJoypad
  
    ; always run OAM even on lag frames
    lda #0
    sta OAMADDR               ;reset spr-ram address register
    lda #OAM
    sta OAM_DMA          ; ------ OAM DMA ------
    SMC_LoadLowByte idx_smc_pcm_playback, a
    clc
    adc #4 ; oam_dma_sample_skip_cnt    ; skip over lost samples during OAM DMA
    SMC_StoreLowByte idx_smc_pcm_playback, a
    
  jsr readjoy_safe

  jsr PauseRoutine          ;handle pause
  ; jsr UpdateTopScore

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
    ; AngularMomentumHandler:
    ;   lda AngularMomentum
    ;   beq NoDecTimers
    ;     ; dec AngularMomentumTimer
    ;     ; bpl NoDecTimers
    ;       ; cmp #0
    ;     lda PlayerAngle
    ;     clc
    ;     adc AngularMomentum
    ;     sta PlayerAngle
        ; lda #2
        ; sta AngularMomentumTimer
        ; sta AngularMomentum
  NoDecTimers:
    inc FrameCounter          ;increment frame counter
PauseSkip:
  ldx #$00
  ldy #$07
  lda PseudoRandomBitReg    ;get first memory location of LSFR bytes
  and #%00000010            ;mask out all but d1
  sta IrqR0                   ;save here
  lda PseudoRandomBitReg+1  ;get second memory location
  and #%00000010            ;mask out all but d1
  eor IrqR0                   ;perform exclusive-OR on d1 from first and second bytes
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
    lda GamePauseStatus       ;if in pause mode, do not bother with sprites at all
    lsr
    bcs SkipSprite0
      lda OperMode
      beq SkipSprite0
        jsr MoveAllSpritesOffscreen
        ; jsr SpriteShuffler
SkipSprite0:

  
  lda PlayPanic
  beq :+
  BankPRG8 #.bank(DECODE)
  .import fill_buffer
    jsr fill_buffer
  BankPRG8 #.bank(LOWCODE)
:
  ; copy the PPUCTRL flags to the version that we will write in the IRQ.
  ; this will restore the nmt select flags for the main screen in IRQ
  ; lda Mirror_PPUCTRL
  ; ; sta IrqPPUCTRL
  ; ; and also reset the flags for the HUD
  ; and #%11111100
  ; sta PPUCTRL

  ; lda OperMode
  ; bne :+
  ;   lda BhopInitalized
  ;   beq SkipMainOper
  ;   BankPRGA #.lobyte(.bank(TITLE_MUSIC))
  ;   .import bhop_play
  ;   jsr bhop_play
  ;   BankPRGA CurrentBank
  ;   jmp :++
  ; :
    ; Original SMB1 music engine
    BankPRG8 #.lobyte(.bank(MUSIC_ENGINE))
    BankPRGA #.lobyte(.bank(MUSIC))
.import DPCM_DATA
    BankPRGC #.lobyte(.bank(DPCM_DATA))
    ; jsr SoundEngine
    .import CustomSoundEngine
    jsr CustomSoundEngine
    BankPRG8 #.lobyte(.bank(LOWCODE))
    BankPRGA CurrentBank

  ; :
  ; lda BankShadow
  ; sta BANK_SELECT
  
SkipMainOper:
  ply
  plx
  pla
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
  .byte GLOBAL_BACKGROUND_COLOR, $30, $00, $10
  .byte $00

NightSnowPaletteData:
  .byte $3f, $00, $04
  .byte $0f, $30, $00, $10
  .byte $00

MushroomPaletteData:
  .byte $3f, $00, $04
  .byte GLOBAL_BACKGROUND_COLOR, $27, $16, $0f
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

.export InitializeMemory
; , InitializeMemoryRAMLo, InitializeMemoryRAMHi

.proc InitializeMemory
;$06 - RAM address low
;$07 - RAM address high
InitializeMemoryRAMLo = $06
InitializeMemoryRAMHi = $07
  ldx #$07          ;set initial high byte to $0700-$07ff
  lda #$00          ;set initial low byte to start of page (at $00 of page)
  sta InitializeMemoryRAMLo
InitPageLoop:
    stx InitializeMemoryRAMHi
InitByteLoop:
      cpx #0
      bne CheckStackPage
        cpy #<idx_superblock
        bcs SkipByte
CheckStackPage:
      cpx #$01          ;check to see if we're on the stack ($0100-$01ff)
      bne InitByte      ;if not, go ahead anyway
        cpy #<StackClear  ;otherwise, check to see if we're at $01?0-$01ff
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

;$00 - used for preset value
; .proc SpriteShuffler
;   lda #$28                    ;load preset value which will put it at
;   sta IrqR0                     ;sprite #10
;   ldx #$0e                    ;start at the end of OAM data offsets
; ShuffleLoop:
;     lda SprDataOffset,x         ;check for offset value against
;     cmp IrqR0                     ;the preset value
;     bcc NextSprOffset           ;if less, skip this part
;     ldy SprShuffleAmtOffset     ;get current offset to preset value we want to add
;     clc
;     adc SprShuffleAmt,y         ;get shuffle amount, add to current sprite offset
;     bcc StrSprOffset            ;if not exceeded $ff, skip second add
;     clc
;     adc IrqR0                     ;otherwise add preset value $28 to offset
; StrSprOffset:
;     sta SprDataOffset,x         ;store new offset here or old one if branched to here
; NextSprOffset: 
;     dex                         ;move backwards to next one
;     bpl ShuffleLoop
;   ldx SprShuffleAmtOffset     ;load offset
;   inx
;   cpx #$03                    ;check if offset + 1 goes to 3
;   bne SetAmtOffset            ;if offset + 1 not 3, store
;     ldx #$00                  ;otherwise, init to 0
; SetAmtOffset:
;   stx SprShuffleAmtOffset
;   ldx #$08                    ;load offsets for values and storage
;   ldy #$02
; SetMiscOffset:
;     lda SprDataOffset+5,y       ;load one of three OAM data offsets
;     sta Misc_SprDataOffset-2,x  ;store first one unmodified, but
;     clc                         ;add eight to the second and eight
;     adc #$08                    ;more to the third one
;     sta Misc_SprDataOffset-1,x  ;note that due to the way X is set up,
;     clc                         ;this code loads into the misc sprite offsets
;     adc #$08
;     sta Misc_SprDataOffset,x        
;     dex
;     dex
;     dex
;     dey
;     bpl SetMiscOffset           ;do this until all misc spr offsets are loaded
;   rts
; .endproc

; .proc SpriteShuffler
;   rts
; .endproc

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
ChkPauseTimer:
  lda GamePauseTimer     ;check if pause timer is still counting down
  beq ChkStart
    dec GamePauseTimer     ;if so, decrement and leave
    rts
ChkStart:
  lda SavedJoypad1Bits   ;check to see if start is pressed
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
ClrPauseTimer:
  lda GamePauseStatus    ;clear timer flag if timer is at zero and start button
  and #%01111111         ;is not pressed
SetPause:
  sta GamePauseStatus
ExitPause:
  rts
.endproc


.proc readjoy
    lda #$01
    ; While the strobe bit is set, buttons will be continuously reloaded.
    ; This means that reading from JOYPAD1 will only return the state of the
    ; first button: button A.
    sta JOYPAD_PORT1
    sta SavedJoypad1Bits
    lsr a        ; now A is 0
    ; By storing 0 into JOYPAD1, the strobe bit is cleared and the reloading stops.
    ; This allows all 8 buttons (newly reloaded) to be read from JOYPAD1.
    sta JOYPAD_PORT1
loop:
    lda JOYPAD_PORT1
    lsr a	       ; bit 0 -> Carry
    rol SavedJoypad1Bits  ; Carry -> bit 0; bit 7 -> Carry
    bcc loop
    rts
.endproc

.proc readjoy_safe
  jsr readjoy
reread:
  lda SavedJoypad1Bits
  pha
    jsr readjoy
  pla
  cmp SavedJoypad1Bits
  bne reread
  rts
.endproc

; .proc OAMandReadJoypad
;   lda #OAM
;   sta OAM_DMA          ; ------ OAM DMA ------
;   ldx #1             ; get put          <- strobe code must take an odd number of cycles total
;   stx SavedJoypad1Bits ; get put get
;   stx JOYPAD_PORT1   ; put get put get
;   dex                ; put get
;   stx JOYPAD_PORT1   ; put get put get
; read_loop:
;   lda JOYPAD_PORT2   ; put get put GET  <- loop code must take an even number of cycles total
;   and #3             ; put get
;   cmp #1             ; put get
;   rol SavedJoypad2Bits, x ; put get put get put get (X = 0; waste 1 cycle and 0 bytes for alignment)
;   lda JOYPAD_PORT1   ; put get put GET
;   and #3             ; put get
;   cmp #1             ; put get
;   rol SavedJoypad1Bits ; put get put get put
;   bcc read_loop      ; get put [get]    <- this branch must not be allowed to cross a page
; ASSERT_PAGE read_loop
;   rts
; .endproc
