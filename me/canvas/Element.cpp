// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/canvas/Element.h>

using namespace me;
using namespace canvas;

Element::Element( game::IGame * gameInstance, unify::V2< float > offset, unify::Size< float > size, Anchor anchor )
	: m_game{ gameInstance }
	, m_enabled{ true }
	, m_anchor{ anchor }
	, m_offset{ offset }
	, m_size{ size }
{
}

void Element::SetName( std::string name )
{
	m_name = name;
}

std::string Element::GetName() const
{
	return m_name;
}

bool Element::IsEnabled() const
{
	return m_enabled;
}

void Element::SetEnabled( bool enabled )
{
	m_enabled = enabled;
}

game::IGame * Element::GetGame()
{
	return m_game;
}


void Element::UpdateLayout( UpdateParams params, unify::Rect< float > parentArea )
{
	switch( m_anchor )
	{
	case Anchor::Free:
		m_actualPosition = parentArea.UL() + m_offset;
		m_actualSize = m_size;
		break;

	case Anchor::TopLeft:
		m_actualPosition = parentArea.UL() + m_offset + unify::V2< float >{ 0, 0 };
		m_actualSize = m_size;
		break;
	case Anchor::Top:
		m_actualPosition = parentArea.UL() + unify::V2< float >{ parentArea.Width() * 0.5f, 0 } + m_offset;
		m_actualSize = m_size;
		break;
	case Anchor::TopRight:
		m_actualPosition = parentArea.UL() + unify::V2< float >{ parentArea.Width(), 0 } + m_offset;
		m_actualSize = m_size;
		break;
	case Anchor::StretchTop:
		m_actualPosition = parentArea.UL() + unify::V2< float >{ 0, m_offset.y };
		m_actualSize = unify::Size< float >{ parentArea.Width(), m_size.height };
		break;

	case Anchor::Left:
		m_actualPosition = parentArea.UL() + unify::V2< float >{ 0, parentArea.Height() * 0.5f } + m_offset;
		m_actualSize = m_size;
		break;
	case Anchor::Center:
		m_actualPosition = parentArea.UL() + unify::V2< float >{ parentArea.Width() * 0.5f, parentArea.Height() * 0.5f } + m_offset;
		m_actualSize = m_size;
		break;
	case Anchor::Right:
		m_actualPosition = parentArea.UL() + unify::V2< float >{ parentArea.Width(), parentArea.Height() * 0.5f } + m_offset;
		m_actualSize = m_size;
		break;
	case Anchor::StretchLeftRight:
		m_actualPosition = parentArea.UL() + unify::V2< float >{ 0, parentArea.Height() * 0.5f + m_offset.y };
		m_actualSize = unify::Size< float >{ parentArea.Width(), m_size.height };
		break;

	case Anchor::BottomLeft:
		m_actualPosition = parentArea.UL() + unify::V2< float >{ 0, parentArea.Height() } + m_offset;
		m_actualSize = m_size;
		break;
	case Anchor::Bottom:
		m_actualPosition = parentArea.UL() + unify::V2< float >{ parentArea.Width() * 0.5f, parentArea.Height() } + m_offset;
		m_actualSize = m_size;
		break;
	case Anchor::BottomRight:
		m_actualPosition = parentArea.UL() + unify::V2< float >{ parentArea.Width(), parentArea.Height() } + m_offset;
		m_actualSize = m_size;
		break;
	case Anchor::StretchBottom:
		m_actualPosition = parentArea.UL() + unify::V2< float >{ 0, parentArea.Height() + m_offset.y };
		m_actualSize = unify::Size< float >{ parentArea.Width(), m_size.height };
		break;

	case Anchor::StretchLeft:
		m_actualPosition = parentArea.UL() + unify::V2< float >{ m_offset.x, 0 };
		m_actualSize = unify::Size< float >{ m_size.width, parentArea.Height() };
		break;
	case Anchor::StretchTopBottom:
		m_actualPosition = parentArea.UL() + unify::V2< float >{ parentArea.Width() * 0.5f + m_offset.x, 0 };
		m_actualSize = unify::Size< float >{ m_size.width, parentArea.Height() };
		break;
	case Anchor::StretchRight:
		m_actualPosition = parentArea.UL() + unify::V2< float >{ parentArea.Width() + m_offset.x, 0 };
		m_actualSize = unify::Size< float >{ m_size.width, parentArea.Height() };
		break;

	case Anchor::StretchFull:
		m_actualPosition = parentArea.UL() + m_offset;
		m_actualSize = parentArea.Size() - unify::Size< float >( m_offset.x, m_offset.y );
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

void Element::SetOffset( unify::V2< float > offset )
{
	m_offset = offset;
}

unify::V2< float > Element::GetOffset() const
{
	return m_offset;
}

unify::Size< float > Element::GetContentSize() const
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
