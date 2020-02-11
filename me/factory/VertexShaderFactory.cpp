// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/game/Game.h>
#include <me/render/IRenderer.h>
#include <me/factory/VertexShaderFactory.h>
#include <me/exception/FailedToCreate.h>
#include <qxml/Document.h>

using namespace me;
using namespace render;

VertexShaderFactory::VertexShaderFactory( game::IGame * gameInstance )
	: m_game( gameInstance )
{
}

IVertexShader::ptr VertexShaderFactory::Produce( unify::Path source, unify::Parameters parameters )
{
	auto renderer = m_game->GetOS()->GetRenderer( 0 );

	qxml::Document doc( source );
	auto && root = *doc.GetRoot()->FindFirstElement( "vertexshader" );

	VertexShaderParameters vertexParameters;
	for ( auto && node : root.Children() )
	{
		if ( node.IsTagName( "source" ) )
		{
			vertexParameters.path = m_game->GetOS()->GetAssetPaths()->FindAsset( unify::Path( node.GetText() ), node.GetDocument()->GetPath().DirectoryOnly() );
		}
		else if ( node.IsTagName( "entry" ) )
		{
			vertexParameters.entryPointName = node.GetText();
		}
		else if ( node.IsTagName( "profile" ) )
		{
			vertexParameters.profile = node.GetText();
		}
		else if ( node.IsTagName( "constants" ) )
		{
			vertexParameters.constantTable = render::ConstantTable( &node );
		}
		else if ( node.IsTagName( "vertexformat" ) )
		{
			vertexParameters.vertexDeclaration.reset( new VertexDeclaration( &node ) );
		}
		else if( node.IsTagName( "trans" ) )
		{
			vertexParameters.trans = unify::Cast< bool >( node.GetText() );
		}
	}
	if ( vertexParameters.vertexDeclaration == nullptr )
	{
		throw exception::FailedToCreate( "Failed to create vertex shader as vertex format is missing!" );
	}
	return renderer->ProduceVS( vertexParameters );
}

IVertexShader::ptr VertexShaderFactory::Produce( unify::Parameters parameters )
{
	throw me::exception::FailedToCreate( "Attempted to create vertex shader from parameters." );
}