// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/debug/IDebug.h>
#include <rm/ILogger.h>

namespace me
{
	namespace game
	{
		class GameLogger : public rm::ILogger
		{
			debug::IDebug * m_debug;
		public:
			GameLogger( debug::IDebug * debug );

			void WriteLine( std::string section, std::string text ) override;
		};
	}
}