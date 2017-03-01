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




ButtonCondition::ButtonCondition( IInputSource::ptr source, size_t subSource, std::string name, bool down )
	: InputCondition( source, subSource, source->InputIndex( subSource, name ) )
	, m_down{ down }
{
}

ButtonCondition::~ButtonCondition() 
{
}
										
bool ButtonCondition::IsTrue() const
{
	IData::ptr data = GetSource()->GetInputData( GetSubSource(), GetIndex() );
	if ( ! data || data->type != InputType::Button )
	{
		throw std::exception( "ButtonCondition is invalid!" );
	}

	ButtonData * buttonData = reinterpret_cast<ButtonData *>(data.get()); 
	return buttonData->down == m_down;
}

float ButtonCondition::GetValue() const
{
	return IsTrue() ? 1.0f : 0.0f;
}




ButtonPressedCondition::ButtonPressedCondition( IInputSource::ptr source, size_t subSource, std::string name )
	: InputCondition( source, subSource, source->InputIndex( subSource, name ) )
{
}

ButtonPressedCondition::~ButtonPressedCondition() 
{
}
										
bool ButtonPressedCondition::IsTrue() const
{
	IData::ptr data = GetSource()->GetInputData( GetSubSource(), GetIndex() );
	if ( ! data || data->type != InputType::Button )
	{
		throw std::exception( "ButtonPressedCondition is invalid!" );
	}

	ButtonData * buttonData = reinterpret_cast<ButtonData *>(data.get());
	return buttonData->pressed;
}

float ButtonPressedCondition::GetValue() const
{
	return IsTrue() ? 1.0f : 0.0f;
}


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

StickCondition::StickCondition( IInputSource::ptr source, size_t subSource, std::string name, StickAxis axis, float threshold, float cap )
	: InputCondition( source, subSource, source->InputIndex( subSource, name ) )
	, m_axis{ axis }
	, m_threshold{ threshold }
	, m_cap{ cap }
{
}

StickCondition::~StickCondition() 
{
}
										
bool StickCondition::IsTrue() const
{
	IData::ptr data = GetSource()->GetInputData( GetSubSource(), GetIndex() );
	if ( ! data || data->type != InputType::Stick )
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
	return abs(value) >= m_threshold && abs(value) <= m_cap;
}

float StickCondition::GetValue() const
{
	IData::ptr data = GetSource()->GetInputData( GetSubSource(), GetIndex() );
	if ( ! data || data->type != InputType::Stick )
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
	return (abs(value) >= m_threshold && abs(value) <= m_cap) ? value : 0.0f;
}





TriggerCondition::TriggerCondition( IInputSource::ptr source, size_t subSource, std::string name, float threshold, float cap )
	: InputCondition( source, subSource, source->InputIndex( subSource, name ) )
	, m_threshold{ threshold }
	, m_cap{ cap }
{
}

TriggerCondition::~TriggerCondition() 
{
}
										
bool TriggerCondition::IsTrue() const
{
	IData::ptr data = GetSource()->GetInputData( GetSubSource(), GetIndex() );
	if ( ! data || data->type != InputType::Trigger )
	{
		throw std::exception( "TriggerCondition is invalid!" );
	}

	TriggerData * triggerData = reinterpret_cast<TriggerData *>(data.get());
	return (abs(triggerData->value) >= m_threshold && abs(triggerData->value) <= m_cap );
}

float TriggerCondition::GetValue() const
{
	IData::ptr data = GetSource()->GetInputData( GetSubSource(), GetIndex() );
	if ( ! data || data->type != InputType::Trigger )
	{
		throw std::exception( "TriggerCondition is invalid!" );
	}

	TriggerData * triggerData = reinterpret_cast<TriggerData *>(data.get());
	return (abs(triggerData->value) >= m_threshold && abs(triggerData->value) <= m_cap ) ? triggerData->value : 0.0f;
}
