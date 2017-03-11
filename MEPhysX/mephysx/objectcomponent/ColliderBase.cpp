// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <mephysx/objectcomponent/ColliderBase.h>
#include <mephysx/objectcomponent/RigidBody.h>
#include <mephysx/Util.h>
#include <me/scene/Object.h>

using namespace me;
using namespace mephysx;
using namespace physx;
using namespace objectcomponent;

ColliderBase::ColliderBase( ColliderBase & colliderBase )
	: me::scene::ObjectComponent( colliderBase.GetTypeName() )
	, m_gameComponent( colliderBase.m_gameComponent )
	, m_material( colliderBase.m_material )
	, m_shape( colliderBase.m_shape )
{
}

ColliderBase::ColliderBase( std::string typeName, mephysx::GameComponent * gameComponent )
	: me::scene::ObjectComponent( typeName )
	, m_gameComponent( gameComponent )
	, m_material( gameComponent->GetPhysics()->createMaterial( 0.5f, 0.5f, 0.6f ), Releaser< PxMaterial > )
{
}

ColliderBase::~ColliderBase()
{
}

void ColliderBase::OnAttach( me::scene::Object * object )
{
	me::scene::ObjectComponent::OnAttach( object );

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

void ColliderBase::OnDetach( me::scene::Object * object )
{
	me::scene::ObjectComponent::OnDetach( object );
}
		
physx::PxShape * ColliderBase::GetShape()
{
	return m_shape ? m_shape.get() : nullptr;
}

const physx::PxShape * ColliderBase::GetShape() const
{
	return m_shape ? m_shape.get() : nullptr;
}
