// Copyright (c) 2003 - 2011, Kit10 Studios LLC
// All Rights Reserved

#include <me/debug/FileLoggerListener.h>
#include <fstream>

using namespace me;
using namespace debug;

FileLoggerListener::FileLoggerListener( unify::Path logFile )
	: m_logFile{ logFile }
{
}

void FileLoggerListener::SetLogFile( unify::Path logFile )
{
	m_logFile.Rename( logFile );
	m_logFile = logFile;
}

void FileLoggerListener::Log( std::string text )
{
	using namespace std;
	ofstream stream;
	stream.open( m_logFile.ToString(), ofstream::out | ofstream::app );
	stream << text;
	stream.close();
}
