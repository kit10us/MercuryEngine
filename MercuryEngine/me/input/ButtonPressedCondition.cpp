// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/input/ButtonPressedCondition.h>

using namespace me;
using namespace input;

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
	if ( ! data )
	{
		return false;
	}

	if ( data->type != InputType::Button )
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
