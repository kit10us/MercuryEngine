// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <TerrainSceneComponent.h>

using namespace me;
using namespace render;
using namespace scene;
using namespace terrain;

TerrainSceneComponent::TerrainSceneComponent( os::IOS * os, unify::Size< int > mapSize, unify::Size< int > terraSize )
	: SceneComponent( os, "TerrainSceneComponent" )
	, m_mapSize{ mapSize }
	, m_terraSize{ terraSize }
 , m_map{ new size_t[ mapSize.width * mapSize.height ] }
{
	for ( int y = 0; y < m_mapSize.height; y++ )
	{
		for ( int x = 0; x < m_mapSize.width; x++ )
		{
			size_t square = y * m_mapSize.height + x;
			m_map[ square ] = 0;
			m_frames.push_back( unify::FrameLite( unify::MatrixTranslate( { (float)terraSize.width * x, 0, y * (float)terraSize.height } ) ) );
		}
	}
}

TerrainSceneComponent::~TerrainSceneComponent()
{
	delete[] m_map;
}

unify::Size< int > TerrainSceneComponent::GetSize() const
{
	return m_mapSize;
}

void TerrainSceneComponent::SetInvalidGeometry( Geometry::ptr geo )
{
	m_invalidGeo = geo;
}

size_t TerrainSceneComponent::AddGround( std::string name, Ground::ptr ground )
{
	size_t index = m_descsList.size();
	m_descMap[ name ] = index;
	m_descsList.push_back( ground );
	return index;
}

bool TerrainSceneComponent::DrawOnMap( unify::V2< int > pos, size_t ground )
{
	if ( ground >= (int)m_descsList.size() )
	{
		return false;
	}

	m_map[ pos.x + pos.y * m_mapSize.width ] = ground;
	return true;
}

bool TerrainSceneComponent::DrawOnMap( unify::V2< int > pos, std::string name )
{
	auto itr = m_descMap.find( name );
	if ( itr == m_descMap.end() )
	{
		return false;
	}

	if ( pos.x < 0 || pos.x >= m_mapSize.width || pos.y < 0 || pos.y >= m_mapSize.height )
	{
		return false;
	}

	m_map[ pos.x + pos.y * m_mapSize.width ] = itr->second;
	return true;
}
