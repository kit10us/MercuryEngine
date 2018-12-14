function Action_Use()
	local this = Scene()
	local player = this:FindObject( "player" )
	local withinDistance = 5.0 -- Because we aren't always on the same level.

	local objects = this:FindObjectsWithinSphere( player:GetPosition(), withinDistance )

	print( "Performing ray test..." )
	print( "  origin    { " .. tostring( player:GetPosition() ) .. " }" )
	print( "  direction { " .. tostring( player:GetForward() ) .. " }" )
	print( "  distance  = " .. withinDistance )
	
	print( "  Object = " .. #objects .. "..." )	
	
	-- Use resource...
	if #objects > 0 then
		local object = objects[ 1 ].object
		local distance = objects[ 1 ].distance
		if distance < 2 then 

			local objPos = object:GetPosition()
			print( "   " .. object:GetName() .. " @ " .. distance .. " { " .. tostring( objPos ) .. " }" )

			if object:HasTag( "resource" ) then
				print( "here" )
				local value = tonumber( object:GetTagValue( "value" ) );
				print( value )
				if value > 0 then
					print( "here 2" )
					--value = value - 1;
					value = 0
					object:AddTag( "value", value );
					if value == 0 then
						this:DestroyObject( object );
						print( "here 3" )
					end
				end
			end			
		end
	end

	--[[
	-- Test
	for i = 1, #objects do
		local object = objects[ i ].object
		local distance = objects[ i ].distance
		local objPos = object:GetPosition()
		print( "   " .. object:GetName() .. " @ " .. distance .. " { " .. tostring( objPos ) .. " }" )
	end
	--]]
	
	

end
