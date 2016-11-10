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
	auto & node = *doc.GetRoot()->FindFirstElement( "pixelshader" );
	PixelShaderParameters parameters;
	parameters.path = node.GetDocument()->GetPath().DirectoryOnly() + node.GetElement( "source" )->GetText();
	parameters.entryPointName = node.GetElement( "entry" )->GetText();
	parameters.profile = node.GetElement( "profile" )->GetText();
	return renderer->ProducePS( parameters );
}
