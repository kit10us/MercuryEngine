// Copyright (c) 2003 - 2011, Kit10 Studios LLC
// All Rights Reserved

#include <me/debug/CSVLoggerListener.h>
#include <iomanip>
#include <sstream>

using namespace me::debug;

CSVLoggerListener::CSVLoggerListener(unify::Path directory, unify::Path filename, std::string extension)
	: FileLoggerListener{directory, filename, extension }
{
	LogCSVHeader();
}

CSVLoggerListener::~CSVLoggerListener()
{
}

void CSVLoggerListener::LogCSVHeader()
{
	auto& stream = GetOFStream();
	stream << "date,time,category,location,text\n";
}

void CSVLoggerListener::LogEvent(const kit::debug::LogEvent* event)
{
	using namespace std;

	auto & stream = GetOFStream();

	auto t = chrono::system_clock::to_time_t(event->time);

	// Push event into history.
	std::stringstream date;
	date << put_time(std::localtime(&t), "%F");

	std::stringstream time;
	time << put_time(std::localtime(&t), "%T");

	std::string formattedText{};
	for (auto ch : event->text)
	{
		if (ch == '\"')
		{
			formattedText += '\'';
		}
		else
		{
			formattedText += ch;
		}
	}

	stream << date.str() << ",";
	stream << time.str() << ",";
	stream << "\"" << event->category << "\",";
	stream << "\"" << event->location << "\",";
	stream << "\"" << formattedText << "\"" << endl;

	stream.flush();

	if ( stream.fail() )
	{
		throw std::exception( "Failed to write to CSV logger!" );
	}
}

