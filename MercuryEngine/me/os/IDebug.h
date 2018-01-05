// Copyright (c) 2003 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/os/ILogListener.h>
#include <unify/Path.h>
#include <list>
#include <string>

namespace me
{
	enum class ErrorLevel
	{
		Critical, // System is left unstable, we should likely shutdown immediately.
		Failure, // We have a failure, the module that reported it is likely corrupted.
		Warning // Error might not be important - state is unknown.
	};

	namespace os
	{
		class IDebug
		{
		public:
			virtual ~IDebug() {}

			/// <summary>
			/// Enabled or disable debug only features. This is helpful when debug only features help with working with release builds. Or to improve debug time, or isolate code for debugging.
			/// Debug features begin with Debug.
			///
			virtual void SetDebug( bool debug ) = 0;

			/// <summary>
			/// Returns true if we have debug only features enabled.
			/// </summary>
			virtual bool IsDebug() = 0;

			/// <summary>
			/// Set the log file. Previous log file is renamed to this log file.
			/// </summary>
			virtual void SetLogFile( unify::Path logFile ) = 0;

			/// <summary>
			/// Set failures as critical failures.
			/// </summary>
			virtual void SetFailureAsCritical( bool faiureAsCritical ) = 0;

			/// <summary>
			/// Assert.
			/// </summary>
			virtual bool Assert( bool assertion ) const = 0;

			/// <summary>
			/// Write a line to the log.
			/// </summary>
			virtual void LogLine( std::string section, std::string line ) = 0;

			/// <summary>
			/// Attach a listener, to listen for log activity.
			/// </summary>
			virtual void AttachLogListener( ILogListener* listener ) = 0;

			/// <summary>
			/// Detach a log listener.
			/// </summary>
			virtual void DetachLogListener( ILogListener* litener ) = 0;

			/// <summary>
			/// Detach all log listener.
			/// </summary>
			virtual void DetachAllLogListeners() = 0;

			/// <summary>
			/// Report an error.
			/// </summary>
			virtual void ReportError( me::ErrorLevel level, std::string source, std::string error ) = 0;

			/// <summary>
			/// Returns true if we'd had a critical error.
			/// </summary>
			virtual bool HadCriticalError() const = 0;

			/// <summary>
			/// Begin a time stamp block by name, so we can determine the time it takes to perform a block of code.
			/// </summary>
			virtual void DebugTimeStampBegin( std::string name ) = 0;

			/// <summary>
			/// End a time stamp block.
			/// </summary>
			virtual void DebugTimeStampEnd( std::string name ) = 0;

			/// <summary>
			/// Get the last time it too to perfome a named block, in seconds.
			/// </summary>
			virtual float DebugGetTimeStamp( std::string name ) = 0;

			/// <summary>
			/// Logged lines are stored, and indexable.
			/// </summary>
			virtual const std::vector< std::string > & GetLogLines() const = 0;

		private:
			bool m_failuresAsCritial;
			unify::Path m_logFile;
			std::list< ILogListener* > m_logListeners;
			std::list< std::string > m_criticalErrors;
		};
	}
}