// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/Mesh.h>
#include <me/factory/EffectFactories.h>
#include <terrain/Direction.h>

namespace terrain
{
	// Describes a piece of ground.
	class Ground
	{
	public:
		typedef std::shared_ptr< Ground > ptr;

		Ground();
		~Ground();

		void AddGeneric( me::Geometry::ptr geometry );

		me::Geometry::ptr GetGeneric( size_t i );
		size_t GetGenericCount() const;

		void SetEdge( Direction edge, me::Geometry::ptr geo );

		me::Geometry::ptr GetEdge( Direction edge );

	private:
		std::vector< me::Geometry::ptr > m_generics;
		std::vector< me::Geometry::ptr > m_edges;
	};
}