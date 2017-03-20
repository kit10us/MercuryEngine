// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/input/IInputSource.h>
#include <list>

namespace me
{
	namespace input
	{
		/// <summary>
		/// Adds common features, so that any other input can inherit these features without having to re-implemenet them.
		/// </summary>
		class InputSource : public IInputSource
		{
			struct Event
			{
				unify::Owner::weak_ptr owner;
				IInputCondition::ptr condition;
				IInputAction::ptr action;
			};

		public:
			InputSource();
			~InputSource();

			void AddEvent( unify::Owner::ptr owner, IInputCondition::ptr condition, IInputAction::ptr action );
			void ClearEvents();

			int HandleEvents();

		private:
			std::list< Event > m_events;
		};
	}
}