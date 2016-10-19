// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxiphysx/objectcomponent/RigidBody.h>
#include <dxiphysx/Util.h>
#include <dxi/scene/Object.h>
#include <dxiphysx/objectcomponent/BoxCollider.h>

using namespace dxi;
using namespace dxiphysx;
using namespace physx;
using namespace objectcomponent;

RigidBody::RigidBody( core::IOS * os, physx::PxPhysics * physics )
: m_os( os )
, m_physics( physics )
{
}

RigidBody::~RigidBody()
{
}

core::IOS * RigidBody::GetOS()
{
	return m_os;
}

const core::IOS * RigidBody::GetOS() const
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

void RigidBody::OnAttach( dxi::scene::Object * object )
{
	PxTransform transform( util::Convert< physx::PxTransform >( object->GetFrame().GetMatrix() ) );
	m_rigidBody.reset( m_physics->createRigidDynamic( transform ), Releaser< physx::PxRigidBody > );
	m_rigidBody->userData = object;

	// Attach any existing colliders.
	for ( int i = 0; i < object->ComponentCount(); ++i )
	{
		objectcomponent::ColliderBase * collider = dynamic_cast< objectcomponent::ColliderBase * >(object->GetComponent( i ).get());
		if ( collider )
		{
			m_rigidBody->attachShape( *collider->GetShape() );
		}
	}
}
		
physx::PxRigidBody * RigidBody::GetRigidBody()
{
	return m_rigidBody ? m_rigidBody.get() : nullptr;
}

const physx::PxRigidBody * RigidBody::GetRigidBody() const
{
	return m_rigidBody ? m_rigidBody.get() : nullptr;
}
