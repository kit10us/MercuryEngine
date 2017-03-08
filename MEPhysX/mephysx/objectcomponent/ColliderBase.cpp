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

namespace {
	std::map< std::string, int, unify::CaseInsensitiveLessThanTest > g_ValuesMap
	{
		{ "enabled", 0 },
	};

	std::vector< std::string > g_ValuesList
	{
		{ "enabled" },
	};
}

ColliderBase::ColliderBase( ColliderBase & colliderBase )
	: m_os( colliderBase.m_os )
	, m_gameComponent( colliderBase.m_gameComponent )
	, m_material( colliderBase.m_material )
	, m_shape( colliderBase.m_shape )
{
}


ColliderBase::ColliderBase( me::IOS * os, GameComponent * gameComponent )
	: m_os( os )
	, m_gameComponent( gameComponent )
	, m_material( gameComponent->GetPhysics()->createMaterial( 0.5f, 0.5f, 0.6f ), Releaser< PxMaterial > )
{
}

ColliderBase::~ColliderBase()
{
}

me::IOS * ColliderBase::GetOS()
{
	return m_os;
}

const me::IOS * ColliderBase::GetOS() const
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

void ColliderBase::OnAttach( me::scene::Object * object )
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

int ColliderBase::GetValueCount() const
{
	return (int)g_ValuesList.size();
}

bool ColliderBase::ValueExists( std::string name ) const
{
	auto && itr = g_ValuesMap.find( name );
	if ( itr == g_ValuesMap.end() )
	{
		return false;
	}
	else
	{
		return true;
	}
}

std::string ColliderBase::GetValueName( int index ) const
{
	if ( index >= (int)g_ValuesList.size() )
	{
		return std::string();
	}
	else
	{
		return g_ValuesList[ index ];
	}
}

int ColliderBase::FindValueIndex( std::string name ) const
{
	auto && itr = g_ValuesMap.find( name );
	if ( itr == g_ValuesMap.end() )
	{
		return -1;
	}
	else
	{
		return itr->second;
	}
}

bool ColliderBase::SetValue( int index, std::string value )
{
	switch ( index )
	{
	default:
		return false;
	case 0:
		m_enabled = unify::Cast< bool >( value );
		break;
	}
	return true;
}

bool ColliderBase::SetValue( std::string name, std::string value )
{
	int index = FindValueIndex( name );
	return SetValue( index, value );
}

std::string ColliderBase::GetValue( int index ) const
{
	switch ( index )
	{
	default:
		return std::string();
	case 0:
		return unify::Cast< std::string >( m_enabled );
	}
}
 
std::string ColliderBase::GetValue( std::string name ) const
{
	int index = FindValueIndex( name );
	return GetValue( index );
}
