// Copyright (c) 2002 - 2018, Kit10 Studios LLC
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

			void Render( const render::Params & params );

			// Keeps geometry, resets count/instances.
			void Reset();

			size_t Count() const;

		private:
			std::map< Geometry *, std::vector< render::InstancesSet > > m_summation;
			size_t m_count;
		};
	}
}