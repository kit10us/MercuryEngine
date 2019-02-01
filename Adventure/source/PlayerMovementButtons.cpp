// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <PlayerMovementButtons.h>
#include <MainScene.h>

using namespace me;
using namespace render;
using namespace scene;
using namespace object;

using namespace adv;

PlayerMovementButtons::PlayerMovementButtons( PlayerMovement::ptr playerMovement, MovementDirection direction )
	: m_playerMovement{ playerMovement }
	, m_direction{ direction }
{
}

bool PlayerMovementButtons::Perform( input::IInputDevice * device, input::IInputCondition * condition, unify::TimeDelta delta )
{
	if( !condition->IsTrue( device ) )
	{
		return true;
	}

	m_playerMovement->Perform( m_direction );

	return true;
}
