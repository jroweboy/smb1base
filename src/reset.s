
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
  phy
    ; Load the current scanline number that we are on
    ldy ScanlineCounter

    ; php
    ; plp
    ; nop
    ; nop
    ; php
    ; plp

; DELAY_BASE = 15
; .if ::MAPPER_MMC3
;   lda #40 - DELAY_BASE

; ; 15—270 cycles of delay: delay=A+15; 0 ≤ A ≤ 255)
; DelayACycles:
;         sec     
; @L:     sbc #5  
;         bcs @L  ;  6 6 6 6 6  FB FC FD FE FF
;         adc #3  ;  2 2 2 2 2  FE FF 00 01 02
;         bcc @4  ;  3 3 2 2 2  FE FF 00 01 02
;         lsr     ;  - - 2 2 2  -- -- 00 00 01
;         beq @5  ;  - - 3 3 2  -- -- 00 00 01
; @4:     lsr     ;  2 2 - - 2  7F 7F -- -- 00
; @5:     bcs @6  ;  2 3 2 3 2  7F 7F 00 00 00
; @6:

; .elseif ::MAPPER_MMC5
;   ; lda #15 - DELAY_BASE
;   ; php
;   ; plp
;   ; php
;   ; plp
; .endif



    lda ScanlineScrollN,y
    and #%00000011
    asl
    asl
    sta PPUADDR

    ; Y position to $2005.
    lda ScanlineScrollY,y
    sta PPUSCROLL

    ; Prepare for the 2 later writes:
    ; We reuse new_x to hold (Y & $F8) << 2.
    and #%11111000
    asl
    asl
    ldx ScanlineScrollX,y
    sta ScanlineScrollX,y

    ; ((Y & $F8) << 2) | (X >> 3) in A for $2006 later.
    txa
    lsr
    lsr
    lsr
    ora ScanlineScrollX,y

    pha
    lda ScanlinePpuMask,y
    tay
    pla

    ; The last two PPU writes must happen during hblank:
    stx PPUSCROLL
    sta PPUADDR
    sty PPUMASK
    
    MAPPER_IRQ_ACK

    ldy ScanlineCounter

    ; Restore new_x.
    txa
    sta ScanlineScrollX,y

    lda ScanlineTarget+1,y
    cmp #$ff
    beq :+
      ; We aren't the last IRQ so setup the next in the chain
      SetScanlineIRQ { ScanlineTarget+1,y }
      inc ScanlineCounter
      bne ExitIrq
    :
    lda #0
    sta ScanlineCounter
ExitIrq:
  ply
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

  lda #0
  sta NmiBackgroundProtect

  ; do mapper specific init
  jsr MapperInit
FinializeMarioInit:
  cli
  lda #$a5                     ;set warm boot flag
  sta WarmBootValidation     
  sta PseudoRandomBitReg       ;set seed for pseudorandom register
  ; lda #%00001111
  ; sta SND_MASTERCTRL_REG       ;enable all sound channels except dmc
  jsr AudioInit
  lda #%00000110
  sta PPUMASK            ;turn off clipping for OAM and background
  farcall MoveAllSpritesOffscreen
  farcall InitializeNameTables     ;initialize both name tables
  inc DisableScreenFlag        ;set flag to disable screen output
  lda Mirror_PPUCTRL
  ora #%10100000               ;enable NMIs and 8x16 sprites
  sta PPUCTRL              ;write contents of A to PPU register 1
  sta Mirror_PPUCTRL       ;and its mirror

  ; do a jsr to the main loop so we can profile it separately
  jsr IdleLoop
.endproc

;-------------------------------------------------------------------------------------
;$00 - vram buffer address table low
;$01 - vram buffer address table high

clabel UpdateScreen

WriteBufferToScreen:
  sta PPUADDR           ;store high byte of vram address
  iny
  lda (NmiR0),y               ;load next byte (second)
  sta PPUADDR           ;store low byte of vram address
  iny
  lda (NmiR0),y               ;load next byte (third)
  asl                       ;shift to left and save in stack
  pha
    lda Mirror_PPUCTRL     ;load mirror of $2000,
    ora #%00000100            ;set ppu to increment by 32 by default
    bcs SetupWrites           ;if d7 of third byte was clear, ppu will
      and #%11111011            ;only increment by 1
SetupWrites:
    sta PPUCTRL         ;write contents of A to PPU register 1
    sta Mirror_PPUCTRL       ;and its mirror
  pla                       ;pull from stack and shift to left again
  asl
  bcc GetLength             ;if d6 of third byte was clear, do not repeat byte
    ora #%00000010            ;otherwise set d1 and increment Y
    iny
GetLength:
  lsr                       ;shift back to the right to get proper length
  lsr                       ;note that d1 will now be in carry
  tax
OutputToVRAM:
  bcs RepeatByte            ;if carry set, repeat loading the same byte
    iny                       ;otherwise increment Y to load next byte
RepeatByte:
  lda (NmiR0), y               ;load more data from buffer and write to vram
  sta PPUDATA
  dex                       ;done writing?
  bne OutputToVRAM
  sec          
  tya
  adc NmiR0                    ;add end length plus one to the indirect at $00
  sta NmiR0                    ;to allow this routine to read another set of updates
  lda #$00
  adc NmiR1
  sta NmiR1
UpdateScreen:
  ldx PPUSTATUS            ;reset flip-flop
  ldy #$00                  ;load first byte from indirect as a pointer
  lda (NmiR0),y  
  bne WriteBufferToScreen   ;if byte is zero we have no further updates to make here
InitScroll:
  sta PPUSCROLL        ;store contents of A into scroll registers
  sta PPUSCROLL        ;and end whatever subroutine led us here
  rts

.proc BankSwitchCHR
  lda ReloadCHRBank
  beq :+
    SwitchAreaCHR ; defined by the mapper
    ldx #0
    stx ReloadCHRBank
  :
  rts
.endproc

.proc NonMaskableInterrupt
  pha
  phx
  phy
  lda NmiDisable
  bpl ContinueNMI
    inc NmiSkipped
    ; ; lag frame, prevent the graphics from going bunk by still running
    ; ; the irq. also run audio to keep it sounding like we didn't lag
    ; ; Unless the screen is off, then we don't care
    ; lda DisableScreenFlag
    ; bne :+
    ;   SetScanlineIRQ #$1f
    ;   lda Mirror_PPUCTRL
    ;   and #%11111110            ;alter name table address to be $2800
    ;   sta PPUCTRL              ;(essentially $2000) but save other bits
    ;   lda #0
    ;   sta PPUSCROLL
    ;   sta PPUSCROLL
    ; :
    ; ; Force the area/player sprite banks to switch even during lag frames
    ; jsr BankSwitchCHR
    ; jsr AudioUpdate
    ; ply
    ; plx
    ; pla
    ; rti
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

  lda NmiBackgroundProtect
  bne :+
    ldx VRAM_Buffer_AddrCtrl  ;load control for pointer to buffer contents
    lda VRAM_AddrTable_Low,x  ;set indirect at $00 to pointer
    sta NmiR0
    lda VRAM_AddrTable_High,x
    sta NmiR1
    jsr UpdateScreen  ;update screen with buffer contents
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
  :
  
  jsr OAMandReadJoypad

  lda Mirror_PPUMASK       ;copy mirror of $2001 to register
  sta PPUMASK

  lda HorizontalScroll
  sta ScanlineScrollX+0
  sta ScanlineScrollX+2
  lda #32
  sta ScanlineScrollY+0
  ; lda Mirror_PPUCTRL
  ; sta IrqScrollBit
  lda Sprite0HitDetectFlag  ;check for flag here
  beq SkipSprite0
  
    SetScanlineIRQ #$1f
    ; cli just in case NMI runs late
    cli

SkipSprite0:
  lda Mirror_PPUCTRL
  ; sta IrqPPUCTRL
  sta ScanlineScrollN+0
  sta ScanlineScrollN+2
  ; and also reset the flags for the HUD
  and #%11111100
  sta PPUCTRL

  ; If the main thread requested a CHR bank switch, do it before the timing window passes
  jsr BankSwitchCHR

  jsr AudioUpdate
  
.if ::DEBUG_WORLD_SELECT
	dec DebugCooldown
	bpl OnCooldown
    inc DebugCooldown
    lda SavedJoypadBits
    and #Select_Button
    beq OnCooldown
      lda #10
      sta DebugCooldown
      lda SavedJoypadBits
      and #B_Button
      beq NextWorld
        ; TODO don't farcall in nmi
        BankPRGA #.bank(PrcNextA)
        jsr PrcNextA
        jmp OnCooldown
    NextWorld:
      jsr PlayerEndWorld
OnCooldown:
.endif

  BankPRGA CurrentBank
.if ::MAPPER_MMC3
  lda BankShadow
  sta BANK_SELECT
.endif
  ply
  plx
  pla
  rti                       ;we are done until the next frame!

.endproc

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

clabel VRAM_AddrTable_Low
clabel VRAM_AddrTable_High
VRAM_AddrTable_Low: .lobytes VRAM_AddrTable
VRAM_AddrTable_High: .hibytes VRAM_AddrTable

clabel VRAM_Buffer_Offset
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

;-------------------------------------------------------------------------------------

;$06 - RAM address low
;$07 - RAM address high
InitializeMemoryRAMLo = $06
InitializeMemoryRAMHi = $07
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
    
.if ::ENABLE_C_CODE
; Reset the stack pointer for C code. Shame we lose the stack ...
; TODO Might wanna investigate a new InitMemory function
.importzp sp
  lda #<(CStack+$20)
  sta sp
  lda #>(CStack+$20)
  sta sp+1
.endif
  lda #0
  rts
.endproc

.if ENABLE_C_CODE
.importzp ptr4, tmp4
; This allows c code to reuse the farcall mechanism
cproc farcall_trampoline
  SMC_StoreValue SafecallA, a
  lda ptr4
  SMC_StoreLowByte FarcallJmpTarget, a
  lda ptr4+1
  SMC_StoreHighByte FarcallJmpTarget, a
  lda tmp4
  ora #MMC5_PRG_ROM
  jmp FarCallCommon
endcproc
.endif

; .if DEBUG_ADD_EXTRA_LAG
;;;;;;;;;;;;;;;;;;;;;;;;
; Delays X*256+A clocks + overhead
; Clobbers A,X. Preserves Y.
; Depends on delay_a_25_clocks within short branch distance
; Time: X*256+A+30 clocks (including JSR)
;;;;;;;;;;;;;;;;;;;;;;;;
:      sbc #7    ; carry set by CMP
delay_256x_a_30_clocks_b:
       cmp #7    ; 2  2  2  2  2  2  2   00 01 02 03 04 05 06   0 0 0 0 0 0 0
       bcs :-    ; 2  2  2  2  2  2  2   00 01 02 03 04 05 06   0 0 0 0 0 0 0
       lsr       ; 2  2  2  2  2  2  2   00 00 01 01 02 02 03   0 1 0 1 0 1 0
       bcs @2    ; 2  3  2  3  2  3  2   00 00 01 01 02 02 03   0 1 0 1 0 1 0
@2:    beq @6    ; 3  3  2  2  2  2  2   00 00 01 01 02 02 03   0 1 0 1 0 1 0
       lsr       ;       2  2  2  2  2         00 00 01 01 01       1 1 0 0 1
       beq @do_x ;       3  3  2  2  2         00 00 01 01 01       1 1 0 0 1
       bcc @do_x ;             3  3  2               01 01 01           0 0 1
@6:    bne @do_x ; 2  2              3   00 00             01   0 1         0
@do_x: txa       ;2
       beq @rts  ;3
       ;4 cycles done. Must consume 256 cycles; 252 cycles remain.
       nop       ;2
       tya       ;2
        ldy #48  ;2
@l:     dey      ;2*48
        bne @l   ;3*48
       tay       ;2-1
       dex       ;2
       jmp @do_x ;3
@rts:  rts
; .endif



cproc galois32
	; rotate the middle bytes left
	ldy seed+2 ; will move to seed+3 at the end
	lda seed+1
	sta seed+2
	; compute seed+1 ($C5>>1 = %1100010)
	lda seed+3 ; original high byte
	lsr
	sta seed+1 ; reverse: 100011
	lsr
	lsr
	lsr
	lsr
	eor seed+1
	lsr
	eor seed+1
	eor seed+0 ; combine with original low byte
	sta seed+1
	; compute seed+0 ($C5 = %11000101)
	lda seed+3 ; original high byte
	asl
	eor seed+3
	asl
	asl
	asl
	asl
	eor seed+3
	asl
	asl
	eor seed+3
	sty seed+3 ; finish rotating byte 2 into 3
	sta seed+0
	rts
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
