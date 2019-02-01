// Copyright (c) 2003 - 2011, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <string>

namespace me
{
	namespace debug
	{
		// Forward declarations.
		class IDebug;

		/// <summary>
		/// Used to create a debug block that auto pops, when we pop the strack frame.
		/// </summary>
		class Block
		{
			IDebug * m_debug;
			std::string m_parent;
			std::string m_name;

		public:
			Block( IDebug * debug, std::string name );
			Block( const Block & parent, std::string name );
			~Block();

			/// <summary>
			/// Gets the name of this block with the parents name as well.
			/// </summary>
			std::string GetResolvedName() const;

			/// <summary>
			/// Log a line in this block.
			/// Ultimately just a pass-through to the debugger logger.
			/// </summary>
			void LogLine( std::string line );
		};
	}
}