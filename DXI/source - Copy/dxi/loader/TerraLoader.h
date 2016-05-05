// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/geo/Terra.h>
#include <dxi/TextureManager.h>
#include <dxi/EffectManager.h>
#include <dxi/GeometryManager.h>
#include <dxi/loader/LoaderUtils.h>
#include <qxml/Document.h>

namespace dxi
{
    namespace loader
    {
		geo::Terra * TerraLoader( const qxml::Element * node, TextureManager::shared_ptr textureManager = TextureManager::shared_ptr(), EffectManager::shared_ptr effectManager = EffectManager::shared_ptr() );

		// Add directly to geometry manager.
		geo::Terra::shared_ptr TerraLoader( const qxml::Element * node, GeometryManager::shared_ptr geometryManager, TextureManager::shared_ptr textureManager = TextureManager::shared_ptr(), EffectManager::shared_ptr effectManager = EffectManager::shared_ptr() );
    }
}