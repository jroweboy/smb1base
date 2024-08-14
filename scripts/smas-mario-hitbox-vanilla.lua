
local colorGreen = 0xAA00FF00;
local colorRed   = 0xAAFF0000;
-- hitbox coordinate offsets (x1,y1,x2,y2)
local hitbox_start = 0x7e0f9c;
local sprite_id = 0x7e0010;
local sprite_lookup = {
  -- enemies
  sprite_id+0,
  sprite_id+1,
  sprite_id+2,
  sprite_id+3,
  sprite_id+4,
  sprite_id+5,
  sprite_id+6,
  sprite_id+7,
  -- powerup
  sprite_id+0x08,
  sprite_id+0x09,
  -- fireball
  sprite_id+0x23,
  sprite_id+0x24,
  -- misc
  sprite_id+0x29,
  sprite_id+0x2a,
  sprite_id+0x2b,
  sprite_id+0x2c,
  sprite_id+0x2d,
  sprite_id+0x2e,
  sprite_id+0x2f,
  sprite_id+0x30,
  sprite_id+0x31,
};

-- draw a box and take care of coordinate checking
local function box(x1,y1,x2,y2,color)
  -- gui.text(50,50,x1..","..y1.." "..x2..","..y2);
  if (x1 > 0 and x1 < 255 and x2 > 0 and x2 < 255 and y1 > 0 and y1 < 224 and y2 > 0 and y2 < 224) then
    emu.drawRectangle(x1,y1,x2-x1,y2-y1,color,true);
  end;
end;

function draw_hb(addr, color)
  local x1 = emu.read(addr+0,emu.memType.snesDebug);
  local y1 = emu.read(addr+1,emu.memType.snesDebug) + 8;
  local x2 = emu.read(addr+2,emu.memType.snesDebug);
  local y2 = emu.read(addr+3,emu.memType.snesDebug) + 8;
  box(x1,y1,x2,y2,color);
end

function frame_start()
  local frame_counter = 0x7e0009;
  if (emu.read(frame_counter, emu.memType.snesDebug) & 1 == 0) then
    return
  end
  -- draw marios in green
  draw_hb(hitbox_start, colorGreen);
  for i = 1, 21, 1
  do
    if (emu.read(sprite_lookup[i], emu.memType.snesDebug) > 0) then
      local this_hitbox = hitbox_start + i * 4;
      draw_hb(this_hitbox, colorRed);
    end
  end
end
  
emu.addEventCallback(frame_start, emu.eventType.nmi)