// Copyright (c) 2003 - 2011, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <vector>
#include <list>
#include <string>
#include <kit/ILogger.h>
#include <kit/IBlock.h>

namespace me
{
	namespace debug
	{
		class Logger : public kit::ILogger
		{
			std::list<kit::ILogListener::ptr> m_listeners;
			std::list<kit::IBlock*> m_blocks;
			std::vector< kit::LogEvent > m_events;

		public:
			Logger();
			~Logger() override;

			void AttachListener( kit::ILogListener::ptr logListener ) override;
			void DetachListener( kit::ILogListener::ptr logListener ) override;
			void Log( std::string text ) override;
			kit::IBlock::ptr MakeBlock( std::string name ) override;
			void BlockLog( std::string text ) override;
			void PushBlock( kit::IBlock* block ) override;
			void PopBlock( kit::IBlock* block ) override;
			std::string GetBlockText() const override;
		};
	}
}