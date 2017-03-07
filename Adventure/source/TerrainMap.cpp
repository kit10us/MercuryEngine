// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <TerrainMap.h>
#include <MEWinMain.h>
#include <me/scene/SceneManager.h>
#include <me/scene/Scene.h>
#include <me/scene/CameraComponent.h>
#include <me/Mesh.h>
#include <me/Terra.h>
#include <me/factory/EffectFactories.h>
#include <me/scene/GeometryComponent.h>

using namespace me;
using namespace scene;

TerrainMap::TerrainMap( IOS * os, unify::Size< int > mapSize, unify::Size< int > terraSize )
	: ObjectComponent( os )
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

TerrainMap::~TerrainMap()
{
	delete[] m_map;
}


std::string TerrainMap::GetType() const
{
	return "Terrain Map";
}

std::string TerrainMap::GetWhat() const
{
	return std::string();
}

bool TerrainMap::Renderable() const
{ 
	return true; 
}

void TerrainMap::OnInit()
{
}

void TerrainMap::OnStart()
{
}

void TerrainMap::OnUpdate( UpdateParams params )
{
}

void TerrainMap::CollectGeometry( GeometryCache & cache, const unify::FrameLite * transform )
{
	if ( !IsEnabled() )
	{
		return;
	}

	for ( int y = 0; y < m_mapSize.height; y++ )
	{
		for ( int x = 0; x < m_mapSize.width; x++ )
		{
			size_t square = y * m_mapSize.height + x;
			int type = m_map[ square ];
			auto & frame = m_frames[ square ];
			if ( type == 0 )
			{
				cache.Add( m_invalidGeo.get(), &frame );
			}
		}
	}
}

void TerrainMap::OnSuspend()
{
}

void TerrainMap::OnResume()
{
}

IObjectComponent * TerrainMap::Duplicate()
{
	return nullptr;
}

void TerrainMap::SetInvalidGeometry( Geometry::ptr geo )
{
	m_invalidGeo = geo;
}
