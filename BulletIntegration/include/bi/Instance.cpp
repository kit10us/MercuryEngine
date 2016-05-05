// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <bi/Instance.h>

using namespace bi;

Instance::Instance( dxi::physics::IObjectSyncer * syncer, boost::shared_ptr< btCollisionShape > collisionShape )
	: m_syncer( syncer )
{
	btScalar mass = 1;
	btVector3 fallInertia( 0, 0, 0 );
	collisionShape->calculateLocalInertia( mass, fallInertia );
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI( mass, this, collisionShape.get(), fallInertia );
	SetShape( collisionShape );
	btRigidBody * body = new btRigidBody( fallRigidBodyCI );
	SetRigidBody( boost::shared_ptr< btRigidBody >( body ) );
	SyncPhysicsFromObject();
}

void Instance::SyncObjectFromPhysics()
{
	m_syncer->SyncFrame( convert( m_rigidBody->getWorldTransform() ) );
}

void Instance::SyncPhysicsFromObject()
{
	m_rigidBody->setWorldTransform( convert( m_syncer->GetWorldFrame() ) );//setMotionState( this );
}

bool Instance::IsActive() const
{
	return m_rigidBody->isActive() && ! m_rigidBody->wantsSleeping();
}

void Instance::SetShape( boost::shared_ptr< btCollisionShape > shape )
{
	m_shape = shape;
}

void Instance::SetRigidBody( boost::shared_ptr< btRigidBody > rigidBody )
{
	m_rigidBody = rigidBody;
}

btRigidBody * Instance::GetRigidBody()
{
	return m_rigidBody.get();
}

const btRigidBody * Instance::GetRigidBody() const
{
	return m_rigidBody.get();
}

void Instance::getWorldTransform( btTransform & worldTrans ) const
{
	worldTrans = convert( m_syncer->GetWorldFrame() );
	worldTrans.inverse();
}

void Instance::setWorldTransform( const btTransform & worldTrans )
{
	m_syncer->SyncFrame( convert( worldTrans ) );
}
