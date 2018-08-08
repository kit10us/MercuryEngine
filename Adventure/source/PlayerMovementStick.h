// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/input/IInputCondition.h>
#include <me/input/IInputAction.h>
#include <me/object/Object.h>

class MainScene;

class PlayerMovementStick : public me::input::IInputAction
{
	me::object::Object * m_object;
	MainScene & m_mainScene;
public:
	PlayerMovementStick( MainScene & mainScene );

	bool Perform( me::input::IInputDevice * device, me::input::IInputCondition * condition, float delta );
};
