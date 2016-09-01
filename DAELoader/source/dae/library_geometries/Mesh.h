// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dae/Source.h>
#include <dae/Polylist.h>
#include <dae/Vertices.h>
#include <dae/Triangles.h>
#include <dae/DocumentNode.h>
#include <dae/BindMaterial_TechniqueCommon.h>
#include <dxi/PrimitiveList.h>
#include <qxml/QXML.h>
#include <map>
#include <vector>
#include <string>

namespace dae
{
	/// <summary>
	/// DAE node: COLLADA->library_geometries->geometry->mesh (1 or more)
	/// </summary>
	class Mesh : public DocumentNode
	{
	public:
		Mesh( IDocument & document, const qxml::Element * node );

		const std::vector< std::shared_ptr< Source > > & GetSource() const;
		std::shared_ptr< Source > GetSource( const std::string & name ) const;
			
		/// <summary>
		/// Build a primitive list using a given/optional material.
		/// </summary>
		void Build( dxi::PrimitiveList & pl, const unify::Matrix & matrix, const BindMaterial_TechniqueCommon & technique ) const;
			
	private:
		std::vector< std::shared_ptr< Source > > m_source;
		std::shared_ptr< Vertices > m_vertices; // 1
		std::vector< std::shared_ptr< Polylist > > m_polylist;
		std::map< std::string, size_t > m_source_map;
	};
}