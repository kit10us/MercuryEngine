#include <gui/Picture.h>

gui::Picture::Picture()
: m_texture( 0 )
{
}

gui::Picture::~Picture()
{
	Destroy();
}

void gui::Picture::Update( float delta, Input & input )
{
	delta; input; // Not used
}

void gui::Picture::Draw()
{
	if( m_texture )
	{
		Rect rect;
		GetRect( rect );
		m_texture->Draw( rect );
	}
}

void gui::Picture::Create( V2 position, Texture * texture )
{
	Set( position, texture->GetSize() );
	m_texture = texture;
}

void gui::Picture::Destroy()
{
	m_texture = 0; // Not ours to manage.
}

