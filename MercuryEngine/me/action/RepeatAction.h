// Copyright (c) 2003 - 2017, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/action/IAction.h>

namespace me
{
	namespace action
	{
		/// <summary>
		/// Ignores an actions return result (forces a true result).
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
