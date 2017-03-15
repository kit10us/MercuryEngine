function MakeTree( scene, position )
	local name
	
	-- Find the first name available...
	for i = 0, 1000, 1 do
		name = "tree_" .. i
		if not scene:FindObject( name )  then		
			break
		end
	end	

	-- Add our object...
	local object = scene:NewObject( name )
	object:Transform():SetPosition( position )
		

	local leaves = Effect( "leaves", "Textured_Ambient.effect" )
	leaves:SetTexture( 0, Texture( "leaves", "leafsolid.bmp" ) )

		
	sphereParameters = ShapeParameters( "sphere" )
	sphereParameters:SetEffect( leaves )
	sphereParameters:SetSegments( 24 )
	sphereParameters:SetRadius( 2.0 )
	sphereParameters:SetDiffuse( Color.NewRGB( 0, 0.8, 0 ) )
	sphereParameters:SetCenter( V3.New( 0, 5, 0 ) );
	object:AddGeometry( Geometry( sphereParameters ) )

	cylinderParameters = ShapeParameters( "cylinder" )
	cylinderParameters:SetEffect( color3d )
	cylinderParameters:SetSegments( 24 )
	cylinderParameters:SetRadius( 0.7 )
	cylinderParameters:SetHeight( 5.0 )
	cylinderParameters:SetDiffuse( Color.NewRGB( 107/255, 88/255, 37/255 ) )
	cylinderParameters:SetCaps( true )
	cylinderParameters:SetCenter( V3.New( 0, 2.5, 0 ) )
	object:AddGeometry( Geometry( cylinderParameters ) )
end

function MakeHouse( scene, position )
	local name
	
	-- Find the first name available...
	for i = 0, 1000, 1 do
		name = "house_" .. i
		if not scene:FindObject( name )  then		
			break
		end
	end	

	-- Add our object...
	local object = scene:NewObject( name )
	object:Transform():SetPosition( position )

	-- Walls
	parameters = ShapeParameters( "cube" )
	parameters:SetEffect( color3d )
	parameters:SetSize3( Size3.New( 6, 6, 10 ) )
	parameters:SetDiffuse( Color.Mul( Color.NewRGB( .2, 0.1, 0.1 ), Color.NewWhite( 2 ) ) )
	parameters:SetCenter( V3.New( 0, 3, 0 ) );
	object:AddGeometry( Geometry( parameters ) )

	-- door
	parameters = ShapeParameters( "cube" )
	parameters:SetEffect( color3d )
	parameters:SetSize3( Size3.New( 2.6, 5, 1 ) )
	parameters:SetDiffuse( Color.Mul( Color.NewRGB( 107/255, 88/255, 37/255 ), Color.NewWhite( 2 ) )) 
	parameters:SetCenter( V3.New( 0, 2.5, -5 ) );
	object:AddGeometry( Geometry( parameters ) )
	
	-- roof
	parameters = ShapeParameters( "pyramid" )
	parameters:SetEffect( color3d )
	parameters:SetSize3( Size3.New( 7, 2, 11 ) )
	parameters:SetDiffuse( Color.Mul( Color.NewRGB( .3, .3, .3 ), Color.NewWhite( 2 ) ) )
	parameters:SetCenter( V3.New( 0, 7, 0 ) );
	object:AddGeometry( Geometry( parameters ) )
end

function OnBeforeStartup( me )
	-- Load textures
	Texture( "invalid", "borgcube.bmp" )
	Texture( "sand", "sand.bmp" )
	Texture( "grass", "grass.bmp" )
	
	-- Create geometry	
	local terraParams = TerraParameters()	
	terraParams:SetSize( Size2.New( 2, 2 ) )
	terraParams:SetConstant( 0 )
	terraParams:SetPoints( 10, 10 )
	
	local effect -- used by all terra creation.
	
	-- Invalid --
	effect = Effect( "invalid", "EffectTextured.effect" )
	effect:SetTexture( 0, Texture("invalid" ) )
	terraParams:SetEffect( effect )
	terraParams:SetTexArea( TexArea.New( 1.0 / 4 * 1, 1.0 / 4 * 1 , 1.0 / 4 * 2, 1.0 / 4 * 2 ) )
	Terra( "invalid", terraParams )
	
	-- Add a grass ground...
	effect = Effect( "grass", "EffectTextured.effect" )
	effect:SetTexture( 0, Texture( "grass" ) )
	terraParams:SetEffect( effect )
	terraParams:SetTexArea( TexArea.NewFull() )
	Terra( "grass", terraParams )

	-- Add a sand ground...
	effect = Effect( "sand", "EffectTextured.effect" )	
	effect:SetTexture( 0, Texture( "sand" ) )
	terraParams:SetEffect( effect )
	terraParams:SetTexArea( TexArea.New( 1.0 / 4 * 1, 1.0 / 4 * 1, 1.0 / 4 * 2, 1.0 / 4 * 2 ) )
	Terra( "sand", terraParams )
	
	-- Scene
	local sceneMain = Scene( "main" )
	
	local proj = Matrix.NewPerspectiveFovLH( math.pi / 4.0, Game.GetWidth()/ Game.GetHeight(), 1, 1000 )
	
	-- Add camera...
	local camera = sceneMain:NewObject( "camera" )
	local cameraComponent = CameraComponent()
	cameraComponent:AttachTo( camera )
	cameraComponent:SetProjection( proj )
	camera:Transform():SetPosition( V3.New( 0, 17, -12 ) )
	camera:Transform():LookAt( V3.New( 0, 0, 0 ) )	

	color3d = Effect( "color3d", "Color_Ambient.effect" )
	borgcubeEffect = Effect( "borgcube", "EffectBorgCube.effect" )
	
	local cube = sceneMain:NewObject( "cube" )
	cube:AddGeometry( Geometry( "cube", "ShapeCube.shape" ) )
	cube:Transform():SetPosition( V3.New( -4.5, 1, 0 ) )

	local pyramid = sceneMain:NewObject( "pyramid" )
	pyramid:AddGeometry( Geometry( "pyramid", "ShapePyramid.shape" ) )
	pyramid:Transform():SetPosition( V3.New( -4.5, 1, 0 ) )
	
	local cylinder = sceneMain:NewObject( "cylinder" )
	cylinder:AddGeometry( Geometry( "cylinder", "ShapeCylinder.shape" ) )
	cylinder:Transform():SetPosition( V3.New( 4.5, 1, 0 ) )

	local player = sceneMain:NewObject( "player" )
	player:AddGeometry( Geometry( "player", "Mickey_Mouse/Mickey_Mouse.dae" ) )
	player:SetModelMatrix( Matrix.NewRotationX( Angle.Degrees( -90 ) ) )
	player:Transform():SetPosition( V3.New( 0, 0, 0 ) )
	
	-- Add stuff
	MakeTree( sceneMain, V3.New( 0, 0, 5 ) )
	
	MakeHouse( sceneMain, V3.New( -7, 0, 3 ) )
	
end

function OnAfterStartup( me )
	local sceneMain = FindScene( "main" )
	local target = sceneMain:FindObject( "player" )
		
	local cameraMotivator = target:GetComponent( "CameraMotivator" );
	
	if not cameraMotivator then
		Debug.LogLine( "Camera motivator NOT FOUND!" )
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
		local keyboard = Input( "Keyboard" )
		if gamepad then	
			motivator:Add( "moveleft", 	InputCondition( "stick", gamepad, 0, "LeftStick", "x", -1.0, -0.3, 0.0, 0.0 ) )
			motivator:Add( "moveright", InputCondition( "stick", gamepad, 0, "LeftStick", "x", -0.0, -0.0, 0.3, 1.0 ) )
			motivator:Add( "moveup", 	InputCondition( "stick", gamepad, 0, "LeftStick", "y", -0.0, -0.0, 0.3, 1.0 ) )
			motivator:Add( "movedown", 	InputCondition( "stick", gamepad, 0, "LeftStick", "y", -1.0, -0.3, 0.0, 0.0 ) )
		elseif keyboard then
			motivator:Add( "moveleft", 	InputCondition( "button", keyboard, 0, "Left", "Down" ) )
			motivator:Add( "moveright",	InputCondition( "button", keyboard, 0, "Right", "Down" ) )
			motivator:Add( "moveup", 	InputCondition( "button", keyboard, 0, "Up", "Down" ) )
			motivator:Add( "movedown", 	InputCondition( "button", keyboard, 0, "Down", "Down" ) )
		end	
	end
end

function OnUpdate( me )
end
