function OnBeforeStartup()
	local scene1 = Scene( "scene1" )
	
	local proj = Matrix.NewPerspectiveFovLH( math.pi / 4.0, Game.GetWidth()/ Game.GetHeight(), 1, 1000 )
	
	-- Add camera...
	local camera = scene1:NewObject( "camera" )
	local cameraComponent = CameraComponent()
	cameraComponent:AttachTo( camera )
	cameraComponent:SetProjection( proj )
	camera:Transform():SetPosition( V3.New( 0, 5, -17 ) )
	camera:Transform():LookAt( V3.New( 0, -2, 0 ) )	
	
	-- Add camera orbit motivator...
	local cameraMotivator = ObjectOrbitMotivator( V3.New( 0, 0, 0 ), V3.New( 0, 1, 0 ), Angle.Degrees( 45 ) )
	cameraMotivator:AttachTo( camera );	

	-- Add geometry...
	
	local cube = scene1:NewObject( "cube" )
	cube:AddGeometry( Geometry( "cube", "media/ShapeCube.shape" ) )
	cube:Transform():SetPosition( V3.New( -4.5, 3, 0 ) )
    cube:AddScript( "rotate", "lua", "scripts/rotatex.lua" )
	
	local pointfield = scene1:NewObject( "pointfield" )
	pointfield:AddGeometry( Geometry( "pointfield", "media/ShapePointField.shape" ) )
	pointfield:Transform():SetPosition( V3.New( -1.5, 3, 0 ) )
    pointfield:AddScript( "rotate", "lua", "scripts/rotatey.lua" )
		
	local pointring = scene1:NewObject( "pointring" )
	pointring:AddGeometry( Geometry( "pointring", "media/ShapePointRing.shape" ) )
	pointring:Transform():SetPosition( V3.New( 1.5, 3, 0 ) )
    pointring:AddScript( "rotate", "lua", "scripts/rotatez.lua" )

	local dashring = scene1:NewObject( "dashring" )
	dashring:AddGeometry( Geometry( "dashring", "media/ShapeDashRing.shape" ) )
	dashring:Transform():SetPosition( V3.New( 4.5, 3, 0 ) )
    dashring:AddScript( "rotate", "lua", "scripts/rotatex.lua" )

	local pyramid = scene1:NewObject( "pyramid" )
	pyramid:AddGeometry( Geometry( "pyramid", "media/ShapePyramid.shape" ) )
	pyramid:Transform():SetPosition( V3.New( -4.5, 0, 0 ) )
    pyramid:AddScript( "rotate", "lua", "scripts/rotatey.lua" )
		
	local circle = scene1:NewObject( "circle" )
	circle:AddGeometry( Geometry( "circle", "media/ShapeCircle.shape" ) )
	circle:Transform():SetPosition( V3.New( -1.5, 0, 0 ) )
    circle:AddScript( "rotate", "lua", "scripts/rotatez.lua" )

	local sphere = scene1:NewObject( "sphere" )
	sphere:AddGeometry( Geometry( "sphere", "media/ShapeSphere.shape" ) )
	sphere:Transform():SetPosition( V3.New( 1.5, 0, 0 ) )
    sphere:AddScript( "rotate", "lua", "scripts/rotatex.lua" )
	   
	local cylinder = scene1:NewObject( "cylinder" )
	cylinder:AddGeometry( Geometry( "cylinder", "media/ShapeCylinder.shape" ) )
	cylinder:Transform():SetPosition( V3.New( 4.5, 0, 0 ) )
    cylinder:AddScript( "rotate", "lua", "scripts/rotatey.lua" )

	local tube = scene1:NewObject( "tube" )
	tube:AddGeometry( Geometry( "tube", "media/ShapeTube.shape" ) )
	tube:Transform():SetPosition( V3.New( -4.5, -3, 0 ) )
    tube:AddScript( "rotate", "lua", "scripts/rotatez.lua" )
		
	local plane = scene1:NewObject( "plane" )
	plane:AddGeometry( Geometry( "plane", "media/ShapePlane.shape" ) )
	plane:Transform():SetPosition( V3.New( -1.5, -3, 0 ) )
    plane:AddScript( "rotate", "lua", "scripts/rotatex.lua" )
		
	local cone = scene1:NewObject( "cone" )
	cone:AddGeometry( Geometry( "cone", "media/ShapeCone.shape" ) )
	cone:Transform():SetPosition( V3.New( 1.5, -3, 0 ) )
    cone:AddScript( "rotate", "lua", "scripts/rotatey.lua" )
	
	
	local terraParams = TerraParameters()
	terraParams:SetEffect( Effect( "pit_image", "EffectPit.effect" ) )
	terraParams:SetSize( Size2.New( 20, 20 ) )
	terraParams:SetConstant( 0 )
	terraParams:SetFaces( 20, 20 )
	terraParams:SetHeightMap( Texture( "test256", "pit.bmp", true, true ), Color.NewGrey( 1 ) )
	
	local terraGeo = Terra( terraParams )
	local terra = scene1:NewObject( "terra" )
	terra:AddGeometry( terraGeo )
	terra:Transform():SetPosition( V3.New( 0, -7, 0 ) );
	
	Game.Command( "AddFPS" )
end

function OnUpdate()
end
