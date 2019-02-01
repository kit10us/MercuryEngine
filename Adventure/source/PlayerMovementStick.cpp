// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <PlayerMovementStick.h>
#include <MainScene.h>

using namespace me;
using namespace render;
using namespace scene;
using namespace object;

PlayerMovementStick::PlayerMovementStick( adv::PlayerMovement::ptr playerMovement )
	: m_playerMovement{ playerMovement }
{
}

bool PlayerMovementStick::Perform( input::IInputDevice * device, input::IInputCondition * condition, unify::TimeDelta delta )
{
	unify::V3< float > stick = condition->GetValue( device ).xzy();
	m_playerMovement->Perform( stick );
		
	return true;
}
