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

			void SetName( std::string name ) override;
			std::string GetName() const override;

			bool IsEnabled() const override;
			void SetEnabled( bool enabled ) override;

			me::IGame * GetGame();

			void UpdateLayout( UpdateParams params, unify::Rect< float > parentArea ) override;

			void SetAnchor( Anchor anchor );
			Anchor GetAnchor() const;

			void SetOffset( unify::V2< float > offset ) override;
			unify::V2< float > GetOffset() const override;

			unify::Size< float > GetContentSize() const override;

			unify::V2< float > GetActualPosition() const;
			unify::Size< float > GetActualSize() const;

		private:
			IGame * m_game;
			std::string m_name;
			bool m_enabled;
			Anchor m_anchor;
			unify::V2< float > m_offset;
			unify::Size< float > m_size;

			unify::V2< float > m_actualPosition;
			unify::Size< float > m_actualSize;
		};
	}
}