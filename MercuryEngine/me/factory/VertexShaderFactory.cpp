// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/Game.h>
#include <me/factory/VertexShaderFactory.h>
#include <me/exception/FailedToCreate.h>
#include <qxml/Document.h>

using namespace me;

VertexShaderFactory::VertexShaderFactory( me::IGame * game )
	: m_game( game )
{
}

IVertexShader::ptr VertexShaderFactory::Produce( unify::Path source, void * data )
{
	auto renderer = m_game->GetOS()->GetRenderer( 0 );

	qxml::Document doc( source );
	auto && root = *doc.GetRoot()->FindFirstElement( "vertexshader" );

	VertexShaderParameters parameters;
	for ( auto && node : root.Children() )
	{
		if ( node.IsTagName( "source" ) )
		{
			parameters.path = m_game->GetOS()->GetAssetPaths().FindAsset( node.GetText(), node.GetDocument()->GetPath().DirectoryOnly() );
		}
		else if ( node.IsTagName( "entry" ) )
		{
			parameters.entryPointName = node.GetText();
		}
		else if ( node.IsTagName( "profile" ) )
		{
			parameters.profile = node.GetText();
		}
		else if ( node.IsTagName( "constants" ) )
		{
			parameters.constants.reset( new shader::ShaderConstants( &node ) );
		}
		else if ( node.IsTagName( "vd" ) )
		{
			parameters.vertexDeclaration.reset( new VertexDeclaration( &node ) );
		}
	}
	return renderer->ProduceVS( parameters );
}
