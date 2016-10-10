// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <memory>
#include <unify/Matrix.h>
#include <unify/TimeDelta.h>
#include <unify/Any.h>
#include <map>

namespace dxi
{
	// This object is created per. geometry user (via Objects). It keeps specific data for
	// geometry state and charateristics. It's specialized per. geometry. ie., a mesh would
	// provide a specialized MeshInstanceData.
	// This pushes all unique data from the geometry, to this object.
	class GeometryInstanceData
	{
	public:
		typedef std::shared_ptr< GeometryInstanceData > ptr;

		GeometryInstanceData();
		virtual ~GeometryInstanceData();

		virtual void Update( unify::Seconds elapsed );

		std::map< std::string, unify::Any > & GetAttributes();
		const std::map< std::string, unify::Any > & GetAttributes() const;

	private:
		std::map< std::string, unify::Any > m_attributes;
	};
}