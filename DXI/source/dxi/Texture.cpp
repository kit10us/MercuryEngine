// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
//

#include <dxi/Texture.h>
#include <dxi/core/Game.h>
#include <d3dx9tex.h>
#include <qxml/Document.h>
#include <unify/RowColumn.h>
#include <unify/Path.h>

using namespace dxi;

bool Texture::s_allowTextureUses = true;

Texture::Texture()
: m_texture( 0 )
, m_useColorKey( false )
, m_flags( 0 )
, m_pool( D3DPOOL_DEFAULT )
, m_textureFormat( D3DFMT_UNKNOWN )
, m_created( false )
{
}

Texture::Texture( const unify::Path & filePath, unsigned long flags )
: Texture()
{
	SetFlags( flags );

	m_filePath = filePath;
	Create();
}

Texture::~Texture()
{
	ClearHeader();
	Destroy();
}

void Texture::Create()
{
	// Load from a file.
	if( m_filePath.Empty() )
	{
		throw unify::Exception( "Not enough information to load Texture!" );
	}

	LoadHeader();

	LoadImage( m_filePath );

	m_created = true;
}

// Will return if already loaded and ready and validated.
void Texture::CreateFromFile( const unify::Path & filePath, unsigned long flags )
{
	m_filePath = filePath;
	SetFlags( flags );

	Create();
}

// Destroyes data, keeps header intact - thus no graphical footprint, yet we can still use it's statistics/dimensions in calculations.
void Texture::Destroy()
{
	if( m_created )
	{
		if ( m_texture )
		{
			m_texture->Release();
			m_texture = 0;
		}
		m_created = false;
	}
}

void Texture::ClearHeader()
{
	m_useColorKey = FALSE;
	m_colorKey = unify::Color::ColorZero();
	m_flags = 0;
	m_spriteMasterList.clear();
	m_spriteArrayMap.clear();
}

const unsigned int Texture::FileWidth() const
{
	return m_fileSize.width;
}

const unsigned int Texture::FileHeight() const	
{
	return m_fileSize.height;
}

const unify::Size< unsigned int > & Texture::ImageSize() const
{
	return m_imageSize;
}

void Texture::SetFlags( unsigned long dwFlags )
{
	Destroy();

	m_flags = dwFlags;

	m_pool = D3DPOOL_DEFAULT;

	// Texture is NOT renderable, and is lockable.
	if( unify::CheckFlag(m_flags, TEXTURE_NORENDER) )
	{
		m_pool = D3DPOOL_SCRATCH;
	}
	
	// Texture is renderable an is lockable.
	else if( unify::CheckFlag(m_flags, TEXTURE_LOCKABLE) )
	{
		m_pool = D3DPOOL_MANAGED;
	}
}
const unsigned long Texture::GetFlags()	const
{
	return m_flags;
}

void Texture::LockRect( unsigned int level, TextureLock & lock, const unify::Rect< long > * rect, unsigned long flags )
{
	// Ensure texture is loaded first...
	if( ! m_created )
	{
		Create();
	}

	// Allow us to convert from our flag type to DX.
	unsigned int dxFlags = flags;

	D3DLOCKED_RECT d3dLockedRect;
	if( FAILED(m_texture->LockRect( level, &d3dLockedRect, (RECT*)rect, dxFlags ) ) )
	{
		lock.pBits = 0;
		lock.uStride = 0;
		throw unify::Exception( "Failed to lock texture: \"" + m_filePath.ToString() + "\"" );
	}

	lock.pBits = (unsigned char*)d3dLockedRect.pBits;
	lock.uStride = d3dLockedRect.Pitch / 4;
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

void Texture::UnlockRect( unsigned int level )
{
	m_texture->UnlockRect( level );
}

bool Texture::Use( unsigned int stage )
{
	if( ! s_allowTextureUses )
	{
		return true;
	}

	// TODO: This statistic keeping and debug features might be best wrapped and removed in release builds.
	// TODO: ...find all places and investigate
	//GetDisplay()->GetStatistics()->Increment( Statistic_SetTexture );

	HRESULT hr = win::DX::GetDxDevice()->SetTexture( stage, m_texture );
	return SUCCEEDED( hr );
}

void Texture::UnsetTexture( unsigned int name )
{
	//GetDisplay()->GetStatistics()->Increment( Statistic_UnsetTexture );
	win::DX::GetDxDevice()->SetTexture( name, 0 );
}

// Load all possible info (short of bits) about the texture
void Texture::Preload()
{
	// Clear the header file
	ClearHeader();

	// Set defaults that can be overrided by texture...
	m_textureFormat = D3DFMT_UNKNOWN;

	// Load any new image header file if there
	LoadHeader();

	// Attempt to load the image for information
	Create();

	// Destroy our texture.
	Destroy();
}

void Texture::LoadHeader()
{
	unify::Path imageHeaderFilepath(m_filePath);
	imageHeaderFilepath.ChangeExtension( ".xml" );

	// Check if the file exists...
	if( ! imageHeaderFilepath.Exists() )
	{
		return;	// It's ok to not have an IHF file.
	}

	qxml::Document document;
	document.Load( imageHeaderFilepath );

	qxml::ElementList sheets;
	document.FindElementsByTagName( sheets, "addsheet" );
	for( unsigned int i = 0; i < sheets.Count(); ++i )
	{
		qxml::Element * element = sheets.Item( i );
		
		std::string name = element->GetStringAttributeElse( "name", "" );
		unify::TexCoords arrayUL( element->GetFloatAttributeElse( "ulu", 0 ), element->GetFloatAttributeElse( "ulv", 0 ) );
		unify::Size< float > size;
		unify::RowColumn< unsigned int > arrayRC( element->GetAttribute( "r" )->GetInteger(), element->GetAttribute( "c" )->GetInteger() );
		assert( arrayRC.row && arrayRC.column );

		if( element->HasAttributes( "sizeu,sizev" ) )
		{
			size.width = element->GetAttribute( "sizeu" )->GetFloat();
			size.height = element->GetAttribute( "sizev" )->GetFloat();
		}
		else
		{
			size = unify::Size< float >( (1 - arrayUL.u) / arrayRC.column, (1 - arrayUL.v) / arrayRC.column );
		}

		if( ! name.empty() )
		{
			unsigned int head = static_cast< unsigned int >( m_spriteMasterList.size() );
			m_spriteArrayMap[ name ] = SpriteArray( head, head + arrayRC.row * arrayRC.column );
		}

		unify::RowColumn< unsigned int > rc;
		for( rc.row = 0; rc.row < arrayRC.row; ++rc.row )
		{
			for( rc.column = 0; rc.column < arrayRC.column; ++rc.column )
			{
				unify::TexCoords ul( arrayUL.u + rc.column * size.width, arrayUL.v + rc.row * size.height );
				unify::TexCoords dr( ul + unify::TexCoords( size.width, size.height ) );
				m_spriteMasterList.push_back( unify::TexArea( ul, dr ) );
			}
		}
	}
}

// Load the actual image file...
void Texture::LoadImage( const unify::Path & filePath )
{
	unsigned int dwFilter = D3DX_FILTER_NONE;;
	
	// Release any previous texture
	Destroy();

	// Verify file exists
	if( ! filePath.Exists() )
	{
		throw unify::Exception( "Failed to load image, file not found! (" + filePath.ToString() + ")" );
	}
	
	// First, load the file to find out it's sizes...	
	D3DXIMAGE_INFO fileDesc;

	HRESULT hr;
	hr = D3DXCreateTextureFromFileExA(
		win::DX::GetDxDevice(),
		(LPCSTR)filePath.ToString().c_str(),
		D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2,		
		D3DX_DEFAULT,
		0,
		m_textureFormat,
		m_pool,
		dwFilter,
		D3DX_FILTER_NONE,// (mip filter): D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR,
		m_colorKey,
		&fileDesc,
		NULL,
		&m_texture);

	if( FAILED(hr) )
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
	m_fileSize.width	= fileDesc.Width;
	m_fileSize.height	= fileDesc.Height;

	// Get the ACTUAL description of the Texture...
	D3DSURFACE_DESC textureDesc;
	m_texture->GetLevelDesc(0, &textureDesc);
	m_imageSize.width = textureDesc.Width;
	m_imageSize.height = textureDesc.Height;
}

bool Texture::HasSpriteArray( const std::string & name ) const
{
	SpriteArrayMap::const_iterator itr = m_spriteArrayMap.find( name );
	return itr == m_spriteArrayMap.end() ? false : true;
}

const Texture::SpriteArray & Texture::FindSpriteArray( const std::string & name ) const
{
	SpriteArrayMap::const_iterator itr = m_spriteArrayMap.find( name );
	if( itr == m_spriteArrayMap.end() )
	{
		throw unify::Exception( "Sprite array not found!" );
	}
	return itr->second;
}

const Texture::SpriteArray & Texture::GetSpriteArray( unsigned int index ) const
{
	assert( index < m_spriteArrayMap.size() );

	unsigned int currentIndex = 0;
	SpriteArrayMap::const_iterator itr = m_spriteArrayMap.begin();
	for( itr; itr != m_spriteArrayMap.end() && currentIndex != index; ++itr, ++currentIndex )
	{
		// Empty.
	}
	return itr->second;
}

unsigned int Texture::SpriteArrayCount() const
{
	return static_cast< unsigned int >( m_spriteArrayMap.size() );
}

const unify::TexArea & Texture::GetSprite( unsigned int index ) const
{
	assert( index < m_spriteMasterList.size() );
	return m_spriteMasterList[ index ];
}

const unify::TexArea & Texture::GetSprite( const std::string & arrayName, unsigned int index ) const
{
	const SpriteArrayMap::const_iterator itr = m_spriteArrayMap.find( arrayName );
	if( itr == m_spriteArrayMap.end() )
	{
		throw unify::Exception( "Sprite array not found!" );
	}

	assert( index < itr->second.tail );

	return m_spriteMasterList[ itr->second.head + index ];
}

unsigned int Texture::SpriteCount() const
{
	return static_cast< unsigned int >( m_spriteMasterList.size() );
}

