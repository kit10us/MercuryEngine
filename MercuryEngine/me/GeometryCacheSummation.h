// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/Geometry.h>
#include <me/RenderParams.h>
#include <me/render/FrameAndMatrix.h>
						
namespace me
{
	class Geometry;

	class GeometryCacheSummation
	{
	public:
		GeometryCacheSummation();

		void Add( Geometry * geometry, render::InstancesSet set );

		void Render( RenderParams params );

		// Keeps geometry, resets count/instances.
		void Reset();

		size_t Count() const;

	private:
		std::map< Geometry *, std::vector< render::InstancesSet > > m_summation;
		size_t m_count;
	};

}