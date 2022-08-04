.include "common.inc"


;-------------------------------------------------------------------------------------

.proc ScreenRoutines
  .import JumpEngine, GetAreaPalette, ClearBuffersDrawIcon, WriteTopScore

  lda ScreenRoutineTask        ;run one of the following subroutines
  jsr JumpEngine

  .word InitScreen
  .word SetupIntermediate
  .word WriteTopStatusLine
  .word WriteBottomStatusLine
  .word DisplayTimeUp
  .word ResetSpritesAndScreenTimer
  .word DisplayIntermediate
  .word ResetSpritesAndScreenTimer
  .word AreaParserTaskControl
  .word GetAreaPalette
  .word GetBackgroundColor
  .word GetAlternatePalette1
  .word DrawTitleScreen
  .word ClearBuffersDrawIcon
  .word WriteTopScore
.endproc


;-------------------------------------------------------------------------------------

.proc InitScreen
  .import MoveAllSpritesOffscreen
  .export GetAreaPalette

  jsr MoveAllSpritesOffscreen ;initialize all sprites including sprite #0
  jsr InitializeNameTables    ;and erase both name and attribute tables
  lda OperMode
  beq NextSubtask             ;if mode still 0, do not load
  ldx #$03                    ;into buffer pointer
  jmp SetVRAMAddr_A
  ; implicit rts

GetAreaPalette:
.import IncSubtask
  ldy AreaType             ;select appropriate palette to load
  ldx AreaPalette,y        ;based on area type
SetVRAMAddr_A:
  stx VRAM_Buffer_AddrCtrl ;store offset into buffer control
NextSubtask:
  jmp IncSubtask           ;move onto next task
AreaPalette:
  .byte $01, $02, $03, $04
.endproc


;-------------------------------------------------------------------------------------
.proc MoveAllSpritesOffscreen
.export MoveAllSpritesOffscreen, MoveSpritesOffscreen

  ldy #$00                ;this routine moves all sprites off the screen
  .byte $2c                 ;BIT instruction opcode
MoveSpritesOffscreen:
  ldy #$04                ;this routine moves all but sprite 0
  lda #$f8                ;off the screen
@SprInitLoop:
    sta Sprite_Y_Position,y ;write 248 into OAM data's Y coordinate
    iny                     ;which will move it off the screen
    iny
    iny
    iny
    bne @SprInitLoop
  rts
.endproc

;-------------------------------------------------------------------------------------
.proc InitializeNameTables
.import InitScroll
.export InitializeNameTables

  lda PPU_STATUS            ;reset flip-flop
  lda Mirror_PPU_CTRL_REG1  ;load mirror of ppu reg $2000
  ora #%00010000            ;set sprites for first 4k and background for second 4k
  and #%11110000            ;clear rest of lower nybble, leave higher alone
  jsr WritePPUReg1
  lda #$24                  ;set vram address to start of name table 1
  jsr WriteNTAddr
  lda #$20                  ;and then set it to name table 0
WriteNTAddr:
  sta PPU_ADDRESS
  lda #$00
  sta PPU_ADDRESS
  ldx #$04                  ;clear name table with blank tile #24
  ldy #$c0
  lda #$24
InitNTLoop:
  sta PPU_DATA              ;count out exactly 768 tiles
  dey
  bne InitNTLoop
  dex
  bne InitNTLoop
  ldy #64                   ;now to clear the attribute table (with zero this time)
  txa
  sta VRAM_Buffer1_Offset   ;init vram buffer 1 offset
  sta VRAM_Buffer1          ;init vram buffer 1
InitATLoop:
  sta PPU_DATA
  dey
  bne InitATLoop
  sta HorizontalScroll      ;reset scroll variables
  sta VerticalScroll
  jmp InitScroll            ;initialize scroll registers to zero
.endproc

;-------------------------------------------------------------------------------------

.proc WriteTopStatusLine
.import IncSubtask
  lda #$00          ;select main status bar
  jsr WriteGameText ;output it
  jmp IncSubtask    ;onto the next task
  ;implicit rts
.endproc

;-------------------------------------------------------------------------------------

.proc SetupIntermediate
.import GetPlayerColors, IncSubtask

  lda BackgroundColorCtrl  ;save current background color control
  pha                      ;and player status to stack
    lda PlayerStatus
    pha
      lda #$00                 ;set background color to black
      sta PlayerStatus         ;and player status to not fiery
      lda #$02                 ;this is the ONLY time background color control
      sta BackgroundColorCtrl  ;is set to less than 4
      jsr GetPlayerColors
    pla                      ;we only execute this routine for
    sta PlayerStatus         ;the intermediate lives display
  pla                      ;and once we're done, we return bg
  sta BackgroundColorCtrl  ;color ctrl and player status from stack
  jmp IncSubtask           ;then move onto the next task
.endproc

;-------------------------------------------------------------------------------------

.proc WriteBottomStatusLine
.import GetSBNybbles, IncSubtask

  jsr GetSBNybbles        ;write player's score and coin tally to screen
  ldx VRAM_Buffer1_Offset
  lda #$20                ;write address for world-area number on screen
  sta VRAM_Buffer1,x
  lda #$73
  sta VRAM_Buffer1+1,x
  lda #$03                ;write length for it
  sta VRAM_Buffer1+2,x
  ldy WorldNumber         ;first the world number
  iny
  tya
  sta VRAM_Buffer1+3,x
  lda #$28                ;next the dash
  sta VRAM_Buffer1+4,x
  ldy LevelNumber         ;next the level number
  iny                     ;increment for proper number display
  tya
  sta VRAM_Buffer1+5,x    
  lda #$00                ;put null terminator on
  sta VRAM_Buffer1+6,x
  txa                     ;move the buffer offset up by 6 bytes
  clc
  adc #$06
  sta VRAM_Buffer1_Offset
  jmp IncSubtask
.endproc

;-------------------------------------------------------------------------------------

.proc DisplayTimeUp
.import OutputInter, IncSubtask
  lda GameTimerExpiredFlag  ;if game timer not expired, increment task
  beq NoTimeUp              ;control 2 tasks forward, otherwise, stay here
  lda #$00
  sta GameTimerExpiredFlag  ;reset timer expiration flag
  lda #$02                  ;output time-up screen to buffer
  jmp OutputInter
NoTimeUp:
  inc ScreenRoutineTask     ;increment control task 2 tasks forward
  jmp IncSubtask
.endproc

;-------------------------------------------------------------------------------------

.proc ResetSpritesAndScreenTimer
.import MoveAllSpritesOffscreen
.export ResetScreenTimer

  lda ScreenTimer             ;check if screen timer has expired
  bne NoReset                 ;if not, branch to leave
  jsr MoveAllSpritesOffscreen ;otherwise reset sprites now

ResetScreenTimer:
  lda #$07                    ;reset timer again
  sta ScreenTimer
  inc ScreenRoutineTask       ;move onto next task
NoReset:
  rts
.endproc


;-------------------------------------------------------------------------------------

.proc DisplayIntermediate
.import IncModeTask_B, ResetScreenTimer, DrawPlayer_Intermediate
.export OutputInter
  lda OperMode                 ;check primary mode of operation
  beq NoInter                  ;if in title screen mode, skip this
  cmp #GameOverModeValue       ;are we in game over mode?
  beq GameOverInter            ;if so, proceed to display game over screen
  lda AltEntranceControl       ;otherwise check for mode of alternate entry
  bne NoInter                  ;and branch if found
  ldy AreaType                 ;check if we are on castle level
  cpy #$03                     ;and if so, branch (possibly residual)
  beq PlayerInter
  lda DisableIntermediate      ;if this flag is set, skip intermediate lives display
  bne NoInter                  ;and jump to specific task, otherwise
PlayerInter:
  jsr DrawPlayer_Intermediate  ;put player in appropriate place for
  lda #$01                     ;lives display, then output lives display to buffer
OutputInter:
  jsr WriteGameText
  jsr ResetScreenTimer
  lda #$00
  sta DisableScreenFlag        ;reenable screen output
  rts
GameOverInter:
  lda #$12                     ;set screen timer
  sta ScreenTimer
  lda #$03                     ;output game over screen to buffer
  jsr WriteGameText
  jmp IncModeTask_B
NoInter:
  lda #$08                     ;set for specific task and leave
  sta ScreenRoutineTask
  rts
.endproc


;-------------------------------------------------------------------------------------

.proc AreaParserTaskControl
.import AreaParserTaskHandler

  inc DisableScreenFlag     ;turn off screen
TaskLoop:
    jsr AreaParserTaskHandler ;render column set of current area
    lda AreaParserTaskNum     ;check number of tasks
    bne TaskLoop              ;if tasks still not all done, do another one
  dec ColumnSets            ;do we need to render more column sets?
  bpl OutputCol
    inc ScreenRoutineTask     ;if not, move on to the next task
OutputCol:
  lda #$06                  ;set vram buffer to output rendered column set
  sta VRAM_Buffer_AddrCtrl  ;on next NMI
  rts
.endproc

;-------------------------------------------------------------------------------------
;$00 - used as temp counter in GetPlayerColors

.proc GetBackgroundColor
.export GetPlayerColors, SetVRAMOffset


  ldy BackgroundColorCtrl   ;check background color control
  beq NoBGColor             ;if not set, increment task and fetch palette
  lda BGColorCtrl_Addr-4,y  ;put appropriate palette into vram
  sta VRAM_Buffer_AddrCtrl  ;note that if set to 5-7, $0301 will not be read
NoBGColor:
  inc ScreenRoutineTask     ;increment to next subtask and plod on through    
  ;fallthrough

GetPlayerColors:
  ldx VRAM_Buffer1_Offset  ;get current buffer offset
  ldy #$00
  lda CurrentPlayer        ;check which player is on the screen
  beq ChkFiery
  ldy #$04                 ;load offset for luigi
ChkFiery:
  lda PlayerStatus         ;check player status
  cmp #$02
  bne StartClrGet          ;if fiery, load alternate offset for fiery player
  ldy #$08
StartClrGet:
  lda #$03                 ;do four colors
  sta $00
ClrGetLoop:
  lda PlayerColors,y       ;fetch player colors and store them
  sta VRAM_Buffer1+3,x     ;in the buffer
  iny
  inx
  dec $00
  bpl ClrGetLoop
  ldx VRAM_Buffer1_Offset  ;load original offset from before
  ldy BackgroundColorCtrl  ;if this value is four or greater, it will be set
  bne SetBGColor           ;therefore use it as offset to background color
  ldy AreaType             ;otherwise use area type bits from area offset as offset
SetBGColor:
  lda BackgroundColors,y   ;to background color instead
  sta VRAM_Buffer1+3,x
  lda #$3f                 ;set for sprite palette address
  sta VRAM_Buffer1,x       ;save to buffer
  lda #$10
  sta VRAM_Buffer1+1,x
  lda #$04                 ;write length byte to buffer
  sta VRAM_Buffer1+2,x
  lda #$00                 ;now the null terminator
  sta VRAM_Buffer1+7,x
  txa                      ;move the buffer pointer ahead 7 bytes
  clc                      ;in case we want to write anything else later
  adc #$07
SetVRAMOffset:
  sta VRAM_Buffer1_Offset  ;store as new vram buffer offset
  rts

BGColorCtrl_Addr:
      .byte $00, $09, $0a, $04

BackgroundColors:
      .byte $22, $22, $0f, $0f ;used by area type if bg color ctrl not set
      .byte $0f, $22, $0f, $0f ;used by background color control if set

PlayerColors:
      .byte $22, $16, $27, $18 ;mario's colors
      .byte $22, $30, $27, $19 ;luigi's colors
      .byte $22, $37, $27, $16 ;fiery (used by both)
.endproc


;-------------------------------------------------------------------------------------

.proc GetAlternatePalette1
.import IncSubtask
.export SetVRAMAddr_B

  lda AreaStyle            ;check for mushroom level style
  cmp #$01
  bne NoAltPal
  lda #$0b                 ;if found, load appropriate palette

SetVRAMAddr_B:
  sta VRAM_Buffer_AddrCtrl
NoAltPal:
  jmp IncSubtask           ;now onto the next task
.endproc

;-------------------------------------------------------------------------------------

;$00 - vram buffer address table low
;$01 - vram buffer address table high

.proc DrawTitleScreen
.import SetVRAMAddr_B
.export IncSubtask, IncModeTask_B
  lda OperMode                 ;are we in title screen mode?
  bne IncModeTask_B            ;if not, exit
  lda #>TitleScreenDataOffset  ;load address $1ec0 into
  sta PPU_ADDRESS              ;the vram address register
  lda #<TitleScreenDataOffset
  sta PPU_ADDRESS
  lda #$03                     ;put address $0300 into
  sta $01                      ;the indirect at $00
  ldy #$00
  sty $00
  lda PPU_DATA                 ;do one garbage read
OutputTScr:
  lda PPU_DATA                 ;get title screen from chr-rom
  sta ($00),y                  ;store 256 bytes into buffer
  iny
  bne ChkHiByte                ;if not past 256 bytes, do not increment
  inc $01                      ;otherwise increment high byte of indirect
ChkHiByte:
  lda $01                      ;check high byte?
  cmp #$04                     ;at $0400?
  bne OutputTScr               ;if not, loop back and do another
  cpy #$3a                     ;check if offset points past end of data
  bcc OutputTScr               ;if not, loop back and do another
  lda #$05                     ;set buffer transfer control to $0300,
  jmp SetVRAMAddr_B            ;increment task and exit

;-------------------------------------------------------------------------------------

WriteTopScore:
.import UpdateNumber

  lda #$fa           ;run display routine to display top score on title
  jsr UpdateNumber
IncModeTask_B:
  inc OperMode_Task  ;move onto next mode
  rts
;-------------------------------------------------------------------------------------

ClearBuffersDrawIcon:

  lda OperMode               ;check game mode
  bne IncModeTask_B          ;if not title screen mode, leave
  ldx #$00                   ;otherwise, clear buffer space
TScrClear:
  sta VRAM_Buffer1-1,x
  sta VRAM_Buffer1-1+$100,x
  dex
  bne TScrClear
  jsr DrawMushroomIcon       ;draw player select icon
IncSubtask:
  inc ScreenRoutineTask      ;move onto next task
  rts
.endproc

;-------------------------------------------------------------------------------------

.proc DrawMushroomIcon
  ldy #$07                ;read eight bytes to be read by transfer routine
IconDataRead:
  lda MushroomIconData,y  ;note that the default position is set for a
  sta VRAM_Buffer1-1,y    ;1-player game
  dey
  bpl IconDataRead
  lda NumberOfPlayers     ;check number of players
  beq ExitIcon            ;if set to 1-player game, we're done
  lda #$24                ;otherwise, load blank tile in 1-player position
  sta VRAM_Buffer1+3
  lda #$ce                ;then load shroom icon tile in 2-player position
  sta VRAM_Buffer1+5
ExitIcon:
  rts
MushroomIconData:
  .byte $07, $22, $49, $83, $ce, $24, $24, $00
.endproc


;-------------------------------------------------------------------------------------
.proc WritePPUReg1
.export WritePPUReg1

  sta PPU_CTRL_REG1         ;write contents of A to PPU register 1
  sta Mirror_PPU_CTRL_REG1  ;and its mirror
  rts
.endproc


;-------------------------------------------------------------------------------------
;$00 - vram buffer address table low
;$01 - vram buffer address table high

.proc WriteBufferToScreen
.export UpdateScreen, InitScroll

  sta PPU_ADDRESS           ;store high byte of vram address
  iny
  lda ($00),y               ;load next byte (second)
  sta PPU_ADDRESS           ;store low byte of vram address
  iny
  lda ($00),y               ;load next byte (third)
  asl                       ;shift to left and save in stack
  pha
    lda Mirror_PPU_CTRL_REG1  ;load mirror of $2000,
    ora #%00000100            ;set ppu to increment by 32 by default
    bcs SetupWrites           ;if d7 of third byte was clear, ppu will
      and #%11111011            ;only increment by 1
SetupWrites:
    jsr WritePPUReg1          ;write to register
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
  lda ($00),y               ;load more data from buffer and write to vram
  sta PPU_DATA
  dex                       ;done writing?
  bne OutputToVRAM
  sec          
  tya
  adc $00                   ;add end length plus one to the indirect at $00
  sta $00                   ;to allow this routine to read another set of updates
  lda #$00
  adc $01
  sta $01
  lda #$3f                  ;sets vram address to $3f00
  sta PPU_ADDRESS
  lda #$00
  sta PPU_ADDRESS
  sta PPU_ADDRESS           ;then reinitializes it for some reason
  sta PPU_ADDRESS

UpdateScreen:
  ldx PPU_STATUS            ;reset flip-flop
  ldy #$00                  ;load first byte from indirect as a pointer
  lda ($00),y  
  bne WriteBufferToScreen   ;if byte is zero we have no further updates to make here
InitScroll:
  sta PPU_SCROLL_REG        ;store contents of A into scroll registers
  sta PPU_SCROLL_REG        ;and end whatever subroutine led us here
  rts
.endproc


;-------------------------------------------------------------------------------------

.proc WriteGameText
.import SetVRAMOffset

  pha                      ;save text number to stack
    asl
    tay                      ;multiply by 2 and use as offset
    cpy #$04                 ;if set to do top status bar or world/lives display,
    bcc LdGameText           ;branch to use current offset as-is
    cpy #$08                 ;if set to do time-up or game over,
    bcc Chk2Players          ;branch to check players
    ldy #$08                 ;otherwise warp zone, therefore set offset
Chk2Players:
    lda NumberOfPlayers      ;check for number of players
    bne LdGameText           ;if there are two, use current offset to also print name
    iny                      ;otherwise increment offset by one to not print name
LdGameText:
    ldx GameTextOffsets,y    ;get offset to message we want to print
    ldy #$00
GameTextLoop:
    lda GameText,x           ;load message data
    cmp #$ff                 ;check for terminator
    beq EndGameText          ;branch to end text if found
    sta VRAM_Buffer1,y       ;otherwise write data to buffer
    inx                      ;and increment increment
    iny
    bne GameTextLoop         ;do this for 256 bytes if no terminator found
EndGameText:
    lda #$00                 ;put null terminator at end
    sta VRAM_Buffer1,y
  pla                      ;pull original text number from stack
  tax
  cmp #$04                 ;are we printing warp zone?
  bcs PrintWarpZoneNumbers
  dex                      ;are we printing the world/lives display?
  bne CheckPlayerName      ;if not, branch to check player's name
  lda NumberofLives        ;otherwise, check number of lives
  clc                      ;and increment by one for display
  adc #$01
  cmp #10                  ;more than 9 lives?
  bcc PutLives
  sbc #10                  ;if so, subtract 10 and put a crown tile
  ldy #$9f                 ;next to the difference...strange things happen if
  sty VRAM_Buffer1+7       ;the number of lives exceeds 19
PutLives:
  sta VRAM_Buffer1+8
  ldy WorldNumber          ;write world and level numbers (incremented for display)
  iny                      ;to the buffer in the spaces surrounding the dash
  sty VRAM_Buffer1+19
  ldy LevelNumber
  iny
  sty VRAM_Buffer1+21      ;we're done here
  rts

CheckPlayerName:
  lda NumberOfPlayers    ;check number of players
  beq ExitChkName        ;if only 1 player, leave
  lda CurrentPlayer      ;load current player
  dex                    ;check to see if current message number is for time up
  bne ChkLuigi
  ldy OperMode           ;check for game over mode
  cpy #GameOverModeValue
  beq ChkLuigi
  eor #%00000001         ;if not, must be time up, invert d0 to do other player
ChkLuigi:
  lsr
  bcc ExitChkName        ;if mario is current player, do not change the name
    ldy #$04
NameLoop:
      lda LuigiName,y        ;otherwise, replace "MARIO" with "LUIGI"
      sta VRAM_Buffer1+3,y
      dey
      bpl NameLoop           ;do this until each letter is replaced
ExitChkName:
  rts

PrintWarpZoneNumbers:
  sbc #$04               ;subtract 4 and then shift to the left
  asl                    ;twice to get proper warp zone number
  asl                    ;offset
  tax
  ldy #$00
WarpNumLoop:
    lda WarpZoneNumbers,x  ;print warp zone numbers into the
    sta VRAM_Buffer1+27,y  ;placeholders from earlier
    inx
    iny                    ;put a number in every fourth space
    iny
    iny
    iny
    cpy #$0c
    bcc WarpNumLoop
  lda #$2c               ;load new buffer pointer at end of message
  jmp SetVRAMOffset

GameText:
TopStatusBarLine:
  .byte $20, $43, $05, "MARIO"
  .byte $20, $52, $0b, "WORLD  TIME"
  .byte $20, $68, $05, "0  ", $2e, $29 ; score trailing digit and coin display
  .byte $23, $c0, $7f, $aa ; attribute table data, clears name table 0 to palette 2
  .byte $23, $c2, $01, $ea ; attribute table data, used for coin icon in status bar
  .byte $ff ; end of data block

WorldLivesDisplay:
  .byte $21, $cd, $07, $24, $24 ; cross with spaces used on
  .byte $29, $24, $24, $24, $24 ; lives display
  .byte $21, $4b, $09, "WORLD  - "
  .byte $22, $0c, $47, $24 ; possibly used to clear time up
  .byte $23, $dc, $01, $ba ; attribute table data for crown if more than 9 lives
  .byte $ff

TwoPlayerTimeUp:
  .byte $21, $cd, $05, "MARIO"
OnePlayerTimeUp:
  .byte $22, $0c, $07, "TIME UP"
  .byte $ff

TwoPlayerGameOver:
  .byte $21, $cd, $05, "MARIO"
OnePlayerGameOver:
  .byte $22, $0b, $09, "GAME OVER"
  .byte $ff

WarpZoneWelcome:
  .byte $25, $84, $15, "WELCOME TO WARP ZONE!"
  .byte $26, $25, $01, $24         ; placeholder for left pipe
  .byte $26, $2d, $01, $24         ; placeholder for middle pipe
  .byte $26, $35, $01, $24         ; placeholder for right pipe
  .byte $27, $d9, $46, $aa         ; attribute data
  .byte $27, $e1, $45, $aa
  .byte $ff

LuigiName:
  .byte "LUIGI"

WarpZoneNumbers:
  .byte "432", $00         ; warp zone numbers, note spaces on middle
  .byte " 5 ", $00         ; zone, partly responsible for
  .byte "876", $00         ; the minus world

GameTextOffsets:
  .byte TopStatusBarLine-GameText, TopStatusBarLine-GameText
  .byte WorldLivesDisplay-GameText, WorldLivesDisplay-GameText
  .byte TwoPlayerTimeUp-GameText, OnePlayerTimeUp-GameText
  .byte TwoPlayerGameOver-GameText, OnePlayerGameOver-GameText
  .byte WarpZoneWelcome-GameText, WarpZoneWelcome-GameText
.endproc


;-------------------------------------------------------------------------------------
.proc GiveOneCoin
.export GetSBNybbles, UpdateNumber, AddToScore
  lda #$01               ;set digit modifier to add 1 coin
  sta DigitModifier+5    ;to the current player's coin tally
  ldx CurrentPlayer      ;get current player on the screen
  ldy CoinTallyOffsets,x ;get offset for player's coin tally
  jsr DigitsMathRoutine  ;update the coin tally
  inc CoinTally          ;increment onscreen player's coin amount
  lda CoinTally
  cmp #100               ;does player have 100 coins yet?
  bne CoinPoints         ;if not, skip all of this
  lda #$00
  sta CoinTally          ;otherwise, reinitialize coin amount
  inc NumberofLives      ;give the player an extra life
  lda #Sfx_ExtraLife
  sta Square2SoundQueue  ;play 1-up sound

CoinPoints:
  lda #$02               ;set digit modifier to award
  sta DigitModifier+4    ;200 points to the player

AddToScore:
  ldx CurrentPlayer      ;get current player
  ldy ScoreOffsets,x     ;get offset for player's score
  jsr DigitsMathRoutine  ;update the score internally with value in digit modifier

GetSBNybbles:
  ldy CurrentPlayer      ;get current player
  lda StatusBarNybbles,y ;get nybbles based on player, use to update score and coins

UpdateNumber:
  jsr PrintStatusBarNumbers ;print status bar numbers based on nybbles, whatever they be
  ldy VRAM_Buffer1_Offset   
  lda VRAM_Buffer1-6,y      ;check highest digit of score
  bne NoZSup                ;if zero, overwrite with space tile for zero suppression
  lda #$24
  sta VRAM_Buffer1-6,y
NoZSup:
  ldx ObjectOffset          ;get enemy object buffer offset
  rts
      
CoinTallyOffsets:
      .byte $17, $1d

ScoreOffsets:
      .byte $0b, $11

StatusBarNybbles:
      .byte $02, $13

.endproc


;-------------------------------------------------------------------------------------
;$00 - used to store status bar nybbles
;$02 - used as temp vram offset
;$03 - used to store length of status bar number

;status bar name table offset and length data
StatusBarData:
      .byte $f0, $06 ; top score display on title screen
      .byte $62, $06 ; player score
      .byte $62, $06
      .byte $6d, $02 ; coin tally
      .byte $6d, $02
      .byte $7a, $03 ; game timer

StatusBarOffset:
      .byte $06, $0c, $12, $18, $1e, $24

.proc PrintStatusBarNumbers
  sta $00            ;store player-specific offset
  jsr OutputNumbers  ;use first nybble to print the coin display
  lda $00            ;move high nybble to low
  lsr                ;and print to score display
  lsr
  lsr
  lsr
OutputNumbers:
  clc                      ;add 1 to low nybble
  adc #$01
  and #%00001111           ;mask out high nybble
  cmp #$06
  bcs ExitOutputN
  pha                      ;save incremented value to stack for now and
    asl                      ;shift to left and use as offset
    tay
    ldx VRAM_Buffer1_Offset  ;get current buffer pointer
    lda #$20                 ;put at top of screen by default
    cpy #$00                 ;are we writing top score on title screen?
    bne SetupNums
    lda #$22                 ;if so, put further down on the screen
SetupNums:
    sta VRAM_Buffer1,x
    lda StatusBarData,y      ;write low vram address and length of thing
    sta VRAM_Buffer1+1,x     ;we're printing to the buffer
    lda StatusBarData+1,y
    sta VRAM_Buffer1+2,x
    sta $03                  ;save length byte in counter
    stx $02                  ;and buffer pointer elsewhere for now
  pla                      ;pull original incremented value from stack
  tax
  lda StatusBarOffset,x    ;load offset to value we want to write
  sec
  sbc StatusBarData+1,y    ;subtract from length byte we read before
  tay                      ;use value as offset to display digits
  ldx $02
DigitPLoop:
  lda DisplayDigits,y      ;write digits to the buffer
  sta VRAM_Buffer1+3,x    
  inx
  iny
  dec $03                  ;do this until all the digits are written
  bne DigitPLoop
  lda #$00                 ;put null terminator at end
  sta VRAM_Buffer1+3,x
  inx                      ;increment buffer pointer by 3
  inx
  inx
  stx VRAM_Buffer1_Offset  ;store it in case we want to use it again
ExitOutputN:
  rts
.endproc

;-------------------------------------------------------------------------------------

.proc DigitsMathRoutine
  lda OperMode              ;check mode of operation
  cmp #TitleScreenModeValue
  beq EraseDMods            ;if in title screen mode, branch to lock score
  ldx #$05
AddModLoop:
  lda DigitModifier,x       ;load digit amount to increment
  clc
  adc DisplayDigits,y       ;add to current digit
  bmi BorrowOne             ;if result is a negative number, branch to subtract
  cmp #10
  bcs CarryOne              ;if digit greater than $09, branch to add
StoreNewD:
  sta DisplayDigits,y       ;store as new score or game timer digit
  dey                       ;move onto next digits in score or game timer
  dex                       ;and digit amounts to increment
  bpl AddModLoop            ;loop back if we're not done yet
EraseDMods:
  lda #$00                  ;store zero here
  ldx #$06                  ;start with the last digit
EraseMLoop:
  sta DigitModifier-1,x     ;initialize the digit amounts to increment
  dex
  bpl EraseMLoop            ;do this until they're all reset, then leave
  rts
BorrowOne:
  dec DigitModifier-1,x     ;decrement the previous digit, then put $09 in
  lda #$09                  ;the game timer digit we're currently on to "borrow
  bne StoreNewD             ;the one", then do an unconditional branch back
CarryOne:
  sec                       ;subtract ten from our digit to make it a
  sbc #10                   ;proper BCD number, then increment the digit
  inc DigitModifier-1,x     ;preceding current digit to "carry the one" properly
  jmp StoreNewD             ;go back to just after we branched here
.endproc

;-------------------------------------------------------------------------------------
.proc UpdateTopScore
  ldx #$05          ;start with mario's score
  jsr TopScoreCheck
  ldx #$0b          ;now do luigi's score
TopScoreCheck:
  ldy #$05                 ;start with the lowest digit
  sec           
GetScoreDiff:
  lda PlayerScoreDisplay,x ;subtract each player digit from each high score digit
  sbc TopScoreDisplay,y    ;from lowest to highest, if any top score digit exceeds
  dex                      ;any player digit, borrow will be set until a subsequent
  dey                      ;subtraction clears it (player digit is higher than top)
  bpl GetScoreDiff      
  bcc NoTopSc              ;check to see if borrow is still set, if so, no new high score
  inx                      ;increment X and Y once to the start of the score
  iny
CopyScore:
  lda PlayerScoreDisplay,x ;store player's score digits into high score memory area
  sta TopScoreDisplay,y
  inx
  iny
  cpy #$06                 ;do this until we have stored them all
  bcc CopyScore
NoTopSc:
  rts
.endproc

.proc DrawOneSpriteRow
  sta $01
  jmp DrawSpriteObject        ;draw them
.endproc

;-------------------------------------------------------------------------------------
;$00-$01 - tile numbers
;$02 - Y coordinate
;$03 - flip control
;$04 - sprite attributes
;$05 - X coordinate


.proc DrawSpriteObject
  lda $03                    ;get saved flip control bits
  lsr
  lsr                        ;move d1 into carry
  lda $00
  bcc NoHFlip                ;if d1 not set, branch
  sta Sprite_Tilenumber+4,y  ;store first tile into second sprite
  lda $01                    ;and second into first sprite
  sta Sprite_Tilenumber,y
  lda #$40                   ;activate horizontal flip OAM attribute
  bne SetHFAt                ;and unconditionally branch
NoHFlip:
  sta Sprite_Tilenumber,y    ;store first tile into first sprite
  lda $01                    ;and second into second sprite
  sta Sprite_Tilenumber+4,y
  lda #$00                   ;clear bit for horizontal flip
SetHFAt:
  ora $04                    ;add other OAM attributes if necessary
  sta Sprite_Attributes,y    ;store sprite attributes
  sta Sprite_Attributes+4,y
  lda $02                    ;now the y coordinates
  sta Sprite_Y_Position,y    ;note because they are
  sta Sprite_Y_Position+4,y  ;side by side, they are the same
  lda $05       
  sta Sprite_X_Position,y    ;store x coordinate, then
  clc                        ;add 8 pixels and store another to
  adc #$08                   ;put them side by side
  sta Sprite_X_Position+4,y
  lda $02                    ;add eight pixels to the next y
  clc                        ;coordinate
  adc #$08
  sta $02
  tya                        ;add eight to the offset in Y to
  clc                        ;move to the next two sprites
  adc #$08
  tay
  inx                        ;increment offset to return it to the
  inx                        ;routine that called this subroutine
  rts
.endproc


;-------------------------------------------------------------------------------------
.proc FloateyNumbersRoutine
.import AddToScore,DumpTwoSpr
.export FloateyNumbersRoutine

  lda FloateyNum_Control,x     ;load control for floatey number
  beq EndFloateyNumber         ;if zero, branch to leave
  cmp #$0b                     ;if less than $0b, branch
  bcc ChkNumTimer
  lda #$0b                     ;otherwise set to $0b, thus keeping
  sta FloateyNum_Control,x     ;it in range
ChkNumTimer:
  tay                          ;use as Y
  lda FloateyNum_Timer,x       ;check value here
  bne DecNumTimer              ;if nonzero, branch ahead
  sta FloateyNum_Control,x     ;initialize floatey number control and leave
EndFloateyNumber:
  rts
DecNumTimer:
  dec FloateyNum_Timer,x       ;decrement value here
  cmp #$2b                     ;if not reached a certain point, branch  
  bne ChkTallEnemy
  cpy #$0b                     ;check offset for $0b
  bne LoadNumTiles             ;branch ahead if not found
  inc NumberofLives            ;give player one extra life (1-up)
  lda #Sfx_ExtraLife
  sta Square2SoundQueue        ;and play the 1-up sound
LoadNumTiles:
  lda ScoreUpdateData,y        ;load point value here
  lsr                          ;move high nybble to low
  lsr
  lsr
  lsr
  tax                          ;use as X offset, essentially the digit
  lda ScoreUpdateData,y        ;load again and this time
  and #%00001111               ;mask out the high nybble
  sta DigitModifier,x          ;store as amount to add to the digit
  jsr AddToScore               ;update the score accordingly
ChkTallEnemy:
  ldy Enemy_SprDataOffset,x    ;get OAM data offset for enemy object
  lda Enemy_ID,x               ;get enemy object identifier
  cmp #Spiny
  beq FloateyPart              ;branch if spiny
  cmp #PiranhaPlant
  beq FloateyPart              ;branch if piranha plant
  cmp #HammerBro
  beq GetAltOffset             ;branch elsewhere if hammer bro
  cmp #GreyCheepCheep
  beq FloateyPart              ;branch if cheep-cheep of either color
  cmp #RedCheepCheep
  beq FloateyPart
  cmp #TallEnemy
  bcs GetAltOffset             ;branch elsewhere if enemy object => $09
  lda Enemy_State,x
  cmp #$02                     ;if enemy state defeated or otherwise
  bcs FloateyPart              ;$02 or greater, branch beyond this part
GetAltOffset:
  ldx SprDataOffset_Ctrl       ;load some kind of control bit
  ldy Alt_SprDataOffset,x      ;get alternate OAM data offset
  ldx ObjectOffset             ;get enemy object offset again
FloateyPart:
  lda FloateyNum_Y_Pos,x       ;get vertical coordinate for
  cmp #$18                     ;floatey number, if coordinate in the
  bcc SetupNumSpr              ;status bar, branch
  sbc #$01
  sta FloateyNum_Y_Pos,x       ;otherwise subtract one and store as new
SetupNumSpr:
  lda FloateyNum_Y_Pos,x       ;get vertical coordinate
  sbc #$08                     ;subtract eight and dump into the
  jsr DumpTwoSpr               ;left and right sprite's Y coordinates
  lda FloateyNum_X_Pos,x       ;get horizontal coordinate
  sta Sprite_X_Position,y      ;store into X coordinate of left sprite
  clc
  adc #$08                     ;add eight pixels and store into X
  sta Sprite_X_Position+4,y    ;coordinate of right sprite
  lda #$02
  sta Sprite_Attributes,y      ;set palette control in attribute bytes
  sta Sprite_Attributes+4,y    ;of left and right sprites
  lda FloateyNum_Control,x
  asl                          ;multiply our floatey number control by 2
  tax                          ;and use as offset for look-up table
  lda FloateyNumTileData,x
  sta Sprite_Tilenumber,y      ;display first half of number of points
  lda FloateyNumTileData+1,x
  sta Sprite_Tilenumber+4,y    ;display the second half
  ldx ObjectOffset             ;get enemy object offset and leave
  rts

;data is used as tiles for numbers
;that appear when you defeat enemies
FloateyNumTileData:
      .byte $ff, $ff ;dummy
      .byte $f6, $fb ; "100"
      .byte $f7, $fb ; "200"
      .byte $f8, $fb ; "400"
      .byte $f9, $fb ; "500"
      .byte $fa, $fb ; "800"
      .byte $f6, $50 ; "1000"
      .byte $f7, $50 ; "2000"
      .byte $f8, $50 ; "4000"
      .byte $f9, $50 ; "5000"
      .byte $fa, $50 ; "8000"
      .byte $fd, $fe ; "1-UP"

;high nybble is digit number, low nybble is number to
;add to the digit of the player's score
ScoreUpdateData:
      .byte $ff ;dummy
      .byte $41, $42, $44, $45, $48
      .byte $31, $32, $34, $35, $38, $00

.endproc

;-------------------------------------------------------------------------------------
;$00-$01 - used to hold tile numbers ($01 addressed in draw floatey number part)
;$02 - used to hold Y coordinate for floatey number
;$03 - residual byte used for flip (but value set here affects nothing)
;$04 - attribute byte for floatey number
;$05 - used as X coordinate for floatey number


.proc FlagpoleGfxHandler
.export FlagpoleGfxHandler,MoveSixSpritesOffscreen,DumpSixSpr,DumpFourSpr,DumpThreeSpr,DumpTwoSpr
      ldy Enemy_SprDataOffset,x      ;get sprite data offset for flagpole flag
      lda Enemy_Rel_XPos             ;get relative horizontal coordinate
      sta Sprite_X_Position,y        ;store as X coordinate for first sprite
      clc
      adc #$08                       ;add eight pixels and store
      sta Sprite_X_Position+4,y      ;as X coordinate for second and third sprites
      sta Sprite_X_Position+8,y
      clc
      adc #$0c                       ;add twelve more pixels and
      sta $05                        ;store here to be used later by floatey number
      lda Enemy_Y_Position,x         ;get vertical coordinate
      jsr DumpTwoSpr                 ;and do sub to dump into first and second sprites
      adc #$08                       ;add eight pixels
      sta Sprite_Y_Position+8,y      ;and store into third sprite
      lda FlagpoleFNum_Y_Pos         ;get vertical coordinate for floatey number
      sta $02                        ;store it here
      lda #$01
      sta $03                        ;set value for flip which will not be used, and
      sta $04                        ;attribute byte for floatey number
      sta Sprite_Attributes,y        ;set attribute bytes for all three sprites
      sta Sprite_Attributes+4,y
      sta Sprite_Attributes+8,y
      lda #$7e
      sta Sprite_Tilenumber,y        ;put triangle shaped tile
      sta Sprite_Tilenumber+8,y      ;into first and third sprites
      lda #$7f
      sta Sprite_Tilenumber+4,y      ;put skull tile into second sprite
      lda FlagpoleCollisionYPos      ;get vertical coordinate at time of collision
      beq ChkFlagOffscreen           ;if zero, branch ahead
      tya
      clc                            ;add 12 bytes to sprite data offset
      adc #$0c
      tay                            ;put back in Y
      lda FlagpoleScore              ;get offset used to award points for touching flagpole
      asl                            ;multiply by 2 to get proper offset here
      tax
      lda FlagpoleScoreNumTiles,x    ;get appropriate tile data
      sta $00
      lda FlagpoleScoreNumTiles+1,x
      jsr DrawOneSpriteRow           ;use it to render floatey number

ChkFlagOffscreen:
      ldx ObjectOffset               ;get object offset for flag
      ldy Enemy_SprDataOffset,x      ;get OAM data offset
      lda Enemy_OffscreenBits        ;get offscreen bits
      and #%00001110                 ;mask out all but d3-d1
      beq ExitDumpSpr                ;if none of these bits set, branch to leave

;-------------------------------------------------------------------------------------

MoveSixSpritesOffscreen:
      lda #$f8                  ;set offscreen coordinate if jumping here

DumpSixSpr:
      sta Sprite_Data+20,y      ;dump A contents
      sta Sprite_Data+16,y      ;into third row sprites

DumpFourSpr:
      sta Sprite_Data+12,y      ;into second row sprites

DumpThreeSpr:
      sta Sprite_Data+8,y

DumpTwoSpr:
      sta Sprite_Data+4,y       ;and into first row sprites
      sta Sprite_Data,y

ExitDumpSpr:
      rts

FlagpoleScoreNumTiles:
      .byte $f9, $50
      .byte $f7, $50
      .byte $fa, $fb
      .byte $f8, $fb
      .byte $f6, $fb

.endproc


;-------------------------------------------------------------------------------------
;$00 - temp store for offset control bit
;$01 - temp vram buffer offset
;$02 - temp store for vertical high nybble in block buffer routine
;$03 - temp adder for high byte of name table address
;$04, $05 - name table address low/high
;$06, $07 - block buffer address low/high

.proc RemoveCoin_Axe
  ldy #$41                 ;set low byte so offset points to $0341
  lda #$03                 ;load offset for default blank metatile
  ldx AreaType             ;check area type
  bne WriteBlankMT         ;if not water type, use offset
  lda #$04                 ;otherwise load offset for blank metatile used in water
WriteBlankMT:
  jsr PutBlockMetatile     ;do a sub to write blank metatile to vram buffer
  lda #$06
  sta VRAM_Buffer_AddrCtrl ;set vram address controller to $0341 and leave
  rts
.endproc

.proc ReplaceBlockMetatile
  jsr WriteBlockMetatile    ;write metatile to vram buffer to replace block object
  inc Block_ResidualCounter ;increment unused counter (residual code)
  dec Block_RepFlag,x       ;decrement flag (residual code)
  rts                       ;leave
.endproc

.proc DestroyBlockMetatile
  lda #$00       ;force blank metatile if branched/jumped to this point
  ; fallthrough
.endproc
.proc WriteBlockMetatile
.import SetVRAMOffset
  ldy #$03                ;load offset for blank metatile
  cmp #$00                ;check contents of A for blank metatile
  beq UseBOffset          ;branch if found (unconditional if branched from 8a6b)
  ldy #$00                ;load offset for brick metatile w/ line
  cmp #$58
  beq UseBOffset          ;use offset if metatile is brick with coins (w/ line)
  cmp #$51
  beq UseBOffset          ;use offset if metatile is breakable brick w/ line
  iny                     ;increment offset for brick metatile w/o line
  cmp #$5d
  beq UseBOffset          ;use offset if metatile is brick with coins (w/o line)
  cmp #$52
  beq UseBOffset          ;use offset if metatile is breakable brick w/o line
  iny                     ;if any other metatile, increment offset for empty block
UseBOffset:
  tya                     ;put Y in A
  ldy VRAM_Buffer1_Offset ;get vram buffer offset
  iny                     ;move onto next byte
  jsr PutBlockMetatile    ;get appropriate block data and write to vram buffer
MoveVOffset:
  dey                     ;decrement vram buffer offset
  tya                     ;add 10 bytes to it
  clc
  adc #10
  jmp SetVRAMOffset       ;branch to store as new vram buffer offset
  ; fallthrough
.endproc
.proc PutBlockMetatile
  stx $00               ;store control bit from SprDataOffset_Ctrl
  sty $01               ;store vram buffer offset for next byte
  asl
  asl                   ;multiply A by four and use as X
  tax
  ldy #$20              ;load high byte for name table 0
  lda $06               ;get low byte of block buffer pointer
  cmp #$d0              ;check to see if we're on odd-page block buffer
  bcc SaveHAdder        ;if not, use current high byte
  ldy #$24              ;otherwise load high byte for name table 1
SaveHAdder:
  sty $03               ;save high byte here
  and #$0f              ;mask out high nybble of block buffer pointer
  asl                   ;multiply by 2 to get appropriate name table low byte
  sta $04               ;and then store it here
  lda #$00
  sta $05               ;initialize temp high byte
  lda $02               ;get vertical high nybble offset used in block buffer routine
  clc
  adc #$20              ;add 32 pixels for the status bar
  asl
  rol $05               ;shift and rotate d7 onto d0 and d6 into carry
  asl
  rol $05               ;shift and rotate d6 onto d0 and d5 into carry
  adc $04               ;add low byte of name table and carry to vertical high nybble
  sta $04               ;and store here
  lda $05               ;get whatever was in d7 and d6 of vertical high nybble
  adc #$00              ;add carry
  clc
  adc $03               ;then add high byte of name table
  sta $05               ;store here
  ldy $01               ;get vram buffer offset to be used
  ;fallthrough
.endproc
.proc RemBridge
.export RemBridge
  lda BlockGfxData,x    ;write top left and top right
  sta VRAM_Buffer1+2,y  ;tile numbers into first spot
  lda BlockGfxData+1,x
  sta VRAM_Buffer1+3,y
  lda BlockGfxData+2,x  ;write bottom left and bottom
  sta VRAM_Buffer1+7,y  ;right tiles numbers into
  lda BlockGfxData+3,x  ;second spot
  sta VRAM_Buffer1+8,y
  lda $04
  sta VRAM_Buffer1,y    ;write low byte of name table
  clc                   ;into first slot as read
  adc #$20              ;add 32 bytes to value
  sta VRAM_Buffer1+5,y  ;write low byte of name table
  lda $05               ;plus 32 bytes into second slot
  sta VRAM_Buffer1-1,y  ;write high byte of name
  sta VRAM_Buffer1+4,y  ;table address to both slots
  lda #$02
  sta VRAM_Buffer1+1,y  ;put length of 2 in
  sta VRAM_Buffer1+6,y  ;both slots
  lda #$00
  sta VRAM_Buffer1+9,y  ;put null terminator at end
  ldx $00               ;get offset control bit here
  rts                   ;and leave

BlockGfxData:
  .byte $45, $45, $47, $47
  .byte $47, $47, $47, $47
  .byte $57, $58, $59, $5a
  .byte $24, $24, $24, $24
  .byte $26, $26, $26, $26
.endproc

;-------------------------------------------------------------------------------------
;$00 - temp vram buffer offset
;$01 - temp metatile buffer offset
;$02 - temp metatile graphics table offset
;$03 - used to store attribute bits
;$04 - used to determine attribute table row
;$05 - used to determine attribute table column
;$06 - metatile graphics table address low
;$07 - metatile graphics table address high

RenderAreaGraphics:
            lda CurrentColumnPos         ;store LSB of where we're at
            and #$01
            sta $05
            ldy VRAM_Buffer2_Offset      ;store vram buffer offset
            sty $00
            lda CurrentNTAddr_Low        ;get current name table address we're supposed to render
            sta VRAM_Buffer2+1,y
            lda CurrentNTAddr_High
            sta VRAM_Buffer2,y
            lda #$9a                     ;store length byte of 26 here with d7 set
            sta VRAM_Buffer2+2,y         ;to increment by 32 (in columns)
            lda #$00                     ;init attribute row
            sta $04
            tax
DrawMTLoop: stx $01                      ;store init value of 0 or incremented offset for buffer
            lda MetatileBuffer,x         ;get first metatile number, and mask out all but 2 MSB
            and #%11000000
            sta $03                      ;store attribute table bits here
            asl                          ;note that metatile format is:
            rol                          ;%xx000000 - attribute table bits, 
            rol                          ;%00xxxxxx - metatile number
            tay                          ;rotate bits to d1-d0 and use as offset here
            lda MetatileGraphics_Low,y   ;get address to graphics table from here
            sta $06
            lda MetatileGraphics_High,y
            sta $07
            lda MetatileBuffer,x         ;get metatile number again
            asl                          ;multiply by 4 and use as tile offset
            asl
            sta $02
            lda AreaParserTaskNum        ;get current task number for level processing and
            and #%00000001               ;mask out all but LSB, then invert LSB, multiply by 2
            eor #%00000001               ;to get the correct column position in the metatile,
            asl                          ;then add to the tile offset so we can draw either side
            adc $02                      ;of the metatiles
            tay
            ldx $00                      ;use vram buffer offset from before as X
            lda ($06),y
            sta VRAM_Buffer2+3,x         ;get first tile number (top left or top right) and store
            iny
            lda ($06),y                  ;now get the second (bottom left or bottom right) and store
            sta VRAM_Buffer2+4,x
            ldy $04                      ;get current attribute row
            lda $05                      ;get LSB of current column where we're at, and
            bne RightCheck               ;branch if set (clear = left attrib, set = right)
            lda $01                      ;get current row we're rendering
            lsr                          ;branch if LSB set (clear = top left, set = bottom left)
            bcs LLeft
            rol $03                      ;rotate attribute bits 3 to the left
            rol $03                      ;thus in d1-d0, for upper left square
            rol $03
            jmp SetAttrib
RightCheck: lda $01                      ;get LSB of current row we're rendering
            lsr                          ;branch if set (clear = top right, set = bottom right)
            bcs NextMTRow
            lsr $03                      ;shift attribute bits 4 to the right
            lsr $03                      ;thus in d3-d2, for upper right square
            lsr $03
            lsr $03
            jmp SetAttrib
LLeft:      lsr $03                      ;shift attribute bits 2 to the right
            lsr $03                      ;thus in d5-d4 for lower left square
NextMTRow:  inc $04                      ;move onto next attribute row  
SetAttrib:  lda AttributeBuffer,y        ;get previously saved bits from before
            ora $03                      ;if any, and put new bits, if any, onto
            sta AttributeBuffer,y        ;the old, and store
            inc $00                      ;increment vram buffer offset by 2
            inc $00
            ldx $01                      ;get current gfx buffer row, and check for
            inx                          ;the bottom of the screen
            cpx #$0d
            bcc DrawMTLoop               ;if not there yet, loop back
            ldy $00                      ;get current vram buffer offset, increment by 3
            iny                          ;(for name table address and length bytes)
            iny
            iny
            lda #$00
            sta VRAM_Buffer2,y           ;put null terminator at end of data for name table
            sty VRAM_Buffer2_Offset      ;store new buffer offset
            inc CurrentNTAddr_Low        ;increment name table address low
            lda CurrentNTAddr_Low        ;check current low byte
            and #%00011111               ;if no wraparound, just skip this part
            bne ExitDrawM
            lda #$80                     ;if wraparound occurs, make sure low byte stays
            sta CurrentNTAddr_Low        ;just under the status bar
            lda CurrentNTAddr_High       ;and then invert d2 of the name table address high
            eor #%00000100               ;to move onto the next appropriate name table
            sta CurrentNTAddr_High
ExitDrawM:  jmp SetVRAMCtrl              ;jump to set buffer to $0341 and leave

;-------------------------------------------------------------------------------------
;$00 - temp attribute table address high (big endian order this time!)
;$01 - temp attribute table address low

RenderAttributeTables:
             lda CurrentNTAddr_Low    ;get low byte of next name table address
             and #%00011111           ;to be written to, mask out all but 5 LSB,
             sec                      ;subtract four 
             sbc #$04
             and #%00011111           ;mask out bits again and store
             sta $01
             lda CurrentNTAddr_High   ;get high byte and branch if borrow not set
             bcs SetATHigh
             eor #%00000100           ;otherwise invert d2
SetATHigh:   and #%00000100           ;mask out all other bits
             ora #$23                 ;add $2300 to the high byte and store
             sta $00
             lda $01                  ;get low byte - 4, divide by 4, add offset for
             lsr                      ;attribute table and store
             lsr
             adc #$c0                 ;we should now have the appropriate block of
             sta $01                  ;attribute table in our temp address
             ldx #$00
             ldy VRAM_Buffer2_Offset  ;get buffer offset
AttribLoop:  lda $00
             sta VRAM_Buffer2,y       ;store high byte of attribute table address
             lda $01
             clc                      ;get low byte, add 8 because we want to start
             adc #$08                 ;below the status bar, and store
             sta VRAM_Buffer2+1,y
             sta $01                  ;also store in temp again
             lda AttributeBuffer,x    ;fetch current attribute table byte and store
             sta VRAM_Buffer2+3,y     ;in the buffer
             lda #$01
             sta VRAM_Buffer2+2,y     ;store length of 1 in buffer
             lsr
             sta AttributeBuffer,x    ;clear current byte in attribute buffer
             iny                      ;increment buffer offset by 4 bytes
             iny
             iny
             iny
             inx                      ;increment attribute offset and check to see
             cpx #$07                 ;if we're at the end yet
             bcc AttribLoop
             sta VRAM_Buffer2,y       ;put null terminator at the end
             sty VRAM_Buffer2_Offset  ;store offset in case we want to do any more
SetVRAMCtrl: lda #$06
             sta VRAM_Buffer_AddrCtrl ;set buffer to $0341 and leave
             rts

;-------------------------------------------------------------------------------------

;$00 - used as temporary counter in ColorRotation

ColorRotatePalette:
       .byte $27, $27, $27, $17, $07, $17

BlankPalette:
       .byte $3f, $0c, $04, $ff, $ff, $ff, $ff, $00

;used based on area type
Palette3Data:
       .byte $0f, $07, $12, $0f 
       .byte $0f, $07, $17, $0f
       .byte $0f, $07, $17, $1c
       .byte $0f, $07, $17, $00

ColorRotation:
              lda FrameCounter         ;get frame counter
              and #$07                 ;mask out all but three LSB
              bne ExitColorRot         ;branch if not set to zero to do this every eighth frame
              ldx VRAM_Buffer1_Offset  ;check vram buffer offset
              cpx #$31
              bcs ExitColorRot         ;if offset over 48 bytes, branch to leave
              tay                      ;otherwise use frame counter's 3 LSB as offset here
GetBlankPal:  lda BlankPalette,y       ;get blank palette for palette 3
              sta VRAM_Buffer1,x       ;store it in the vram buffer
              inx                      ;increment offsets
              iny
              cpy #$08
              bcc GetBlankPal          ;do this until all bytes are copied
              ldx VRAM_Buffer1_Offset  ;get current vram buffer offset
              lda #$03
              sta $00                  ;set counter here
              lda AreaType             ;get area type
              asl                      ;multiply by 4 to get proper offset
              asl
              tay                      ;save as offset here
GetAreaPal:   lda Palette3Data,y       ;fetch palette to be written based on area type
              sta VRAM_Buffer1+3,x     ;store it to overwrite blank palette in vram buffer
              iny
              inx
              dec $00                  ;decrement counter
              bpl GetAreaPal           ;do this until the palette is all copied
              ldx VRAM_Buffer1_Offset  ;get current vram buffer offset
              ldy ColorRotateOffset    ;get color cycling offset
              lda ColorRotatePalette,y
              sta VRAM_Buffer1+4,x     ;get and store current color in second slot of palette
              lda VRAM_Buffer1_Offset
              clc                      ;add seven bytes to vram buffer offset
              adc #$07
              sta VRAM_Buffer1_Offset
              inc ColorRotateOffset    ;increment color cycling offset
              lda ColorRotateOffset
              cmp #$06                 ;check to see if it's still in range
              bcc ExitColorRot         ;if so, branch to leave
              lda #$00
              sta ColorRotateOffset    ;otherwise, init to keep it in range
ExitColorRot: rts                      ;leave

;-------------------------------------------------------------------------------------
;METATILE GRAPHICS TABLE

.define MetatileGraphics Palette0_MTiles, Palette1_MTiles, Palette2_MTiles, Palette3_MTiles
MetatileGraphics_Low: .lobytes MetatileGraphics
MetatileGraphics_High: .hibytes MetatileGraphics

Palette0_MTiles:
  .byte $24, $24, $24, $24 ;blank
  .byte $27, $27, $27, $27 ;black metatile
  .byte $24, $24, $24, $35 ;bush left
  .byte $36, $25, $37, $25 ;bush middle
  .byte $24, $38, $24, $24 ;bush right
  .byte $24, $30, $30, $26 ;mountain left
  .byte $26, $26, $34, $26 ;mountain left bottom/middle center
  .byte $24, $31, $24, $32 ;mountain middle top
  .byte $33, $26, $24, $33 ;mountain right
  .byte $34, $26, $26, $26 ;mountain right bottom
  .byte $26, $26, $26, $26 ;mountain middle bottom
  .byte $24, $c0, $24, $c0 ;bridge guardrail
  .byte $24, $7f, $7f, $24 ;chain
  .byte $b8, $ba, $b9, $bb ;tall tree top, top half
  .byte $b8, $bc, $b9, $bd ;short tree top
  .byte $ba, $bc, $bb, $bd ;tall tree top, bottom half
  .byte $60, $64, $61, $65 ;warp pipe end left, points up
  .byte $62, $66, $63, $67 ;warp pipe end right, points up
  .byte $60, $64, $61, $65 ;decoration pipe end left, points up
  .byte $62, $66, $63, $67 ;decoration pipe end right, points up
  .byte $68, $68, $69, $69 ;pipe shaft left
  .byte $26, $26, $6a, $6a ;pipe shaft right
  .byte $4b, $4c, $4d, $4e ;tree ledge left edge
  .byte $4d, $4f, $4d, $4f ;tree ledge middle
  .byte $4d, $4e, $50, $51 ;tree ledge right edge
  .byte $6b, $70, $2c, $2d ;mushroom left edge
  .byte $6c, $71, $6d, $72 ;mushroom middle
  .byte $6e, $73, $6f, $74 ;mushroom right edge
  .byte $86, $8a, $87, $8b ;sideways pipe end top
  .byte $88, $8c, $88, $8c ;sideways pipe shaft top
  .byte $89, $8d, $69, $69 ;sideways pipe joint top
  .byte $8e, $91, $8f, $92 ;sideways pipe end bottom
  .byte $26, $93, $26, $93 ;sideways pipe shaft bottom
  .byte $90, $94, $69, $69 ;sideways pipe joint bottom
  .byte $a4, $e9, $ea, $eb ;seaplant
  .byte $24, $24, $24, $24 ;blank, used on bricks or blocks that are hit
  .byte $24, $2f, $24, $3d ;flagpole ball
  .byte $a2, $a2, $a3, $a3 ;flagpole shaft
  .byte $24, $24, $24, $24 ;blank, used in conjunction with vines

Palette1_MTiles:
  .byte $a2, $a2, $a3, $a3 ;vertical rope
  .byte $99, $24, $99, $24 ;horizontal rope
  .byte $24, $a2, $3e, $3f ;left pulley
  .byte $5b, $5c, $24, $a3 ;right pulley
  .byte $24, $24, $24, $24 ;blank used for balance rope
  .byte $9d, $47, $9e, $47 ;castle top
  .byte $47, $47, $27, $27 ;castle window left
  .byte $47, $47, $47, $47 ;castle brick wall
  .byte $27, $27, $47, $47 ;castle window right
  .byte $a9, $47, $aa, $47 ;castle top w/ brick
  .byte $9b, $27, $9c, $27 ;entrance top
  .byte $27, $27, $27, $27 ;entrance bottom
  .byte $52, $52, $52, $52 ;green ledge stump
  .byte $80, $a0, $81, $a1 ;fence
  .byte $be, $be, $bf, $bf ;tree trunk
  .byte $75, $ba, $76, $bb ;mushroom stump top
  .byte $ba, $ba, $bb, $bb ;mushroom stump bottom
  .byte $45, $47, $45, $47 ;breakable brick w/ line 
  .byte $47, $47, $47, $47 ;breakable brick 
  .byte $45, $47, $45, $47 ;breakable brick (not used)
  .byte $b4, $b6, $b5, $b7 ;cracked rock terrain
  .byte $45, $47, $45, $47 ;brick with line (power-up)
  .byte $45, $47, $45, $47 ;brick with line (vine)
  .byte $45, $47, $45, $47 ;brick with line (star)
  .byte $45, $47, $45, $47 ;brick with line (coins)
  .byte $45, $47, $45, $47 ;brick with line (1-up)
  .byte $47, $47, $47, $47 ;brick (power-up)
  .byte $47, $47, $47, $47 ;brick (vine)
  .byte $47, $47, $47, $47 ;brick (star)
  .byte $47, $47, $47, $47 ;brick (coins)
  .byte $47, $47, $47, $47 ;brick (1-up)
  .byte $24, $24, $24, $24 ;hidden block (1 coin)
  .byte $24, $24, $24, $24 ;hidden block (1-up)
  .byte $ab, $ac, $ad, $ae ;solid block (3-d block)
  .byte $5d, $5e, $5d, $5e ;solid block (white wall)
  .byte $c1, $24, $c1, $24 ;bridge
  .byte $c6, $c8, $c7, $c9 ;bullet bill cannon barrel
  .byte $ca, $cc, $cb, $cd ;bullet bill cannon top
  .byte $2a, $2a, $40, $40 ;bullet bill cannon bottom
  .byte $24, $24, $24, $24 ;blank used for jumpspring
  .byte $24, $47, $24, $47 ;half brick used for jumpspring
  .byte $82, $83, $84, $85 ;solid block (water level, green rock)
  .byte $24, $47, $24, $47 ;half brick (???)
  .byte $86, $8a, $87, $8b ;water pipe top
  .byte $8e, $91, $8f, $92 ;water pipe bottom
  .byte $24, $2f, $24, $3d ;flag ball (residual object)

Palette2_MTiles:
  .byte $24, $24, $24, $35 ;cloud left
  .byte $36, $25, $37, $25 ;cloud middle
  .byte $24, $38, $24, $24 ;cloud right
  .byte $24, $24, $39, $24 ;cloud bottom left
  .byte $3a, $24, $3b, $24 ;cloud bottom middle
  .byte $3c, $24, $24, $24 ;cloud bottom right
  .byte $41, $26, $41, $26 ;water/lava top
  .byte $26, $26, $26, $26 ;water/lava
  .byte $b0, $b1, $b2, $b3 ;cloud level terrain
  .byte $77, $79, $77, $79 ;bowser's bridge
      
Palette3_MTiles:
  .byte $53, $55, $54, $56 ;question block (coin)
  .byte $53, $55, $54, $56 ;question block (power-up)
  .byte $a5, $a7, $a6, $a8 ;coin
  .byte $c2, $c4, $c3, $c5 ;underwater coin
  .byte $57, $59, $58, $5a ;empty block
  .byte $7b, $7d, $7c, $7e ;axe


