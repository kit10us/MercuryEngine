// Copyright (c) 2003 - 2011, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/debug/IErrorHandler.h>
#include <kit/debug/ILogger.h>
#include <kit/debug/IBlock.h>
#include <unify/Path.h>
#include <memory>
#include <list>
#include <string>
#include <functional>

namespace me::debug
{
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
		/// Set the log directory.
		/// </summary>
		virtual void SetLogDirectory( unify::Path directory ) = 0;

		/// <summary>
		/// Set the log filename.
		/// </summary>
		virtual void SetLogFilename(unify::Path filename) = 0;

		/// <summary>
		/// Set if an ErrorLevel should be handled as critical (non-resolvable).
		/// </summary>
		virtual void SetErrorAsCritical( ErrorLevel level, bool isCritical ) = 0;

		/// <sumamry>
		/// Returns true if we want to handle an ErrorLevel as critical .
		/// </summary>
		virtual bool GetErrorAsCritical( ErrorLevel level ) const = 0;

		/// <summary>
		/// Assert.
		/// </summary>
		virtual bool Assert( bool assertion ) const = 0;

		/// <summary>
		/// Returns the entire blocks.
		/// </summary>
		virtual void SetErrorHandler( IErrorHandler::ptr errorHandler ) = 0;

		/// <summary>
		/// Report an error.
		/// </summary>
		virtual ReportErrorResult ReportError( ErrorLevel level, std::string error, bool canContinue = false, bool canRetry = false ) = 0;

		/// <summary>
		/// Try/catch a function allowing us to report the error, continue or retry.
		/// </summary>
		virtual void Try( std::function< void() > func, ErrorLevel level, bool canContinue = false, bool canRetry = false ) = 0;
				
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
		/// Get the logger.
		/// </summary>
		virtual kit::debug::ILogger* GetLogger() = 0;
	};
}