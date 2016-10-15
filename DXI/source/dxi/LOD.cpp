// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/LOD.h>
#include <algorithm>

using namespace dxi;

QLOD::QLOD()
{
}

QLOD::~QLOD()
{
	Destroy();
}

void QLOD::Create()
{
	// TODO:
}

void QLOD::Destroy()
{
	m_list.clear();
}

void QLOD::Validate()
{
	// TODO:
}

void QLOD::Invalidate()
{
	// TODO:
}

void QLOD::Add( Geometry::ptr geometry, float distanceStart )
{
	m_list.push_back( LODNode( geometry, distanceStart ) );
	m_list.sort();

	ComputeBounds();
}

void QLOD::Render( const RenderInfo & renderInfo, GeometryInstanceData * instanceData )
{
	if( m_list.empty() )
	{
		return;	
	}
	
	if( m_list.size() == 1 )
	{
		m_list.begin()->GetGeometry()->Render( renderInfo, instanceData );
		return;
	}

	// Find the last iterator that doesn't exceed our distance.
	std::list< LODNode >::iterator itr;
	for( itr = m_list.begin(); itr != m_list.end() && itr->DistanceStart() < renderInfo.DistanceFromCamera(); ++itr )
	{
	}

	itr->GetGeometry()->Render( renderInfo, instanceData );
}

void QLOD::Update( const RenderInfo & renderInfo, GeometryInstanceData * instanceData )
{
	// Update all geometry...
	for( auto node : m_list )
	{
		node.GetGeometry()->Update( renderInfo, instanceData );
	}
}

const unify::BBox< float > & QLOD::ComputeBounds()
{
	// Merge all bouns into our bounds...
	m_BBox.Clear();
	for( std::list< LODNode >::iterator itr = m_list.begin(); itr != m_list.end(); ++itr )
	{
		m_BBox +=itr->GetGeometry()->GetBBox();
	}

	return m_BBox;
}

Geometry::ptr QLOD::GetLOD( unsigned int index )
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
