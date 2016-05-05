// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/PixelShaderManager.h>
#include <dxi/PixelShaderFactory.h>

using namespace dxi;

PixelShaderManager::PixelShaderManager()
: ResourceManager( ResourceDesc( "", "xml;ps" ), std::shared_ptr< ResourceManager< PixelShader >::IFactory >( new PixelShaderFactory ) )
{
}

PixelShaderManager::ResourcePtr PixelShaderManager::LoadFromFile( const ResourceID & name, const unify::Path & filePath, const std::string & entryPointName, const std::string & profile )
{
	ResourcePtr resource( Find( name ) );
	if( resource.use_count() )
	{
		return resource;
	}
	return Add( name, new PixelShader( filePath, entryPointName, profile ) );
}
