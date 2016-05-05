// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once
#include <unify/Size.h>
#include <unify/TexArea.h>
#include <memory>
#include <unify/TimeDelta.h>

namespace dxi
{
	class Texture;

	namespace animation
	{
		class Frame
		{
		public:
			Frame();
			Frame( const Frame & frame );
			Frame( std::shared_ptr< Texture > texture, const unify::TexArea & area, unify::Seconds duration = 0 );
			~Frame() throw ();
			bool operator==( const Frame & frame ) const;
			bool operator!=( const Frame & frame ) const;
			unify::Seconds GetDuration() const;
			const unify::TexArea & GetArea() const;
			unify::Size< float > GetSizeInPixels() const;
			std::shared_ptr< Texture > GetTexture() const;
		private:
			std::shared_ptr< Texture > m_texture;
			unify::TexArea m_area;
			unify::Seconds m_duration;
		};
	} // namespace animation
} // namespace dxi