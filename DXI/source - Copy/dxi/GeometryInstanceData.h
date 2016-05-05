// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <memory>
#include <unify/Matrix.h>
#include <unify/TimeDelta.h>

namespace dxi
{
	// This object is created per. geometry user (via Objects). It keeps specific data for
	// geometry state and charateristics. It's specialized per. geometry. I.e., a mesh would
	// provide a specialized MeshInstanceData.
	// This pushes all unique data from the geometry, to this object.
	class GeometryInstanceData
	{
	public:
		typedef std::shared_ptr< GeometryInstanceData > shared_ptr;
		GeometryInstanceData()
		{
		}
		virtual ~GeometryInstanceData() {}

		virtual void Update( unify::Seconds elapsed ) {}
	};
}