// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/GeometryManager.h>
#include <dxi/geo/Mesh.h>
#include <dxi/GeometryFactory.h>

using namespace dxi;

GeometryManager::GeometryManager( TextureManager::shared_ptr textureManager, PixelShaderManager::shared_ptr pixelShaderManager, VertexShaderManager::shared_ptr vertexShaderManager, EffectManager::shared_ptr effectManager )
: ResourceManager( ResourceDesc( "", "x;xml;ase" ), std::shared_ptr< ResourceManager< geo::Geometry >::IFactory >( new GeometryFactory ) )
, m_textureManager( textureManager ) 
, m_pixelShaderManager( pixelShaderManager )
, m_vertexShaderManager( vertexShaderManager )
, m_effectManager( effectManager )
{
}

GeometryManager::ResourcePtr GeometryManager::LoadFromFile( const ResourceID & name, const unify::Path & filePath )
{
	return Add( name, new geo::Mesh( filePath, 0, Managers( m_textureManager, m_vertexShaderManager, m_pixelShaderManager, m_effectManager ) ) );
}

TextureManager::shared_ptr GeometryManager::GetTextureManager() const
{
	return m_textureManager;
}

PixelShaderManager::shared_ptr GeometryManager::GetPixelShaderManager() const
{
	return m_pixelShaderManager;
}

VertexShaderManager::shared_ptr GeometryManager::GetVertexShaderManager() const
{
	return m_vertexShaderManager;
}

EffectManager::shared_ptr GeometryManager::GetEffectManager() const
{
	return m_effectManager;
}