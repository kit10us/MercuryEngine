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
		GeometrySourceFactory( util::IEffectSolver * effectSolver );

		virtual dxi::Geometry * Produce( unify::Path path ) override;

	private:
		std::shared_ptr< util::IEffectSolver > m_effectSolver;
	};
}