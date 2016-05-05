// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/PixelShader.h>
#include <dxi/ResourceManager.h>
#include <memory>

namespace dxi
{
	class PixelShaderManager : public ResourceManager< PixelShader >
	{
	public:
		typedef std::shared_ptr< PixelShaderManager > shared_ptr;
		typedef std::weak_ptr< PixelShaderManager > weak_ptr;

		PixelShaderManager();

		// If the resource is not found, load it from a file.
		virtual ResourcePtr LoadFromFile( const ResourceID & name, const unify::Path & filePath, const std::string & entryPointName, const std::string & profile );
	};
}