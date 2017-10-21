// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/canvas/Element.h>
#include <me/render/Effect.h>
#include <me/IGame.h>

namespace me
{
	namespace canvas
	{
		class Image : public Element
		{
		public:
			Image( me::IGame * game, render::Effect::ptr effect, Anchor anchor = Anchor::Center, unify::V2< float > scale = { 1, 1 }, unify::V2< float > offset = {0, 0} );

			unify::Size< float > GetSize( unify::Size< float > area ) const override;
		
			void Build( unify::Size< float > area );

			void SetScale( unify::V2< float > scale );
			
			unify::V2< float > GetScale() const;

			void UpdateLayout( UpdateParams params, unify::Size< float > area ) override;

			void Update( UpdateParams params ) override;
		
			void Render( render::Params params ) override;
		
			void OnSuspend() override;
		
			void OnResume() override;

		private:
			render::Effect::ptr m_effect;
			render::IVertexBuffer::ptr m_vertexBuffer;
			bool m_changed;
			unify::V2< float > m_scale;
			bool m_shrinkToFit;
		};
	}
}