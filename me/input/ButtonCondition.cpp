// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/input/ButtonCondition.h>

using namespace me;
using namespace input;

ButtonCondition::ButtonCondition( size_t subSource, std::string name, bool down )
	: InputCondition( subSource, name )
	, m_down{ down }
{
}

ButtonCondition::~ButtonCondition() 
{
}
										
bool ButtonCondition::IsTrue( IInputDevice* device ) const
{
	IData::ptr data = device->GetInputData( GetSubSource(), GetName() );
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

unify::V3< float > ButtonCondition::GetValue( IInputDevice* device ) const
{
	return{ IsTrue( device ) ? 1.0f : 0.0f };
}
