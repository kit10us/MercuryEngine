// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <MELuaDLL.h>
#include <lua.hpp>
#include <me/action/IAction.h>

namespace melua
{
	/// <summary>
	/// InputAction which triggers an IAction.
	/// </summary>
	class FunctionAction : public me::action::IAction
	{
	public:
		FunctionAction( int refIndex );

		/// <summary>
		/// Perform the action.	Returns true on success.
		/// </summary>
		bool Perform() override;

	private:
		int m_refIndex;
	};
}