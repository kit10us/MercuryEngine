// Copyright (c) 2003 - 2011, Kit10 Studios LLC
// All Rights Reserved

#include <me/debug/DefaultDebug.h>
#include <me/debug/Block.h>
#include <me/debug/DefaultErrorHandler.h>
#include <me/exception/Handled.h>
#include <unify/Exception.h>
#include <cassert>

using namespace me;
using namespace debug;

DefaultDebug::DefaultDebug()
	: m_failuresAsCritial{ true }
	, m_prependSectionText{ "[" }
	, m_appendSectionText{ "]" }
	, m_prependLineText{ " " }
	, m_appendLineText{ "\n" }
	, m_errorHandler{ IErrorHandler::ptr{ new DefaultErrorHandler( this ) } }
	, m_isDebug
#ifdef _DEBUG
		{ true }
#else
		{ false }
#endif
	, m_fileLogger{ new FileLoggerListener{ unify::Path{ "default.log" } } }
{
	AttachLogListener( m_fileLogger );
}

DefaultDebug::~DefaultDebug()
{
	DetachLogListener( m_fileLogger );
	delete m_fileLogger;
	m_fileLogger = {};
}

void DefaultDebug::SetDebug( bool debug )
{
	m_isDebug = debug;
}

bool DefaultDebug::IsDebug()
{
	return m_isDebug;
}

void DefaultDebug::SetLogFile( unify::Path logFile )
{
	Block defaultDebugBlock( this, "DefaultDebug" );
	Block block( this, "SetLogFile" );
	m_fileLogger->SetLogFile( logFile );
	LogLine( "changed to: " + logFile.ToString() );
}

void DefaultDebug::SetFailureAsCritical( bool faiureAsCritical )
{
	m_failuresAsCritial = faiureAsCritical;
}

bool DefaultDebug::GetFailureAsCritical() const
{
	return m_failuresAsCritial;
}

void DefaultDebug::SetAppendSectionText( std::string text )
{
	m_prependSectionText = text;
}

std::string DefaultDebug::GetAppendSectionText() const
{
	return m_appendSectionText;
}

void DefaultDebug::SetPrependSectionText( std::string text )
{
	m_prependSectionText = text;
}

std::string DefaultDebug::GetPrependSectionText() const
{
	return m_prependSectionText;
}

void DefaultDebug::SetAppendLineText( std::string text )
{
	m_appendLineText;
}

std::string DefaultDebug::GetAppendLineText() const
{
	return m_appendLineText;
}

void DefaultDebug::SetPrependLineText( std::string text )
{
	m_prependLineText = text;
}

std::string DefaultDebug::GetPrependLineText() const
{
	return m_appendLineText;
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

void DefaultDebug::LogSectionLine( std::string section, std::string line )
{
	std::string text = (!section.empty() ? (m_prependSectionText + section + m_appendSectionText) : std::string{}) + (m_prependLineText + line + GetAppendLineText());

	m_logLines.push_back( text );

	for (auto logger : m_logListeners)
	{
		logger->Log( text );
	}
}

void DefaultDebug::LogLine( std::string line )
{
	LogSectionLine( GetBlocksText(), line );
}

void DefaultDebug::AttachLogListener( me::ILogListener* listener )
{
	using namespace std;

	// First message to logger is to confirm it is attached.
	// The message is only for the newly attached logger.
	listener->Log( "THIS LOGGER ATTACHED\r\n" );

	// Log existing text lines.
	for (auto text : m_logLines)
	{
		listener->Log( text );
	}

	m_logListeners.push_back( listener );
}

void DefaultDebug::DetachLogListener( me::ILogListener* listener )
{
	m_logListeners.remove( listener );
}

void DefaultDebug::DetachAllLogListeners()
{
	m_logListeners.clear();
}

void DefaultDebug::SetErrorHandler( IErrorHandler::ptr errorHandler )
{
	{
		m_errorHandler = errorHandler;
	}
	int i; i = 0;
}

ReportErrorResult DefaultDebug::ReportError( me::ErrorLevel level, std::string source, std::string error, bool canContinue, bool canRetry )
{
	switch( level )
	{
	case me::ErrorLevel::Engine:
		m_criticalErrors.push_back( "Engine Failure (" + source + "): " + error );
		LogLine( "Engine Failure (" + source + "): " + error );
		break;

	case me::ErrorLevel::Extension:
		m_criticalErrors.push_back( "Extension Failure (" + source + "): " + error );
		LogLine( "Extension Failure (" + source + "): " + error );
		break;

	case me::ErrorLevel::Critical:
		m_criticalErrors.push_back( "Critical Failure (" + source + "): " + error );
		LogLine( "Critical Failure (" + source + "): " + error );
		break;

	case me::ErrorLevel::Failure:
		LogLine( "Failure (" + source + "): " + error );
		if( m_failuresAsCritial )
		{
			m_criticalErrors.push_back( "Critical Failure (" + source + "): " + error );
		}
		break;

	case me::ErrorLevel::Warning:
		LogLine( "Warning (" + source + "): " + error );
		break;
	}

	return m_errorHandler->ReportError( level, source, error, canContinue, canRetry );
}

void DefaultDebug::Try( std::function< void() > func, me::ErrorLevel level, bool canContinue, bool canRetry )
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
				result = ReportError( level, GetBlocksText(), ex.what(), canContinue, canRetry );
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

const std::vector< std::string > & DefaultDebug::GetLogLines() const
{
	return m_logLines;
}

void DefaultDebug::PushBlock( Block* block )
{
	LogSectionLine( GetBlocksText() + ">>" + block->GetName(), "" );
	m_blocks.push_back( block );
}

void DefaultDebug::PopBlock( Block* block )
{
	if ( m_criticalErrors.size() )
	{
		return; // Prevent block changes during a critical failure.
	}

	if ( (m_blocks.back()) != block )
	{
		this->ReportError( ErrorLevel::Engine, "DefaultDebug", "Top of block is not the same as block being popped!", false, false );
	}
	m_blocks.pop_back();
}

const Block* DefaultDebug::GetTopBlock() const
{
	Block* top = m_blocks.empty() ? nullptr : m_blocks.back();
	return top;
}

std::string DefaultDebug::GetBlocksText() const
{
	std::string text;
	for ( auto block = m_blocks.begin(); block != m_blocks.end(); block++ )
	{
		if (!text.empty())
		{
			text += "::";
		}
		text += (*block)->GetName();
	}

	return text;
}