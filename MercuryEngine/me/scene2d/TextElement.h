// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/scene2d/Element.h>
#include <me/Effect.h>
#include <me/IGame.h>

namespace me
{
	namespace scene2d
	{
		class TextElement : public Element
		{
		public:
			TextElement( me::IGame * game, Effect::ptr effect, std::string text = std::string() );
		
			void BuildText();

			void SetText( std::string text );

			void Update( const RenderInfo & renderInfo ) override;
		
			void Render( const RenderInfo & renderInfo ) override;
		
			void OnSuspend() override;
		
			void OnResume() override;

		private:
			IGame * m_game;
			std::string m_text;
			Effect::ptr m_effect;
			IVertexBuffer::ptr m_vertexBuffer;
			bool m_changed;

		};
	}
}