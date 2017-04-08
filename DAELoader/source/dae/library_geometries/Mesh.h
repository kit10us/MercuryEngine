// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dae/Source.h>
#include <dae/Polylist.h>
#include <dae/Vertices.h>
#include <dae/Triangles.h>
#include <dae/DocumentNode.h>
#include <dae/BindMaterial_TechniqueCommon.h>
#include <me/render/Mesh.h>
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

		/// <summary>
		/// Build a primitive list using a given/optional material.
		/// </summary>
		void Build( me::render::Mesh & mesh, const unify::Matrix & matrix, const BindMaterial_TechniqueCommon & technique ) const;
			
	private:
		struct ContributingInput
		{
			const Input_Shared * input;
			const Source * source;
			int offset;
		};

		void GetSources( std::list< ContributingInput > & sources, const Input_Shared * input, int offset, size_t & pStride ) const;

		std::vector< std::shared_ptr< Source > > m_source;
		std::shared_ptr< Vertices > m_vertices; // 1
		std::vector< std::shared_ptr< Polylist > > m_polylist;
		std::map< std::string, size_t > m_source_map;
	};
}