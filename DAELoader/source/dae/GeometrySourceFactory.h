// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/render/Geometry.h>
#include <rm/ResourceManagerSimple.h>
#include <dae/Document.h>

namespace dae
{
	class GeometrySourceFactory : public rm::ISourceFactory< me::render::Geometry >
	{
	public:
		GeometrySourceFactory( me::render::IRenderer * renderer, util::IEffectSolver * effectSolver );

		me::render::Geometry::ptr Produce( unify::Path path, void * data ) override;

	private:
		me::render::IRenderer * m_renderer;
		std::shared_ptr< util::IEffectSolver > m_effectSolver;
	};
}