// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/dae/DAE_Source.h>
#include <dxi/dae/DAE_Polylist.h>
#include <dxi/dae/DAE_Vertices.h>
#include <dxi/dae/DAE_Triangles.h>
#include <dxi/dae/DAE_DocumentNode.h>
#include <dxi/dae/DAE_BindMaterial_TechniqueCommon.h>
#include <dxi/PrimitiveList.h>
#include <qxml/QXML.h>
#include <map>
#include <vector>
#include <string>

namespace dxi
{
	namespace dae
	{
		/// <summary>
		/// DAE node: COLLADA->library_geometries->geometry->mesh (1 or more)
		/// </summary>
		class Mesh : public DocumentNode
		{
		public:
			Mesh( IDocument & document, const qxml::Element * node );

			const std::vector< boost::shared_ptr< Source > > & GetSource() const;
			boost::shared_ptr< Source > GetSource( const std::string & name ) const;
			
			/// <summary>
			/// Build a primitive list using a given/optional material.
			/// </summary>
			void Build( PrimitiveList & pl, const unify::Matrix & matrix, const BindMaterial_TechniqueCommon & technique ) const;
			
		private:
			std::vector< boost::shared_ptr< Source > > m_source;
			boost::shared_ptr< Vertices > m_vertices; // 1
			std::vector< boost::shared_ptr< Polylist > > m_polylist;
			std::map< std::string, size_t > m_source_map;
		};
	}
}