// Copyright (c) 2003 - 2011, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/debug/ILogListener.h>
#include <unify/Path.h>

namespace me
{
	namespace debug
	{
		class FileLoggerListener : public ILogListener
		{
		private:
			unify::Path m_logFile;
		public:
			FileLoggerListener( unify::Path logFile );

			void SetLogFile( unify::Path logFile );
			void Log( std::string text ) override;
		};
	}
}