// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/IGame.h>
#include <me/canvas/Anchor.h>
#include <me/canvas/IElement.h>
#include <unify/Size.h>

namespace me
{
	namespace canvas
	{
		class Element : public IElement
		{
		public:
			Element( me::IGame * game, unify::V2< float > offset = { 0, 0 }, unify::Size< float > size = { 0, 0 }, Anchor anchor = Anchor::Center );

			bool IsEnabled() const override;
			void SetEnabled( bool enabled ) override;

			me::IGame * GetGame();

			void UpdateLayout( UpdateParams params, unify::Size< float > area ) override;

			void SetAnchor( Anchor anchor );
			Anchor GetAnchor() const;

			unify::V2< float > GetOffset() const override;
			unify::Size< float > GetSize( unify::Size< float > area ) const override;

			unify::V2< float > GetActualPosition() const;
			unify::Size< float > GetActualSize() const;

		private:
			IGame * m_game;
			bool m_enabled;
			Anchor m_anchor;
			unify::V2< float > m_offset;
			unify::Size< float > m_size;

			unify::V2< float > m_actualPosition;
			unify::Size< float > m_actualSize;
		};
	}
}