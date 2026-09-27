// Copyright (c) 2003 - 2011, Kit10 Studios LLC
// All Rights Reserved

#include <me/debug/FileLoggerListener.h>
#include <iomanip>
#include <sstream>

using namespace me::debug;

FileLoggerListener::FileLoggerListener( unify::Path directory, unify::Path filename, std::string extension )
	: m_extension{ extension }
	, m_logFilename{ unify::Path{ filename.ToString() + extension } }
	, m_logDirectory{ directory }
{
	OpenLogFile();
}

FileLoggerListener::~FileLoggerListener()
{
	CloseLogFile();
}

void FileLoggerListener::OpenLogFile()
{
	using namespace std;

	m_stream.open(GetPath().ToString(), ofstream::out | ofstream::app);
	if (m_stream.is_open() == false)
	{
		throw std::exception("Failed to open file logger!");
	}
}

void FileLoggerListener::CloseLogFile()
{
	if (m_stream.is_open())
	{
		m_stream.close();
	}
}

std::ofstream & FileLoggerListener::GetOFStream()
{
	return m_stream;
}

std::string FileLoggerListener::GetExtension() const
{
	return m_extension;
}

void FileLoggerListener::SetLogDirectory(unify::Path directory)
{
	CloseLogFile();

	auto oldPath = GetPath();

	m_logDirectory = directory;

	auto newPath = GetPath();
	newPath.Delete();

	oldPath.Rename(newPath);

	OpenLogFile();
}

void FileLoggerListener::SetLogFilename(unify::Path filename)
{
	CloseLogFile();

	auto oldPath = GetPath();

	m_logFilename = filename;
	m_logFilename.ChangeExtension(GetExtension());

	auto newPath = GetPath();
	newPath.Delete();

	oldPath.Rename(newPath);

	OpenLogFile();
}

unify::Path FileLoggerListener::GetLogDirectory() const
{
	return m_logDirectory;
}

unify::Path FileLoggerListener::GetLogFilename() const
{
	return m_logFilename;
}

unify::Path FileLoggerListener::GetPath() const
{
	unify::Path fullFilename{ m_logDirectory };
	fullFilename += m_logFilename;
	return fullFilename;
}

bool FileLoggerListener::LogEvent( const kit::debug::LogEvent* event )
{
	using namespace std;
	
	auto t = chrono::system_clock::to_time_t(event->time);

	// Push event into history.
	std::stringstream date;
	date << put_time(std::localtime(&t), "%F");

	std::stringstream time;
	time << put_time(std::localtime(&t), "%T");

	auto & stream = GetOFStream();

	stream << date.str() << " " << time.str() << ": ";
	
	if (event->category.empty() == false)
	{
		stream << "[" << event->category << "] ";
	}
	else
	{
		stream << "[General] ";
	}
	
	if (event->location.empty() == false)
	{
		stream << event->location << ": ";
	}
	
	stream <<  event->text << endl;

	stream.flush();

	// throw std::exception("Failed to write to file logger!");
	return stream.fail() != true;
}
