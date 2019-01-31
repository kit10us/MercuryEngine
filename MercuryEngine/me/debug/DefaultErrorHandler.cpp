// Copyright (c) 2003 - 2011, Evil Quail LLC
// All Rights Reserved

#include <me/debug/DefaultErrorHandler.h>
#include <me/exception/Exception.h>
#include <me/exception/FileNotFound.h>

using namespace me;
using namespace debug;

DefaultErrorHandler::DefaultErrorHandler( IDebug* debug )
	: m_debug{ debug }
{
}

DefaultErrorHandler::~DefaultErrorHandler()
{
}

ReportErrorResult DefaultErrorHandler::ReportError( me::ErrorLevel level, std::string source, std::string error, bool canContinue, bool canRetry )
{
	switch( level )
	{
	case me::ErrorLevel::Critical:
		throw unify::Exception( "Critical Failure (" + source + "): " + error );
	case me::ErrorLevel::Failure:
		if( m_debug->GetFailureAsCritical() || ! canContinue )
		{
			throw unify::Exception( "Failure (" + source + "): " + error );
		}
		break;
	case me::ErrorLevel::Warning:
		break;
	}
	
	return ReportErrorResult::Continue;
}
