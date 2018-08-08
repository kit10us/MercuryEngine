// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/input/ButtonPressedCondition.h>

using namespace me;
using namespace input;

ButtonPressedCondition::ButtonPressedCondition( size_t subSource, std::string name )
	: InputCondition( subSource, name )
{
}

ButtonPressedCondition::~ButtonPressedCondition() 
{
}
										
bool ButtonPressedCondition::IsTrue( IInputDevice* device ) const
{
	IData::ptr data = device->GetInputData( GetSubSource(), GetName() );
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

unify::V3< float > ButtonPressedCondition::GetValue( IInputDevice* device ) const
{
	return{ IsTrue( device ) ? 1.0f : 0.0f };
}
