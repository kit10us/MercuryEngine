#include <gui/DropDown.h>
#include <assert.h>

gui::DropDown::DropDown()
: m_current( 0 )
, m_onChangeFunctor( 0 )
, m_hotKeyIsSet( false )
, m_hotKey( 0 )
{
}

gui::DropDown::~DropDown()
{
	Destroy();
}

void gui::DropDown::Update( float delta, Input & input )
{
    delta; // NOT USED

	// If we have the input locked, our drop down list should be on display.
	if( m_dropDownLock.Locked() )
	{
		// Hot key increments, or wraps...
		if ( ( m_hotKeyIsSet && input.WasKeyPressed( m_hotKey ) ) )
		{
			if ( m_choices.size() > 1 )
			{
				m_current++;
				if ( m_current >= m_choices.size() )
				{
					m_current = 0;
				}

				if ( m_onChangeFunctor )
				{
					(*m_onChangeFunctor)( *this );
				}

				input.Unlock( m_dropDownLock );
			}
		}
	}
	else if ( ! input.Locked() )
	{
		// Hot key increments, or wraps...
		if ( ( m_hotKeyIsSet && input.WasKeyPressed( m_hotKey ) ) )
		{
			if ( m_choices.size() > 1 )
			{
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

		// Via mouse button...
		V2 mousePosition = input.GetMousePosition();
		Rect rect;
		GetRect( rect );
		if ( input.WasLeftMousePressed() && rect.In( mousePosition )  )
		{
			input.Lock( m_dropDownLock );
		}
	}
}

void gui::DropDown::Draw()
{
}

void gui::DropDown::Create( V2 position, Size size, Font * font )
{
	Set( position, size );
	m_font = font;
}

void gui::DropDown::Destroy()
{
	delete m_onChangeFunctor;
	m_onChangeFunctor = 0;

	m_font = 0; // Not ours to manage.
}

size_t gui::DropDown::AddChoice( std::string text )
{
	m_choices.push_back( text );
	return m_choices.size() - 1;
}

const std::string & gui::DropDown::GetChoiceText( size_t index ) const
{
	assert( index < NumberOfChoices() );
	return m_choices[ index ];
}

size_t gui::DropDown::NumberOfChoices() const
{
	return m_choices.size();
}

size_t gui::DropDown::GetCurrentChoice() const
{
	return m_current;
}

void gui::DropDown::SetCurrentChoice( size_t choice )
{
	m_current = choice;
}

void gui::DropDown::SetOnChange( OnEventFunctor * onChangeFunctor )
{
	delete m_onChangeFunctor;
	m_onChangeFunctor = 0;
	m_onChangeFunctor = onChangeFunctor;
}

void gui::DropDown::SetHotKey( unsigned int hotKey )
{
	m_hotKeyIsSet = true;
	m_hotKey = hotKey;
}

void gui::DropDown::ClearHotKey()
{
	m_hotKeyIsSet = false;
}

