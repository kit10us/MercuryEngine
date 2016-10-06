// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/factory/PixelShaderFactories.h>
#include <dxi/factory/VertexShaderFactory.h>
#include <dxi/factory/ShapeFactory.h>
#include <dxi/win/DXILib.h>

using namespace dxi;
using namespace core;

class MyGame : public Game
{
public:

	void Startup()
	{
		// Call base startup.
		Game::Startup();

        // Create geometry.
		{
			shapes::ShapeBaseParameters commonParameters;
			commonParameters.SetEffect( GetManager< Effect >()->Add( "borgcube", "media/EffectBorgCube.xml" ) );

			shapes::CubeParameters cubeParameters;
			cubeParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
			cubeParameters.SetDiffuseFaces( unify::Color::ColorRed(), unify::Color::ColorGreen(), unify::Color::ColorBlue(), unify::Color::ColorYellow(), unify::Color::ColorCyan(), unify::Color::ColorMagenta() );
			GetManager< Geometry >()->Add( "cube", shapes::CreateShape( commonParameters + cubeParameters ) );

			shapes::PointFieldParameters pointFieldParameters;
			pointFieldParameters.SetMinorRadius( .5f );
			pointFieldParameters.SetMajorRadius( 1.0f );
			pointFieldParameters.SetCount( 1000 );
			GetManager< Geometry >()->Add( "pointfield", shapes::CreateShape( commonParameters + pointFieldParameters ) );

			shapes::PointRingParameters pointRingParameters;
			pointRingParameters.SetMinorRadius( .25f );
			pointRingParameters.SetMajorRadius( .75f );
			pointRingParameters.SetCount( 1000 );
			GetManager< Geometry >()->Add( "pointring", shapes::CreateShape( commonParameters + pointRingParameters ) );

			shapes::DashRingParameters dashRingParameters;
			dashRingParameters.SetMinorRadius( 0.625f );
			dashRingParameters.SetMajorRadius( 1 );
			dashRingParameters.SetCount( 5 );
			dashRingParameters.SetDiffuse( unify::Color::ColorGreen() );
			GetManager< Geometry >()->Add( "dashring", shapes::CreateShape( commonParameters + dashRingParameters ) );

			shapes::PyramidParameters pyramidParameters;
			pyramidParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
			pyramidParameters.SetDiffuse( unify::Color::ColorYellow() );
			GetManager< Geometry >()->Add( "pyramid", shapes::CreateShape( commonParameters + pyramidParameters ) );

			shapes::CircleParameters circleParameters;
			circleParameters.SetSegments( 24 );
			circleParameters.SetRadius( 1 );
			circleParameters.SetDiffuse( unify::Color::ColorRed() );
			GetManager< Geometry >()->Add( "circle", shapes::CreateShape( commonParameters + circleParameters ) );

			shapes::SphereParameters sphereParameters;
			sphereParameters.SetSegments( 24 );
			sphereParameters.SetRadius( 1 );
			sphereParameters.SetDiffuse( unify::Color::ColorRed() );
			GetManager< Geometry >()->Add( "sphere", shapes::CreateShape( commonParameters + sphereParameters ) );

			shapes::CylinderParameters cylinderParameters;
			cylinderParameters.SetSegments( 24 );
			cylinderParameters.SetRadius( 1 );
			cylinderParameters.SetHeight( 2 );
			cylinderParameters.SetDiffuse( unify::Color::ColorMagenta() );
			GetManager< Geometry >()->Add( "cylinder", shapes::CreateShape( commonParameters + cylinderParameters ) );

			shapes::TubeParameters tubeParameters;
			tubeParameters.SetSegments( 24 );
			tubeParameters.SetMajorRadius( 1 );
			tubeParameters.SetMinorRadius( 0.5f );
			tubeParameters.SetHeight( 2 );
			tubeParameters.SetDiffuse( unify::Color::ColorBlue() );
			GetManager< Geometry >()->Add( "tube", shapes::CreateShape( commonParameters + tubeParameters ) );

			shapes::PlaneParameters planeParameters;
			planeParameters.SetSegments( 2 );
			planeParameters.SetSize( unify::Size< float >( 2, 2 ) );
			planeParameters.SetDiffuse( unify::Color::ColorCyan() );
			GetManager< Geometry >()->Add( "plane", shapes::CreateShape( commonParameters + planeParameters ) );

			shapes::ConeParameters coneParameters;
			coneParameters.SetSegments( 24 );
			coneParameters.SetRadius( 1 );
			coneParameters.SetHeight( 2 );
			coneParameters.SetDiffuse( unify::Color::ColorRed() );
			GetManager< Geometry >()->Add( "cone", shapes::CreateShape( commonParameters + coneParameters ) );
		}

		// Add a scene.
		{
			scene::Scene::shared_ptr scene = GetSceneManager()->Add( "scene" );

			auto cube = scene->Add( "cube" );
			cube->SetGeometry( GetManager< Geometry >()->Find( "cube" ) );
			cube->GetFrame().SetPosition( unify::V3< float >( -4.5f, 3, 0 ) );

			auto pointfield = scene->Add( "pointfield" );
			pointfield->SetGeometry( GetManager< Geometry >()->Find( "pointfield" ) );
			pointfield->GetFrame().SetPosition( unify::V3< float >( -1.5f, 3, 0 ) );

			auto pointring = scene->Add( "pointring" );
			pointring->SetGeometry( GetManager< Geometry >()->Find( "pointring" ) );
			pointring->GetFrame().SetPosition( unify::V3< float >( 1.5f, 3, 0 ) );
			
			auto dashring = scene->Add( "dashring" );
			dashring->SetGeometry( GetManager< Geometry >()->Find( "dashring" ) );
			dashring->GetFrame().SetPosition( unify::V3< float >( 4.5f, 3, 0 ) );
			
			auto pyramid = scene->Add( "pyramid" );
			pyramid->SetGeometry( GetManager< Geometry >()->Find( "pyramid" ) );
			pyramid->GetFrame().SetPosition( unify::V3< float >( -4.5f, 0, 0 ) );
			
			auto circle = scene->Add( "circle" );
			circle->SetGeometry( GetManager< Geometry >()->Find( "circle" ) );
			circle->GetFrame().SetPosition( unify::V3< float >( -1.5f, 0, 0 ) );
			
			auto sphere = scene->Add( "sphere" );
			sphere->SetGeometry( GetManager< Geometry >()->Find( "sphere" ) );
			sphere->GetFrame().SetPosition( unify::V3< float >( 1.5f, 0, 0 ) );
			
			auto cylinder = scene->Add( "cylinder" );
			cylinder->SetGeometry( GetManager< Geometry >()->Find( "cylinder" ) );
			cylinder->GetFrame().SetPosition( unify::V3< float >( 4.5f, 0, 0 ) );
			
			auto tube = scene->Add( "tube" );
			tube->SetGeometry( GetManager< Geometry >()->Find( "tube" ) );
			tube->GetFrame().SetPosition( unify::V3< float >( -4.5f, -3, 0 ) );
			
			auto plane = scene->Add( "plane" );
			plane->SetGeometry( GetManager< Geometry >()->Find( "plane" ) );
			plane->GetFrame().SetPosition( unify::V3< float >( -1.5f, -3, 0 ) );
			
			auto cone = scene->Add( "cone" );
			cone->SetGeometry( GetManager< Geometry >()->Find( "cone" ) );
			cone->GetFrame().SetPosition( unify::V3< float >( 1.5f, -3, 0 ) );

			scene::Object::shared_ptr cameraObject( scene->Add( "camera" ) );
			scene::Camera::shared_ptr camera( new scene::Camera( cameraObject ) );
			scene->SetCamera( "camera" );
			scene->GetCamera().SetProjection( unify::Matrix::MatrixPerspectiveFovLH( D3DX_PI / 4.0f, GetOS().GetResolution().AspectRatioHW(), 1, 1000 ) );
			camera->GetObject()->GetFrame().SetPosition( unify::V3< float >( 0, 5, -17 ) );
			camera->GetObject()->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );
		}
	}

	bool Update( RenderInfo & renderInfo, IInput & input )
	{
		// Use of camera controls to simplify camera movement...
		scene::Object::shared_ptr cameraObject = GetSceneManager()->Find( "scene" )->GetCamera().GetObject();
		cameraObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 1, 0 ), unify::Angle::AngleInRadians( renderInfo.GetDelta() ) );
		cameraObject->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );

		return Game::Update( renderInfo, input );
	}

	void Render( const RenderInfo & renderInfo )
	{
		Game::Render( renderInfo );
	}

	void Shutdown()
	{		
		Game::Shutdown();
	}
} game;

