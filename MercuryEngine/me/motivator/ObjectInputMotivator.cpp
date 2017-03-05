// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/motivator/ObjectInputMotivator.h>
#include <unify/Angle.h>
#include <unify/V3.h>
#include <unify/Quaternion.h>

using namespace me;
using namespace motivator;
using namespace input;

namespace {
	std::map< std::string, int, unify::CaseInsensitiveLessThanTest > g_ValuesMap
	{
		{ "enabled", 0 },
		{ "walkSpeed", 1 },
		{ "runSpeed", 2 },
		{ "lookXSpeed", 3 },
		{ "lookYSpeed", 4 }
	};

	std::vector< std::string > g_ValuesList
	{
		{ "enabled" },
		{ "walkSpeed" },
		{ "runSpeed" },
		{ "lookXSpeed" },
		{ "lookYSpeed" }
	};
}

ObjectInputMotivator::ObjectInputMotivator()
{
}

ObjectInputMotivator::~ObjectInputMotivator() 
{
}

std::string ObjectInputMotivator::GetType() const
{
	return "ObjectInputMotivator";
}

std::string ObjectInputMotivator::GetWhat() const
{
	return std::string();
}								

bool ObjectInputMotivator::IsEnabled() const
{
	return m_enabled;
}

void ObjectInputMotivator::SetEnabled( bool enabled )
{
	m_enabled = enabled;
}

bool ObjectInputMotivator::Updateable() const
{
	return true;
}

bool ObjectInputMotivator::Renderable() const
{
	return false;
}

void ObjectInputMotivator::OnAttach( scene::Object * object )
{
	m_target = object;
}

void ObjectInputMotivator::OnDetach()
{
	m_target = nullptr;
}

void ObjectInputMotivator::OnInit()
{
}

void ObjectInputMotivator::OnStart()
{
}

void ObjectInputMotivator::OnUpdate( UpdateParams params )
{
	if ( ! m_target )
	{
		return;
	}

	if ( ! m_enabled )
	{
		return;
	}


	auto && runOnMotivation = m_motivations[ "runOn" ];

	// Digital movement...
	auto && strafeLeftMotivation = m_motivations[ "strafeleft" ];
	auto && strafeRightMotivation = m_motivations[ "straferight" ];
	auto && walkForwardMotivation = m_motivations[ "walkforward" ];
	auto && walkBackwardMotivation = m_motivations[ "walkbackward" ];	 

	// Analog movement...
	auto && walkXMotivation = m_motivations[ "walkx" ];
	auto && walkYMotivation = m_motivations[ "walkY" ];

	// Look...
	auto && lookXMotivation = m_motivations[ "lookX" ];
	auto && lookYMotivation = m_motivations[ "lookY" ];

	float speed = 1.0f;
	if ( runOnMotivation && runOnMotivation->IsTrue() )
	{
		speed = 3.0f;
	}

	if ( strafeLeftMotivation && strafeLeftMotivation->IsTrue() )
	{
		m_target->GetFrame().MoveBy( unify::V3< float >( -1, 0, 0 ) * params.renderInfo.GetDelta() * speed );
	}
	else if ( strafeRightMotivation && strafeRightMotivation->IsTrue() )
	{
		m_target->GetFrame().MoveBy( unify::V3< float >( 1, 0, 0 ) * params.renderInfo.GetDelta() * speed );
	}

	if ( walkForwardMotivation && walkForwardMotivation->IsTrue() )
	{
		m_target->GetFrame().MoveBy( unify::V3< float >( 0, 0, 1 ) * params.renderInfo.GetDelta() * speed );
	}
	else if ( walkBackwardMotivation && walkBackwardMotivation->IsTrue() )
	{
		m_target->GetFrame().MoveBy( unify::V3< float >( 0, 0, -1 ) * params.renderInfo.GetDelta() * speed );
	}

	if ( walkXMotivation && walkXMotivation->IsTrue() )
	{
		unify::V3< float > left = m_target->GetFrame().GetLeft();
		unify::Matrix matrix( unify::MatrixTranslate( left * walkXMotivation->GetValue() * params.renderInfo.GetDelta() * speed * 5.0f ) );
		m_target->GetFrame().PostMul( matrix );
	}

	if ( walkYMotivation && walkYMotivation->IsTrue() )
	{
		unify::V3< float > forward = m_target->GetFrame().GetForward();
		unify::Matrix matrix( unify::MatrixTranslate( forward * walkYMotivation->GetValue() * params.renderInfo.GetDelta() * speed * 5.0f ) );
		m_target->GetFrame().PostMul( matrix );
	}

	bool lookXMotivated = (lookXMotivation && lookXMotivation->IsTrue());
	bool lookYMotivated = (lookYMotivation && lookYMotivation->IsTrue());
	if ( lookXMotivated || lookYMotivated )
	{
		unify::Matrix matrix = unify::MatrixIdentity();

		if ( lookXMotivated )
		{
			float changeX = lookXMotivation->GetValue();
			matrix *= unify::MatrixRotationY( unify::AngleInRadians( params.renderInfo.GetDelta() * changeX * 0.4f ) );
		}

		if ( lookYMotivated )
		{
			float changeY = lookYMotivation->GetValue();
			matrix *= unify::MatrixRotationX( unify::AngleInRadians( params.renderInfo.GetDelta() * changeY * 0.4f ) );
		}

		m_target->GetFrame().PreMul( matrix );
	}

}

void ObjectInputMotivator::CollectGeometry( GeometryCache & cache, const unify::FrameLite * frame )
{
}

void ObjectInputMotivator::OnSuspend()
{
}

void ObjectInputMotivator::OnResume()
{
}

scene::IObjectComponent * ObjectInputMotivator::Duplicate()
{
	return nullptr; // Not supported.
}

void ObjectInputMotivator::Add( std::string motivation, IInputCondition::ptr condition )
{
	m_motivations[ motivation ] = condition;
}

int ObjectInputMotivator::GetValueCount() const
{
	return (int)g_ValuesList.size();
}

bool ObjectInputMotivator::ValueExists( std::string name ) const
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

std::string ObjectInputMotivator::GetValueName( int index ) const
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

int ObjectInputMotivator::FindValueIndex( std::string name ) const
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

bool ObjectInputMotivator::SetValue( int index, std::string value )
{
	switch ( index )
	{
	default:
		return false;
	case 0:
		m_enabled = unify::Cast< bool >( value );
		break;
	case 1:
		m_walkSpeed = unify::Cast< float >( value );
		break;
	case 2:
		m_runSpeed = unify::Cast< float >( value );
		break;
	case 3:
		m_lookXSpeed = unify::Cast< float >( value );
		break;
	case 4:
		m_lookYSpeed = unify::Cast< float >( value );
		break;
	}
	return true;
}

bool ObjectInputMotivator::SetValue( std::string name, std::string value )
{
	int index = FindValueIndex( name );
	return SetValue( index, value );
}

std::string ObjectInputMotivator::GetValue( int index ) const
{
	switch ( index )
	{
	default:
		return std::string();
	case 0:
		return unify::Cast< std::string >( m_enabled );
	case 1:
		return unify::Cast< std::string >( m_walkSpeed );
	case 2:
		return unify::Cast< std::string >( m_runSpeed );
	case 3:
		return unify::Cast< std::string >( m_lookXSpeed );
	case 4:
		return unify::Cast< std::string >( m_lookYSpeed );
	}
}
 
std::string ObjectInputMotivator::GetValue( std::string name ) const
{
	int index = FindValueIndex( name );
	return GetValue( index );
}
