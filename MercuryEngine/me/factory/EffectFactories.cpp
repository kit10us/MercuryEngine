// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <me/factory/EffectFactories.h>
#include <me/factory/PixelShaderFactories.h>
#include <me/factory/VertexShaderFactory.h>
#include <me/exception/FailedToCreate.h>
#include <me/Game.h>

using namespace me;

EffectFactory::EffectFactory( me::IGame * game )
	: m_game( game )
{
}
	  
std::shared_ptr< Effect > EffectFactory::Produce( unify::Path source, void * data )
{
	auto game = dynamic_cast<Game *>(m_game);
	auto textureManager = game->GetManager< ITexture >();
	auto pixelShaderManager = game->GetManager< IPixelShader >();
	auto vertexShaderManager = game->GetManager< IVertexShader >();

	Effect* effect{};
	try
	{
		qxml::Document doc( source );
		auto & effectNode = *doc.GetRoot()->FindFirstElement( "effect" );

		effect = new Effect( source );

		for ( auto&& child : effectNode.Children() )
		{
			//void SetName( const std::string & sName );
			//void SetFlags( unsigned int dwFlags );

			if ( child.IsTagName( "texture" ) )
			{
				std::string name = child.GetAttribute< std::string >( "name" );
				unsigned char stage = child.GetAttributeElse< unsigned char >( "stage", 0 );

				TextureParameters parameters;
				if ( child.HasAttributes( "min" ) )
				{
					parameters.min = Filtering::FromString( child.GetAttribute< std::string >( "min" ) );
				}
				if ( child.HasAttributes( "mag" ) )
				{
					parameters.mag = Filtering::FromString( child.GetAttribute< std::string >( "mag" ) );
				}
				if ( child.HasAttributes( "mip" ) )
				{
					parameters.mip = Filtering::FromString( child.GetAttribute< std::string >( "mip" ) );
				}
				if ( child.HasAttributes( "lockable" ) )
				{
					parameters.lockable = child.GetAttribute< bool >( "lockable" );
				}
				if ( child.HasAttributes( "renderable" ) )
				{
					parameters.renderable = child.GetAttribute< bool >( "renderable" );
				}

				unify::Path source( child.GetAttribute< std::string >( "source" ) );
				effect->SetTexture( stage, textureManager->Add( name, source, child.GetDocument()->GetPath().DirectoryOnly(), &parameters ) );
			}
			else if ( child.IsTagName( "blend" ) )
			{
				std::string blend = child.GetText();
				// TODO:
			}

			//void SetCulling( unsigned int dwValue );
			//void SetLighting( unsigned int dwValue );
			else if ( child.IsTagName( "pixelshader" ) )
			{
				unify::Path source( child.GetAttribute< std::string >( "source" ) );
				effect->SetPixelShader( pixelShaderManager->Add( child.GetAttribute< std::string >( "name" ), source ) );
			}
			else if ( child.IsTagName( "vertexshader" ) )
			{
				unify::Path source = m_game->GetOS()->GetAssetPaths().FindAsset( unify::Path( child.GetAttribute< std::string >( "source" ) ), doc.GetPath().DirectoryOnly() );
				effect->SetVertexShader( vertexShaderManager->Add( child.GetAttribute< std::string >( "name" ), source ) );
			}
			//void AddFrame( size_t frameIndex, float influence );
		}
	}
	catch ( std::exception & ex )
	{
		throw me::exception::FailedToCreate( "Failed to create Effect \"" + source.ToString() + "\", due to the following error:\n" + ex.what() );
	}

	return Effect::ptr( effect );
}