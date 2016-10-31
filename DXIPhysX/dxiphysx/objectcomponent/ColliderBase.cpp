// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxiphysx/objectcomponent/ColliderBase.h>
#include <dxiphysx/objectcomponent/RigidBody.h>
#include <dxiphysx/Util.h>
#include <dxi/scene/Object.h>

using namespace dxi;
using namespace dxiphysx;
using namespace physx;
using namespace objectcomponent;

ColliderBase::ColliderBase( ColliderBase & colliderBase )
	: m_os( colliderBase.m_os )
	, m_gameComponent( colliderBase.m_gameComponent )
	, m_material( colliderBase.m_material )
	, m_shape( colliderBase.m_shape )
{
}


ColliderBase::ColliderBase( core::IOS * os, GameComponent * gameComponent )
	: m_os( os )
	, m_gameComponent( gameComponent )
	, m_material( gameComponent->GetPhysics()->createMaterial( 0.5f, 0.5f, 0.6f ), Releaser< PxMaterial > )
{
}

ColliderBase::~ColliderBase()
{
}

core::IOS * ColliderBase::GetOS()
{
	return m_os;
}

const core::IOS * ColliderBase::GetOS() const
{
	return m_os;
}

bool ColliderBase::IsEnabled() const
{
	return m_enabled;
}

void ColliderBase::SetEnabled( bool enabled )
{
	m_enabled = enabled;
}

void ColliderBase::OnAttach( dxi::scene::Object * object )
{
	// Attach to RigidBody.
	for( int i = 0; i < object->ComponentCount(); ++i )
	{
		objectcomponent::RigidBody * body = dynamic_cast< objectcomponent::RigidBody * >( object->GetComponent( i ).get() );
		if ( body )
		{
			body->GetRigidBody()->attachShape( *m_shape.get() );
			break; // Only one possible rigid body.
		}
	}
}
		
physx::PxShape * ColliderBase::GetShape()
{
	return m_shape ? m_shape.get() : nullptr;
}

const physx::PxShape * ColliderBase::GetShape() const
{
	return m_shape ? m_shape.get() : nullptr;
}
