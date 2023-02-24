
local mario_hb = 0x04AC; -- 1x4
local mario_x  = 0x0086;
local hscroll  = 0x073f; -- emu.getLabelAddress("HorizontalScroll");
local mario_y  = 0x00ce;
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

PlayerX = emu.getLabelAddress("SprObject_X_Position").address
PlayerY = emu.getLabelAddress("SprObject_Y_Position").address

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
	
	if (emu.read(mario_hb,emu.memType.nesDebug) > 0) then 
		a = emu.read(mario_hb,emu.memType.nesDebug);
		b = emu.read(mario_hb+1,emu.memType.nesDebug);
		c = emu.read(mario_hb+2,emu.memType.nesDebug);
		d = emu.read(mario_hb+3,emu.memType.nesDebug);
		box(a,b,c,d, colorGreen);
		a = emu.read(mario_x, emu.memType.nesDebug);
		b = emu.read(mario_y, emu.memType.nesDebug);
		offset = emu.read(hscroll, emu.memType.nesDebug);
		box(a - offset,b+32,a+2 - offset,b+2+32,colorRed);
	end;
  
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
