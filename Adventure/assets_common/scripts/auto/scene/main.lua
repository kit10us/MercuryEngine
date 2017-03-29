require "prefabs"
require "terrain"
require "map_a"
require "OA"
require "OAC"
require "actions"

local start_position = V3( 4, 0, 0 )

function BuildCube( position )
	local object = this:NewObject( MakeObjectName( "cube" ) )
	local cube = Geometry( "cube", "ShapeCube.shape" )
	object:AddGeometry( cube )	
	object:SetModelMatrix( Matrix() * MatrixTranslate( V3( 0, 1, 0 ) ) )
	object:Transform():SetPosition( position )
	return object
end

function BuildPortalSphere( position )
	local object = this:NewObject( MakeObjectName( "sphere" ) )
	
	local sphereParameters = ShapeParameters( "sphere" )
	sphereParameters:SetEffect( Effect( "color_trans", "Color_Trans.effect" ) )
	
	sphereParameters:SetRadius( 2.0 )
	sphereParameters:SetDiffuse( Blue( 1, 0.1 ) )
	object:AddGeometry( Geometry( MakeGeometryName( "sphere" ), sphereParameters ) )	

	sphereParameters:SetRadius( 1.4 )
	sphereParameters:SetDiffuse( Blue( 1, 0.1 ) )
	object:AddGeometry( Geometry( MakeGeometryName( "sphere" ), sphereParameters ) )	

	sphereParameters:SetRadius( 1.0 )
	sphereParameters:SetDiffuse( Blue( 1, 0.2 ) )
	object:AddGeometry( Geometry( MakeGeometryName( "sphere" ), sphereParameters ) )	
	
	sphereParameters:SetRadius( 0.8 )
	sphereParameters:SetDiffuse( Blue( 1, 0.2 ) )
	object:AddGeometry( Geometry( MakeGeometryName( "sphere" ), sphereParameters ) )	

	sphereParameters:SetRadius( 0.6 )
	sphereParameters:SetDiffuse( Blue( 1, 0.2 ) )
	object:AddGeometry( Geometry( MakeGeometryName( "sphere" ), sphereParameters ) )	

	sphereParameters:SetRadius( 0.2 )
	sphereParameters:SetDiffuse( White() )
	object:AddGeometry( Geometry( MakeGeometryName( "sphere" ), sphereParameters ) )	
	
	object:SetModelMatrix( Matrix() * MatrixTranslate( V3( 0, 1, 0 ) ) )
	object:Transform():SetPosition( position )
	return object
end

function BuildTestSpheres()
	local object = this:NewObject( MakeObjectName( "sphere" ) )	
	local sphereParameters = ShapeParameters( "sphere" )
	sphereParameters:SetEffect( Effect( "color_Trans", "Color_Trans.effect" ) )
	sphereParameters:SetRadius( 1.0 )
	sphereParameters:SetDiffuse( Blue( 1, 1 ) )
	object:AddGeometry( Geometry( MakeGeometryName( "sphere" ), sphereParameters ) )	
	object:SetModelMatrix( Matrix() * MatrixTranslate( V3( 0, 1, 0 ) ) )
	object:Transform():SetPosition( V3( -3, 0, -10 ) )

	local object = this:NewObject( MakeObjectName( "sphere" ) )	
	local sphereParameters = ShapeParameters( "sphere" )
	sphereParameters:SetEffect( Effect( "color_Trans", "Color_Trans.effect" ) )
	sphereParameters:SetRadius( 1.0 )
	sphereParameters:SetDiffuse( Blue( 1, 0.6 ) )
	object:AddGeometry( Geometry( MakeGeometryName( "sphere" ), sphereParameters ) )	
	object:SetModelMatrix( Matrix() * MatrixTranslate( V3( 0, 1, 0 ) ) )
	object:Transform():SetPosition( V3( 0, 0, -10 ) )

	local object = this:NewObject( MakeObjectName( "sphere" ) )	
	local sphereParameters = ShapeParameters( "sphere" )
	sphereParameters:SetEffect( Effect( "color_Trans", "Color_Trans.effect" ) )
	sphereParameters:SetRadius( 1.0 )
	sphereParameters:SetDiffuse( Blue( 1, 0.2 ) )
	object:AddGeometry( Geometry( MakeGeometryName( "sphere" ), sphereParameters ) )	
	object:SetModelMatrix( Matrix() * MatrixTranslate( V3( 0, 1, 0 ) ) )
	object:Transform():SetPosition( V3( 3, 0, -10 ) )

	
	
	
	
	local object = this:NewObject( MakeObjectName( "sphere" ) )	
	local sphereParameters = ShapeParameters( "sphere" )
	sphereParameters:SetEffect( Effect( "color_Trans2", "Color_Trans2.effect" ) )
	sphereParameters:SetRadius( 1.0 )
	sphereParameters:SetDiffuse( Blue( 1, 1 ) )
	object:AddGeometry( Geometry( MakeGeometryName( "sphere" ), sphereParameters ) )	
	object:SetModelMatrix( Matrix() * MatrixTranslate( V3( 0, 1, 0 ) ) )
	object:Transform():SetPosition( V3( -3, 0, -13 ) )

	local object = this:NewObject( MakeObjectName( "sphere" ) )	
	local sphereParameters = ShapeParameters( "sphere" )
	sphereParameters:SetEffect( Effect( "color_Trans2", "Color_Trans2.effect" ) )
	sphereParameters:SetRadius( 1.0 )
	sphereParameters:SetDiffuse( Blue( 1, 0.6 ) )
	object:AddGeometry( Geometry( MakeGeometryName( "sphere" ), sphereParameters ) )	
	object:SetModelMatrix( Matrix() * MatrixTranslate( V3( 0, 1, 0 ) ) )
	object:Transform():SetPosition( V3( 0, 0, -13 ) )

	local object = this:NewObject( MakeObjectName( "sphere" ) )	
	local sphereParameters = ShapeParameters( "sphere" )
	sphereParameters:SetEffect( Effect( "color_Trans2", "Color_Trans2.effect" ) )
	sphereParameters:SetRadius( 1.0 )
	sphereParameters:SetDiffuse( Blue( 1, 0.2 ) )
	object:AddGeometry( Geometry( MakeGeometryName( "sphere" ), sphereParameters ) )	
	object:SetModelMatrix( Matrix() * MatrixTranslate( V3( 0, 1, 0 ) ) )
	object:Transform():SetPosition( V3( 3, 0, -13 ) )

	
	
	
	local object = this:NewObject( MakeObjectName( "sphere" ) )	
	local sphereParameters = ShapeParameters( "sphere" )
	sphereParameters:SetEffect( Effect( "color_Trans3", "Color_Trans3.effect" ) )
	sphereParameters:SetRadius( 1.0 )
	sphereParameters:SetDiffuse( Blue( 1, 1 ) )
	object:AddGeometry( Geometry( MakeGeometryName( "sphere" ), sphereParameters ) )	
	object:SetModelMatrix( Matrix() * MatrixTranslate( V3( 0, 1, 0 ) ) )
	object:Transform():SetPosition( V3( -3, 0, -16 ) )

	local object = this:NewObject( MakeObjectName( "sphere" ) )	
	local sphereParameters = ShapeParameters( "sphere" )
	sphereParameters:SetEffect( Effect( "color_Trans3", "Color_Trans3.effect" ) )
	sphereParameters:SetRadius( 1.0 )
	sphereParameters:SetDiffuse( Blue( 1, 0.6 ) )
	object:AddGeometry( Geometry( MakeGeometryName( "sphere" ), sphereParameters ) )	
	object:SetModelMatrix( Matrix() * MatrixTranslate( V3( 0, 1, 0 ) ) )
	object:Transform():SetPosition( V3( 0, 0, -16 ) )

	local object = this:NewObject( MakeObjectName( "sphere" ) )	
	local sphereParameters = ShapeParameters( "sphere" )
	sphereParameters:SetEffect( Effect( "color_Trans3", "Color_Trans3.effect" ) )
	sphereParameters:SetRadius( 1.0 )
	sphereParameters:SetDiffuse( Blue( 1, 0.2 ) )
	object:AddGeometry( Geometry( MakeGeometryName( "sphere" ), sphereParameters ) )	
	object:SetModelMatrix( Matrix() * MatrixTranslate( V3( 0, 1, 0 ) ) )
	object:Transform():SetPosition( V3( 3, 0, -16 ) )
end




function OnBeforeStart()
	local start_position = V3( 4, 0, 0 )

	CreateTerrain( Size2( 30, 30 ),  Size2( 4, 4 ) )
	
	local proj = MatrixPerspectiveFovLH( math.pi / 4.0, Game.GetWidth()/ Game.GetHeight(), 1, 1000 )
	
	-- Add camera...
	local camera = this:NewObject( "camera" )
	
	local cameraComponent = CameraComponent()
	camera:AddComponent( cameraComponent )
	cameraComponent:SetProjection( proj )
	camera:Transform():SetPosition( V3( 0, 17, -12 ) )
	camera:Transform():LookAt( V3( 0, 0, 0 ) )	

	local color3d = Effect( "color3d", "Color_Ambient.effect" )
	local borgcubeEffect = Effect( "borgcube", "EffectBorgCube.effect" )
	
	local pyramid = this:NewObject( "pyramid" )
	pyramid:AddGeometry( Geometry( "pyramid", "ShapePyramid.shape" ) )
	pyramid:SetModelMatrix( Matrix() * MatrixTranslate( V3( 0, 1, 0 ) ) )
	pyramid:Transform():SetPosition( V3( 0, 0, 0 ) )
	
	-- Create sky castle...
	BuildCube(   V3( 4 + 0, 28, 0 ) )
	BuildCube(   V3( 4 + 2, 28, 0 ) )
	BuildCube(   V3( 4 + 4, 28, 0 ) )
	BuildCube(   V3( 4 + 4, 28, 2 ) )
	BuildCube(   V3( 4 + 4, 28, 4 ) )
	BuildCube(   V3( 4 + 4, 28, 6 ) )
	
	local sphere = BuildPortalSphere( V3( 4 + 4, 30, 6 ) )
	
	
	-- Physics tests...
	
	local physics = MEPhysics()

	-- Create jump objects...
	
	local jump1 = BuildCube( V3( 13, 1, 8 ) )	
	local jump2 = BuildCube( V3( 16, 1, -25 ) )

	jump1:AddComponent( OAC_OnEnter_SetPosition( V3( 2 ), jump2 ) )
	jump2:AddComponent( OAC_OnExit_SetPosition( V3( 2 ), pyramid ) )
	pyramid:AddComponent( OAC_OnEnter_SetPosition( V3( 2 ), V3( 4, 30, 0 ) ) )
	sphere:AddComponent( OAC_OnEnter_SetPosition( V3( 2 ), start_position ) )	

	local player = this:NewObject( "player" )
	player:AddGeometry( Geometry( "player", "Mickey_Mouse/Mickey_Mouse.dae" ) )
	player:SetModelMatrix( MatrixRotationX( Angle.Degrees( -90 ) ) )
	player:Transform():SetPosition( start_position )
	player:Transform():PreMul( MatrixRotationY( Angle.Degrees( 180 ) ) )
	player:AddComponent( physics:CreateEntity():AsObjectComponent() )
	
	-- Add stuff
	BuildTree( V3( -8, 0, 16 ) )	
	BuildTree( V3( -3, 0, 11 ) )	
	BuildTree( V3( 2, 0, 8 ) )	
	BuildTree( V3( -4, 0, 7 ) )	
	BuildTree( V3( 0, 0, 5 ) )	
	BuildHouse( V3( 7, 0, 12 ) )
	
	BuildTestSpheres()
end

function OnAfterStart()
	CreateMap()

	local player = this:FindObject( "player" )

	local gamepad = Input( "Gamepad" )
	if gamepad then	
		local PressY = InputCondition( "button", 0, "Y", "pressed" )
		local action = InputAction( player, OA_SetPosition( start_position ) )
		gamepad:AddAction( "scene", PressY, action )
		
		local PressStart = InputCondition( "button", 0, "Back", "pressed" )
		gamepad:AddAction( "scene", PressStart, A_QuitGame() )
	end

end
