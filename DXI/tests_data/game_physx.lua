function OnStart()
	color3d = Effect( "color3d", "media/EffectColor.effect" )
	borgcubeEffect = Effect( "borgcube", "media/EffectBorgCube.effect" )
	
	scene1 = Scene( "scene1" )	
	
	-- Add PhysX
	--pxScene = PhysX.CreateSceneComponent()
	--pxScene:AttachTo( scene1 )
	--scene1:AddComponent( pxScene )
	
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
		
	local offset = 3
	for d = 0, 17 do
		for r = 0, 9 do
			for c = 0, 9 do
				local x = -offset * 10 * 0.5 + (r * offset) + r % 2
				local y = 10 + d * offset
				local z = -offset * 10 * 0.5 + (c * offset) + c % 3
				object = group:AddChild( "geo_" .. tostring( i ) ); i = i + 1;
				
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
					--pxCube = PhysX.CreateBox( V3.New( 1.0, 1.0, 1.0 ) )
					--body = PhysX.CreateRigidDynamic()
					
					--physx::PxMat44 tm = *(physx::PxMat44*)&object->GetFrame().GetMatrix();
					--PxTransform localTm( tm );
					--PxRigidDynamic * body = m_physics->createRigidDynamic( localTm );
					--body->attachShape( *cube );
					--body->userData = object;
					--PxRigidBodyExt::updateMassAndInertia( *body, 10.0f );
					--m_scene->addActor( *body );

					--dxiphysx::ObjectComponent * oc = new dxiphysx::ObjectComponent( m_os, "PhysX", body );
					--object->AddComponent( scene::IComponent::ptr( oc ) );					
					
					object:AddTag( "cube" )					
				else
					object:SetGeometry(  sphereCyan )
					object:AddTag( "sphere" )
				end		
				
				object:Transform():SetPosition( V3.New( x, y, z ) )
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
end

function OnUpdate()
	group:Transform():RotateAbout( V3.New( 0, 1, 0 ), Update.GetDelta() )
end
