// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/action/IAction.h>
#include <me/object/action/IObjectAction.h>

namespace me
{
	namespace action
	{
		/// <summary>
		/// Performs an Object Action.
		/// </summary>
		class ObjectAction : public IAction
		{
			object::Object* m_object;
			object::action::IObjectAction::ptr m_action;

		public:
			ObjectAction( object::Object* object, object::action::IObjectAction::ptr action );

		public: // IAction...
			bool Perform() override;
		};
	}
}
