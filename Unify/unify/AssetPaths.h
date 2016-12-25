// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/Path.h>
#include <list>

namespace unify
{
	class AssetPaths
	{
	public:
		AssetPaths();

		void AddSource( unify::Path source );
		
		unify::Path FindAsset( unify::Path path, unify::Path firstPlace = "" );

	private:
		std::list< unify::Path > m_paths;
	};
}
