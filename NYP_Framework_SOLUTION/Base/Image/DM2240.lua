function SaveToLuaFile(outputString, overwrite)
   print("SaveToLuaFile...")

   local f;						-- The file
   if overwrite == 1 then		-- Wipe the contents with new data
      f = assert(io.open("Image/DM2240_HighScore.lua", "w"))
   elseif overwrite == 0 then	-- Append with new data
      f = assert(io.open("Image/DM2240_HighScore.lua", "a"))
   end
   -- Write to the file
   f:write(outputString)
   -- Close the file
   f:close()
   print("OK")
end
	
function CalculateDistanceSquare(x1,y1,z1,x2,y2,z2)
	local distanceSquare = (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1) + (z2 - z1)*(z2 - z1)
	print(distanceSquare)
	return distanceSquare
end

function GetMinMax(...)
	local minValue = -1
	local maxValue = -1
	local avgValue = -1
	local numValues = 0
	for i,v in ipairs(arg) do
		if minValue == -1 then
			minValue = v
		elseif v < minValue then
			minValue = v
		end

		if maxValue == -1 then
			maxValue = v
		elseif v > maxValue then
			maxValue = v
		end

		avgValue = avgValue + v
		numValues = numValues + 1
    end
	avgValue = avgValue / numValues
	print(minValue, maxValue, avgValue, numValues)
	return minValue, maxValue, avgValue, numValues
end

title = "DM2240 - Week 14 Scripting"
width = 800
height = 600

-- Keyboard Inputs
moveForward  = "W"
moveBackward = "S"
moveLeft     = "A"
moveRight    = "D"
reload		 = "R"
changeSweapon = "Q" 
mouseSensitivity = 40.0
jump		 = VK_SPACE

-- Game play
--player
health = 100.0
numberofPweapons = 3
numberofSweapons = 2
--Game
timer = 100.0

-- Default Entities Spawn positions
--Ground Terrain
GroundPos = {0.0, -10.0, 0.0}
GroundSize = {100.0, 100.0, 100.0}
GroundGrid = {10.0, 1.0, 10.0}
--CPlayerInfo start position
CPlayerInfoStartPos = {-40,0, -40}
CottagePos = {-100.0, -10.0, 0.0}
Create1Pos = {-20.0, -6.0, -20.0}
Create2Pos = {100.0, -6.0, 10.0}
--Tank
TankInitPos = { 0.0, 0.0, 0.0}
TankSize = { 2.0, 2.0, 2.0 }
TankBasePos = {basex=0.0, basey=-10.0, basez=-300.0}
TankBaseUpdate = { x=0.0, y=0.0, z=0.5}
TankBaseAnimate = { Min=-280, Max=90}
TankTurretUpdate = { r=1.0, x=0.0, y=1.0, z=0.0}
TankTurretAnimate = { Min=-90, Max=90}
TankGunUpdate = { r=0.1, x=1.0, y=0.0, z=0.0}
TankGunAnimate = { Min=-15, Max=10}
--Enemies
Enemy1Pos = {160, 10, 60}
Enemy2Pos = {0, 0, 160}
Enemy3Pos = {70, 0, -100}

-- Way points
Waypoint_A_1 = {x=80.0, y=0.0, z=60.0}
Waypoint_A_2 = {x=80.0, y=0.0, z=-60.0}
Waypoint_A_3 = {x=160.0, y=0.0, z=0.0}
Waypoint_B_1 = {x=0.0, y=0.0, z=160.0}
Waypoint_B_2 = {x=0.0, y=0.0, z=60.0}
Waypoint_B_3 = {x=-40.0, y=0.0, z=110.0}
Waypoint_C_1 = {x=100.0, y=0.0, z=-100.0}
Waypoint_C_2 = {x=40.0, y=0.0, z=-100.0}