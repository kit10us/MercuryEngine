// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/ResourceManager.h>
#include <dxi/TextureManager.h>
#include <dxi/PixelShaderManager.h>
#include <dxi/VertexShaderFactory.h>
#include <dxi/EffectManager.h>
#include <dxi/geo/Geometry.h>
#include <memory>

namespace dxi
{
	class GeometryManager : public ResourceManager< geo::Geometry >
	{
	public:
		typedef std::shared_ptr< GeometryManager > shared_ptr;
		typedef std::weak_ptr< GeometryManager > weak_ptr;

		GeometryManager( TextureManager::shared_ptr textureManager = TextureManager::shared_ptr(), PixelShaderManager::shared_ptr pixelShaderManager = PixelShaderManager::shared_ptr(), std::shared_ptr< ResourceManager< VertexShader > > vertexShaderManager = std::shared_ptr< ResourceManager< VertexShader > >(), EffectManager::shared_ptr = EffectManager::shared_ptr() );
		virtual ResourcePtr LoadFromFile( const ResourceID & name, const unify::Path & filePath );
		TextureManager::shared_ptr GetTextureManager() const;
		PixelShaderManager::shared_ptr GetPixelShaderManager() const;
		std::shared_ptr< ResourceManager< VertexShader > > GetVertexShaderManager() const;
		EffectManager::shared_ptr GetEffectManager() const;

	private:
		TextureManager::shared_ptr m_textureManager;
		PixelShaderManager::shared_ptr m_pixelShaderManager;
		VertexShaderManager m_vertexShaderManager;
		EffectManager::shared_ptr m_effectManager;
	};
}