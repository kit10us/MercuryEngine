// Copyright (c) 2003 - 2011, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <kit/debug/ILogListener.h>
#include <kit/debug/ILogger.h>
#include <unify/Path.h>
#include <fstream>

namespace me
{
	namespace debug
	{
		class FileLoggerListener : public kit::debug::ILogListener
		{
		private:
			unify::Path m_path;
			std::ofstream m_stream;

		public:
			FileLoggerListener( unify::Path path );
			~FileLoggerListener();

			void LogEvent( const kit::debug::LogEvent* event ) override;

			void SetLogPath( unify::Path path );
		};
	}
}