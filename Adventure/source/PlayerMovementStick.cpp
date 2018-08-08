// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <PlayerMovementStick.h>
#include <MainScene.h>

using namespace me;
using namespace render;
using namespace scene;
using namespace object;

PlayerMovementStick::PlayerMovementStick( MainScene & mainScene )
	: m_mainScene{ mainScene }
{
}

bool PlayerMovementStick::Perform( input::IInputDevice * device, input::IInputCondition * condition, float delta )
{
	float speed = 6.0f;

	unify::V3< float > stick = condition->GetValue( device ).xzy();

	// Normalize, so if we move in an angle, we aren't moving twice as fast.
	float length = stick.Normalize();
	if( length > 0.2f )
	{
		m_mainScene.m_newMove = true;
		m_mainScene.m_move = stick;
	}
		
	return true;
}
