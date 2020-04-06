// Copyright (c) 2003 - 2011, Kit10 Studios LLC
// All Rights Reserved

#include <me/debug/DefaultErrorHandler.h>
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

ReportErrorResult DefaultErrorHandler::ReportError( ErrorLevel level, std::string source, std::string error, bool canContinue, bool canRetry )
{
	switch( level )
	{
	case ErrorLevel::Engine:
		throw unify::Exception( "Engine Failure (" + source + "): " + error );
		break;

	case ErrorLevel::Extension:
		throw unify::Exception( "Extension Failure (" + source + "): " + error );
		break;

	case ErrorLevel::Critical:
		throw unify::Exception( "Critical Failure (" + source + "): " + error );
		break;

	case ErrorLevel::Failure:
		if( m_debug->GetFailureAsCritical() || ! canContinue )
		{
			throw unify::Exception( "Failure (" + source + "): " + error );
		}
		break;

	case ErrorLevel::Warning:
		break;
	}
	
	return ReportErrorResult::Continue;
}
