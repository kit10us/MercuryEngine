// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

///<summary>
/// Geometry for an object
///		Geometry--->NONE
///
/// An object would have geometry to be rendered, as it's apearance. Presently other
/// objects inherit from Geometry that intend to render. Geometry doesn't contain a
/// location, that's held in the object. Geometry can be safely referenced by more than
/// on object.
///</summary>

#pragma once

#include <me/Geometry.h>

namespace dxi
{
	class QLOD;

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


	class QLOD : public me::Geometry
	{
	public:
		QLOD();
		~QLOD();

		// ::Geometry::QResource...
		void Create();
		void Destroy();
		void Validate();
		void Invalidate();

		// ::Geometry...
		void Render( const me::RenderInfo & renderInfo, me::GeometryInstanceData * instanceData );
		void Update( const me::RenderInfo & renderInfo, me::GeometryInstanceData * instanceData );

		void Add( me::Geometry::ptr geometry, float distance );
		me::Geometry::ptr GetLOD( unsigned int index );

		virtual const unify::BBox< float > & ComputeBounds();

	protected:
		std::list< LODNode > m_list;
	};
}