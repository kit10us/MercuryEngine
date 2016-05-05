// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/font/CharacterKey.h>
#include <string>

namespace dxi
{
	namespace font
	{
		class FontString
		{
		public:
			FontString();
			FontString( const char * string );
			FontString( const std::string & string );
			~FontString() throw ();

			unsigned int Size() const;
			bool Empty() const;
			CharacterKey GetKey( unsigned int index ) const;
		private:
			std::string m_string;
		};
	} // namespace font
} // namespace dxi