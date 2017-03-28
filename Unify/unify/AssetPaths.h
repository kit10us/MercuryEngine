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

		void AddSource( Path source );
		
		Path FindAsset( Path path, Path firstPlace = Path() );

		std::string GetPaths( Path root = Path() ) const;

	private:
		std::list< Path > m_paths;
	};
}
