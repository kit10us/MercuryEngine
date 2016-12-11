local axisIndex = 1
local totalRotation = 0

function OnStart()
	color3d = Effect( "color3d", "media/EffectColor.effect" )
	borgcubeEffect = Effect( "borgcube", "media/EffectBorgCube.effect" )
	
	scene1 = Scene( "scene1" )
	root = scene1:FindObject( "root" )
	
	proj = Matrix.NewPerspectiveFovLH( math.pi / 4.0, Game.GetWidth() / Game.GetHeight(), 1, 1000 )
	
	-- Add camera...
	camera = scene1:AddCamera( "camera", proj )	
	camera:Transform():SetPosition( V3.New( 0, 5, -17 ) )
	camera:Transform():LookAt( V3.Zero() )

	scene1:SetSize( Game.GetWidth(), Game.GetHeight() )

	Debug.WriteLine( "peter" )
	
	cubeParameters = ShapeParameters( "cube" )
	cubeParameters:SetEffect( color3d )
	cubeParameters:SetSize3( Size3.New( 2, 2, 2 ) )
	cubeParameters:SetDiffuseFaces( Color.NewRed(), Color.NewGreen(), Color.NewBlue(), Color.NewRGB( 1, 1, 0 ), Color.NewRGB( 0, 1, 1 ), Color.NewRGB( 1, 0, 1 ) )
	cube = scene1:NewObject( "cube" )
	cube:SetGeometry( Geometry( cubeParameters ) )
	cube:Transform():SetPosition( V3.New( -4.5, 3, 0 ) )


	pointfieldParameters = ShapeParameters( "pointfield" )
	pointfieldParameters:SetEffect( color3d )
	pointfieldParameters:SetMinorRadius( 0.5 )
	pointfieldParameters:SetMajorRadius( 1.0 )
	pointfieldParameters:SetCount( 1000 )
	pointfieldParameters:SetDiffuse( Color.NewWhite() )
	pointfield = scene1:NewObject( "pointfield" )
	pointfield:SetGeometry( Geometry( pointfieldParameters ) )
	pointfield:Transform():SetPosition( V3.New( -1.5, 3, 0 ) )
	
	pointringParameters = ShapeParameters( "pointring" )
	pointringParameters:SetEffect( color3d )
	pointringParameters:SetMinorRadius( 0.25 )
	pointringParameters:SetMajorRadius( 0.75 )
	pointringParameters:SetCount( 1000 )
	pointringParameters:SetDiffuse( Color.NewRed() )
	pointring = scene1:NewObject( "pointring" )
	pointring:SetGeometry( Geometry( pointringParameters ) )
	pointring:Transform():SetPosition( V3.New( 1.5, 3, 0 ) )

	dashringParameters = ShapeParameters( "dashring" )
	dashringParameters:SetEffect( color3d )
	dashringParameters:SetMinorRadius( 0.5 )
	dashringParameters:SetMajorRadius( 1.0 )
	dashringParameters:SetSize( 0.5 )
	dashringParameters:SetCount( 5 )
	dashringParameters:SetDiffuse( Color.NewGreen() )
	dashring = scene1:NewObject( "dashring" )
	dashring:SetGeometry( Geometry( dashringParameters ) )
	dashring:Transform():SetPosition( V3.New( 4.5, 3, 0 ) )	
	
	pyramidParameters = ShapeParameters( "pyramid" )
	pyramidParameters:SetEffect( color3d )
	pyramidParameters:SetSize3( Size3.New( 2, 2, 2 ) )
	pyramidParameters:SetDiffuse( Color.NewRGB( 0, 1, 1 ) )
	pyramid = scene1:NewObject( "pyramid" )
	pyramid:SetGeometry( Geometry( pyramidParameters ) )
	pyramid:Transform():SetPosition( V3.New( -4.5, 0, 0 ) )	
	
	circleParameters = ShapeParameters( "circle" )
	circleParameters:SetEffect( color3d );
	circleParameters:SetSegments( 24 );
	circleParameters:SetRadius( 1.0 );
	circleParameters:SetDiffuse( Color.NewBlue() );
	circle = scene1:NewObject( "circle" )
	circle:SetGeometry( Geometry( circleParameters ) );
	circle:Transform():SetPosition( V3.New( -1.5, 0, 0 ) )
	
	sphereParameters = ShapeParameters( "sphere" )
	sphereParameters:SetEffect( color3d )
	sphereParameters:SetSegments( 24 )
	sphereParameters:SetRadius( 1.0 )
	sphereParameters:SetDiffuse( Color.NewRGB( 1, 1, 0 ) )
	sphere = scene1:NewObject( "sphere" )
	sphere:SetGeometry( Geometry( sphereParameters ) )
	sphere:Transform():SetPosition( V3.New( 1.5, 0, 0 ) )

	cylinderParameters = ShapeParameters( "cylinder" )
	cylinderParameters:SetEffect( color3d )
	cylinderParameters:SetSegments( 24 )
	cylinderParameters:SetRadius( 1.0 )
	cylinderParameters:SetHeight( 2.0 )
	cylinderParameters:SetDiffuse( Color.NewRGB(1, 0, 1 ) )
	cylinderParameters:SetCaps( true )
	cylinder = scene1:NewObject( "cylinder" )
	cylinder:SetGeometry( Geometry( cylinderParameters ) )
	cylinder:Transform():SetPosition( V3.New( 4.5, 0, 0 ) )
	
	tubeParameters = ShapeParameters( "tube" )
	tubeParameters:SetEffect( color3d )
	tubeParameters:SetSegments( 24 )
	tubeParameters:SetMajorRadius( 1.0 )
	tubeParameters:SetMinorRadius( 0.5 )
	tubeParameters:SetHeight( 2.0 )
	tubeParameters:SetDiffuse( Color.NewRed() )
	tube = scene1:NewObject( "tube" )
	tube:SetGeometry( Geometry( tubeParameters ) )
	tube:Transform():SetPosition( V3.New( -4.5, -3, 0 ) )

	
	
	planeParameters = ShapeParameters( "plane" )
	planeParameters:SetEffect( color3d )
	planeParameters:SetSegments( 2 )
	planeParameters:SetSize2( Size2.New( 2.0, 2.0 ) )
	planeParameters:SetDiffuse( Color.NewRGB( 0, 1, 1 ) )
	plane = scene1:NewObject( "plane" )
	plane:SetGeometry( Geometry( planeParameters ) )
	plane:Transform():SetPosition( V3.New( -1.5, -3, 0 ) )
	

	coneParameters = ShapeParameters( "cone" )
	coneParameters:SetEffect( borgcubeEffect )
	coneParameters:SetSegments( 24 )
	coneParameters:SetRadius( 1 )
	coneParameters:SetHeight( 2 )
	coneParameters:SetCaps( true )
	cone = scene1:NewObject( "cone" )
	cone:SetGeometry( Geometry( coneParameters ) )
	cone:Transform():SetPosition( V3.New( 1.5, -3, 0 ) )
end

function OnUpdate()
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

	--group:Transform():RotateAbout( axis, rotation )
end
