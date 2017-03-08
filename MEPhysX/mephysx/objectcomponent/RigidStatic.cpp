// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <mephysx/objectcomponent/RigidStatic.h>
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

std::string RigidStatic::GetType() const
{
	return "RigidStatic";
}

std::string RigidStatic::GetWhat() const
{
	return std::string();
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

int RigidStatic::GetValueCount() const
{
	return (int)g_ValuesList.size();
}

bool RigidStatic::ValueExists( std::string name ) const
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

std::string RigidStatic::GetValueName( int index ) const
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

int RigidStatic::FindValueIndex( std::string name ) const
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

bool RigidStatic::SetValue( int index, std::string value )
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

bool RigidStatic::SetValue( std::string name, std::string value )
{
	int index = FindValueIndex( name );
	return SetValue( index, value );
}

std::string RigidStatic::GetValue( int index ) const
{
	switch ( index )
	{
	default:
		return std::string();
	case 0:
		return unify::Cast< std::string >( m_enabled );
	}
}
 
std::string RigidStatic::GetValue( std::string name ) const
{
	int index = FindValueIndex( name );
	return GetValue( index );
}
