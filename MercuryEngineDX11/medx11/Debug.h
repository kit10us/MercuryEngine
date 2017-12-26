// Copyright (c) 2003 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/IDebug.h>
#include <unify/Path.h>
#include <list>
#include <string>

namespace medx11
{
	class Debug : public me::IDebug
	{
	public:
		Debug();
		virtual ~Debug();

		void SetLogFile( unify::Path logFile ) override;
		void SetFailureAsCritical( bool faiureAsCritical ) override;

		bool Assert( bool assertion ) const override;

		void LogLine( std::string line, int indent = 2 ) override;
		void LogLine( std::string section, std::string line ) override;
		void AttachLogListener( me::ILogListener * listener ) override;
		void DetachLogListener( me::ILogListener * litener ) override;
		void DetachAllLogListeners() override;

		void ReportError( me::ErrorLevel level, std::string source, std::string error ) override;

		bool HadCriticalError() const override;

	private:
		bool m_failuresAsCritial;
		unify::Path m_logFile;
		std::list< me::ILogListener* > m_logListeners;
		std::list< std::string > m_criticalErrors;
	};
}