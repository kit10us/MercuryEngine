// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once
#include <dxi/win/DirectX.h>
#include <unify/Rect.h>
#include <unify/Flags.h>
#include <unify/Size.h>
#include <unify/Color.h>
#include <unify/TexArea.h>
#include <unify/Path.h>
#include <string>
#include <memory>
#include <map>

namespace dxi
{
	#define TEXTURE_LOCKABLE		FLAG01	// D3DPOOL_MANAGED
	#define TEXTURE_NORENDER		FLAG02	// D3DPOOL_SCRATCH

	// TODO: Re-arch...
	struct TextureMode
	{
		enum TYPE
		{
			Correct,	// Repeats the same texture on all sides, identically
			Wrapped		// Wraps the texture around the object so it uses only a portion on each side
		};

		static TYPE FromString( std::string mode )
		{
			if( unify::StringIs( mode, "CORRECT" ) )
			{
				return Correct;
			}
			else if( unify::StringIs( mode, "WRAPPED" ) )
			{
				return Wrapped;
			}
			assert( 0 );
			throw 0;
		}
	};
    
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

	#define LOCK_READONLY	D3DLOCK_READONLY

	// TODO: Move to its own file and remove hung
	struct TextureLock
	{
		TextureLock()
		{
			pBits = 0;
		}

		unsigned char* pBits;
		unsigned int uStride;		// pos_x * width_stride + pos_y * height_stride == color at pos_x, pos_y // AKA pitch
		unsigned int uWidth;
		unsigned int uHeight;
	};


	class Texture
	{
	public:
		typedef std::shared_ptr< Texture > shared_ptr;
		typedef std::weak_ptr< Texture > weak_ptr;

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

		Texture();
		Texture( const unify::Path & filePath, unsigned long flags = 0 );
		virtual ~Texture();

		// ::Resource...
		void Preload();
		void Create();
		void Destroy();

		void ClearHeader();

		void CreateFromFile( const unify::Path & filePath, unsigned long flags = FLAGNULL );

		const unsigned int FileWidth() const;
		const unsigned int FileHeight() const;


		const unify::Size< unsigned int > & ImageSize() const;

		bool Use( unsigned int stage );
		static void UnsetTexture( unsigned int name );

		void SetFlags( unsigned long flags );

		const unsigned long GetFlags()	const;

		void LockRect(unsigned int level, TextureLock & lock, const unify::Rect< long > * rect, unsigned long flags );
		void UnlockRect( unsigned int level );

		bool HasSpriteArray( const std::string & name ) const;
		const Texture::SpriteArray & FindSpriteArray( const std::string & name ) const;
		const Texture::SpriteArray & GetSpriteArray( unsigned int index ) const;
		const unify::TexArea & GetSprite( const std::string & arrayName, unsigned int index ) const;
		unsigned int SpriteArrayCount() const;
		const unify::TexArea & GetSprite( unsigned int index ) const;
		unsigned int SpriteCount() const;

	protected:
		unify::Path m_filePath;
		bool m_created;

		void LoadImage( const unify::Path & filePath );
		void LoadHeader();
	
		unify::Size< unsigned int > m_fileSize;
		unify::Size< unsigned int > m_imageSize;
		
		bool m_useColorKey;
		unify::Color m_colorKey;
		unsigned long m_flags;
		
		SpriteArrayMap m_spriteArrayMap;
		SpriteMasterList m_spriteMasterList;

		// DirectX Specific Data
		IDirect3DTexture9 * m_texture;
		D3DPOOL	m_pool;
		D3DFORMAT m_textureFormat;
	};
} // namespace dxi