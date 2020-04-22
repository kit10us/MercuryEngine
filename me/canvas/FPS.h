// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/canvas/TextElement.h>
#include <unify/TimeDelta.h>

namespace me::canvas
{
	/// <summary>
	/// A text element that displays the frames per second.
	/// </summary>
	class FPS : public TextElement
	{
		unify::TimeDelta m_updateRate;
		unify::TimeDelta m_timeTillUpdate;

	public:
		FPS( me::game::IGame * gameInstance, render::Effect::ptr effect, Anchor anchor = Anchor::TopRight, unify::V2< float > scale = { 1, 1 }, unify::V2< float > offset = {0, 0} );
		
		void Update( const UpdateParams & params ) override;
	};
}