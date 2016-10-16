// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dae/GeometrySourceFactory.h>
#include "DaeDLL.h"
#include <memory.h>
#include <dxi/win/DXILib.h>
#include <dxi/win/DXRenderer.h>

void Deleter( dae::GeometrySourceFactory * factory )
{
	delete factory;
}

__declspec(dllexport) bool DXILoader( dxi::core::Game * game, const qxml::Document * doc )
{
	using namespace dxi;
	using namespace core;

	if( doc == nullptr ) return false;

	// Setup DAE factory.
	class MyEffectSolver : public dae::util::IEffectSolver
	{
		dxi::core::Game * m_game;
		Effect::ptr m_color;
		Effect::ptr m_textured;
	public:

		MyEffectSolver( Game * game, std::string colorEffectName, unify::Path colorEffectPath, std::string textureEffectName, unify::Path textureEffectPath )
			: m_game( game )
		{
			m_color = GetGame()->GetManager< Effect >()->Add( colorEffectName, colorEffectPath );
			m_textured = GetGame()->GetManager< Effect >()->Add( textureEffectName, textureEffectPath );
		}

		dxi::core::Game * GetGame() override
		{
			return m_game;
		}				  

		dxi::Effect::ptr GetEffect( const dae::Shading & shading ) const
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
	game->GetManager< dxi::Geometry >()->AddFactory( "dae", GeometryFactoryPtr( daeFactory ) );

	return true;
}

