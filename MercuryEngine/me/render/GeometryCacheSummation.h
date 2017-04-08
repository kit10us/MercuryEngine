// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/render/Geometry.h>
#include <me/render/RenderParams.h>
#include <me/render/FrameAndMatrix.h>
						
namespace me
{
	namespace render
	{
		class Geometry;

		class GeometryCacheSummation
		{
		public:
			GeometryCacheSummation();

			void Add( Geometry * geometry, render::InstancesSet set );

			void Render( render::Params params );

			// Keeps geometry, resets count/instances.
			void Reset();

			size_t Count() const;

		private:
			std::map< Geometry *, std::vector< render::InstancesSet > > m_summation;
			size_t m_count;
		};
	}
}