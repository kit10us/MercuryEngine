// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
//

#if defined( DIRECTX9 )
#elif defined( DIRECTX11 )
#include <DirectXTex.h>
#include <DDS.h>
#pragma comment( lib, "DirectXTex" )
#endif

#include <dxi/Texture.h>
#include <dxi/win/DXDevice.h>
#include <dxi/core/Game.h>
#include <dxi/exception/NotImplemented.h>
#include <atlbase.h>
#include <dxi/exception/FailedToLock.h>

// MS agressive macros.
#ifdef LoadImage
#undef LoadImage
#endif

using namespace dxi;

class Texture::Pimpl
{
public:
	Pimpl( Texture & owner )
		: m_owner( owner )
	{
	}

	~Pimpl()
	{
	}

	void Destroy()
	{
		m_texture = nullptr;
	}

	void Set( bool renderable, bool lockable )
	{
		m_owner.m_renderable = renderable;
		m_owner.m_lockable = lockable;
	}

	void LockRect( unsigned int level, TextureLock & lock, const unify::Rect< long > * rect, bool readonly )
	{
		if( m_owner.m_lockable == false )
		{
			throw exception::FailedToLock( "Texture is not lockable!" );
		}
#if defined( DIRECTX9 )
		// Allow us to convert from our flag type to DX.
		unsigned int dxFlags = readonly ? D3DLOCK_READONLY : 0;

		D3DLOCKED_RECT d3dLockedRect;
		if( FAILED( m_texture->LockRect( level, &d3dLockedRect, (RECT*)rect, dxFlags ) ) )
		{
			lock.pBits = 0;
			lock.uStride = 0;
			throw unify::Exception( "Failed to lock texture: \"" + m_owner.m_filePath.ToString() + "\"" );
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
			lock.uWidth = m_owner.m_imageSize.width;
			lock.uHeight = m_owner.m_imageSize.height;
		}
#elif defined( DIRECTX11 )
		throw exception::NotImplemented( "DX11" );
#endif

	}

	void UnlockRect( unsigned int level )
	{
#if defined( DIRECTX9 )
		m_texture->UnlockRect( level );
#elif defined( DIRECTX11 )
		throw exception::NotImplemented( "DX11" );
#endif
	}

	void LoadImage( unify::Path filePath )
	{
#if defined( DIRECTX9 )
		// Set defaults that can be overrided by texture...
		D3DFORMAT textureFormat = D3DFMT_UNKNOWN;

		// First, load the file to find out it's sizes...	
		D3DXIMAGE_INFO fileDesc;
		unsigned int dwFilter = D3DX_FILTER_NONE;


		D3DPOOL pool = D3DPOOL_DEFAULT;
		if( m_owner.m_lockable )
		{
			pool = m_owner.m_renderable ? D3DPOOL_MANAGED : D3DPOOL_SCRATCH;
		}

		HRESULT result;
		result = D3DXCreateTextureFromFileExA(
			win::DX::GetDxDevice(),
			(LPCSTR)filePath.ToString().c_str(),
			D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT,
			0,
			textureFormat,
			pool,
			dwFilter,
			D3DX_FILTER_NONE,// (mip filter): D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR,
			m_owner.m_colorKey,
			&fileDesc,
			NULL,
			&m_texture );

		if( FAILED( result ) )
		{
			Destroy();
			throw unify::Exception( "Failed to load image\"" + filePath.ToString() + "\"!" );
		}

		// Handle the colorkey
		if( m_owner.m_useColorKey )	// D3DUSAGE_DYNAMIC must be set for D3DPOOL_DEFAULT textures (else D3DPOOL_MANAGED)
		{
			//D3DUtil_SetColorKey( m_texture[0][0], m_colorKey );
		}

		// Get the file dimensions...
		m_owner.m_fileSize.width = fileDesc.Width;
		m_owner.m_fileSize.height = fileDesc.Height;

		// Get the ACTUAL description of the Texture...
		D3DSURFACE_DESC textureDesc;
		m_texture->GetLevelDesc( 0, &textureDesc );
		m_owner.m_imageSize.width = textureDesc.Width;
		m_owner.m_imageSize.height = textureDesc.Height;
#elif defined( DIRECTX11 )
		HRESULT result = S_OK;

		//DirectX::Image sourceImages{};
		DirectX::TexMetadata texMetadata{};	

		DirectX::ScratchImage scratch{};

		if ( m_owner.m_filePath.IsExtension( "DDS" ) )
		{
			result = DirectX::LoadFromDDSFile( unify::Cast< std::wstring >( m_owner.m_filePath.ToString() ).c_str(), 0, &texMetadata, scratch );
		}
		else if ( m_owner.m_filePath.IsExtension( "BMP" ) || m_owner.m_filePath.IsExtension( "JPG" )  || m_owner.m_filePath.IsExtension( "JPEG" ) || m_owner.m_filePath.IsExtension( "TIFF" ) || m_owner.m_filePath.IsExtension( "TIF" ) || m_owner.m_filePath.IsExtension( "HDP" ) )
		{
			result = DirectX::LoadFromWICFile( unify::Cast< std::wstring >( m_owner.m_filePath.ToString() ).c_str(), 0, &texMetadata, scratch );												  			
		}
		else if( m_owner.m_filePath.IsExtension( "TGA" ) )
		{
			result = DirectX::LoadFromTGAFile( unify::Cast< std::wstring >( m_owner.m_filePath.ToString() ).c_str(), &texMetadata, scratch );
		}
		assert( !FAILED( result ) );

		unsigned int width = scratch.GetImage( 0, 0, 0 )->width;
		unsigned int height = scratch.GetImage( 0, 0, 0 )->height;

		D3D11_SUBRESOURCE_DATA data{};
		data.pSysMem = scratch.GetImage( 0, 0, 0 )->pixels;
		data.SysMemPitch = scratch.GetImage( 0, 0, 0 )->rowPitch;
		data.SysMemSlicePitch = scratch.GetImage( 0, 0, 0 )->slicePitch;

		D3D11_TEXTURE2D_DESC textureDesc{};
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = scratch.GetImage( 0, 0, 0 )->format;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DYNAMIC;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		textureDesc.MiscFlags = 0;
		result = win::DX::GetDxDevice()->CreateTexture2D( &textureDesc, &data, &m_texture );
		if( FAILED( result ) )
		{
			Destroy();
			throw unify::Exception( "Failed to load image\"" + filePath.ToString() + "\"!" );
		}

		scratch.Release();

		D3D11_SAMPLER_DESC colorMapDesc{};
		colorMapDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		colorMapDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		colorMapDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		colorMapDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		colorMapDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		colorMapDesc.MipLODBias = 0.0f;
		colorMapDesc.BorderColor[0] = 0;
		colorMapDesc.BorderColor[1] = 0;
		colorMapDesc.BorderColor[2] = 0;
		colorMapDesc.BorderColor[3] = 0;
		colorMapDesc.MinLOD = 0;
		colorMapDesc.MaxLOD = D3D11_FLOAT32_MAX;
		result = win::DX::GetDxDevice()->CreateSamplerState( &colorMapDesc, &m_colorMapSampler );
		assert( !FAILED( result ) );

		D3D11_SHADER_RESOURCE_VIEW_DESC textureResourceDesc{};
		textureResourceDesc.Format = textureDesc.Format;
		textureResourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		textureResourceDesc.Texture2D.MipLevels = 1;
		textureResourceDesc.Texture2D.MostDetailedMip = 0;

		result = win::DX::GetDxDevice()->CreateShaderResourceView( m_texture, &textureResourceDesc, &m_colorMap );
		assert( !FAILED( result ) );
#endif
	}

	bool Use( unsigned int stage )
	{
#if defined( DIRECTX9 )
		HRESULT hr = win::DX::GetDxDevice()->SetTexture( stage, m_texture );
		return SUCCEEDED( hr );
#elif defined( DIRECTX11 )
		win::DX::GetDxContext()->PSSetShaderResources( 0, 1, &m_colorMap.p );
		win::DX::GetDxContext()->PSSetSamplers( 0, 1, &m_colorMapSampler.p );
		return true;
#endif
	}

	private:
		Texture & m_owner;

#if defined( DIRECTX9 )
		CComPtr< IDirect3DTexture9 > m_texture;
#elif defined( DIRECTX11 )
		CComPtr< ID3D11Texture2D > m_texture;
		CComPtr< ID3D11SamplerState > m_colorMapSampler;
		CComPtr< ID3D11ShaderResourceView > m_colorMap;
#endif
	};
		

	bool Texture::s_allowTextureUses = true;

	Texture::Texture()
		: m_useColorKey( false )
		, m_created( false )
		, m_renderable( true )
		, m_lockable( false )
		, m_pimpl( new Pimpl( *this ) )
	{
	}

	Texture::Texture( const unify::Path & filePath, bool renderable, bool lockable )
		: Texture()
	{
		m_renderable = renderable;
		m_lockable = lockable;

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

		m_pimpl->LoadImage( m_filePath );

		m_created = true;
	}

	// Will return if already loaded and ready and validated.
	void Texture::CreateFromFile( const unify::Path & filePath, bool renderable, bool lockable )
	{
		m_filePath = filePath;
		SetRenderable( renderable );
		SetLockable( lockable );
		Create();
	}

	// Destroyes data, keeps header intact - thus no graphical footprint, yet we can still use it's statistics/dimensions in calculations.
	void Texture::Destroy()
	{
		m_pimpl->Destroy();
		m_created = false;
	}

	void Texture::ClearHeader()
	{
		m_useColorKey = FALSE;
		m_colorKey = unify::Color::ColorZero();
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

	void Texture::SetRenderable( bool renderable )
	{
		Destroy();

		m_renderable = renderable;
	}

	bool Texture::GetRenderable() const
	{
		return m_renderable;
	}

	void Texture::SetLockable( bool lockable )
	{
		Destroy();

		m_lockable = lockable;
	}

	bool Texture::GetLockable() const
	{
		return m_lockable;
	}

	void Texture::LockRect( unsigned int level, TextureLock & lock, const unify::Rect< long > * rect, bool readonly )
	{
		// Ensure texture is loaded first...
		if( !m_created )
		{
			Create();
		}

		m_pimpl->LockRect( level, lock, rect, readonly );
	}

	void Texture::UnlockRect( unsigned int level )
	{
		m_pimpl->UnlockRect( level );
	}

	bool Texture::Use( unsigned int stage )
	{
		if( !s_allowTextureUses )
		{
			return true;
		}

		// TODO: This statistic keeping and debug features might be best wrapped and removed in release builds.
		// TODO: ...find all places and investigate
		//GetDisplay()->GetStatistics()->Increment( Statistic_SetTexture );

		return m_pimpl->Use( stage );
	}

	// Load all possible info (short of bits) about the texture
	void Texture::Preload()
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

	void Texture::LoadHeader()
	{
		unify::Path imageHeaderFilepath( m_filePath.ChangeExtension( ".xml" ) );

		// Check if the file exists...
		if( !imageHeaderFilepath.Exists() )
		{
			return;	// It's ok to not have an IHF file.
		}

		qxml::Document document;
		document.Load( imageHeaderFilepath );

		for( const auto sheet : document.GetRoot()->Children( "addsheet" ) )
		{
			std::string name = sheet.GetAttributeElse< std::string >( "name", "" );
			unify::TexCoords arrayUL( sheet.GetAttributeElse< float >( "ulu", 0 ), sheet.GetAttributeElse< float >( "ulv", 0 ) );
			unify::Size< float > size;
			unify::RowColumn< unsigned int > arrayRC{ qxml::AttributeCast< unify::RowColumn< unsigned int >, unsigned int >( sheet, "r", "c" ) };
			assert( arrayRC.row && arrayRC.column );

			if( sheet.HasAttributes( "sizeu,sizev" ) )
			{
				size.width = sheet.GetAttribute( "sizeu" )->Get< float >();
				size.height = sheet.GetAttribute( "sizev" )->Get< float >();
			}
			else
			{
				size = unify::Size< float >( (1 - arrayUL.u) / arrayRC.column, (1 - arrayUL.v) / arrayRC.column );
			}

			if( !name.empty() )
			{
				unsigned int head = static_cast< unsigned int >(m_spriteMasterList.size());
				m_spriteArrayMap[name] = SpriteArray( head, head + arrayRC.row * arrayRC.column );
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
		// Release any previous texture
		Destroy();

		// Verify file exists
		if( !filePath.Exists() )
		{
			throw unify::Exception( "Failed to load image, file not found! (" + filePath.ToString() + ")" );
		}

		m_pimpl->LoadImage( filePath );
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
		return static_cast< unsigned int >(m_spriteArrayMap.size());
	}

	const unify::TexArea & Texture::GetSprite( unsigned int index ) const
	{
		assert( index < m_spriteMasterList.size() );
		return m_spriteMasterList[index];
	}

	const unify::TexArea & Texture::GetSprite( const std::string & arrayName, unsigned int index ) const
	{
		const SpriteArrayMap::const_iterator itr = m_spriteArrayMap.find( arrayName );
		if( itr == m_spriteArrayMap.end() )
		{
			throw unify::Exception( "Sprite array not found!" );
		}

		assert( index < itr->second.tail );

		return m_spriteMasterList[itr->second.head + index];
	}

	unsigned int Texture::SpriteCount() const
	{
		return static_cast< unsigned int >(m_spriteMasterList.size());
	}

