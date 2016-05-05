// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <string>

namespace dxi
{
	namespace font
	{
		class CharacterKey
		{
		public:
			CharacterKey();
			CharacterKey( char keyFromChar );
			CharacterKey( const std::string & keyFromString );
			~CharacterKey() throw ();

			bool operator==( const CharacterKey & key ) const;
			bool operator<( const CharacterKey & key ) const;
			bool empty() const;
		protected:
			std::string m_stringKey;
		};
	} // namespace font
} // namespace dxi
