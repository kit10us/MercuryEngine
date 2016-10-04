// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/geo/Mesh.h>
#include <dxi/scene/Scene.h>
#include <dxi/TextureManager.h>
#include <dxi/GeometryManager.h>
#include <dxi/GeometryGroup.h>
#include <dxi/geo/Terra.h>
#include <dxi/pathing/PathSystem.h>

using namespace dxi;
using namespace core;

/// Dynamic Paths
///
///
class MyGame : public Game
{
protected:
	scene::Scene m_scene1;
	scene::Scene m_scene2;
	scene::Object::shared_ptr m_camera1;
	scene::Object::shared_ptr m_camera2;
	float m_rotation;

public:
    void Startup();
    bool Update( unify::Seconds elapsed, RenderInfo & renderInfo, IInput & input );
    void Render();
    void Shutdown();
} game;

class ObjectPathController : public controllers::IController
{
public:
	ObjectPathController( scene::Object * object, pathing::PathController::shared_ptr pathController )
		: m_object( object )
		, m_pathController( pathController )
	{
		pathController->SetFrame( &object->GetFrame() );
	}

	void Update( unify::Seconds elapsed, IInput & input )
	{
		pathing::PathController * pathController = m_pathController.get();
		pathController->Update( elapsed );
	}
private:
	scene::Object * m_object;
	pathing::PathController::shared_ptr m_pathController;
};

void MyGame::Startup()
{
	Game::Startup();

	GetTextureManager()->Add( "borgcube", new Texture( "media/borgcube.bmp" ) );
	GetTextureManager()->Add( "earth", new Texture( "media/earthb.bmp", TEXTURE_LOCKABLE ) );
	GetTextureManager()->Add( "cutout", new Texture( "media/EarthWaterCutoutMap.bmp", TEXTURE_LOCKABLE ) );

	m_scene1.GetPathSystem().LoadFromFile( "media/paths1.xml" );

	scene::Object::shared_ptr object;

	// Scene 1...
/*
    shapes::CubeParameters cubeParameters;
	cubeParameters.SetFVF( FVF::XYZ | FVF::Diffuse | FVF::Tex1 );
	cubeParameters.SetTexture( GetTextureManager()->Find( "borgcube" ) );
    cubeParameters.SetSize( unify::Size3< float>( 2, 2, 2 ) );
	cubeParameters.SetDiffuseFaces( unify::Color::Red(), unify::Color::Green(), unify::Color::Blue(), unify::Color::Yellow(), unify::Color::Cyan(), unify::Color::Magenta() );
	GetGeometryManager()->Add( "cube",  new Mesh( cubeParameters ) );
	object.reset( new scene::Object( GetGeometryManager()->Find( "cube" ) ) );
	object->GetFrame().SetPosition( unify::V3< float >( -4.5f, 3, 0 ) );
	m_scene1.Add( "cube", object );
	
    shapes::PointFieldParameters pointFieldParameters;
	pointFieldParameters.SetMinorRadius( 0.5f );
	pointFieldParameters.SetMajorRadius( 1.0f );
	pointFieldParameters.SetCount( 1000 );
	GetGeometryManager()->Add( "pointfield", new Mesh( pointFieldParameters ) );
	object.reset( new scene::Object( GetGeometryManager()->Find( "pointfield" ) ) );
	object->GetFrame().SetPosition( unify::V3< float >( -1.5f, 3, 0 ) );
	m_scene1.Add( "pointfield", object );

    shapes::PointRingParameters pointRingParameters;
	pointRingParameters.SetMinorRadius( 0.25f );
	pointRingParameters.SetMajorRadius( 0.75f );
	pointRingParameters.SetCount( 1000 );
	GetGeometryManager()->Add( "pointring", new Mesh( pointRingParameters ) );
	object.reset( new scene::Object( GetGeometryManager()->Find( "pointring" ) ) );
	object->GetFrame().SetPosition( unify::V3< float >( 1.5f, 3, 0 ) );
	m_scene1.Add( "pointring", object );

    shapes::DashRingParameters dashRingParameters;
	dashRingParameters.SetRadius2( 1.25f );
	dashRingParameters.SetRadius( 2 );
	dashRingParameters.SetCount( 5 );
	dashRingParameters.SetDiffuse( unify::Color::Green() );
	GetGeometryManager()->Add( "dashring", new Mesh( dashRingParameters ) );
	object.reset( new scene::Object( GetGeometryManager()->Find( "dashring" ) ) );
	object->GetFrame().SetPosition( unify::V3< float >( 4.5f, 3, 0 ) );
	m_scene1.Add( "dashring", object );

    shapes::PyramidParameters pyramidParameters;
    pyramidParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
	pyramidParameters.SetDiffuse( unify::Color::Yellow() );
	GetGeometryManager()->Add( "pyramid", new Mesh( Shape::Pyramid, args ) );
	object.reset( new scene::Object( GetGeometryManager()->Find( "pyramid" ) ) );
	object->GetFrame().SetPosition( unify::V3< float >( -4.5f, 0, 0 ) );
	m_scene1.Add( "pyramid", object );

    shapes::CircleParameters circleParameters;
	circleParameters.SetSegments( 24 );
	circleParameters.SetRadius( 1 );
	circleParameters.SetDiffuse( unify::Color::Red() );
	GetGeometryManager()->Add( "circle", new Mesh( circleParameters ) );
	object.reset( new scene::Object( GetGeometryManager()->Find( "circle" ) ) );
	object->GetFrame().SetPosition( unify::V3< float >( -1.5f, 0, 0 ) );
	m_scene1.Add( "circle", object );

    shapes::SphereParameters sphereParameters;
	sphereParameters.SetSegments( 24 );
	sphereParameters.SetRadius( 1 );
	sphereParameters.SetDiffuse( unify::Color::Red() );
	GetGeometryManager()->Add( "sphere", new Mesh( sphereParameters ) );
	object.reset( new scene::Object( GetGeometryManager()->Find( "sphere" ) ) );
	object->GetFrame().SetPosition( unify::V3< float >( 1.5f, 0, 0 ) );
	m_scene1.Add( "sphere", object );

    shapes::CylinderParameters cylinderParameters;
	cylinderParameters.SetSegments( 24 );
	cylinderParameters.SetRadius( 1 );
	cylinderParameters.SetHeight( 2 );
	cylinderParameters.SetDiffuse( unify::Color::Magenta() );
	GetGeometryManager()->Add( "cylinder", new Mesh( cylinderParameters ) );
	object.reset( new scene::Object( GetGeometryManager()->Find( "cylinder" ) ) );
	object->GetFrame().SetPosition( unify::V3< float >( 4.5f, 0, 0 ) );
	m_scene1.Add( "cylinder", object );

    shapes::TubeParameters tubeParameters;
	tubeParameters.SetSegments( 24 );
	tubeParameters.SetRadius( 1 );
	tubeParameters.SetRadius2( 0.5f );
	tubeParameters.SetHeight( 2 );
	tubeParameters.SetDiffuse( unify::Color::Blue() );
	GetGeometryManager()->Add( "tube", new Mesh( tubeParameters ) );
	object.reset( new scene::Object( GetGeometryManager()->Find( "tube" ) ) );
	object->GetFrame().SetPosition( unify::V3< float >( -4.5f, -3, 0 ) );
	m_scene1.Add( "tube", object );

    shapes::PlaneParameters planeParameters;
	planeParameters.SetSegments( 2 );
    planeParameters.SetSize( unify::Size< float >( 2, 2 ) );
	planeParameters.SetDiffuse( unify::Color::Cyan() );
	GetGeometryManager()->Add( "plane", new Mesh( planeParameters ) );
	object.reset( new scene::Object( GetGeometryManager()->Find( "plane" ) ) );
	object->GetFrame().SetPosition( unify::V3< float >( -1.5f, -3, 0 ) );
	m_scene1.Add( "plane", object );

    shapes::ConeParameters coneParameters;
	coneParameters.SetSegments( 24 );
	coneParameters.SetRadius( 1 );
	coneParameters.SetHeight( 2 );
	coneParameters.SetDiffuse( 0, unify::Color::Red() );
	GetGeometryManager()->Add( "cone", new Mesh( coneParameters ) );
	object.reset( new scene::Object( GetGeometryManager()->Find( "cone" ) ) );
	object->GetFrame().SetPosition( unify::V3< float >( 1.5f, -3, 0 ) );
	m_scene1.Add( "cone", object );
    */

    // Create shape geometry...
    shapes::ShapeBaseParameters commonParameters;
    commonParameters.SetFVF( FVF::XYZ | FVF::Diffuse | FVF::Tex1 );
    commonParameters.SetTexture( GetTextureManager()->Find( "borgcube" ) );

    shapes::CubeParameters cubeParameters;
    cubeParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
    cubeParameters.SetDiffuseFaces( unify::Color::ColorRed(), unify::Color::ColorGreen(), unify::Color::ColorBlue(), unify::Color::ColorYellow(), unify::Color::ColorCyan(), unify::Color::ColorMagenta() );
    GetGeometryManager()->Add( "cube",  new Mesh( commonParameters + cubeParameters ) );

    shapes::PointFieldParameters pointFieldParameters;
    pointFieldParameters.SetMinorRadius( 0.5f );
    pointFieldParameters.SetMajorRadius( 1.0f );
    pointFieldParameters.SetCount( 1000 );
    GetGeometryManager()->Add( "pointfield", new Mesh( commonParameters + pointFieldParameters ) );

    shapes::PointRingParameters pointRingParameters;
    pointRingParameters.SetMinorRadius( .25f );
    pointRingParameters.SetMajorRadius( .75f );
    pointRingParameters.SetCount( 1000 );
    GetGeometryManager()->Add( "pointring", new Mesh( commonParameters + pointRingParameters ) );

    shapes::DashRingParameters dashRingParameters;
    dashRingParameters.SetMinorRadius( 1.25f );
    dashRingParameters.SetMajorRadius( 2.0f );
    dashRingParameters.SetSize( 0.5f );
    dashRingParameters.SetCount( 5 );
    dashRingParameters.SetDiffuse( unify::Color::ColorGreen() );
    GetGeometryManager()->Add( "dashring", new Mesh( commonParameters + dashRingParameters ) );

    shapes::PyramidParameters pyramidParameters;
    pyramidParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
    pyramidParameters.SetDiffuse( unify::Color::ColorYellow() );
    GetGeometryManager()->Add( "pyramid", new Mesh( commonParameters + pyramidParameters ) );

    shapes::CircleParameters circleParameters;
    circleParameters.SetSegments( 24 );
    circleParameters.SetRadius( 1.0f );
    circleParameters.SetDiffuse( unify::Color::ColorBlue() );
    GetGeometryManager()->Add( "circle", new Mesh( commonParameters + circleParameters ) );

    shapes::SphereParameters sphereParameters;
    sphereParameters.SetSegments( 24 );
    sphereParameters.SetRadius( 1.0f );
    sphereParameters.SetDiffuse( unify::Color::ColorCyan() );
    GetGeometryManager()->Add( "sphere", new Mesh( commonParameters + sphereParameters ) );

    shapes::CylinderParameters cylinderParameters;
    cylinderParameters.SetSegments( 24 );
    cylinderParameters.SetRadius( 1.0f );
    cylinderParameters.SetHeight( 2.0f );
    cylinderParameters.SetDiffuse( unify::Color::ColorMagenta() );
    GetGeometryManager()->Add( "cylinder", new Mesh( commonParameters + cylinderParameters ) );

    shapes::TubeParameters tubeParameters;
    tubeParameters.SetSegments( 24 );
    tubeParameters.SetMajorRadius( 1.0f );
    tubeParameters.SetMinorRadius( 0.5f );
    tubeParameters.SetHeight( 2.0f );
    tubeParameters.SetDiffuse( unify::Color::ColorRed() );
    GetGeometryManager()->Add( "tube", new Mesh( commonParameters + tubeParameters ) );

    shapes::PlaneParameters planeParameters;
    planeParameters.SetSegments( 2 );
    planeParameters.SetSize( unify::Size< float >( 2.0f, 2.0f ) );
    planeParameters.SetDiffuse( unify::Color::ColorCyan() );
    GetGeometryManager()->Add( "plane", new Mesh( commonParameters + planeParameters ) );

    shapes::ConeParameters coneParameters;
    coneParameters.SetSegments( 24 );
    coneParameters.SetRadius( 1 );
    coneParameters.SetHeight( 2 );
    coneParameters.SetDiffuse( unify::Color::ColorRed() );
    GetGeometryManager()->Add( "cone", new Mesh( commonParameters + coneParameters ) );


    // Add shapes...

    object.reset( new scene::Object( GetGeometryManager()->Find( "cube" ) ) );
    object->GetFrame().SetPosition( unify::V3< float >( -4.5f, 3, 0 ) );
    m_scene1.Add( "cube", object );
    
    object.reset( new scene::Object( GetGeometryManager()->Find( "pointfield" ) ) );
    object->GetFrame().SetPosition( unify::V3< float >( -1.5f, 3, 0 ) );
    m_scene1.Add( "pointfield", object );

    object.reset( new scene::Object( GetGeometryManager()->Find( "pointring" ) ) );
    object->GetFrame().SetPosition( unify::V3< float >( 1.5f, 3, 0 ) );
    m_scene1.Add( "pointring", object );

    object.reset( new scene::Object( GetGeometryManager()->Find( "dashring" ) ) );
    object->GetFrame().SetPosition( unify::V3< float >( 4.5f, 3, 0 ) );
    m_scene1.Add( "dashring", object );

    object.reset( new scene::Object( GetGeometryManager()->Find( "pyramid" ) ) );
    object->GetFrame().SetPosition( unify::V3< float >( -4.5f, 0, 0 ) );
    m_scene1.Add( "pyramid", object );

    object.reset( new scene::Object( GetGeometryManager()->Find( "circle" ) ) );
    object->GetFrame().SetPosition( unify::V3< float >( -1.5f, 0, 0 ) );
    m_scene1.Add( "circle", object );

    object.reset( new scene::Object( GetGeometryManager()->Find( "sphere" ) ) );
    object->GetFrame().SetPosition( unify::V3< float >( 1.5f, 0, 0 ) );
    m_scene1.Add( "sphere", object );

    object.reset( new scene::Object( GetGeometryManager()->Find( "cylinder" ) ) );
    object->GetFrame().SetPosition( unify::V3< float >( 4.5f, 0, 0 ) );
    m_scene1.Add( "cylinder", object );

    object.reset( new scene::Object( GetGeometryManager()->Find( "tube" ) ) );
    object->GetFrame().SetPosition( unify::V3< float >( -4.5f, -3, 0 ) );
    m_scene1.Add( "tube", object );

    object.reset( new scene::Object( GetGeometryManager()->Find( "plane" ) ) );
    object->GetFrame().SetPosition( unify::V3< float >( -1.5f, -3, 0 ) );
    m_scene1.Add( "plane", object );

    object.reset( new scene::Object( GetGeometryManager()->Find( "cone" ) ) );
    object->GetFrame().SetPosition( unify::V3< float >( 1.5f, -3, 0 ) );
    m_scene1.Add( "cone", object );

	m_camera1.reset( new scene::Object() );
	m_scene1.Add( "camera1", m_camera1 );
	m_scene1.SetCamera( "camera1" );
	m_scene1.SetSize( GetOS().GetResolution() );
	m_camera1->SetController( controllers::IController::shared_ptr( new ObjectPathController( m_camera1.get(), m_scene1.GetPathSystem().CreatePathController( "camera", true ) ) ) );

	// Scene 2...

	GeometryGroup * geometryGroup = new GeometryGroup();
	Terra * terra;

    Terra::Parameters terraParameters;
    terraParameters.SetSize( unify::Size< float >( 10, 7 ) );
    terraParameters.SetFVF( FVF::XYZ | FVF::Tex1 );
    terraParameters.SetRowsColumns( unify::RowColumn< unsigned int >( 200, 140 ) );
    terraParameters.SetConstant( 0 );
    terraParameters.SetTexArea( unify::TexArea::Full() );
	terra = new Terra( terraParameters );
	dxi::ColorOp depthOp;
	depthOp.r = 0.5f;
	depthOp.g = 0.25f;
	depthOp.b = -1.0f;
	terra->ApplyHeightMap( GetTextureManager()->Find( "earth" ), depthOp );
	terra->GetPrimitiveList().GetEffect( 0 ).SetTexture( 0, GetTextureManager()->Find( "earth" ) );
	terra->GetPrimitiveList().GetEffect( 0 ).SetBlend( Blend( Usage::False ) );
	geometryGroup->Add( Geometry::shared_ptr( terra ) );

    terraParameters.Reset();
    terraParameters.SetSize( unify::Size< float >( 10, 7 ) );
    terraParameters.SetFVF( FVF::XYZ | FVF::Tex1 );
    terraParameters.SetRowsColumns( unify::RowColumn< unsigned int >( 10, 7 ) );
    terraParameters.SetConstant( 0.1f );
    terraParameters.SetTexArea( unify::TexArea::Full() );
	terra = new Terra( terraParameters );
	dxi::ColorOp noDrawOp;
	noDrawOp.r = 1.0f;
	terra->ApplyAlphaMap( GetTextureManager()->Find( "cutout" ), noDrawOp );
	terra->GetPrimitiveList().GetEffect( 0 ).SetTexture( 0, GetTextureManager()->Find( "earth" ) );
	terra->GetPrimitiveList().GetEffect( 0 ).SetBlend( Blend( Usage::True, Blend::Effect::One, Blend::Effect::One ) );
	geometryGroup->Add( Geometry::shared_ptr( terra ) );

	object.reset( new scene::Object( Geometry::shared_ptr( geometryGroup ) ) );
	m_scene2.Add( "terrain", object );

	m_camera2.reset( new scene::Object );
	m_scene2.Add( "camera2", m_camera2 );
	m_scene2.SetCamera( "camera2" );
	m_scene2.SetSize( GetOS().GetResolution() / 3 );
	m_scene2.SetPosition( unify::V2< int >( 20, GetOS().GetResolution().height - ( GetOS().GetResolution().height / 3 ) - 20 ) );

	m_rotation = 0;
}

bool MyGame::Update( unify::Seconds elapsed, IInput & input )
{
	unify::V3< float > eye( 10.0f * sin( m_rotation ), 10.0f * sin( m_rotation ), 10.0f * cos( m_rotation ) );
	
	m_camera2->GetFrame().SetPosition( eye );
	m_camera2->GetFrame().LookAt( unify::V3< float >(0, 0, 0) );

	m_rotation += 0.01f;
	if ( m_rotation > 3.1415936535f * 2 )
	{
		m_rotation -= 3.1415936545f * 2;
	}

	m_scene1.Update( elapsed, input );
	m_scene2.Update( elapsed, input );

	return Game::Update( elapsed, renderInfo, input );
}

void MyGame::Render()
{
	m_scene1.Render();
	m_scene2.Render();
}

void MyGame::Shutdown()
{
	m_scene1.Clear();
	m_scene2.Clear();
	
	Game::Shutdown();
}

