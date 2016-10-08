axisIndex = 1
totalRotation = 0

function OnStart()
	resources.add( "geometry", "cube", "media/ShapeCube.shape" )
	resources.add( "geometry", "pointfield", "media/ShapePointField.shape" )
	resources.add( "geometry", "pointring", "media/ShapePointRing.shape" )
	resources.add( "geometry", "dashring", "media/ShapeDashRing.shape" )
	resources.add( "geometry", "pyramid", "media/ShapePyramid.shape" )
	resources.add( "geometry", "circle", "media/ShapeCircle.shape" )
	resources.add( "geometry", "sphere", "media/ShapeSphere.shape" )
	resources.add( "geometry", "cylinder", "media/ShapeCylinder.shape" )
	resources.add( "geometry", "tube", "media/ShapeTube.shape" )
	resources.add( "geometry", "plane", "media/ShapePlane.shape" )
	resources.add( "geometry", "cone", "media/ShapeCone.shape" )
	
	scene1 = Scene( "scene1" )
	root = scene1:FindObject( "root" )
	
	proj = matrix.makeperspectivefovlh( math.pi / 4.0, game.getwidth()/ game.getheight(), 1, 1000 )
	Camera.SetProjection( "x", proj )
	
	-- Add camera...
	camera = root:AddCamera( "camera", proj )	
	camera:SetPosition( V3.New( 0, 5, -17 ) )
	camera:LookAt( V3.Zero() ) 

	scene1:SetSize( game.getwidth(), game.getheight() )

	group = root:AddChild( "group" )
	
	cube = group:AddChild( "cube" )
	cube:SetGeometry( "cube" )
	cube:SetPosition( V3.New( -4.5, 3, 0 ) )	
	
	pointfield = group:AddChild( "pointfield" )
	pointfield:SetGeometry( "pointfield" )
	pointfield:SetPosition( V3.New( -1.5, 3, 0 ) )
		
	pointring = group:AddChild( "pointring" )
	pointring:SetGeometry( "pointring" )
	pointring:SetPosition( V3.New( 1.5, 3, 0 ) )

	dashring = group:AddChild( "dashring" )
	dashring:SetGeometry( "dashring" )
	dashring:SetPosition( V3.New( 4.5, 3, 0 ) )

	pyramid = group:AddChild( "pyramid" )
	pyramid:SetGeometry( "pyramid" )
	pyramid:SetPosition( V3.New( -4.5, 0, 0 ) )
		
	circle = group:AddChild( "circle" )
	circle:SetGeometry( "circle" )
	circle:SetPosition( V3.New( -1.5, 0, 0 ) )

	sphere = group:AddChild( "sphere" )
	sphere:SetGeometry( "sphere" )
	sphere:SetPosition( V3.New( 1.5, 0, 0 ) )
	   
	cylinder = group:AddChild( "cylinder" )
	cylinder:SetGeometry( "cylinder" )
	cylinder:SetPosition( V3.New( 4.5, 0, 0 ) )

	tube = group:AddChild( "tube" )
	tube:SetGeometry( "tube" )
	tube:SetPosition( V3.New( -4.5, -3, 0 ) )
		
	plane = group:AddChild( "plane" )
	plane:SetGeometry( "plane" )
	plane:SetPosition( V3.New( -1.5, -3, 0 ) )
		
	cone = group:AddChild( "cone" )
	cone:SetGeometry( "cone" )
	cone:SetPosition( V3.New( 1.5, -3, 0 ) )
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

	--camera:RotateAbout( axis, rotation )
	group:RotateAbout( axis, rotation )
end
