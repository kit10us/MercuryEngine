// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <canvas/Element.h>
#include <me/render/Effect.h>
#include <me/game/IGame.h>

namespace canvas
{
	class Image : public Element
	{
	public:
		Image( me::game::IGame * gameInstance, me::render::Effect::ptr effect, Anchor anchor = Anchor::Center, unify::V2< float > scale = { 1, 1 }, unify::V2< float > offset = {0, 0} );

		unify::Size< float > GetContentSize() const override;
		
		void SetScale( unify::V2< float > scale );
			
		unify::V2< float > GetScale() const;

		void UpdateLayout( me::UpdateParams params, unify::Rect< float > parentArea ) override;

		void Update( const me::UpdateParams & params ) override;
		
		void Render( const me::render::Params & params ) override;
		
		void OnSuspend() override;
		
		void OnResume() override;

	private:
		me::render::Effect::ptr m_effect;
		me::render::IVertexBuffer::ptr m_vertexBuffer;
		bool m_changed;
		unify::V2< float > m_scale;
		bool m_shrinkToFit;
	};
}