// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <sculpter/PointTable.h>
#include <sculpter/exception/SculpterFailedToCreate.h>

using namespace sculpter;

PointTable::PointTable( std::initializer_list< NamedPoint > points )
{
	assert( points.size() );

	for( auto point : points )
	{
		AddPoint( point );
	}
}

PointTable::PointTable( Point origin )
{
	AddPoint( origin );
}

void PointTable::AddNamedPoint( std::string name, Point point )
{
	if( ! m_points.size() && ! unify::StringIs( name, "origin" ) )
	{
		throw exception::FailedToCreate( "First point in Point Table must be named \"origin\"." );
	}

	m_namedPoints[name] = m_points.size();
	m_points.push_back( point );
}

void PointTable::AddPoint( NamedPoint point )
{
	AddNamedPoint( point.name, point.point );
}

void PointTable::AddPoint( Point point )
{
	if( !m_points.size() )
	{
		AddNamedPoint( "origin", point );
	}
	else
	{
		m_points.push_back( point );
	}
}

PointTable::~PointTable()
{
}