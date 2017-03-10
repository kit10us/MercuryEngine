// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/input/ButtonCondition.h>

using namespace me;
using namespace input;

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
	if ( !data )
	{
		return false;
	}

	if ( data->type != InputType::Button )
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
