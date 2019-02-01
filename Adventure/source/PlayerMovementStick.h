// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/input/IInputCondition.h>
#include <me/input/IInputAction.h>
#include <adv/PlayerMovement.h>

class MainScene;

class PlayerMovementStick : public me::input::IInputAction
{
	adv::PlayerMovement::ptr m_playerMovement;
public:
	PlayerMovementStick( adv::PlayerMovement::ptr playerMovement );

	bool Perform( me::input::IInputDevice * device, me::input::IInputCondition * condition, unify::TimeDelta delta );
};
