#include <gui/Button.h>
#include <Draw2D.h>

gui::Button::Button()
: m_onPressFunctor( 0 )
, m_hotKeyIsSet( false )
, m_hotKey( 0 )
{
}

gui::Button::~Button()
{
	Destroy();
}

void gui::Button::Update( float delta, Input & input )
{
	if ( ! input.Locked() )
	{
		// Via hot key...
		if ( ( m_hotKeyIsSet && input.WasKeyPressed( m_hotKey ) ) )
		{
			if ( m_onPressFunctor )
			{
				(*m_onPressFunctor)( *this );
			}
		}

		// Via mouse button...
		V2 mousePosition = input.GetMousePosition();
		Rect rect;
		GetRect( rect );
		if ( input.WasLeftMousePressed() && rect.In( mousePosition ) )
		{
			if ( m_onPressFunctor )
			{
				(*m_onPressFunctor)( *this );
			}
		}
	}

	Text::Update( delta, input );
}

void gui::Button::Draw()
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

	Text::Draw();
}

void gui::Button::Create( V2 position, Size size, std::string text, Font * font )
{
	Text::Create( position, size, text, font );
	Set( position, size );
}

void gui::Button::SetOnPress( OnEventFunctor * onPressFunctor )
{
	delete m_onPressFunctor;
	m_onPressFunctor = 0;
	m_onPressFunctor = onPressFunctor;
}

void gui::Button::SetHotKey( unsigned int hotKey )
{
	m_hotKeyIsSet = true;
	m_hotKey = hotKey;
}

void gui::Button::ClearHotKey()
{
	m_hotKeyIsSet = false;
}

void gui::Button::Destroy()
{
	delete m_onPressFunctor;
	m_onPressFunctor = 0;
	
	Text::Destroy();
}

