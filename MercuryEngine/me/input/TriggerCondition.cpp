// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/input/TriggerCondition.h>

using namespace me;
using namespace input;

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
	if ( !data )
	{
		return false;
	}

	if ( data->type != InputType::Trigger )
	{
		throw std::exception( "TriggerCondition is invalid!" );
	}

	TriggerData * triggerData = reinterpret_cast<TriggerData *>(data.get());
	return (abs(triggerData->value) >= m_threshold && abs(triggerData->value) <= m_cap );
}

float TriggerCondition::GetValue() const
{
	IData::ptr data = GetSource()->GetInputData( GetSubSource(), GetIndex() );
	if ( !data )
	{
		return false;
	}

	if ( data->type != InputType::Trigger )
	{
		throw std::exception( "TriggerCondition is invalid!" );
	}

	TriggerData * triggerData = reinterpret_cast<TriggerData *>(data.get());
	return (abs(triggerData->value) >= m_threshold && abs(triggerData->value) <= m_cap ) ? triggerData->value : 0.0f;
}
