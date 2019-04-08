// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/factory/PixelShaderFactories.h>
#include <me/render/Blend.h>
#include <me/exception/FailedToCreate.h>
#include <me/render/IRenderer.h>
#include <qxml/Document.h>

using namespace me;
using namespace render;

PixelShaderFactory::PixelShaderFactory( game::IGame * gameInstance )
	: m_game( gameInstance )
{
}

IPixelShader::ptr PixelShaderFactory::Produce( unify::Path source, unify::Parameters parameters )
{
	auto renderer = m_game->GetOS()->GetRenderer( 0 );

	qxml::Document doc {};
	m_game->Debug()->Try( [&]
	{
		doc.Load( source );
	}, ErrorLevel::Failure, false, true );
	auto && root = *doc.GetRoot()->FindFirstElement( "pixelshader" );

	PixelShaderParameters pixelShaderParameters;
	for ( auto && node : root.Children() )
	{
		if ( node.IsTagName( "source" ) )
		{
			pixelShaderParameters.path = m_game->GetOS()->GetAssetPaths()->FindAsset( unify::Path( node.GetText() ), node.GetDocument()->GetPath().DirectoryOnly() );
		}
		else if ( node.IsTagName( "entry" ) )
		{
			pixelShaderParameters.entryPointName = node.GetText();
		}
		else if ( node.IsTagName( "profile" ) )
		{
			pixelShaderParameters.profile = node.GetText();
		}
		else if ( node.IsTagName( "constants" ) )
		{
			pixelShaderParameters.constantTable = render::ConstantTable( &node );
		}
		else if( node.IsTagName( "blend" ) )
		{
			pixelShaderParameters.trans = true;
			pixelShaderParameters.blendDesc = BlendDesc( &node );
		}
		else if( node.IsTagName( "trans" ) )
		{
			pixelShaderParameters.trans = unify::Cast< bool >( node.GetText() );
		}
	}
	return renderer->ProducePS( pixelShaderParameters );
}

IPixelShader::ptr PixelShaderFactory::Produce( unify::Parameters parameters )
{
	throw me::exception::FailedToCreate( "Attempted to create pixel shader from raw data." );
}