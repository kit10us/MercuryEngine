// Copyright (c) 2002 - 2011, Evil Quail LLC
// All Rights Reserved

#include <dxi/core/Win32Game.h>
#include <dxi/Mesh.h>
#include <dxi/scene/Scene.h>
#include <dxi/TextureManager.h>
#include <dxi/GeometryManager.h>
#include <dxi/GeometryGroup.h>
#include <dxi/Terra.h>
#include <bi/Scene.h>
#include <boost/cast.hpp>

using namespace dxi;
using namespace core;

class MyGame : public Win32Game
{
protected:
	TextureManager::shared_ptr m_textureManager;
	GeometryManager::shared_ptr m_geometryManager;
	scene::Scene m_scene1;
	boost::shared_ptr< bi::Scene > m_physicsScene;
	scene::Object::shared_ptr m_camera1;
	float m_rotation;
	scene::Object::shared_ptr m_control;

public:

	void Startup()
	{
		Win32Game::Startup();

		// Create managers...
		m_textureManager.reset( new TextureManager() );
		m_geometryManager.reset( new GeometryManager( m_textureManager ) );

		m_textureManager->Add( "borgcube", new Texture( "media/borgcube.bmp" ) );
		m_textureManager->Add( "earth", new Texture( "media/earthb.bmp", TEXTURE_LOCKABLE ) );
		m_textureManager->Add( "water_cutout", new Texture( "media/EarthWaterCutoutMap.bmp", TEXTURE_LOCKABLE ) );
	
		m_physicsScene.reset( new bi::Scene );
		m_scene1.SetPhysicsScene( m_physicsScene );
		m_scene1.SetRenderPhysics( true );

		Geometry::shared_ptr geometry;
		scene::Object::shared_ptr object;

		// Scene 1...
		{
			// Graphics...
            shapes::CubeParameters cubeParameters;
			cubeParameters.SetFVF( FVF::XYZ | FVF::Diffuse | FVF::Tex1 );
			cubeParameters.SetTexture( m_textureManager->Find( "borgcube" ) );
            cubeParameters.SetSize( unify::Size3< float >( 2, 2, 2 ) );
			cubeParameters.SetDiffuseFaces( unify::Color::ColorRed(), unify::Color::ColorGreen(), unify::Color::ColorBlue(), unify::Color::ColorYellow(), unify::Color::ColorCyan(), unify::Color::ColorMagenta() );
			geometry = m_geometryManager->Add( "cube",  new Mesh( cubeParameters ) );
			object.reset( new scene::Object( geometry, unify::V3< float >( -4.5f, 13, 0 ) ) );
			
			// Physics...
			boost::shared_ptr< btCollisionShape > collisionShape( new btBoxShape( btVector3( 1, 1, 1 ) ) );
			bi::Instance * physics( new bi::Instance( object.get(), collisionShape ) );
			object->SetPhysics( boost::shared_ptr< physics::IInstance >( physics ) );

			m_scene1.Add( "cube", object ); // Physics is added automatically.
			m_control = object;
		}
		
		{
            shapes::PointFieldParameters pointFieldParameters;
			pointFieldParameters.SetMinorRadius( 0.5f );
			pointFieldParameters.SetMajorRadius( 1.0f );
			pointFieldParameters.SetCount( 1000 );
			geometry = m_geometryManager->Add( "pointfield", new Mesh( pointFieldParameters ) );
			object.reset( new scene::Object( geometry, unify::V3< float >( -1.5f, 13, 0 ) ) );

			// Physics...
			boost::shared_ptr< btCollisionShape > collisionShape( new btSphereShape( 1 ) );
			bi::Instance * physics( new bi::Instance( object.get(), collisionShape ) );
			object->SetPhysics( boost::shared_ptr< physics::IInstance >( physics ) );
			m_scene1.Add( "pointfield", object );
		}
		{
            shapes::PointRingParameters pointRingParameters;
			pointRingParameters.SetMajorRadius( 1.0f );
			pointRingParameters.SetMinorRadius( 0.5f );
			pointRingParameters.SetCount( 1000 );
			geometry = m_geometryManager->Add( "pointring", new Mesh( pointRingParameters ) );
			object.reset( new scene::Object( geometry, unify::V3< float >( 0.5f, 1, 1 ) ) );

			// Physics...
			boost::shared_ptr< btCollisionShape > collisionShape( new btCylinderShape( btVector3( 1, 1, 2 ) ) );
			bi::Instance * physics( new bi::Instance( object.get(), collisionShape ) );
			object->SetPhysics( boost::shared_ptr< physics::IInstance >( physics ) );
			m_scene1.Add( "pointring", object );
		}

		{
            shapes::DashRingParameters dashRingParameters;
			dashRingParameters.SetFVF( FVF::XYZ | FVF::Diffuse | FVF::Tex1 );
			dashRingParameters.SetTexture( m_textureManager->Find( "borgcube" ) );
			dashRingParameters.SetMajorRadius( 2 );
			dashRingParameters.SetMinorRadius( 1.25f );
			dashRingParameters.SetCount( 5 );
			dashRingParameters.SetDiffuse( unify::Color::ColorGreen() );
			geometry = m_geometryManager->Add( "dashring", new Mesh( dashRingParameters ) );
			object.reset( new scene::Object( geometry, unify::V3< float >( 4.5f, 13, 0 ) ) );

			// Physics...
			boost::shared_ptr< btCollisionShape > collisionShape( new btCylinderShape( btVector3( 1, 0.1f, 1 ) ) );
			bi::Instance * physics( new bi::Instance( object.get(), collisionShape ) );
			object->SetPhysics( boost::shared_ptr< physics::IInstance >( physics ) );

			m_scene1.Add( "dashring", object );
		}
		/*
		{
			args.Reset();
			args.SetFVF( FVF::XYZ | FVF::Diffuse | FVF::Tex1 );
			args.SetTexture( m_textureManager->Find( "borgcube" ) );
			args.SetWidth( 2 );
			args.SetHeight( 2 );
			args.SetDepth( 2 );
			args.SetDiffuse( 0, unify::Color::ColorYellow() );
			geometry = m_geometryManager->Add( "pyramid", new Mesh( Shape::Pyramid, args ) );
			object.reset( new object::Object( geometry, unify::V3< float >( -4.5f, 10, 0 ) ) );

			// Physics...
			boost::shared_ptr< btCollisionShape > collisionShape( new btBoxShape( btVector3( 2, 0.1f, 2 ) ) );
			
			btDefaultMotionState * fallMotionState(
					new btDefaultMotionState( 
							btTransform( btQuaternion( 0, 0, 0, 1 ), btVector3( 0, 50, 0 ) ) 
						) );
			btScalar mass = 1;
			btVector3 fallInertia( 0, 0, 0 );
			collisionShape->calculateLocalInertia( mass, fallInertia );
			btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI( mass, fallMotionState, collisionShape, fallInertia );

			bi::Instance * physics( new bi::Instance( object.get(), collisionShape ) );
			physics->SetShape( boost::shared_ptr< btCollisionShape >( collisionShape ) );
			physics->SetMotionState( boost::shared_ptr< btDefaultMotionState > ( fallMotionState ) );
			physics->SetRigidBody( boost::shared_ptr< btRigidBody >( new btRigidBody( fallRigidBodyCI ) ) );
			object->SetPhysics( boost::shared_ptr< physics::IInstance >( physics ) );

			m_scene1.Add( "pyramid", object );
		}
		*/
		{
            shapes::CircleParameters circleParameters;
			circleParameters.SetFVF( FVF::XYZ | FVF::Diffuse | FVF::Tex1 );
			circleParameters.SetTexture( m_textureManager->Find( "borgcube" ) );
			circleParameters.SetSegments( 24 );
			circleParameters.SetRadius( 1.0f );
			circleParameters.SetDiffuse( unify::Color::ColorRed() );
			geometry = m_geometryManager->Add( "circle", new Mesh( circleParameters ) );
			object.reset( new scene::Object( geometry, unify::V3< float >( -1.5f, 10, 0 ) ) );
			
			// Physics...
			boost::shared_ptr< btCollisionShape > collisionShape( new btBoxShape( btVector3( 0.5f, 0.1f, 0.5f ) ) );
			bi::Instance * physics( new bi::Instance( object.get(), collisionShape ) );
			object->SetPhysics( boost::shared_ptr< physics::IInstance >( physics ) );
			
			m_scene1.Add( "circle", object );
		}

		{
            shapes::SphereParameters sphereParameters;
			sphereParameters.SetFVF( FVF::XYZ | FVF::Diffuse | FVF::Tex1 );
			sphereParameters.SetTexture( m_textureManager->Find( "borgcube" ) );
			sphereParameters.SetSegments( 24 );
			sphereParameters.SetRadius( 1 );
			sphereParameters.SetDiffuse( unify::Color::ColorRed() );
			geometry = m_geometryManager->Add( "sphere", new Mesh( sphereParameters ) );
			object.reset( new scene::Object( geometry, unify::V3< float >( 1.5f, 10, 0 ) ) );
		
			// Physics...
			boost::shared_ptr< btCollisionShape > collisionShape( new btSphereShape( 1 ) );
			bi::Instance * physics( new bi::Instance( object.get(), collisionShape ) );
			object->SetPhysics( boost::shared_ptr< physics::IInstance >( physics ) );

			m_scene1.Add( "sphere", object );
		}

		{
            shapes::CylinderParameters cylinderParameters;
			cylinderParameters.SetFVF( FVF::XYZ | FVF::Diffuse | FVF::Tex1 );
			cylinderParameters.SetTexture( m_textureManager->Find( "borgcube" ) );
			cylinderParameters.SetSegments( 24 );
			cylinderParameters.SetRadius( 1 );
			cylinderParameters.SetHeight( 2 );
			cylinderParameters.SetDiffuse( unify::Color::ColorMagenta() );
			geometry = m_geometryManager->Add( "cylinder", new Mesh( cylinderParameters ) );
			object.reset( new scene::Object( geometry, unify::V3< float >( 4.5f, 10, 0 ) ) );

			// Physics...
			boost::shared_ptr< btCollisionShape > collisionShape( new btCylinderShape( btVector3( 0.5f, 1, 0.5f ) ) );
			bi::Instance * physics( new bi::Instance( object.get(), collisionShape ) );
			object->SetPhysics( boost::shared_ptr< physics::IInstance >( physics ) );

			m_scene1.Add( "cylinder", object );
		}

		{
            shapes::TubeParameters tubeParameters;
			tubeParameters.SetFVF( FVF::XYZ | FVF::Diffuse | FVF::Tex1 );
			tubeParameters.SetTexture( m_textureManager->Find( "borgcube" ) );
			tubeParameters.SetSegments( 24 );
			tubeParameters.SetMajorRadius( 1.0f );
			tubeParameters.SetMinorRadius( 0.5f );
			tubeParameters.SetHeight( 2 );
			tubeParameters.SetDiffuse( unify::Color::ColorBlue() );
			geometry = m_geometryManager->Add( "tube", new Mesh( tubeParameters ) );
			object.reset( new scene::Object( geometry, unify::V3< float >( -4.5f, 7, 0 ) ) );
		
			// Physics...
			boost::shared_ptr< btCollisionShape > collisionShape( new btCylinderShape( btVector3( 0.5f, 1, 0.5f ) ) );
			bi::Instance * physics( new bi::Instance( object.get(), collisionShape ) );
			object->SetPhysics( boost::shared_ptr< physics::IInstance >( physics ) );

			m_scene1.Add( "tube", object );
		}

		{
            shapes::PlaneParameters planeParameters;
			planeParameters.SetFVF( FVF::XYZ | FVF::Diffuse | FVF::Tex1 );
			planeParameters.SetTexture( m_textureManager->Find( "borgcube" ) );
			planeParameters.SetSegments( 2 );
            planeParameters.SetSize( unify::Size< float >( 2.0f, 2.0f ) );
			planeParameters.SetDiffuse( unify::Color::ColorCyan() );
			geometry = m_geometryManager->Add( "plane", new Mesh( planeParameters ) );
			object.reset( new scene::Object( geometry, unify::V3< float >( -1.5f, 7, 0 ) ) );

			// Physics...
			boost::shared_ptr< btCollisionShape > collisionShape( new btBoxShape( btVector3( 1, 0.1f, 1 ) ) );
			bi::Instance * physics( new bi::Instance( object.get(), collisionShape ) );
			object->SetPhysics( boost::shared_ptr< physics::IInstance >( physics ) );

			m_scene1.Add( "plane", object );
		}

		{
            shapes::ConeParameters coneParameters;
			coneParameters.SetFVF( FVF::XYZ | FVF::Diffuse | FVF::Tex1 );
			coneParameters.SetTexture( m_textureManager->Find( "borgcube" ) );
			coneParameters.SetSegments( 24 );
			coneParameters.SetRadius( 1 );
			coneParameters.SetHeight( 2 );
			coneParameters.SetDiffuse( unify::Color::ColorRed() );
			geometry = m_geometryManager->Add( "cone", new Mesh( coneParameters ) );
			object.reset( new scene::Object( geometry, unify::V3< float >( 1.5f, 7, 0 ) ) );
		
			// Physics...
			boost::shared_ptr< btCollisionShape > collisionShape( new btConeShape( 1, 2 ) );
			bi::Instance * physics( new bi::Instance( object.get(), collisionShape ) );
			object->SetPhysics( boost::shared_ptr< physics::IInstance >( physics ) );

			m_scene1.Add( "cone", object );
		}
		{
			GeometryGroup * geometryGroup = new GeometryGroup();
			Terra * terra;

            Terra::Parameters terraParameters;
            terraParameters.SetSize( unify::Size< float >( 20, 14 ) );
            terraParameters.SetFVF( FVF::XYZ | FVF::Tex1 );
            terraParameters.SetRowsColumns( unify::RowColumn< unsigned int >( 200, 140 ) );
            terraParameters.SetConstant( 0 );
            terraParameters.SetTexArea( unify::TexArea::Full() );
			terra = new Terra( terraParameters );
			dxi::ColorOp depthOp;
			depthOp.r = 0.5f;
			depthOp.g = 0.25f;
			depthOp.b = -1.0f;
			terra->ApplyHeightMap( m_textureManager->Find( "earth" ), depthOp );
			terra->GetPrimitiveList().GetEffect( 0 ).SetTexture( 0, m_textureManager->Find( "earth" ) );
			terra->GetPrimitiveList().GetEffect( 0 ).SetBlend( Blend( Usage::False ) );
			geometryGroup->Add( Geometry::shared_ptr( terra ) );

            terraParameters.SetSize( unify::Size< float >( 20, 14 ) );
            terraParameters.SetFVF( FVF::XYZ | FVF::Tex1 );
            terraParameters.SetRowsColumns( unify::RowColumn< unsigned int >( 10, 7 ) );
            terraParameters.SetConstant( 0.1f );
            terraParameters.SetTexArea( unify::TexArea::Full() );
			terra = new Terra( terraParameters );
			dxi::ColorOp noDrawOp;
			noDrawOp.r = 1.0f;
			terra->ApplyAlphaMap( m_textureManager->Find( "water_cutout" ), noDrawOp );
			terra->GetPrimitiveList().GetEffect( 0 ).SetTexture( 0, m_textureManager->Find( "earth" ) );
			terra->GetPrimitiveList().GetEffect( 0 ).SetBlend( Blend( Usage::True, Blend::Effect::One, Blend::Effect::One ) );
			geometryGroup->Add( Geometry::shared_ptr( terra ) );
			object.reset( new scene::Object( Geometry::shared_ptr( geometryGroup ) ) );
			m_scene1.Add( "terrain", object );
		}


		m_camera1.reset( new scene::Object() );
		m_scene1.Add( "camera1", m_camera1 );
		m_scene1.SetCamera( "camera1" );
		m_scene1.SetSize( GetResolution() );

		m_rotation = 0;
	}

	bool Update( unify::Seconds elapsed, IInput & input )
	{
		if( m_camera1 )
		{
			unify::V3< float > eye( 30.0f * sin( m_rotation ), 30 + 10.0f * sin( m_rotation ), 30.0f * cos( m_rotation ) );
			m_camera1->GetFrame().SetPosition( eye );
			m_camera1->GetFrame().LookAt( unify::V3< float >(0, 0, 0) );
		}

		m_rotation += elapsed;
		if ( m_rotation > 3.1415936535f * 2 )
		{
			m_rotation -= 3.1415936545f * 2;
		}

		m_scene1.Update( elapsed, input );

		if( m_control )
		{
			bi::Instance * physics = boost::polymorphic_downcast< bi::Instance * >( m_control->GetPhysics().get() );
			btRigidBody * body = physics->GetRigidBody();
			btTransform transform;
			if( input.KeyDown( Key::Space ) )
			{
				body->clearForces();
   				body->setLinearVelocity( btVector3( 0, 10, 0 ) );
			}
			else if( input.KeyDown( Key::Left ) )
			{
				body->clearForces();
   				body->setLinearVelocity( btVector3( 5, 10, 0 ) );
			}
			else if( input.KeyDown( Key::Right ) )
			{
				body->clearForces();
   				body->setLinearVelocity( btVector3( -5, 10, 0 ) );
			}
			else if( input.KeyDown( Key::Up ) )
			{
				body->clearForces();
   				body->setLinearVelocity( btVector3( 0, 10, -5 ) );
			}
			else if( input.KeyDown( Key::Down ) )
			{
				body->clearForces();
   				body->setLinearVelocity( btVector3( 0, 10, 5 ) );
			}
			//body->setCenterOfMassTransform( transform );
		}
		return Win32Game::Update( elapsed, input );
	}

	void Render()
	{
		m_scene1.Render();
	}

	void Shutdown()
	{
		m_scene1.Clear();
		m_geometryManager.reset();
		m_textureManager.reset();
		Win32Game::Shutdown();
	}
} game;
