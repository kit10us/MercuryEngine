// Copyright (c) 2003 - 2017, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/action/IAction.h>
#include <vector>

namespace me
{
	namespace action
	{
		/// <summary>
		/// Performs an action from a set, moving to the next action in the set every time.
		/// </summary>
		class ToggleAction : public IAction
		{
		public:
			ToggleAction( std::initializer_list< IAction::ptr > actions );

		public: // IAction...
			bool Perform() override;

		private:
			std::vector< IAction::ptr > m_actions;
			size_t m_next;
		};
	}
}
