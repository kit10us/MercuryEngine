// Copyright (c) 2003 - 2011, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <unify/Exception.h>
#include <string>

namespace me::debug
{

	enum class ErrorLevel
	{
		Engine,		// We have a failure within the engine itself.
		Extension,	// We have a failure within an extension.
		Critical,	// System is left unstable, we should likely shutdown immediately.
		Failure,	// We have a failure, the module that reported it is likely corrupted.
		Warning		// Error might not be important - state is unknown.
	};

	const size_t ErrorLevelCount = 5;

	static std::string ErrorLevelToString( ErrorLevel level )
	{
		switch ( level )
		{
			using enum ErrorLevel;
		case Engine:
			return "Engine";
		case Extension:
			return "Extension";
		case Critical:
			return "Critical";
		case Failure:
			return "Failure";
		case Warning:
			return "Warning";
		default:
			throw unify::Exception( "Invalid error level!" );
		}
	}
}