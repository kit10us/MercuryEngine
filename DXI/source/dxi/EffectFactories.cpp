// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/EffectFactories.h>
#include <dxi/PixelShaderFactories.h>
#include <dxi/VertexShaderFactory.h>
#include <dxi/exception/FailedToCreate.h>
#include <dxi/core/Game.h>

using namespace dxi;

Effect * dxi::ProduceEffect( const qxml::Element * effectNode )
{
	assert( effectNode );

	auto textureManager = dxi::core::Game::GetGameInstance()->GetManager< Texture >();
	auto pixelShaderManager = dxi::core::Game::GetGameInstance()->GetManager< PixelShader >();
	auto vertexShaderManager = dxi::core::Game::GetGameInstance()->GetManager< VertexShader >();

	Effect * effect = new Effect;

	for( auto&& child : effectNode->Children() )
	{
		//void SetName( const std::string & sName );
		//void SetFlags( unsigned int dwFlags );

		if( child.IsTagName( "texture" ) )
		{
			std::string name = child.GetAttribute< std::string >( "name" );
			unsigned char stage = static_cast< unsigned char >(child.GetAttributeElse< int >( "stage", 0 ));
			unify::Path source = child.GetDocument()->GetPath().DirectoryOnly() + child.GetAttribute< std::string >( "source" );
			effect->SetTexture( stage, textureManager->Add( name, source ) );
		}
		else if( child.IsTagName( "blend" ) )
		{
			std::string blend = child.GetText();
			effect->SetBlend( Blend( blend ) );
		}

		//void SetCulling( unsigned int dwValue );
		//void SetLighting( unsigned int dwValue );
		else if( child.IsTagName( "pixelshader" ) )
		{
			effect->SetPixelShader( pixelShaderManager->Add( &child ) );
		}
		else if( child.IsTagName( "vertexshader" ) )
		{
			effect->SetVertexShader( vertexShaderManager->Add( &child ) );
		}
		//void AddFrame( size_t frameIndex, float influence );
	}

	return effect;
}

Effect * EffectSourceFactory::Produce( unify::Path path )
{
	qxml::Document doc( path );
	const qxml::Element * childNode = doc.GetRoot()->FindFirstElement( "effect" );
	return ProduceEffect( childNode );
}