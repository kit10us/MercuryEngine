#include <Texture.h>
#include <TesterBase.h>
#include <assert.h>

Texture::Texture( DisplayActual & displayActual )
: m_displayActual( displayActual )
{
}

Texture::Texture( DisplayActual & displayActual, const std::string filePath, bool powerOf2Sizes, MipLevels mipLevels, Filter filter, bool mirror , bool dither )
: m_displayActual( displayActual )
{
	Load( filePath, powerOf2Sizes, mipLevels, filter, mirror, dither );
}

Texture::~Texture()
{
	Destroy();
}

void Texture::Destroy()
{
	m_texture = 0;
}

void Texture::Load( const std::string filePath, bool powerOf2Sizes, MipLevels mipLevels, Filter filter, bool mirror, bool dither )
{
	HRESULT hr = S_OK;
	/*
HRESULT D3DXCreateTextureFromFileEx(
  __in     LPDIRECT3DDEVICE9 pDevice,
  __in     LPCTSTR pSrcFile,
  __in     UINT Width,
  __in     UINT Height,
  __in     UINT MipLevels,
  __in     DWORD Usage,
  __in     D3DFORMAT Format,
  __in     D3DPOOL Pool,
  __in     DWORD Filter,
  __in     DWORD MipFilter,
  __in     D3DCOLOR ColorKey,
  __inout  D3DXIMAGE_INFO *pSrcInfo,
  __out    PALETTEENTRY *pPalette,
  __out    LPDIRECT3DTEXTURE9 *ppTexture
);	
	*/
	D3DFORMAT d3dfFormat = D3DFMT_A8R8G8B8;
	unsigned int width = powerOf2Sizes ? D3DX_DEFAULT : D3DX_DEFAULT_NONPOW2;
	unsigned int height = powerOf2Sizes ? D3DX_DEFAULT : D3DX_DEFAULT_NONPOW2;

	// Filter...
	unsigned int filterFinal = int();
	switch( filter )
	{
	case FilterNone:
		filterFinal = D3DX_FILTER_NONE;
		break;
	case FilterPoint:
		filterFinal = D3DX_FILTER_POINT;
		break;
	case FilterLinear:
		filterFinal = D3DX_FILTER_LINEAR;
		break;
	case FilterTriangle:
		filterFinal = D3DX_FILTER_TRIANGLE;
		break;
	case FilterBox:
		filterFinal = D3DX_FILTER_BOX;
		break;
	}

	if ( dither )
	{
		filterFinal |= D3DX_FILTER_DITHER;
	}

	if ( mirror )
	{
		filterFinal |= D3DX_FILTER_MIRROR;
	}

	unsigned int mipFilterFinal = filterFinal;

	hr = D3DXCreateTextureFromFileExA( m_displayActual.GetDxDevice(), filePath.c_str(),
		width, height,
		mipLevels,
		0,
		d3dfFormat, //D3DFMT_A8R8G8B8, //D3DFMT_R5G6B5  ?
		
		D3DPOOL_MANAGED,
		//D3DPOOL_DEFAULT,
		filterFinal,/*D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR,*/
		mipFilterFinal,
		0,
		&m_imageInfo,
		0,

		&m_texture );
	if( FAILED( hr ) )
	{
		throw std::exception( "Failed to create texture!" );
	}
}

void Texture::Use( unsigned int stage )
{
	HRESULT hr;
	hr = m_displayActual.GetDxDevice()->SetTexture( stage, m_texture );
	assert( SUCCEEDED( hr ) );
}

Size Texture::GetSize() const
{
	return Size( static_cast< float >( m_imageInfo.Width ), static_cast< float >( m_imageInfo.Height ) );
}

void Texture::Draw( Rect rect )
{
	struct Vertex 
	{
		float x, y, z, rhw;
		float u, v;
	} vb [] =
	{
		{ rect.ul.x, rect.ul.y, 1, 0, 0, 0 },
		{ rect.dr.x, rect.ul.y, 1, 0, 1, 0 },
		{ rect.ul.x, rect.dr.y, 1, 0, 0, 1 },
		{ rect.dr.x, rect.dr.y, 1, 0, 1, 1 }
	};
	unsigned int fvf = D3DFVF_XYZRHW | D3DFVF_TEX1;

	HRESULT hr = S_OK;
	IDirect3DDevice9 & dxDevice = *m_displayActual.GetDxDevice();
	Use( 0 );

	hr = dxDevice.SetFVF( fvf );
	assert( SUCCEEDED( hr ) );

	hr = dxDevice.DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, (void*)&vb, sizeof( Vertex ) );
	assert( SUCCEEDED( hr ) );
}

void Texture::Draw( V2 position, Size size )
{
	Rect rect( position.x, position.y, position.x + size.width - 1, position.y + size.height - 1);
	Draw( rect );
}

void Texture::Draw( V2 position, float scale )
{
	Rect rect( position, V2( position.x + ( GetSize().width - 1 ) * scale, position.y + ( GetSize().height - 1 ) * scale ) );
	Draw( rect );
}

IDirect3DTexture9 * Texture::GetTexture()
{
	return m_texture;
}
