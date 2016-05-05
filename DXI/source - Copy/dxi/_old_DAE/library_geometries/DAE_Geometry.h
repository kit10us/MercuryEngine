// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once
#include <dxi/dae/library_geometries/DAE_Mesh.h>
#include <dxi/dae/DAE_DocumentNode.h>
#include <dxi/dae/library_effects/DAE_Effect.h>
#include <dxi/PrimitiveList.h>
#include <qxml/QXML.h>
#include <boost/shared_ptr.hpp>
#include <string>

namespace dxi
{
	namespace dae
	{
		/// <summary>
		/// DAE node: COLLADA->library_geometries->geometry (1 or more)
		/// </summary>
		class Geometry : public DocumentNode
		{
		public:
			struct MeshSource
			{
				enum TYPE
				{
					ConvexMesh,
					Mesh,
					Spline,
					BRep
				};
			};

			Geometry( IDocument & document, const qxml::Element * node );

			const std::string & GetID() const;
			const std::string & GetName() const;
			MeshSource::TYPE GetMeshSource() const;
			const Mesh & GetMesh() const;

			/// <summary>
			/// Build into a primitive list a complete geometry.
			/// </summary>
			void Build( PrimitiveList & pl, const unify::Matrix & matrix, const BindMaterial_TechniqueCommon & technique ) const;

		private:
			std::string m_id;
			std::string m_name;
			MeshSource::TYPE m_meshSource;
			boost::shared_ptr< Mesh > m_mesh;
		};
	}
}
