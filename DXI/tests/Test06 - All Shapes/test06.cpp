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
#include <dxi/win/DXILib.h>
#include <DXIWinMain.h>

using namespace dxi;
using namespace core;

class MyGame : public Game
{
public:
	void Startup()
	{
		// Load our effect from a file...
		GetManager< Effect >()->Add( "texture3d", "media/EffectTextured.effect" ); // Load an effect into the manager.

		Effect::ptr effect = GetManager< Effect >()->Find( "texture3d" ); // Demonstrate how the effect is pulled from the manager by name.

		scene::Scene::shared_ptr mainScene = GetSceneManager()->Add( "main" );

		scene::Object::shared_ptr cameraObject = mainScene->GetRoot()->AddChild( "camera" );

		scene::Camera::shared_ptr camera( new scene::Camera( cameraObject ) );
		mainScene->SetCamera( "camera" );

		mainScene->GetCamera().SetProjection(
			unify::Matrix::MatrixPerspectiveFovLH( 3.1415926535f / 4.0f, GetOS().GetResolution().AspectRatioWH(), 1, 1000 )
		);
		camera->GetObject()->GetFrame().SetPosition( unify::V3< float >( 0, 5, -10 ) );
		camera->GetObject()->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );

		VertexDeclaration::ptr vd = effect->GetVertexShader()->GetVertexDeclaration();

		auto group = mainScene->GetRoot()->AddChild( "group" );

		// All shapes are created by hand to show the specific attributes.

		std::vector< unify::V3< float > > positions{
			unify::V3< float >( -4.5f, 3.0f, 0.0f ),
			unify::V3< float >( -1.5f, 3.0f, 0.0f ),
			unify::V3< float >( 1.5f, 3.0f, 0.0f ),
			unify::V3< float >( 4.5f, 3.0f, 0.0f ),

			unify::V3< float >( -4.5f, 0.0f, 0.0f ),
			unify::V3< float >( -1.5f, 0.0f, 0.0f ),
			unify::V3< float >( 1.5f, 0.0f, 0.0f ),
			unify::V3< float >( 4.5f, 0.0f, 0.0f ),

			unify::V3< float >( -4.5f, -3.0f, 0.0f ),
			unify::V3< float >( -1.5f, -3.0f, 0.0f ),
			unify::V3< float >( 1.5f, -3.0f, 0.0f ),
			unify::V3< float >( 4.5f, -3.0f, 0.0f )
		};

		size_t shape = 0;

		auto & cube = *group->AddChild( "cube" );
		shapes::CubeParameters cubeParameters;
		cubeParameters.SetEffect( effect );
		cubeParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
		cubeParameters.SetDiffuseFaces( unify::Color::ColorRed(), unify::Color::ColorGreen(), unify::Color::ColorBlue(), unify::Color::ColorYellow(), unify::Color::ColorCyan(), unify::Color::ColorMagenta() );
		cubeParameters.SetCenter( positions[shape++] );
		cube.SetGeometry( Geometry::shared_ptr( shapes::CreateShape( cubeParameters ) ) );

		auto & pointField = *group->AddChild( "pointField" );
		shapes::PointFieldParameters pointFieldParameters;
		pointFieldParameters.SetEffect( effect );
		pointFieldParameters.SetMinorRadius( 0.5f );
		pointFieldParameters.SetMajorRadius( 1.0f );
		pointFieldParameters.SetCount( 1000 );
		pointFieldParameters.SetCenter( positions[shape++] );
		pointFieldParameters.SetDiffuse( unify::Color::ColorWhite() );
		pointField.SetGeometry( Geometry::shared_ptr( shapes::CreateShape( pointFieldParameters ) ) );

		auto & pointRing = *group->AddChild( "pointRing" );
		shapes::PointRingParameters pointRingParameters;
		pointRingParameters.SetEffect( effect );
		pointRingParameters.SetMinorRadius( 0.25f );
		pointRingParameters.SetMajorRadius( 0.75f );
		pointRingParameters.SetCount( 1000 );
		pointRingParameters.SetCenter( positions[shape++] );
		pointRingParameters.SetDiffuse( unify::Color::ColorRed() );
		pointRing.SetGeometry( Geometry::shared_ptr( shapes::CreateShape( pointRingParameters ) ) );

		auto & dashRing = *group->AddChild( "dashRing" );
		shapes::DashRingParameters dashRingParameters;
		dashRingParameters.SetEffect( effect );
		dashRingParameters.SetMinorRadius( 0.5f );
		dashRingParameters.SetMajorRadius( 1.0f );
		dashRingParameters.SetSize( 0.5f );
		dashRingParameters.SetCount( 5 );
		dashRingParameters.SetDiffuse( unify::Color::ColorGreen() );
		dashRingParameters.SetCenter( positions[shape++] );
		dashRing.SetGeometry( Geometry::shared_ptr( shapes::CreateShape( dashRingParameters ) ) );

		auto & pyramid = *group->AddChild( "pyramid" );
		shapes::PyramidParameters pyramidParameters;
		pyramidParameters.SetEffect( effect );
		pyramidParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
		pyramidParameters.SetDiffuse( unify::Color::ColorYellow() );
		pyramidParameters.SetCenter( positions[shape++] );
		pyramid.SetGeometry( Geometry::shared_ptr( shapes::CreateShape( pyramidParameters ) ) );

		auto & circle = *group->AddChild( "circle" );
		shapes::CircleParameters circleParameters;
		circleParameters.SetEffect( effect );
		circleParameters.SetSegments( 24 );
		circleParameters.SetRadius( 1.0f );
		circleParameters.SetDiffuse( unify::Color::ColorBlue() );
		circleParameters.SetCenter( positions[shape++] );
		circle.SetGeometry( Geometry::shared_ptr( shapes::CreateShape( circleParameters ) ) );

		auto & sphere = *group->AddChild( "sphere" );
		shapes::SphereParameters sphereParameters;
		sphereParameters.SetEffect( effect );
		sphereParameters.SetSegments( 24 );
		sphereParameters.SetRadius( 1.0f );
		sphereParameters.SetDiffuse( unify::Color::ColorCyan() );
		sphereParameters.SetCenter( positions[shape++] );
		sphere.SetGeometry( Geometry::shared_ptr( shapes::CreateShape( sphereParameters ) ) );

		auto & cylinder = *group->AddChild( "cylinder" );
		shapes::CylinderParameters cylinderParameters;
		cylinderParameters.SetEffect( effect );
		cylinderParameters.SetSegments( 24 );
		cylinderParameters.SetRadius( 1.0f );
		cylinderParameters.SetHeight( 2.0f );
		cylinderParameters.SetDiffuse( unify::Color::ColorMagenta() );
		cylinderParameters.SetCenter( positions[shape++] );
		cylinderParameters.SetCaps( true );
		cylinder.SetGeometry( Geometry::shared_ptr( shapes::CreateShape( cylinderParameters ) ) );

		auto & tube = *group->AddChild( "tube" );
		shapes::TubeParameters tubeParameters;
		tubeParameters.SetEffect( effect );
		tubeParameters.SetSegments( 24 );
		tubeParameters.SetMajorRadius( 1.0f );
		tubeParameters.SetMinorRadius( 0.5f );
		tubeParameters.SetHeight( 2.0f );
		tubeParameters.SetDiffuse( unify::Color::ColorRed() );
		tubeParameters.SetCenter( positions[shape++] );
		tube.SetGeometry( Geometry::shared_ptr( shapes::CreateShape( tubeParameters ) ) );

		auto & plane = *group->AddChild( "plane" );
		shapes::PlaneParameters planeParameters;
		planeParameters.SetEffect( effect );
		planeParameters.SetSegments( 2 );
		planeParameters.SetSize( unify::Size< float >( 2.0f, 2.0f ) );
		planeParameters.SetDiffuse( unify::Color::ColorCyan() );
		planeParameters.SetCenter( positions[shape++] );
		plane.SetGeometry( Geometry::shared_ptr( shapes::CreateShape( planeParameters ) ) );

		auto & cone = *group->AddChild( "cone" );
		shapes::ConeParameters coneParameters;
		coneParameters.SetEffect( effect );
		coneParameters.SetSegments( 24 );
		coneParameters.SetRadius( 1.0f );
		coneParameters.SetHeight( 2.0f );
		coneParameters.SetDiffuse( unify::Color::ColorGreen() );
		coneParameters.SetCenter( positions[shape++] );
		coneParameters.SetCaps( true );
		cone.SetGeometry( Geometry::shared_ptr( shapes::CreateShape( coneParameters ) ) );

		//mainScene->GetRenderInfo().SetOption( RenderOption::NoFrame, true );
	}

	bool Update( RenderInfo & renderInfo, IInput & input )
	{
		static size_t axisIndex = 1;
		static unify::Angle totalRotation{};
		unify::Angle rotation = unify::Angle::AngleInRadians( renderInfo.GetDelta() );
		totalRotation += rotation;
		if( totalRotation.Fix360() != 0 )
		{
			rotation = totalRotation; // Left over
			axisIndex++;
			if( axisIndex >= 3 )
			{
				axisIndex = 0;
			}
		}					  

		unify::V3< float > axis( (axisIndex == 0) ? 1.0f : 0.0f, (axisIndex == 1) ? 1.0f : 0.0f, (axisIndex == 2) ? 1.0f : 0.0f );

		auto group = GetSceneManager()->Find( "main" )->FindObject( "camera" );
		group->GetFrame().RotateAbout( axis, rotation );

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

RegisterGame( game );
