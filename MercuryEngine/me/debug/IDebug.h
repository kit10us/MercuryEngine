// Copyright (c) 2003 - 2011, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/debug/ILogListener.h>
#include <unify/Path.h>
#include <memory>
#include <list>
#include <string>
#include <functional>

namespace me
{
	enum class ErrorLevel
	{
		Critical, // System is left unstable, we should likely shutdown immediately.
		Failure, // We have a failure, the module that reported it is likely corrupted.
		Warning // Error might not be important - state is unknown.
	};

	enum class ReportErrorResult
	{
		Continue,
		Retry,
		Abort
	};

	namespace debug
	{
		/// <summary>
		/// Handlers errors, allowing custom functionality.
		/// </summary>
		class IErrorHandler
		{
		public:
			typedef std::shared_ptr< IErrorHandler > ptr;

			~IErrorHandler() {}

			/// <summary>
			/// Report an error to the error handler.
			/// </summary>
			virtual ReportErrorResult ReportError( me::ErrorLevel level, std::string source, std::string error, bool canContinue = false, bool canRetry = false ) = 0;
		};
	
		/// <summary>
		/// Debugging features used to better troubleshoot issues.
		/// </summary>
		class IDebug
		{
		public:
			typedef std::shared_ptr< IDebug > ptr;

			virtual ~IDebug() {}

			/// <summary>
			/// Enabled or disable debug only features. This is helpful when debug only features help with working with release builds. Or to improve debug time, or isolate code for debugging.
			/// Debug features begin with Debug.
			/// </summary>
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

			/// <sumamry>
			/// Returns true if we want to handle failures as ciritcal.
			/// </summary>
			virtual bool GetFailureAsCritical() const = 0;

			/// <summary>
			/// Set text to append to the end of every section.
			/// </summary>
			virtual void SetAppendSectionText( std::string text ) = 0;

			/// <summary>
			/// Get text to append to the end of every section.
			/// </summary>
			virtual std::string GetAppendSectionText() const = 0;

			/// <summary>
			/// Set text to prepend to the end of every section.
			/// </summary>
			virtual void SetPrependSectionText( std::string text ) = 0;

			/// <summary>
			/// Get text to prepend to the end of every section.
			/// </summary>
			virtual std::string GetPrependSectionText() const = 0;

			/// <summary>
			/// Set text to append to the end of every line.
			/// </summary>
			virtual void SetAppendLineText( std::string text ) = 0;

			/// <summary>
			/// Get text to append to the end of every line.
			/// </summary>
			virtual std::string GetAppendLineText() const = 0;

			/// <summary>
			/// Set text to prepend to the end of every line.
			/// </summary>
			virtual void SetPrependLineText( std::string text ) = 0;
			
			/// <summary>
			/// Get text to prepend to the end of every line.
			/// </summary>
			virtual std::string GetPrependLineText( ) const = 0;

			/// <summary>
			/// Assert.
			/// </summary>
			virtual bool Assert( bool assertion ) const = 0;

			/// <summary>
			/// Write line to the log noting what section the line comes from.
			/// </summary>
			virtual void LogSectionLine( std::string section, std::string line ) = 0;

			/// <summary>
			/// Write a line to the log. The section comes from the top of the block.
			/// </summary>
			virtual void LogLine( std::string line ) = 0;

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
			/// Returns the entire blocks.
			/// </summary>
			virtual void SetErrorHandler( IErrorHandler::ptr errorHandler ) = 0;

			/// <summary>
			/// Report an error.
			/// </summary>
			virtual ReportErrorResult ReportError( me::ErrorLevel level, std::string source, std::string error, bool canContinue = false, bool canRetry = false ) = 0;

			/// <summary>
			/// Try/catch a function allowing us to report the error, continue or retry.
			/// </summary>
			virtual void Try( std::function< void() > func, me::ErrorLevel level, std::string source, bool canContinue = false, bool canRetry = false ) = 0;
				
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

			/// <summary>
			/// Push a block, a mechanism to track where we are in our code.
			/// </summary>
			virtual void PushBlock( std::string name ) = 0;

			/// <summary>
			/// Pop a block, a mechanism to track where we are in our code.
			/// </summary>
			virtual void PopBlock( std::string name ) = 0;

			/// <summary>
			/// Returns the entire blocks.
			/// </summary>
			virtual std::string GetBlocks( std::string newline ) const = 0;
		};
	}
}