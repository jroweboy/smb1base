
-- draw a box and take care of coordinate checking
local function box(x1,y1,x2,y2,color)
  -- gui.text(50,50,x1..","..y1.." "..x2..","..y2);
  if (x1 > 0 and x1 < 255 and x2 > 0 and x2 < 255 and y1 > 0 and y1 < 224 and y2 > 0 and y2 < 224) then
    emu.drawRectangle(x1,y1,x2-x1,y2-y1,color,true);
  end;
end;
-- hitbox coordinate offsets (x1,y1,x2,y2)
local mario_hb = 0x04AC; -- 1x4
local enemy_hb = 0x04B0; -- 5x4
local coin_hb  = 0x04E0; -- 3x4
local fiery_hb = 0x04C8; -- 2x4
local hammer_hb= 0x04D0; -- 9x4
local power_hb = 0x04C4; -- 1x4

-- addresses to check, to see whether the hitboxes should be drawn at all
local mario_ch = 0x000E;
local enemy_ch = 0x000F;
local coin_ch  = 0x0030;
local fiery_ch = 0x0024;
local hammer_ch= 0x002A;
local power_ch = 0x0014;

local mario_x  = 0x0086;
local hscroll  = 0x073f; -- emu.getLabelAddress("HorizontalScroll");
local mario_y  = 0x00ce;

local colorGreen = 0xAA00FF00;
local colorRed   = 0xAAFF0000;

local a,b,c,d;

function frame_start()
  -- from 0x04AC are about 0x48 addresse that indicate a hitbox
  -- different items use different addresses, some share
  -- there can for instance only be one powerup on screen at any time (the star in 1.1 gets replaced by the flower, if you get it)
  -- we cycle through the animation addresses for each type of hitbox, draw the corresponding hitbox if they are drawn
  -- we draw: mario (1), enemies (5), coins (3), hammers (9), powerups (1). (bowser and (his) fireball are considered enemies)

  -- mario
  if (emu.read(mario_hb,emu.memType.cpu) > 0) then 
    a,b,c,d = emu.read(mario_hb,emu.memType.nesDebug),emu.read(mario_hb+1,emu.memType.nesDebug),emu.read(mario_hb+2,emu.memType.nesDebug),emu.read(mario_hb+3,emu.memType.nesDebug);
    box(a,b,c,d, colorGreen);
    a,b = emu.read(mario_x,emu.memType.nesDebug),emu.read(mario_y,emu.memType.nesDebug);
    offset = emu.read(hscroll, emu.memType.nesDebug);
    box(a - offset,b+32,a+2 - offset,b+2+32,colorRed);
  end;
  
  -- enemies
  if (emu.read(enemy_ch  ,emu.memType.nesDebug) > 0) then 
    a,b,c,d = emu.read(enemy_hb,emu.memType.nesDebug),   emu.read(enemy_hb+1,emu.memType.nesDebug), emu.read(enemy_hb+2,emu.memType.nesDebug), emu.read(enemy_hb+3,emu.memType.nesDebug);
    box(a,b,c,d, colorGreen);
  end;
  if (emu.read(enemy_ch+1,emu.memType.nesDebug) > 0) then 
    a,b,c,d = emu.read(enemy_hb+4,emu.memType.nesDebug), emu.read(enemy_hb+5,emu.memType.nesDebug), emu.read(enemy_hb+6,emu.memType.nesDebug), emu.read(enemy_hb+7,emu.memType.nesDebug);
    box(a,b,c,d, colorGreen);
  end;
  if (emu.read(enemy_ch+2,emu.memType.nesDebug) > 0) then 
    a,b,c,d = emu.read(enemy_hb+8,emu.memType.nesDebug), emu.read(enemy_hb+9,emu.memType.nesDebug), emu.read(enemy_hb+10,emu.memType.nesDebug),emu.read(enemy_hb+11,emu.memType.nesDebug);
    box(a,b,c,d, colorGreen);
  end;
  if (emu.read(enemy_ch+3,emu.memType.nesDebug) > 0) then 
    a,b,c,d = emu.read(enemy_hb+12,emu.memType.nesDebug),emu.read(enemy_hb+13,emu.memType.nesDebug),emu.read(enemy_hb+14,emu.memType.nesDebug),emu.read(enemy_hb+15,emu.memType.nesDebug);
    box(a,b,c,d, colorGreen);
  end;
  if (emu.read(enemy_ch+4,emu.memType.nesDebug) > 0) then 
    a,b,c,d = emu.read(enemy_hb+16,emu.memType.nesDebug),emu.read(enemy_hb+17,emu.memType.nesDebug),emu.read(enemy_hb+18,emu.memType.nesDebug),emu.read(enemy_hb+19,emu.memType.nesDebug)
    box(a,b,c,d, colorGreen);
  end;

  -- coins
  if (emu.read(coin_ch  ,emu.memType.nesDebug) > 0) then
    box(
      emu.read(coin_hb,emu.memType.nesDebug),
      emu.read(coin_hb+1,emu.memType.nesDebug),
      emu.read(coin_hb+2,emu.memType.nesDebug),
      emu.read(coin_hb+3,emu.memType.nesDebug),  colorGreen);
  end;
  if (emu.read(coin_ch+1,emu.memType.nesDebug) > 0) then box(emu.read(coin_hb+4,emu.memType.nesDebug), emu.read(coin_hb+5,emu.memType.nesDebug), emu.read(coin_hb+6,emu.memType.nesDebug),  emu.read(coin_hb+7,emu.memType.nesDebug),  colorGreen); end;
  if (emu.read(coin_ch+2,emu.memType.nesDebug) > 0) then box(emu.read(coin_hb+8,emu.memType.nesDebug), emu.read(coin_hb+9,emu.memType.nesDebug), emu.read(coin_hb+10,emu.memType.nesDebug), emu.read(coin_hb+11,emu.memType.nesDebug), colorGreen); end;
  
  -- (mario's) fireballs
  if (emu.read(fiery_ch  ,emu.memType.nesDebug) > 0) then box(emu.read(fiery_hb,emu.memType.nesDebug),   emu.read(fiery_hb+1,emu.memType.nesDebug), emu.read(fiery_hb+2,emu.memType.nesDebug), emu.read(fiery_hb+3,emu.memType.nesDebug),  colorGreen); end;
  if (emu.read(fiery_ch+1,emu.memType.nesDebug) > 0) then box(emu.read(fiery_hb+4,emu.memType.nesDebug), emu.read(fiery_hb+5,emu.memType.nesDebug), emu.read(fiery_hb+6,emu.memType.nesDebug),emu.read(fiery_hb+7,emu.memType.nesDebug), colorGreen); end;
  
  -- hammers
  if (emu.read(hammer_ch  ,emu.memType.nesDebug) > 0) then box(emu.read(hammer_hb,emu.memType.nesDebug),   emu.read(hammer_hb+1,emu.memType.nesDebug), emu.read(hammer_hb+2,emu.memType.nesDebug), emu.read(hammer_hb+3,emu.memType.nesDebug),  colorGreen); end;
  if (emu.read(hammer_ch+1,emu.memType.nesDebug) > 0) then box(emu.read(hammer_hb+4,emu.memType.nesDebug), emu.read(hammer_hb+5,emu.memType.nesDebug), emu.read(hammer_hb+6,emu.memType.nesDebug), emu.read(hammer_hb+7,emu.memType.nesDebug),  colorGreen); end;
  if (emu.read(hammer_ch+2,emu.memType.nesDebug) > 0) then box(emu.read(hammer_hb+8,emu.memType.nesDebug), emu.read(hammer_hb+9,emu.memType.nesDebug), emu.read(hammer_hb+10,emu.memType.nesDebug),emu.read(hammer_hb+11,emu.memType.nesDebug), colorGreen); end;
  if (emu.read(hammer_ch+3,emu.memType.nesDebug) > 0) then box(emu.read(hammer_hb+12,emu.memType.nesDebug),emu.read(hammer_hb+13,emu.memType.nesDebug),emu.read(hammer_hb+14,emu.memType.nesDebug),emu.read(hammer_hb+15,emu.memType.nesDebug), colorGreen); end;
  if (emu.read(hammer_ch+4,emu.memType.nesDebug) > 0) then box(emu.read(hammer_hb+16,emu.memType.nesDebug),emu.read(hammer_hb+17,emu.memType.nesDebug),emu.read(hammer_hb+18,emu.memType.nesDebug),emu.read(hammer_hb+19,emu.memType.nesDebug), colorGreen); end;
  if (emu.read(hammer_ch+5,emu.memType.nesDebug) > 0) then box(emu.read(hammer_hb+20,emu.memType.nesDebug),emu.read(hammer_hb+21,emu.memType.nesDebug),emu.read(hammer_hb+22,emu.memType.nesDebug),emu.read(hammer_hb+23,emu.memType.nesDebug), colorGreen); end;
  if (emu.read(hammer_ch+6,emu.memType.nesDebug) > 0) then box(emu.read(hammer_hb+24,emu.memType.nesDebug),emu.read(hammer_hb+25,emu.memType.nesDebug),emu.read(hammer_hb+26,emu.memType.nesDebug),emu.read(hammer_hb+27,emu.memType.nesDebug), colorGreen); end;
  if (emu.read(hammer_ch+7,emu.memType.nesDebug) > 0) then box(emu.read(hammer_hb+28,emu.memType.nesDebug),emu.read(hammer_hb+29,emu.memType.nesDebug),emu.read(hammer_hb+30,emu.memType.nesDebug),emu.read(hammer_hb+31,emu.memType.nesDebug), colorGreen); end;
  if (emu.read(hammer_ch+8,emu.memType.nesDebug) > 0) then box(emu.read(hammer_hb+32,emu.memType.nesDebug),emu.read(hammer_hb+33,emu.memType.nesDebug),emu.read(hammer_hb+34,emu.memType.nesDebug),emu.read(hammer_hb+35,emu.memType.nesDebug), colorGreen); end;

  -- powerup
  if (emu.read(power_ch,emu.memType.nesDebug) > 0) then box(emu.read(power_hb,emu.memType.nesDebug),emu.read(power_hb+1,emu.memType.nesDebug),emu.read(power_hb+2,emu.memType.nesDebug),emu.read(power_hb+3,emu.memType.nesDebug), colorGreen); end;

end
  
emu.addEventCallback(frame_start, emu.eventType.nmi)