// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

/// <summary>
/// "All Programmatic 3D Shapes"
/// goals and requirements:
/// * Example of the creation of the programmatic
/// </summary>

#include <me/Game.h>
#include <sg/ShapeCreators.h>
#include <dxi/win/DXILib.h>
#include <DXIWinMain.h>
#include <me/scene/SceneManager.h>
#include <me/scene/BBoxRendererComponent.h>
#include <me/scene/AutoBBoxSceneComponent.h>
#include <me/scene/GeometryComponent.h>
#include <me/scene/CameraComponent.h>

using namespace me;

class MyGame : public Game
{
public:
	void Startup()
	{
		using namespace scene;

		SceneManager * sceneManager = dynamic_cast< scene::SceneManager * >(GetComponent( "SceneManager", 0 ).get());

		// Load our effect from a file...
		GetManager< Effect >()->Add( "texture3d", "media/EffectTextured.effect" ); // Load an effect into the manager.

		Effect::ptr color3d = GetManager< Effect >()->Add( "color3d", "media/EffectColor.effect" );

		Effect::ptr effect = GetManager< Effect >()->Find( "texture3d" ); // Demonstrate how the effect is pulled from the manager by name.

		Scene::ptr mainScene = sceneManager->Add( "main" );

		Object::ptr camera = mainScene->GetRoot()->AddChild( "camera" );
		camera->AddComponent( IObjectComponent::ptr( new CameraComponent( GetOS() ) ) );
		CameraComponent * cameraComponent = unify::polymorphic_downcast< CameraComponent * >( camera->GetComponent( "Camera" ).get() );
																			 
		cameraComponent->SetProjection(
			unify::MatrixPerspectiveFovLH( 3.1415926535f / 4.0f,
				800 / 600
				, 1, 1000 ) );

		camera->GetFrame().SetPosition( unify::V3< float >( 0, 5, -17 ) );
		camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );

		IVertexDeclaration::ptr vd = effect->GetVertexShader()->GetVertexDeclaration();

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

		bool createBBs = true;

		if ( createBBs )
		{
			mainScene->AddComponent( ISceneComponent::ptr( new scene::AutoBBoxSceneComponent( GetOS(), color3d ) ) );
		}

		auto & cube = *group->AddChild( "cube" );
		sg::CubeParameters cubeParameters;
		cubeParameters.SetEffect( effect );
		cubeParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
		cubeParameters.SetDiffuseFaces( unify::Color::ColorRed(), unify::Color::ColorGreen(), unify::Color::ColorBlue(), unify::Color::ColorYellow(), unify::Color::ColorCyan(), unify::Color::ColorMagenta() );
		AddGeometryComponent( &cube, GetOS(), Geometry::ptr( sg::CreateShape( GetOS()->GetRenderer( 0 ), cubeParameters ) ) );
		cube.GetFrame().SetPosition( positions[shape++] );

		auto & pointField = *group->AddChild( "pointField" );
		sg::PointFieldParameters pointFieldParameters;
		pointFieldParameters.SetEffect( effect );
		pointFieldParameters.SetMinorRadius( 0.5f );
		pointFieldParameters.SetMajorRadius( 1.0f );
		pointFieldParameters.SetCount( 1000 );
		pointFieldParameters.SetDiffuse( unify::Color::ColorWhite() );
		AddGeometryComponent( &pointField, GetOS(), Geometry::ptr( sg::CreateShape( GetOS()->GetRenderer( 0 ), pointFieldParameters ) ) );
		pointField.GetFrame().SetPosition( positions[shape++] );

		auto & pointRing = *group->AddChild( "pointRing" );
		sg::PointRingParameters pointRingParameters;
		pointRingParameters.SetEffect( effect );
		pointRingParameters.SetMinorRadius( 0.25f );
		pointRingParameters.SetMajorRadius( 0.75f );
		pointRingParameters.SetCount( 1000 );
		pointRingParameters.SetDiffuse( unify::Color::ColorRed() );
		AddGeometryComponent( &pointRing, GetOS(), Geometry::ptr( sg::CreateShape( GetOS()->GetRenderer( 0 ), pointRingParameters ) ) );
		pointRing.GetFrame().SetPosition( positions[shape++] );

		auto & dashRing = *group->AddChild( "dashRing" );
		sg::DashRingParameters dashRingParameters;
		dashRingParameters.SetEffect( effect );
		dashRingParameters.SetMinorRadius( 0.5f );
		dashRingParameters.SetMajorRadius( 1.0f );
		dashRingParameters.SetSize( 0.5f );
		dashRingParameters.SetCount( 5 );
		dashRingParameters.SetDiffuse( unify::Color::ColorGreen() );
		AddGeometryComponent( &dashRing, GetOS(), Geometry::ptr( sg::CreateShape( GetOS()->GetRenderer( 0 ), dashRingParameters ) ) );
		dashRing.GetFrame().SetPosition( positions[shape++] );

		auto & pyramid = *group->AddChild( "pyramid" );
		sg::PyramidParameters pyramidParameters;
		pyramidParameters.SetEffect( effect );
		pyramidParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
		pyramidParameters.SetDiffuse( unify::Color::ColorYellow() );
		AddGeometryComponent( &pyramid, GetOS(), Geometry::ptr( sg::CreateShape( GetOS()->GetRenderer( 0 ), pyramidParameters ) ) );
		pyramid.GetFrame().SetPosition( positions[shape++] );

		auto & circle = *group->AddChild( "circle" );
		sg::CircleParameters circleParameters;
		circleParameters.SetEffect( effect );
		circleParameters.SetSegments( 24 );
		circleParameters.SetRadius( 1.0f );
		circleParameters.SetDiffuse( unify::Color::ColorBlue() );
		AddGeometryComponent( &circle, GetOS(), Geometry::ptr( sg::CreateShape( GetOS()->GetRenderer( 0 ), circleParameters ) ) );
		circle.GetFrame().SetPosition( positions[shape++] );

		auto & sphere = *group->AddChild( "sphere" );
		sg::SphereParameters sphereParameters;
		sphereParameters.SetEffect( effect );
		sphereParameters.SetSegments( 24 );
		sphereParameters.SetRadius( 1.0f );
		sphereParameters.SetDiffuse( unify::Color::ColorCyan() );
		AddGeometryComponent( &sphere, GetOS(), Geometry::ptr( sg::CreateShape( GetOS()->GetRenderer( 0 ), sphereParameters ) ) );
		sphere.GetFrame().SetPosition( positions[shape++] );

		auto & cylinder = *group->AddChild( "cylinder" );
		sg::CylinderParameters cylinderParameters;
		cylinderParameters.SetEffect( effect );
		cylinderParameters.SetSegments( 24 );
		cylinderParameters.SetRadius( 1.0f );
		cylinderParameters.SetHeight( 2.0f );
		cylinderParameters.SetDiffuse( unify::Color::ColorMagenta() );
		cylinderParameters.SetCaps( true );
		AddGeometryComponent( &cylinder, GetOS(), Geometry::ptr( sg::CreateShape( GetOS()->GetRenderer( 0 ), cylinderParameters ) ) );
		cylinder.GetFrame().SetPosition( positions[shape++] );

		auto & tube = *group->AddChild( "tube" );
		sg::TubeParameters tubeParameters;
		tubeParameters.SetEffect( effect );
		tubeParameters.SetSegments( 24 );
		tubeParameters.SetMajorRadius( 1.0f );
		tubeParameters.SetMinorRadius( 0.5f );
		tubeParameters.SetHeight( 2.0f );
		tubeParameters.SetDiffuse( unify::Color::ColorRed() );
		AddGeometryComponent( &tube, GetOS(), Geometry::ptr( sg::CreateShape( GetOS()->GetRenderer( 0 ), tubeParameters ) ) );
		tube.GetFrame().SetPosition( positions[shape++] );

		auto & plane = *group->AddChild( "plane" );
		sg::PlaneParameters planeParameters;
		planeParameters.SetEffect( effect );
		planeParameters.SetSegments( 2 );
		planeParameters.SetSize( unify::Size< float >( 2.0f, 2.0f ) );
		planeParameters.SetDiffuse( unify::Color::ColorCyan() );
		AddGeometryComponent( &plane, GetOS(), Geometry::ptr( sg::CreateShape( GetOS()->GetRenderer( 0 ), planeParameters ) ) );
		plane.GetFrame().SetPosition( positions[shape++] );

		auto & cone = *group->AddChild( "cone" );
		sg::ConeParameters coneParameters;
		coneParameters.SetEffect( effect );
		coneParameters.SetSegments( 24 );
		coneParameters.SetRadius( 1.0f );
		coneParameters.SetHeight( 2.0f );
		coneParameters.SetDiffuse( unify::Color::ColorGreen() );
		coneParameters.SetCaps( true );
		AddGeometryComponent( &cone, GetOS(), Geometry::ptr( sg::CreateShape( GetOS()->GetRenderer( 0 ), coneParameters ) ) );
		cone.GetFrame().SetPosition( positions[shape++] );
	}

	void Update( RenderInfo & renderInfo )
	{
		static size_t axisIndex = 1;
		static unify::Angle totalRotation{};
		unify::Angle rotation = unify::AngleInRadians( renderInfo.GetDelta() );
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

		scene::SceneManager * sceneManager = dynamic_cast< scene::SceneManager * >(GetComponent( "SceneManager", 0 ).get());

		auto group = sceneManager->Find( "main" )->FindObject( "group" );
		group->GetFrame().PreMul( unify::Quaternion( axis, rotation ) );
	}
} game;

RegisterGame( game );
