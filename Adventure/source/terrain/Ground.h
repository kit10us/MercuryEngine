// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/render/Mesh.h>
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

		void AddGeneric( me::render::Geometry::ptr geometry );

		me::render::Geometry::ptr GetGeneric( size_t i );
		size_t GetGenericCount() const;

		void SetEdge( Direction edge, me::render::Geometry::ptr geo );

		me::render::Geometry::ptr GetEdge( Direction edge );

		void AddFriend( size_t other );

		bool IsFriend( size_t other ) const;

	private:
		std::vector< me::render::Geometry::ptr > m_generics;
		std::vector< me::render::Geometry::ptr > m_edges;
		std::vector< size_t > m_friends;
	};
}