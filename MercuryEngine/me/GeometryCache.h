// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/GeometryCacheSummation.h>
#include <me/Geometry.h>
#include <me/RenderParams.h>
#include <me/render/FrameAndMatrix.h>
						
namespace me
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