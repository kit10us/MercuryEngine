// Copyright (c) 2003 - 2011, Kit10 Studios LLC
// All Rights Reserved

#include <me/debug/DefaultErrorHandler.h>
#include <me/exception/FileNotFound.h>

using namespace me::debug;

DefaultErrorHandler::DefaultErrorHandler( IDebug* debug )
	: m_debug{ debug }
{
}

DefaultErrorHandler::~DefaultErrorHandler()
{
}

ReportErrorResult DefaultErrorHandler::ReportError( ErrorLevel level, std::string error, bool canContinue, bool canRetry )
{
	if ( m_debug->GetErrorAsCritical( level ) )
	{
		throw unify::Exception( ErrorLevelToString( level ) + ": " + error );
	}
	else
	{
		return ReportErrorResult::Continue;
	}
}
