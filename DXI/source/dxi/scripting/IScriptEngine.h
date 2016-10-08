// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/scripting/IModule.h>
#include <dxi/scripting/ExecuteResult.h>
#include <unify/Path.h>

namespace dxi
{
	namespace scripting
	{
		/// Provides common script engine features.
		/// </summary>
		class IScriptEngine
		{
		public:
			virtual ~IScriptEngine() {}

			virtual ExecuteResult ExecuteString( std::string line ) = 0;
			
			virtual ExecuteResult ExecuteFile( unify::Path path ) = 0;

			virtual IModule::ptr LoadModule( unify::Path path ) = 0;
		};
	}
}