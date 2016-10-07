// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/geo/GeometryGroup.h>
#include <dxi/geo/Terra.h>
#include <dxi/pi/EnvironmentConstant.h>
#include <dxi/pi/EnvironmentBox.h>
#include <dxi/pi/PhysicsScene.h>
#include <DXIWinMain.h>

using namespace dxi;
using namespace core;

class MyGame : public Game
{
protected:
	scene::Scene m_scene;
	scene::Object::shared_ptr m_camera;
	float m_rotation;

public:

	void Startup()
	{
		Game::Startup();

		GetTextureManager()->Add( "borgcube", new Texture( "media/borgcube.bmp" ) );
		GetTextureManager()->Add( "earth", new Texture( "media/earthb.bmp", TEXTURE_LOCKABLE ) );
		GetTextureManager()->Add( "water_cutout", new Texture( "media/EarthWaterCutoutMap.bmp", TEXTURE_LOCKABLE ) );
	
		// Create the physics scene...
		pi::PhysicsScene * physicsScene = new pi::PhysicsScene;
		m_scene.SetPhysicsScene( boost::shared_ptr< physics::IScene >( physicsScene ) );
		Geometry::shared_ptr geometry;
        scene::Object::shared_ptr object;
		boost::shared_ptr< pi::PhysicsInstance > physics;

		// Scene 1...

        shapes::ShapeBaseParameters commonParameters;
		commonParameters.SetFVF( FVF::XYZ | FVF::Diffuse | FVF::Tex1 );
		commonParameters.SetTexture( GetTextureManager()->Find( "borgcube" ) );

        shapes::CubeParameters cubeParameters;
        cubeParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
		cubeParameters.SetDiffuseFaces( unify::Color::ColorRed(), unify::Color::ColorGreen(), unify::Color::ColorBlue(), unify::Color::ColorYellow(), unify::Color::ColorCyan(), unify::Color::ColorMagenta() );
		geometry = GetGeometryManager()->Add( "cube", new Mesh( commonParameters + cubeParameters ) );
		object.reset( new scene::Object( geometry, unify::V3< float >( -4.5f, 13, 0 ) ) );
		physics.reset( new pi::PhysicsInstance( object.get() ) ); 
		physics->SetMoveable( true );
		physics->SetPhysical( true );
		object->SetPhysics( physics );
		m_scene.Add( "cube", object );
		
        shapes::PointFieldParameters pointFieldParameters;
		pointFieldParameters.SetMinorRadius( -.5f );
		pointFieldParameters.SetMajorRadius( 1.0f );
		pointFieldParameters.SetCount( 1000 );
		geometry = GetGeometryManager()->Add( "pointfield", new Mesh( commonParameters + pointFieldParameters ) );
		object.reset( new scene::Object( geometry, unify::V3< float >( -1.5f, 13, 0 ) ) );
		physics.reset( new pi::PhysicsInstance( object.get() ) ); 
		physics->SetMoveable( true );
		physics->SetPhysical( true );		
		object->SetPhysics( physics );
		m_scene.Add( "pointfield", object );

        shapes::PointRingParameters pointRingParameters;
		pointRingParameters.SetMinorRadius( 0.25f );
		pointRingParameters.SetMajorRadius( 0.75f );
		pointRingParameters.SetCount( 1000 );
		geometry = GetGeometryManager()->Add( "pointring", new Mesh( commonParameters + pointRingParameters ) );
		object.reset( new scene::Object( geometry, unify::V3< float >( 1.5f, 13, 0 ) ) );
		physics.reset( new pi::PhysicsInstance( object.get() ) ); 
		physics->SetMoveable( true );
		physics->SetPhysical( true );
		object->SetPhysics( physics );
		m_scene.Add( "pointring", object );

        shapes::DashRingParameters dashRingParameters;
		dashRingParameters.SetMinorRadius( 1.25f );
		dashRingParameters.SetMajorRadius( 2 );
		dashRingParameters.SetCount( 5 );
		dashRingParameters.SetDiffuse( unify::Color::ColorGreen() );
		geometry = GetGeometryManager()->Add( "dashring", new Mesh( commonParameters + dashRingParameters ) );
		object.reset( new scene::Object( geometry, unify::V3< float >( 4.5f, 13, 0 ) ) );
		physics.reset( new pi::PhysicsInstance( object.get() ) ); 
		physics->SetMoveable( true );
		physics->SetPhysical( true );
		object->SetPhysics( physics );
		m_scene.Add( "dashring", object );

        shapes::PyramidParameters pyramidParameters;
        pyramidParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
		pyramidParameters.SetDiffuse( unify::Color::ColorYellow() );
		geometry = GetGeometryManager()->Add( "pyramid", new Mesh( commonParameters + pyramidParameters ) );
		object.reset( new scene::Object( geometry, unify::V3< float >( -4.5f, 10, 0 ) ) );
		physics.reset( new pi::PhysicsInstance( object.get() ) ); 
		physics->SetMoveable( true );
		physics->SetPhysical( true );
		object->SetPhysics( physics );
		m_scene.Add( "pyramid", object );

        shapes::CircleParameters circleParameters;
		circleParameters.SetSegments( 24 );
		circleParameters.SetRadius( 1 );
		circleParameters.SetDiffuse( unify::Color::ColorRed() );
		geometry = GetGeometryManager()->Add( "circle", new Mesh( commonParameters + circleParameters ) );
		object.reset( new scene::Object( geometry, unify::V3< float >( -1.5f, 10, 0 ) ) );
		physics.reset( new pi::PhysicsInstance( object.get() ) ); 
		physics->SetMoveable( true );
		physics->SetPhysical( true );
		object->SetPhysics( physics );
		m_scene.Add( "circle", object );

        shapes::SphereParameters sphereParameters;
		sphereParameters.SetSegments( 24 );
		sphereParameters.SetRadius( 1 );
		sphereParameters.SetDiffuse( unify::Color::ColorRed() );
		geometry = GetGeometryManager()->Add( "sphere", new Mesh( commonParameters + sphereParameters ) );
		object.reset( new scene::Object( geometry, unify::V3< float >( 1.5f, 10, 0 ) ) );
		physics.reset( new pi::PhysicsInstance( object.get() ) ); 
		physics->SetMoveable( true );
		physics->SetPhysical( true );
		object->SetPhysics( physics );
		m_scene.Add( "sphere", object );

        shapes::CylinderParameters cylinderParameters;
		cylinderParameters.SetSegments( 24 );
		cylinderParameters.SetRadius( 1 );
		cylinderParameters.SetHeight( 2 );
		cylinderParameters.SetDiffuse( unify::Color::ColorMagenta() );
		geometry = GetGeometryManager()->Add( "cylinder", new Mesh( commonParameters + cylinderParameters ) );
		object.reset( new scene::Object( geometry, unify::V3< float >( 4.5f, 10, 0 ) ) );
		physics.reset( new pi::PhysicsInstance( object.get() ) ); 
		physics->SetMoveable( true );
		physics->SetPhysical( true );
		object->SetPhysics( physics );
		m_scene.Add( "cylinder", object );

        shapes::TubeParameters tubeParameters;
		tubeParameters.SetSegments( 24 );
		tubeParameters.SetMajorRadius( 1 );
		tubeParameters.SetMinorRadius( 0.5f );
		tubeParameters.SetHeight( 2 );
		tubeParameters.SetDiffuse( unify::Color::ColorBlue() );
		geometry = GetGeometryManager()->Add( "tube", new Mesh( commonParameters + tubeParameters ) );
		object.reset( new scene::Object( geometry, unify::V3< float >( -4.5f, 7, 0 ) ) );
		physics.reset( new pi::PhysicsInstance( object.get() ) ); 
		physics->SetMoveable( true );
		physics->SetPhysical( true );
		object->SetPhysics( physics );
		m_scene.Add( "tube", object );

        shapes::PlaneParameters planeParameters;
		planeParameters.SetSegments( 2 );
        planeParameters.SetSize( unify::Size< float >( 2, 2 ) );
		planeParameters.SetDiffuse( unify::Color::ColorCyan() );
		geometry = GetGeometryManager()->Add( "plane", new Mesh( commonParameters + planeParameters ) );
		object.reset( new scene::Object( geometry, unify::V3< float >( -1.5f, 7, 0 ) ) );
		physics.reset( new pi::PhysicsInstance( object.get() ) ); 
		physics->SetMoveable( true );
		physics->SetPhysical( true );
		object->SetPhysics( physics );
		m_scene.Add( "plane", object );

        shapes::ConeParameters coneParameters;
		coneParameters.SetSegments( 24 );
		coneParameters.SetRadius( 1 );
		coneParameters.SetHeight( 2 );
		coneParameters.SetDiffuse( unify::Color::ColorRed() );
		geometry = GetGeometryManager()->Add( "cone", new Mesh( commonParameters + coneParameters ) );
		object.reset( new scene::Object( geometry, unify::V3< float >( 1.5f, 7, 0 ) ) );
		physics.reset( new pi::PhysicsInstance( object.get() ) ); 
		physics->SetMoveable( true );
		physics->SetPhysical( true );
		object->SetPhysics( physics );
		m_scene.Add( "cone", object );

		m_camera.reset( new scene::Object );
		m_scene.Add( "camera", m_camera );
		m_scene.SetCamera( "camera" );
		m_scene.SetSize( GetOS().GetResolution() );

		// Ground...

		GeometryGroup * geometryGroup = new GeometryGroup();
		Terra * terra;

        Terra::Parameters terraParameters( unify::Size< float >( 20, 14 ), FVF::XYZ | FVF::Tex1, unify::RowColumn< unsigned int >( 200, 140 ), 0, unify::TexArea::Full() );
		terra = new Terra( terraParameters );
		dxi::ColorOp depthOp;
		depthOp.r = 0.5f;
		depthOp.g = 0.25f;
		depthOp.b = -1.0f;
		terra->ApplyHeightMap( GetTextureManager()->Find( "earth" ), depthOp );
		terra->GetPrimitiveList().GetEffect( 0 ).SetTexture( 0, GetTextureManager()->Find( "earth" ) );
		terra->GetPrimitiveList().GetEffect( 0 ).SetBlend( Blend( Usage::False ) );
		geometryGroup->Add( Geometry::shared_ptr( terra ) );

		terraParameters = Terra::Parameters( unify::Size< float >( 20, 14 ), FVF::XYZ | FVF::Tex1, unify::RowColumn< unsigned int >( 10, 7 ), 0.1f, unify::TexArea::Full() );
		terra = new Terra( terraParameters );
		dxi::ColorOp noDrawOp;
		noDrawOp.r = 1.0f;
		terra->ApplyAlphaMap( GetTextureManager()->Find( "cutout" ), noDrawOp );
		terra->GetPrimitiveList().GetEffect( 0 ).SetTexture( 0, GetTextureManager()->Find( "earth" ) );
		terra->GetPrimitiveList().GetEffect( 0 ).SetBlend( Blend( Usage::True, Blend::Effect::One, Blend::Effect::One ) );
		geometryGroup->Add( Geometry::shared_ptr( terra ) );

		object.reset( new scene::Object( Geometry::shared_ptr( geometryGroup ) ) );
		physics.reset( new pi::PhysicsInstance( object.get() ) ); 
		physics->SetMoveable( false );
		physics->SetPhysical( true );

		m_scene.Add( "terrain", object );

		// Environments
		//physicsScene.AddEnvironment( pi::PhysicsEnvironment::shared_ptr( new pi::EnvironmentConstant( unify::V3< float >( 0, -0.1f, 0 ) ) ) );

		physicsScene->AddEnvironment( pi::PhysicsEnvironment::shared_ptr( new pi::EnvironmentBox( 
			unify::Frame().SetPosition( unify::V3< float >( 0, -15, 0 ) ), 
			unify::BBox< float >( unify::V3< float >( -10, -15, -10 ), unify::V3< float >( 10, 15, 10 ) ),
			unify::V3< float >( 0, 0.3f, 0 ) 
			) ) );

		physicsScene->AddEnvironment( pi::PhysicsEnvironment::shared_ptr( new pi::EnvironmentBox( 
			unify::Frame().MoveBy( unify::V3< float >( 0, 15, 0 ) ), 
			unify::BBox< float >( unify::V3< float >( -10, -15, -10 ), unify::V3< float >( 10, 15, 10 ) ),
			unify::V3< float >( 0, -0.3f, 0 ) 
			) ) );

		m_rotation = 0;
	}

	bool Update( RenderInfo & renderInfo, IInput & input )
	{
		unify::V3< float > eye( 30.0f * sin( m_rotation ), 30 + 10.0f * sin( m_rotation ), 30.0f * cos( m_rotation ) );
		m_camera->GetFrame().SetPosition( eye );
		m_camera->GetFrame().LookAt( unify::V3< float >(0, 0, 0) );

		m_rotation += 0.01f;
		if ( m_rotation > 3.1415936535f * 2 )
		{
			m_rotation -= 3.1415936545f * 2;
		}

		m_scene.Update( elapsed, input );

		return Game::Update( renderInfo, input );
	}

	void Render()
	{
		m_scene.Render();
	}

	void Shutdown()
	{
		m_scene.Clear();
		
		Game::Shutdown();
	}
} game;

RegisterGame( game );
