#pragma once
#include <pod.h>
#include <d3dx9tex.h>
#include <string>
#include <DisplayActual.h>

class Texture
{
public:
	typedef unsigned int MipLevels;
	static const unsigned int MipLevelsAll = 0;
	static const unsigned int MipLevelsFromFile = D3DX_FROM_FILE;

	enum Filter {
		FilterNone, // D3DX_FILTER_NONE
		FilterPoint, // D3DX_FILTER_POINT
		FilterLinear, // D3DX_FILTER_LINEAR
		FilterTriangle, // D3DX_FILTER_TRIANGLE
		FilterBox // D3DX_FILTER_BOX
	};

	Texture( DisplayActual & displayActual );
	Texture( DisplayActual & displayActual, const std::string filePath, bool powerOf2Sizes = false, MipLevels mipLevels = MipLevelsAll, Filter filter = FilterTriangle, bool mirror = false, bool dither = true );
	virtual ~Texture();
	void Destroy();
	void Load( const std::string filePath, bool powerOf2Sizes = false, MipLevels mipLevels= MipLevelsAll, Filter filter = FilterTriangle, bool mirror = false, bool dither = true );
	void Use( unsigned int stage );
	Size GetSize() const;
	void Draw( V2 position, float scale = 1.0f );
	void Draw( V2 position, Size size );
	void Draw( Rect rect );

	IDirect3DTexture9 * GetTexture();

protected:
	DisplayActual & m_displayActual;
	D3DXIMAGE_INFO m_imageInfo;
	CComPtr< IDirect3DTexture9 > m_texture;
};

