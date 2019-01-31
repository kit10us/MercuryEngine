// Copyright (c) 2003 - 2011, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/debug/IDebug.h>

namespace me
{
	namespace debug
	{
		class DefaultErrorHandler : public IErrorHandler
		{
			IDebug::ptr m_debug;
		public:
			DefaultErrorHandler( IDebug* debug );
			~DefaultErrorHandler();

			/// <summary>
			/// Report an error to the error handler.
			/// </summary>
			ReportErrorResult ReportError( me::ErrorLevel level, std::string source, std::string error, bool canContinue, bool canRetry ) override;
		};
	}
}