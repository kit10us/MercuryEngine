// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dae/GeometrySourceFactory.h>
#include <memory.h>
#include <me/IRenderer.h>
#include <me/IGame.h>

void Deleter( dae::GeometrySourceFactory * factory )
{
	delete factory;
}

extern "C" __declspec(dllexport) bool MELoader( me::IGame * game, const qxml::Element * element );

__declspec(dllexport) bool MELoader( me::IGame * _game, const qxml::Element * element )
{
	using namespace me;

	auto game = dynamic_cast<Game *>(_game);

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

	const auto color = element->FindFirstElement( "color" );
	std::string colorEffectName = color->GetAttribute< std::string >( "name" );
	unify::Path colorEffectPath = color->GetAttribute< std::string >( "source" );

	const auto texture = element->FindFirstElement( "texture" );
	std::string textureEffectName = texture->GetAttribute< std::string >( "name" );
	unify::Path textureEffectPath = texture->GetAttribute< std::string >( "source" );

	dae::GeometrySourceFactory * daeFactory = new dae::GeometrySourceFactory( game->GetOS()->GetRenderer( 0 ), new MyEffectSolver( game, colorEffectName, colorEffectPath, textureEffectName, textureEffectPath ) );
	game->GetManager< Geometry >()->AddFactory( ".dae", GeometryFactoryPtr( daeFactory ) );

	return true;
}

