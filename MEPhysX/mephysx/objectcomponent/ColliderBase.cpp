// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <mephysx/objectcomponent/ColliderBase.h>
#include <mephysx/objectcomponent/RigidBody.h>
#include <mephysx/Util.h>
#include <me/object/Object.h>

using namespace me;
using namespace mephysx;
using namespace physx;
using namespace objectcomponent;

ColliderBase::ColliderBase( ColliderBase & colliderBase )
	: me::object::ObjectComponent( colliderBase )
	, m_gameComponent( colliderBase.m_gameComponent )
	, m_material( colliderBase.m_material )
	, m_shape( colliderBase.m_shape )
{
}

ColliderBase::ColliderBase( std::string typeName, mephysx::GameComponent * gameComponent )
	: me::object::ObjectComponent( typeName, false, false )
	, m_gameComponent( gameComponent )
	, m_material( gameComponent->GetPhysics()->createMaterial( 0.5f, 0.5f, 0.6f ), Releaser< PxMaterial > )
{
}

ColliderBase::~ColliderBase()
{
}

void ColliderBase::OnAttach( me::object::Object * object )
{
	me::object::ObjectComponent::OnAttach( object );

	// Attach to RigidBody.
	for( int i = 0; i < object->GetComponentCount(); ++i )
	{
		objectcomponent::RigidBody * body = dynamic_cast< objectcomponent::RigidBody * >( object->GetComponent( i ).get() );
		if ( body )
		{
			body->GetRigidBody()->attachShape( *m_shape.get() );
			break; // Only one possible rigid body.
		}
	}
}

void ColliderBase::OnDetach( me::object::Object * object )
{
	me::object::ObjectComponent::OnDetach( object );
}
		
physx::PxShape * ColliderBase::GetShape()
{
	return m_shape ? m_shape.get() : nullptr;
}

const physx::PxShape * ColliderBase::GetShape() const
{
	return m_shape ? m_shape.get() : nullptr;
}
