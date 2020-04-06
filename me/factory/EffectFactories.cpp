// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/factory/EffectFactories.h>
#include <me/factory/PixelShaderFactories.h>
#include <me/factory/VertexShaderFactory.h>
#include <me/exception/FailedToCreate.h>
#include <me/game/Game.h>

using namespace me;
using namespace render;

EffectFactory::EffectFactory( game::IGame * gameInstance )
	: m_game( gameInstance )
{
}
	  
std::shared_ptr< Effect > EffectFactory::Produce( unify::Path source, unify::Parameters parameters )
{
	auto debug = m_game->GetOS()->Debug();
	auto block{ debug->MakeBlock( "EffectFactories::Produce(" + source.ToString() + ")" ) };

	auto gameInstance = dynamic_cast< game::Game * >(m_game);
	auto textureManager = gameInstance->GetManager< ITexture >();
	auto pixelShaderManager = gameInstance->GetManager< IPixelShader >();
	auto vertexShaderManager = gameInstance->GetManager< IVertexShader >();

	Effect* effect{};

	qxml::Document doc;
	const qxml::Element * effectNode {};
	
	// Load document.
	m_game->GetOS()->Debug()->Try( [&]
	{
		doc.Load( source );
		effectNode = doc.GetRoot()->FindFirstElement( "effect" );
	}, debug::ErrorLevel::Failure, false, true );

	effect = new Effect( source );

	for ( auto&& child : effectNode->Children() )
	{
		//void SetName( std::string sName );
		//void SetFlags( unsigned int dwFlags );

		// Load textures.
		if ( child.IsTagName( "texture" ) )
		{
			m_game->GetOS()->Debug()->Try( [&]
			{
				unify::Parameters parameters {};
				{
					child.AttributesToParameters( parameters,
					{ { "min" }, { "mag" }, { "mip" }, { "lockaccess.cpu" }, { "lockaccess.gpu" }, { "source" }, { "width" }, { "height" } }
					);
				};

				std::string name = child.GetAttributeElse< std::string >( "name", unify::Path( parameters.Get< std::string >( "source" ) ).FilenameNoExtension() );
				unsigned char stage = child.GetAttributeElse< unsigned char >( "stage", 0 );

				effect->SetTexture( stage, textureManager->Add( name, unify::Path( parameters.Get< std::string >( "source" ) ), child.GetDocument()->GetPath().DirectoryOnly(), parameters ) );
			}, debug::ErrorLevel::Failure );
		}

		//void SetCulling( unsigned int dwValue );
		//void SetLighting( unsigned int dwValue );

		// Load pixel shader.
		else if ( child.IsTagName( "pixelshader" ) )
		{
			m_game->GetOS()->Debug()->Try( [&]
			{
				auto path = unify::Path( child.GetAttribute< std::string >( "source" ) );
				unify::Path source = m_game->GetOS()->GetAssetPaths()->FindAsset( path, doc.GetPath().DirectoryOnly() );
				effect->SetPixelShader( pixelShaderManager->Add( child.GetAttributeElse< std::string >( "name", path.FilenameNoExtension() ), source ) );
			}, debug::ErrorLevel::Failure, true, true );
		}

		// Load 
		else if ( child.IsTagName( "vertexshader" ) )
		{
			m_game->GetOS()->Debug()->Try( [&]
			{
				auto path = unify::Path( child.GetAttribute< std::string >( "source" ) );
				unify::Path source = m_game->GetOS()->GetAssetPaths()->FindAsset( path, doc.GetPath().DirectoryOnly() );
				effect->SetVertexShader( vertexShaderManager->Add( child.GetAttributeElse< std::string >( "name", path.FilenameNoExtension() ), source ) );
			}, debug::ErrorLevel::Failure, true, true );
		}
		//void AddFrame( size_t frameIndex, float influence );
	}

	return Effect::ptr( effect );
}

std::shared_ptr< Effect > EffectFactory::Produce( unify::Parameters parameters )
{
	throw me::exception::FailedToCreate( "Attempted to create effect from parameters." );
}
