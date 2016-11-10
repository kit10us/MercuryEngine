// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/animation/Frame.h>
#include <me/ITexture.h>

using namespace dxi;
using namespace animation;
using namespace me;

Frame::Frame()
: m_duration( 0 )
{
}

Frame::Frame( const Frame & frame )
: m_texture( frame.m_texture )
, m_area( frame.m_area )
, m_duration( frame.m_duration )
{
}

Frame::Frame( ITexture::ptr texture, const unify::TexArea & area, unify::Seconds duration )
: m_texture( texture )
, m_area( area )
, m_duration( duration )
{
}

Frame::~Frame() throw ()
{
}

bool Frame::operator==( const Frame & frame ) const
{
	return (m_texture == frame.m_texture) && (m_area == frame.m_area) && (m_duration == frame.m_duration);
}

bool Frame::operator!=( const Frame & frame ) const
{
	return !(*this == frame);
}

unify::Seconds Frame::GetDuration() const
{
	return m_duration;
}

unify::Size< float > Frame::GetSizeInPixels() const
{
	unify::Size< float > sizeInPixels( m_area.Size() );
	sizeInPixels.width *= m_texture->ImageSize().width;
	sizeInPixels.height *= m_texture->ImageSize().height;
	return sizeInPixels;
}

const unify::TexArea & Frame::GetArea() const
{
	return m_area;
}

ITexture::ptr Frame::GetTexture() const
{
	return m_texture;
}
