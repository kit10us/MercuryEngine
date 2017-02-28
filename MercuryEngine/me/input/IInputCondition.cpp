// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/input/IInputCondition.h>

using namespace me;
using namespace input;

InputCondition::InputCondition( IInputSource::ptr source, size_t subSource, size_t index )
	: m_source{ source }
	, m_subSource{ subSource }
	, m_index{ index }
{
}

InputCondition::~InputCondition()
{
}

IInputSource::ptr InputCondition::GetSource() const
{
	return m_source;
}

size_t InputCondition::GetIndex() const
{
	return m_index;
}

size_t InputCondition::GetSubSource() const
{
	return m_subSource;
}


ButtonCondition::ButtonCondition( IInputSource::ptr source, size_t subSource, std::string name, std::string condition )
	: InputCondition( source, subSource, source->InputIndex( subSource, name ) )
	, m_condition{ source->InputConditionIndex( subSource, GetIndex(), condition ) }
{
}

ButtonCondition::~ButtonCondition() 
{
}
										
size_t ButtonCondition::GetCondition() const
{
	return m_condition;
}

bool ButtonCondition::IsTrue() const
{
	return GetSource()->GetState( GetSubSource(), GetIndex(), GetCondition() ) == State::True;
}

float ButtonCondition::GetValue() const
{
	return IsTrue() ? 1.0f : 0.0f;
}


AnalogCondition::AnalogCondition( IInputSource::ptr source, size_t subSource, std::string name, float threshold, float cap )
	: InputCondition( source, subSource, source->InputIndex( subSource, name ) )
	, m_threshold( threshold )
	, m_cap( cap )
{
}

AnalogCondition::~AnalogCondition() 
{
}
										
bool AnalogCondition::IsTrue() const
{
	float value = GetSource()->GetValue( GetSubSource(), GetIndex() );
	return abs( value ) >= m_threshold;
}

float AnalogCondition::GetValue() const
{
	float value = GetSource()->GetValue( GetSubSource(), GetIndex() );
	if ( value < 0.0f )
	{
		return (value * -1.0f) < m_threshold ? 0.0f : (-1.0f * value) > m_cap ? (m_cap * -1.0f) : value;
	}
	else
	{
		return value < m_threshold ? 0.0f : value > m_cap ? m_cap : value;
	}
}
											

IInputCondition::ptr me::input::MakeButtonCondition( IInputSource::ptr source, size_t subSource, std::string name, std::string condition )
{
	return IInputCondition::ptr( new ButtonCondition( source, subSource, name, condition ) );
}

IInputCondition::ptr me::input::MakeAnalogCondition( IInputSource::ptr source, size_t subSource, std::string name, float threshold, float cap )
{
	return IInputCondition::ptr( new AnalogCondition( source, subSource, name, threshold, cap ) );
}
