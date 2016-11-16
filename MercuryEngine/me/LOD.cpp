// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/LOD.h>
#include <algorithm>

using namespace me;

LOD::LOD()
{
}

LOD::~LOD()
{
}

void LOD::Add( Geometry::ptr geometry, float distanceStart )
{
	m_list.push_back( LODNode( geometry, distanceStart ) );
	m_list.sort();

	ComputeBounds();
}

void LOD::Render( const RenderInfo & renderInfo, GeometryInstanceData * instanceData, std::list< RenderInstance > & list )
{
	if( m_list.empty() )
	{
		return;	
	}
	
	if( m_list.size() == 1 )
	{
		m_list.begin()->GetGeometry()->Render( renderInfo, instanceData, list );
		return;
	}

	// Find the last iterator that doesn't exceed our distance.
	std::list< LODNode >::iterator itr;
	for( itr = m_list.begin(); itr != m_list.end() && itr->DistanceStart() < renderInfo.DistanceFromCamera(); ++itr )
	{
	}

	itr->GetGeometry()->Render( renderInfo, instanceData, list );
}

void LOD::Update( const RenderInfo & renderInfo, GeometryInstanceData * instanceData )
{
	// Update all geometry...
	for( auto node : m_list )
	{
		node.GetGeometry()->Update( renderInfo, instanceData );
	}
}

const unify::BBox< float > & LOD::ComputeBounds()
{
	// Merge all bouns into our bounds...
	m_BBox.Clear();
	for( std::list< LODNode >::iterator itr = m_list.begin(); itr != m_list.end(); ++itr )
	{
		m_BBox +=itr->GetGeometry()->GetBBox();
	}

	return m_BBox;
}

Geometry::ptr LOD::GetLOD( unsigned int index )
{
	// This should ensure the next step doesn't ever hit end (overflow).
	if( index >= (unsigned int)m_list.size() )
	{
		return Geometry::ptr();
	}

	std::list< LODNode >::iterator itr;
	for( unsigned int iteratorIndex = 0; iteratorIndex < index; ++itr, ++iteratorIndex );
	{
	}

	return itr->GetGeometry();
}
