// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/input/IInputCondition.h>
#include <me/input/IInputAction.h>
#include <me/object/action/IObjectAction.h>
#include <me/object/Object.h>
#include <memory>

namespace me
{
	namespace input
	{
		namespace action
		{
			/// <summary>
			/// InputAction which triggers an IObjectAction.
			/// </summary>
			class ObjectAction : public IInputAction
			{
			public:
				ObjectAction( object::Object * object, me::object::action::IObjectAction::ptr action );

				/// <summary>
				/// Perform the action.	Returns true on success.
				/// </summary>
				bool Perform( IInputDevice * device, IInputCondition* input, float delta ) override;

			private:
				me::object::Object * m_object;
				me::object::action::IObjectAction::ptr m_action;
			};
		}
	}
}