// Copyright (c) 2003 - 2011, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/debug/IDebug.h>
#include <me/debug/DefaultErrorHandler.h>
#include <me/debug/FileLoggerListener.h>
#include <me/debug/Logger.h>
#include <unify/Path.h>
#include <list>
#include <string>
#include <chrono>
#include <map>

namespace me::debug
{
	/// <summary>
	/// The default/generic implementation of an IDebug.
	/// </summary>
	class DefaultDebug : public IDebug
	{
		struct TimeStartEnd
		{
			std::chrono::high_resolution_clock::time_point start;
			std::chrono::high_resolution_clock::time_point end;
		};

		bool m_errorAsCritical[ErrorLevelCount]{ true, true, true, true, false };
		bool m_failuresAsCritial{ true };
		bool m_warningAsCritical{ false };
		std::list< std::string > m_criticalErrors{};
		std::map< std::string, TimeStartEnd > m_timeStamps{};

		bool m_isDebug
#ifdef _DEBUG
		{ true };
#else
		{ false };
#endif

	protected:
		Logger m_logger{};
		IErrorHandler::ptr m_errorHandler{ IErrorHandler::ptr{ new DefaultErrorHandler( this ) } };
		std::shared_ptr< FileLoggerListener > m_fileLogger{
			new FileLoggerListener{ unify::Path{ "default.log" } }
		};

	public:
		DefaultDebug();
		virtual ~DefaultDebug();

	public: // me::debug::IDebug...
		void SetDebug( bool debug ) override;
		bool IsDebug() override;

		void SetLogPath( unify::Path path ) override;

		void SetErrorAsCritical( ErrorLevel level, bool isCritical ) override;

		bool GetErrorAsCritical( ErrorLevel level ) const override;

		bool Assert( bool assertion ) const override;

		void SetErrorHandler( IErrorHandler::ptr errorHandler ) override;

		ReportErrorResult ReportError( ErrorLevel level, std::string error, bool canContinue, bool canRetry ) override;

		void Try( std::function< void() > func, ErrorLevel level, bool canContinue, bool canRetry ) override;

		bool HadCriticalError() const override;

		void DebugTimeStampBegin( std::string name ) override;
		void DebugTimeStampEnd( std::string name )	 override;
		float DebugGetTimeStamp( std::string name )	 override;

		kit::debug::ILogger* GetLogger() override;
	};
}