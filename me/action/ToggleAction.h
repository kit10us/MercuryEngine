// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/action/IAction.h>
#include <vector>

namespace me::action
{
	/// <summary>
	/// Performs an action from a set, moving to the next action in the set every time.
	/// </summary>
	class ToggleAction : public IAction
	{
		std::vector< IAction::ptr > m_actions;
		size_t m_next;

	public:
		ToggleAction( std::initializer_list< IAction::ptr > actions );

	public: // IAction...
		bool Perform() override;
	};
}