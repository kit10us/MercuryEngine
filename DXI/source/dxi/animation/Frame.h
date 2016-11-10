// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once
#include <me/ITexture.h>
#include <unify/Size.h>
#include <unify/TexArea.h>
#include <unify/TimeDelta.h>
#include <memory>

namespace dxi
{
	namespace animation
	{
		class Frame
		{
		public:
			Frame();
			Frame( const Frame & frame );
			Frame( me::ITexture::ptr texture, const unify::TexArea & area, unify::Seconds duration = 0 );
			~Frame() throw ();
			bool operator==( const Frame & frame ) const;
			bool operator!=( const Frame & frame ) const;
			unify::Seconds GetDuration() const;
			const unify::TexArea & GetArea() const;
			unify::Size< float > GetSizeInPixels() const;
			me::ITexture::ptr GetTexture() const;
		private:
			me::ITexture::ptr m_texture;
			unify::TexArea m_area;
			unify::Seconds m_duration;
		};
	}
}