// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <me/factory/ScriptFactory.h>
#include <me/exception/FailedToCreate.h>
#include <me/game/Game.h>

using namespace me;
using namespace script;

ScriptFactory::ScriptFactory( game::IGame * gameInstance )
	: m_game( gameInstance )
{
}
	  
IScript::ptr ScriptFactory::Produce( unify::Path source, void * data )
{
	auto gameInstance = dynamic_cast< game::Game * >(m_game);

	/*
	Effect* effect{};
	try
	{
		qxml::Document doc( source );
		auto & effectNode = *doc.GetRoot()->FindFirstElement( "effect" );

		effect = new Effect( source );

		for ( auto&& child : effectNode.Children() )
		{
			//void SetName( std::string sName );
			//void SetFlags( unsigned int dwFlags );

			if ( child.IsTagName( "texture" ) )
			{
				TextureParameters parameters( &child );

				std::string name = child.GetAttributeElse< std::string >( "name", parameters.source.FilenameNoExtension() );
				unsigned char stage = child.GetAttributeElse< unsigned char >( "stage", 0 );

				effect->SetTexture( stage, textureManager->Add( name, parameters.source, child.GetDocument()->GetPath().DirectoryOnly(), &parameters ) );
			}

			//void SetCulling( unsigned int dwValue );
			//void SetLighting( unsigned int dwValue );
			else if ( child.IsTagName( "pixelshader" ) )
			{
				auto path = unify::Path( child.GetAttribute< std::string >( "source" ) );
				unify::Path source = m_game->GetOS()->GetAssetPaths().FindAsset( path, doc.GetPath().DirectoryOnly() );
				effect->SetPixelShader( pixelShaderManager->Add( child.GetAttributeElse< std::string >( "name", path.FilenameNoExtension() ), source ) );
			}
			else if ( child.IsTagName( "vertexshader" ) )
			{
				auto path = unify::Path( child.GetAttribute< std::string >( "source" ) );
				unify::Path source = m_game->GetOS()->GetAssetPaths().FindAsset( path, doc.GetPath().DirectoryOnly() );
				effect->SetVertexShader( vertexShaderManager->Add( child.GetAttributeElse< std::string >( "name", path.FilenameNoExtension() ), source ) );
			}
			//void AddFrame( size_t frameIndex, float influence );
		}
	}
	catch ( std::exception & ex )
	{
		throw me::exception::FailedToCreate( "Failed to create Effect \"" + source.ToString() + "\", due to the following error:\n" + ex.what() );
	}
	*/

	return IScript::ptr( {} );
}