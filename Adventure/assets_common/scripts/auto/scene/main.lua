require "prefabs"
require "terrain"
require "map_a"
require "OA"
require "OAC"
require "actions"
require "shapes"

local start_position = V3( 4, 0, 0 )

function BuildCube( position )
	local object = this:NewObject( MakeObjectName( "cube" ) )
	local effect = Effect( VertexShader( "textured.xml" ), PixelShader( "textured.xml" ), Texture( "articulate.bmp" ) );
	object:AddGeometry( ShapeCube( effect ), MatrixTranslate( V3( 0, 1, 0 ) ) )	
	object:Transform():SetPosition( position )
	return object
end

function BuildPortalSphere( position )
	local object = this:NewObject( MakeObjectName( "sphere" ) )
	object:Transform():SetPosition( position )
	
	local sphereParameters = ShapeParameters( "sphere" )	
	sphereParameters:SetEffect( Effect( PixelShader( "color_trans.xml" ), VertexShader( "color_trans.xml" ) ) )
	
	local modelMatrix = MatrixTranslate( V3( 0, 1, 0 ) )
	
	local radius = 2.0
	
	for	s = 0, 3, 1 do	
		sphereParameters:SetRadius( radius )
		sphereParameters:SetDiffuse( Blue( 1, 0.2 ) )
		object:AddGeometry( Geometry( MakeGeometryName( "sphere" ), sphereParameters ), modelMatrix )	
		radius = radius * 0.7
	end

	sphereParameters:SetRadius( 0.2 )
	sphereParameters:SetDiffuse( White() )
	object:AddGeometry( Geometry( MakeGeometryName( "sphere" ), sphereParameters ), modelMatrix )	
	return object
end

function OnBeforeStart()
	local start_position = V3( 4, 0, 0 )
	
	local color_ambient = Effect( PixelShader( "Color_Ambient.xml" ), VertexShader( "Color_Ambient.xml" ) )
	local color_bubble = Effect( PixelShader( "Color_Bubble.xml" ), VertexShader( "Color_Bubble.xml" ) )
	

	CreateTerrain( Size2( 30, 30 ),  Size2( 4, 4 ) )
	
	local proj = MatrixPerspectiveFovLH( math.pi / 4.0, Game.GetWidth()/ Game.GetHeight(), 1, 1000 )
	
	-- Add camera...
	local camera = this:NewObject( "camera" )
	
	local cameraComponent = CameraComponent()
	camera:AddComponent( cameraComponent )
	cameraComponent:SetProjection( proj )
	camera:Transform():SetPosition( V3( 0, 17, -12 ) )
	camera:Transform():LookAt( V3( 0, 0, 0 ) )	

	
	local pyramid = this:NewObject( "pyramid" )
	pyramid:AddGeometry( ShapePyramid( color_bubble ), MatrixTranslate( V3( 0, 1, 0 ) ) )
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
	player:AddGeometry( Geometry( "player", "Mickey_Mouse/Mickey_Mouse.dae" ), MatrixRotationX( Angle.Degrees( -90 ) ) )
		
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
	
	BuildChest( V3( 13, 0, 0 ) )
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
