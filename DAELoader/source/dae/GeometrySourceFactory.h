// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/Geometry.h>
#include <rm/ResourceManagerSimple.h>
#include <dae/Document.h>

namespace dae
{
	class GeometrySourceFactory : public rm::ISourceFactory< dxi::Geometry >
	{
	public:
		GeometrySourceFactory( dxi::core::IRenderer * renderer, util::IEffectSolver * effectSolver );

		dxi::Geometry::ptr Produce( unify::Path path, void * data ) override;

	private:
		dxi::core::IRenderer * m_renderer;
		std::shared_ptr< util::IEffectSolver > m_effectSolver;
	};
}