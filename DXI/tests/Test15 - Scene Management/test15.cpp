// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/geo/GeometryGroup.h>
#include <dxi/geo/Terra.h>
#include <boost/tuple/tuple.hpp>

using namespace dxi;
using namespace core;

class MyGame : public Game
{
public:
	void Startup();
	bool Update( unify::Seconds elapsed, RenderInfo & renderInfo, IInput & input );
	void Render();
	void Shutdown();
} game;

class CameraRotation : public events::Listener
{
    float m_unitTime;
public:
    CameraRotation()
        : m_unitTime( 0 )
    {
    }

    void operator()( boost::any & eventDataAny )
    {
        scene::Object::EventData::OnUpdate & eventDataTuple = boost::any_cast< scene::Object::EventData::OnUpdate & >( eventDataAny );

        scene::Object * object = eventDataTuple.get< 0 >();
        unify::Seconds elapsed = eventDataTuple.get< 1 >();
        IInput & input = eventDataTuple.get< 2 >();

        float rotation( m_unitTime * 3.1415936535f * 2 );
	    unify::V3< float > eye( 10.0f * sin( rotation ), 10.0f * sin( rotation ), 10.0f * cos( rotation ) );
	    object->GetFrame().SetPosition( eye );
	    object->GetFrame().LookAt( unify::V3< float >(0, 0, 0) );

	    m_unitTime += elapsed * 0.1f;
	    if ( m_unitTime > 1.0f )
	    {
		    m_unitTime -= 1.0f;
	    }
    }
};

void MyGame::Startup()
{
	Game::Startup();

	GetTextureManager()->Add( "borgcube", new Texture( "media/borgcube.bmp" ) );
	GetTextureManager()->Add( "earth", new Texture( "media/earthb.bmp", TEXTURE_LOCKABLE ) );
	GetTextureManager()->Add( "cutout", new Texture( "media/EarthWaterCutoutMap.bmp", TEXTURE_LOCKABLE ) );

	scene::Object::shared_ptr object;

	// Scene 1...
    
    scene::Scene::shared_ptr scene1 = GetSceneManager()->Add( "scene1", new scene::Scene );

    // Create shape geometry...
    shapes::ShapeBaseParameters commonParameters;
	commonParameters.SetFVF( FVF::XYZ | FVF::Diffuse | FVF::Tex1 );
	commonParameters.SetTexture( GetTextureManager()->Find( "borgcube" ) );

    shapes::CubeParameters cubeParameters;
    cubeParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
    cubeParameters.SetDiffuseFaces(unify::Color::ColorRed(),unify::Color::ColorGreen(),unify::Color::ColorBlue(),unify::Color::ColorYellow(),unify::Color::ColorCyan(),unify::Color::ColorMagenta() );
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
	dashRingParameters.SetMinorRadius( 0.625f );
	dashRingParameters.SetMajorRadius( 1.0f );
	dashRingParameters.SetSize( 0.5f );
	dashRingParameters.SetCount( 5 );
	dashRingParameters.SetDiffuse(unify::Color::ColorGreen() );
	GetGeometryManager()->Add( "dashring", new Mesh( commonParameters + dashRingParameters ) );

    shapes::PyramidParameters pyramidParameters;
	pyramidParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
	pyramidParameters.SetDiffuse(unify::Color::ColorYellow() );
    GetGeometryManager()->Add( "pyramid", new Mesh( commonParameters + pyramidParameters ) );

    shapes::CircleParameters circleParameters;
	circleParameters.SetSegments( 24 );
	circleParameters.SetRadius( 1.0f );
	circleParameters.SetDiffuse(unify::Color::ColorBlue() );
    GetGeometryManager()->Add( "circle", new Mesh( commonParameters + circleParameters ) );

    shapes::SphereParameters sphereParameters;
	sphereParameters.SetSegments( 24 );
	sphereParameters.SetRadius( 1.0f );
	sphereParameters.SetDiffuse(unify::Color::ColorCyan() );
	GetGeometryManager()->Add( "sphere", new Mesh( commonParameters + sphereParameters ) );

    shapes::CylinderParameters cylinderParameters;
	cylinderParameters.SetSegments( 24 );
	cylinderParameters.SetRadius( 1.0f );
	cylinderParameters.SetHeight( 2.0f );
	cylinderParameters.SetDiffuse(unify::Color::ColorMagenta() );
	GetGeometryManager()->Add( "cylinder", new Mesh( commonParameters + cylinderParameters ) );

    shapes::TubeParameters tubeParameters;
	tubeParameters.SetSegments( 24 );
	tubeParameters.SetMajorRadius( 1.0f );
	tubeParameters.SetMinorRadius( 0.5f );
	tubeParameters.SetHeight( 2.0f );
	tubeParameters.SetDiffuse(unify::Color::ColorRed() );
	GetGeometryManager()->Add( "tube", new Mesh( commonParameters + tubeParameters ) );

    shapes::PlaneParameters planeParameters;
	planeParameters.SetSegments( 2 );
    planeParameters.SetSize( unify::Size< float >( 2.0f, 2.0f ) );
	planeParameters.SetDiffuse(unify::Color::ColorCyan() );
	GetGeometryManager()->Add( "plane", new Mesh( commonParameters + planeParameters ) );

    shapes::ConeParameters coneParameters;
	coneParameters.SetSegments( 24 );
	coneParameters.SetRadius( 1 );
	coneParameters.SetHeight( 2 );
	coneParameters.SetDiffuse(unify::Color::ColorRed() );
	GetGeometryManager()->Add( "cone", new Mesh( commonParameters + coneParameters ) );


    // Add shapes...

    object.reset( new scene::Object( GetGeometryManager()->Find( "cube" ) ) );
	object->GetFrame().SetPosition( unify::V3< float >( -4.5f, 3, 0 ) );
	scene1->Add( "cube", object );
    
    object.reset( new scene::Object( GetGeometryManager()->Find( "pointfield" ) ) );
	object->GetFrame().SetPosition( unify::V3< float >( -1.5f, 3, 0 ) );
	scene1->Add( "pointfield", object );

	object.reset( new scene::Object( GetGeometryManager()->Find( "pointring" ) ) );
	object->GetFrame().SetPosition( unify::V3< float >( 1.5f, 3, 0 ) );
	scene1->Add( "pointring", object );

	object.reset( new scene::Object( GetGeometryManager()->Find( "dashring" ) ) );
	object->GetFrame().SetPosition( unify::V3< float >( 4.5f, 3, 0 ) );
	scene1->Add( "dashring", object );

    object.reset( new scene::Object( GetGeometryManager()->Find( "pyramid" ) ) );
    object->GetFrame().SetPosition( unify::V3< float >( -4.5f, 0, 0 ) );
	scene1->Add( "pyramid", object );

	object.reset( new scene::Object( GetGeometryManager()->Find( "circle" ) ) );
	object->GetFrame().SetPosition( unify::V3< float >( -1.5f, 0, 0 ) );
	scene1->Add( "circle", object );

	object.reset( new scene::Object( GetGeometryManager()->Find( "sphere" ) ) );
	object->GetFrame().SetPosition( unify::V3< float >( 1.5f, 0, 0 ) );
	scene1->Add( "sphere", object );

	object.reset( new scene::Object( GetGeometryManager()->Find( "cylinder" ) ) );
	object->GetFrame().SetPosition( unify::V3< float >( 4.5f, 0, 0 ) );
	scene1->Add( "cylinder", object );

	object.reset( new scene::Object( GetGeometryManager()->Find( "tube" ) ) );
	object->GetFrame().SetPosition( unify::V3< float >( -4.5f, -3, 0 ) );
	scene1->Add( "tube", object );

    object.reset( new scene::Object( GetGeometryManager()->Find( "plane" ) ) );
	object->GetFrame().SetPosition( unify::V3< float >( -1.5f, -3, 0 ) );
	scene1->Add( "plane", object );

	object.reset( new scene::Object( GetGeometryManager()->Find( "cone" ) ) );
	object->GetFrame().SetPosition( unify::V3< float >( 1.5f, -3, 0 ) );
	scene1->Add( "cone", object );

    scene::Object::shared_ptr camera1;
	camera1.reset( new scene::Object() );
    camera1->GetListenerMap().Add( "OnUpdate", events::Listener::shared_ptr( new CameraRotation ) );
	scene1->Add( "camera1", camera1 );
	scene1->SetCamera( "camera1" );
	scene1->SetSize( GetOS().GetResolution() );

	// Scene 2...
    scene::Scene * scene2 = new scene::Scene;
    GetSceneManager()->AddScene( "scene2", scene2 );

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

	scene2->Add( "group", object );
    scene::Object::shared_ptr camera2;
	camera2.reset( new scene::Object );
    camera2->GetListenerMap().Add( "OnUpdate", events::Listener::shared_ptr( new CameraRotation ) );
	scene2->Add( "camera2", camera2 );
	scene2->SetCamera( "camera2" );
	scene2->SetSize( GetOS().GetResolution() / 3 );
	scene2->SetPosition( unify::V2< int >( 20, GetOS().GetResolution().height - ( GetOS().GetResolution().height / 3 ) - 20 ) );
}

bool MyGame::Update( unify::Seconds elapsed, IInput & input )
{
	return Game::Update( elapsed, renderInfo, input );
}

void MyGame::Render()
{
}

void MyGame::Shutdown()
{
	Game::Shutdown();
}
