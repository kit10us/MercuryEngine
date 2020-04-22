// Copyright (c) 2003 - 2011, Kit10 Studios LLC
// All Rights Reserved

#include <me/debug/DefaultDebug.h>
#include <me/debug/DefaultErrorHandler.h>
#include <me/exception/Handled.h>
#include <unify/Exception.h>
#include <cassert>

using namespace me::debug;

DefaultDebug::DefaultDebug()
{	m_fileLogger.reset( new FileLoggerListener{ unify::Path{ "default.log" } } );
	m_logger.AttachListener( m_fileLogger );
}

DefaultDebug::~DefaultDebug()
{
	m_fileLogger.reset();
}

void DefaultDebug::SetDebug( bool debug )
{
	m_isDebug = debug;
}

bool DefaultDebug::IsDebug()
{
	return m_isDebug;
}

void DefaultDebug::SetLogPath( unify::Path path )
{
	m_fileLogger->SetLogPath( path );
	GetLogger()->Log( "Change file logger path to \"" + path.ToString() + "\"." );
}

void DefaultDebug::SetErrorAsCritical( ErrorLevel level, bool isCritical )
{
	m_errorAsCritical[(size_t)level] = isCritical;
}

bool DefaultDebug::GetErrorAsCritical( ErrorLevel level ) const
{
	return m_errorAsCritical[(size_t)level];
}

bool DefaultDebug::Assert( bool assertion ) const
{
	bool debug =
#ifdef _DEBUG
		debug = true;
#else
		debug = false;
#endif

	assert( assertion );
	// TODO: Proper assertion based on debug or release.
	return false;
}

void DefaultDebug::SetErrorHandler( IErrorHandler::ptr errorHandler )
{
	{
		m_errorHandler = errorHandler;
	}
	int i; i = 0;
}

ReportErrorResult DefaultDebug::ReportError( ErrorLevel level, std::string error, bool canContinue, bool canRetry )
{
	bool isCritical;
	switch ( level )
	{
		using enum me::debug::ErrorLevel;
	case Engine:
		m_criticalErrors.push_back( "Engine Failure: \"" + error + "\"" );
		GetLogger()->Log( "Engine Failure! \"" + error + "\"" );
		break;

	case ErrorLevel::Extension:
		m_criticalErrors.push_back( "Extension Failure: \"" + error + "\"" );
		GetLogger()->Log( "Extension Failure: \"" + error + "\"" );
		break;

	case ErrorLevel::Critical:
		GetLogger()->Log( "Critical Failure: \"" + error + "\"" );
		{
			m_criticalErrors.push_back( "Critical Failure: \"" + error + "\"" );
		}
		break;

	case ErrorLevel::Failure:
		GetLogger()->Log( "General Failure: \"" + error + "\"" );
		isCritical = m_failuresAsCritial;
		break;

	case ErrorLevel::Warning:
		isCritical = m_warningAsCritical;
		GetLogger()->Log( "Warning: " + error );
		break;
	}

	std::string errorText = ErrorLevelToString( level ) + ": \"" + error + "\"";
	GetLogger()->Log( errorText );

	if ( isCritical )
	{
		m_criticalErrors.push_back( errorText );
	}

	return m_errorHandler->ReportError( level, error, canContinue, canRetry );
}

void DefaultDebug::Try( std::function< void() > func, ErrorLevel level, bool canContinue, bool canRetry )
{
	ReportErrorResult result {};
	bool failed = false;
	std::exception exception {};
	
	do
	{
		try
		{
			func();
		}
		catch( me::exception::Handled & )
		{
			throw;
		}
		catch( std::exception & ex )
		{
			failed = true;
			exception = ex;
			result = ReportErrorResult::Abort;

			// Try to report the error as the report error handler my throw for an abort.
			try
			{
				result = ReportError( level, ex.what(), canContinue, canRetry );
			}
			catch( ... )
			{
			}
		}
	} while ( failed && result == ReportErrorResult::Retry );

	if ( failed && result == ReportErrorResult::Abort )
	{
		throw exception::Handled( exception.what() );
	}
}

bool DefaultDebug::HadCriticalError() const
{
	return m_criticalErrors.size() != 0;
}

void DefaultDebug::DebugTimeStampBegin( std::string name )
{
	if( !IsDebug() ) return;

	using namespace std::chrono;

	auto now = high_resolution_clock::now();
	
	m_timeStamps[name].start = now;
	m_timeStamps[name].end = now;
}

void DefaultDebug::DebugTimeStampEnd( std::string name )
{
	if( !IsDebug() ) return;

	auto itr = m_timeStamps.find( name );
	if( itr == m_timeStamps.end() )
	{
		return;
	}

	using namespace std::chrono;

	auto now = high_resolution_clock::now();

	m_timeStamps[name].end = now;
}

float DefaultDebug::DebugGetTimeStamp( std::string name )
{
	auto itr = m_timeStamps.find( name );
	if( itr == m_timeStamps.end() )
	{
		return 0.0f;
	}
	
	using namespace std::chrono;

	auto durationS = duration_cast< duration< float > >( itr->second.end - itr->second.start );

	return durationS.count();
}

kit::debug::ILogger* DefaultDebug::GetLogger()
{
	return &m_logger;
}
