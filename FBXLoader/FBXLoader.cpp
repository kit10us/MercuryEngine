// Copyright (c) 2003 - 2014, Evil Quail LLC
// All Rights Reserved

#include <memory.h>
#include <me/IRenderer.h>
#include <me/IGame.h>

#include "stdafx.h"
#include "FBXLoader.h"


// This is an example of an exported variable
FBXLOADER_API int nFBXLoader=0;

// This is an example of an exported function.
FBXLOADER_API int fnFBXLoader(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
// see FBXLoader.h for the class definition
CFBXLoader::CFBXLoader()
{
    return;
}



/*
void Deleter( dae::GeometrySourceFactory * factory )
{
	delete factory;
}
*/

extern "C" FBXLOADER_API bool DXILoader( me::IGame * game, const qxml::Document * doc );

FBXLOADER_API bool DXILoader( me::IGame * _game, const qxml::Document * doc )
{
	using namespace me;

	/*
	auto game = dynamic_cast<Game *>(_game);

	if( doc == nullptr ) 
	{
		game->ReportError( me::ErrorLevel::Failure, "DAELoader", "Configuraiton file missing!" );
	}

	// Setup DAE factory.
	class MyEffectSolver : public dae::util::IEffectSolver
	{
		Game * m_game;
		Effect::ptr m_color;
		Effect::ptr m_textured;
	public:

		MyEffectSolver( Game * game, std::string colorEffectName, unify::Path colorEffectPath, std::string textureEffectName, unify::Path textureEffectPath )
			: m_game( game )
		{
			m_color = GetGame()->GetManager< Effect >()->Add( colorEffectName, colorEffectPath );
			m_textured = GetGame()->GetManager< Effect >()->Add( textureEffectName, textureEffectPath );
		}

		Game * GetGame() override
		{
			return m_game;
		}				  

		Effect::ptr GetEffect( const dae::Shading & shading ) const
		{
			if( shading.GetDiffuse().GetType() == dae::Shading::Property::ColorType )
			{
				return m_color;
			}
			else
			{
				return m_textured;
			}
		}
	};

	const auto color = doc->GetRoot()->FindFirstElement( "color" );
	std::string colorEffectName = color->GetAttribute< std::string >( "name" );
	unify::Path colorEffectPath = color->GetAttribute< std::string >( "source" );

	const auto texture = doc->GetRoot()->FindFirstElement( "texture" );
	std::string textureEffectName = texture->GetAttribute< std::string >( "name" );
	unify::Path textureEffectPath = texture->GetAttribute< std::string >( "source" );

	dae::GeometrySourceFactory * daeFactory = new dae::GeometrySourceFactory( game->GetOS()->GetRenderer( 0 ), new MyEffectSolver( game, colorEffectName, colorEffectPath, textureEffectName, textureEffectPath ) );
	game->GetManager< Geometry >()->AddFactory( ".dae", GeometryFactoryPtr( daeFactory ) );
	*/

	return true;
}

