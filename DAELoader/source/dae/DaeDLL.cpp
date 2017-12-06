// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <dae/GeometrySourceFactory.h>
#include <memory.h>
#include <me/render/IRenderer.h>
#include <me/IGame.h>

void Deleter( dae::GeometrySourceFactory * factory )
{
	delete factory;
}

extern "C" __declspec(dllexport) bool MELoader( me::IGame * game, const qxml::Element * element );

__declspec(dllexport) bool MELoader( me::IGame * _game, const qxml::Element * element )
{
	using namespace me;
	using namespace render;

	auto game = dynamic_cast<Game *>(_game);

	// Setup DAE factory.
	class MyEffectSolver : public dae::util::IEffectSolver
	{
		Game * m_game;
		Effect::ptr m_default_color;
		Effect::ptr m_default_texture;
	public:

		MyEffectSolver( Effect::ptr default_color, Effect::ptr default_texture )
			: m_default_color{ default_color }
			, m_default_texture{ default_texture }
		{
		}

		Effect::ptr GetEffect( const dae::Effect * effect ) const
		{
			// Assume default color texture if effect is null...
			if( effect == nullptr )
			{
				return m_default_color;
			}

			auto & shading = effect->GetProfileCOMMON()->GetTechnique().GetShading();
			if( shading.GetDiffuse().GetType() == dae::Shading::Property::ColorType )
			{
				return m_default_color;
			}
			else
			{
				return m_default_texture;
			}
		}
	};


	Effect::ptr color;
	{
		IPixelShader::ptr ps;
		{
			const auto node = element->FindFirstElement( "colorps" );
			std::string name = node->GetAttributeElse< std::string >( "name", std::string() );
			unify::Path path( node->GetAttributeElse< std::string >( "source", std::string() ) );

			if( !name.empty() && path.Empty() )
			{
				ps = game->GetManager< IPixelShader >()->Find( name );
			}
			else if( name.empty() && ! path.Empty() )
			{
				ps = game->GetManager< IPixelShader >()->Add( path );
			}
			else if( !name.empty() && !path.Empty() )
			{
				ps = game->GetManager< IPixelShader >()->Add( name, path );
			}
		}

		IVertexShader::ptr vs;
		{
			const auto node = element->FindFirstElement( "colorvs" );
			std::string name = node->GetAttributeElse< std::string >( "name", std::string() );
			unify::Path path( node->GetAttributeElse< std::string >( "source", std::string() ) );

			if( !name.empty() && path.Empty() )
			{
				vs = game->GetManager< IVertexShader >()->Find( name );
			}
			else if( name.empty() && !path.Empty() )
			{
				vs = game->GetManager< IVertexShader >()->Add( path );
			}
			else if( !name.empty() && !path.Empty() )
			{
				vs = game->GetManager< IVertexShader >()->Add( name, path );
			}
		}
		color.reset( new Effect( vs, ps ) );
	}
	
	Effect::ptr textured;
	{
		IPixelShader::ptr ps;
		{
			const auto node = element->FindFirstElement( "texturedps" );

			std::string name = node->GetAttributeElse< std::string >( "name", std::string() );
			unify::Path path( node->GetAttributeElse< std::string >( "source", std::string() ) );

			if( !name.empty() && path.Empty() )
			{
				ps = game->GetManager< IPixelShader >()->Find( name );
			}
			else if( name.empty() && !path.Empty() )
			{
				ps = game->GetManager< IPixelShader >()->Add( path );
			}
			else if( !name.empty() && !path.Empty() )
			{
				ps = game->GetManager< IPixelShader >()->Add( name, path );
			}
		}

		IVertexShader::ptr vs;
		{
			const auto node = element->FindFirstElement( "texturedvs" );
			std::string name = node->GetAttributeElse< std::string >( "name", std::string() );
			unify::Path path( node->GetAttributeElse< std::string >( "source", std::string() ) );

			if( !name.empty() && path.Empty() )
			{
				vs = game->GetManager< IVertexShader >()->Find( name );
			}
			else if( name.empty() && !path.Empty() )
			{
				vs = game->GetManager< IVertexShader >()->Add( path );
			}
			else if( !name.empty() && !path.Empty() )
			{
				vs = game->GetManager< IVertexShader >()->Add( name, path );
			}
		}
		textured.reset( new Effect( vs, ps ) );
	}

	auto effectSolver = new MyEffectSolver( color, textured );

	dae::GeometrySourceFactory * daeFactory = new dae::GeometrySourceFactory( game, effectSolver );
	game->GetManager< Geometry >()->AddFactory( ".dae", GeometryFactoryPtr( daeFactory ) );

	return true;
}

