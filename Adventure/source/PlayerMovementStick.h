// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/input/IInputCondition.h>
#include <me/input/IInputAction.h>
#include <me/object/Object.h>

class PlayerMovementStick : public me::input::IInputAction
{
	me::object::Object * m_object;
public:
	PlayerMovementStick( me::object::Object * object );

	bool Perform( me::input::IInputDevice * device, me::input::IInputCondition * condition, float delta );
};
