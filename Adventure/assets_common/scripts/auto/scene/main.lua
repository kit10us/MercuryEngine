require "prefabs"
require "terrain"

function OnBeforeStart()
	CreateTerrain( Size2( 30, 30 ),  Size2( 4, 4 ) )
	
	local proj = MatrixPerspectiveFovLH( math.pi / 4.0, Game.GetWidth()/ Game.GetHeight(), 1, 1000 )
	
	-- Add camera...
	local camera = this:NewObject( "camera" )
	
	local cameraComponent = CameraComponent()
	camera:Attach( cameraComponent )
	cameraComponent:SetProjection( proj )
	camera:Transform():SetPosition( V3( 0, 17, -12 ) )
	camera:Transform():LookAt( V3( 0, 0, 0 ) )	

	local color3d = Effect( "color3d", "Color_Ambient.effect" )
	local borgcubeEffect = Effect( "borgcube", "EffectBorgCube.effect" )
	
	local cube = this:NewObject( "cube" )
	cube:AddGeometry( Geometry( "cube", "ShapeCube.shape" ) )
	cube:Transform():SetPosition( V3( 0, 1, 0 ) )

	local pyramid = this:NewObject( "pyramid" )
	pyramid:AddGeometry( Geometry( "pyramid", "ShapePyramid.shape" ) )
	pyramid:Transform():SetPosition( V3( -4, 1, 0 ) )
	
	local cylinder = this:NewObject( "cylinder" )
	cylinder:AddGeometry( Geometry( "cylinder", "ShapeCylinder.shape" ) )
	cylinder:Transform():SetPosition( V3( 0, 1, 4 ) )

	local player = this:NewObject( "player" )
	player:AddGeometry( Geometry( "player", "Mickey_Mouse/Mickey_Mouse.dae" ) )
	player:SetModelMatrix( MatrixRotationX( Angle.Degrees( -90 ) ) )
	player:Transform():SetPosition( V3( 32, 0, -32 ) )
	player:Transform():PreMul( MatrixRotationY( Angle.Degrees( 180 ) ) )
	
	-- Add stuff
	BuildTree( V3( -8, 0, 16 ) )	
	BuildTree( V3( -3, 0, 11 ) )	
	BuildTree( V3( 2, 0, 8 ) )	
	BuildTree( V3( -4, 0, 7 ) )	
	BuildTree( V3( 0, 0, 5 ) )	
	BuildHouse( V3( 7, 0, 12 ) )
end

function OnAfterStart()
	print( this:SendCommand( "DrawOnMap", "16, 14, sand" ) )

	-- Top-Left "L"
	print( this:SendCommand( "DrawOnMap", "16, 12, sand" ) )
	print( this:SendCommand( "DrawOnMap", "17, 12, sand" ) )
	print( this:SendCommand( "DrawOnMap", "16, 11, sand" ) )

	-- Top-Right "L"
	print( this:SendCommand( "DrawOnMap", "21, 12, sand" ) )
	print( this:SendCommand( "DrawOnMap", "22, 12, sand" ) )
	print( this:SendCommand( "DrawOnMap", "22, 11, sand" ) )

	-- Bott0m-Left "L"
	print( this:SendCommand( "DrawOnMap", "16, 8, sand" ) )
	print( this:SendCommand( "DrawOnMap", "16, 7, sand" ) )
	print( this:SendCommand( "DrawOnMap", "17, 7, sand" ) )

	-- Botton-Right "L"
	print( this:SendCommand( "DrawOnMap", "22, 8, sand" ) )
	print( this:SendCommand( "DrawOnMap", "21, 7, sand" ) )
	print( this:SendCommand( "DrawOnMap", "22, 7, sand" ) )
	

	-- Center "+"

	print( this:SendCommand( "DrawOnMap", "19, 12, sand" ) )
	print( this:SendCommand( "DrawOnMap", "19, 11, sand" ) )

	print( this:SendCommand( "DrawOnMap", "17, 10, sand" ) )
	print( this:SendCommand( "DrawOnMap", "18, 10, sand" ) )
	print( this:SendCommand( "DrawOnMap", "19, 10, sand" ) )
	print( this:SendCommand( "DrawOnMap", "20, 10, sand" ) )
	print( this:SendCommand( "DrawOnMap", "21, 10, sand" ) )
	
	print( this:SendCommand( "DrawOnMap", "19, 9, sand" ) )
	print( this:SendCommand( "DrawOnMap", "19, 8, sand" ) )
	
	
	
	
	local target = this:FindObject( "player" )
	local cameraMotivator = target:GetComponent( "CameraMotivator" );
	
	if not cameraMotivator then
		print( "Camera motivator NOT FOUND!" )
	else	
		cameraMotivator:SetValue( "speed", 4.0 );
		
		local gamepad = Input( "Gamepad" )
		if gamepad then	
			
			local cap_low = V3(-1.0 * 1.0)
			local threshold_low = V3(-1.0 * 0.3)
			local threshold_high = V3(1.0 * 0.3)
			local cap_high = V3(1.0 * 1.0)
			--motivator:Add( "move", 	InputCondition( "stick", gamepad, 0, "LeftStick", cap_low, threshold_low, threshold_high, cap_high ) )
		end

		--[[
		local keyboard = Input( "Keyboard" )	
		if keyboard then
			motivator:Add( "moveleft", 	InputCondition( "button", keyboard, 0, "Left", "Down" ) )
			motivator:Add( "moveright",	InputCondition( "button", keyboard, 0, "Right", "Down" ) )
			motivator:Add( "moveup", 	InputCondition( "button", keyboard, 0, "Up", "Down" ) )
			motivator:Add( "movedown", 	InputCondition( "button", keyboard, 0, "Down", "Down" ) )
		end	
		--]]
	end
end
