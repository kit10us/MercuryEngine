function OnStart()

	color3d = Effect( "color3d", "EffectColorInstanced3d.effect" )
	borgcubeEffect = Effect( "borgcube", "EffectBorgCube.effect" )
	
	scene1 = Scene( "scene1" )	
	
	Game.Command( "AddFPS" )
	
	-- Add PhysX
	pxScene = PxSceneComponent()
	pxScene:AttachTo( scene1 )
	
	root = scene1:FindObject( "root" )
	
	proj = Matrix.NewPerspectiveFovLH( math.pi / 4.0, Game.GetWidth() / Game.GetHeight(), 1, 1000 )
	
	-- Add camera...
	camera = scene1:NewObject( "camera" )
	cameraComponent = CameraComponent()
	cameraComponent:AttachTo( camera )
	cameraComponent:SetProjection( proj )
	camera:Transform():SetPosition( V3.New( 0, 25, -47 ) )
	camera:Transform():LookAt( V3.Zero() )

	scene1:SetSize( Game.GetWidth(), Game.GetHeight() )

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

	local numR = 20
	local numC = 20
	local numL = 2
	local offset = 3
	for l = 0, numL - 1 do
		for r = 0, numR - 1 do
			for c = 0, numC - 1 do
				local x = (-offset * numR * 0.5) + (r * offset)
				local y = 10 + l * offset
				local z = (-offset * numC * 0.5) + (c * offset) + c % 3
				object = scene1:NewObject( "geo_" .. tostring( i ) ); i = i + 1;
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
	plane = scene1:NewObject( "plane" )
	plane:SetGeometry( Geometry( planeParameters ) )
	plane:Transform():SetPosition( V3.New( 0, -20, 0 ) )

	local terraParams = TerraParameters()
	terraParams:SetEffect( Effect( "pit_image", "EffectPit.effect" ) )
	terraParams:SetSize( Size2.New( 30, 30 ) )
	terraParams:SetConstant( 0 )
	terraParams:SetPoints( 100, 100 )
	terraParams:SetHeightMap( Texture( "test256", "steps.bmp", true, true ), Color.NewGrey( 4 ) )
	local terraGeo = Terra( terraParams )
	local terra = scene1:NewObject( "terra" )
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
	
end