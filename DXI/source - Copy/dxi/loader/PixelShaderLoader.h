// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/PixelShaderManager.h>
#include <qxml/Document.h>

namespace dxi
{
    namespace loader
    {
		// Converts a node into a pixel shader.
		PixelShader * PixelShaderLoader( const qxml::Element * node );

		// If a manager is included, the resource is automatically added to the manager, or if the resource already exists, it is returned instead of creating a new one.
		PixelShader::shared_ptr PixelShaderLoader( const qxml::Element * node, PixelShaderManager::shared_ptr pixelShaderManager );
    }
}