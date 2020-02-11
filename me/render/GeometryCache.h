// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/render/GeometryCacheSummation.h>
#include <me/render/Geometry.h>
#include <me/render/RenderParams.h>
#include <me/render/FrameAndMatrix.h>
						
namespace me
{
	namespace render
	{
		class GeometryCache
		{
		public:
			GeometryCache();
			void Add( Geometry * geometry, render::FrameAndMatrix fm );
			void Sum( GeometryCacheSummation & summation );
			void Reset();

		private:
			std::map< Geometry *, std::vector< render::FrameAndMatrix > > m_cache;
		};
	}
}