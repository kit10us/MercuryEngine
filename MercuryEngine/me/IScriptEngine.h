// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/IGameComponent.h>
#include <me/scene/IObjectComponent.h>
#include <unify/Path.h>

namespace me
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
	class IScriptEngine : public IGameComponent
	{
	public:
		virtual ~IScriptEngine() {}

		virtual ExecuteResult ExecuteString( std::string line ) = 0;
			
		virtual ExecuteResult ExecuteFile( unify::Path path ) = 0;

		//virtual scene::IObjectComponent::ptr LoadModule( unify::Path path ) = 0;
	};
}