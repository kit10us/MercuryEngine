local axisIndex = 1
local totalRotation = 0
local autoRotate = true

function OnStart( me )
	local scene1 = Scene( "scene1" )
	local root = scene1:FindObject( "root" )
	
	local proj = Matrix.NewPerspectiveFovLH( math.pi / 4.0, game.GetWidth()/ game.GetHeight(), 1, 1000 )
	
	-- Add camera...
	local camera = root:AddCamera( "camera", proj )	
	camera:Transform():SetPosition( V3.New( 0, 5, -17 ) )
	camera:Transform():LookAt( V3.Zero() ) 

	scene1:SetSize( game.GetWidth(), game.GetHeight() )

	group = root:AddChild( "group" )
	
	local cube = group:AddChild( "cube" )
	cube:SetGeometry( Geometry( "cube", "media/ShapeCube.shape" ) )
	cube:Transform():SetPosition( V3.New( -4.5, 3, 0 ) )
    cube:AddScript( "rotate", "lua", "script/rotatex.lua" )
	
	local pointfield = group:AddChild( "pointfield" )
	pointfield:SetGeometry( Geometry( "pointfield", "media/ShapePointField.shape" ) )
	pointfield:Transform():SetPosition( V3.New( -1.5, 3, 0 ) )
    pointfield:AddScript( "rotate", "lua", "script/rotatey.lua" )
		
	local pointring = group:AddChild( "pointring" )
	pointring:SetGeometry( Geometry( "pointring", "media/ShapePointRing.shape" ) )
	pointring:Transform():SetPosition( V3.New( 1.5, 3, 0 ) )
    pointring:AddScript( "rotate", "lua", "script/rotatez.lua" )

	local dashring = group:AddChild( "dashring" )
	dashring:SetGeometry( Geometry( "dashring", "media/ShapeDashRing.shape" ) )
	dashring:Transform():SetPosition( V3.New( 4.5, 3, 0 ) )
    dashring:AddScript( "rotate", "lua", "script/rotatex.lua" )

	local pyramid = group:AddChild( "pyramid" )
	pyramid:SetGeometry( Geometry( "pyramid", "media/ShapePyramid.shape" ) )
	pyramid:Transform():SetPosition( V3.New( -4.5, 0, 0 ) )
    pyramid:AddScript( "rotate", "lua", "script/rotatey.lua" )
		
	local circle = group:AddChild( "circle" )
	circle:SetGeometry( Geometry( "circle", "media/ShapeCircle.shape" ) )
	circle:Transform():SetPosition( V3.New( -1.5, 0, 0 ) )
    circle:AddScript( "rotate", "lua", "script/rotatez.lua" )

	local sphere = group:AddChild( "sphere" )
	sphere:SetGeometry( Geometry( "sphere", "media/ShapeSphere.shape" ) )
	sphere:Transform():SetPosition( V3.New( 1.5, 0, 0 ) )
    sphere:AddScript( "rotate", "lua", "script/rotatex.lua" )
	   
	local cylinder = group:AddChild( "cylinder" )
	cylinder:SetGeometry( Geometry( "cylinder", "media/ShapeCylinder.shape" ) )
	cylinder:Transform():SetPosition( V3.New( 4.5, 0, 0 ) )
    cylinder:AddScript( "rotate", "lua", "script/rotatey.lua" )

	local tube = group:AddChild( "tube" )
	tube:SetGeometry( Geometry( "tube", "media/ShapeTube.shape" ) )
	tube:Transform():SetPosition( V3.New( -4.5, -3, 0 ) )
    tube:AddScript( "rotate", "lua", "script/rotatez.lua" )
		
	local plane = group:AddChild( "plane" )
	plane:SetGeometry( Geometry( "plane", "media/ShapePlane.shape" ) )
	plane:Transform():SetPosition( V3.New( -1.5, -3, 0 ) )
    plane:AddScript( "rotate", "lua", "script/rotatex.lua" )
		
	local cone = group:AddChild( "cone" )
	cone:SetGeometry( Geometry( "cone", "media/ShapeCone.shape" ) )
	cone:Transform():SetPosition( V3.New( 1.5, -3, 0 ) )
    cone:AddScript( "rotate", "lua", "script/rotatey.lua" )
end

function OnUpdate( me )
	local rotation = Update.GetDelta()

	local gamepad = Input( "Gamepad" )
	if gamepad then
		if gamepad:GetState( 0, "Back", "Pressed" ) == 1 then
			game.Quit()
		end

		if gamepad:GetState( 0, "Y", "Pressed" ) == 1 then
			autoRotate = not autoRotate;
			Debug.WriteLine( "autoRotate = " .. tostring( autoRotate ) )
		end
		
		if not autoRotate then
			if gamepad:HasValue( 0, "ThumbLX" ) then
				local v = gamepad:GetValue( 0, "ThumbLX" )
				if v > 0.1  or v < -0.1 then
					local rotation = Matrix.NewRotationY( Update.GetDelta() * v * 2.0 )
					group:Transform():PreMul( rotation )
				end
			end	
			if gamepad:HasValue( 0, "ThumbLY" ) then
				local v = gamepad:GetValue( 0, "ThumbLY" )
				if v > 0.1  or v < -0.1 then
					local rotation = Matrix.NewRotationX( Update.GetDelta() * v * 2.0 )
					group:Transform():PreMul( rotation )
				end
			end	
			
			if gamepad:HasValue( 0, "LeftTrigger" ) then
				local v = gamepad:GetValue( 0, "LeftTrigger" )
				local rotation = Matrix.NewRotationZ( Update.GetDelta() * v * 2.0 )
				group:Transform():PreMul( rotation )				
			end
			if gamepad:HasValue( 0, "RightTrigger" ) then
				local v = gamepad:GetValue( 0, "RightTrigger" )
				local rotation = Matrix.NewRotationZ( Update.GetDelta() * v * -2.0 )
				group:Transform():PreMul( rotation )				
			end
		end
	end
	
	
	if autoRotate == true then
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
	end

	
	Debug.WriteLine("niled references: size " .. tostring( collectgarbage("count") ))
	
end
