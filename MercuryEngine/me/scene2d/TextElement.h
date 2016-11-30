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
			TextElement( me::IGame * game, Effect::ptr effect, std::string text = std::string(), Anchor anchor = Anchor::Center, unify::V2< float > scale = { 1, 1 }, unify::V2< float > offset = {0, 0} );

			unify::Size< float > GetSize( unify::Size< float > area ) const override;
		
			void BuildText( unify::Size< float > area );

			void SetText( std::string text );

			void SetScale( unify::V2< float > scale );
			
			unify::V2< float > GetScale() const;

			void UpdateLayout( IRenderer * renderer, const RenderInfo & renderInfo, unify::Size< float > area ) override;

			void Update( IRenderer * renderer, const RenderInfo & renderInfo ) override;
		
			void Render( IRenderer * renderer, const RenderInfo & renderInfo ) override;
		
			void OnSuspend() override;
		
			void OnResume() override;

		private:
			std::string m_text;
			Effect::ptr m_effect;
			IVertexBuffer::ptr m_vertexBuffer;
			bool m_changed;
			unify::V2< float > m_scale;
			bool m_shrinkToFit;
		};
	}
}