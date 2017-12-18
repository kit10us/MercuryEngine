// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <me/factory/PixelShaderFactories.h>
#include <me/render/Blend.h>
#include <me/exception/FailedToCreate.h>
#include <qxml/Document.h>

using namespace me;
using namespace render;

PixelShaderFactory::PixelShaderFactory( game::IGame * gameInstance )
	: m_game( gameInstance )
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
			parameters.path = m_game->GetOS()->GetAssetPaths().FindAsset( unify::Path( node.GetText() ), node.GetDocument()->GetPath().DirectoryOnly() );
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
		else if( node.IsTagName( "blend" ) )
		{
			parameters.trans = true;
			parameters.blendDesc = BlendDesc( &node );
		}
		else if( node.IsTagName( "trans" ) )
		{
			parameters.trans = unify::Cast< bool >( node.GetText() );
		}
	}
	return renderer->ProducePS( parameters );
}
