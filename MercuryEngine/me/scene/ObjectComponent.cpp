// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/scene/ObjectComponent.h>

using namespace me;
using namespace scene;

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

ObjectComponent::ObjectComponent( ObjectComponent & component )
	: m_os( component.m_os )
{
}

ObjectComponent::ObjectComponent( IOS * os )
: m_os( os )
{
}

ObjectComponent::~ObjectComponent()
{
}

IOS * ObjectComponent::GetOS()
{
	return m_os;
}

const IOS * ObjectComponent::GetOS() const
{
	return m_os;
}

bool ObjectComponent::IsEnabled() const
{
	return m_enabled;
}

void ObjectComponent::SetEnabled( bool enabled )
{
	m_enabled = enabled;
}

int ObjectComponent::GetValueCount() const
{
	return (int)g_ValuesList.size();
}

bool ObjectComponent::ValueExists( std::string name ) const
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

std::string ObjectComponent::GetValueName( int index ) const
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

int ObjectComponent::FindValueIndex( std::string name ) const
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

bool ObjectComponent::SetValue( int index, std::string value )
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

bool ObjectComponent::SetValue( std::string name, std::string value )
{
	int index = FindValueIndex( name );
	return SetValue( index, value );
}

std::string ObjectComponent::GetValue( int index ) const
{
	switch ( index )
	{
	default:
		return std::string();
	case 0:
		return unify::Cast< std::string >( m_enabled );
	}
}
 
std::string ObjectComponent::GetValue( std::string name ) const
{
	int index = FindValueIndex( name );
	return GetValue( index );
}
