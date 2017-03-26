require "prefabs"
require "terrain"


function SetPositionObjectAction( object, size, target )
	local physics = MEPhysics()
	local collider = physics:CreateBoxCollider( size )
	object:AddComponent( collider:AsObjectComponent() )
	local document = XMLDocument()
	local element = document:AddElement( "SetPosition" )
	element:AddAttribute( "type", "object" )
	element:AddText( target:GetName() )
	collider:SetOnEnter( ObjectAction( element ) )	
end

function SetPositionAbsoluteAction( object, size, position )
	local physics = MEPhysics()
	local collider = physics:CreateBoxCollider( size )
	object:AddComponent( collider:AsObjectComponent() )
	local document = XMLDocument()
	local element = document:AddElement( "SetPosition" )
	element:AddAttribute( "type", "absolute" )
	element:AddText( tostring( position ) )
	collider:SetOnEnter( ObjectAction( element ) )	
end

function BuildCube( position )
	local object = this:NewObject( MakeObjectName( "cube" ) )
	local cube = Geometry( "cube", "ShapeCube.shape" )
	object:AddGeometry( cube )	
	object:SetModelMatrix( Matrix() * MatrixTranslate( V3( 0, 1, 0 ) ) )
	object:Transform():SetPosition( position )
	return object
end

function BuildSphere( position )
	local object = this:NewObject( MakeObjectName( "sphere" ) )
	local sphere = Geometry( "sphere", "ShapeSphere.shape" )
	object:AddGeometry( sphere )	
	object:SetModelMatrix( Matrix() * MatrixTranslate( V3( 0, 1, 0 ) ) )
	object:Transform():SetPosition( position )
	return object
end

function OnBeforeStart()
	local start_position = V3( 4, 0, 0 )


	CreateTerrain( Size2( 30, 30 ),  Size2( 4, 4 ) )
	
	local proj = MatrixPerspectiveFovLH( math.pi / 4.0, Game.GetWidth()/ Game.GetHeight(), 1, 1000 )
	
	-- Add camera...
	local camera = this:NewObject( "camera" )
	
	local cameraComponent = CameraComponent()
	camera:AddComponent( cameraComponent )
	cameraComponent:SetProjection( proj )
	camera:Transform():SetPosition( V3( 0, 17, -12 ) )
	camera:Transform():LookAt( V3( 0, 0, 0 ) )	

	local color3d = Effect( "color3d", "Color_Ambient.effect" )
	local borgcubeEffect = Effect( "borgcube", "EffectBorgCube.effect" )
	
	local pyramid = this:NewObject( "pyramid" )
	pyramid:AddGeometry( Geometry( "pyramid", "ShapePyramid.shape" ) )
	pyramid:SetModelMatrix( Matrix() * MatrixTranslate( V3( 0, 1, 0 ) ) )
	pyramid:Transform():SetPosition( V3( 0, 0, 0 ) )
	
	-- Create sky castle...
	BuildCube(   V3( 4 + 0, 28, 0 ) )
	BuildCube(   V3( 4 + 2, 28, 0 ) )
	BuildCube(   V3( 4 + 4, 28, 0 ) )
	BuildCube(   V3( 4 + 4, 28, 2 ) )
	BuildCube(   V3( 4 + 4, 28, 4 ) )
	BuildCube(   V3( 4 + 4, 28, 6 ) )
	local sphere = BuildSphere( V3( 4 + 4, 30, 6 ) )
	
	
	-- Physics tests...
	
	local physics = MEPhysics()

	-- Create jump objects...
	
	local jump1 = BuildCube( V3( 13, 1, 8 ) )	
	local jump2 = BuildCube( V3( 16, 1, -25 ) )

	SetPositionObjectAction( jump1, V3( 2 ), jump2 )
	SetPositionObjectAction( jump2, V3( 2 ), pyramid )
	SetPositionAbsoluteAction( pyramid, V3( 2 ), V3( 4, 30, 0 ) )
	SetPositionAbsoluteAction( sphere, V3( 2 ), start_position ) 

	

	local player = this:NewObject( "player" )
	player:AddGeometry( Geometry( "player", "Mickey_Mouse/Mickey_Mouse.dae" ) )
	player:SetModelMatrix( MatrixRotationX( Angle.Degrees( -90 ) ) )
	player:Transform():SetPosition( start_position )
	player:Transform():PreMul( MatrixRotationY( Angle.Degrees( 180 ) ) )
	player:AddComponent( physics:CreateEntity():AsObjectComponent() )
	
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
	this:SendCommand( "DrawOnMap", "16, 12, sand" )
	this:SendCommand( "DrawOnMap", "17, 12, sand" )
	this:SendCommand( "DrawOnMap", "16, 11, sand" )

	-- Top-Right "L"
	this:SendCommand( "DrawOnMap", "21, 12, sand" )
	this:SendCommand( "DrawOnMap", "22, 12, sand" )
	this:SendCommand( "DrawOnMap", "22, 11, sand" )

	-- Bott0m-Left "L"
	this:SendCommand( "DrawOnMap", "16, 8, sand" ) 
	this:SendCommand( "DrawOnMap", "16, 7, sand" ) 
	this:SendCommand( "DrawOnMap", "17, 7, sand" ) 

	-- Botton-Right "L"
	this:SendCommand( "DrawOnMap", "22, 8, sand" ) 
	this:SendCommand( "DrawOnMap", "21, 7, sand" ) 
	this:SendCommand( "DrawOnMap", "22, 7, sand" ) 
	

	-- Center "+"

	this:SendCommand( "DrawOnMap", "19, 12, sand" )
	this:SendCommand( "DrawOnMap", "19, 11, sand" )

	this:SendCommand( "DrawOnMap", "17, 10, sand" )
	this:SendCommand( "DrawOnMap", "18, 10, sand" )
	this:SendCommand( "DrawOnMap", "19, 10, sand" )
	this:SendCommand( "DrawOnMap", "20, 10, sand" )
	this:SendCommand( "DrawOnMap", "21, 10, sand" )
	
	this:SendCommand( "DrawOnMap", "19, 9, sand" ) 
	this:SendCommand( "DrawOnMap", "19, 8, sand" ) 
	
	
	
	
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
