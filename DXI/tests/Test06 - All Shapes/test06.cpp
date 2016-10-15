// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

/// <summary>
/// "All Programmatic 3D Shapes"
/// goals and requirements:
/// * Example of the creation of the programmatic
/// </summary>

#include <dxi/core/Game.h>
#include <dxi/shapes/ShapeCreators.h>
#include <dxi/win/DXILib.h>
#include <DXIWinMain.h>

#include <dxi/scene/BBoxRendererComponent.h>

using namespace dxi;
using namespace core;

class MyGame : public Game
{
public:
	void Startup()
	{
		// Load our effect from a file...
		GetManager< Effect >()->Add( "texture3d", "media/EffectTextured.effect" ); // Load an effect into the manager.

		Effect::ptr color3d = GetManager< Effect >()->Add( "color3d", "media/EffectColor.effect" );

		Effect::ptr effect = GetManager< Effect >()->Find( "texture3d" ); // Demonstrate how the effect is pulled from the manager by name.

		scene::Scene::shared_ptr mainScene = GetSceneManager()->Add( "main" );

		scene::Object::ptr camera = mainScene->GetRoot()->AddChild( "camera" );
		camera->AddComponent( scene::IComponent::ptr( new scene::Camera ) );
		scene::Camera * cameraComponent = unify::polymorphic_downcast< scene::Camera * >( camera->GetComponent( "Camera" ).get() );
																			 
		cameraComponent->SetProjection(
			unify::Matrix::MatrixPerspectiveFovLH( 3.1415926535f / 4.0f,
				800 / 600
				, 1, 1000 ) );

		camera->GetFrame().SetPosition( unify::V3< float >( 0, 5, -17 ) );
		camera->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ) );

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

		bool createBBs = true;

		auto & cube = *group->AddChild( "cube" );
		shapes::CubeParameters cubeParameters;
		cubeParameters.SetEffect( effect );
		cubeParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
		cubeParameters.SetDiffuseFaces( unify::Color::ColorRed(), unify::Color::ColorGreen(), unify::Color::ColorBlue(), unify::Color::ColorYellow(), unify::Color::ColorCyan(), unify::Color::ColorMagenta() );
		cube.SetGeometry( Geometry::ptr( shapes::CreateShape( cubeParameters ) ) );
		cube.GetFrame().SetPosition( positions[shape++] );
		if ( createBBs )
			cube.AddComponent( scene::IComponent::ptr( new scene::BBoxRendererComponent( color3d ) ) );

		auto & pointField = *group->AddChild( "pointField" );
		shapes::PointFieldParameters pointFieldParameters;
		pointFieldParameters.SetEffect( effect );
		pointFieldParameters.SetMinorRadius( 0.5f );
		pointFieldParameters.SetMajorRadius( 1.0f );
		pointFieldParameters.SetCount( 1000 );
		pointFieldParameters.SetDiffuse( unify::Color::ColorWhite() );
		pointField.SetGeometry( Geometry::ptr( shapes::CreateShape( pointFieldParameters ) ) );
		pointField.GetFrame().SetPosition( positions[shape++] );
		if ( createBBs )
			pointField.AddComponent( scene::IComponent::ptr( new scene::BBoxRendererComponent( color3d ) ) );

		auto & pointRing = *group->AddChild( "pointRing" );
		shapes::PointRingParameters pointRingParameters;
		pointRingParameters.SetEffect( effect );
		pointRingParameters.SetMinorRadius( 0.25f );
		pointRingParameters.SetMajorRadius( 0.75f );
		pointRingParameters.SetCount( 1000 );
		pointRingParameters.SetDiffuse( unify::Color::ColorRed() );
		pointRing.SetGeometry( Geometry::ptr( shapes::CreateShape( pointRingParameters ) ) );
		pointRing.GetFrame().SetPosition( positions[shape++] );
		if ( createBBs )
			pointRing.AddComponent( scene::IComponent::ptr( new scene::BBoxRendererComponent( color3d ) ) );

		auto & dashRing = *group->AddChild( "dashRing" );
		shapes::DashRingParameters dashRingParameters;
		dashRingParameters.SetEffect( effect );
		dashRingParameters.SetMinorRadius( 0.5f );
		dashRingParameters.SetMajorRadius( 1.0f );
		dashRingParameters.SetSize( 0.5f );
		dashRingParameters.SetCount( 5 );
		dashRingParameters.SetDiffuse( unify::Color::ColorGreen() );
		dashRing.SetGeometry( Geometry::ptr( shapes::CreateShape( dashRingParameters ) ) );
		dashRing.GetFrame().SetPosition( positions[shape++] );
		if ( createBBs )
			dashRing.AddComponent( scene::IComponent::ptr( new scene::BBoxRendererComponent( color3d ) ) );

		auto & pyramid = *group->AddChild( "pyramid" );
		shapes::PyramidParameters pyramidParameters;
		pyramidParameters.SetEffect( effect );
		pyramidParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
		pyramidParameters.SetDiffuse( unify::Color::ColorYellow() );
		pyramid.SetGeometry( Geometry::ptr( shapes::CreateShape( pyramidParameters ) ) );
		pyramid.GetFrame().SetPosition( positions[shape++] );
		if ( createBBs )
			pyramid.AddComponent( scene::IComponent::ptr( new scene::BBoxRendererComponent( color3d ) ) );

		auto & circle = *group->AddChild( "circle" );
		shapes::CircleParameters circleParameters;
		circleParameters.SetEffect( effect );
		circleParameters.SetSegments( 24 );
		circleParameters.SetRadius( 1.0f );
		circleParameters.SetDiffuse( unify::Color::ColorBlue() );
		circle.SetGeometry( Geometry::ptr( shapes::CreateShape( circleParameters ) ) );
		circle.GetFrame().SetPosition( positions[shape++] );
		if ( createBBs )
			circle.AddComponent( scene::IComponent::ptr( new scene::BBoxRendererComponent( color3d ) ) );

		auto & sphere = *group->AddChild( "sphere" );
		shapes::SphereParameters sphereParameters;
		sphereParameters.SetEffect( effect );
		sphereParameters.SetSegments( 24 );
		sphereParameters.SetRadius( 1.0f );
		sphereParameters.SetDiffuse( unify::Color::ColorCyan() );
		sphere.SetGeometry( Geometry::ptr( shapes::CreateShape( sphereParameters ) ) );
		sphere.GetFrame().SetPosition( positions[shape++] );
		if ( createBBs )
			sphere.AddComponent( scene::IComponent::ptr( new scene::BBoxRendererComponent( color3d ) ) );

		auto & cylinder = *group->AddChild( "cylinder" );
		shapes::CylinderParameters cylinderParameters;
		cylinderParameters.SetEffect( effect );
		cylinderParameters.SetSegments( 24 );
		cylinderParameters.SetRadius( 1.0f );
		cylinderParameters.SetHeight( 2.0f );
		cylinderParameters.SetDiffuse( unify::Color::ColorMagenta() );
		cylinderParameters.SetCaps( true );
		cylinder.SetGeometry( Geometry::ptr( shapes::CreateShape( cylinderParameters ) ) );
		cylinder.GetFrame().SetPosition( positions[shape++] );
		if ( createBBs )
			cylinder.AddComponent( scene::IComponent::ptr( new scene::BBoxRendererComponent( color3d ) ) );

		auto & tube = *group->AddChild( "tube" );
		shapes::TubeParameters tubeParameters;
		tubeParameters.SetEffect( effect );
		tubeParameters.SetSegments( 24 );
		tubeParameters.SetMajorRadius( 1.0f );
		tubeParameters.SetMinorRadius( 0.5f );
		tubeParameters.SetHeight( 2.0f );
		tubeParameters.SetDiffuse( unify::Color::ColorRed() );
		tube.SetGeometry( Geometry::ptr( shapes::CreateShape( tubeParameters ) ) );
		tube.GetFrame().SetPosition( positions[shape++] );
		if ( createBBs )
			tube.AddComponent( scene::IComponent::ptr( new scene::BBoxRendererComponent( color3d ) ) );

		auto & plane = *group->AddChild( "plane" );
		shapes::PlaneParameters planeParameters;
		planeParameters.SetEffect( effect );
		planeParameters.SetSegments( 2 );
		planeParameters.SetSize( unify::Size< float >( 2.0f, 2.0f ) );
		planeParameters.SetDiffuse( unify::Color::ColorCyan() );
		plane.SetGeometry( Geometry::ptr( shapes::CreateShape( planeParameters ) ) );
		plane.GetFrame().SetPosition( positions[shape++] );
		if ( createBBs )
			plane.AddComponent( scene::IComponent::ptr( new scene::BBoxRendererComponent( color3d ) ) );

		auto & cone = *group->AddChild( "cone" );
		shapes::ConeParameters coneParameters;
		coneParameters.SetEffect( effect );
		coneParameters.SetSegments( 24 );
		coneParameters.SetRadius( 1.0f );
		coneParameters.SetHeight( 2.0f );
		coneParameters.SetDiffuse( unify::Color::ColorGreen() );
		coneParameters.SetCaps( true );
		cone.SetGeometry( Geometry::ptr( shapes::CreateShape( coneParameters ) ) );
		cone.GetFrame().SetPosition( positions[shape++] );
		if ( createBBs )
			cone.AddComponent( scene::IComponent::ptr( new scene::BBoxRendererComponent( color3d ) ) );

		//mainScene->GetRenderInfo().SetOption( RenderOption::NoFrame, true );
	}

	bool Update( RenderInfo & renderInfo )
	{
		// For testing rendering time:
		if ( renderInfo.FrameID() == 900 ) Quit();

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

		auto group = GetSceneManager()->Find( "main" )->FindObject( "group" );
		group->GetFrame().RotateAbout( axis, rotation );

		return Game::Update( renderInfo );
	}
} game;

RegisterGame( game );
