// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once
#include <sg/ShapeCreators.h>
#include <rm/ResourceManagerSimple.h>
#include <me/IGame.h>

namespace sg
{
	class ShapeFactory : public rm::ISourceFactory< me::Geometry >
	{
	public:
		ShapeFactory( me::IGame * game );
		me::Geometry::ptr Produce( unify::Path source, void * data ) override;

	private:
		me::IGame * m_game;
	};
}