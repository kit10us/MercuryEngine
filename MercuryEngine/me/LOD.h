// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/Geometry.h>

namespace me
{
	class LOD : public me::Geometry
	{
	public:
		LOD();
		~LOD();

		void Update( const me::RenderInfo & renderInfo, me::GeometryInstanceData * instanceData );
		void Render( const me::RenderInfo & renderInfo, me::GeometryInstanceData * instanceData, std::list< RenderInstance > & list );

		void Add( me::Geometry::ptr geometry, float distance );
		me::Geometry::ptr GetLOD( unsigned int index );

		virtual const unify::BBox< float > & ComputeBounds();

	protected:

		class LODNode
		{
		public:
			LODNode( me::Geometry::ptr geometry, float distanceStart )
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

			me::Geometry::ptr & GetGeometry() { return m_geometry; }

			me::Geometry::ptr m_geometry;
			float m_distanceStart;
		};

		std::list< LODNode > m_list;
	};
}