// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <mephysx/objectcomponent/ControllerBase.h>

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

ControllerBase::ControllerBase( ControllerBase & controller )
	: m_os( controller.m_os )
	, m_sceneComponent( controller.m_sceneComponent )
{
}


ControllerBase::ControllerBase( me::IOS * os, SceneComponent * sceneComponent )
	: m_os( os )
	, m_sceneComponent( sceneComponent )
{
}

ControllerBase::~ControllerBase()
{
}

me::IOS * ControllerBase::GetOS()
{
	return m_os;
}

const me::IOS * ControllerBase::GetOS() const
{
	return m_os;
}

bool ControllerBase::IsEnabled() const
{
	return m_enabled;
}

void ControllerBase::SetEnabled( bool enabled )
{
	m_enabled = enabled;
}

PxController * ControllerBase::GetController()
{
	return m_controller.get();
}

const PxController * ControllerBase::GetController() const
{
	return m_controller.get();
}

MEPHYSX_API SceneComponent * ControllerBase::GetScene()
{
	return m_sceneComponent;
}

MEPHYSX_API const SceneComponent * ControllerBase::GetScene() const
{
	return m_sceneComponent;
}

int ControllerBase::GetValueCount() const
{
	return (int)g_ValuesList.size();
}

bool ControllerBase::ValueExists( std::string name ) const
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

std::string ControllerBase::GetValueName( int index ) const
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

int ControllerBase::FindValueIndex( std::string name ) const
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

bool ControllerBase::SetValue( int index, std::string value )
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

bool ControllerBase::SetValue( std::string name, std::string value )
{
	int index = FindValueIndex( name );
	return SetValue( index, value );
}

std::string ControllerBase::GetValue( int index ) const
{
	switch ( index )
	{
	default:
		return std::string();
	case 0:
		return unify::Cast< std::string >( m_enabled );
	}
}
 
std::string ControllerBase::GetValue( std::string name ) const
{
	int index = FindValueIndex( name );
	return GetValue( index );
}
