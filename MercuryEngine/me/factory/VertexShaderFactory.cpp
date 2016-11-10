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
	auto & node = *doc.GetRoot()->FindFirstElement( "vertexshader" );

	VertexShaderParameters parameters;
	parameters.path = node.GetDocument()->GetPath().DirectoryOnly() + node.GetElement( "source" )->GetText();
	parameters.entryPointName = node.GetElement( "entry" )->GetText();
	parameters. profile = node.GetElement( "profile" )->GetText();
	parameters.vertexDeclaration = renderer->ProduceVD( node.GetElement( "vd" ) );
	return renderer->ProduceVS( parameters );
}
