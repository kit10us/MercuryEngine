local axisIndex = 1
local totalRotation = 0
local autoRotate = true

function OnStart( me )
	local sceneMain = Scene.FindScene( "main" )
	local camera = sceneMain:FindObject( "camera" )
	
	color3d = Effect( "color3d", "EffectColor.effect" )
	borgcubeEffect = Effect( "borgcube", "EffectBorgCube.effect" )
		
	local cube = sceneMain:NewObject( "cube" )
	cube:SetGeometry( Geometry( "cube", "ShapeCube.shape" ) )
	cube:Transform():SetPosition( V3.New( -4.5, 0, 0 ) )
    cube:AddScript( "rotate", "lua", "rotatex.lua" )

	local pyramid = sceneMain:NewObject( "pyramid" )
	pyramid:SetGeometry( Geometry( "pyramid", "ShapePyramid.shape" ) )
	pyramid:Transform():SetPosition( V3.New( -4.5, 0, 0 ) )
    pyramid:AddScript( "rotate", "lua", "rotatey.lua" )
	
	local cylinder = sceneMain:NewObject( "cylinder" )
	cylinder:SetGeometry( Geometry( "cylinder", "ShapeCylinder.shape" ) )
	cylinder:Transform():SetPosition( V3.New( 4.5, 0, 0 ) )
    cylinder:AddScript( "rotate", "lua", "rotatey.lua" )
		
	local gamepad = Input( "Gamepad" );
	local cameraMotivator = camera:GetComponent( "CameraMotivator" );
	
	if cameraMotivator == nil then
		Debug.LogLine( "Camera motivator NOT FOUND!" )
	else	
		
		cameraMotivator:SetValue( "WalkSpeed", 2 );
		cameraMotivator:SetValue( "RunSpeed", 4 );
		cameraMotivator:SetValue( "LookXSpeed", 2 );
		cameraMotivator:SetValue( "LookYSpeed", 2 );

		--  Get the InputMotivator from the cameraMotivator component.
		local motivator = InputMotivator( cameraMotivator );				
		motivator:Add( "runOn", InputCondition( "button", gamepad, 0, "X", "Down" ) )
		motivator:Add( "walkx", InputCondition( "stick", gamepad, 0, "LeftStick", "x", 0.2, 1 ) )
		motivator:Add( "walky", InputCondition( "stick", gamepad, 0, "LeftStick", "y", 0.2, 1 ) )
		motivator:Add( "lookX", InputCondition( "stick", gamepad, 0, "RightStick", "x", 0.2, 1 ) )
		motivator:Add( "lookY", InputCondition( "stick", gamepad, 0, "RightStick", "y", 0.2, 1 ) )
	end
end

function OnUpdate( me )
end
