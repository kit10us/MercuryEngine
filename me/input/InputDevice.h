// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/input/IInputDevice.h>
#include <list>

namespace me
{
	namespace input
	{
		/// <summary>
		/// Adds common features, so that any other input can inherit these features without having to re-implemenet them.
		/// </summary>
		class InputDevice : public IInputDevice
		{
			struct Event
			{
				unify::Owner::weak_ptr owner;
				IInputCondition::ptr condition;
				IInputAction::ptr action;
			};

		public:
			InputDevice();
			~InputDevice();

			void AddEvent( unify::Owner::ptr owner, IInputCondition::ptr condition, IInputAction::ptr action );
			void ClearEvents();

			int HandleEvents( unify::TimeDelta delta );

		private:
			std::list< Event > m_events;
		};
	}
}