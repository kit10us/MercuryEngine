// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/font/TextBox.h>
#include <dxi/RenderState.h>

using namespace dxi;
using namespace font;

TextBox::TextBox( core::Game * game )
	: m_game( game )
{
}

TextBox::TextBox( std::shared_ptr< Font > font, const unify::V2< float > & position, const FontString & fontString, float scale )
: m_font( font )
, m_position( position * scale )
, m_string( fontString )
, m_scale( scale )
, m_orientation( unify::NinePoint::Center )
{
	SyncCharacterRenderList();
}

TextBox::~TextBox()
{
}

std::shared_ptr< Font > TextBox::GetFont()
{
	return m_font;
}

const std::shared_ptr< Font > TextBox::GetFont() const
{
	return m_font;
}			  

void TextBox::SyncCharacterRenderList()
{
	m_characterRenderList.clear();
	if( m_string.Empty() )
	{
		return;
	}

	unify::V2< float > offset( m_position );
    m_totalSize.Zero();
	for( unsigned int i = 0; i < m_string.Size(); ++i )
	{
		CharacterKey key = m_string.GetKey( i );
		CharacterRender characterRender( m_font->GetCharacter( key ), offset, m_scale );
		offset.x += characterRender.GetSize().width;
        m_totalSize.width += characterRender.GetSize().width;
        m_totalSize.height = std::max<>( m_totalSize.height, characterRender.GetSize().height );
		m_characterRenderList.push_back( characterRender );
	}
}

void TextBox::Update( unify::Seconds elapsed )
{
	CharacterRenderList::iterator itrEnd = m_characterRenderList.end();
	for( CharacterRenderList::iterator itr = m_characterRenderList.begin(); itr != itrEnd; ++itr )
	{
		itr->Update( elapsed );
	}
}

void TextBox::Render( const RenderInfo & renderInfo, bool is3D )
{
	unify::Matrix oldProjection, oldView, oldWorld;
	// TODO: Transform::Get( Transform::Index::World, oldWorld );

	/*
	DX11 porting
	bool oldClipping = RenderState::Get( RenderState::Clipping ) == 0 ? false : true;
	*/

	unify::Matrix origin( unify::Matrix::MatrixIdentity() );

	// We are rendering to 2D, so create a new orthogonal matrix...
	if( renderInfo.IsOptionFalse( RenderOption::NoFrame ) )
	{
		// TODO: Transform::Get( Transform::Index::Projection, oldProjection );
		// TODO: Transform::Get( Transform::Index::View, oldView );

		// TODO: unify::Matrix projection( unify::Matrix::MatrixOrthoOffCenterLH( 0, static_cast< float >( m_game->GetOS().GetResolution().width ), static_cast< float >( m_game->GetOS().GetResolution().height ), 0, -1, 1000 ) );
		// TODO: Transform::Set( Transform::Index::Projection, projection );

		/*
		DX11 porting
		RenderState::Set( RenderState::Clipping, false );
		*/

		unify::Matrix view( unify::Matrix::MatrixIdentity() );
		// TODO: Transform::Set( Transform::Index::View, view );
	}
	else
	{
		origin = oldWorld;
		// DebugBreak();
		// TODO: Found this during update - it doesn't rotate a matrix, it MAKES a rotation matrix...: origin.RotationX( 3.14159265f ); // Flip origin
	}

	if( renderInfo.IsOptionFalse( RenderOption::NoEffectSet ) )
	{
		/*
		// TODO:
		win::DX::GetDxDevice()->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
		win::DX::GetDxDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
		*/
	}

	for( auto character : m_characterRenderList )
	{
		character.Render( renderInfo, origin, is3D );
	}

	// Restore matrices...
	// TODO: Transform::Set( Transform::Index::World, oldWorld );
	if( renderInfo.IsOptionFalse( RenderOption::NoFrame ) )
	{
		// DX11 porting: RenderState::Set( RenderState::Clipping, oldClipping );
		// TODO: Transform::Set( Transform::Index::Projection, oldProjection );
		// TODO: Transform::Set( Transform::Index::View, oldView );
	}
}