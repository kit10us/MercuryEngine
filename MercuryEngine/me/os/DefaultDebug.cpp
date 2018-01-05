// Copyright (c) 2003 - 2011, Quentin S. Smith
// All Rights Reserved

#include <me/os/DefaultDebug.h>
#include <unify/Exception.h>
#include <Windows.h>
#include <fstream>
#include <cassert>


using namespace me;
using namespace os;

DefaultDebug::DefaultDebug()
	: m_failuresAsCritial{ true }
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
	unify::Path::Rename( logFile, m_logFile );
	m_logFile = logFile;
	LogLine( "Debug", "Log file: " + m_logFile.ToString() );
}

void DefaultDebug::SetFailureAsCritical( bool faiureAsCritical )
{
	m_failuresAsCritial = faiureAsCritical;
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

void DefaultDebug::LogLine( std::string section, std::string line )
{
	m_logLines.push_back( "[" + section + "] " + line );
}

void DefaultDebug::AttachLogListener( me::ILogListener* listener )
{
	using namespace std;

	if( m_logFile.Empty() ) return;

	string line;
	ifstream in( m_logFile.ToString() );
	while( getline( in, line ) )
	{
		listener->Log( line + "\n" );
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

void DefaultDebug::ReportError( me::ErrorLevel level, std::string source, std::string error )
{
	switch( level )
	{
	case me::ErrorLevel::Critical:
		m_criticalErrors.push_back( "Critical Failure (" + source + "): " + error );
		LogLine( "Debug", "Critical Failure (" + source + "): " + error );
		throw unify::Exception( "Critical Failure (" + source + "): " + error );
	case me::ErrorLevel::Failure:
		LogLine( "Debug", "Failure (" + source + "): " + error );
		if( m_failuresAsCritial )
		{
			m_criticalErrors.push_back( "Critical Failure (" + source + "): " + error );
			throw unify::Exception( "Failure (" + source + "): " + error );
		}
		break;
	case me::ErrorLevel::Warning:
		LogLine( "Debug", "Warning (" + source + "): " + error );
		break;
	}
	return;
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