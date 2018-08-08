// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/action/IAction.h>
#include <list>

namespace me
{
	namespace action
	{
		/// <summary>
		/// Performs a list of actions. Stops on the first failuire.
		/// </summary>
		class ActionList : public IAction
		{
		public:
			ActionList();

			void AddAction( IAction::ptr action );

		public: // IAction...
			bool Perform() override;

		private:
			std::list< IAction::ptr > m_actions;
		};
	}
}
