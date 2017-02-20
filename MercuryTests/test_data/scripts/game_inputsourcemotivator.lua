local axisIndex = 1
local totalRotation = 0
local autoRotate = true

function OnStart( me )
	local scene1 = Scene( "scene1" )
	local root = scene1:FindObject( "root" )
	
	color3d = Effect( "color3d", "EffectColor.effect" )
	borgcubeEffect = Effect( "borgcube", "EffectBorgCube.effect" )
	
	
	local proj = Matrix.NewPerspectiveFovLH( math.pi / 4.0, Game.GetWidth()/ Game.GetHeight(), 1, 1200 )
	
	-- Add camera...
	camera = scene1:NewObject( "camera" )
	cameraComponent = CameraComponent()
	cameraComponent:AttachTo( camera )
	cameraComponent:SetProjection( proj )
	camera:Transform():SetPosition( V3.New( 0, 5, -17 ) )
	camera:Transform():LookAt( V3.Zero() )	

	scene1:SetSize( Game.GetWidth(), Game.GetHeight() )

	local cube = scene1:NewObject( "cube" )
	cube:SetGeometry( Geometry( "cube", "ShapeCube.shape" ) )
	cube:Transform():SetPosition( V3.New( -4.5, 3, 0 ) )
    cube:AddScript( "rotate", "lua", "rotatex.lua" )
	
	local pointfield = scene1:NewObject( "pointfield" )
	pointfield:SetGeometry( Geometry( "pointfield", "ShapePointField.shape" ) )
	pointfield:Transform():SetPosition( V3.New( -1.5, 3, 0 ) )
    pointfield:AddScript( "rotate", "lua", "rotatey.lua" )
		
	local pointring = scene1:NewObject( "pointring" )
	pointring:SetGeometry( Geometry( "pointring", "ShapePointRing.shape" ) )
	pointring:Transform():SetPosition( V3.New( 1.5, 3, 0 ) )
    pointring:AddScript( "rotate", "lua", "rotatez.lua" )

	local dashring = scene1:NewObject( "dashring" )
	dashring:SetGeometry( Geometry( "dashring", "ShapeDashRing.shape" ) )
	dashring:Transform():SetPosition( V3.New( 4.5, 3, 0 ) )
    dashring:AddScript( "rotate", "lua", "rotatex.lua" )

	local pyramid = scene1:NewObject( "pyramid" )
	pyramid:SetGeometry( Geometry( "pyramid", "ShapePyramid.shape" ) )
	pyramid:Transform():SetPosition( V3.New( -4.5, 0, 0 ) )
    pyramid:AddScript( "rotate", "lua", "rotatey.lua" )
		
	local circle = scene1:NewObject( "circle" )
	circle:SetGeometry( Geometry( "circle", "ShapeCircle.shape" ) )
	circle:Transform():SetPosition( V3.New( -1.5, 0, 0 ) )
    circle:AddScript( "rotate", "lua", "rotatez.lua" )

	local sphere = scene1:NewObject( "sphere" )
	sphere:SetGeometry( Geometry( "sphere", "ShapeSphere.shape" ) )
	sphere:Transform():SetPosition( V3.New( 1.5, 0, 0 ) )
    sphere:AddScript( "rotate", "lua", "rotatex.lua" )
	   
	local cylinder = scene1:NewObject( "cylinder" )
	cylinder:SetGeometry( Geometry( "cylinder", "ShapeCylinder.shape" ) )
	cylinder:Transform():SetPosition( V3.New( 4.5, 0, 0 ) )
    cylinder:AddScript( "rotate", "lua", "rotatey.lua" )

	local tube = scene1:NewObject( "tube" )
	tube:SetGeometry( Geometry( "tube", "ShapeTube.shape" ) )
	tube:Transform():SetPosition( V3.New( -4.5, -3, 0 ) )
    tube:AddScript( "rotate", "lua", "rotatez.lua" )
		
	local plane = scene1:NewObject( "plane" )
	plane:SetGeometry( Geometry( "plane", "ShapePlane.shape" ) )
	plane:Transform():SetPosition( V3.New( -1.5, -3, 0 ) )
    plane:AddScript( "rotate", "lua", "rotatex.lua" )
		
	cone = scene1:NewObject( "cone" )
	cone:SetGeometry( Geometry( "cone", "ShapeCone.shape" ) )
	cone:Transform():SetPosition( V3.New( 1.5, -3, 0 ) )
    cone:AddScript( "rotate", "lua", "rotatey.lua" )
	Debug.WriteLine( "cone is " .. type(cone) )
	
	local kb = Input( "Keyboard" );
	local mouse = Input( "Mouse" );
	local inputMotivator = ObjectInputMotivator()
	inputMotivator:Add( "runOn",		InputCondition( "button", kb, 0, "LShift", "Down" ) )
	inputMotivator:Add( "StrafeLeft",   InputCondition( "button", kb, 0, "Left", "Down" ) )
	inputMotivator:Add( "StrafeRight",  InputCondition( "button", kb, 0, "Right", "Down" ) )
	inputMotivator:Add( "WalkForward",  InputCondition( "button", kb, 0, "Up", "Down" ) )
	inputMotivator:Add( "WalkBackward", InputCondition( "button", kb, 0, "Down", "Down" ) )
	inputMotivator:AttachTo( camera );	
end

function OnUpdate( me )
end
