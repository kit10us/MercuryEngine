// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/scene/Camera.h>
#include <dxi/Mesh.h>
#include <dxi/TextureManager.h>
#include <dxi/GeometryManager.h>
#include <dxi/GeometryGroup.h>

using namespace dxi;
using namespace core;

class MyGame : public Game
{
protected:
	GeometryGroup m_geometryGroup;
	float m_rotation;
	scene::Object::shared_ptr m_camera;
	scene::Camera m_cameraController;

public:

	void Startup()
	{
		Game::Startup();

		GetTextureManager()->LoadFromFile( "borgcube", "media/borgcube.bmp" );

        // Create geometry...
        
        shapes::ShapeBaseParameters commonParameters;
        commonParameters.SetFVF( FVF::XYZ | FVF::Diffuse | FVF::Tex1 );
		commonParameters.SetTexture( GetTextureManager()->Find( "borgcube" ) );

        shapes::CubeParameters cubeParameters;
        cubeParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
		cubeParameters.SetDiffuseFaces( unify::Color::ColorRed(), unify::Color::ColorGreen(), unify::Color::ColorBlue(), unify::Color::ColorYellow(), unify::Color::ColorCyan(), unify::Color::ColorMagenta() );
		cubeParameters.SetCenter( unify::V3< float >( -4.5, 3, 0 ) );
		GetGeometryManager()->Add( "cube",  new Mesh( commonParameters + cubeParameters ) );
		
        shapes::PointFieldParameters pointFieldParameters;
		pointFieldParameters.SetMinorRadius( 0.5f );
		pointFieldParameters.SetMajorRadius( 1.0f );
		pointFieldParameters.SetCount( 1000 );
		pointFieldParameters.SetCenter( unify::V3< float >( -1.5, 3, 0 ) );
		GetGeometryManager()->Add( "pointfield", new Mesh( commonParameters + pointFieldParameters ) );

        shapes::PointRingParameters pointRingParameters;
		pointRingParameters.SetMinorRadius( 0.25f );
		pointRingParameters.SetMajorRadius( 0.75f );
		pointRingParameters.SetCount( 1000 );
		pointRingParameters.SetCenter( unify::V3< float >( 1.5, 3, 0 ) );
		GetGeometryManager()->Add( "pointring", new Mesh( commonParameters + pointRingParameters ) );

        shapes::DashRingParameters dashRingParameters;
		dashRingParameters.SetMinorRadius( 0.625f );
		dashRingParameters.SetMajorRadius( 1 );
		dashRingParameters.SetCount( 5 );
		dashRingParameters.SetDiffuse( unify::Color::ColorGreen() );
		dashRingParameters.SetCenter( unify::V3< float >( 4.5, 3, 0 ) );
		GetGeometryManager()->Add( "dashring", new Mesh( commonParameters + dashRingParameters ) );

        shapes::PyramidParameters pyramidParameters;
        pyramidParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
		pyramidParameters.SetDiffuse( unify::Color::ColorYellow() );
		pyramidParameters.SetCenter( unify::V3< float >( -4.5, 0, 0 ) );
		GetGeometryManager()->Add( "pyramid", new Mesh( commonParameters + pyramidParameters ) );

        shapes::CircleParameters circleParameters;
		circleParameters.SetSegments( 24 );
		circleParameters.SetRadius( 1 );
		circleParameters.SetDiffuse( unify::Color::ColorRed() );
		circleParameters.SetCenter( unify::V3< float >( -1.5, 0, 0 ) );
		GetGeometryManager()->Add( "circle", new Mesh( commonParameters + circleParameters ) );

        shapes::SphereParameters sphereParameters;
		sphereParameters.SetSegments( 24 );
		sphereParameters.SetRadius( 1 );
		sphereParameters.SetDiffuse( unify::Color::ColorRed() );
		sphereParameters.SetCenter( unify::V3< float >( 1.5, 0, 0 ) );
		GetGeometryManager()->Add( "sphere", new Mesh( commonParameters + sphereParameters ) );

        shapes::CylinderParameters cylinderParameters;
		cylinderParameters.SetSegments( 24 );
		cylinderParameters.SetRadius( 1 );
		cylinderParameters.SetHeight( 2 );
		cylinderParameters.SetDiffuse( unify::Color::ColorMagenta() );
		cylinderParameters.SetCenter( unify::V3< float >( 4.5, 0, 0 ) );
		GetGeometryManager()->Add( "cylinder", new Mesh( commonParameters + cylinderParameters ) );

        shapes::TubeParameters tubeParameters;
		tubeParameters.SetSegments( 24 );
		tubeParameters.SetMajorRadius( 1 );
		tubeParameters.SetMinorRadius( 0.5f );
		tubeParameters.SetHeight( 2 );
		tubeParameters.SetDiffuse( unify::Color::ColorBlue() );
		tubeParameters.SetCenter( unify::V3< float >( -4.5, -3, 0 ) );
		GetGeometryManager()->Add( "tube", new Mesh( commonParameters + tubeParameters ) );

        shapes::PlaneParameters planeParameters;
		planeParameters.SetSegments( 2 );
        planeParameters.SetSize( unify::Size< float >( 2, 2 ) );
		planeParameters.SetDiffuse( unify::Color::ColorCyan() );
		planeParameters.SetCenter( unify::V3< float >( -1.5, -3, 0 ) );
		GetGeometryManager()->Add( "plane", new Mesh( commonParameters + planeParameters ) );

        shapes::ConeParameters coneParameters;
		coneParameters.SetSegments( 24 );
		coneParameters.SetRadius( 1 );
		coneParameters.SetHeight( 2 );
		coneParameters.SetDiffuse( unify::Color::ColorRed() );
		coneParameters.SetCenter( unify::V3< float >( 1.5, -3, 0 ) );
		GetGeometryManager()->Add( "cone", new Mesh( commonParameters + coneParameters ) );

		m_geometryGroup.Add( GetGeometryManager()->Find( "cube" ) );
		m_geometryGroup.Add( GetGeometryManager()->Find( "pointfield" ) );
		m_geometryGroup.Add( GetGeometryManager()->Find( "pointring" ) );
		m_geometryGroup.Add( GetGeometryManager()->Find( "dashring" ) );
		m_geometryGroup.Add( GetGeometryManager()->Find( "pyramid" ) );
		m_geometryGroup.Add( GetGeometryManager()->Find( "circle" ) );
		m_geometryGroup.Add( GetGeometryManager()->Find( "sphere" ) );
		m_geometryGroup.Add( GetGeometryManager()->Find( "cylinder" ) );
		m_geometryGroup.Add( GetGeometryManager()->Find( "tube" ) );
		m_geometryGroup.Add( GetGeometryManager()->Find( "plane" ) );
		m_geometryGroup.Add( GetGeometryManager()->Find( "cone" ) );

		m_camera.reset( new scene::Object );
		m_cameraController.SetObject( m_camera );

		m_rotation = 0;
	}

	bool Update( unify::Seconds elapsed, IInput & input )
	{
		unify::V3< float > eye( 10.0f * sin( m_rotation ), 10.0f * sin( m_rotation ), 10.0f * cos( m_rotation ) );
		m_camera->GetFrame().SetPosition( eye );
		m_camera->GetFrame().LookAt( unify::V3< float >(0, 0, 0) );
		m_cameraController.Use();

		m_rotation += 0.01f;
		if ( m_rotation > 3.1415936535f * 2 )
		{
			m_rotation -= 3.1415936545f * 2;
		}

		return Game::Update( elapsed, input );
	}

	void Render()
	{

		RenderInfo renderInfo;
		m_geometryGroup.Render( renderInfo, 0 );
	}

	void Shutdown()
	{
		m_geometryGroup.Clear();
		Game::Shutdown();
	}
} game;

