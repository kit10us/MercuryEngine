local axisIndex = 1
local totalRotation = 0

function OnStart( me )
	color3d = Effect( "color3d", "media/EffectColor.effect" )
	
	scene1 = Scene( "scene1" )
	root = scene1:FindObject( "root" )
	
	proj = Matrix.NewPerspectiveFovLH( math.pi / 4.0, game.GetWidth()/ game.GetHeight(), 1, 1000 )
	
	-- Add camera...
	camera = root:AddCamera( "camera", proj )	
	camera:Transform():SetPosition( V3.New( 0, 5, -17 ) )
	camera:Transform():LookAt( V3.Zero() )

	if game.GetRendererCount() > 1 then
		proj2 = Matrix.NewPerspectiveFovLH( 1, game.GetWidth()/ game.GetHeight(), 1, 1000 )		--proj2 = Matrix.NewOrthoOffCenterLH( 0, game.GetWidth(), 0, game.GetHeight(), -100, 100 )
		camera2 = root:AddCamera( "Camera2", proj )
		camera2:Transform():SetPosition( V3.New( 0, 0, -27 ) )
		--camera2:Transform():LookAt( V3.Zero() )
		cameraComponent = camera2:GetComponent( "Camera" )
		cameraComponent:SetRenderer( 1 )
	end

	scene1:SetSize( game.GetWidth(), game.GetHeight() )

	group = root:AddChild( "group" )
	
	cube = group:AddChild( "cube" )
	cube:SetGeometry( Geometry( "cube", "media/ShapeCube.shape" ) )
	cube:Transform():SetPosition( V3.New( -4.5, 3, 0 ) )
    cube:AddScript( "rotate", "lua", "script/rotate.lua" )
	
	pointfield = group:AddChild( "pointfield" )
	pointfield:SetGeometry( Geometry( "pointfield", "media/ShapePointField.shape" ) )
	pointfield:Transform():SetPosition( V3.New( -1.5, 3, 0 ) )
    pointfield:AddScript( "rotate", "lua", "script/rotate.lua" )
		
	pointring = group:AddChild( "pointring" )
	pointring:SetGeometry( Geometry( "pointring", "media/ShapePointRing.shape" ) )
	pointring:Transform():SetPosition( V3.New( 1.5, 3, 0 ) )
    pointring:AddScript( "rotate", "lua", "script/rotate.lua" )

	dashring = group:AddChild( "dashring" )
	dashring:SetGeometry( Geometry( "dashring", "media/ShapeDashRing.shape" ) )
	dashring:Transform():SetPosition( V3.New( 4.5, 3, 0 ) )
    dashring:AddScript( "rotate", "lua", "script/rotate.lua" )

	pyramid = group:AddChild( "pyramid" )
	pyramid:SetGeometry( Geometry( "pyramid", "media/ShapePyramid.shape" ) )
	pyramid:Transform():SetPosition( V3.New( -4.5, 0, 0 ) )
    pyramid:AddScript( "rotate", "lua", "script/rotate.lua" )
		
	circle = group:AddChild( "circle" )
	circle:SetGeometry( Geometry( "circle", "media/ShapeCircle.shape" ) )
	circle:Transform():SetPosition( V3.New( -1.5, 0, 0 ) )
    circle:AddScript( "rotate", "lua", "script/rotate.lua" )

	sphere = group:AddChild( "sphere" )
	sphere:SetGeometry( Geometry( "sphere", "media/ShapeSphere.shape" ) )
	sphere:Transform():SetPosition( V3.New( 1.5, 0, 0 ) )
    sphere:AddScript( "rotate", "lua", "script/rotate.lua" )
	   
	cylinder = group:AddChild( "cylinder" )
	cylinder:SetGeometry( Geometry( "cylinder", "media/ShapeCylinder.shape" ) )
	cylinder:Transform():SetPosition( V3.New( 4.5, 0, 0 ) )
    cylinder:AddScript( "rotate", "lua", "script/rotate.lua" )

	tube = group:AddChild( "tube" )
	tube:SetGeometry( Geometry( "tube", "media/ShapeTube.shape" ) )
	tube:Transform():SetPosition( V3.New( -4.5, -3, 0 ) )
    tube:AddScript( "rotate", "lua", "script/rotate.lua" )
		
	plane = group:AddChild( "plane" )
	plane:SetGeometry( Geometry( "plane", "media/ShapePlane.shape" ) )
	plane:Transform():SetPosition( V3.New( -1.5, -3, 0 ) )
    plane:AddScript( "rotate", "lua", "script/rotate.lua" )
		
	cone = group:AddChild( "cone" )
	cone:SetGeometry( Geometry( "cone", "media/ShapeCone.shape" ) )
	cone:Transform():SetPosition( V3.New( 1.5, -3, 0 ) )
    cone:AddScript( "rotate", "lua", "script/rotate.lua" )

	
	-- Front Back up down left right..
	frontShapeParameters = ShapeParameters( "cone" )
	frontShapeParameters:SetEffect( color3d )
	frontShapeParameters:SetSegments( 24 )
	frontShapeParameters:SetRadius( 1 )
	frontShapeParameters:SetHeight( 2 )
	frontShapeParameters:SetDiffuse( Color.NewGreen() )
	frontShapeParameters:SetCaps( true )
	frontShape = Geometry( frontShapeParameters )
	front = root:AddChild( "front" )
	front:SetGeometry( frontShape )
	front:Transform():SetPosition( V3.New( 0, 0, 10 ) );

	back = root:AddChild( "back" )
	back:SetGeometry( "sphere" )
	back:Transform():SetPosition( V3.New( 0, 0, -10 ) );
	
	up = root:AddChild( "pyramid" )
	up:SetGeometry( "pyramid" )
	up:Transform():SetPosition( V3.New( 0, 10, 0 ) );

	down = root:AddChild( "down" )
	down:SetGeometry( "cylinder" )
	down:Transform():SetPosition( V3.New( 0, -10, 0 ) );
	
	
	
end

function OnUpdate( me )
	rotation = Update.GetDelta()
	
	totalRotation = totalRotation + rotation
	pi2 = 3.1415926535 * 2
	if totalRotation > pi2 then
		totalRotation = totalRotation - pi2
		rotation = totalRotation -- Left over
		axisIndex = axisIndex + 1
		if axisIndex >= 3 then
			axisIndex = 0
		end
	end

	axis = V3.Zero()
	if axisIndex == 0 then
		axis.x = 1
	elseif axisIndex == 1 then
		axis.y = 1
	elseif axisIndex == 2 then
		axis.z = 1
	end

	group:Transform():RotateAbout( axis, rotation )
end