// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <sculpter/spline/SplineList.h>

using namespace sculpter;
using namespace spline;

SplineList::SplineList()
{
}

SplineList::SplineList( std::initializer_list< ISpline::ptr > splines )
	: m_splines{ splines }
{
}

SplineList::~SplineList()
{
}

void SplineList::AddSpline( ISpline::ptr spline )
{
	m_splines.push_back( spline );
}

size_t SplineList::GetSplineCount() const
{
	return m_splines.size();
}

const ISpline::ptr SplineList::GetSpline( size_t index ) const
{
	assert( index <= m_splines.size() );
	return m_splines[index];
}

ISpline::ptr SplineList::operator[]( size_t index )
{
	assert( index < m_splines.size() );
	return m_splines[index];
}

const ISpline::ptr SplineList::operator[]( size_t index ) const
{
	assert( index < m_splines.size() );
	return m_splines[index];
}
