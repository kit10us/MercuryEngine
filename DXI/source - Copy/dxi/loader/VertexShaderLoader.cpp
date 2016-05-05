// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/loader/VertexShaderLoader.h>
#include <qxml/Document.h>

using namespace dxi;

VertexShader * loader::VertexShaderLoader( const qxml::Element * node )
{
	VertexShader * shader = new VertexShader;

	if ( node->HasAttributes( "source" ) )
	{
		// Create from a file source if specified...
		unify::Path path( node->GetDocument()->GetPath().DirectoryOnly(), node->GetStringAttribute( "source" ) );
		shader->CreateFromFile( path, node->GetStringAttribute( "entry" ), node->GetStringAttribute( "version" ) );
	}
	else if ( node->HasElements( "code" ) )
	{
		// Assume we have inline shader code...
		const qxml::Element * codeNode = node->GetElement( "code" );
		shader->CreateFromCode( codeNode->GetText(), node->GetStringAttribute( "entry" ), node->GetStringAttribute( "version" ) );
	}

	// Vertex declaration...
	if ( node->HasElements( "vd" ) )
	{
		const qxml::Element * vdNode = node->GetElement( "vd" );
		qjson::Object jsonObject( std::string( vdNode->GetText() ) );
		VertexDeclaration vertexDeclaration( jsonObject );
		shader->SetVertexDeclaration( vertexDeclaration );
	}


	return shader;
}

VertexShader::shared_ptr loader::VertexShaderLoader( const qxml::Element * node, VertexShaderManager VertexShaderManager )
{
    std::string name = node->GetStringAttribute( "name" );

	// If we specified a VertexShader manager, check if a VertexShader already exists with the same name, if so, return it.
	if ( VertexShaderManager && VertexShaderManager->Exists( name ) )
	{
		return VertexShaderManager->Find( name );
	}

	// Create the VertexShader.
	VertexShader * VertexShader = loader::VertexShaderLoader( node );

	// Add the VertexShader to the VertexShader manager, if specified.
	if ( VertexShaderManager )
	{
		return VertexShaderManager->Add( name, VertexShader );
	}
	else
	{
		return VertexShader::shared_ptr( VertexShader );
	}
}