local axisIndex = 0
local totalRotation = 0

function OnStart()
	local scene1 = Scene( "scene1" )
	local root = scene1:FindObject( "root" )
	
	local proj = Matrix.NewPerspectiveFovLH( math.pi / 4.0, Game.GetWidth()/ Game.GetHeight(), 1, 1000 )
	
	-- Add camera...
	local camera = root:AddCamera( "camera", proj )	
	camera:Transform():SetPosition( V3.New( 0, 5, -17 ) )
	camera:Transform():LookAt( V3.New( 0, -2, 0 ) ) 

	scene1:SetSize( Game.GetWidth(), Game.GetHeight() )

	local cube = scene1:NewObject( "cube" )
	cube:SetGeometry( Geometry( "cube", "media/ShapeCube.shape" ) )
	cube:Transform():SetPosition( V3.New( -4.5, 3, 0 ) )
    cube:AddScript( "rotate", "lua", "script/rotatex.lua" )
	
	local pointfield = scene1:NewObject( "pointfield" )
	pointfield:SetGeometry( Geometry( "pointfield", "media/ShapePointField.shape" ) )
	pointfield:Transform():SetPosition( V3.New( -1.5, 3, 0 ) )
    pointfield:AddScript( "rotate", "lua", "script/rotatey.lua" )
		
	local pointring = scene1:NewObject( "pointring" )
	pointring:SetGeometry( Geometry( "pointring", "media/ShapePointRing.shape" ) )
	pointring:Transform():SetPosition( V3.New( 1.5, 3, 0 ) )
    pointring:AddScript( "rotate", "lua", "script/rotatez.lua" )

	local dashring = scene1:NewObject( "dashring" )
	dashring:SetGeometry( Geometry( "dashring", "media/ShapeDashRing.shape" ) )
	dashring:Transform():SetPosition( V3.New( 4.5, 3, 0 ) )
    dashring:AddScript( "rotate", "lua", "script/rotatex.lua" )

	local pyramid = scene1:NewObject( "pyramid" )
	pyramid:SetGeometry( Geometry( "pyramid", "media/ShapePyramid.shape" ) )
	pyramid:Transform():SetPosition( V3.New( -4.5, 0, 0 ) )
    pyramid:AddScript( "rotate", "lua", "script/rotatey.lua" )
		
	local circle = scene1:NewObject( "circle" )
	circle:SetGeometry( Geometry( "circle", "media/ShapeCircle.shape" ) )
	circle:Transform():SetPosition( V3.New( -1.5, 0, 0 ) )
    circle:AddScript( "rotate", "lua", "script/rotatez.lua" )

	local sphere = scene1:NewObject( "sphere" )
	sphere:SetGeometry( Geometry( "sphere", "media/ShapeSphere.shape" ) )
	sphere:Transform():SetPosition( V3.New( 1.5, 0, 0 ) )
    sphere:AddScript( "rotate", "lua", "script/rotatex.lua" )
	   
	local cylinder = scene1:NewObject( "cylinder" )
	cylinder:SetGeometry( Geometry( "cylinder", "media/ShapeCylinder.shape" ) )
	cylinder:Transform():SetPosition( V3.New( 4.5, 0, 0 ) )
    cylinder:AddScript( "rotate", "lua", "script/rotatey.lua" )

	local tube = scene1:NewObject( "tube" )
	tube:SetGeometry( Geometry( "tube", "media/ShapeTube.shape" ) )
	tube:Transform():SetPosition( V3.New( -4.5, -3, 0 ) )
    tube:AddScript( "rotate", "lua", "script/rotatez.lua" )
		
	local plane = scene1:NewObject( "plane" )
	plane:SetGeometry( Geometry( "plane", "media/ShapePlane.shape" ) )
	plane:Transform():SetPosition( V3.New( -1.5, -3, 0 ) )
    plane:AddScript( "rotate", "lua", "script/rotatex.lua" )
		
	local cone = scene1:NewObject( "cone" )
	cone:SetGeometry( Geometry( "cone", "media/ShapeCone.shape" ) )
	cone:Transform():SetPosition( V3.New( 1.5, -3, 0 ) )
    cone:AddScript( "rotate", "lua", "script/rotatey.lua" )
	
	
	local terraParams = TerraParameters()
	terraParams:SetEffect( Effect( "pit_image", "media/EffectPit.effect" ) )
	terraParams:SetSize( Size2.New( 20, 20 ) )
	terraParams:SetConstant( 0 )
	terraParams:SetFaces( 20, 20 )
	terraParams:SetHeightMap( Texture( "test256", "media/pit.bmp", true, true ), Color.NewGrey( 1 ) )
	
	local terraGeo = Terra( terraParams )
	local terra = scene1:NewObject( "terra" )
	terra:SetGeometry( terraGeo )
	terra:Transform():SetPosition( V3.New( 0, -7, 0 ) );		
	
end

function OnUpdate()
	local rotation = Update.GetDelta()
	
	--[[
	totalRotation = totalRotation + rotation
	local pi2 = 3.1415926535 * 2
	if totalRotation > pi2 then
		totalRotation = totalRotation - pi2
		rotation = totalRotation -- Left over		
		axisIndex = axisIndex + 1
		if axisIndex >= 3 then
			axisIndex = 0
		end
	end

	local axis = V3.Zero()
	if axisIndex == 0 then
		axis.x = 1
	elseif axisIndex == 1 then
		axis.y = 1
	elseif axisIndex == 2 then
		axis.z = 1
	end

	group:Transform():RotateAbout( axis, rotation )
	--]]
	--group:Transform():RotateAbout( V3.New( 0, 1, 0 ), rotation )

end
