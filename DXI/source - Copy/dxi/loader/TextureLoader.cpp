// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/loader/TextureLoader.h>
#include <qxml/Document.h>

using namespace dxi;

Texture * loader::TextureLoader( const qxml::Element * node )
{
	// Collect texture parameters.
	unify::Path source(node->GetDocument()->GetPath());
	source.ChangeFilename( node->GetStringAttributeElse("source", std::string()) );
    unsigned int flags = 0;
    if ( node->HasAttributes( "lockable" ) )
    {
        flags |= TEXTURE_LOCKABLE;
    }
    if ( node->HasAttributes( "norender" ) )
    {
        flags |= TEXTURE_NORENDER;
    }

	// Create the texture.
	return new Texture( source, flags );

}

Texture::shared_ptr loader::TextureLoader( const qxml::Element * node, dxi::TextureManager::shared_ptr textureManager )
{
    std::string name = node->GetStringAttribute( "name" );

	// If we specified a texture manager, check if a texture already exists with the same name, if so, return it.
	if ( textureManager && textureManager->Exists( name ) )
	{
		return textureManager->Find( name );
	}

	// Create the texture.
	Texture * texture = loader::TextureLoader( node );

	// Add the texture to the texture manager, if specified.
	if ( textureManager )
	{
		return textureManager->Add( name, texture );
	}
	else
	{
		return Texture::shared_ptr( texture );
	}
}