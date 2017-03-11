// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/motivator/ObjectOrbitMotivator.h>
#include <unify/String.h>
#include <map>

using namespace me;
using namespace motivator;

namespace
{
	std::map< std::string, int, unify::CaseInsensitiveLessThanTest > g_ValuesMap
	{
		{ "origin", 0 },
		{ "orbit", 1 },
		{ "angleASecond", 2 }
	};
	std::vector< std::string > g_ValuesList
	{
		{ "origin" },
		{ "orbit" },
		{ "angleASecond" }
	};
}

ObjectOrbitMotivator::ObjectOrbitMotivator( unify::V3< float > origin, unify::V3< float > orbit, unify::Angle angleASecond )
	: scene::ObjectComponent( "ObjectOrbitMotivator" )
	, m_origin{ origin }
	, m_orbit{ orbit }
	, m_angleASecond{ angleASecond }
{
}

ObjectOrbitMotivator::~ObjectOrbitMotivator() 
{
}

std::string ObjectOrbitMotivator::GetWhat() const
{
	return std::string();
}								

bool ObjectOrbitMotivator::Updateable() const
{
	return true;
}

bool ObjectOrbitMotivator::Renderable() const
{
	return false;
}
 
void ObjectOrbitMotivator::OnUpdate( UpdateParams params )
{
	if ( ! m_enabled )
	{
		return;
	}

	m_object->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( m_orbit, m_angleASecond * params.renderInfo.GetDelta() ) );
	
	m_object->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );
}

void ObjectOrbitMotivator::CollectGeometry( GeometryCache & cache, const unify::FrameLite * frame )
{
}

void ObjectOrbitMotivator::OnSuspend()
{
}

void ObjectOrbitMotivator::OnResume()
{
}

scene::IObjectComponent * ObjectOrbitMotivator::Duplicate()
{
	auto duplicate = new ObjectOrbitMotivator( m_origin, m_orbit, m_angleASecond );
	return duplicate;
}

int ObjectOrbitMotivator::GetValueCount() const
{
	return scene::ObjectComponent::GetValueCount() + g_ValuesList.size();
}

bool ObjectOrbitMotivator::ValueExists( std::string name ) const
{
	if ( scene::ObjectComponent::ValueExists( name ) )
	{
		return true;
	}

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

std::string ObjectOrbitMotivator::GetValueName( int index ) const
{
	if ( index >= GetValueCount() )
	{
		return std::string();
	}

	int baseValueCount = scene::ObjectComponent::GetValueCount();
	if ( index < baseValueCount )
	{
		return scene::ObjectComponent::GetValueName( index );
	}
	else
	{
		return g_ValuesList[ index - baseValueCount ];
	}
}

int ObjectOrbitMotivator::FindValueIndex( std::string name ) const
{
	auto && itr = g_ValuesMap.find( name );
	if ( itr == g_ValuesMap.end() )
	{
		return scene::ObjectComponent::FindValueIndex( name );
	}
	else
	{
		return itr->second + scene::ObjectComponent::GetValueCount();
	}
}

bool ObjectOrbitMotivator::SetValue( int index, std::string value )
{
	int baseValueCount = scene::ObjectComponent::GetValueCount();
	if ( index < baseValueCount )
	{
		return scene::ObjectComponent::SetValue( index, value );
	}

	switch ( index - baseValueCount )
	{
	case 0:
		m_origin = unify::V3< float >( value );
		return true;
	case 1:
		m_orbit = unify::V3< float >( value );
		return true;
	case 2:
		m_angleASecond = unify::Angle( value );
		return true;
	}

	return false;
}

bool ObjectOrbitMotivator::SetValue( std::string name, std::string value )
{
	int index = FindValueIndex( name );
	return SetValue( index, value );
}

std::string ObjectOrbitMotivator::GetValue( int index ) const
{
	int baseValueCount = scene::ObjectComponent::GetValueCount();
	if ( index < baseValueCount )
	{
		return scene::ObjectComponent::GetValue( index );
	}

	switch ( index - baseValueCount )
	{
	case 0:
		return m_origin.ToString();
	case 1:
		return m_orbit.ToString();
	case 2:
		return m_angleASecond.ToString( false );
	}

	return std::string();
}
 
std::string ObjectOrbitMotivator::GetValue( std::string name ) const
{
	int index = FindValueIndex( name );
	return GetValue( index );
}
