// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxiphysx/objectcomponent/RigidStatic.h>
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

RigidStatic::RigidStatic( RigidStatic & rigidStatic )
	: m_os( rigidStatic.m_os )
	, m_gameComponent( rigidStatic.m_gameComponent )
{
	PxTransform transform( util::Convert< physx::PxTransform >( unify::MatrixIdentity() ) );
	m_rigidStatic.reset( m_gameComponent->GetPhysics()->createRigidStatic( transform ), Releaser< physx::PxRigidStatic > );
}

RigidStatic::RigidStatic( me::IOS * os, GameComponent * gameComponent )
: m_os( os )
, m_gameComponent( gameComponent )
{
	PxTransform transform( util::Convert< physx::PxTransform >( unify::MatrixIdentity() ) );
	m_rigidStatic.reset( m_gameComponent->GetPhysics()->createRigidStatic( transform ), Releaser< physx::PxRigidStatic > );
}

RigidStatic::~RigidStatic()
{
}

me::IOS * RigidStatic::GetOS()
{
	return m_os;
}

const me::IOS * RigidStatic::GetOS() const
{
	return m_os;
}

std::string RigidStatic::GetName() const
{
	return "RigidStatic";
}
	
bool RigidStatic::IsEnabled() const
{
	return m_enabled;
}

void RigidStatic::SetEnabled( bool enabled )
{
	m_enabled = enabled;
}

void RigidStatic::OnAttach( me::scene::Object * object )
{
	// Sync physx to object.
	PxTransform transform( util::Convert< physx::PxTransform >( object->GetFrame().GetMatrix() ) );
	m_rigidStatic->userData = object;
	m_rigidStatic->setGlobalPose( transform );

	// Attach any existing colliders.
	for ( int i = 0; i < object->ComponentCount(); ++i )
	{
		objectcomponent::ColliderBase * collider = dynamic_cast< objectcomponent::ColliderBase * >(object->GetComponent( i ).get());
		if ( collider )
		{
			m_rigidStatic->attachShape( *collider->GetShape() );
		}
	}

	me::scene::Scene * scene = object->GetScene();
	ISceneComponent::ptr component = scene->GetComponent( "PhysXScene" );
	SceneComponent * sceneComponent = dynamic_cast< SceneComponent * >( component.get() );
	sceneComponent->GetScene()->addActor( *m_rigidStatic.get() );
}
		
IObjectComponent * RigidStatic::Duplicate()
{
	auto duplicate = new RigidStatic( *this );
	return duplicate;
}

physx::PxRigidStatic * RigidStatic::GetRigidStatic()
{
	return m_rigidStatic ? m_rigidStatic.get() : nullptr;
}

const physx::PxRigidStatic * RigidStatic::GetRigidStatic() const
{
	return m_rigidStatic ? m_rigidStatic.get() : nullptr;
}
