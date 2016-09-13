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

#include <dxi/Geometry.h>

namespace dxi
{
	class QLOD;

	class LODNode
	{
	public:
		LODNode( Geometry::shared_ptr geometry, float distanceStart )
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

		Geometry::shared_ptr & GetGeometry() { return m_geometry; }

		Geometry::shared_ptr m_geometry;
		float m_distanceStart;
	};


	class QLOD : public Geometry
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
		void Render( const RenderInfo & renderInfo, GeometryInstanceData * instanceData );
		void Update( unify::Seconds elapsed, GeometryInstanceData * instanceData );

		void Add( Geometry::shared_ptr geometry, float distance );
		Geometry::shared_ptr GetLOD( unsigned int index );

		virtual const unify::BBox< float > & ComputeBounds();

	protected:
		std::list< LODNode > m_list;
	};
}