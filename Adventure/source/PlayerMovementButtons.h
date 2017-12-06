// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/input/IInputAction.h>
#include <me/input/IInputCondition.h>
#include <me/object/Object.h>

class MainScene;

class PlayerMovementButtons : public me::input::IInputAction
{
	me::object::Object * m_object;
	MainScene & m_mainScene;
public:
	PlayerMovementButtons( MainScene & mainScene );

	bool Perform( me::input::IInputDevice * device, me::input::IInputCondition * condition, float delta );
};
