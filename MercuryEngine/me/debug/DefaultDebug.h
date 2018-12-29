// Copyright (c) 2003 - 2011, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/debug/IDebug.h>
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
			std::string m_prependSectionText;
			std::string m_appendSectionText;
			std::string m_prependLineText;
			std::string m_appendLineText;

			std::list< std::string > m_criticalErrors;
			std::map< std::string, TimeStartEnd > m_timeStamps;
			std::list< std::string > m_blocks;

		protected:
			unify::Path m_logFile;
			std::vector< std::string > m_logLines;
			std::list< me::ILogListener* > m_logListeners;

		public:
			DefaultDebug();
			virtual ~DefaultDebug();

			void SetDebug( bool debug ) override;
			bool IsDebug() override;

			void SetLogFile( unify::Path logFile ) override;
			void SetFailureAsCritical( bool faiureAsCritical ) override;

			void SetAppendSectionText( std::string text ) override;
			std::string GetAppendSectionText() const override;

			void SetPrependSectionText( std::string text ) override;
			std::string GetPrependSectionText() const override;

			void SetAppendLineText( std::string text ) override;
			std::string GetAppendLineText() const override;

			void SetPrependLineText( std::string text ) override;
			std::string GetPrependLineText() const override;

			bool Assert( bool assertion ) const override;

			void LogSectionLine( std::string section, std::string line ) override;
			void LogLine( std::string line ) override;
			void AttachLogListener( me::ILogListener * listener ) override;
			void DetachLogListener( me::ILogListener * litener ) override;
			void DetachAllLogListeners() override;

			ReportErrorResult ReportError( me::ErrorLevel level, std::string source, std::string error ) override;

			bool HadCriticalError() const override;

			void DebugTimeStampBegin( std::string name ) override;
			void DebugTimeStampEnd( std::string name )	 override;
			float DebugGetTimeStamp( std::string name )	 override;

			const std::vector< std::string > & GetLogLines() const override;

			void PushBlock( std::string name ) override;
			void PopBlock( std::string name ) override;
			std::string GetBlocks( std::string newline ) const override;

		};
	}
}