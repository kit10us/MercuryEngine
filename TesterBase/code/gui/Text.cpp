#include <gui/Text.h>

gui::Text::Text()
: m_color( D3DCOLOR_XRGB( 255, 255, 255 ) )
, m_alignment( Origin::Center )
{
}

gui::Text::~Text()
{
	Destroy();
}

void gui::Text::Update( float delta, Input & input )
{
	delta; input; // Not used
}

void gui::Text::Draw()
{
	Rect rect;
	GetRect( rect );

	if ( m_font )
	{
		m_font->Draw( rect, m_text, m_color, m_alignment );
	}
}

void gui::Text::Create( V2 position, Size size, std::string text, Font * font )
{
	Set( position, size );
	m_text = text;
	m_font = font;
}

void gui::Text::Create( V2 position, std::string text, Font * font )
{
	Create( position, font->GetTextSize( text ), text, font );
}

void gui::Text::Destroy()
{
	m_font = 0; // Not ours to manage.
}

void gui::Text::SetText( std::string text )
{
	m_text = text;
}

const std::string & gui::Text::GetText() const
{
	return m_text;
}

void gui::Text::SetColor( D3DCOLOR color )
{
	m_color = color;
}

D3DCOLOR gui::Text::GetColor() const
{
	return m_color;
}

void gui::Text::SetAlignment( Origin::TYPE alignment )
{
	m_alignment = alignment;
}

Origin::TYPE gui::Text::GetAlignment() const
{
	return m_alignment;
}

