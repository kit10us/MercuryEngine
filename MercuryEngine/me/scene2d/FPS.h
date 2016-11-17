// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/scene2d/TextElement.h>

namespace me
{
	namespace scene2d
	{
		class FPS : public TextElement
		{
		public:
			FPS( me::IGame * game, Effect::ptr effect );
		
			void Update( const IRenderer * renderer, const RenderInfo & renderInfo ) override;
		};
	}
}