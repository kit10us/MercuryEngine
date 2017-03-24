// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <terrain/Map.h>
#include <algorithm>

using namespace me;
using namespace scene;
using namespace object;
using namespace terrain;

Map::Map( Map & component )
	: ObjectComponent( component )
	, m_mapSize{ component.m_mapSize }
	, m_terraSize{ component.m_terraSize }
{
	Initialize();
}

Map::Map( unify::Size< int > mapSize, unify::Size< float > terraSize )
	: ObjectComponent( "Map", true, true )
	, m_mapSize{ mapSize }
	, m_terraSize{ terraSize }
	, m_map{ new int[ mapSize.width * mapSize.height ] }
{
	Initialize();
}

Map::~Map()
{
	delete[] m_map;
}

void Map::Initialize()
{
	for ( int i = 0; i < m_mapSize.height * m_mapSize.width; i++ )
	{
		{
			m_map[i] = -1;
			m_frames.push_back( unify::FrameLite( unify::MatrixIdentity() ) );
		}
	}
}

unify::Size< int > Map::GetSize() const
{
	return m_mapSize;
}

void Map::SetInvalid( me::Geometry::ptr invalid )
{
	m_invalid = invalid;
}

void Map::SetBase( Base::ptr base )
{
	m_base = base;
}

int Map::AddGround( std::string name, Ground::ptr ground )
{
	size_t index = m_groundList.size();
	m_groundMap[ name ] = index;
	m_groundList.push_back( ground );
	return index;
}

int Map::GetMap( int x, int y ) const
{
	y = m_mapSize.height - y - 1; // Invert y

	if( x < 0 || y < 0 || x >= m_mapSize.width || y >= m_mapSize.height )
	{
		return -1;
	}

	return m_map[ y * m_mapSize.height + x ];
}

unify::FrameLite * Map::GetFrame( int x, int y )
{
	y = m_mapSize.height - y - 1; // Invert y

	if( x < 0 || y < 0 || x >= m_mapSize.width || y >= m_mapSize.height )
	{
		return nullptr;
	}

	return &m_frames[y * m_mapSize.height + x];
}

int Map::FindGround( std::string name ) const
{
	auto itr = m_groundMap.find( name );
	if( itr == m_groundMap.end() )
	{
		return -1;
	}

	return itr->second;
}

me::Geometry::ptr Map::GetGroundGeometry( const int * type )
{
	int UL = type[(size_t)Direction::UL];
	int U = type[(size_t)Direction::U];
	int UR = type[(size_t)Direction::UR];
	int L = type[(size_t)Direction::L];
	int C = type[(size_t)Direction::C];
	int R = type[(size_t)Direction::R];
	int DL = type[(size_t)Direction::DL];
	int D = type[(size_t)Direction::D];
	int DR = type[(size_t)Direction::DR];

	Ground::ptr ground = m_groundList[C];
	if     ( U != C && L != C && R == C && D == C )	return ground->GetEdge( Direction::UL );
	else if( U != C && L == C && R == C && D == C )	return ground->GetEdge( Direction::U );
	else if( U != C && L == C && R != C && D == C )	return ground->GetEdge( Direction::UR );
	else if( U == C && L != C && R == C && D == C )	return ground->GetEdge( Direction::L );
	else if( U == C && L == C && R == C && D == C ) return ground->GetEdge( Direction::C );
	else if( U == C && L == C && R != C && D == C )	return ground->GetEdge( Direction::R );
	else if( U == C && L != C && R == C && D != C )	return ground->GetEdge( Direction::DL );
	else if( U == C && L == C && R == C && D != C )	return ground->GetEdge( Direction::D );
	else if( U == C && L == C && R != C && D != C )	return ground->GetEdge( Direction::DR );

	else if( U != C && L != C && R == C && D != C )	return ground->GetEdge( Direction::ULD );
	else if( U != C && L == C && R != C && D != C )	return ground->GetEdge( Direction::URD );
	else if( U != C && L != C && R != C && D == C )	return ground->GetEdge( Direction::ULR );
	else if( U == C && L != C && R != C && D != C )	return ground->GetEdge( Direction::LRD );

	else if( U != C && L == C && R == C && D != C )	return ground->GetEdge( Direction::UD );
	else if( U == C && L != C && R != C && D == C )	return ground->GetEdge( Direction::LR );

	else return ground->GetEdge( Direction::Surround );
}

bool Map::DrawOnMap( unify::V2< int > pos, std::string ground )
{
	int groundIndex = -1;
	{
		groundIndex = FindGround( ground );
	}

	if ( pos.x < 0 || pos.x >= m_mapSize.width || pos.y < 0 || pos.y >= m_mapSize.height )
	{
		return false;
	}

	m_map[pos.x + pos.y * m_mapSize.width] = groundIndex;
	return true;
}

void Map::CollectGeometry( GeometryCache & cache, const unify::FrameLite * transform )
{
	if( !IsEnabled() )
	{
		return;
	}

	assert( m_base.get() );

	unify::V3< float > origin( 0.0f - m_mapSize.width * m_terraSize.width * 0.5f, 0, 0.0f + m_mapSize.height * m_terraSize.height * 0.5f );

	int map_squares = m_mapSize.width * m_mapSize.height;

	for( int y = 0; y < m_mapSize.height; y++ )
	{
		for( int x = 0; x < m_mapSize.width; x++ )
		{

			int type[] =
			{
				GetMap( x - 1, y - 1 ),
				GetMap( x + 0, y - 1 ),
				GetMap( x + 1, y - 1 ),
				GetMap( x - 1, y + 0 ),
				GetMap( x + 0, y + 0 ),
				GetMap( x + 1, y + 0 ),
				GetMap( x - 1, y + 1 ),
				GetMap( x + 0, y + 1 ),
				GetMap( x + 1, y + 1 ),
			};

			auto frame = GetFrame( x, y );
			assert( frame );
			*frame = *transform;
			frame->PostMul( 
				unify::MatrixTranslate(
					unify::V3< float >{ origin.x + (float)m_terraSize.width * x, origin.y + 0, origin.z - y * (float)m_terraSize.height } )
			);

			if( type[ (size_t)Direction::C ] == -1 )
			{
				cache.Add( m_base->GetGeneric( 0 ).get(), frame );
			}
			else
			{
				auto geo = GetGroundGeometry( type );
				if( !geo )
				{
					cache.Add( m_invalid.get(), frame );
				}
				else
				{
					cache.Add( geo.get(), frame );
				}
			}
		}
	}
}

void Map::OnSuspend()
{
}

void Map::OnResume()
{
}

IObjectComponent::ptr Map::Duplicate()
{
	auto duplicate = new Map( *this );
	return me::object::IObjectComponent::ptr( duplicate );
}

std::string Map::GetWhat() const
{
	return std::string();
}
