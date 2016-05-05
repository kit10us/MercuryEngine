// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/Texture.h>
#include <dxi/ResourceManager.h>
#include <memory>

namespace dxi
{
	class TextureManager : public ResourceManager< Texture > 
	{
	public:
		typedef std::shared_ptr< TextureManager > shared_ptr;
		typedef std::weak_ptr< TextureManager > weak_ptr;

		TextureManager();
		virtual ResourcePtr LoadFromFile( const ResourceID & name, const unify::Path & filePath, unsigned int flags = 0 );
	};
}