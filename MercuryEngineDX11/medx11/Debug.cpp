// Copyright (c) 2003 - 2011, Quentin S. Smith
// All Rights Reserved

#include <medx11/Debug.h>
#include <unify/Exception.h>
#include <Windows.h>
#include <fstream>
#include <cassert>

using namespace medx11;

Debug::Debug()
	: m_failuresAsCritial{ true }

{
}

Debug::~Debug()
{
}

void Debug::SetLogFile( unify::Path logFile )
{
	unify::Path::Rename( logFile, m_logFile );

	m_logFile = logFile;
}

void Debug::SetFailureAsCritical( bool faiureAsCritical )
{
	m_failuresAsCritial = faiureAsCritical;
}


bool Debug::Assert( bool assertion ) const
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

void Debug::LogLine( std::string line, int indent )
{
	using namespace std;

	std::string text = "";
	for( int i = 0; i < indent; i++ )
	{
		text += " ";
	}
	text += line + "\n";

	if( m_logFile.Empty() ) return;

	ofstream out( m_logFile.ToString(), ios_base::out | ios_base::app );
	out << text;

	OutputDebugStringA( text.c_str() );

	for( auto && listener : m_logListeners )
	{
		listener->Log( text );
	}
}

void Debug::LogLine( std::string section, std::string line )
{
	LogLine( "[" + section + "] " + line, 0 );
}

void Debug::AttachLogListener( me::ILogListener* listener )
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

void Debug::DetachLogListener( me::ILogListener* listener )
{
	m_logListeners.remove( listener );
}

void Debug::DetachAllLogListeners()
{
	m_logListeners.clear();
}

void Debug::ReportError( me::ErrorLevel level, std::string source, std::string error )
{
	switch( level )
	{
	case me::ErrorLevel::Critical:
		m_criticalErrors.push_back( "Critical Failure (" + source + "): " + error );
		LogLine( "Critical Failure (" + source + "): " + error );
		throw unify::Exception( "Critical Failure (" + source + "): " + error );
	case me::ErrorLevel::Failure:
		LogLine( "Failure (" + source + "): " + error );
		if( m_failuresAsCritial )
		{
			m_criticalErrors.push_back( "Critical Failure (" + source + "): " + error );
			throw unify::Exception( "Failure (" + source + "): " + error );
		}
		break;
	case me::ErrorLevel::Warning:
		LogLine( "Warning (" + source + "): " + error );
		break;
	}
	return;
}

bool Debug::HadCriticalError() const
{
	return m_criticalErrors.size() != 0;
}

