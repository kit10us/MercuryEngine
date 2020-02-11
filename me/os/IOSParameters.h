// Copyright (c) 2003 - 2011, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <unify/Path.h>
#include <string>

namespace me
{
	namespace os
	{
		class IOSParameters
		{
		public:
			virtual ~IOSParameters() {}

			virtual unify::Path GetProgramPath() const = 0;
			virtual unify::Path GetRunPath() const = 0;
			virtual size_t GetArgumentCount() const = 0;
			virtual std::string GetArgument( size_t i ) const = 0;

		};
	}
}