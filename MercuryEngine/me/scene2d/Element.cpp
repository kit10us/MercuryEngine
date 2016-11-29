// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <me/scene2d/Element.h>

using namespace me;
using namespace scene2d;

Element::Element( me::IGame * game, unify::V2< float > offset, unify::Size< float > size, Anchor anchor )
	: m_game{ game }
	, m_enabled{ true }
	, m_anchor{ anchor }
	, m_offset{ offset }
	, m_size{ size }
{
}

bool Element::IsEnabled() const
{
	return m_enabled;
}

void Element::SetEnabled( bool enabled )
{
	m_enabled = enabled;
}

me::IGame * Element::GetGame()
{
	return m_game;
}


void Element::UpdateLayout( IRenderer * renderer, const RenderInfo & renderInfo, unify::Size< float > area )
{
	switch( m_anchor )
	{
	case Anchor::Free:
		m_actualPosition = m_offset;
		m_actualSize = m_size;
		break;

	case Anchor::TopLeft:
		m_actualPosition = unify::V2< float >{ 0, 0 } + m_offset;
		m_actualSize = m_size;
		break;
	case Anchor::Top:
		m_actualPosition = unify::V2< float >{ area.width * 0.5f, 0 } + m_offset;
		m_actualSize = m_size;
		break;
	case Anchor::TopRight:
		m_actualPosition = unify::V2< float >{ area.width, 0 } + m_offset;
		m_actualSize = m_size;
		break;
	case Anchor::StretchTop:
		m_actualPosition = unify::V2< float >{ 0, m_offset.y };
		m_actualSize = unify::Size< float >{ area.width, m_size.height };
		break;

	case Anchor::Left:
		m_actualPosition = unify::V2< float >{ 0, area.height * 0.5f } + m_offset;
		m_actualSize = m_size;
		break;
	case Anchor::Center:
		m_actualPosition = area.Center() + m_offset;
		m_actualSize = m_size;
		break;
	case Anchor::Right:
		m_actualPosition = unify::V2< float >{ area.width, area.height * 0.5f } + m_offset;
		m_actualSize = m_size;
		break;
	case Anchor::StretchLeftRight:
		m_actualPosition = unify::V2< float >{ 0, area.height * 0.5f + m_offset.y };
		m_actualSize = unify::Size< float >{ area.width, m_size.height };
		break;

	case Anchor::BottomLeft:
		m_actualPosition = unify::V2< float >{ 0, area.height } + m_offset;
		m_actualSize = m_size;
		break;
	case Anchor::Bottom:
		m_actualPosition = unify::V2< float >{ area.width * 0.5f, area.height } + m_offset;
		m_actualSize = m_size;
		break;
	case Anchor::BottomRight:
		m_actualPosition = unify::V2< float >{ area.width, area.height } + m_offset;
		m_actualSize = m_size;
		break;
	case Anchor::StretchBottom:
		m_actualPosition = unify::V2< float >{ 0, area.height + m_offset.y };
		m_actualSize = unify::Size< float >{ area.width, m_size.height };
		break;

	case Anchor::StretchLeft:
		m_actualPosition = unify::V2< float >{ m_offset.x, 0 };
		m_actualSize = unify::Size< float >{ m_size.width, area.height };
		break;
	case Anchor::StretchTopBottom:
		m_actualPosition = unify::V2< float >{ area.width * 0.5f + m_offset.x, 0 };
		m_actualSize = unify::Size< float >{ m_size.width, area.height };
		break;
	case Anchor::StretchRight:
		m_actualPosition = unify::V2< float >{ area.width + m_offset.x, 0 };
		m_actualSize = unify::Size< float >{ m_size.width, area.height };
		break;

	case Anchor::StretchFull:
		m_actualPosition = area.Center();
		m_actualSize = area;
		break;
	}
}

void Element::SetAnchor( Anchor anchor )
{
	m_anchor = anchor;
}

Anchor Element::GetAnchor() const
{
	return m_anchor;
}

unify::V2< float > Element::GetOffset() const
{
	return m_offset;
}

unify::Size< float > Element::GetSize() const
{
	return m_size;
}

unify::V2< float > Element::GetActualPosition() const
{
	return m_actualPosition;
}

unify::Size< float > Element::GetActualSize() const
{
	return m_actualSize;
}
