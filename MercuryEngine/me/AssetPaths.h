// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/Path.h>
#include <list>

namespace me
{
	class AssetPaths
	{
	public:
		AssetPaths();

		void AddSource( unify::Path source );
		
		unify::Path FindAsset( unify::Path path );

	private:
		std::list< unify::Path > m_paths;
	};
}
