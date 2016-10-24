// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/factory/EffectFactories.h>
#include <dxi/factory/PixelShaderFactories.h>
#include <dxi/factory/VertexShaderFactory.h>
#include <dxi/exception/FailedToCreate.h>
#include <dxi/core/Game.h>

using namespace dxi;

EffectFactory::EffectFactory( core::IGame * game )
	: m_game( game )
{
}
	  
std::shared_ptr< Effect > EffectFactory::Produce( unify::Path source )
{
	auto game = dynamic_cast<core::Game *>(m_game);
	auto textureManager = game->GetManager< Texture >();
	auto pixelShaderManager = game->GetManager< PixelShader >();
	auto vertexShaderManager = game->GetManager< VertexShader >();

	qxml::Document doc( source );
	auto & effectNode = *doc.GetRoot()->FindFirstElement( "effect" );

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
			// TODO:
		}

		//void SetCulling( unsigned int dwValue );
		//void SetLighting( unsigned int dwValue );
		else if( child.IsTagName( "pixelshader" ) )
		{
			effect->SetPixelShader( pixelShaderManager->Add( child.GetAttribute< std::string >( "name" ), doc.GetPath().DirectoryOnly() + child.GetAttribute< std::string >( "source" ) ) );
		}
		else if( child.IsTagName( "vertexshader" ) )
		{
			effect->SetVertexShader( vertexShaderManager->Add( child.GetAttribute< std::string >( "name" ), doc.GetPath().DirectoryOnly() + child.GetAttribute< std::string >( "source" ) ) );
		}
		//void AddFrame( size_t frameIndex, float influence );
	}

	return Effect::ptr( effect );
}