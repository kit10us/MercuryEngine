// Copyright (c) 2002 - 2011, Evil Quail LLC
// All Rights Reserved

#include <dxi/core/Win32Game.h>
#include <dxi/scene/SceneManager.h>
#include <dxi/Input.h>
#include <dxi/controllers/IController.h>
#include <dxi/XMLConvert.h>

#include <bi/SceneIntegration.h>

#include <boost/cast.hpp>

using namespace dxi;
using namespace core;


class ClosestNotMeRayResultCallback : public btCollisionWorld::ClosestRayResultCallback
{
	const btRigidBody * m_me;
public:
	ClosestNotMeRayResultCallback( const btVector3 & rayFromWorld, const btVector3 & rayToWorld, const btRigidBody * me )
		: ClosestRayResultCallback( rayFromWorld, rayToWorld )
		, m_me( me )
	{
	}

	bool needsCollision( btBroadphaseProxy * proxy0 ) const
	{
		if( proxy0->m_clientObject == m_me )
		{
			return false;
		}
		return ClosestRayResultCallback::needsCollision( proxy0 );
	}
};

class ClosestNotUsRayResultCallback : public btCollisionWorld::ClosestRayResultCallback
{
	std::vector< const btRigidBody * > m_us;
public:
	ClosestNotUsRayResultCallback( const btVector3 &	rayFromWorld, const btVector3 & rayToWorld, const std::vector< const btRigidBody * > & us )
		: ClosestRayResultCallback( rayFromWorld, rayToWorld )
		, m_us( us )
	{
	}

	bool needsCollision( btBroadphaseProxy * proxy0 ) const
	{
		for( std::vector< const btRigidBody * >::const_iterator itr = m_us.begin(), end = m_us.end(); itr != end; ++itr )
		{
			if( proxy0->m_clientObject == *itr )
			{
				return false;
			}
		}
		return ClosestRayResultCallback::needsCollision( proxy0 );
	}
};


namespace dxi
{
	namespace controllers
	{
		class Walker : public IController
		{
		public:
			class Producer : public controllers::ControllerFactory::Producer
			{
			public:
				IController::shared_ptr Produce( scene::Object * object, const qxml::Element * node, scene::Scene * scene ) const
				{
					dxi::core::Win32Game & game = *dxi::core::Win32Game::GetGameInstance();
					Walker * walker = new Walker( bi::GetRigidBody( object ), bi::GetWorld( scene ) );
					return IController::shared_ptr( walker );
				}
			};

			class State
			{
			public:
				typedef enum TYPE {
					Idle,
					Falling
				};
			};

			Walker( btRigidBody * body, btDiscreteDynamicsWorld * world )
				: m_body( body )
				, m_world( world )
				, m_state( State::Falling )
				, m_turnAngle( 0 )
				, m_forwardSpeedCap( 10.0f )
				, m_backwardSpeedCap( 5.0f )
			{
			}

			void Update( unify::Seconds elapsed, IInput & input )
			{
				if( ! m_body ) return;

				btTransform transform = m_body->getWorldTransform();

				switch( m_state )
				{
				case State::Falling:
					{
						btVector3 rayFromWorld = m_body->getWorldTransform().getOrigin();
						btVector3 rayToWorld = rayFromWorld + btVector3( 0, -2, 0 );
						ClosestNotMeRayResultCallback result( rayFromWorld, rayToWorld, m_body );
						m_world->rayTest( rayFromWorld, rayToWorld, result );

						bool hasHit = result.hasHit();
						float marginal = 0.0000001f;
						if( result.hasHit() && m_body->getLinearVelocity().y() < marginal )
						{
							//float hitDistance = rayFromWorld.distance( result.m_hitPointWorld );
							m_state = State::Idle;
						}
					}
					break;
			
				case State::Idle:
					{
						btVector3 forward = transform.getBasis().inverse()[ 2 ];
						btVector3 linearVelocity = m_body->getLinearVelocity();
						float jumpMagnitude = 13.0f;
						float walkForwardSpeed = 10.0f;
						float walkBackwardSpeed = 7.5f;

						// Jump...
						if( input.KeyDown( Key::Space ) )
						{
							m_body->activate( true );
							m_body->setAngularVelocity( btVector3( 0, 0, 0 ) );
							m_body->applyCentralImpulse( btVector3( 0, jumpMagnitude, 0 ) );
							m_state = State::Falling;
						}
						else
						{
							// Forward and backward...
							if( input.KeyDown( Key::Up ) )
							{
								linearVelocity += forward * walkForwardSpeed * elapsed;
								btScalar speed = linearVelocity.length();
								if( speed > m_forwardSpeedCap )
								{
									float d = m_forwardSpeedCap / speed;
									linearVelocity *= d;
								}
							}
							else if( input.KeyDown( Key::Down ) )
							{
								linearVelocity += -forward * walkBackwardSpeed * elapsed;
								btScalar speed = linearVelocity.length();
								if( speed > m_backwardSpeedCap )
								{
									float d = m_backwardSpeedCap / speed;
									linearVelocity *= d;
								}
							}
							else
							{
								linearVelocity *= elapsed;
							}


							// Turning...
							if( input.KeyDown( Key::Left ) )
							{
								m_turnAngle -= 3.1415926f * elapsed;
							}
							else if( input.KeyDown( Key::Right ) )
							{
								m_turnAngle += 3.1415926f * elapsed;
							}
							transform.setRotation( btQuaternion( btVector3( 0.0, 1.0, 0.0 ), m_turnAngle ) );
							m_body->activate( true );
							m_body->setLinearVelocity( linearVelocity );
							m_body->setAngularVelocity( btVector3( 0, 0, 0 ) ); // This fixed the problem where the matrix seemed off/wrong.
							m_body->setWorldTransform( transform );
						}
					}
					break;
				}
			}

		private:
			btRigidBody * m_body;
			btDiscreteDynamicsWorld * m_world;
			State::TYPE m_state;
			float m_turnAngle;
			float m_forwardSpeedCap;
			float m_backwardSpeedCap;
		};	
			
		class CameraFollow : public IController
		{
		public:
			class Producer : public ControllerFactory::Producer
			{
			public:
				IController::shared_ptr Produce( scene::Object * object, const qxml::Element * node, scene::Scene * scene ) const
				{
					CameraFollow * controller = new CameraFollow( object );
					std::string followName = node->GetStringAttribute( "follow" );
					scene::Object::shared_ptr follow( scene->FindObject( followName ) );
					if( ! object )
					{
						throw std::exception( "CameraFollow controller requires a valid follow object!" );
					}

					unify::Quaternion orbit;
					if( ! XMLConvert( node->GetElement( "orbit" ), orbit ) )
					{
						throw std::exception( "CameraFollow controller requires a valid orbit axis!" );
					}
					float distance = node->GetFloatAttribute( "distance" );

					btDiscreteDynamicsWorld * world = bi::GetWorld( scene );

					controller->SetFollow( follow );
					controller->SetDistance( distance );
					controller->SetOrbit( orbit );
					controller->SetWorld( world );
					return IController::shared_ptr( controller );
				}
			};

			CameraFollow( scene::Object * object )
				: m_object( object )
				, m_world( 0 )
				, m_distance( 1.0f )
			{
			}

			void SetWorld( btDiscreteDynamicsWorld * world )
			{
				m_world = world;
			}

			void SetFollow( scene::Object::shared_ptr follow )
			{
				m_follow = follow;
			}

			void SetDistance( float distance )
			{
				m_distance = distance;
			}

			void SetOrbit( unify::Quaternion & orbit )
			{
				m_orbit = orbit;
			}

			void Update( unify::Seconds elapsed, IInput & input )
			{
				elapsed;
				if( m_follow.expired() )
				{
					return;
				}

				scene::Object * object = m_object;
				const scene::Object * follow = m_follow.lock().get();

				unify::V3< float > followPosition( follow->GetFrame().GetPosition() );
				unify::Quaternion orientation( follow->GetFrame().GetRotation() );

				object->GetFrame().LookAt( followPosition );
				object->GetFrame().Orbit( followPosition, orientation.Conjugate() * m_orbit, m_distance );

				// Fix camera for collisions...
				if( m_world )
				{
					btVector3 rayFromWorld = bi::convert( object->GetFrame().GetPosition() );
					btVector3 rayToWorld = bi::convert( followPosition );
					std::vector< const btRigidBody * > us;
					us.push_back( bi::GetRigidBody( follow ) );
					ClosestNotUsRayResultCallback result( rayFromWorld, rayToWorld, us );
					m_world->rayTest( rayFromWorld, rayToWorld, result );

					bool hasHit = result.hasHit();
					if( result.hasHit() )
					{
						btVector3 center;
						btScalar radius;
						result.m_collisionObject->getCollisionShape()->getBoundingSphere( center, radius );
						float distance = rayToWorld.distance( result.m_hitPointWorld );
						btVector3 dif( rayToWorld - result.m_hitPointWorld );
						btVector3 newPosition( rayToWorld - dif * ((distance - radius) / distance) );
						object->GetFrame().SetPosition( bi::convert( newPosition ) );
					}
				}

				if( object->GetPhysics() )
				{
					object->GetPhysics()->SyncPhysicsFromObject();
				}
			}
		private:
			scene::Object * m_object;
			btDiscreteDynamicsWorld * m_world;
			scene::Object::weak_ptr m_follow;
			float m_distance;
			unify::Quaternion m_orbit;
		};

		class ObjectPath : public IController
		{
		public:
			class Producer : public ControllerFactory::Producer
			{
			public:
				IController::shared_ptr Produce( scene::Object * object, const qxml::Element * node, scene::Scene * scene ) const
				{
					std::string pathName = node->GetStringAttribute( "path" );
					bool looping = node->GetBooleanAttributeElse( "looping", false );
					pathing::PathController::shared_ptr pathController( scene->GetPathSystem().CreatePathController( pathName, looping ) );
					pathController->SetFrame( &object->GetFrame() );
					ObjectPath * controller = new ObjectPath( object, pathController );
					return IController::shared_ptr( controller );
				}
			};

			ObjectPath( scene::Object * object, pathing::PathController::shared_ptr pathController )
				: m_object( object )
				, m_pathController( pathController )
			{
			}

			void Update( unify::Seconds elapsed, IInput & input )
			{
				pathing::PathController * pathController = m_pathController.get();
				pathController->Update( elapsed );
			}
		private:
			scene::Object * m_object;
			pathing::PathController::shared_ptr m_pathController;
		};
		}
}

class AddCustomControllersLF : public loader::SceneLoader::LoaderFunctor
{
public:
	void operator()( const qxml::Element * element, scene::Scene::shared_ptr scene )
	{
		controllers::ControllerFactory * factory = scene.get()->GetControllerFactory();
		factory->AddProducer( "walker", controllers::ControllerFactory::Producer::shared_ptr( new controllers::Walker::Producer ) );
		factory->AddProducer( "camerafollow", controllers::ControllerFactory::Producer::shared_ptr( new controllers::CameraFollow::Producer ) );
		factory->AddProducer( "path", controllers::ControllerFactory::Producer::shared_ptr( new controllers::ObjectPath::Producer ) );
	}
};



class MyGame : public Win32Game
{
protected:
	boost::shared_ptr< scene::SceneManager > m_sceneManager;
public:
	void Startup();
	bool Update( unify::Seconds elapsed, IInput & input );
	void Render();
	void Shutdown();
} game;

void MyGame::Startup()
{
	Win32Game::Startup();

	// Create managers...
	TextureManager::shared_ptr textureManager( new TextureManager() );
	PixelShaderManager::shared_ptr pixelShaderManager( new PixelShaderManager() );
	VertexShaderManager::shared_ptr vertexShaderManager( new VertexShaderManager() );
	GeometryManager::shared_ptr geometryManager( new GeometryManager( textureManager, pixelShaderManager, vertexShaderManager ) );

	m_sceneManager.reset( new scene::SceneManager );
	bi::IntegrateManager( *m_sceneManager.get() );
    m_sceneManager->GetSceneLoader().AddLoaderFunctor( "addcustomcontrollers", loader::SceneLoader::LoaderFunctor::shared_ptr( new AddCustomControllersLF ) );
	m_sceneManager->SetGeometryManager( geometryManager );
	scene::Scene::weak_ptr mainScene = m_sceneManager->LoadScene( "main", "media/scene1.xml" );
}

bool MyGame::Update( unify::Seconds elapsed, IInput & input )
{
	//elapsed *= 0.01f;
	m_sceneManager->Update( elapsed, input );
	return Win32Game::Update( elapsed, input );
}

void MyGame::Render()
{
	m_sceneManager->Render();
}

void MyGame::Shutdown()
{
	m_sceneManager.reset();
	Win32Game::Shutdown();
}
