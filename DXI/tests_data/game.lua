local axisIndex = 1
local totalRotation = 0

function OnStart( me )
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
	
	proj = matrix.MakePerspectiveFovLH( math.pi / 4.0, game.GetWidth()/ game.GetHeight(), 1, 1000 )
	
	-- Add camera...
	camera = root:AddCamera( "camera", proj )	
	camera:Transform():SetPosition( V3.New( 0, 5, -17 ) )
	camera:Transform():LookAt( V3.Zero() ) 

	scene1:SetSize( game.GetWidth(), game.GetHeight() )

	group = root:AddChild( "group" )
	
	cube = group:AddChild( "cube" )
	cube:SetGeometry( "cube" )
	cube:Transform():SetPosition( V3.New( -4.5, 3, 0 ) )
    cube:AddScript( "rotate", "lua", "script/rotate.lua" )
	
	pointfield = group:AddChild( "pointfield" )
	pointfield:SetGeometry( "pointfield" )
	pointfield:Transform():SetPosition( V3.New( -1.5, 3, 0 ) )
    pointfield:AddScript( "rotate", "lua", "script/rotate.lua" )
		
	pointring = group:AddChild( "pointring" )
	pointring:SetGeometry( "pointring" )
	pointring:Transform():SetPosition( V3.New( 1.5, 3, 0 ) )
    pointring:AddScript( "rotate", "lua", "script/rotate.lua" )

	dashring = group:AddChild( "dashring" )
	dashring:SetGeometry( "dashring" )
	dashring:Transform():SetPosition( V3.New( 4.5, 3, 0 ) )
    dashring:AddScript( "rotate", "lua", "script/rotate.lua" )

	pyramid = group:AddChild( "pyramid" )
	pyramid:SetGeometry( "pyramid" )
	pyramid:Transform():SetPosition( V3.New( -4.5, 0, 0 ) )
    pyramid:AddScript( "rotate", "lua", "script/rotate.lua" )
		
	circle = group:AddChild( "circle" )
	circle:SetGeometry( "circle" )
	circle:Transform():SetPosition( V3.New( -1.5, 0, 0 ) )
    circle:AddScript( "rotate", "lua", "script/rotate.lua" )

	sphere = group:AddChild( "sphere" )
	sphere:SetGeometry( "sphere" )
	sphere:Transform():SetPosition( V3.New( 1.5, 0, 0 ) )
    sphere:AddScript( "rotate", "lua", "script/rotate.lua" )
	   
	cylinder = group:AddChild( "cylinder" )
	cylinder:SetGeometry( "cylinder" )
	cylinder:Transform():SetPosition( V3.New( 4.5, 0, 0 ) )
    cylinder:AddScript( "rotate", "lua", "script/rotate.lua" )

	tube = group:AddChild( "tube" )
	tube:SetGeometry( "tube" )
	tube:Transform():SetPosition( V3.New( -4.5, -3, 0 ) )
    tube:AddScript( "rotate", "lua", "script/rotate.lua" )
		
	plane = group:AddChild( "plane" )
	plane:SetGeometry( "plane" )
	plane:Transform():SetPosition( V3.New( -1.5, -3, 0 ) )
    plane:AddScript( "rotate", "lua", "script/rotate.lua" )
		
	cone = group:AddChild( "cone" )
	cone:SetGeometry( "cone" )
	cone:Transform():SetPosition( V3.New( 1.5, -3, 0 ) )
    cone:AddScript( "rotate", "lua", "script/rotate.lua" )
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
