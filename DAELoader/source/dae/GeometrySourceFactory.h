// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/Geometry.h>
#include <rm/ResourceManagerSimple.h>
#include <dae/Document.h>

namespace dae
{
	class GeometrySourceFactory : public rm::ISourceFactory< me::Geometry >
	{
	public:
		GeometrySourceFactory( me::IRenderer * renderer, util::IEffectSolver * effectSolver );

		me::Geometry::ptr Produce( unify::Path path, void * data ) override;

	private:
		me::IRenderer * m_renderer;
		std::shared_ptr< util::IEffectSolver > m_effectSolver;
	};
}