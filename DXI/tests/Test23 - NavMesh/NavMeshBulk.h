/// Bulk header for NavMesh stuff. Temporary until developement progresses to a more stable point.
#pragma once

#include <unify/V3.h>
#include <unify/Matrix.h>
#include <unify/TimeDelta.h>
#include <unify/Triangle.h>
#include <unify/BBox.h>
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>
#include <list>
#include <map>


// Utility functions for point generation. This can be useful outside of the scope of NavMesh.
typedef size_t PointIndex;
typedef size_t TriangleIndex;
typedef std::list< unify::V3< float > > PointList;
typedef std::list< unify::Triangle< PointIndex > > TriangleList;
typedef std::pair< PointIndex, PointIndex > LineSegment;
void GeneratePointsAcrossLine( PointList & points, unify::V3< float > begin, unify::V3< float > end );
void GenerateTrianglesAcrossArea( PointList & points, TriangleList & triangles, unify::V3< float > ul, unify::V3< float > ur, unify::V3< float > dl, unify::V3< float > dr, unify::Size< size_t > size );


struct IDebugRenderer
{
    virtual ~IDebugRenderer() { }
    virtual void RenderMatrix( const unify::Matrix & matrix, float size ) const = 0;
    virtual void RenderTriangle( const unify::Triangle< unify::V3< float > > & triangle ) const = 0;
    virtual void RenderPath( const std::list< unify::V3< float > > & path ) const = 0;
};

class Sector
{
public:
    Sector();
    ~Sector();

    void AddPoint( const unify::V3< float > & point );
    void AddPoints( std::list< unify::V3< float > > points );
    void AddTriangle( unify::Triangle< PointIndex > triangle, PointIndex offset );
    void AddTriangles( std::list< unify::Triangle< PointIndex > > triangles, PointIndex offset );

    size_t NumberOfPoints() const;
    size_t NumberOfTriangles() const;
    unify::V3< float > GetPoint( PointIndex pointIndex ) const;
    unify::Triangle< PointIndex > GetTriangle( TriangleIndex triangleIndex ) const;
    unify::Triangle< unify::V3< float > > GetTriangleV3( TriangleIndex triangleIndex ) const;

    // Produces a linear set of positions 
    void MakeOutline( std::list< PointIndex > & outline ) const;

    bool IsDirty() const;
    const unify::BBox< float > & GetBBox() const;
    
    void Generate();

protected:
    std::vector< unify::V3< float > > m_points;
    std::vector< unify::Triangle< PointIndex > > m_triangles;

    // Due to add rules, first is always less than second.
    std::vector< std::pair< PointIndex, PointIndex > > m_lineSegments;

    bool m_dirty;
    unify::BBox< float > m_bbox;
};

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

typedef std::pair< Sector *, unify::V3< float > > SectorAndPoint;

class NavMesh
{
public:
    NavMesh();
    virtual ~NavMesh();

    void Destroy();

    void SetDebugRenderer( IDebugRenderer * debugRenderer );
    const IDebugRenderer * GetDebugRenderer() const;

    Sector * AddSector( const std::string & name );
    Sector * FindSector( const std::string & name );
    SectorAndPoint FindClosestSector( const unify::V3< float > & point, float radius );

    // First position bellow point.
    // Returns true upon hit.
    bool NavMesh::GroundHitTest( const unify::V3< float > & point, unify::V3< float > & hitPoint, Sector * & hitSector ) const;

    void Update( unify::Seconds elapsed );
    void Render();

private:
    boost::shared_ptr< IDebugRenderer > m_debugRenderer;
    std::map< std::string, boost::shared_ptr< Sector > > m_sectorMap;
    std::vector< Sector * > m_sectors;
};
