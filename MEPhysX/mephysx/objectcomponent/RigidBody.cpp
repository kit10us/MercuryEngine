// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <mephysx/objectcomponent/RigidBody.h>
#include <mephysx/Util.h>
#include <me/scene/Object.h>
#include <mephysx/objectcomponent/BoxCollider.h>
#include <mephysx/SceneComponent.h>
#include <me/scene/Scene.h>

using namespace me;
using namespace scene;
using namespace mephysx;
using namespace physx;
using namespace objectcomponent;

RigidBody::RigidBody( RigidBody & rigidBody )
	: me::scene::ObjectComponent( "RigidBody" )
	, m_gameComponent( rigidBody.m_gameComponent )
{
	PxTransform transform( util::Convert< physx::PxTransform >( unify::MatrixIdentity() ) );
	m_rigidBody.reset( m_gameComponent->GetPhysics()->createRigidDynamic( transform ), Releaser< physx::PxRigidBody > );
}

RigidBody::RigidBody( mephysx::GameComponent * gameComponent )
	: me::scene::ObjectComponent( "RigidBody" )
	, m_gameComponent( gameComponent )
{
	PxTransform transform( util::Convert< physx::PxTransform >( unify::MatrixIdentity() ) );
	m_rigidBody.reset( m_gameComponent->GetPhysics()->createRigidDynamic( transform ), Releaser< physx::PxRigidBody > );
}

RigidBody::~RigidBody()
{
}

std::string RigidBody::GetWhat() const
{
	return std::string();
}					 

void RigidBody::OnAttach( me::scene::Object * object )
{
	me::scene::ObjectComponent::OnAttach( object );

	// Sync physx to object.
	PxTransform transform( util::Convert< physx::PxTransform >( object->GetFrame().GetMatrix() ) );
	m_rigidBody->userData = object;
	m_rigidBody->setGlobalPose( transform );

	// Attach any existing colliders.
	for ( int i = 0; i < object->GetComponentCount(); ++i )
	{
		objectcomponent::ColliderBase * collider = dynamic_cast< objectcomponent::ColliderBase * >(object->GetComponent( i ).get());
		if ( collider )
		{
			m_rigidBody->attachShape( *collider->GetShape() );
		}
	}
	PxRigidBodyExt::updateMassAndInertia( *m_rigidBody, 10.0f );

	me::scene::Scene * scene = object->GetScene();
	ISceneComponent::ptr component = scene->GetComponent( "PhysXScene" );
	SceneComponent * sceneComponent = dynamic_cast< SceneComponent * >( component.get() );
	sceneComponent->GetScene()->addActor( *m_rigidBody.get() );
}

void RigidBody::OnDetach( me::scene::Object * object )
{
	me::scene::ObjectComponent::OnDetach( object );
}

me::scene::IObjectComponent * RigidBody::Duplicate()
{
	return new RigidBody( *this );
}
		
physx::PxRigidBody * RigidBody::GetRigidBody()
{
	return m_rigidBody ? m_rigidBody.get() : nullptr;
}

const physx::PxRigidBody * RigidBody::GetRigidBody() const
{
	return m_rigidBody ? m_rigidBody.get() : nullptr;
}
