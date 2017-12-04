// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/input/IInputAction.h>
#include <me/input/IInputCondition.h>
#include <me/object/Object.h>

class PlayerMovementButtons : public me::input::IInputAction
{
	me::object::Object * m_object;
public:
	PlayerMovementButtons( me::object::Object * object );

	bool Perform( me::input::IInputDevice * device, me::input::IInputCondition * condition, float delta );
};
