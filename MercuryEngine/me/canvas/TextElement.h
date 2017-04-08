// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/canvas/Element.h>
#include <me/render/Effect.h>
#include <me/IGame.h>

namespace me
{
	namespace canvas
	{
		class TextElement : public Element
		{
		public:
			TextElement( me::IGame * game, render::Effect::ptr effect, std::string text = std::string(), Anchor anchor = Anchor::Center, unify::V2< float > scale = { 1, 1 }, unify::V2< float > offset = {0, 0} );

			unify::Size< float > GetSize( unify::Size< float > area ) const override;
		
			void BuildText( unify::Size< float > area );

			void SetText( std::string text );

			void SetScale( unify::V2< float > scale );
			
			unify::V2< float > GetScale() const;

			void UpdateLayout( UpdateParams params, unify::Size< float > area ) override;

			void Update( UpdateParams params ) override;
		
			void Render( render::Params params ) override;
		
			void OnSuspend() override;
		
			void OnResume() override;

		private:
			std::string m_text;
			render::Effect::ptr m_effect;
			render::IVertexBuffer::ptr m_vertexBuffer;
			bool m_changed;
			unify::V2< float > m_scale;
			bool m_shrinkToFit;
		};
	}
}