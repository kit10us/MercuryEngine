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
		std::string m_extension;
		unify::Path m_logDirectory;
		unify::Path m_logFilename;
		std::ofstream m_stream{};

		/// <summary>
		/// Open the log file.
		/// </summary>
		void OpenLogFile();

		/// <summary>
		/// Close the log file.
		/// </summary>
		void CloseLogFile();

	protected:
		std::ofstream & GetOFStream();
		virtual std::string GetExtension() const;

	public:
		FileLoggerListener( unify::Path directory, unify::Path filename, std::string extension = ".log");
		~FileLoggerListener();

		/// <summary>
		/// Set the directory for the log file.
		/// </summary>
		void SetLogDirectory( unify::Path directory );

		/// <summary>
		/// Set the directory for the log file.
		/// </summary>
		void SetLogFilename(unify::Path directory);

		/// <summary>
		/// Get the log directory.
		/// </summary>
		unify::Path GetLogDirectory() const;

		/// <summary>
		/// Get the log directory.
		/// </summary>
		unify::Path GetLogFilename() const;
		
		unify::Path GetPath() const override;

	public: // kit::debug::ILogListener...
		void LogEvent( const kit::debug::LogEvent* event ) override;
	};
}