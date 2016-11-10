// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
//

#include <medx9/Texture.h>
#include <medx9/Renderer.h>
#include <qxml/Document.h>
#include <me/exception/NotImplemented.h>
#include <me/exception/FailedToLock.h>
#include <atlbase.h>

// MS agressive macros.
#ifdef LoadImage
#undef LoadImage
#endif

using namespace medx9;
using namespace me;

class Texture::Pimpl
{
	Texture & m_owner;
	Renderer * m_renderer;

	CComPtr< IDirect3DTexture9 > m_texture;

	TextureParameters m_parameters;

	bool m_created;
	unify::Size< unsigned int > m_fileSize;
	unify::Size< unsigned int > m_imageSize;
	bool m_useColorKey;
	unify::Color m_colorKey;
	SpriteArrayMap m_spriteArrayMap;
	SpriteMasterList m_spriteMasterList;

public:
	Pimpl( Texture & owner, me::IRenderer * renderer, TextureParameters parameters )
		: m_owner( owner )
		, m_renderer( dynamic_cast< Renderer * >( renderer ) )
		, m_useColorKey( false )
		, m_created( false )
		, m_parameters( parameters )
	{
	}

	Pimpl( Texture & owner, me::IRenderer * renderer, const unify::Path & filePath, TextureParameters parameters )
		: Pimpl( owner, renderer, parameters )
	{
		Create();
	}

	~Pimpl()
	{
	}

	void Create()
	{
		// Load from a file.
		if ( m_parameters.source.Empty() )
		{
			throw unify::Exception( "Not enough information to load Texture!" );
		}

		LoadHeader();

		LoadImage( m_parameters.source );

		m_created = true;
	}	 


	// Destroyes data, keeps header intact - thus no graphical footprint, yet we can still use it's statistics/dimensions in calculations.
	void Destroy()
	{
		m_texture = nullptr;
		m_created = false; // TODO: Can solve this from m_texture.
	}

	void LockRect( unsigned int level, TextureLock & lock, const unify::Rect< long > * rect, bool readonly )
	{
		if ( !m_created )
		{
			Create();
		}

		if( m_parameters.lockable == false )
		{
			throw exception::FailedToLock( "Texture is not lockable!" );
		}
		
		// Allow us to convert from our flag type to DX.
		unsigned int dxFlags = readonly ? D3DLOCK_READONLY : 0;

		D3DLOCKED_RECT d3dLockedRect;
		if( FAILED( m_texture->LockRect( level, &d3dLockedRect, (RECT*)rect, dxFlags ) ) )
		{
			lock.pBits = 0;
			lock.uStride = 0;
			throw unify::Exception( "Failed to lock texture: \"" + m_parameters.source.ToString() + "\"" );
		}

		lock.pBits = (unsigned char*)d3dLockedRect.pBits;
		lock.uStride = d3dLockedRect.Pitch;
		lock.bpp = 4;
		if( rect )
		{
			lock.uWidth = rect->right - rect->left;
			lock.uHeight = rect->bottom - rect->top;
		}
		else
		{
			lock.uWidth = m_imageSize.width;
			lock.uHeight = m_imageSize.height;
		}
	}

	void UnlockRect( unsigned int level )
	{
		m_texture->UnlockRect( level );
	}

	void LoadImage( unify::Path filePath )
	{
		// Release any previous texture
		Destroy();

		// Verify file exists
		if ( !filePath.Exists() )
		{
			throw unify::Exception( "Failed to load image, file not found! (" + filePath.ToString() + ")" );
		}

		// Set defaults that can be overrided by texture...
		D3DFORMAT textureFormat = D3DFMT_A8R8G8B8;

		// First, load the file to find out it's sizes...	
		D3DXIMAGE_INFO fileDesc;
		unsigned int dwFilter = D3DX_FILTER_NONE;


		D3DPOOL pool = D3DPOOL_DEFAULT;
		if( m_parameters.lockable )
		{
			pool = m_parameters.renderable ? D3DPOOL_MANAGED : D3DPOOL_SCRATCH;
		}

		HRESULT result;
		result = D3DXCreateTextureFromFileExA(
			m_renderer->GetDxDevice(),
			(LPCSTR)filePath.ToString().c_str(),
			D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT,
			0,
			textureFormat,
			pool,
			dwFilter,
			D3DX_FILTER_NONE,// (mip filter): D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR,
			m_colorKey,
			&fileDesc,
			NULL,
			&m_texture );

		if( FAILED( result ) )
		{
			Destroy();
			throw unify::Exception( "Failed to load image\"" + filePath.ToString() + "\"!" );
		}

		// Handle the colorkey
		if( m_useColorKey )	// D3DUSAGE_DYNAMIC must be set for D3DPOOL_DEFAULT textures (else D3DPOOL_MANAGED)
		{
			//D3DUtil_SetColorKey( m_texture[0][0], m_colorKey );
		}

		// Get the file dimensions...
		m_fileSize.width = fileDesc.Width;
		m_fileSize.height = fileDesc.Height;

		// Get the ACTUAL description of the Texture...
		D3DSURFACE_DESC textureDesc;
		m_texture->GetLevelDesc( 0, &textureDesc );
		m_imageSize.width = textureDesc.Width;
		m_imageSize.height = textureDesc.Height;
	}

	bool Use( unsigned int stage )
	{
		HRESULT hr = m_renderer->GetDxDevice()->SetTexture( stage, m_texture );
		return SUCCEEDED( hr );
	}	   

	void ClearHeader()
	{
		m_useColorKey = FALSE;
		m_colorKey = unify::Color::ColorZero();
		m_spriteMasterList.clear();
		m_spriteArrayMap.clear();
	}

	const unsigned int FileWidth() const
	{
		return m_fileSize.width;
	}

	const unsigned int FileHeight() const
	{
		return m_fileSize.height;
	}

	const unify::Size< unsigned int > & ImageSize() const
	{
		return m_imageSize;
	}

	void SetRenderable( bool renderable )
	{
		Destroy();

		m_parameters.renderable = renderable;
	}

	bool GetRenderable() const
	{
		return m_parameters.renderable;
	}

	void SetLockable( bool lockable )
	{
		Destroy();

		m_parameters.lockable = lockable;
	}

	bool GetLockable() const
	{
		return m_parameters.lockable;
	}

	void Preload()
	{
		// Clear the header file
		ClearHeader();

		// Load any new image header file if there
		LoadHeader();

		// Attempt to load the image for information
		Create();

		// Destroy our texture.
		Destroy();
	}

	void LoadHeader()
	{
		unify::Path imageHeaderFilepath( m_parameters.source.ChangeExtension( ".xml" ) );

		// Check if the file exists...
		if ( !imageHeaderFilepath.Exists() )
		{
			return;	// It's ok to not have an IHF file.
		}

		qxml::Document document;
		document.Load( imageHeaderFilepath );

		for ( const auto sheet : document.GetRoot()->Children( "addsheet" ) )
		{
			std::string name = sheet.GetAttributeElse< std::string >( "name", "" );
			unify::TexCoords arrayUL( sheet.GetAttributeElse< float >( "ulu", 0 ), sheet.GetAttributeElse< float >( "ulv", 0 ) );
			unify::Size< float > size;
			unify::RowColumn< unsigned int > arrayRC{ qxml::AttributeCast< unify::RowColumn< unsigned int >, unsigned int >( sheet, "r", "c" ) };
			assert( arrayRC.row && arrayRC.column );

			if ( sheet.HasAttributes( "sizeu,sizev" ) )
			{
				size.width = sheet.GetAttribute( "sizeu" )->Get< float >();
				size.height = sheet.GetAttribute( "sizev" )->Get< float >();
			}
			else
			{
				size = unify::Size< float >( (1 - arrayUL.u) / arrayRC.column, (1 - arrayUL.v) / arrayRC.column );
			}

			if ( !name.empty() )
			{
				unsigned int head = static_cast< unsigned int >(m_spriteMasterList.size());
				m_spriteArrayMap[name] = SpriteArray( head, head + arrayRC.row * arrayRC.column );
			}

			unify::RowColumn< unsigned int > rc;
			for ( rc.row = 0; rc.row < arrayRC.row; ++rc.row )
			{
				for ( rc.column = 0; rc.column < arrayRC.column; ++rc.column )
				{
					unify::TexCoords ul( arrayUL.u + rc.column * size.width, arrayUL.v + rc.row * size.height );
					unify::TexCoords dr( ul + unify::TexCoords( size.width, size.height ) );
					m_spriteMasterList.push_back( unify::TexArea( ul, dr ) );
				}
			}
		}
	}

	bool HasSpriteArray( const std::string & name ) const
	{
		SpriteArrayMap::const_iterator itr = m_spriteArrayMap.find( name );
		return itr == m_spriteArrayMap.end() ? false : true;
	}

	const Texture::SpriteArray & FindSpriteArray( const std::string & name ) const
	{
		SpriteArrayMap::const_iterator itr = m_spriteArrayMap.find( name );
		if ( itr == m_spriteArrayMap.end() )
		{
			throw unify::Exception( "Sprite array not found!" );
		}
		return itr->second;
	}

	const Texture::SpriteArray & GetSpriteArray( unsigned int index ) const
	{
		assert( index < m_spriteArrayMap.size() );

		unsigned int currentIndex = 0;
		SpriteArrayMap::const_iterator itr = m_spriteArrayMap.begin();
		for ( itr; itr != m_spriteArrayMap.end() && currentIndex != index; ++itr, ++currentIndex )
		{
			// Empty.
		}
		return itr->second;
	}

	unsigned int SpriteArrayCount() const
	{
		return static_cast< unsigned int >(m_spriteArrayMap.size());
	}

	const unify::TexArea & GetSprite( unsigned int index ) const
	{
		assert( index < m_spriteMasterList.size() );
		return m_spriteMasterList[index];
	}

	const unify::TexArea & GetSprite( const std::string & arrayName, unsigned int index ) const
	{
		const SpriteArrayMap::const_iterator itr = m_spriteArrayMap.find( arrayName );
		if ( itr == m_spriteArrayMap.end() )
		{
			throw unify::Exception( "Sprite array not found!" );
		}

		assert( index < itr->second.tail );

		return m_spriteMasterList[itr->second.head + index];
	}

	unsigned int SpriteCount() const
	{
		return static_cast< unsigned int >(m_spriteMasterList.size());
	}
};


Texture::Texture( me::IRenderer * renderer, TextureParameters parameters )
	: m_pimpl( new Pimpl( *this, renderer, parameters ) )
{
	Create();
}

Texture::~Texture()
{
}

void Texture::Create()
{
	m_pimpl->Create();
}

// Destroyes data, keeps header intact - thus no graphical footprint, yet we can still use it's statistics/dimensions in calculations.
void Texture::Destroy()
{
	m_pimpl->Destroy();
}

void Texture::ClearHeader()
{
	m_pimpl->ClearHeader();
}

const unsigned int Texture::FileWidth() const
{
	return m_pimpl->FileWidth();
}

const unsigned int Texture::FileHeight() const
{
	return m_pimpl->FileHeight();
}

const unify::Size< unsigned int > & Texture::ImageSize() const
{
	return m_pimpl->ImageSize();
}

bool Texture::GetRenderable() const
{
	return m_pimpl->GetRenderable();
}

bool Texture::GetLockable() const
{
	return m_pimpl->GetLockable();
}

void Texture::LockRect( unsigned int level, TextureLock & lock, const unify::Rect< long > * rect, bool readonly )
{
	m_pimpl->LockRect( level, lock, rect, readonly );
}

void Texture::UnlockRect( unsigned int level )
{
	m_pimpl->UnlockRect( level );
}

bool Texture::Use( unsigned int stage )
{
	return m_pimpl->Use( stage );
}

// Load all possible info (short of bits) about the texture
void Texture::Preload()
{
	m_pimpl->Preload();
}

void Texture::LoadHeader()
{
	m_pimpl->LoadHeader();
}

// Load the actual image file...
void Texture::LoadImage( const unify::Path & filePath )
{
	m_pimpl->LoadImage( filePath );
}

bool Texture::HasSpriteArray( const std::string & name ) const
{
	return m_pimpl->HasSpriteArray( name );
}

const Texture::SpriteArray & Texture::FindSpriteArray( const std::string & name ) const
{
	return m_pimpl->FindSpriteArray( name );
}

const Texture::SpriteArray & Texture::GetSpriteArray( unsigned int index ) const
{
	return m_pimpl->GetSpriteArray( index );
}

unsigned int Texture::SpriteArrayCount() const
{
	return m_pimpl->SpriteArrayCount();
}

const unify::TexArea & Texture::GetSprite( unsigned int index ) const
{
	return m_pimpl->GetSprite( index );
}

const unify::TexArea & Texture::GetSprite( const std::string & arrayName, unsigned int index ) const
{
	return m_pimpl->GetSprite( arrayName, index );
}

unsigned int Texture::SpriteCount() const
{
	return m_pimpl->SpriteCount();
}								  