// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/action/IAction.h>

namespace me::action
{
	/// <summary>
	/// Repeats an action n times.
	/// </summary>
	class RepeatAction : public IAction
	{
		IAction::ptr m_action;
		size_t m_times;

	public:
		RepeatAction( IAction::ptr action, size_t times );

	public: // IAction...
		bool Perform() override;
	};
}