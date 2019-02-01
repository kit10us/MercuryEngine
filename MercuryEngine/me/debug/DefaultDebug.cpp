// Copyright (c) 2003 - 2011, Kit10 Studios LLC
// All Rights Reserved

#include <me/debug/DefaultDebug.h>
#include <me/debug/Block.h>
#include <me/debug/DefaultErrorHandler.h>
#include <unify/Exception.h>
#include <fstream>
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
{
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

void DefaultDebug::SetLogFile( unify::Path logFile )
{
	Block( this, "DefaultDebug" );

	logFile.Rename( m_logFile );
	m_logFile = logFile;
	LogLine( "Log file: " + m_logFile.ToString() );
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
	m_logLines.push_back( ( ! section.empty() ? (  m_prependSectionText + section + m_appendSectionText ) : std::string{} )  + ( m_prependLineText + line + GetAppendLineText() ) );
}

void DefaultDebug::LogLine( std::string line )
{
	LogSectionLine( GetBlocks( " " ), line );
}

void DefaultDebug::AttachLogListener( me::ILogListener* listener )
{
	using namespace std;

	if( m_logFile.Empty() ) return;

	string line;
	ifstream in( m_logFile.ToString() );
	while( getline( in, line ) )
	{
		listener->Log( line );
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
	m_errorHandler = errorHandler;
}

ReportErrorResult DefaultDebug::ReportError( me::ErrorLevel level, std::string source, std::string error, bool canContinue, bool canRetry )
{
	switch( level )
	{
	case me::ErrorLevel::Critical:
		m_criticalErrors.push_back( "Critical Failure (" + source + "): " + error );
		LogLine( "Critical Failure (" + source + "): " + error );
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


/// <summary>
/// Push a block, a mechanism to track where we are in our code.
/// </summary>
void DefaultDebug::PushBlock( std::string name )
{
	m_blocks.push_back( name );
	LogSectionLine( "", "Entering " + name + " block" );
}

/// <summary>
/// Pop a block, a mechanism to track where we are in our code.
/// </summary>
void DefaultDebug::PopBlock( std::string name )
{
	if ( m_criticalErrors.size() )
	{
		return; // Prevent block changes during a critical failure.
	}

	if ( (m_blocks.back()) != name )
	{
		this->ReportError( ErrorLevel::Critical, "DefaultDebug", "Top of block is not the same as block being popped!", false, false );
	}
	m_blocks.pop_back();
}

/// <summary>
/// Pop a block, a mechanism to track where we are in our code.
/// </summary>
std::string DefaultDebug::GetBlocks( std::string newline ) const
{
	std::string blocks{};
	for( auto itr = m_blocks.begin(); itr != m_blocks.end(); ++itr )
	{
		blocks += *itr + newline; 
	}
	return blocks;
}