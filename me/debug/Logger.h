// Copyright (c) 2003 - 2011, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <vector>
#include <list>
#include <string>
#include <kit/debug/ILogger.h>
#include <kit/debug/IBlock.h>

namespace me
{
	namespace debug
	{
		class Logger : public kit::debug::ILogger
		{
			std::list<kit::debug::ILogListener::ptr> m_listeners;
			std::list<kit::debug::IBlock*> m_blocks;
			std::vector< kit::debug::LogEvent > m_events;

			// We create a shared bool that allows those dependant on our existance to check if we are alive/valid.
			std::shared_ptr< bool > m_alive;

		public:
			Logger();
			~Logger() override;

			void AttachListener( kit::debug::ILogListener::ptr logListener ) override;
			void DetachListener( kit::debug::ILogListener::ptr logListener ) override;
			void Log( std::string text ) override;
			kit::debug::IBlock::ptr CreateBlock( std::string name ) override;
		};
	}
}