// Copyright (c) 2003 - 2017, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/action/IAction.h>
#include <list>

namespace me
{
	namespace action
	{
		/// <summary>
		/// Ignores an actions return result (forces a true result).
		/// </summary>
		class IgnoreReturn : public IAction
		{
		public:
			IgnoreReturn( IAction::ptr action );

		public: // IAction...
			bool Perform() override;

		private:
			IAction::ptr m_action;
		};
	}
}
