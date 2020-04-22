// Copyright (c) 2003 - 2011, Kit10 Studios LLC
// All Rights Reserved

#include <me/debug/Logger.h>
#include <me/debug/Block.h>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace me::debug;

Logger::Logger()
	: m_alive{ new bool{ true } }
{
}

Logger::~Logger()
{
	Log( "LOGGER RELEASED" );
	m_listeners.clear();
	*m_alive = false;
}

void Logger::AttachListener( kit::debug::ILogListener::ptr logListener )
{
	using namespace std;

	// First message to logger is to confirm it is attached.
	// The message is only for the newly attached logger.
	const auto event{ kit::debug::LogEvent{ "LOG LISTENER ATTACHED" } };
	logListener->LogEvent( &event );

	// Log existing text lines to the new listener.
	for ( auto event : m_events )
	{
		logListener->LogEvent( &event  );
	}

	m_listeners.push_back( logListener );

}

void Logger::DetachListener( kit::debug::ILogListener::ptr logListener )
{
	const auto event{ kit::debug::LogEvent{ "LOG LISTENER DETACHED" } };
    logListener->LogEvent( &event );
    m_listeners.remove( logListener );
}

void Logger::Log( std::string text )
{
	// Add time stamp to log text.
	using namespace std;
	auto currentTime = chrono::system_clock::now();
	auto t = chrono::system_clock::to_time_t( currentTime );

	// Push event into history.
	std::stringstream ss;
	ss << put_time( std::localtime(&t), "%F %T" );
	m_events.push_back( { kit::debug::LogEvent{ "" + ss.str() + ": " + text } } );
	
	// Inform listeners of log event.
	for ( auto listener : m_listeners )
    {
        listener->LogEvent( &m_events.back() );
    }
}

kit::debug::IBlock::ptr Logger::CreateBlock( std::string name )
{
	return kit::debug::IBlock::ptr{ new me::debug::Block( this, name, m_alive ) };
}
