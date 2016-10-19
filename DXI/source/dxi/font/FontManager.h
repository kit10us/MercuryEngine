// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once
#include <map>
#include <memory>
#include <dxi/font/Font.h>

namespace dxi
{
	namespace font
	{
		typedef std::string FontName;
		typedef std::map< FontName, std::shared_ptr< Font > > FontMap;

		class FontManager
		{
		public:
			typedef std::shared_ptr< FontManager > shared_ptr;

			FontManager( animation::SpriteManager::ptr spriteManager );
			~FontManager() throw ();
			void LoadFromFile( const FontName & name, const unify::Path & filePath );
			void AddFont( const FontName & name, std::shared_ptr< Font > font );
			void RemoveFont( const FontName & name );
			bool FontExists( const FontName & name ) const;
			const std::shared_ptr< Font > & FindFont( const FontName & name ) const;
			const FontName & GetFontName( const unsigned int index ) const;
			unsigned int Count() const;
		private:
			animation::SpriteManager::ptr m_spriteManager;
			FontMap m_fonts;
		};
	} // namespace font
} // namespace dxi