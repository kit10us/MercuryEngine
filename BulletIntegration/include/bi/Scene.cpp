// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <bi/Scene.h>
#include <boost/cast.hpp>
#include <MMSystem.h>

using namespace bi;

Scene::Scene()
	: m_lastTime( timeGetTime() )
	, m_broadphase( new btDbvtBroadphase )
	, m_collisionConfiguration( new btDefaultCollisionConfiguration )
	, m_dispatcher( new btCollisionDispatcher( m_collisionConfiguration.get() ) )
	, m_solver( new btSequentialImpulseConstraintSolver )
	, m_dynamicsWorld( new btDiscreteDynamicsWorld( m_dispatcher.get(), m_broadphase.get(), m_solver.get(), m_collisionConfiguration.get() ) )
	, m_debugDraw( new DebugDraw() )
{
	m_dynamicsWorld->setGravity( btVector3( 0, -20, 0 ) );
	m_dynamicsWorld->setDebugDrawer( m_debugDraw.get() );
	m_debugDraw->setDebugMode( btIDebugDraw::DBG_DrawWireframe );

	// Test ground...
	m_groundShape.reset( new btStaticPlaneShape( btVector3( 0, 1, 0 ) , 1 ) );
	m_groundMotionState.reset( new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0))) );
    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI( 
		0, m_groundMotionState.get(), m_groundShape.get(), btVector3( 0, 0, 0 )
		);

	m_groundRigidBody.reset( new btRigidBody( groundRigidBodyCI ) );
	m_dynamicsWorld->addRigidBody( m_groundRigidBody.get() );
}

Scene::~Scene()
{
	// Order of destroy matter...
	m_dynamicsWorld.reset();
}

btDiscreteDynamicsWorld * Scene::GetWorld()
{
	return m_dynamicsWorld.get();
}

const btDiscreteDynamicsWorld * Scene::GetWorld() const
{
	return m_dynamicsWorld.get();
}

void Scene::AddShape( const std::string & name, boost::shared_ptr< btCollisionShape > shape )
{
	m_shapes[ name ] = shape;
}

bool Scene::FindShape( const std::string & name, boost::shared_ptr< btCollisionShape > & shape )
{
	std::map< std::string, boost::shared_ptr< btCollisionShape > >::iterator itr = m_shapes.find( name );
	if( itr == m_shapes.end() )
	{
		shape.reset();
		return false;
	}
	shape = itr->second;
	return true;
}

void Scene::Add( boost::shared_ptr< dxi::physics::IInstance > object )
{
	m_allObjects.push_back( object );
	bi::Instance * biInstance( boost::polymorphic_downcast< bi::Instance * >( object.get() ) );
	m_dynamicsWorld->addRigidBody( biInstance->GetRigidBody() );
}

void Scene::Add( dxi::scene::Object::shared_ptr object, const std::string & shapeName )
{
	boost::shared_ptr< btCollisionShape > collisionShape;
	if( ! shapeName.empty() )
	{
		if( ! FindShape( shapeName, collisionShape ) )
		{
			throw std::exception( std::string( std::string( "Failed to find shape associated with object! (" ) + shapeName + ")" ).c_str() );
		}
	}

	bi::Instance * physics( new bi::Instance( object.get(), collisionShape ) );
	object->SetPhysics( boost::shared_ptr< dxi::physics::IInstance >( physics ) );
}

void Scene::Update( unify::Seconds elapsed, dxi::IInput & input )
{
	m_dynamicsWorld->stepSimulation( elapsed, 10 );
}

void Scene::Render()
{
	m_dynamicsWorld->debugDrawWorld();
}
