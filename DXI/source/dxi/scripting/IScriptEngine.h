// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/core/IGameComponent.h>
#include <dxi/scene/IObjectComponent.h>
#include <dxi/scripting/ExecuteResult.h>
#include <unify/Path.h>

namespace dxi
{
	namespace scripting
	{
		/// Provides common script engine features.
		/// </summary>
		class IScriptEngine : public core::IGameComponent
		{
		public:
			virtual ~IScriptEngine() {}

			virtual ExecuteResult ExecuteString( std::string line ) = 0;
			
			virtual ExecuteResult ExecuteFile( unify::Path path ) = 0;

			virtual scene::IObjectComponent::ptr LoadModule( unify::Path path ) = 0;
		};
	}
}