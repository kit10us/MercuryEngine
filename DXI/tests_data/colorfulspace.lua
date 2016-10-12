local axisIndex = 1
local totalRotation = 0
local autoRotate = true

function OnStart( me )
	local scene1 = Scene( "scene1" )
	local root = scene1:FindObject( "root" )
	
	color3d = Effect( "color3d", "media/EffectColor.effect" )
	borgcubeEffect = Effect( "borgcube", "media/EffectBorgCube.effect" )
	
	
	local proj = Matrix.NewPerspectiveFovLH( math.pi / 4.0, Game.GetWidth()/ Game.GetHeight(), 1, 1200 )
	
	-- Add camera...
	camera = root:AddCamera( "camera", proj )	
	camera:Transform():SetPosition( V3.New( 0, 5, -17 ) )
	camera:Transform():LookAt( V3.Zero() ) 

	scene1:SetSize( Game.GetWidth(), Game.GetHeight() )

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
	
	local pointfieldParameters = ShapeParameters( "pointfield" )
	pointfieldParameters:SetEffect( color3d )
	pointfieldParameters:SetMinorRadius( 1000 )
	pointfieldParameters:SetMajorRadius( 1001.0 )
	pointfieldParameters:SetCount( 10000 )
	pointfieldParameters:SetDiffuse( Color.NewWhite() )
	local spacefield = root:AddChild( "spacefield" )
	spacefield:SetGeometry( Geometry( pointfieldParameters ) )
	
	local pointringParameters = ShapeParameters( "pointring" )
	pointringParameters:SetEffect( color3d )
	pointringParameters:SetMinorRadius( 10 )
	pointringParameters:SetMajorRadius( 100 )
	pointringParameters:SetCount( 100000 )
	pointringParameters:SetDiffuse( Color.NewRed() )
	local myring = group:AddChild( "myring" )
	myring:SetGeometry( Geometry( pointringParameters ) )
	
	local pointringParameters = ShapeParameters( "pointring" )
	pointringParameters:SetEffect( color3d )
	pointringParameters:SetMinorRadius( 5 )
	pointringParameters:SetMajorRadius( 50 )
	pointringParameters:SetCount( 100000 )
	pointringParameters:SetDiffuse( Color.NewBlue() )
	local myring2 = group:AddChild( "myring2" )
	myring2:SetGeometry( Geometry( pointringParameters ) )
	myring2:AddScript( "rotatex", "lua", "script/rotatex.lua" )
	myring2:AddScript( "rotatez", "lua", "script/rotatez.lua" )
	myring2:AddScript( "rotatey", "lua", "script/rotatey.lua" )

	local pointringParameters = ShapeParameters( "pointring" )
	pointringParameters:SetEffect( color3d )
	pointringParameters:SetMinorRadius( 20 )
	pointringParameters:SetMajorRadius( 150 )
	pointringParameters:SetCount( 1000000 )
	pointringParameters:SetDiffuse( Color.NewGreen() )
	local myring3 = group:AddChild( "myring3" )
	myring3:SetGeometry( Geometry( pointringParameters ) )
	myring3:AddScript( "rotatey", "lua", "script/rotatey.lua" )
	myring3:AddScript( "rotatex", "lua", "script/rotatex.lua" )
	myring3:AddScript( "rotatez", "lua", "script/rotatez.lua" )
	myring3:AddScript( "rotatey", "lua", "script/rotatey.lua" )
end

function OnUpdate( me )
	local rotation = Update.GetDelta()
	
	local mouse = Input( "Mouse" )
	if mouse then
		if mouse:GetState( 0, "RightButton", "Pressed" ) == 1 then
			autoRotate = not autoRotate
		end
		
		if not autoRotate then
			if mouse:GetState( 0, "LeftButton", "Down" ) == 1 then
				local changeX = mouse:GetValue( 0, "ChangeX" )
				local changeY = mouse:GetValue( 0, "ChangeY" )
				
				local rotation = Matrix.NewRotationY( Update.GetDelta() * changeX * 0.2 )
				group:Transform():PostMul( rotation )

				local rotation = Matrix.NewRotationX( Update.GetDelta() * changeY * 0.2 )
				group:Transform():PostMul( rotation )			
			end
			
			
			local mouseWheel = mouse:GetValue( 0, "MouseWheel" )
			if mouseWheel > 0 or mouseWheel < 0 then
				local v = 1 + ( mouseWheel * Update.GetDelta() )
				Debug.WriteLine( tostring( v ) ) 
				local scale = Matrix.NewScale( v )
				group:Transform():PostMul( scale )
			end			
		end
	end	
	
	local gamepad = Input( "Gamepad" )
	if gamepad then
		if gamepad:GetState( 0, "Back", "Pressed" ) == 1 then
			Game.Quit()
		end

		if gamepad:GetState( 0, "Y", "Pressed" ) == 1 then
			autoRotate = not autoRotate
		end
		
		if not autoRotate then
			if gamepad:HasValue( 0, "ThumbLX" ) then
				local v = gamepad:GetValue( 0, "ThumbLX" )
				if v > 0.1  or v < -0.1 then
					local rotation = Matrix.NewRotationY( Update.GetDelta() * v * 2.0 )
					group:Transform():PostMul( rotation )
				end
			end	
			if gamepad:HasValue( 0, "ThumbLY" ) then
				local v = gamepad:GetValue( 0, "ThumbLY" )
				if v > 0.1  or v < -0.1 then
					local rotation = Matrix.NewRotationX( Update.GetDelta() * v * 2.0 )
					group:Transform():PostMul( rotation )
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
			
			if gamepad:HasValue( 0, "ThumbRX" ) then
				local v = gamepad:GetValue( 0, "ThumbRX" )
				if v > 0.1  or v < -0.1 then
					local rotation = Matrix.NewRotationY( Update.GetDelta() * v * 2.0 )
					camera:Transform():PreMul( rotation )
				end
			end	
			if gamepad:HasValue( 0, "ThumbRY" ) then
				local v = gamepad:GetValue( 0, "ThumbRY" )
				if v > 0.1  or v < -0.1 then
					local rotation = Matrix.NewRotationX( Update.GetDelta() * v * -2.0 )
					camera:Transform():PreMul( rotation )
				end
			end	

			if gamepad:GetState( 0, "DPAD_DOWN", "Down" ) == 1 then
				local v = 1 + ( -1 * Update.GetDelta() )
				Debug.WriteLine( tostring( v ) ) 
				local scale = Matrix.NewScale( v )
				group:Transform():PostMul( scale )
			end								
			if gamepad:GetState( 0, "DPAD_UP", "Down" ) == 1 then
				local v = 1 + ( 1 * Update.GetDelta() )
				Debug.WriteLine( tostring( v ) ) 
				local scale = Matrix.NewScale( v )
				group:Transform():PostMul( scale )
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
		
		if axisIndex == 0 then
			axis.y = -1
		elseif axisIndex == 1 then
			axis.x = -1
		elseif axisIndex == 2 then
			axis.z = -1
		end
		
		camera:Transform():RotateAbout( axis, rotation )
		
	end
end
