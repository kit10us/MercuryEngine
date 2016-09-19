// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/Path.h>

namespace dxi
{
	namespace scripting
	{
		enum class ExecuteResult
		{
			Pass,
			Fail,
			Pending
		};

		/// <summary>
		/// Provides common script engine features.
		/// </summary>
		class IScriptEngine
		{
		public:
			virtual ~IScriptEngine() {}

			virtual ExecuteResult ExecuteString( std::string line ) = 0;
			
			virtual ExecuteResult ExecuteFile( unify::Path path ) = 0;
		};
	}
}