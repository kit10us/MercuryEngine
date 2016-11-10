// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/Path.h>
#include <windows.h>

namespace me
{
	class IGame;

	class Extension
	{
	public:
		Extension( unify::Path source );
		virtual ~Extension();

		bool Load( IGame * game );
		void Free();

	private:
		unify::Path m_source;

		HMODULE m_moduleHandle; 
	};
}