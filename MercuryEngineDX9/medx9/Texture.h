// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once
#include <medx9/Renderer.h>
#include <me/TextureLock.h>
#include <unify/Rect.h>
#include <unify/Color.h>
#include <unify/TexArea.h>
#include <unify/Path.h>
#include <string>
#include <memory>
#include <atlbase.h>

// MS agressive macros.
#ifdef LoadImage
#undef LoadImage
#endif

#ifdef min
#undef min
#endif

namespace medx9
{

	//////////////////////////////////
	// Font definitions...

#define FONT_BOLD        0x0001
#define FONT_ITALIC      0x0002
#define FONT_ZENABLE     0x0004

	// Font rendering flags
#define FONT_CENTERED_X  0x0001
#define FONT_CENTERED_Y  0x0002
#define FONT_TWOSIDED    0x0004
#define FONT_FILTERED    0x0008

#define SPRITEANIMLOOP_NONE			0
#define SPRITEANIMLOOP_REPEAT		1	// 1 loop period
#define SPRITEANIMLOOP_FORWARDBACK	2	// 2 loop periods

	class Texture : public me::ITexture
	{
	public:
		typedef std::shared_ptr< Texture > ptr;

		struct SpriteArray
		{
			SpriteArray() {}
			SpriteArray( unsigned int head, unsigned int tail ) : head( head ), tail( tail ) {}
			unsigned int head;
			unsigned int tail;
		};

		typedef std::map< const std::string, SpriteArray > SpriteArrayMap;
		typedef std::vector< unify::TexArea > SpriteMasterList;

		static bool s_allowTextureUses;

		Texture( me::IRenderer * renderer, me::TextureParameters parameters = me::TextureParameters() );
		virtual ~Texture();

		// ::Resource...
		void Preload();
		void Create();
		void Destroy();

		const unsigned int FileWidth() const;
	
		const unsigned int FileHeight() const;

		const unify::Size< unsigned int > & ImageSize() const;

		bool Use( unsigned int stage );

		bool GetRenderable() const;

		bool GetLockable() const;

		void LockRect( unsigned int level, me::TextureLock & lock, const unify::Rect< long > * rect, bool readonly );
		void UnlockRect( unsigned int level );

		me::SpriteDictionary & GetSpriteDictionary() override;
		const me::SpriteDictionary & GetSpriteDictionary() const override;

		const me::TextureParameters * GetParameters() const override;

	protected:
		void LoadImage( const unify::Path & filePath );
		void LoadHeader();

		const Renderer * m_renderer;
		CComPtr< IDirect3DTexture9 > m_texture;
		me::TextureParameters m_parameters;
		bool m_created;
		unify::Size< unsigned int > m_fileSize;
		unify::Size< unsigned int > m_imageSize;
		bool m_useColorKey;
		unify::Color m_colorKey;
		me::SpriteDictionary m_spriteDictionary;
	};
}