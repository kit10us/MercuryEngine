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

std::string RigidBody::GetType() const
{
	return "RigidBody";
}

std::string RigidBody::GetWhat() const
{
	return std::string();
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

int RigidBody::GetValueCount() const
{
	return (int)g_ValuesList.size();
}

bool RigidBody::ValueExists( std::string name ) const
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

std::string RigidBody::GetValueName( int index ) const
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

int RigidBody::FindValueIndex( std::string name ) const
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

bool RigidBody::SetValue( int index, std::string value )
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

bool RigidBody::SetValue( std::string name, std::string value )
{
	int index = FindValueIndex( name );
	return SetValue( index, value );
}

std::string RigidBody::GetValue( int index ) const
{
	switch ( index )
	{
	default:
		return std::string();
	case 0:
		return unify::Cast< std::string >( m_enabled );
	}
}
 
std::string RigidBody::GetValue( std::string name ) const
{
	int index = FindValueIndex( name );
	return GetValue( index );
}
