// Copyright (c) 2003 - 2011, Kit10 Studios LLC
// All Rights Reserved

#include <me/debug/FileLoggerListener.h>

using namespace me;
using namespace debug;

FileLoggerListener::FileLoggerListener( unify::Path path )
	: m_path{}
{
	SetLogPath( path );
}

FileLoggerListener::~FileLoggerListener()
{
}

void FileLoggerListener::SetLogPath( unify::Path path )
{
	if ( m_stream.is_open() )
	{
		m_stream.close();
	}
	m_path.Rename( path );
	m_path = path;
}

void FileLoggerListener::LogEvent( const kit::LogEvent* event )
{
	using namespace std;
	if ( !m_stream.is_open() )
	{
		m_stream.open( m_path.ToString(), ofstream::out | ofstream::app );
	}

	if ( !m_stream.is_open() )
	{
		throw std::exception( "File logger stream is not open!" );
	}


	m_stream << event->text << endl;
	m_stream.flush();

	if ( m_stream.fail() )
	{
		throw std::exception( "Failed to write to file logger!" );
	}
}
