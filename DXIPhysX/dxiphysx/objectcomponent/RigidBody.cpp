// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxiphysx/objectcomponent/RigidBody.h>
#include <dxiphysx/Util.h>
#include <me/scene/Object.h>
#include <dxiphysx/objectcomponent/BoxCollider.h>
#include <dxiphysx/SceneComponent.h>
#include <me/scene/Scene.h>

using namespace me;
using namespace scene;
using namespace dxiphysx;
using namespace physx;
using namespace objectcomponent;

RigidBody::RigidBody( RigidBody & rigidBody )
	: m_os( rigidBody.m_os )
	, m_gameComponent( rigidBody.m_gameComponent )
{
	PxTransform transform( util::Convert< physx::PxTransform >( unify::MatrixIdentity() ) );
	m_rigidBody.reset( m_gameComponent->GetPhysics()->createRigidDynamic( transform ), Releaser< physx::PxRigidBody > );
}

RigidBody::RigidBody( me::IOS * os, GameComponent * gameComponent )
	: m_os( os )
	, m_gameComponent( gameComponent )
{
	PxTransform transform( util::Convert< physx::PxTransform >( unify::MatrixIdentity() ) );
	m_rigidBody.reset( m_gameComponent->GetPhysics()->createRigidDynamic( transform ), Releaser< physx::PxRigidBody > );
}

RigidBody::~RigidBody()
{
}

me::IOS * RigidBody::GetOS()
{
	return m_os;
}

const me::IOS * RigidBody::GetOS() const
{
	return m_os;
}

std::string RigidBody::GetName() const
{
	return "RigidBody";
}
	
bool RigidBody::IsEnabled() const
{
	return m_enabled;
}

void RigidBody::SetEnabled( bool enabled )
{
	m_enabled = enabled;
}

void RigidBody::OnAttach( me::scene::Object * object )
{
	// Sync physx to object.
	PxTransform transform( util::Convert< physx::PxTransform >( object->GetFrame().GetMatrix() ) );
	m_rigidBody->userData = object;
	m_rigidBody->setGlobalPose( transform );

	// Attach any existing colliders.
	for ( int i = 0; i < object->ComponentCount(); ++i )
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

IObjectComponent * RigidBody::Duplicate()
{
	auto duplicate = new RigidBody( *this );
	return duplicate;
}
		
physx::PxRigidBody * RigidBody::GetRigidBody()
{
	return m_rigidBody ? m_rigidBody.get() : nullptr;
}

const physx::PxRigidBody * RigidBody::GetRigidBody() const
{
	return m_rigidBody ? m_rigidBody.get() : nullptr;
}
