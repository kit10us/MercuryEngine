// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dae/GeometrySourceFactory.h>
#include "DaeDLL.h"
#include <memory.h>
#include <dxi/win/DXILib.h>

void Deleter( dae::GeometrySourceFactory * factory )
{
	delete factory;
}

__declspec(dllexport) bool DXILoader( dxi::core::Game * game )
{
	using namespace dxi;
	using namespace core;

	// Setup DAE factory.
	class MyEffectSolver : public dae::util::IEffectSolver
	{
		Effect::ptr m_color;
		Effect::ptr m_textured;
	public:

		MyEffectSolver( Game & game )
		{
			m_color = game.GetManager< Effect >()->Add( "color", "media/EffectColor.effect" );
			m_textured = game.GetManager< Effect >()->Add( "textured", "media/EffectTextured.effect" );
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
	dae::GeometrySourceFactory * daeFactory = new dae::GeometrySourceFactory( new MyEffectSolver( *game ) );
	game->GetManager< dxi::Geometry >()->AddFactory( "dae", daeFactory );

	return true;
}

