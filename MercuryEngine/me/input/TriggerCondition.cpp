// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/input/TriggerCondition.h>

using namespace me;
using namespace input;

TriggerCondition::TriggerCondition( size_t subSource, std::string name, float threshold, float cap )
	: InputCondition( subSource, name )
	, m_threshold{ threshold }
	, m_cap{ cap }
{
}

TriggerCondition::~TriggerCondition() 
{
}
										
bool TriggerCondition::IsTrue( IInputDevice* device ) const
{
	IData::ptr data = device->GetInputData( GetSubSource(), GetName() );
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

unify::V3< float > TriggerCondition::GetValue( IInputDevice* device ) const
{
	IData::ptr data = device->GetInputData( GetSubSource(), GetName() );
	if ( !data )
	{
		return unify::V3< float >(0.0f);
	}

	if ( data->type != InputType::Trigger )
	{
		throw std::exception( "TriggerCondition is invalid!" );
	}

	TriggerData * triggerData = reinterpret_cast<TriggerData *>(data.get());
	return unify::V3< float >( (abs(triggerData->value) >= m_threshold && abs(triggerData->value) <= m_cap ) ? triggerData->value : 0.0f );
}
