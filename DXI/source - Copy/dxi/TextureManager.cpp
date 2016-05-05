// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/TextureManager.h>
#include <dxi/TextureFactory.h>

using namespace dxi;

TextureManager::TextureManager()
: ResourceManager( ResourceDesc( "", "dds;jpg;bmp" ), std::shared_ptr< ResourceManager< Texture >::IFactory >( new TextureFactory ) )
{
}

TextureManager::ResourcePtr TextureManager::LoadFromFile( const ResourceID & name, const unify::Path & filePath, unsigned int flags )
{
	ResourcePtr resource( Find( name ) );
	if( resource.use_count() )
	{
		return resource;
	}
	return Add( name, new Texture( filePath, flags ) );
}
