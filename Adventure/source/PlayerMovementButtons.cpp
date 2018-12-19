// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <PlayerMovementButtons.h>
#include <MainScene.h>

using namespace me;
using namespace render;
using namespace scene;
using namespace object;

PlayerMovementButtons::PlayerMovementButtons( MainScene & mainScene )
	: m_mainScene{ mainScene }
{
}

bool PlayerMovementButtons::Perform( input::IInputDevice * device, input::IInputCondition * condition, unify::TimeDelta delta )
{
	if( !condition->IsTrue( device ) )
	{
		return true;
	}

	m_mainScene.m_newMove = true;
			
	if( unify::string::StringIs( condition->GetName(), "Up" ) )
	{
		m_mainScene.m_move.z = 1.0f;
	}
	else if( unify::string::StringIs( condition->GetName(), "Down" ) )
	{
		m_mainScene.m_move.z = -1.0f;
	}
	else if( unify::string::StringIs( condition->GetName(), "Left" ) )
	{
		m_mainScene.m_move.x = -1.0f;
	}
	else if( unify::string::StringIs( condition->GetName(), "Right" ) )
	{
		m_mainScene.m_move.x = 1.0f;
	}

	// Normalize, so if we move in an angle, we aren't moving twice as fast.
	m_mainScene.m_move.Normalize();

	return true;
}
