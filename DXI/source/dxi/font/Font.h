// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <map>
#include <dxi/font/CharacterKey.h>
#include <dxi/font/Character.h>
#include <dxi/animation/SpriteManager.h>

namespace dxi
{
	namespace font
	{
		typedef std::map< const CharacterKey, Character > CharacterMap;

		class Font
		{
		public:
			Font();
			Font( const unify::Path & filePath, animation::SpriteManager::shared_ptr spriteManager );
			~Font() throw ();

			void CreateFromFile( const unify::Path & filePath, animation::SpriteManager::shared_ptr spriteManager );
			Character GetCharacter( const CharacterKey & key ) const;
			unsigned int CharacterCount() const;

		private:
			unify::Path m_filePath;
			CharacterMap m_characterMap;
			Character m_invalidCharacter;
			std::shared_ptr< Effect > m_effect;
		};
	} // namespace font
} // namespace dxi
