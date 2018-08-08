// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/render/Geometry.h>

namespace me
{
	namespace render
	{
		class LOD : public Geometry
		{
		public:
			LOD();
			~LOD();

			void Update( const UpdateParams & params, GeometryInstanceData * instanceData ) override;
			void Render( const render::Params & params, GeometryInstanceData * instanceData, render::MatrixFeed & matrixFeed ) override;

			void Add( Geometry::ptr geometry, float distance );
			Geometry::ptr GetLOD( unsigned int index );

			virtual const unify::BBox< float > & ComputeBounds();

		protected:

			class LODNode
			{
			public:
				LODNode( Geometry::ptr geometry, float distanceStart )
					: m_geometry( geometry )
					, m_distanceStart( distanceStart )
				{
				}

				~LODNode()
				{
					m_geometry.reset();
				}

				LODNode & operator=( LODNode & node )
				{
					m_geometry = node.m_geometry;
					m_distanceStart = node.m_distanceStart;
				}

				bool operator<( const LODNode & node )
				{
					return m_distanceStart < node.m_distanceStart;
				}

				float DistanceStart() const
				{
					return m_distanceStart;
				}

				Geometry::ptr & GetGeometry() { return m_geometry; }

				Geometry::ptr m_geometry;
				float m_distanceStart;
			};

			std::list< LODNode > m_list;
		};
	}
}