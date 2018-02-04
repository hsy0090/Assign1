--Saving score to file
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

--Calculate dist
function CalculateDistanceSquare(x1,y1,z1,x2,y2,z2)
	local distanceSquare = (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1) + (z2 - z1)*(z2 - z1)
	print(distanceSquare)
	return distanceSquare
end

--Variable Values
function GetMinMax(...)
	
	local sum = 0
	local n = 0
	local ave = 0

	--for i,v in ipairs(arg) do
	--	print(i, v)
	--end

	--local minimum = math.min(...)
	--local maximum = math.max(...)
	
	print(minimum)
	return  arg.n,ave,maximum,minimum
end

title = "DM2240 - Week 14 Scripting"

--Window size
width = 800
height = 600

--Player Init
playerPos = {100,0,200};

--Keyboard Inputs
mouseSensitivity = 400
keyFORWARD = "W"
keyBACKWARD = "S"
keyLEFT = "A"
keyRIGHT = "D"