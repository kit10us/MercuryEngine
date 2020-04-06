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

namespace me
{
	namespace debug
	{
		class DefaultDebug : public IDebug
		{
			struct TimeStartEnd
			{
				std::chrono::high_resolution_clock::time_point start;
				std::chrono::high_resolution_clock::time_point end;
			};

			bool m_failuresAsCritial;
			bool m_isDebug;

			std::list< std::string > m_criticalErrors;
			std::map< std::string, TimeStartEnd > m_timeStamps;

		protected:
			Logger m_logger;
			IErrorHandler::ptr m_errorHandler;
			std::shared_ptr< FileLoggerListener > m_fileLogger;

		public:
			DefaultDebug();
			virtual ~DefaultDebug();

			void SetDebug( bool debug ) override;
			bool IsDebug() override;

			void SetLogPath( unify::Path path ) override;

			void SetFailureAsCritical( bool faiureAsCritical ) override;

			bool GetFailureAsCritical() const override;

			bool Assert( bool assertion ) const override;

			void SetErrorHandler( IErrorHandler::ptr errorHandler ) override;

			ReportErrorResult ReportError( ErrorLevel level, std::string source, std::string error, bool canContinue, bool canRetry ) override;

			void Try( std::function< void() > func, ErrorLevel level, bool canContinue, bool canRetry ) override;

			bool HadCriticalError() const override;

			void DebugTimeStampBegin( std::string name ) override;
			void DebugTimeStampEnd( std::string name )	 override;
			float DebugGetTimeStamp( std::string name )	 override;

			kit::ILogger* GetLogger() override;
			kit::IBlock::ptr MakeBlock( std::string name ) override;
		};
	}
}