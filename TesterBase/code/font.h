#pragma once

#include <pod.h>
#include <d3dx9.h>
#include <string>
#include <map>
#include <vector>
#include <DisplayActual.h>

class Font
{
public:
    Font( DisplayActual & displayActual );
	~Font();

	bool AddVersion( unsigned int size, bool italic, bool bold );
	void Destroy();

    void Draw( V2 position, const std::string text, D3DCOLOR color = D3DCOLOR_XRGB( 255, 255, 255 ), Origin::TYPE alignment = Origin::Center );
	void Draw( size_t version, V2 position, const std::string text, D3DCOLOR color = D3DCOLOR_XRGB( 255, 255, 255 ), Origin::TYPE alignment = Origin::Center );

    void Draw( Rect rect, const std::string text, D3DCOLOR color = D3DCOLOR_XRGB( 255, 255, 255 ), Origin::TYPE alignment = Origin::Center );
	void Draw( size_t version, Rect rect, const std::string text, D3DCOLOR color = D3DCOLOR_XRGB( 255, 255, 255 ), Origin::TYPE alignment = Origin::Center );

    void DrawWithGlyphs( const Rect& rect, const std::string& text, Origin::TYPE alignment );
    void DrawWithGlyphs( size_t version, const Rect& rect, const std::string& text, Origin::TYPE alignment );

    void DrawWithD3DXCreateText( const Rect& rect, const std::string& text );
    void DrawWithD3DXCreateText( size_t version, const Rect& rect, const std::string& text );

    Size GetTextSize( const std::string text ) const;
    Size GetTextSize( size_t version, const std::string text ) const;

    bool HasVersion( unsigned int size, bool italic, bool bold );
    size_t FindVersion( unsigned int size, bool italic, bool bold );

    void SetRenderStateAndStorePrevious( const D3DRENDERSTATETYPE renderState, const DWORD value, std::map< D3DRENDERSTATETYPE, DWORD > &previousRenderStates );

private:
    struct GlyphData
    {
        GlyphData()
        : texture( 0 )
        {
            glyphDimensions.left = 0;
            glyphDimensions.top = 0;
            glyphDimensions.right = 0;
            glyphDimensions.bottom = 0;
        }
        
        IDirect3DTexture9* texture;;
        RECT glyphDimensions;
        POINT vectorToNextCharacter;  
    };

    Size GlyphDataFromGlyphs( size_t version, const WORD* const glyphIdentifiers, size_t numberOfGlyphIdentifiers, std::map< unsigned int, GlyphData >& glyphMap );

    class FontVersion
    {
    public:
        FontVersion( unsigned int size, bool italic, bool bold, ID3DXFont * font )
            : m_size( size )
            , m_italic( italic )
            , m_bold( bold )
            , m_dxFont( font ) // WARNING: Do not AddRef here, we are the primary owner.
        {
        }

        FontVersion( const FontVersion & fontVersion )
            : m_size( fontVersion.m_size )
            , m_italic( fontVersion.m_italic )
            , m_bold( fontVersion.m_bold )
            , m_dxFont( fontVersion.m_dxFont )
        {
            m_dxFont->AddRef(); // Prevents shallow copies from destroying
        }

        ~FontVersion()
        {
            if ( m_dxFont )
            {
                m_dxFont->Release();
                m_dxFont = 0;
            }
        }

        bool Match( unsigned int size, bool italic, bool bold ) const
        {
            return m_size == size && m_italic == italic && m_bold == bold;
        }

        ID3DXFont * GetDxFont() const
        {
            return m_dxFont;
        }

    private:
        unsigned int m_size;
        bool m_italic;
        bool m_bold;
        ID3DXFont * m_dxFont;
    };

	DisplayActual & m_displayActual;
    size_t m_defaultVersion;
    std::vector< FontVersion > m_versions;
    D3DXMATRIX m_tweak;
};

