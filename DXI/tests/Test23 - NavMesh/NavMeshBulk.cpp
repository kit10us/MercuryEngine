#include "NavMeshBulk.h"
#include <dxi/core/Game.h>

using namespace dxi;

void GeneratePointsAcrossLine( std::list< unify::V3< float > > & points, unify::V3< float > begin, unify::V3< float > end, size_t numberOfPoints )
{
    unify::V3< float > segmentLength = end - begin;
    for ( size_t i = 0; i != numberOfPoints; ++i )
    {
        float ratio = static_cast< float >( i ) / static_cast< float >( numberOfPoints );
        points.push_back( begin + segmentLength * ratio );
    }
}

void GenerateTrianglesAcrossArea( PointList & points, TriangleList & triangles, unify::V3< float > ul, unify::V3< float > ur, unify::V3< float > dl, unify::V3< float > dr, unify::Size< size_t > size )
{
    for ( size_t y = 0, yend = size.height + 1; y != yend; ++y )
    {
        float ratioy = static_cast< float >( y ) / static_cast< float >( size.height );
        for ( size_t x = 0, xend = size.width + 1; x != xend; ++x )
        {
            float ratiox = static_cast< float >( x ) / static_cast< float >( size.width );
            unify::V3< float > l( unify::V3< float >::V3Interpolate( ul, dl, ratiox ) );
            unify::V3< float > r( unify::V3< float >::V3Interpolate( ur, dr, ratiox ) );
            unify::V3< float > p( unify::V3< float >::V3Interpolate( l, r, ratioy ) );
            points.push_back( p );
        }
    }

    for ( size_t y = 0, yend = size.height; y != yend; ++y )
    {
        for ( size_t x = 0, xend = size.width; x != xend; ++x )
        {
            // Triangles are added in pairs.
            PointIndex pu = ( size.width + 1 ) * y;
            PointIndex pd = ( size.width + 1 ) * ( y + 1 );
            triangles.push_back( unify::Triangle< PointIndex >( pu + x, pu + x + 1 , pd + x ) );
            triangles.push_back( unify::Triangle< PointIndex >( pu + x + 1 , pd + x + 1, pd + x ) );
        }
    }
}



//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

Sector::Sector()
: m_dirty( false )
{
}

Sector::~Sector()
{
}

void Sector::AddPoint( const unify::V3< float > & point )
{
    m_dirty = true;
    m_points.push_back( point );
    m_bbox.AddPoint( point );
}

void Sector::AddPoints( std::list< unify::V3< float > > points )
{
    for ( std::list< unify::V3< float > >::iterator itr = points.begin(), end = points.end(); itr != end; ++itr )
    {
        AddPoint( *itr );
    }
}

void Sector::AddTriangle( unify::Triangle< PointIndex > triangle, PointIndex offset )
{
    m_dirty = true;
    m_triangles.push_back( triangle + offset );

    std::vector< LineSegment > newSegments;
    newSegments.push_back( LineSegment( std::min<>( triangle.v[ 0 ], triangle.v[ 1 ] ), std::max<>( triangle.v[ 0 ], triangle.v[ 1 ] ) ) );
    newSegments.push_back( LineSegment( std::min<>( triangle.v[ 0 ], triangle.v[ 2 ] ), std::max<>( triangle.v[ 0 ], triangle.v[ 2 ] ) ) );
    newSegments.push_back( LineSegment( std::min<>( triangle.v[ 1 ], triangle.v[ 2 ] ), std::max<>( triangle.v[ 1 ], triangle.v[ 2 ] ) ) );

    // Ensure we add only new segments...
    for ( size_t ns = 0, nsend = newSegments.size(); ns != nsend; ++ns )
    {
        bool segmentAlreadyExists = false;
        for ( size_t i = 0, end = m_lineSegments.size(); i != end; ++i )
        {
            if ( m_lineSegments[ i ].first == newSegments[ ns ].first && m_lineSegments[ i ].second == newSegments[ ns ].second )
            {
                segmentAlreadyExists = true;
                break;
            }
        }

        if ( ! segmentAlreadyExists )
        {
            m_lineSegments.push_back( newSegments[ ns ] );
        }
    }
}

void Sector::AddTriangles( std::list< unify::Triangle< PointIndex > > triangles, PointIndex offset )
{
    for ( std::list< unify::Triangle< PointIndex > >::iterator itr = triangles.begin(), end = triangles.end(); itr != end; ++itr )
    {
        AddTriangle( *itr, offset );
    }
}

size_t Sector::NumberOfPoints() const
{
    return m_points.size();
}

size_t Sector::NumberOfTriangles() const
{
    return m_triangles.size();
}

unify::V3< float > Sector::GetPoint( PointIndex pointIndex ) const
{
    return m_points.at( pointIndex );
}

unify::Triangle< PointIndex > Sector::GetTriangle( TriangleIndex triangleIndex ) const
{
    return m_triangles.at( triangleIndex );
}

unify::Triangle< unify::V3< float > > Sector::GetTriangleV3( TriangleIndex triangleIndex ) const
{
    const unify::Triangle< PointIndex > & triangle = m_triangles.at( triangleIndex );
    return unify::Triangle< unify::V3< float > >( m_points[ triangle.v[ 0 ] ], m_points[ triangle.v[ 1 ] ], m_points[ triangle.v[ 2 ] ] );
}

void Sector::MakeOutline( std::list< PointIndex > & outline ) const
{
}

bool Sector::IsDirty() const
{
    return m_dirty;
}

const unify::BBox< float > & Sector::GetBBox() const
{
    return m_bbox;
}

void Sector::Generate()
{
    if ( m_dirty )
    {
        m_dirty = false;
    }
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

NavMesh::NavMesh()
{
}

NavMesh::~NavMesh()
{
    Destroy();
    m_sectorMap.clear();
}

void NavMesh::Destroy()
{
    m_debugRenderer.reset();
}

void NavMesh::SetDebugRenderer( IDebugRenderer * debugRenderer )
{
    m_debugRenderer.reset( debugRenderer );
}

const IDebugRenderer * NavMesh::GetDebugRenderer() const
{
    return m_debugRenderer ? m_debugRenderer.get() : 0;
}

Sector * NavMesh::AddSector( const std::string & name )
{
    Sector * sector = new Sector;
    m_sectorMap[ name ] = boost::shared_ptr< Sector >( sector );
    m_sectors.push_back( sector );
    return sector;
}

Sector * NavMesh::FindSector( const std::string & name )
{
    std::map< std::string, boost::shared_ptr< Sector > >::iterator itr = m_sectorMap.find( name );
    return ( itr == m_sectorMap.end() ) ? 0 : itr->second.get();
}

bool NavMesh::GroundHitTest( const unify::V3< float > & point, unify::V3< float > & hitPoint, Sector * & hitSector ) const
{
    // Loop sectors for a hit.

    unify::V3< float > direction = point + unify::V3< float >( 0, -1, 0 );
    for ( size_t s = 0, send = m_sectors.size(); s != send; ++s )
    {
    }

	return false; // TODO:
}

void NavMesh::Update( unify::Seconds elapsed )
{
    // TODO:
}

void NavMesh::Render()
{
    if ( m_debugRenderer )
    {
        for ( size_t s = 0, send = m_sectors.size(); s != send; ++s )
        {
            Sector * sector = m_sectors[ s ];
            for ( size_t t = 0, tend = sector->NumberOfTriangles(); t != tend; ++t )
            {
                unify::Triangle< unify::V3< float > > triangle = sector->GetTriangleV3( t );
                m_debugRenderer->RenderTriangle( triangle );
            }
        }
    }
}
