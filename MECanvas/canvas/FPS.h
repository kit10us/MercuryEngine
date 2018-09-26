// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <canvas/TextElement.h>

namespace canvas
{
	class FPS : public TextElement
	{
	public:
		FPS( me::game::IGame * gameInstance, me::render::Effect::ptr effect, Anchor anchor = Anchor::TopRight, unify::V2< float > scale = { 1, 1 }, unify::V2< float > offset = {0, 0} );
		
		void Update( const me::UpdateParams & params ) override;
	private:
		unify::TimeDelta m_updateRate;
		unify::TimeDelta m_timeTillUpdate;
	};
}
