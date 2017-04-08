// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/TexArea.h>
#include <unify/Path.h>
#include <map>
#include <vector>

namespace me
{
	namespace render
	{
		class SpriteDictionary
		{
		public:
			struct SpriteArray
			{
				SpriteArray() {}
				SpriteArray( unsigned int head, unsigned int tail ) : head( head ), tail( tail ) {}
				unsigned int head;
				unsigned int tail;
			};

			typedef std::map< const std::string, SpriteArray > SpriteArrayMap;
			typedef std::vector< unify::TexArea > SpriteMasterList;

			SpriteDictionary();
			~SpriteDictionary();

			void LoadDictionary( unify::Path source );

			bool HasSpriteArray( const std::string & name ) const;
			const SpriteArray & FindSpriteArray( const std::string & name ) const;
			const SpriteArray & GetSpriteArray( unsigned int index ) const;
			const unify::TexArea & GetSprite( const std::string & arrayName, unsigned int index ) const;
			unsigned int SpriteArrayCount() const;
			const unify::TexArea & GetSprite( unsigned int index ) const;
			unsigned int SpriteCount() const;

			/// <summary>
			/// Test if we support a ascii numberic characters.
			/// </summary>
			bool HasAsciiNumeric() const;

			/// <summary>
			/// Test if we support a ascii lowercase characters.
			/// </summary>
			bool HasAsciiLowercase() const;

			/// <summary>
			/// Test if we support a ascii uppercase characters.
			/// </summary>
			bool HasAsciiUppercase() const;

			/// <summary>
			/// Test if we support a specific ascii character.
			/// </summary>
			bool HasAscii( char ascii ) const;

			/// <summary>
			/// Get the texture mapping for a specific ascii character. Retruns the null ascii if not found.
			/// Can test for character existance via HasAsciiCharacter.
			/// </summary>
			unify::TexArea GetAscii( char ascii ) const;

		private:
			SpriteArrayMap m_spriteArrayMap;
			SpriteMasterList m_spriteMasterList;
			std::map< char, unify::TexArea > m_ascii;
			unify::TexArea m_asciiNull;
			bool m_asciiNumeric;
			bool m_asciiLowercase;
			bool m_asciiUppercase;
		};
	}
}