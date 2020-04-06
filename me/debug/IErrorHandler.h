// Copyright (c) 2003 - 2011, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <memory>
#include <string>

namespace me
{
	namespace debug
	{
		enum class ErrorLevel
		{
			Engine,		// We have a failure within the engine itself.
			Extension,	// We have a failure within an extension.
			Critical,	// System is left unstable, we should likely shutdown immediately.
			Failure,	// We have a failure, the module that reported it is likely corrupted.
			Warning		// Error might not be important - state is unknown.
		};

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
			virtual ReportErrorResult ReportError( ErrorLevel level, std::string source, std::string error, bool canContinue = false, bool canRetry = false ) = 0;
		};
	}
}