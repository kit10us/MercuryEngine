// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dae/library_geometries/Mesh.h>
#include <dae/DocumentNode.h>
#include <dae/library_effects/Effect.h>
#include <me/Mesh.h>
#include <qxml/QXML.h>
#include <string>

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
		void Build( me::Mesh & mesh, const unify::Matrix & matrix, const BindMaterial_TechniqueCommon & technique ) const;

	private:
		std::string m_id;
		std::string m_name;
		MeshSource::TYPE m_meshSource;
		std::shared_ptr< Mesh > m_mesh;
	};
}