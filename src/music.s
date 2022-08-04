.include "common.inc"

;-------------------------------------------------------------------------------------

MusicSelectData:
      .byte WaterMusic, GroundMusic, UndergroundMusic, CastleMusic
      .byte CloudMusic, PipeIntroMusic

.proc GetAreaMusic
.export GetAreaMusic

  lda OperMode           ;if in title screen mode, leave
  beq ExitGetM
  lda AltEntranceControl ;check for specific alternate mode of entry
  cmp #$02               ;if found, branch without checking starting position
  beq ChkAreaType        ;from area object data header
  ldy #$05               ;select music for pipe intro scene by default
  lda PlayerEntranceCtrl ;check value from level header for certain values
  cmp #$06
  beq StoreMusic         ;load music for pipe intro scene if header
  cmp #$07               ;start position either value $06 or $07
  beq StoreMusic
ChkAreaType:
  ldy AreaType           ;load area type as offset for music bit
  lda CloudTypeOverride
  beq StoreMusic         ;check for cloud type override
  ldy #$04               ;select music for cloud type level if found
StoreMusic:
  lda MusicSelectData,y  ;otherwise select appropriate music for level type
  sta AreaMusicQueue     ;store in queue and leave
ExitGetM:
  rts
.endproc
