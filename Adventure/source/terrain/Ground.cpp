// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <terrain/Ground.h>

using namespace me;
using namespace render;
using namespace object;
using namespace terrain;

Ground::Ground()
	: m_edges{ DirectionCount }
{
}

Ground::~Ground()
{
}

void Ground::AddGeneric( Geometry::ptr geometry )
{
	m_generics.push_back( geometry );
}

Geometry::ptr Ground::GetGeneric( size_t i )
{
	if ( i >= m_generics.size() )
	{
		return Geometry::ptr();
	}
	return m_generics[ i ];
}

size_t Ground::GetGenericCount() const
{
	return m_generics.size();
}

void Ground::SetEdge( Direction edge, Geometry::ptr geo )
{
	m_edges[(size_t)edge] = geo;
}

Geometry::ptr Ground::GetEdge( Direction edge )
{
	return m_edges[(size_t)edge];
}

void Ground::AddFriend( size_t other )
{
	m_friends.push_back( other );
}

bool Ground::IsFriend( size_t other ) const
{
	return std::find( m_friends.begin(), m_friends.end(), other ) == m_friends.end() ? false : true;
}
