// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/factory/PixelShaderFactories.h>
#include <me/exception/FailedToCreate.h>
#include <qxml/Document.h>

using namespace me;

PixelShaderFactory::PixelShaderFactory( IGame * game )
	: m_game( game )
{
}

IPixelShader::ptr PixelShaderFactory::Produce( unify::Path source, void * data )
{
	auto renderer = m_game->GetOS()->GetRenderer( 0 );
	qxml::Document doc( source );
	auto && root = *doc.GetRoot()->FindFirstElement( "pixelshader" );

	PixelShaderParameters parameters;
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
			parameters.constants.reset( new shader::ConstantBuffer( &node ) );
		}
	}
	return renderer->ProducePS( parameters );
}
