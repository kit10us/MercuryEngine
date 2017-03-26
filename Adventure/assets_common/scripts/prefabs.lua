require "colors"
require "util"

function BuildTree( position )
	local this = Scene()
	local name = MakeObjectName( "tree" )
	local object = this:NewObject( name )
	object:Transform():SetPosition( position )

	local color3d = Effect( "color3d" )

	local leaves = Effect( "leaves", "Textured_Ambient.effect" )
	leaves:SetTexture( 0, Texture( "leaves", "leafsolid.bmp" ) )

		
	sphereParameters = ShapeParameters( "sphere" )
	sphereParameters:SetEffect( leaves )
	sphereParameters:SetSegments( 24 )
	sphereParameters:SetRadius( 2.0 )
	sphereParameters:SetDiffuse( Green( 0.8 ) )
	sphereParameters:SetCenter( V3( 0, 5, 0 ) );
	object:AddGeometry( Geometry( sphereParameters ) )

	cylinderParameters = ShapeParameters( "cylinder" )
	cylinderParameters:SetEffect( color3d )
	cylinderParameters:SetSegments( 24 )
	cylinderParameters:SetRadius( 0.7 )
	cylinderParameters:SetHeight( 5.0 )
	cylinderParameters:SetDiffuse( Brown() )
	cylinderParameters:SetCaps( true )
	cylinderParameters:SetCenter( V3( 0, 2.5, 0 ) )
	object:AddGeometry( Geometry( cylinderParameters ) )
end

function BuildHouse( position )
	this = Scene()
	local name = MakeObjectName( "house" )
	local object = this:NewObject( name )
	object:Transform():SetPosition( position )

	local color3d = Effect( "color3d" )


	-- Walls
	parameters = ShapeParameters( "cube" )
	parameters:SetEffect( color3d )
	parameters:SetSize3( Size3( 6, 6, 10 ) )
	parameters:SetDiffuse( RGB( .2, 0.1, 0.1 ) * White( 2 ) )
	parameters:SetCenter( V3( 0, 3, 0 ) )
	object:AddGeometry( Geometry( parameters ) )

	-- door
	parameters = ShapeParameters( "cube" )
	parameters:SetEffect( color3d )
	parameters:SetSize3( Size3( 2.6, 5, 1 ) )
	parameters:SetDiffuse( RGB( 107/255, 88/255, 37/255 ) * White( 2 ) )
	parameters:SetCenter( V3( 0, 2.5, -5 ) )
	object:AddGeometry( Geometry( parameters ) )
	
	-- roof
	parameters = ShapeParameters( "pyramid" )
	parameters:SetEffect( color3d )
	parameters:SetSize3( Size3( 7, 2, 11 ) )
	parameters:SetDiffuse( RGB( .3, .3, .3 ) * White( 2 ) )
	parameters:SetCenter( V3( 0, 7, 0 ) )
	object:AddGeometry( Geometry( parameters ) )
end
