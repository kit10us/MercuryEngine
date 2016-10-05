#include <gui/Thing.h>

void gui::PositionFrom( Thing & thing, const Thing & from, Origin::TYPE origin, V2 offset )
{
	Rect fromRect;
	from.GetRect( fromRect );

	Rect thingRect;
	thing.GetRect( thingRect );

	V2 position( 0, 0 );

	switch( origin )
	{
	case Origin::UL:
		position = fromRect.ul;
		break;

	case Origin::UR:
		position.x = fromRect.dr.x;
		position.y = fromRect.ul.y;
		break;

	case Origin::DL:
		position.x = fromRect.ul.x;
		position.y = fromRect.dr.y;
		break;

	case Origin::DR:
		position = fromRect.dr;
		break;

	case Origin::Bottom: // Centered at horizontally.
		position.x = fromRect.ul.x + fromRect.GetSize().width * 0.5f;
		position.x -= thingRect.GetSize().width * 0.5f;
		position.y = fromRect.dr.y;
		break;

	case Origin::Right: // Centered vertically.
		position.x = fromRect.dr.x;
		position.y = fromRect.ul.y + fromRect.GetSize().height * 0.5f;
		position.y -= thingRect.GetSize().height * 0.5f;
		break;
	}

	position += offset;
	thing.MoveTo( position );
}

void gui::Thing::GetRect( Rect & rect ) const
{
	rect.ul.x = m_position.x;
	rect.ul.y = m_position.y;
	rect.dr.x = m_position.x + m_size.width - 1.0f;
	rect.dr.y = m_position.y + m_size.height - 1.0f;
}

const V2 & gui::Thing::GetPosition() const
{
	return m_position;
}

const Size & gui::Thing::GetSize() const
{
	return m_size;
}

void gui::Thing::Set( const V2 & position, const Size & size )
{
	m_position = position;
	m_size = size;
}

void gui::Thing::MoveTo( const V2 & position )
{
	m_position = position;
}

void gui::Thing::MoveBy( const V2 & position )
{
	m_position += position;
}

void gui::Thing::AdjustSize( const Size & size )
{
	m_size += size;
}

void gui::Thing::SetUserData( float userData )
{
	m_userData = userData;
}

float gui::Thing::GetUserData() const
{
	return m_userData;
}