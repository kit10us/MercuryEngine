// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/action/IAction.h>

namespace me::action
{
	/// <summary>
	/// Ignores an actions return result (forces a true result).
	/// </summary>
	class IgnoreReturn : public IAction
	{
		IAction::ptr m_action;

	public:
		IgnoreReturn( IAction::ptr action );

	public: // IAction...
		bool Perform() override;
	};
}