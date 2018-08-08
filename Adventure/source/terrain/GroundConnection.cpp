// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <terrain/GroundConnection.h>
#include <algorithm>

using namespace me;
using namespace render;
using namespace object;
using namespace terrain;

GroundConnection::GroundConnection()
{
}

GroundConnection::~GroundConnection()
{
}

void GroundConnection::AddConnection( Direction direction, Geometry::ptr geometry )
{
	m_connection[ (size_t)direction ].geometry = geometry;
}

const Connection & GroundConnection::GetConnection( Direction direction )
{
	return m_connection[ (size_t)direction ];
}

void GroundConnection::Reverse()
{
	std::swap( m_connection[(size_t)Direction::UL], m_connection[(size_t)Direction::DR] );
	std::swap( m_connection[(size_t)Direction::U], m_connection[(size_t)Direction::D] );
	std::swap( m_connection[(size_t)Direction::UR], m_connection[(size_t)Direction::DL] );
	std::swap( m_connection[(size_t)Direction::L], m_connection[(size_t)Direction::R] );
}
