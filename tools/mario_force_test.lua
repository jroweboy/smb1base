
local mario_hb = 0x04AC; -- 1x4
local colorGreen = 0xAA00FF00;
local colorRed   = 0xAAFF0000;

previousMouse = emu.getMouseState()
drawingLine = false
startX = 0
startY = 0
endX = 0
endY = 0

ForceJump = false

PlayerStateAddr = emu.getLabelAddress("Player_State").address
VerticalForceAddr = emu.getLabelAddress("VerticalForce").address
PlayerYSpeedAddr = emu.getLabelAddress("SprObject_Y_Speed").address
PlayerXAddr = emu.getLabelAddress("SprObject_X_Position").address
PlayerYAddr = emu.getLabelAddress("SprObject_Y_Position").address
PlayerOffsetAddr = emu.getLabelAddress("HorizontalScroll").address

PlayerX = 0
PlayerY = 0
PlayerOffset = 0
VerticalForce = 0x20
PlayerYSpeed = 0xfc
VerticalForceDown = 0x40

-- draw a box and take care of coordinate checking
local function box(x1,y1,x2,y2,color)
  -- gui.text(50,50,x1..","..y1.." "..x2..","..y2);
  if (x1 > 0 and x1 < 255 and x2 > 0 and x2 < 255 and y1 > 0 and y1 < 224 and y2 > 0 and y2 < 224) then
    emu.drawRectangle(x1,y1,x2-x1,y2-y1,color,true);
  end;
end;

function mouseDrawLine()
	if (emu.read(mario_hb,emu.memType.nesDebug) == 0) then
		return
	end
	PlayerOffset = emu.read(PlayerOffsetAddr, emu.memType.nesDebug);
	PlayerX = emu.read(PlayerXAddr, emu.memType.nesDebug) + 8 - PlayerOffset;
	PlayerY = emu.read(PlayerYAddr, emu.memType.nesDebug) - 8 + 32;
	-- box(PlayerX, PlayerY + 32, PlayerX + 2 - PlayerOffset, PlayerY + 2 + 32, colorRed);
	
	mouse = emu.getMouseState()
	if previousMouse.left == false and mouse.left == true then
		-- capture the current position and start drawing the line
		startX = mouse.x
		startY = mouse.y
		endX = mouse.x
		endY = mouse.y
		drawingLine = true
	elseif previousMouse.left == true and mouse.left == false then
		-- left mouse button released so apply velocity
		endX = mouse.x
		endY = mouse.y
		drawingLine = false
		ForceJump = true
		VerticalForce = 0x20
		PlayerYSpeed = 0xfc
		VerticalForceDown = 0x40
	elseif mouse.left == true then
		-- while dragging the mouse update the end position every frame
		endX = mouse.x
		endY = mouse.y
	end
	  
	if drawingLine == true then
		emu.drawLine(PlayerX, PlayerY, PlayerX - (startX - endX), PlayerY - (startY - endY))
	end
	
	previousMouse = mouse
	
	if ForceJump == true then
		StartJump = 0x1
		emu.write(PlayerStateAddr, StartJump, emu.memType.nesDebug)
		emu.write(PlayerYSpeedAddr, PlayerYSpeed, emu.memType.nesDebug)
		emu.write(VerticalForceAddr, VerticalForce, emu.memType.nesDebug)
		ForceJump = false
	end
end

emu.addEventCallback(mouseDrawLine, emu.eventType.nmi)
