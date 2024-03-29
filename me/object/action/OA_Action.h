// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/object/action/IObjectAction.h>
#include <me/action/IAction.h>
#include <me/dyna/IDynaPosition.h>
#include <list>

namespace me
{
	namespace object
	{
		namespace action
		{
			/// <summary>
			/// Perform an Action (ignores our object).
			/// </summary>
			class Action : public IObjectAction
			{
			public:
				Action( me::action::IAction::ptr action );

				bool Perform( Object* object, unify::TimeDelta delta ) override;

			private:
				me::action::IAction::ptr m_action;
			};
		}
	}
}