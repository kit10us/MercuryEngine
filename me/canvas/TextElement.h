// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/canvas/Element.h>
#include <me/render/Effect.h>
#include <me/game/IGame.h>

namespace me
{
	namespace canvas
	{
		class TextElement : public Element
		{
		public:
			TextElement( me::game::IGame * gameInstance, render::Effect::ptr effect, std::string text = std::string(), Anchor anchor = Anchor::Center, unify::V2< float > scale = { 1, 1 }, unify::V2< float > offset = {0, 0} );

			/// <summary>
			/// Returns the size of the text. Include scaling.
			/// </summary>
			unify::Size< float > GetContentSize() const override;
		
			void SetText( std::string text );

			void SetScale( unify::V2< float > scale );
			
			unify::V2< float > GetScale() const;

			void UpdateLayout( UpdateParams params, unify::Rect< float > parentArea ) override;

			void Update( const UpdateParams & params ) override;
		
			void Render( const render::Params & params ) override;
		
			void OnSuspend() override;
		
			void OnResume() override;

		private:
			std::string m_text;
			render::Effect::ptr m_effect;
			render::IConstantBuffer::ptr m_vertexCB;
			render::IConstantBuffer::ptr m_pixelCB;
			render::IVertexBuffer::ptr m_vertexBuffer;
			bool m_changed;
			unify::V2< float > m_scale;
			bool m_shrinkToFit;
		};
	}
}