// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/action/IAction.h>

namespace me
{
	namespace action
	{
		/// <summary>
		/// Repeats an action n times.
		/// </summary>
		class RepeatAction : public IAction
		{
		public:
			RepeatAction( IAction::ptr action, size_t times );

		public: // IAction...
			bool Perform() override;

		private:
			IAction::ptr m_action;
			size_t m_times;
		};
	}
}
