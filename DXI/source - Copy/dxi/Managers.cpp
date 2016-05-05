// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/Managers.h>

using namespace dxi;

Managers::Managers( TextureManager::shared_ptr textureManager, VertexShaderManager vertexShaderManager, PixelShaderManager::shared_ptr pixelShaderManager, EffectManager::shared_ptr effectManager, GeometryManager::shared_ptr geometryManager )
: m_textureManager( textureManager )
, m_vertexShaderManager( vertexShaderManager )
, m_pixelShaderManager( pixelShaderManager )
, m_effectManager( effectManager )
, m_geometryManager( geometryManager )
{
}

Managers::Managers( VertexShaderManager vertexShaderManager, PixelShaderManager::shared_ptr pixelShaderManager, EffectManager::shared_ptr effectManager, GeometryManager::shared_ptr geometryManager )
: m_textureManager()
, m_vertexShaderManager( vertexShaderManager )
, m_pixelShaderManager( pixelShaderManager )
, m_effectManager( effectManager )
, m_geometryManager( geometryManager )
{
}

Managers::Managers( PixelShaderManager::shared_ptr pixelShaderManager, EffectManager::shared_ptr effectManager, GeometryManager::shared_ptr geometryManager )
: m_textureManager()
, m_vertexShaderManager()
, m_pixelShaderManager( pixelShaderManager )
, m_effectManager( effectManager )
, m_geometryManager( geometryManager )
{
}

Managers::Managers( EffectManager::shared_ptr effectManager, GeometryManager::shared_ptr geometryManager )
: m_textureManager()
, m_vertexShaderManager()
, m_pixelShaderManager()
, m_effectManager( effectManager )
, m_geometryManager( geometryManager )
{
}

Managers::Managers( GeometryManager::shared_ptr geometryManager )
: m_textureManager()
, m_vertexShaderManager()
, m_pixelShaderManager()
, m_effectManager()
, m_geometryManager( geometryManager )
{
}

TextureManager::shared_ptr Managers::GetTextureManager() const
{
	return m_textureManager;
}

VertexShaderManager Managers::GetVertexShaderManager() const
{
	return m_vertexShaderManager;
}

PixelShaderManager::shared_ptr Managers::GetPixelShaderManager() const
{
	return m_pixelShaderManager;
}

EffectManager::shared_ptr Managers::GetEffectManager() const
{
	return m_effectManager;
}

GeometryManager::shared_ptr Managers::GetGeometryManager() const
{
	return m_geometryManager;
}
