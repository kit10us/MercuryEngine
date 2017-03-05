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
	: m_origin{ origin }
	, m_orbit{ orbit }
	, m_angleASecond{ angleASecond }
{
}

ObjectOrbitMotivator::~ObjectOrbitMotivator() 
{
}

std::string ObjectOrbitMotivator::GetType() const
{
	return "ObjectOrbitMotivator";
}

std::string ObjectOrbitMotivator::GetWhat() const
{
	return std::string();
}								

bool ObjectOrbitMotivator::IsEnabled() const
{
	return m_enabled;
}

void ObjectOrbitMotivator::SetEnabled( bool enabled )
{
	m_enabled = enabled;
}

bool ObjectOrbitMotivator::Updateable() const
{
	return true;
}

bool ObjectOrbitMotivator::Renderable() const
{
	return false;
}

void ObjectOrbitMotivator::OnAttach( scene::Object * object )
{
	m_target = object;
}

void ObjectOrbitMotivator::OnDetach()
{
	m_target = nullptr;
}

void ObjectOrbitMotivator::OnInit()
{
}

void ObjectOrbitMotivator::OnStart()
{
}

void ObjectOrbitMotivator::OnUpdate( UpdateParams params )
{
	if ( ! m_target )
	{
		return;
	}

	if ( ! m_enabled )
	{
		return;
	}

	m_target->GetFrame().Orbit( unify::V3< float >( 0, 0, 0 ), unify::Quaternion( m_orbit, m_angleASecond * params.renderInfo.GetDelta() ) );
	
	m_target->GetFrame().LookAt( unify::V3< float >( 0, 0, 0 ), unify::V3< float >( 0, 1, 0 ) );
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
	return 3;
}

bool ObjectOrbitMotivator::ValueExists( std::string name ) const
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

std::string ObjectOrbitMotivator::GetValueName( int index ) const
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

int ObjectOrbitMotivator::FindValueIndex( std::string name ) const
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

bool ObjectOrbitMotivator::SetValue( int index, std::string value )
{
	switch ( index )
	{
	default:
		return false;
	case 0:
		m_origin = unify::V3< float >( value );
		break;
	case 1:
		m_orbit = unify::V3< float >( value );
		break;
	case 2:
		m_angleASecond = unify::Angle( value );
		break;
	}
	return true;
}

bool ObjectOrbitMotivator::SetValue( std::string name, std::string value )
{
	int index = FindValueIndex( name );
	return SetValue( index, value );
}

std::string ObjectOrbitMotivator::GetValue( int index ) const
{
	switch ( index )
	{
	default:
		return std::string();
	case 0:
		return m_origin.ToString();
		break;
	case 1:
		return m_orbit.ToString();
		break;
	case 2:
		return m_angleASecond.ToString( false );
		break;
	}
}
 
std::string ObjectOrbitMotivator::GetValue( std::string name ) const
{
	int index = FindValueIndex( name );
	return GetValue( index );
}
