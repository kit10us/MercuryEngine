// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/factory/EffectFactories.h>
#include <dxi/factory/PixelShaderFactories.h>
#include <dxi/factory/VertexShaderFactory.h>
#include <dxi/exception/FailedToCreate.h>
#include <dxi/core/Game.h>

using namespace dxi;

Effect * ProduceEffect( const qxml::Element & effectNode )
{
	auto textureManager = dxi::core::Game::GetInstance()->GetManager< Texture >();
	auto pixelShaderManager = dxi::core::Game::GetInstance()->GetManager< PixelShader >();
	auto vertexShaderManager = dxi::core::Game::GetInstance()->GetManager< VertexShader >();

	Effect * effect = new Effect;

	for( auto&& child : effectNode.Children() )
	{
		//void SetName( const std::string & sName );
		//void SetFlags( unsigned int dwFlags );

		if( child.IsTagName( "texture" ) )
		{
			std::string name = child.GetAttribute< std::string >( "name" );
			unsigned char stage = child.GetAttributeElse< unsigned char >( "stage", 0 );
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
			effect->SetPixelShader( pixelShaderManager->Add( child ) );
		}
		else if( child.IsTagName( "vertexshader" ) )
		{
			effect->SetVertexShader( vertexShaderManager->Add( child ) );
		}
		//void AddFrame( size_t frameIndex, float influence );
	}

	return effect;
}

Effect * EffectXMLFactory::Produce( const qxml::Element & element )
{
	return ProduceEffect( element );
}