// Copyright (c) 2003 - 2011, Kit10 Studios LLC
// All Rights Reserved

#include <me/debug/Logger.h>
#include <me/debug/Block.h>
#include <me/debug/BlockPopper.h>
#include <kit/ILogListener.h>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace me;
using namespace debug;

Logger::Logger()
{
}

Logger::~Logger()
{
	m_blocks.clear();
	m_listeners.clear();
}

void Logger::AttachListener( kit::ILogListener::ptr logListener )
{
	using namespace std;

	// First message to logger is to confirm it is attached.
	// The message is only for the newly attached logger.
	const auto event{ kit::LogEvent{ "LOG LISTENER ATTACHED" } };
	logListener->LogEvent( &event );

	// Log existing text lines to the new listener.
	for ( auto event : m_events )
	{
		logListener->LogEvent( &event  );
	}

	m_listeners.push_back( logListener );

}

void Logger::DetachListener( kit::ILogListener::ptr logListener )
{
	const auto event{ kit::LogEvent{ "LOG LISTENER DETACHED" } };
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
	m_events.push_back( { kit::LogEvent{ "[" + ss.str() + "] " + text } } );
	
	// Inform listeners of log event.
	for ( auto listener : m_listeners )
    {
        listener->LogEvent( &m_events.back() );
    }
}

kit::IBlock::ptr Logger::MakeBlock( std::string name )
{
	kit::IBlock::ptr block{ new me::debug::Block( this, name ), BlockPopper( this ) };
	PushBlock( block.get() );
	return block;
}

void Logger::BlockLog( std::string text )
{
	Log( "[" + GetBlockText() + "] " + text );
}

void Logger::PushBlock( kit::IBlock* block )
{
	m_blocks.push_back( block );
	Log( GetBlockText() + " >> " + block->GetName() );
}

void Logger::PopBlock( kit::IBlock* block )
{
	
	//if ( m_criticalErrors.size() )
	//{
	//	return; // Prevent block changes during a critical failure.
	//}

	//if ( (m_blocks.back()) != block )
	//{
	//	this->ReportError( ErrorLevel::Engine, "DefaultDebug", "Top of block is not the same as block being popped!", false, false );
	//}
	
	Log( GetBlockText() + "<<" + block->GetName() );
	m_blocks.remove( block );
	delete block;
}

std::string Logger::GetBlockText() const
{
	std::string text;
	for ( auto block = m_blocks.begin(); block != m_blocks.end(); block++ )
	{
		if ( !text.empty() )
		{
			text += "::";
		}
		text += (*block)->GetName();
	}

	return text;
}