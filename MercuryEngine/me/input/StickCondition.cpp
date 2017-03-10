// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/input/StickCondition.h>

using namespace me;
using namespace input;

StickAxis input::StickAxisFromString( std::string axis )
{
	if ( unify::StringIs( axis, "x" ) )
	{
		return StickAxis::X;
	}
	else if ( unify::StringIs( axis, "y" ) )
	{
		return StickAxis::Y;
	}
	else if ( unify::StringIs( axis, "z" ) )
	{
		return StickAxis::Z;
	}
	throw std::exception( "Invalid StickAxis!" );
}

StickCondition::StickCondition( IInputSource::ptr source, size_t subSource, std::string name, StickAxis axis, float cap_low, float threshhold_low, float threshhold_high, float cap_high )
	: InputCondition( source, subSource, source->InputIndex( subSource, name ) )
	, m_axis{ axis }
	, m_threshhold_low{ threshhold_low }
	, m_threshhold_high{ threshhold_high }
	, m_cap_low{ cap_low }
	, m_cap_high{ cap_high }
{
}

StickCondition::~StickCondition() 
{
}
										
bool StickCondition::IsTrue() const
{
	IData::ptr data = GetSource()->GetInputData( GetSubSource(), GetIndex() );
	if ( ! data )
	{
		return false;
	}
	
	if( data->type != InputType::Stick )
	{
		throw std::exception( "StickCondition is invalid!" );
	}

	StickData * stickData = reinterpret_cast<StickData *>(data.get());
	float value;
	switch ( m_axis )
	{
	case StickAxis::X:
		value = stickData->axis.x;
		break;
	case StickAxis::Y:
		value = stickData->axis.y;
		break;
	case StickAxis::Z:
		value = stickData->axis.z;
		break;
	}
	return (value >= m_cap_low && value <= m_threshhold_low) || (value >= m_threshhold_high && value <= m_cap_high);
}

float StickCondition::GetValue() const
{
	IData::ptr data = GetSource()->GetInputData( GetSubSource(), GetIndex() );
	if ( !data )
	{
		return false;
	}

	if ( data->type != InputType::Stick )
	{
		throw std::exception( "StickCondition is invalid!" );
	}

	StickData * stickData = reinterpret_cast<StickData *>(data.get());
	float value;
	switch ( m_axis )
	{
	case StickAxis::X:
		value = stickData->axis.x;
		break;
	case StickAxis::Y:
		value = stickData->axis.y;
		break;
	case StickAxis::Z:
		value = stickData->axis.z;
		break;
	}
	
	if ( !((value >= m_cap_low && value <= m_threshhold_low) || (value >= m_threshhold_high && value <= m_cap_high)) )
	{
		return 0.0f;
	}	
	else
	{
		return value;
	}
}																			 
