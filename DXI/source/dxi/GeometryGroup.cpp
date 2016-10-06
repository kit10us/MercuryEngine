// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include "dxi/GeometryGroup.h"

using namespace dxi;

GeometryGroup::GeometryGroup()
{
}

GeometryGroup::~GeometryGroup()
{
	Clear();
}

void GeometryGroup::Clear()
{
	m_geometryList.clear();
}

void GeometryGroup::Render( const RenderInfo & renderInfo, GeometryInstanceData * instanceData )
{
	// Render all geometry...
	for( std::vector< Geometry::shared_ptr >::iterator itr = m_geometryList.begin(); itr != m_geometryList.end(); ++itr )
	{ 
		(*itr)->Render( renderInfo, instanceData );
	}
}

void GeometryGroup::Update( const RenderInfo & renderInfo, GeometryInstanceData * instanceData )
{
	// Update all geometry...
	for( std::vector< Geometry::shared_ptr >::iterator itr = m_geometryList.begin(); itr != m_geometryList.end(); ++itr )
	{
		(*itr)->Update( renderInfo, instanceData );
	}
}

void GeometryGroup::Add( dxi::Geometry::shared_ptr geometry )
{
	m_BBox.AddBBox( geometry->GetBBox() );
	m_geometryList.push_back( geometry );
}

const unify::BBox< float > & GeometryGroup::ComputeBounds()
{
	// Merge all boudns into our bounds...
	m_BBox.Initialize();
	for( std::vector< Geometry::shared_ptr >::iterator itr = m_geometryList.begin(); itr != m_geometryList.end(); ++itr )
	{
		m_BBox.AddBBox( (*itr)->GetBBox() );
	}
	return m_BBox;
}

