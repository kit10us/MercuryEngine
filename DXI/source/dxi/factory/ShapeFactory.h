// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once
#include <dxi/shapes/ShapeCreators.h>
#include <rm/ResourceManagerSimple.h>
#include <dxi/core/IGame.h>

namespace dxi
{
	class ShapeFactory : public rm::ISourceFactory< Geometry >
	{
	public:
		ShapeFactory( core::IGame * game );
		Geometry::ptr Produce( unify::Path source, void * data ) override;

	private:
		core::IGame * m_game;
	};
}