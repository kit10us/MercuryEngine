// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/Mesh.h>
#include <me/factory/EffectFactories.h>
#include <terrain/Direction.h>

namespace terrain
{
	// All other grounds go on top of a base.
	class Base
	{
	public:
		typedef std::shared_ptr< Base > ptr;

		Base();
		~Base();

		void AddGeneric( me::Geometry::ptr geometry );

		me::Geometry::ptr GetGeneric( size_t i );
		size_t GetGenericCount() const;

	private:
		std::vector< me::Geometry::ptr > m_generics;
	};
}