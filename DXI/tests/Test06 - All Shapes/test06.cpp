// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

/// <summary>
/// "All Programmatic 3D Shapes"
/// goals and requirements:
/// * Example of the creation of the programmatic
/// </summary>

#include <dxi/core/Game.h>
#include <dxi/GeometryGroup.h>
#include <dxi/shapes/ShapeCreators.h>

using namespace dxi;
using namespace core;

class MyGame : public Game
{
protected:

public:
	void Startup()
	{
		// Call base startup.
		Game::Startup();

		// Load our effect from a file...
		GetManager< Effect >()->Add( "textured_3d", "media/EffectTextured.xml" ); // Load an effect into the manager.

		Effect::shared_ptr effect = GetManager< Effect >()->Find( "textured_3d" ); // Demonstrate how the effect is pulled from the manager by name.

		scene::Scene::shared_ptr mainScene = GetSceneManager()->Add( "main", new scene::Scene );

		scene::Object::shared_ptr cameraObject( new scene::Object ); // Necessary to store the camera object -somewhere- outside of the camera, as is weak_ptr in camera.
		mainScene->Add( "camera", cameraObject );		
		scene::Camera::shared_ptr camera( new scene::Camera( cameraObject ) );
		mainScene->SetCamera( "camera" );
		mainScene->GetCamera().SetProjection( unify::Matrix::MatrixPerspectiveFovLH( D3DX_PI / 4.0f, GetOS()->GetResolution().AspectRatioHW(), 1, 1000 ) );
		camera->GetObject()->GetFrame().SetPosition( unify::V3< float >( 0, 5, -17 ) );
		camera->GetObject()->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );

		VertexDeclaration vd = effect->GetVertexShader()->GetVertexDeclaration();

		GeometryGroup * geometryGroup = new GeometryGroup();

		// All shapes are created by hand to show the specific attributes.
	
		// Cube in Upper-left
        shapes::CubeParameters cubeParameters;
        cubeParameters.SetEffect( effect );
        cubeParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
        cubeParameters.SetDiffuseFaces( unify::Color::ColorRed(), unify::Color::ColorGreen(), unify::Color::ColorBlue(), unify::Color::ColorYellow(),	unify::Color::ColorCyan(), unify::Color::ColorMagenta() );
        cubeParameters.SetCenter( unify::V3< float >( -4.5f, 3.0f, 0.0f ) );
		geometryGroup->Add( Geometry::shared_ptr( shapes::CreateShape( cubeParameters ) ) );
  
		shapes::PointFieldParameters pointFieldParameters;
		pointFieldParameters.SetEffect( effect );
        pointFieldParameters.SetMinorRadius( 0.5f );
        pointFieldParameters.SetMajorRadius( 1.0f );
        pointFieldParameters.SetCount( 1000 );
		pointFieldParameters.SetCenter( unify::V3< float >( -1.5, 3, 0 ) );
		pointFieldParameters.SetDiffuse( unify::Color::ColorWhite() );
		geometryGroup->Add( Geometry::shared_ptr( shapes::CreateShape( pointFieldParameters ) ) );

        shapes::PointRingParameters pointRingParameters;
		pointRingParameters.SetEffect( effect );
        pointRingParameters.SetMinorRadius( 0.25f );
		pointRingParameters.SetMajorRadius( 0.75f );
		pointRingParameters.SetCount( 1000 );
		pointRingParameters.SetCenter( unify::V3< float >( 1.5, 3, 0 ) );
		pointRingParameters.SetDiffuse( unify::Color::ColorRed() );
		geometryGroup->Add( Geometry::shared_ptr( shapes::CreateShape( pointRingParameters ) ) );

        shapes::DashRingParameters dashRingParameters;
        dashRingParameters.SetEffect( effect );
		dashRingParameters.SetMinorRadius( 0.5f );
		dashRingParameters.SetMajorRadius( 1.0f );
		dashRingParameters.SetSize( 0.5f );
		dashRingParameters.SetCount( 5 );
		dashRingParameters.SetDiffuse( unify::Color::ColorGreen() );
		dashRingParameters.SetCenter( unify::V3< float >( 4.5, 3, 0 ) );
		geometryGroup->Add( Geometry::shared_ptr( shapes::CreateShape( dashRingParameters ) ) );

		shapes::PyramidParameters pyramidParameters; 
		pyramidParameters.SetEffect( effect );
		pyramidParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
		pyramidParameters.SetDiffuse( unify::Color::ColorYellow() );
		pyramidParameters.SetCenter( unify::V3< float >( -4.5, 0, 0 ) );
		geometryGroup->Add( Geometry::shared_ptr( shapes::CreateShape( pyramidParameters ) ) );

		shapes::CircleParameters circleParameters; 
		circleParameters.SetEffect( effect );
		circleParameters.SetSegments( 24 );
		circleParameters.SetRadius( 1.0f );
		circleParameters.SetDiffuse( unify::Color::ColorBlue() );
		circleParameters.SetCenter( unify::V3< float >( -1.5, 0, 0 ) );
		geometryGroup->Add( Geometry::shared_ptr( shapes::CreateShape( circleParameters ) ) );

		shapes::SphereParameters sphereParameters; 
		sphereParameters.SetEffect( effect );
		sphereParameters.SetSegments( 24 );
		sphereParameters.SetRadius( 1.0f );
		sphereParameters.SetDiffuse( unify::Color::ColorCyan() );
		sphereParameters.SetCenter( unify::V3< float >( 1.5, 0, 0 ) );
		geometryGroup->Add( Geometry::shared_ptr( shapes::CreateShape( sphereParameters ) ) );

		shapes::CylinderParameters cylinderParameters; 
		cylinderParameters.SetEffect( effect );
		cylinderParameters.SetSegments( 24 );
		cylinderParameters.SetRadius( 1.0f );
		cylinderParameters.SetHeight( 2.0f );
		cylinderParameters.SetDiffuse( unify::Color::ColorMagenta() );
		cylinderParameters.SetCenter( unify::V3< float >( 4.5, 0, 0 ) );
		geometryGroup->Add( Geometry::shared_ptr( shapes::CreateShape( cylinderParameters ) ) );

		shapes::TubeParameters tubeParameters; 
		tubeParameters.SetEffect( effect );
		tubeParameters.SetSegments( 24 );
		tubeParameters.SetMajorRadius( 1.0f );
		tubeParameters.SetMinorRadius( 0.5f );
		tubeParameters.SetHeight( 2.0f );
		tubeParameters.SetDiffuse( unify::Color::ColorRed() );
		tubeParameters.SetCenter( unify::V3< float >( -4.5, -3, 0 ) );
		geometryGroup->Add( Geometry::shared_ptr( shapes::CreateShape( tubeParameters ) ) );

		shapes::PlaneParameters planeParameters; 
		planeParameters.SetEffect( effect );
		planeParameters.SetSegments( 2 );
		planeParameters.SetSize( unify::Size< float >( 2.0f, 2.0f ) );
		planeParameters.SetDiffuse( unify::Color::ColorCyan() );
		planeParameters.SetCenter( unify::V3< float >( -1.5, -3, 0 ) );
		geometryGroup->Add( Geometry::shared_ptr( shapes::CreateShape( planeParameters ) ) );

		shapes::ConeParameters coneParameters; 
		coneParameters.SetEffect( effect );
		coneParameters.SetSegments( 24 );
		coneParameters.SetRadius( 1.0f );
		coneParameters.SetHeight( 2.0f );
		coneParameters.SetDiffuse( unify::Color::ColorGreen() );
		coneParameters.SetCenter( unify::V3< float >( 1.5, -3, 0 ) );
		coneParameters.SetCaps( true );
		geometryGroup->Add( Geometry::shared_ptr( shapes::CreateShape( coneParameters ) ) );
		
		scene::Object::shared_ptr object( new scene::Object( Geometry::shared_ptr( geometryGroup ) ) );
		mainScene->Add( object );

		//mainScene->GetRenderInfo().SetOption( RenderOption::NoFrame, true );
	}

	bool Update( unify::Seconds elapsed, IInput & input )
	{
		// Use of camera controls to simplify camera movement...
		scene::Object::shared_ptr cameraObject = GetSceneManager()->Find( "main" )->GetCamera().GetObject();
		cameraObject->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::V2< float >( 1, 0 ), unify::Angle::AngleInRadians( elapsed ) );
		cameraObject->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );

		return Game::Update( elapsed, input );
	}

	void Render()
	{
		Game::Render();
	}

	void Shutdown()
	{
		Game::Shutdown();
	}
} game;

