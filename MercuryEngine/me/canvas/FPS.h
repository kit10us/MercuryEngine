// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/canvas/TextElement.h>

namespace me
{
	namespace canvas
	{
		class FPS : public TextElement
		{
		public:
			FPS( me::IGame * game, render::Effect::ptr effect, Anchor anchor = Anchor::TopRight, unify::V2< float > scale = { 1, 1 }, unify::V2< float > offset = {0, 0} );
		
			void Update( UpdateParams params ) override;
		private:
			float m_updateRate;
			float m_secondsTillUpdate;
		};
	}
}