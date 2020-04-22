// Copyright (c) 2003 - 2011, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/debug/ErrorLevel.h>
#include <unify/Exception.h>
#include <memory>
#include <string>

namespace me::debug
{
	/// <summary>
	/// Conveys the result of how an error was handled.
	/// </summary>
	enum class ReportErrorResult
	{
		Continue,
		Retry,
		Abort
	};

	/// <summary>
	/// Handlers errors, allowing custom functionality.
	/// </summary>
	class IErrorHandler
	{
	public:
		typedef std::shared_ptr< IErrorHandler > ptr;

		~IErrorHandler() {}

		/// <summary>
		/// Report an error to the error handler.
		/// </summary>
		virtual ReportErrorResult ReportError( ErrorLevel level, std::string error, bool canContinue = false, bool canRetry = false ) = 0;
	};
}