// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/loader/PixelShaderLoader.h>
#include <qxml/Document.h>

using namespace dxi;

PixelShader * loader::PixelShaderLoader( const qxml::Element * node )
{
	PixelShader * shader = new PixelShader;

	if ( node->HasAttributes( "source" ) )
	{
		// Create from a file source if specified...
		unify::Path path( node->GetDocument()->GetPath().DirectoryOnly(), node->GetStringAttribute( "source" ) );
		shader->CreateFromFile( path, node->GetStringAttribute( "entry" ), node->GetStringAttribute( "version" ) );
	}
	else
	{
		// Assume we have inline shader code...
		shader->CreateFromCode( node->GetText(), node->GetStringAttribute( "entry" ), node->GetStringAttribute( "version" ) );
	}

	return shader;
}

PixelShader::shared_ptr loader::PixelShaderLoader( const qxml::Element * node, PixelShaderManager::shared_ptr PixelShaderManager )
{
    std::string name = node->GetStringAttribute( "name" );

	// If we specified a PixelShader manager, check if a PixelShader already exists with the same name, if so, return it.
	if ( PixelShaderManager && PixelShaderManager->Exists( name ) )
	{
		return PixelShaderManager->Find( name );
	}

	// Create the PixelShader.
	PixelShader * PixelShader = loader::PixelShaderLoader( node );

	// Add the PixelShader to the PixelShader manager, if specified.
	if ( PixelShaderManager )
	{
		return PixelShaderManager->Add( name, PixelShader );
	}
	else
	{
		return PixelShader::shared_ptr( PixelShader );
	}
}