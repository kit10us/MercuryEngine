// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/game/Game.h>
#include <me/factory/VertexShaderFactory.h>
#include <me/exception/FailedToCreate.h>
#include <qxml/Document.h>

using namespace me;
using namespace render;

VertexShaderFactory::VertexShaderFactory( game::IGame * gameInstance )
	: m_game( gameInstance )
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
			parameters.path = m_game->GetOS()->GetAssetPaths()->FindAsset( unify::Path( node.GetText() ), node.GetDocument()->GetPath().DirectoryOnly() );
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
			parameters.constantTable = render::ConstantTable( &node );
		}
		else if ( node.IsTagName( "vertexformat" ) )
		{
			parameters.vertexDeclaration.reset( new VertexDeclaration( &node ) );
		}
		else if( node.IsTagName( "trans" ) )
		{
			parameters.trans = unify::Cast< bool >( node.GetText() );
		}
	}
	if ( parameters.vertexDeclaration == nullptr )
	{
		throw exception::FailedToCreate( "Failed to create vertex shader as vertex format is missing!" );
	}
	return renderer->ProduceVS( parameters );
}
