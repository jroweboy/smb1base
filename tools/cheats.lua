function getlabel(name)
    label = emu.getLabelAddress(name)
    if label == nil then
        emu.log("Label: " .. name .. " NOT FOUND!")
    end
    return label.address
end

PlayerSize = getlabel("PlayerSize")
PlayerStatus = getlabel("PlayerStatus")
PlayerAngle = getlabel("PlayerAngle")
PlayerXPos = getlabel("SprObject_X_Position")
PlayerYPos = getlabel("SprObject_Y_Position")
PlayerXSpeed = getlabel("SprObject_X_Speed")
PlayerYSpeed = getlabel("SprObject_Y_Speed")

playersizetoggle = false

function Main()
  input = emu.getInput(0)
  -- make player have fire flower
  if input.select and input.down then
    if playersizetoggle then
      emu.write(PlayerSize, 1, emu.memType.nesDebug)
      emu.write(PlayerStatus, 0, emu.memType.nesDebug)
      playersizetoggle = false
    else
      emu.write(PlayerSize, 0, emu.memType.nesDebug)
      emu.write(PlayerStatus, 2, emu.memType.nesDebug)
      playersizetoggle = true
    end
  end
  -- if input.select and input.right then
  --   angle = emu.read(PlayerAngle, emu.memType.nesDebug, false)
  --   emu.write(PlayerAngle, (angle + 1) & 0xff, emu.memType.nesDebug)
  -- end
    emu.write(PlayerXSpeed, 0, emu.memType.nesDebug)
  if input.select and input.right then
    pos = emu.read(PlayerXPos, emu.memType.nesDebug, false)
    -- emu.write(PlayerXPos, pos + 2, emu.memType.nesDebug)
    emu.write(PlayerXSpeed, 40, emu.memType.nesDebug)
  end
  if input.select and input.left then
    pos = emu.read(PlayerXPos, emu.memType.nesDebug, false)
    -- emu.write(PlayerXPos, pos - 2, emu.memType.nesDebug)
    emu.write(PlayerXSpeed, -40, emu.memType.nesDebug)
  end
  if input.select and input.up then
    pos = emu.read(PlayerYPos, emu.memType.nesDebug, false)
    if pos > 5 then
      emu.write(PlayerYPos, pos - 2, emu.memType.nesDebug)
      emu.write(PlayerYSpeed, 0, emu.memType.nesDebug)
    end
  end
  if input.select and input.down then
    pos = emu.read(PlayerYPos, emu.memType.nesDebug, false)
    emu.write(PlayerYPos, pos + 2, emu.memType.nesDebug)
    emu.write(PlayerYSpeed, 0, emu.memType.nesDebug)
  end

  emu.write(PlayerYSpeed, 0, emu.memType.nesDebug)
end

emu.addEventCallback(Main, emu.eventType.startFrame)
emu.displayMessage("Script", "smb1 cheats")