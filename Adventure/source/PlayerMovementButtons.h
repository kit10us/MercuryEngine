// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/input/IInputAction.h>
#include <me/input/IInputCondition.h>
#include <adv/PlayerMovement.h>

namespace adv
{
	/// <summary>
	/// Player movement controlled through button presses.
	/// <summary>
	class PlayerMovementButtons : public me::input::IInputAction
	{
		PlayerMovement::ptr m_playerMovement;
		MovementDirection m_direction;
	public:
		PlayerMovementButtons( PlayerMovement::ptr playerMovement, MovementDirection direction );

		bool Perform( me::input::IInputDevice * device, me::input::IInputCondition * condition, unify::TimeDelta delta );
	};
}