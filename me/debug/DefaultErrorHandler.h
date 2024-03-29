// Copyright (c) 2003 - 2011, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/debug/IDebug.h>

namespace me::debug
{
	class DefaultErrorHandler : public IErrorHandler
	{
		IDebug * m_debug;
	public:
		DefaultErrorHandler( IDebug* debug );
		~DefaultErrorHandler();

		/// <summary>
		/// Report an error to the error handler.
		/// </summary>
		ReportErrorResult ReportError( ErrorLevel level, std::string error, bool canContinue, bool canRetry ) override;
	};
}