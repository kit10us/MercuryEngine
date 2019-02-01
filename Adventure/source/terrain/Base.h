// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/render/Mesh.h>
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

		void AddGeneric( me::render::Geometry::ptr geometry );

		me::render::Geometry::ptr GetGeneric( size_t i );
		size_t GetGenericCount() const;

	private:
		std::vector< me::render::Geometry::ptr > m_generics;
	};
}