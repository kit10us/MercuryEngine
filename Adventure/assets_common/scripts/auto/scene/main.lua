require "prefabs"

function OnBeforeStart()
	-- Load textures
	Texture( "invalid", "borgcube.bmp" )
	Texture( "sand", "sand.bmp" )
	Texture( "grass", "grass.bmp" )
	
	-- Create geometry	
	local terraParams = TerraParameters()	
	terraParams:SetSize( Size2( 2, 2 ) )
	terraParams:SetConstant( 0 )
	terraParams:SetPoints( 10, 10 )
	
	local effect -- used by all terra creation.
	
	-- Invalid --
	effect = Effect( "invalid", "EffectTextured.effect" )
	effect:SetTexture( 0, Texture("invalid" ) )
	terraParams:SetEffect( effect )
	terraParams:SetTexArea( TexArea( 1.0 / 4 * 1, 1.0 / 4 * 1 , 1.0 / 4 * 2, 1.0 / 4 * 2 ) )
	Terra( "invalid", terraParams )
	
	-- Add a grass ground...
	effect = Effect( "grass", "EffectTextured.effect" )
	type( effect );
	effect:SetTexture( 0, Texture( "grass" ) )
	terraParams:SetEffect( effect )
	terraParams:SetTexArea( TexArea() )
	Terra( "grass", terraParams )

	-- Add a sand ground...
	effect = Effect( "sand", "EffectTextured.effect" )	
	effect:SetTexture( 0, Texture( "sand" ) )
	terraParams:SetEffect( effect )
	terraParams:SetTexArea( TexArea( 1.0 / 4 * 1, 1.0 / 4 * 1, 1.0 / 4 * 2, 1.0 / 4 * 2 ) )
	Terra( "sand", terraParams )

	
	-- Scene
	local sceneMain = this;
	
	local proj = MatrixPerspectiveFovLH( math.pi / 4.0, Game.GetWidth()/ Game.GetHeight(), 1, 1000 )
	
	-- Add camera...
	local camera = sceneMain:NewObject( "camera" )
	print( "camera = " .. type( camera ) )
	
	local cameraComponent = CameraComponent()
	camera:Attach( cameraComponent )
	cameraComponent:SetProjection( proj )
	camera:Transform():SetPosition( V3( 0, 17, -12 ) )
	camera:Transform():LookAt( V3( 0, 0, 0 ) )	

	local color3d = Effect( "color3d", "Color_Ambient.effect" )
	local borgcubeEffect = Effect( "borgcube", "EffectBorgCube.effect" )
	
	local cube = sceneMain:NewObject( "cube" )
	cube:AddGeometry( Geometry( "cube", "ShapeCube.shape" ) )
	cube:Transform():SetPosition( V3( 4, 1, 0 ) )

	local pyramid = sceneMain:NewObject( "pyramid" )
	pyramid:AddGeometry( Geometry( "pyramid", "ShapePyramid.shape" ) )
	pyramid:Transform():SetPosition( V3( 8, 1, 0 ) )
	
	local cylinder = sceneMain:NewObject( "cylinder" )
	cylinder:AddGeometry( Geometry( "cylinder", "ShapeCylinder.shape" ) )
	cylinder:Transform():SetPosition( V3( 12, 1, 0 ) )

	local player = sceneMain:NewObject( "player" )
	player:AddGeometry( Geometry( "player", "Mickey_Mouse/Mickey_Mouse.dae" ) )
	player:SetModelMatrix( MatrixRotationX( Angle.Degrees( -90 ) ) )
	player:Transform():SetPosition( V3( 1, 0, 3 ) )
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
	local target = this:FindObject( "player" )
	local cameraMotivator = target:GetComponent( "CameraMotivator" );
	
	if not cameraMotivator then
		print( "Camera motivator NOT FOUND!" )
	else	
		cameraMotivator:SetValue( "speed", 4.0 );
	
		--  Get the InputMotivator from the cameraMotivator component.
		
		--[[
		local motivator = Component( "motivator", cameraMotivator )
		
		if not motivator then
			Debug.LogLine( "Motivator not found!" )
		else
			Debug.LogLine( "Motivator found." )
		end
		--]]

		local motivator = InputMotivator( cameraMotivator );				
		
		local gamepad = Input( "Gamepad" )
		if gamepad then	
			motivator:Add( "moveleft", 	InputCondition( "stick", gamepad, 0, "LeftStick", "x", -1.0, -0.3, 0.0, 0.0 ) )
			motivator:Add( "moveright", InputCondition( "stick", gamepad, 0, "LeftStick", "x", -0.0, -0.0, 0.3, 1.0 ) )
			motivator:Add( "moveup", 	InputCondition( "stick", gamepad, 0, "LeftStick", "y", -0.0, -0.0, 0.3, 1.0 ) )
			motivator:Add( "movedown", 	InputCondition( "stick", gamepad, 0, "LeftStick", "y", -1.0, -0.3, 0.0, 0.0 ) )
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
