local axisIndex = 1
local totalRotation = 0
local autoRotate = true

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
end

function OnAfterStartup( me )
	local sceneMain = Scene.FindScene( "main" )
	local camera = sceneMain:FindObject( "camera" )
	
	color3d = Effect( "color3d", "EffectColor.effect" )
	borgcubeEffect = Effect( "borgcube", "EffectBorgCube.effect" )
	
	local cube = sceneMain:NewObject( "cube" )
	cube:SetGeometry( Geometry( "cube", "ShapeCube.shape" ) )
	cube:Transform():SetPosition( V3.New( -4.5, 1, 0 ) )

	local pyramid = sceneMain:NewObject( "pyramid" )
	pyramid:SetGeometry( Geometry( "pyramid", "ShapePyramid.shape" ) )
	pyramid:Transform():SetPosition( V3.New( -4.5, 1, 0 ) )
	
	local cylinder = sceneMain:NewObject( "cylinder" )
	cylinder:SetGeometry( Geometry( "cylinder", "ShapeCylinder.shape" ) )
	cylinder:Transform():SetPosition( V3.New( 4.5, 1, 0 ) )

	--[[	
	local tree_001 = sceneMain:NewObject( "tree_001" )
	tree_001:SetGeometry( Geometry( "pine", "pine_req/pine_tree.dae" ) )
	tree_001:Transform():PostMulM( Matrix.NewRotationX( Angle.Degrees( 90 ) ) )
	tree_001:Transform():PostMulM( Matrix.NewScale( 0.1 ) )
	tree_001:Transform():SetPosition( V3.New( 0, 1, 0 ) )
	--]]
	
		
	local cameraMotivator = camera:GetComponent( "CameraMotivator" );
	
	if not cameraMotivator then
		Debug.LogLine( "Camera motivator NOT FOUND!" )
	else	
		cameraMotivator:SetValue( "speed", 4.0 );
	
		--  Get the InputMotivator from the cameraMotivator component.
		local motivator = InputMotivator( cameraMotivator );				
		
		local gamepad = Input( "Gamepad" )
		if gamepad then	
			motivator:Add( "moveleft", 	InputCondition( "stick", gamepad, 0, "LeftStick", "x", -1.0, -0.3, 0.0, 0.0 ) )
			motivator:Add( "moveright", InputCondition( "stick", gamepad, 0, "LeftStick", "x", -0.0, -0.0, 0.3, 1.0 ) )
			motivator:Add( "moveup", 	InputCondition( "stick", gamepad, 0, "LeftStick", "y", -0.0, -0.0, 0.3, 1.0 ) )
			motivator:Add( "movedown", 	InputCondition( "stick", gamepad, 0, "LeftStick", "y", -1.0, -0.3, 0.0, 0.0 ) )
		end
		local keyboard = Input( "Keyboard" )
		if keyboard then
			motivator:Add( "moveleft", 	InputCondition( "button", keyboard, 0, "Left", "Down" ) )
			motivator:Add( "moveright",	InputCondition( "button", keyboard, 0, "Right", "Down" ) )
			motivator:Add( "moveup", 	InputCondition( "button", keyboard, 0, "Up", "Down" ) )
			motivator:Add( "movedown", 	InputCondition( "button", keyboard, 0, "Down", "Down" ) )
		end		
	end
end

function OnUpdate( me )
end
