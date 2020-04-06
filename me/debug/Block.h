// Copyright (c) 2003 - 2011, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <kit/IBlock.h>
#include <kit/ILogger.h>

namespace me
{
	namespace debug
	{
		class Block : public kit::IBlock
		{
			kit::ILogger* m_logger;
			std::string m_name;

		public:
			Block( kit::ILogger* logger, std::string name );
			~Block() override;

			/// <summary>
			/// Gets the name of this block.
			/// </summary>
			std::string GetName() const override;

			/// <summary>
			/// Log a line in this block.
			/// Ultimately just a pass-through to the debugger logger.
			/// </summary>
			void Log( std::string line ) override;
		};
	}
}