// Copyright (c) 2003 - 2017, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <memory>

namespace me
{
	namespace action
	{
		/// <summary>
		/// Does something; can be used with other objects as an interchangable action to have due to some sort of event.
		/// </summary>
		class IAction
		{
		public:
			typedef std::shared_ptr< IAction > ptr;

			virtual ~IAction() {}

			/// <summary>
			/// Perform the action.	Returns true on success.
			/// </summary>
			virtual bool Perform() = 0;
		};
	}
}
