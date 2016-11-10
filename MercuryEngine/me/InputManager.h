// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/IInputSource.h>
#include <list>

namespace me
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

		IInputSource::ptr Find( std::string name );

		void Update();

		void Clear();

	private:
		std::list< IInputSource::ptr > m_sources;
	};
}