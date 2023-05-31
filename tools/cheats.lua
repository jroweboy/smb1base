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

playersizetoggle = false

function Main()
	input = emu.getInput(0)
	-- make player have fire flower
	if input.select and input.up then
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
	if input.select and input.right then
		angle = emu.read(PlayerAngle, emu.memType.nesDebug, false)
		emu.write(PlayerAngle, (angle + 1) & 0xff, emu.memType.nesDebug)
	end
end

emu.addEventCallback(Main, emu.eventType.startFrame)
emu.displayMessage("Script", "smb1 cheats")