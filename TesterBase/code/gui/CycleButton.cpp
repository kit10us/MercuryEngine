#include <gui/CycleButton.h>
#include <Draw2D.h>
#include <assert.h>

gui::CycleButton::CycleButton()
: m_current( 0 )
, m_onChangeFunctor( 0 )
, m_hotKeyIsSet( false )
, m_hotKey( 0 )
, m_cycle( 0 )
{
}

gui::CycleButton::~CycleButton()
{
	Destroy();
}

void gui::CycleButton::Update( float delta, Input & input )
{
	V2 mousePosition = input.GetMousePosition();
	Rect rect;
	GetRect( rect );
	if ( ( m_hotKeyIsSet && input.WasKeyPressed( m_hotKey ) )  || ( input.WasLeftMousePressed() && rect.In( mousePosition ) ) )
	{
		if ( m_choices.size() > 1 )
		{
			m_cycle = 1.0f;
			m_current++;
			if ( m_current >= m_choices.size() )
			{
				m_current = 0;
			}

			if ( m_onChangeFunctor )
			{
				(*m_onChangeFunctor)( *this );
			}
		}
	}
	else
	{
		if ( m_cycle > 0.0f )
		{
			m_cycle -= delta * 0.25f;
			if ( m_cycle < 0.0f )
			{
				m_cycle = 0.0f;
			}
		}
	}
}

void gui::CycleButton::Draw()
{
	unsigned char alpha = 255;
	D3DCOLOR bodyColorUL( D3DCOLOR_ARGB( alpha, 130, 130, 230 ) );
	D3DCOLOR bodyColorUR( D3DCOLOR_ARGB( alpha, 50, 50, 200 ) );
	D3DCOLOR bodyColorDL( D3DCOLOR_ARGB( alpha, 50, 50, 200 ) );
	D3DCOLOR bodyColorDR( D3DCOLOR_ARGB( alpha, 50, 50, 120 ) );

	D3DCOLOR borderColorUL( D3DCOLOR_ARGB( alpha, 20, 20, 60 ) );
	D3DCOLOR borderColorUR( D3DCOLOR_ARGB( alpha, 50, 50, 200 ) );
	D3DCOLOR borderColorDL( D3DCOLOR_ARGB( alpha, 50, 50, 200 ) );
	D3DCOLOR borderColorDR( D3DCOLOR_ARGB( alpha, 100, 100, 250 ) );

	Rect rect;
	GetRect( rect );

	// Body
	// TODO: Draw2D::DrawRectangle( rect, bodyColorUL, bodyColorUR, bodyColorDR, bodyColorDL, 0 );

	// Border
	// TODO: Draw2D::DrawRectangle( rect, borderColorUL, borderColorUR, borderColorDR, borderColorDL, bodyColorUL, bodyColorUR, bodyColorDR, bodyColorDL, 4 );

	// Text::Draw();

	if ( m_font && m_current <= m_choices.size() )
	{
		m_font->Draw( rect, m_choices[ m_current ] );
	}
}

void gui::CycleButton::Create( V2 position, Size size, Font * font )
{
	Set( position, size );
	m_font = font;
}

void gui::CycleButton::Destroy()
{
	delete m_onChangeFunctor;
	m_onChangeFunctor = 0;

	m_font = 0; // Not ours to manage.
}

size_t gui::CycleButton::AddChoice( std::string text )
{
	m_choices.push_back( text );
	return m_choices.size() - 1;
}

const std::string & gui::CycleButton::GetChoiceText( size_t index ) const
{
	assert( index < NumberOfChoices() );
	return m_choices[ index ];
}

size_t gui::CycleButton::NumberOfChoices() const
{
	return m_choices.size();
}

size_t gui::CycleButton::GetCurrentChoice() const
{
	return m_current;
}

std::string gui::CycleButton::GetCurrentText() const
{
	return m_choices[ m_current ];
}

void gui::CycleButton::SetCurrentChoice( size_t choice )
{
	m_current = choice;
}

void gui::CycleButton::SetOnChange( OnEventFunctor * onChangeFunctor )
{
	delete m_onChangeFunctor;
	m_onChangeFunctor = 0;
	m_onChangeFunctor = onChangeFunctor;
}

void gui::CycleButton::SetHotKey( unsigned int hotKey )
{
	m_hotKeyIsSet = true;
	m_hotKey = hotKey;
}

void gui::CycleButton::ClearHotKey()
{
	m_hotKeyIsSet = false;
}

