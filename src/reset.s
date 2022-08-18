
.include "common.inc"



.import OperModeExecutionTree, MoveSpritesOffscreen, UpdateTopScore
.import InitScroll, UpdateScreen, SoundEngine, PauseRoutine

;-------------------------------------------------------------------------------------
;INTERRUPT VECTORS

.segment "VECTORS"
    .word (NonMaskableInterrupt)
    .word (Start)
    .word ($fff0)  ;unused

.segment "FIXED"

;-------------------------------------------------------------------------------------

.proc Start
.import MoveAllSpritesOffscreen, InitializeNameTables, WritePPUReg1
  sei                          ;pretty standard 6502 type init here
  cld
  lda #%00010000               ;init PPU control register 1 
  sta PPU_CTRL
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
  sta PPU_MASK            ;turn off clipping for OAM and background
  jsr MoveAllSpritesOffscreen
  jsr InitializeNameTables     ;initialize both name tables
  inc DisableScreenFlag        ;set flag to disable screen output
  lda Mirror_PPU_CTRL
  ora #%10000000               ;enable NMIs
  jsr WritePPUReg1
EndlessLoop:
  jmp EndlessLoop              ;endless loop, need I say more?
.endproc

.proc NonMaskableInterrupt

  lda Mirror_PPU_CTRL       ;disable NMIs in mirror reg
  and #%01111111            ;save all other bits
  sta Mirror_PPU_CTRL
  and #%01111110            ;alter name table address to be $2800
  sta PPU_CTRL         ;(essentially $2000) but save other bits
  lda Mirror_PPU_MASK       ;disable OAM and background display by default
  and #%11100110
  ldy DisableScreenFlag     ;get screen disable flag
  bne ScreenOff             ;if set, used bits as-is
    lda Mirror_PPU_MASK     ;otherwise reenable bits and save them
    ora #%00011110
ScreenOff:
  sta Mirror_PPU_MASK       ;save bits for later but not in register at the moment
  and #%11100111            ;disable screen for now
  sta PPU_MASK
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
  lda Mirror_PPU_MASK       ;copy mirror of $2001 to register
  sta PPU_MASK
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
  lda Mirror_PPU_CTRL       ;load saved mirror of $2000
  pha
    sta PPU_CTRL
    lda GamePauseStatus       ;if in pause mode, do not perform operation mode stuff
    lsr
    bcs SkipMainOper
    jsr OperModeExecutionTree ;otherwise do one of many, many possible subroutines
SkipMainOper:
    lda PPU_STATUS            ;reset flip-flop
  pla
  ora #%10000000            ;reactivate NMIs
  sta PPU_CTRL
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
      cpx #$01          ;check to see if we're on the stack ($0100-$01ff)
      bne InitByte      ;if not, go ahead anyway
      cpy #$60          ;otherwise, check to see if we're at $0160-$01ff
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
;$00 - temp joypad bit
ReadJoypads: 
  lda #$01               ;reset and clear strobe of joypad ports
  sta JOYPAD_PORT
  lsr
  tax                    ;start with joypad 1's port
  sta JOYPAD_PORT
  jsr ReadPortBits
  inx                    ;increment for joypad 2's port
ReadPortBits:
  ldy #$08
PortLoop:
  pha                    ;push previous bit onto stack
    lda JOYPAD_PORT,x      ;read current bit on joypad port
    sta $00                ;check d1 and d0 of port output
    lsr                    ;this is necessary on the old
    ora $00                ;famicom systems in japan
    lsr
    pla                    ;read bits from stack
    rol                    ;rotate bit from carry flag
    dey
    bne PortLoop           ;count down bits left
    sta SavedJoypadBits,x  ;save controller status here always
    pha
      and #%00110000         ;check for select or start
      and JoypadBitMask,x    ;if neither saved state nor current state
      beq Save8Bits          ;have any of these two set, branch
    pla
    and #%11001111         ;otherwise store without select
    sta SavedJoypadBits,x  ;or start bits and leave
    rts
Save8Bits:
  pla
  sta JoypadBitMask,x    ;save with all bits in another place and leave
  rts

;-------------------------------------------------------------------------------------
;$00 - used for preset value
.proc SpriteShuffler
  ; ldy AreaType                ;load level type, likely residual code
  lda #$28                    ;load preset value which will put it at
  sta $00                     ;sprite #10
  ldx #$0e                    ;start at the end of OAM data offsets
ShuffleLoop:
    lda SprDataOffset,x         ;check for offset value against
    cmp $00                     ;the preset value
    bcc NextSprOffset           ;if less, skip this part
    ldy SprShuffleAmtOffset     ;get current offset to preset value we want to add
    clc
    adc SprShuffleAmt,y         ;get shuffle amount, add to current sprite offset
    bcc StrSprOffset            ;if not exceeded $ff, skip second add
    clc
    adc $00                     ;otherwise add preset value $28 to offset
StrSprOffset:
    sta SprDataOffset,x         ;store new offset here or old one if branched to here
NextSprOffset: 
    dex                         ;move backwards to next one
    bpl ShuffleLoop
  ldx SprShuffleAmtOffset     ;load offset
  inx
  cpx #$03                    ;check if offset + 1 goes to 3
  bne SetAmtOffset            ;if offset + 1 not 3, store
    ldx #$00                  ;otherwise, init to 0
SetAmtOffset:
  stx SprShuffleAmtOffset
  ldx #$08                    ;load offsets for values and storage
  ldy #$02
SetMiscOffset:
    lda SprDataOffset+5,y       ;load one of three OAM data offsets
    sta Misc_SprDataOffset-2,x  ;store first one unmodified, but
    clc                         ;add eight to the second and eight
    adc #$08                    ;more to the third one
    sta Misc_SprDataOffset-1,x  ;note that due to the way X is set up,
    clc                         ;this code loads into the misc sprite offsets
    adc #$08
    sta Misc_SprDataOffset,x        
    dex
    dex
    dex
    dey
    bpl SetMiscOffset           ;do this until all misc spr offsets are loaded
  rts
.endproc
