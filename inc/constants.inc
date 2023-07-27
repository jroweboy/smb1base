
.ifndef __CONSTANTS_INC
__CONSTANTS_INC = 1

;-------------------------------------------------------------------------------------
;DEFINES

;NES specific hardware defines

PPUCTRL               = $2000
PPUMASK               = $2001
PPUSTATUS             = $2002
OAMADDR               = $2003
OAMDATA               = $2004
PPUSCROLL             = $2005
PPUADDR               = $2006
PPUDATA               = $2007

SND_REGISTER          = $4000
SND_SQUARE1_REG       = $4000
SND_SQUARE2_REG       = $4004
SND_TRIANGLE_REG      = $4008
SND_NOISE_REG         = $400c
SND_DELTA_REG         = $4010
SND_MASTERCTRL_REG    = $4015

OAM                   = $02
OAM_DMA               = $4014
JOYPAD_PORT           = $4016
JOYPAD_PORT1          = $4016
JOYPAD_PORT2          = $4017
APU_FRAMECOUNTER      = $4017

;-------------------------------------------------------------------------------------
;CONSTANTS

GLOBAL_BACKGROUND_COLOR = $02

;sound effects constants
Sfx_SmallJump         = %10000000
Sfx_Flagpole          = %01000000
Sfx_Fireball          = %00100000
Sfx_PipeDown_Injury   = %00010000
Sfx_EnemySmack        = %00001000
Sfx_EnemyStomp        = %00000100
Sfx_Bump              = %00000010
Sfx_BigJump           = %00000001

Sfx_BowserFall        = %10000000
Sfx_ExtraLife         = %01000000
Sfx_PowerUpGrab       = %00100000
Sfx_TimerTick         = %00010000
Sfx_Blast             = %00001000
Sfx_GrowVine          = %00000100
Sfx_GrowPowerUp       = %00000010
Sfx_CoinGrab          = %00000001

Sfx_BowserFlame       = %00000010
Sfx_BrickShatter      = %00000001

;music constants
Silence               = %10000000

StarPowerMusic        = %01000000
PipeIntroMusic        = %00100000
CloudMusic            = %00010000
CastleMusic           = %00001000
UndergroundMusic      = %00000100
WaterMusic            = %00000010
GroundMusic           = %00000001

TimeRunningOutMusic   = %01000000
EndOfLevelMusic       = %00100000
AltGameOverMusic      = %00010000
EndOfCastleMusic      = %00001000
VictoryMusic          = %00000100
GameOverMusic         = %00000010
DeathMusic            = %00000001

;enemy object constants 
GreenKoopa            = $00
BuzzyBeetle           = $02
RedKoopa              = $03
HammerBro             = $05
Goomba                = $06
Bloober               = $07
BulletBill_FrenzyVar  = $08
GreyCheepCheep        = $0a
RedCheepCheep         = $0b
Podoboo               = $0c
PiranhaPlant          = $0d
GreenParatroopaJump   = $0e
RedParatroopa         = $0f
GreenParatroopaFly    = $10
Lakitu                = $11
Spiny                 = $12
FlyCheepCheepFrenzy   = $14
FlyingCheepCheep      = $14
BowserFlame           = $15
Fireworks             = $16
BBill_CCheep_Frenzy   = $17
Stop_Frenzy           = $18
Bowser                = $2d
PowerUpObject         = $2e
VineObject            = $2f
FlagpoleFlagObject    = $30
StarFlagObject        = $31
JumpspringObject      = $32
BulletBill_CannonVar  = $33
RetainerObject        = $35
DiscoLakitu           = $36
TallEnemy             = $09

; CHR rom tile constants. PPU address to be specific

BRICK_CHUNK_TILE = $b6

EXPLOSION_TILE_1 = $f0
EXPLOSION_TILE_2 = $f1
EXPLOSION_TILE_3 = $f2

JUMPING_COIN_TILE_1 = $b0
JUMPING_COIN_TILE_2 = $b1
JUMPING_COIN_TILE_3 = $b2
JUMPING_COIN_TILE_4 = $b3

FLOATEY_NUM_00 = $a0
FLOATEY_NUM_10 = $a1
FLOATEY_NUM_20 = $a2
FLOATEY_NUM_40 = $a3
FLOATEY_NUM_50 = $a4
FLOATEY_NUM_80 = $a5
FLOATEY_NUM_0  = $a6
FLOATEY_NUM_1  = $a7
FLOATEY_NUM_UP = $a8

MUSHROOM_TOP_LEFT  = $86
MUSHROOM_TOP_RIGHT = $87
MUSHROOM_BOT_LEFT  = $96
MUSHROOM_BOT_RIGHT = $97

FIREFLOWER_TOP_LEFT = $85
FIREFLOWER_BOT_LEFT = $95

STAR_TOP_LEFT = $88
STAR_BOT_LEFT = $98

PLATFORM_GIRDER = $bc
PLATFORM_CLOUD = $c2

HAMMER_HEAD_1 = $fa
HAMMER_HEAD_2 = $fc
HAMMER_TAIL_1 = $fb
HAMMER_TAIL_2 = $fd

VINE_TILE_1 = $d2
VINE_TILE_2 = $d3

BRICK_BUMP_TILE_1 = $b7
BRICK_BUMP_TILE_2 = $b8
BLOCK_USED_TILE = $b9

FIREBALL_TILE1 = $b4 ; must be consecutive!
; fireball tile 2 must be b5

GOAL_FLAG_TRIANGLE = $d8
GOAL_FLAG_SKULL = $d9

BUBBLE_TILE = $92

STAR_FLAG_TOP_LEFT  = $e4
STAR_FLAG_TOP_RIGHT = $e5
STAR_FLAG_BOT_LEFT  = $f4
STAR_FLAG_BOT_RIGHT = $f5

; Metatile constants
CRACKED_BRICK_NONSOLID  = $88
CLOUD_METATILE          = $89
CRACKED_BRICK_METATILE  = $8a
CRACKED_BRICK_METATILE2 = $8b
BRIDGE_METATILE         = $8c

.enum
  World1
  World2
  World3
  World4
  World5
  World6
  World7
  World8
.endenum

.enum
  Level1
  Level2
  Level3
  Level4
.endenum

.enum
  MODE_TITLESCREEN
  MODE_GAMEPLAY
  MODE_VICTORY
  MODE_GAMEOVER
.endenum


WarmBootOffset        = <DisplayDigits
ColdBootOffset        = <WarmBootValidation

TitleScreenDataOffset = $0ec0                     ; swapped sprites and bg
; TitleScreenDataOffset = $1ec0                     ; address in CHR ROM for titlescreen nametable layout

A_Button              = %10000000
B_Button              = %01000000
Select_Button         = %00100000
Start_Button          = %00010000
Up_Dir                = %00001000
Down_Dir              = %00000100
Left_Dir              = %00000010
Right_Dir             = %00000001


MARIO_ROTATION_ANGLE_MAX = 64
SLINGSHOT_MAGNITUDE_MAX = 48
SLING_INIT_POS = 100

.enum PlayerState
Normal = 0
Jumping = 1
Climbing = 3
Slingshot = 4
AutoWalk = 5
.endenum

.endif ;__CONSTANTS_INC
