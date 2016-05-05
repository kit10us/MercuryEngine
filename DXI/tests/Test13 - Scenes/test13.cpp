// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/geo/GeometryGroup.h>
#include <dxi/geo/Terra.h>
#include <dxi/loader/EffectLoader.h>
#include <unify/Size3.h>

using namespace dxi;
using namespace core;

class MyGame : public Game
{
protected:
	scene::Scene::shared_ptr m_scene1;
	scene::Scene::shared_ptr m_scene2;
	scene::Object::shared_ptr m_camera1;
	scene::Object::shared_ptr m_camera2;
	float m_rotation;

public:
	bool Setup( core::IOS & os )
	{
		os.SetResolution( unify::Size< unsigned int >( 800, 600 ) );
		os.SetFullscreen( false );
		return true;
	}

	void Startup()
	{
		Game::Startup();

		// Scene 1...
		m_scene1.reset( new scene::Scene );

		Effect::shared_ptr colorEffect = loader::EffectLoader( "color_3d", "media/EffectColor.xml", GetManagers() );

		shapes::CubeParameters cubeParameters;
        cubeParameters.SetEffect( colorEffect );
		cubeParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
        cubeParameters.SetDiffuseFaces( unify::Color::ColorRed(), unify::Color::ColorGreen(), unify::Color::ColorBlue(), unify::Color::ColorYellow(), unify::Color::ColorCyan(), unify::Color::ColorMagenta() );
		cubeParameters.SetCenter( unify::V3< float >( -4.5, 3, 0 ) );
		m_scene1->Add( "cube", scene::MakeObject( new geo::Mesh( cubeParameters ) ) );

		shapes::PointFieldParameters pointFieldParameters; 
		pointFieldParameters.SetEffect( colorEffect );
		pointFieldParameters.SetMinorRadius( 0.5f );
		pointFieldParameters.SetMajorRadius( 1.0f );
		pointFieldParameters.SetCount( 1000 );
		pointFieldParameters.SetCenter( unify::V3< float >( -1.5, 3, 0 ) );
		pointFieldParameters.SetDiffuse( unify::Color::ColorWhite() );
        m_scene1->Add( "PointField", scene::MakeObject( new geo::Mesh( pointFieldParameters ) ) );
				
		shapes::PointRingParameters pointRingParameters; 
		pointRingParameters.SetEffect( colorEffect );
		pointRingParameters.SetMinorRadius( 0.25f );
		pointRingParameters.SetMajorRadius( 0.75f );
		pointRingParameters.SetCount( 1000 );
		pointRingParameters.SetCenter( unify::V3< float >( 1.5, 3, 0 ) );
		pointRingParameters.SetDiffuse( unify::Color::ColorRed() );
		m_scene1->Add( "PointRing", scene::MakeObject( new geo::Mesh( pointRingParameters ) ) );

		shapes::DashRingParameters dashRingParameters; 
		dashRingParameters.SetEffect( colorEffect );
		dashRingParameters.SetMinorRadius( 0.625f );
		dashRingParameters.SetMajorRadius( 1.0f );
		dashRingParameters.SetSize( 0.5f );
		dashRingParameters.SetCount( 5 );
		dashRingParameters.SetDiffuse( unify::Color::ColorGreen() );
		dashRingParameters.SetCenter( unify::V3< float >( 4.5, 3, 0 ) );
		m_scene1->Add( "DashRing", scene::MakeObject( new geo::Mesh( dashRingParameters ) ) );

		shapes::PyramidParameters pyramidParameters; 
		pyramidParameters.SetEffect( colorEffect );
		pyramidParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
		pyramidParameters.SetDiffuse( unify::Color::ColorYellow() );
		pyramidParameters.SetCenter( unify::V3< float >( -4.5, 0, 0 ) );
		m_scene1->Add( "Pyramid", scene::MakeObject( new geo::Mesh( pyramidParameters ) ) );

		shapes::CircleParameters circleParameters; 
		circleParameters.SetEffect( colorEffect );
		circleParameters.SetSegments( 24 );
		circleParameters.SetRadius( 1.0f );
		circleParameters.SetDiffuse( unify::Color::ColorBlue() );
		circleParameters.SetCenter( unify::V3< float >( -1.5, 0, 0 ) );
		m_scene1->Add( "Circle", scene::MakeObject( new geo::Mesh( circleParameters ) ) );

		shapes::SphereParameters sphereParameters; 
		sphereParameters.SetEffect( colorEffect );
		sphereParameters.SetSegments( 24 );
		sphereParameters.SetRadius( 1.0f );
		sphereParameters.SetDiffuse( unify::Color::ColorCyan() );
		sphereParameters.SetCenter( unify::V3< float >( 1.5, 0, 0 ) );
		m_scene1->Add( "Sphere", scene::MakeObject( new geo::Mesh( sphereParameters ) ) );

		shapes::CylinderParameters cylinderParameters; 
		cylinderParameters.SetEffect( colorEffect );
		cylinderParameters.SetSegments( 24 );
		cylinderParameters.SetRadius( 1.0f );
		cylinderParameters.SetHeight( 2.0f );
		cylinderParameters.SetDiffuse( unify::Color::ColorMagenta() );
		cylinderParameters.SetCenter( unify::V3< float >( 4.5, 0, 0 ) );
		m_scene1->Add( "Cylinder", scene::MakeObject( new geo::Mesh( cylinderParameters ) ) );

		shapes::TubeParameters tubeParameters; 
		tubeParameters.SetEffect( colorEffect );
		tubeParameters.SetSegments( 24 );
		tubeParameters.SetMajorRadius( 1.0f );
		tubeParameters.SetMinorRadius( 0.5f );
		tubeParameters.SetHeight( 2.0f );
		tubeParameters.SetDiffuse( unify::Color::ColorRed() );
		tubeParameters.SetCenter( unify::V3< float >( -4.5, -3, 0 ) );
		m_scene1->Add( "Tube", scene::MakeObject( new geo::Mesh( tubeParameters ) ) );

		shapes::PlaneParameters planeParameters; 
		planeParameters.SetEffect( colorEffect );
		planeParameters.SetSegments( 2 );
		planeParameters.SetSize( unify::Size< float >( 2.0f, 2.0f ) );
		planeParameters.SetDiffuse( unify::Color::ColorCyan() );
		planeParameters.SetCenter( unify::V3< float >( -1.5, -3, 0 ) );
		m_scene1->Add( "Plane", scene::MakeObject( new geo::Mesh( planeParameters ) ) );

		shapes::ConeParameters coneParameters; 
		coneParameters.SetEffect( colorEffect );
		coneParameters.SetSegments( 24 );
		coneParameters.SetRadius( 1.0f );
		coneParameters.SetHeight( 2.0f );
		coneParameters.SetDiffuse( unify::Color::ColorGreen() );
		coneParameters.SetCenter( unify::V3< float >( 1.5, -3, 0 ) );
		m_scene1->Add( "Cone", scene::MakeObject( new geo::Mesh( coneParameters ) ) );

		m_camera1.reset( new scene::Object );
		m_scene1->Add( "camera", m_camera1 );
		m_scene1->SetCamera( "camera" );
		m_scene1->SetSize( GetOS().GetResolution() );


		// Scene 2...
		m_scene2.reset( new scene::Scene );

		GetTextureManager()->Add( "land", new Texture( "media/earthb.bmp", TEXTURE_LOCKABLE ) );
		GetTextureManager()->Add( "cutout", new Texture( "media/EarthWaterCutoutMap.bmp", TEXTURE_LOCKABLE ) );

		Effect::shared_ptr landEffect = loader::EffectLoader( "land", "media/EffectTextured.xml", GetManagers() );
		landEffect->SetBlend( Blend( Usage::False ) );
		landEffect->SetTexture( 0, FindTexture( "land" ) );

		Effect::shared_ptr cutoutEffect = loader::EffectLoader( "cutout", "media/EffectTextured.xml", GetManagers() );
		cutoutEffect->SetBlend( Blend( Usage::True, Blend::Effect::One, Blend::Effect::One ) );
		cutoutEffect->SetTexture( 0, FindTexture( "land" ) );

		geo::GeometryGroup * geometryGroup = new geo::GeometryGroup();
		geo::Terra * terra;

        geo::Terra::Parameters terraParameters;
		terraParameters.SetSize( unify::Size< float >( 10, 7 ) );
		terraParameters.SetEffect( FindEffect( "land" ) );
		terraParameters.SetRowsColumns( unify::RowColumn< unsigned int >( 200, 140 ) );
		terraParameters.SetConstant( 0.0f );
		terraParameters.SetTexArea( unify::TexArea::Full() );
		terraParameters.SetHeightMap( geo::Terra::TextureOpMap( FindTexture( "land" ), unify::ColorUnit::ColorUnitARGB( 0.0f, 0.7f, 0.25f, -1.0f ) ) );
		terra = new geo::Terra( terraParameters );
		geometryGroup->Add( geo::Geometry::shared_ptr( terra ) );

		terraParameters.Reset();
		terraParameters.SetSize( unify::Size< float >( 10, 7 ) );
		terraParameters.SetEffect( FindEffect( "cutout" ) );
		terraParameters.SetRowsColumns( unify::RowColumn< unsigned int >( 10, 7 ) );
		terraParameters.SetConstant( 0.1f );
		terraParameters.SetTexArea( unify::TexArea::Full() );
		terraParameters.SetAlphaMap( geo::Terra::TextureOpMap( FindTexture( "cutout" ), unify::ColorUnit::ColorUnitRed( 1.0f, 0.0f ) ) );
		terra = new geo::Terra( terraParameters );
		geometryGroup->Add( geo::Geometry::shared_ptr( terra ) );

		m_scene2->Add( "terrain", scene::MakeObject( geometryGroup ) );

		m_camera2.reset( new scene::Object );
		m_scene2->Add( "camera2", m_camera2 );
		m_scene2->SetCamera( "camera2" );
		m_scene2->SetSize( GetOS().GetResolution() / 3 );
		m_scene2->SetPosition( unify::V2< int >( 20, GetOS().GetResolution().height - ( GetOS().GetResolution().height / 3 ) - 20 ) );

		m_rotation = 0;
	}

	bool Update( unify::Seconds elapsed, IInput & input )
	{
		unify::V3< float > eye( 10.0f * sin( m_rotation ), 10.0f * sin( m_rotation ), 10.0f * cos( m_rotation ) );
		m_camera1->GetFrame().SetPosition( eye );
		m_camera1->GetFrame().LookAt( unify::V3< float >(0, 0, 0) );
		m_camera2->GetFrame().SetPosition( eye );
		m_camera2->GetFrame().LookAt( unify::V3< float >(0, 0, 0) );

		m_rotation += 0.01f;
		if ( m_rotation > 3.1415936535f * 2 )
		{
			m_rotation -= 3.1415936545f * 2;
		}

		return Game::Update( elapsed, input );
	}

	void Render()
	{
		m_scene1->Render();
		m_scene2->Render();
	}

	void Shutdown()
	{
		m_scene1.reset();
		m_scene2.reset();
		
		Game::Shutdown();
	}
} game;

