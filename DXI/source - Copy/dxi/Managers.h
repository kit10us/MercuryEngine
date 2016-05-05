// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/TextureManager.h>
#include <dxi/VertexShaderFactory.h>
#include <dxi/PixelShaderManager.h>
#include <dxi/EffectManager.h>
#include <dxi/GeometryManager.h>

namespace dxi
{
	///<summary>
	/// Simplify the passing of managers by grouping all important managers in one container. 
	///</summary>
	class Managers
	{
		TextureManager::shared_ptr m_textureManager;
		VertexShaderManager m_vertexShaderManager;
		PixelShaderManager::shared_ptr m_pixelShaderManager;
		EffectManager::shared_ptr m_effectManager;
		GeometryManager::shared_ptr m_geometryManager;

	public:
		Managers( TextureManager::shared_ptr textureManager = TextureManager::shared_ptr(), VertexShaderManager vertexShaderManager = VertexShaderManager(), PixelShaderManager::shared_ptr pixelShaderManager = PixelShaderManager::shared_ptr(), EffectManager::shared_ptr effectManager = EffectManager::shared_ptr(), GeometryManager::shared_ptr geometryManager = GeometryManager::shared_ptr() );
		Managers( VertexShaderManager vertexShaderManager, PixelShaderManager::shared_ptr pixelShaderManager = PixelShaderManager::shared_ptr(), EffectManager::shared_ptr effectManager = EffectManager::shared_ptr(), GeometryManager::shared_ptr geometryManager = GeometryManager::shared_ptr() );
		Managers( PixelShaderManager::shared_ptr pixelShaderManager, EffectManager::shared_ptr effectManager = EffectManager::shared_ptr(), GeometryManager::shared_ptr geometryManager = GeometryManager::shared_ptr() );
		Managers( EffectManager::shared_ptr effectManager, GeometryManager::shared_ptr geometryManager = GeometryManager::shared_ptr() );
		Managers( GeometryManager::shared_ptr geometryManager );

		TextureManager::shared_ptr GetTextureManager() const;
		VertexShaderManager GetVertexShaderManager() const;
		PixelShaderManager::shared_ptr GetPixelShaderManager() const;
		EffectManager::shared_ptr GetEffectManager() const;
		GeometryManager::shared_ptr GetGeometryManager() const;
	};
}
