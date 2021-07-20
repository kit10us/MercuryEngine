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


	// Add time stamp to log text.
	using namespace std;

	// Log existing text lines to the new listener.
	for ( auto event : m_events )
	{
		logListener->LogEvent( &event );
	}

	m_listeners.push_back( logListener );

}

void Logger::DetachListener( kit::debug::ILogListener::ptr logListener )
{
	// Add time stamp to log text.
	using namespace std;

    m_listeners.remove( logListener );
}

std::list<kit::debug::ILogListener::ptr> Logger::GetListeners()
{
	return m_listeners;
}

void Logger::Log(std::string text, std::string catagory, std::string location)
{
	// Add time stamp to log text.
	using namespace std;
	auto currentTime = chrono::system_clock::now();
	auto t = chrono::system_clock::to_time_t( currentTime );

	// Push event into history.
	kit::debug::LogEvent logEvent{};
	logEvent.text = text;
	logEvent.time = currentTime;
	logEvent.catagory = catagory;
	logEvent.location = location;
	
	m_events.push_back( logEvent );
	
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
