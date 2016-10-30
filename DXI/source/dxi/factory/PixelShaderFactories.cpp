// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/factory/PixelShaderFactories.h>
#include <dxi/exception/FailedToCreate.h>
#include <qxml/Document.h>

using namespace dxi;

PixelShaderFactory::PixelShaderFactory( core::IGame * game )
	: m_game( game )
{
}

PixelShader::ptr PixelShaderFactory::Produce( unify::Path source, void * data )
{
	qxml::Document doc( source );
	auto & node = *doc.GetRoot()->FindFirstElement( "pixelshader" );
	unify::Path shaderSource{ node.GetDocument()->GetPath().DirectoryOnly() + node.GetElement( "source" )->GetText() };
	std::string entry = node.GetElement( "entry" )->GetText();
	std::string profile = node.GetElement( "profile" )->GetText();
	return PixelShader::ptr( new PixelShader( m_game->GetOS()->GetRenderer(0), shaderSource, entry, profile ) );
}
