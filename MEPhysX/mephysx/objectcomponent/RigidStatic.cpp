// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <mephysx/objectcomponent/RigidStatic.h>
#include <mephysx/Util.h>
#include <me/object/Object.h>
#include <mephysx/objectcomponent/BoxCollider.h>
#include <mephysx/SceneComponent.h>
#include <me/scene/Scene.h>

using namespace me;
using namespace scene;
using namespace mephysx;
using namespace physx;
using namespace objectcomponent;

RigidStatic::RigidStatic( RigidStatic & rigidStatic )
	: me::object::ObjectComponent( "RigidStatic" )
	, m_gameComponent( rigidStatic.m_gameComponent )
{
	PxTransform transform( util::Convert< physx::PxTransform >( unify::MatrixIdentity() ) );
	m_rigidStatic.reset( m_gameComponent->GetPhysics()->createRigidStatic( transform ), Releaser< physx::PxRigidStatic > );
}

RigidStatic::RigidStatic( mephysx::GameComponent * gameComponent )
: me::object::ObjectComponent( "RigidStatic" )
, m_gameComponent( gameComponent )
{
	PxTransform transform( util::Convert< physx::PxTransform >( unify::MatrixIdentity() ) );
	m_rigidStatic.reset( m_gameComponent->GetPhysics()->createRigidStatic( transform ), Releaser< physx::PxRigidStatic > );
}

RigidStatic::~RigidStatic()
{
}

std::string RigidStatic::GetWhat() const
{
	return std::string();
}					   
	
void RigidStatic::OnAttach( me::object::Object * object )
{
	me::object::ObjectComponent::OnAttach( object );

	// Sync physx to object.
	PxTransform transform( util::Convert< physx::PxTransform >( object->GetFrame().GetMatrix() ) );
	m_rigidStatic->userData = object;
	m_rigidStatic->setGlobalPose( transform );

	// Attach any existing colliders.
	for ( int i = 0; i < object->GetComponentCount(); ++i )
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

void RigidStatic::OnDetach( me::object::Object * object )
{
	me::object::ObjectComponent::OnDetach( object );
}

		
me::object::IObjectComponent::ptr RigidStatic::Duplicate()
{
	return object::IObjectComponent::ptr( new RigidStatic( *this ) );
}

physx::PxRigidStatic * RigidStatic::GetRigidStatic()
{
	return m_rigidStatic ? m_rigidStatic.get() : nullptr;
}

const physx::PxRigidStatic * RigidStatic::GetRigidStatic() const
{
	return m_rigidStatic ? m_rigidStatic.get() : nullptr;
}