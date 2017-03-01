// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/input/IInputSource.h>
#include <unify/String.h>
#include <vector>
#include <map>

namespace me
{
	namespace input
	{
		/// <summary>
		/// Managers all of our input sources.
		/// </summary>
		class InputManager
		{
		public:
			InputManager();
			~InputManager();

			void AddInputSource( IInputSource::ptr source );

			size_t GetSourceCount() const;
			IInputSource::ptr GetSource( size_t index ) const;
			IInputSource::ptr FindSource( std::string name );

			void Update();

			void Clear();

		private:
			std::vector< IInputSource::ptr > m_sourceList;
			std::map< std::string, IInputSource::ptr, unify::CaseInsensitiveLessThanTest > m_sourceMap;
		};
	}
}