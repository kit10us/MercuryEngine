// Copyright (c) 2003 - 2011, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/debug/FileLoggerListener.h>

namespace me::debug
{
	/// <summary>
	/// Writes log to a file.
	/// </summary>
	class CSVLoggerListener : public FileLoggerListener
	{
	public:
		CSVLoggerListener( unify::Path directory, unify::Path filename, std::string extension = ".csv");
		~CSVLoggerListener();

		void LogCSVHeader();
		
	public: // kit::debug::ILogListener...
		void LogEvent( const kit::debug::LogEvent* event ) override;
	
	};
}