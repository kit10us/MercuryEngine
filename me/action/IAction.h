// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <memory>

namespace me::action
{
	/// <summary>
	/// Does something; can be used with other objects as an interchangable action to have due to some sort of event.
	/// </summary>
	class IAction
	{
	public:
		typedef std::shared_ptr< IAction > ptr;

		virtual ~IAction() = default;

		/// <summary>
		/// Perform the action.	Returns true on success.
		/// </summary>
		virtual bool Perform() = 0;
	};
}