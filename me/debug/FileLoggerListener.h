// Copyright (c) 2003 - 2011, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <kit/debug/ILogListener.h>
#include <kit/debug/ILogger.h>
#include <unify/Path.h>
#include <fstream>

namespace me::debug
{
	/// <summary>
	/// Writes log to a file.
	/// </summary>
	class FileLoggerListener : public kit::debug::ILogListener
	{
	private:
		unify::Path m_path{};
		std::ofstream m_stream{};

	public:
		FileLoggerListener( unify::Path path );
		~FileLoggerListener();

		/// <summary>
		/// Set the path to the log file to be written to.
		/// </summary>
		void SetLogPath( unify::Path path );
		
	public: // kit::debug::ILogListener...
		void LogEvent( const kit::debug::LogEvent* event ) override;
	};
}