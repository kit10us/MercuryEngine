// Copyright (c) 2003 - 2011, Kit10 Studios LLC
// All Rights Reserved

#include <me/debug/DefaultDebug.h>
#include <me/debug/DefaultErrorHandler.h>
#include <me/exception/Handled.h>
#include <unify/Exception.h>
#include <cassert>

using namespace me::debug;

DefaultDebug::DefaultDebug(unify::Path logPath, unify::Path filename )
	: m_fileLogger{ new FileLoggerListener{ logPath, filename } }
	, m_csvLogger{ new CSVLoggerListener{ logPath, filename } }
{
	m_logger.AttachListener( m_fileLogger );
	m_logger.AttachListener(m_csvLogger);
}

DefaultDebug::~DefaultDebug()
{
}

void DefaultDebug::SetDebug( bool debug )
{
	m_isDebug = debug;
}

bool DefaultDebug::IsDebug()
{
	return m_isDebug;
}

void DefaultDebug::SetLogDirectory( unify::Path directory )
{
	m_fileLogger->SetLogDirectory( directory );
	m_csvLogger->SetLogDirectory(directory);
}

void DefaultDebug::SetLogFilename(unify::Path filename)
{
	m_fileLogger->SetLogFilename( filename );
	m_csvLogger->SetLogFilename(filename);
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
	bool isCritical{};
	std::string errorText{};
	switch ( level )
	{
	case ErrorLevel::Engine:
	case ErrorLevel::Extension:
	case ErrorLevel::Critical:
		errorText = ErrorLevelToString(level) + " failure: \"" + error + "\"";
		isCritical = true;
		break;

	case ErrorLevel::Failure:
		errorText = ErrorLevelToString(level) + " failure: \"" + error + "\"";
		isCritical = m_failuresAsCritial;
		break;

	case ErrorLevel::Warning:
		errorText = ErrorLevelToString(level) + ": \"" + error + "\"";
		break;
	}

	GetLogger()->Log( errorText );

	if ( isCritical == true )
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
