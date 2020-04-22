// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/action/IAction.h>
#include <list>

namespace me::action
{
	/// <summary>
	/// Performs a list of actions. Stops on the first failuire.
	/// </summary>
	class ActionList : public IAction
	{
		std::list< IAction::ptr > m_actions;

	public:
		ActionList();

		/// <summary>
		/// Add an action to this action list.
		/// </summary>
		void AddAction( IAction::ptr action );

	public: // IAction...
		bool Perform() override;
	};
}