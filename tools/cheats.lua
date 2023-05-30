function getlabel(name)
    label = emu.getLabelAddress(name)
    if label == nil then
        emu.log("Label: " .. name .. " NOT FOUND!")
    end
    return label.address
end

PlayerSize = getlabel("PlayerSize")
PlayerStatus = getlabel("PlayerStatus")

function Main()
	input = emu.getInput(0)
	-- make player have fire flower
	if input.select and input.up then
		emu.write(PlayerSize, 0, emu.memType.nesDebug)
		emu.write(PlayerStatus, 2, emu.memType.nesDebug)
	end
	if input.select and input.down then
		emu.write(PlayerSize, 1, emu.memType.nesDebug)
		emu.write(PlayerStatus, 0, emu.memType.nesDebug)
	end
end

emu.addEventCallback(Main, emu.eventType.startFrame)
emu.displayMessage("Script", "smb1 cheats")