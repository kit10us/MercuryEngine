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
	camera = root:AddCamera( "camera", proj )	
	camera:Transform():SetPosition( V3.New( 0, 5, -17 ) )
	camera:Transform():LookAt( V3.Zero() ) 

	scene1:SetSize( Game.GetWidth(), Game.GetHeight() )

	local cube = scene1:NewObject( "cube" )
	cube:SetGeometry( Geometry( "cube", "ShapeCube.shape" ) )
	cube:Transform():SetPosition( V3.New( -4.5, 3, 0 ) )
    cube:AddScript( "rotate", "lua", "script/rotatex.lua" )
	
	local pointfield = scene1:NewObject( "pointfield" )
	pointfield:SetGeometry( Geometry( "pointfield", "ShapePointField.shape" ) )
	pointfield:Transform():SetPosition( V3.New( -1.5, 3, 0 ) )
    pointfield:AddScript( "rotate", "lua", "script/rotatey.lua" )
		
	local pointring = scene1:NewObject( "pointring" )
	pointring:SetGeometry( Geometry( "pointring", "ShapePointRing.shape" ) )
	pointring:Transform():SetPosition( V3.New( 1.5, 3, 0 ) )
    pointring:AddScript( "rotate", "lua", "script/rotatez.lua" )

	local dashring = scene1:NewObject( "dashring" )
	dashring:SetGeometry( Geometry( "dashring", "ShapeDashRing.shape" ) )
	dashring:Transform():SetPosition( V3.New( 4.5, 3, 0 ) )
    dashring:AddScript( "rotate", "lua", "script/rotatex.lua" )

	local pyramid = scene1:NewObject( "pyramid" )
	pyramid:SetGeometry( Geometry( "pyramid", "ShapePyramid.shape" ) )
	pyramid:Transform():SetPosition( V3.New( -4.5, 0, 0 ) )
    pyramid:AddScript( "rotate", "lua", "script/rotatey.lua" )
		
	local circle = scene1:NewObject( "circle" )
	circle:SetGeometry( Geometry( "circle", "ShapeCircle.shape" ) )
	circle:Transform():SetPosition( V3.New( -1.5, 0, 0 ) )
    circle:AddScript( "rotate", "lua", "script/rotatez.lua" )

	local sphere = scene1:NewObject( "sphere" )
	sphere:SetGeometry( Geometry( "sphere", "ShapeSphere.shape" ) )
	sphere:Transform():SetPosition( V3.New( 1.5, 0, 0 ) )
    sphere:AddScript( "rotate", "lua", "script/rotatex.lua" )
	   
	local cylinder = scene1:NewObject( "cylinder" )
	cylinder:SetGeometry( Geometry( "cylinder", "ShapeCylinder.shape" ) )
	cylinder:Transform():SetPosition( V3.New( 4.5, 0, 0 ) )
    cylinder:AddScript( "rotate", "lua", "script/rotatey.lua" )

	local tube = scene1:NewObject( "tube" )
	tube:SetGeometry( Geometry( "tube", "ShapeTube.shape" ) )
	tube:Transform():SetPosition( V3.New( -4.5, -3, 0 ) )
    tube:AddScript( "rotate", "lua", "script/rotatez.lua" )
		
	local plane = scene1:NewObject( "plane" )
	plane:SetGeometry( Geometry( "plane", "ShapePlane.shape" ) )
	plane:Transform():SetPosition( V3.New( -1.5, -3, 0 ) )
    plane:AddScript( "rotate", "lua", "script/rotatex.lua" )
		
	local cone = scene1:NewObject( "cone" )
	cone:SetGeometry( Geometry( "cone", "ShapeCone.shape" ) )
	cone:Transform():SetPosition( V3.New( 1.5, -3, 0 ) )
    cone:AddScript( "rotate", "lua", "script/rotatey.lua" )
end

function OnUpdate( me )
	local rotation = Update.GetDelta()
	
	local keyboard = Input( "Keyboard" )
	if keyboard then
		if keyboard:GetState( 0, "ESCAPE", "Pressed" ) == 1 then
			Game.Quit()
		end
		
		if keyboard:GetState( 0, "Space", "Pressed" ) == 1 then
			autoRotate = not autoRotate
		end
		
		if not autoRotate then
			if keyboard:GetState( 0, "Left", "Down" ) == 1 then
				local change = 1.0
				local rotation = Matrix.NewRotationY( Update.GetDelta() * change )
				group:Transform():PostMul( rotation )
			elseif keyboard:GetState( 0, "Right", "Down" ) == 1 then
				local change = -1.0
				local rotation = Matrix.NewRotationY( Update.GetDelta() * change )
				group:Transform():PostMul( rotation )			
			end			
			if keyboard:GetState( 0, "Up", "Down" ) == 1 then
				local change = 1.0
				local rotation = Matrix.NewRotationX( Update.GetDelta() * change )
				group:Transform():PostMul( rotation )
			elseif keyboard:GetState( 0, "Down", "Down" ) == 1 then
				local change = -1.0
				local rotation = Matrix.NewRotationX( Update.GetDelta() * change )
				group:Transform():PostMul( rotation )			
			end			
		end		
	end
	
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
