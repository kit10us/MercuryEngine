local axisIndex = 0
local totalRotation = 0

function OnStart()

	color3d = Effect( "color3d", "media/EffectColor.effect" )
	borgcubeEffect = Effect( "borgcube", "media/EffectBorgCube.effect" )
	
	scene1 = Scene( "scene1" )	
	
	-- Add PhysX
	pxScene = PxSceneComponent()
	pxScene:AttachTo( scene1 )
	
	root = scene1:FindObject( "root" )
	
	proj = Matrix.NewPerspectiveFovLH( math.pi / 4.0, Game.GetWidth() / Game.GetHeight(), 1, 1000 )
	
	-- Add camera...
	camera = root:AddChild( "camera" )
	cameraComponent = CameraComponent()
	cameraComponent:AttachTo( camera )
	cameraComponent:SetProjection( proj )
	camera:Transform():SetPosition( V3.New( 0, 25, -47 ) )
	camera:Transform():LookAt( V3.Zero() )

	scene1:SetSize( Game.GetWidth(), Game.GetHeight() )

	group = root:AddChild( "group" )

	local i = 1

	cubeParameters = ShapeParameters( "cube" )
	cubeParameters:SetSize3( Size3.New( 2, 2, 2 ) )
	cubeParameters:SetEffect( color3d )
	cubeParameters:SetDiffuse( Color.NewRed() )
	local cubeGeoRed = Geometry( cubeParameters )
	cubeParameters:SetDiffuse( Color.NewGreen() )
	local cubeGeoGreen = Geometry( cubeParameters )
	cubeParameters:SetDiffuse( Color.NewBlue() )
	local cubeGeoBlue = Geometry( cubeParameters )
	
	sphereParameters = ShapeParameters( "sphere" )
	sphereParameters:SetEffect( color3d )
	sphereParameters:SetSegments( 24 )
	sphereParameters:SetRadius( 1.0 )
	sphereParameters:SetDiffuse( Color.NewRGB( 1, 1, 0 ) )
	local sphereCyan = Geometry( sphereParameters )

	local numR = 30
	local numC = 30
	local numL = 1
	local offset = 3
	for l = 0, numL - 1 do
		for r = 0, numR - 1 do
			for c = 0, numC - 1 do
				local x = (-offset * numR * 0.5) + (r * offset)
				local y = 10 + l * offset
				local z = (-offset * numC * 0.5) + (c * offset) + c % 3
				object = group:AddChild( "geo_" .. tostring( i ) ); i = i + 1;
				object:Transform():SetPosition( V3.New( x, y, z ) )
				
				local sh = i % 2
				if sh == 0 then
					local col = i % 3
					if col == 0 then
						object:SetGeometry(  cubeGeoRed )
					elseif col == 1 then
						object:SetGeometry(  cubeGeoGreen )
					else
						object:SetGeometry(  cubeGeoBlue )
					end
					local pxShape = PhysX.CreateBoxCollider( V3.New( 1.0, 1.0, 1.0 ) )
					pxShape:AttachTo( object )
				else
					local pxShape = PhysX.CreateSphereCollider( 1.0 )
					pxShape:AttachTo( object )
					object:SetGeometry( sphereCyan )
				end		
				pxBody = PhysX.CreateRigidBody()	
				pxBody:AttachTo( object )
			
			end
		end
	end
	
	planeParameters = ShapeParameters( "plane" )
	planeParameters:SetEffect( color3d )
	planeParameters:SetSegments( 2 )
	planeParameters:SetSize2( Size2.New( 40.0, 40.0 ) )
	planeParameters:SetDiffuse( Color.NewRGB( 0, 1, 1 ) )
	plane = group:AddChild( "plane" )
	plane:SetGeometry( Geometry( planeParameters ) )
	plane:Transform():SetPosition( V3.New( 0, -20, 0 ) )
	
	local terraParams = TerraParameters()
	terraParams:SetEffect( Effect( "pit_image", "media/EffectPit.effect" ) )
	terraParams:SetSize( Size2.New( 30, 30 ) )
	terraParams:SetConstant( 0 )
	terraParams:SetPoints( 100, 100 )
	terraParams:SetHeightMap( Texture( "test256", "media/steps.png", true, true ), Color.NewGrey( 4 ) )
	local terraGeo = Terra( terraParams )
	local terra = group:AddChild( "terra" )
	terra:SetGeometry( terraGeo )
	terra:Transform():SetPosition( V3.New( 0, -12, 0 ) );
	local pxShape = PhysX.CreateHeightFieldCollider( terraGeo, 30, 30 )
	pxShape:AttachTo( terra )
	local pxBody = PhysX.CreateRigidStatic()
	pxBody:AttachTo( terra )
	pxBody:SetDisableGravity( true )
	
end

function OnUpdate()
	--group:Transform():RotateAbout( V3.New( 0, 1, 0 ), Update.GetDelta() )
	
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
