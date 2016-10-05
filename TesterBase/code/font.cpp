#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif

#include <Font.h>
#include <TesterBase.h>
#include <assert.h>
#include <media/vertex/Textured.h>


Font::Font( DisplayActual & displayActual )
	: m_displayActual( displayActual )
	, m_defaultVersion( 0 )
{
}

Font::~Font()
{
	Destroy();
}

bool Font::AddVersion( unsigned int size, bool italic, bool bold )
{
	Destroy();

	D3DXFONT_DESCA fontDesc = D3DXFONT_DESCA();
    fontDesc.Height = size; //Height, in logical units, of the font's character cell or character.
	fontDesc.Width = 0; // Width, in logical units, of characters in the font.
    fontDesc.Weight = bold ? 1000 : 400 ; // Weight of the font in the range from 0 through 1000.
	fontDesc.MipLevels = 0; // Number of mip levels requested. If this value is zero or D3DX_DEFAULT, a complete mipmap chain is created. If the value is 1, the texture space is mapped identically to the screen space.
	fontDesc.Italic = italic; // Set to TRUE for an Italic font.
	fontDesc.CharSet = DEFAULT_CHARSET; // Character set.
	fontDesc.OutputPrecision = OUT_TT_PRECIS; // Output precision. The output precision defines how closely the output must match the requested font height, width, character orientation, escapement, pitch, and font type.
	fontDesc.Quality = CLIP_DEFAULT_PRECIS; // Output quality.
	fontDesc.PitchAndFamily = DEFAULT_PITCH; // Pitch and family of the font.
	strcpy( fontDesc.FaceName, "Arial" ); // A null-terminated string or characters that specifies the typeface name of the font. The length of the string must not exceed 32 characters, including the terminating null character. If FaceName is an empty string, the first font that matches the other specified attributes will be used. If the compiler settings require Unicode, the data type TCHAR resolves to WCHAR; otherwise, the data type resolves to CHAR.

	HRESULT hr = S_OK;
    ID3DXFont * dxFont;
	hr = D3DXCreateFontIndirectA( m_displayActual.GetDxDevice(), &fontDesc, &dxFont );
    if ( FAILED( hr ) )
    {
        return false;
    }

    m_versions.push_back( FontVersion( size, italic, bold, dxFont ) );

    return true;
}

void Font::Destroy()
{
}

void Font::Draw( V2 position, const std::string text, D3DCOLOR color, Origin::TYPE alignment )
{
    Draw( m_defaultVersion, position, text, color, alignment );
}

void Font::Draw( size_t version, V2 position, const std::string text, D3DCOLOR color, Origin::TYPE alignment )
{
    if ( version >= m_versions.size() )
    {
        throw std::exception( "Font version not found!" );
    }

    ID3DXFont * dxFont = m_versions[ version ].GetDxFont();

	Size size = GetTextSize( version, text );
	RECT rect = { 0, 0, 0, 0 };
	rect.left += static_cast< long >( position.x - size.width );
	rect.top += static_cast< long >( position.y - size.height);
	rect.right += static_cast< long >( position.x + size.width );
	rect.bottom += static_cast< long >( position.y + size.height );

	unsigned int format = int(); // To remove warning C4701: potentially uninitialized local variable 'format' used
	switch( alignment )
	{
	case Origin::UL:
		format = DT_TOP | DT_LEFT;
		break;
	case Origin::UR:
		format = DT_TOP | DT_RIGHT;
		break;
	case Origin::DL:
		format = DT_BOTTOM | DT_LEFT;
		break;
	case Origin::DR:
		format = DT_BOTTOM | DT_RIGHT;
		break;
	case Origin::Top:
		format = DT_TOP | DT_CENTER;
		break;
	case Origin::Right:
		format = DT_RIGHT | DT_VCENTER;
		break;
	case Origin::Bottom:
		format = DT_BOTTOM | DT_CENTER;
		break;
	case Origin::Left:
		format = DT_LEFT | DT_VCENTER;
		break;
	case Origin::Center:
		// Fall through...
	default:
		format = DT_CENTER | DT_VCENTER;
		break;
	}

	dxFont->DrawTextA( 0, text.c_str(), -1, &rect, format, color );
}

void Font::Draw( const Rect rect, const std::string text, D3DCOLOR color, Origin::TYPE alignment )
{
    Draw( m_defaultVersion, rect, text, color, alignment );
}

void Font::Draw( size_t version, const Rect rect, const std::string text, D3DCOLOR color, Origin::TYPE alignment )
{
    if ( version >= m_versions.size() )
    {
        throw std::exception( "Font version not found!" );
    }

    ID3DXFont * dxFont = m_versions[ version ].GetDxFont();

    Size size = GetTextSize( version, text );
	RECT rectWindows = { 0, 0, 0, 0 };
	rectWindows.left = static_cast< long >( rect.ul.x );
	rectWindows.top = static_cast< long >( rect.ul.y );
	rectWindows.right = static_cast< long >( rect.dr.x );
	rectWindows.bottom = static_cast< long >( rect.dr.y );

	unsigned int format = int(); // To remove warning C4701: potentially uninitialized local variable 'format' used
	switch( alignment )
	{
	case Origin::UL:
		format = DT_TOP | DT_LEFT;
		break;
	case Origin::UR:
		format = DT_TOP | DT_RIGHT;
		break;
	case Origin::DL:
		format = DT_BOTTOM | DT_LEFT;
		break;
	case Origin::DR:
		format = DT_BOTTOM | DT_RIGHT;
		break;
	case Origin::Top:
		format = DT_TOP | DT_CENTER;
		break;
	case Origin::Right:
		format = DT_RIGHT | DT_VCENTER;
		break;
	case Origin::Bottom:
		format = DT_BOTTOM | DT_CENTER;
		break;
	case Origin::Left:
		format = DT_LEFT | DT_VCENTER;
		break;
	case Origin::Center:
		// Fall through...
	default:
		format = DT_CENTER | DT_VCENTER;
		break;
	}

	dxFont->DrawTextA( 0, text.c_str(), -1, &rectWindows, format, color );
}

Size Font::GetTextSize( const std::string text ) const
{
    return GetTextSize( m_defaultVersion, text );
}

Size Font::GetTextSize( size_t version, const std::string text ) const
{
    if ( version >= m_versions.size() )
    {
        throw std::exception( "Font version not found!" );
    }

    ID3DXFont * dxFont = m_versions[ version ].GetDxFont();

    RECT rect = { 0, 0, 0, 0 };
	dxFont->DrawTextA( 0, text.c_str(), -1, &rect, DT_CALCRECT, 0 );
	return Size( static_cast< float >( rect.right ), static_cast< float >( rect.bottom ) );
}

void Font::DrawWithGlyphs( const Rect& rect, const std::string& text, Origin::TYPE alignment )
{
    DrawWithGlyphs( m_defaultVersion, rect, text, alignment );
}

void Font::DrawWithGlyphs( size_t version, const Rect& rect, const std::string& text, Origin::TYPE alignment )
{
    if ( version >= m_versions.size() )
    {
        throw std::exception( "Font version not found!" );
    }

    if ( text.empty() )
    {
        return;
    }
    
    ID3DXFont * dxFont = m_versions[ version ].GetDxFont();

    IDirect3DDevice9 & dxDevice = *m_displayActual.GetDxDevice();

    dxDevice.SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    dxDevice.SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
    dxDevice.SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
    dxDevice.SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
    dxDevice.SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

    dxDevice.SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    dxDevice.SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );    
    
    WORD* glyphBuffer = new WORD[ text.size() ];
    
    GetGlyphIndicesA( dxFont->GetDC(), text.c_str(), static_cast< int >( text.size() ), glyphBuffer, GGI_MARK_NONEXISTING_GLYPHS );

    std::map< unsigned int, GlyphData > glyphMap;
    Size textSize = GlyphDataFromGlyphs( version, glyphBuffer, text.size(), glyphMap );

    float startX = 0.0f;
    float startY = 0.0f;

    // Determine the starting position for writing based on the size and the origin
    // Not effective for multiline text
    switch( alignment )
	{
	case Origin::UL:
		startX = rect.ul.x;//DT_TOP | DT_LEFT;
        startY = rect.ul.y;
		break;
	case Origin::UR:
		startX = rect.dr.x - textSize.width;//DT_TOP | DT_RIGHT;
        startY = rect.ul.y;
		break;
	case Origin::DL:
		startX = rect.ul.x;//DT_BOTTOM | DT_LEFT;
        startY = rect.dr.y - textSize.height;
		break;
	case Origin::DR:
		startX = rect.dr.x - textSize.width;//DT_BOTTOM | DT_RIGHT;
        startY = rect.dr.y - textSize.height;
		break;
	case Origin::Top:
        startX = rect.ul.x + rect.GetSize().width / 2 - textSize.width / 2;//DT_TOP | DT_CENTER;
        startY = rect.ul.y;
		break;
	case Origin::Right:
		startX = rect.dr.x - textSize.width;//DT_RIGHT | DT_VCENTER;
        startY = rect.ul.y + rect.GetSize().height / 2 - textSize.height / 2;
		break;
	case Origin::Bottom:
		startX = rect.ul.x + rect.GetSize().width / 2 - textSize.width / 2;//DT_BOTTOM | DT_CENTER;
        startY = rect.dr.y - textSize.height;
		break;
	case Origin::Left:
		startX = rect.ul.x;//DT_LEFT | DT_VCENTER;
        startY = rect.ul.y + rect.GetSize().height / 2 - textSize.height / 2;
		break;
	case Origin::Center:
		// Fall through...
	default:
		startX = rect.ul.x + rect.GetSize().width / 2 - textSize.width / 2;//DT_CENTER | DT_VCENTER;
        startY = rect.ul.y + rect.GetSize().height / 2 - textSize.height / 2;
		break;
	}

    float iterativeTextWidth = 0;

    for ( size_t i = 0, size = text.size(); i < size; ++i )    
    {
        HRESULT hr = S_OK;

        unsigned int glyph = glyphBuffer[ i ];
        GlyphData currentGlyph = glyphMap[ glyph ];

        IDirect3DTexture9* texture = currentGlyph.texture;
        RECT glyphDimensions = currentGlyph.glyphDimensions;
        POINT vectorToNextCharacter = currentGlyph.vectorToNextCharacter;                

        float glyphWidth = static_cast< float >( glyphDimensions.right - glyphDimensions.left );
        float glyphHeight = static_cast< float >( glyphDimensions.bottom - glyphDimensions.top );

        if ( texture )
        {
            D3DSURFACE_DESC description;
            hr = texture->GetLevelDesc( 0, &description );
            assert( SUCCEEDED( hr ) );

            float textureWidth = static_cast< float >( description.Width );
            float textureHeight = static_cast< float>( description.Height );                

            Rect textureCoord( V2( glyphDimensions.left / textureWidth, glyphDimensions.top / textureHeight  ), 
                               V2( glyphDimensions.right / textureWidth, glyphDimensions.bottom / textureHeight ) );

            V2 pos( startX + iterativeTextWidth + vectorToNextCharacter.x , startY + vectorToNextCharacter.y );
            
            media::vertex::Textured vb[ 4 ] =
            {
                media::vertex::Textured( pos, 1, textureCoord.ul.x, textureCoord.ul.y ),
                media::vertex::Textured( pos + V2( glyphWidth, 0 ), 1, 0, textureCoord.dr.x, textureCoord.ul.y ),
                media::vertex::Textured( pos + V2( 0, glyphHeight ), 1, 0, textureCoord.ul.x, textureCoord.dr.y ),
                media::vertex::Textured( pos + V2( glyphWidth, glyphHeight ), 1, 0, textureCoord.dr.x, textureCoord.dr.y )
            };

            
	        hr = m_displayActual.GetDxDevice()->SetTexture( 0, texture );
	        assert( SUCCEEDED( hr ) );

	        hr = dxDevice.SetFVF( media::vertex::Textured::FVF );
	        assert( SUCCEEDED( hr ) );

	        hr = dxDevice.DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, (void*)&vb, sizeof( media::vertex::Textured ) );
	        assert( SUCCEEDED( hr ) );
        }

        iterativeTextWidth += glyphWidth + vectorToNextCharacter.x;
    }       

    delete [] glyphBuffer;

    dxDevice.SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
    dxDevice.SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
    dxDevice.SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
    dxDevice.SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
    dxDevice.SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ZERO );

    dxDevice.SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
    dxDevice.SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );   
}

void Font::DrawWithD3DXCreateText( const Rect& rect, const std::string& text )
{
    DrawWithD3DXCreateText( m_defaultVersion, rect, text );
}

void Font::DrawWithD3DXCreateText( size_t version, const Rect & rect, const std::string & text )
{
    rect;

    if ( version >= m_versions.size() )
    {
        throw std::exception( "Font version not found!" );
    }

    if ( text.empty() )
    {
        return;
    }
    
    ID3DXFont * dxFont = m_versions[ version ].GetDxFont();

    HRESULT hr = S_OK;
    ID3DXMesh* mesh = 0;

    IDirect3DDevice9 & dxDevice = *m_displayActual.GetDxDevice();

    std::map< D3DRENDERSTATETYPE, DWORD > previousRenderStates;

    // Set new render states    
    SetRenderStateAndStorePrevious( D3DRS_CLIPPLANEENABLE, 0L, previousRenderStates );
    SetRenderStateAndStorePrevious( D3DRS_VERTEXBLEND, D3DVBF_DISABLE, previousRenderStates );
    SetRenderStateAndStorePrevious( D3DRS_FILLMODE, D3DFILL_SOLID, previousRenderStates );
    SetRenderStateAndStorePrevious( D3DRS_SHADEMODE, D3DSHADE_GOURAUD, previousRenderStates );
    SetRenderStateAndStorePrevious( D3DRS_CULLMODE, D3DCULL_NONE, previousRenderStates );
    SetRenderStateAndStorePrevious( D3DRS_FOGENABLE, FALSE, previousRenderStates );
    SetRenderStateAndStorePrevious( D3DRS_SPECULARENABLE, TRUE, previousRenderStates );
    SetRenderStateAndStorePrevious( D3DRS_LIGHTING, TRUE, previousRenderStates );
    SetRenderStateAndStorePrevious( D3DRS_AMBIENT, D3DCOLOR_XRGB(255, 255, 255), previousRenderStates );
    SetRenderStateAndStorePrevious( D3DRS_COLORVERTEX, FALSE, previousRenderStates );
    SetRenderStateAndStorePrevious( D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL, previousRenderStates );
    SetRenderStateAndStorePrevious( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL, previousRenderStates );
    SetRenderStateAndStorePrevious( D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL, previousRenderStates );
    SetRenderStateAndStorePrevious( D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL, previousRenderStates );
    SetRenderStateAndStorePrevious( D3DRS_ALPHATESTENABLE, FALSE, previousRenderStates );
    SetRenderStateAndStorePrevious( D3DRS_ZENABLE, D3DZB_TRUE, previousRenderStates );
    SetRenderStateAndStorePrevious( D3DRS_ZFUNC, D3DCMP_LESS, previousRenderStates );
    //SetRenderStateAndStorePrevious( D3DRS_ZBIAS, 0, previousRenderStates );
    SetRenderStateAndStorePrevious( D3DRS_STENCILENABLE, FALSE, previousRenderStates );
    SetRenderStateAndStorePrevious( D3DRS_ALPHABLENDENABLE, FALSE, previousRenderStates );
    SetRenderStateAndStorePrevious( D3DRS_COLORWRITEENABLE, 0xF, previousRenderStates );
    SetRenderStateAndStorePrevious( D3DRS_DITHERENABLE, TRUE, previousRenderStates );

    hr = D3DXCreateTextA( &dxDevice, dxFont->GetDC(), text.c_str(), 0.001f, 0.4f, &mesh, 0, 0 );
    assert( SUCCEEDED( hr ) );

    D3DMATERIAL9 mtrl;

    ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );

    mtrl.Diffuse.r = mtrl.Ambient.r = 0.75f;
    mtrl.Diffuse.g = mtrl.Ambient.g = 0.0f;
    mtrl.Diffuse.b = mtrl.Ambient.b = 0.75f;
    mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;

    hr = dxDevice.SetMaterial( &mtrl );    
    assert( SUCCEEDED( hr ) );

    D3DXMATRIX prevMatrixView, prevMatrixWorld, prevMatrixProjection;
    dxDevice.GetTransform( D3DTS_VIEW, &prevMatrixView ); 
    dxDevice.GetTransform( D3DTS_WORLD, &prevMatrixWorld );
    dxDevice.GetTransform( D3DTS_PROJECTION, &prevMatrixProjection );

    // figure out the bounding box of the mesh to adjust the projection
    // and mesh position.
    D3DXVECTOR3 min, max;
    
    D3DXVECTOR3* data = 0;
    
    hr = mesh->LockVertexBuffer( 0, reinterpret_cast<LPVOID *>(&data));
    assert( SUCCEEDED( hr ) );

    DWORD numVertices = mesh->GetNumVertices();
    DWORD dwStride = mesh->GetFVF();
    hr = D3DXComputeBoundingBox( data, numVertices, dwStride, &min, &max );
    assert( SUCCEEDED( hr ) );

    hr = mesh->UnlockVertexBuffer(); 
    assert( SUCCEEDED( hr ) );

    D3DXMATRIX scaling;
    D3DXMATRIX rotation;
    D3DXMATRIX translate;

    D3DXMatrixScaling( &scaling, 10.0f, 10.0f, 1.0f );
    D3DXMatrixRotationX( &rotation, D3DX_PI );
    D3DXMatrixTranslation( &translate, 0, 100, 0.0f );
    // This will need to me set ONCE ever upon creation to tweak the font into a "normal" ready state.
    m_tweak = scaling * rotation * translate;


    dxDevice.SetTransform( D3DTS_WORLD, &m_tweak );

    hr = mesh->DrawSubset( 0 );
    assert( SUCCEEDED( hr ) );

    mesh->Release(); 

    dxDevice.SetTransform( D3DTS_PROJECTION, &prevMatrixProjection );
    dxDevice.SetTransform( D3DTS_VIEW, &prevMatrixView );
    dxDevice.SetTransform( D3DTS_WORLD, &prevMatrixWorld );    

    // Restore the old states
    dxDevice.LightEnable( 0, FALSE );

    std::map< D3DRENDERSTATETYPE, DWORD >::iterator iter;
    std::map< D3DRENDERSTATETYPE, DWORD >::const_iterator end;
    for ( iter = previousRenderStates.begin(), end = previousRenderStates.end();
          iter != end; ++iter )
    {
        dxDevice.SetRenderState( (*iter).first, (*iter).second );
    }
}

void Font::SetRenderStateAndStorePrevious( const D3DRENDERSTATETYPE renderState, const DWORD value, std::map< D3DRENDERSTATETYPE, DWORD > &previousRenderStates )
{
    IDirect3DDevice9 & dxDevice = *m_displayActual.GetDxDevice();
    
    DWORD previousRenderState;
    dxDevice.GetRenderState( renderState, &previousRenderState );

    dxDevice.SetRenderState( renderState, value );

    previousRenderStates[ renderState ] = previousRenderState;
}

bool Font::HasVersion( unsigned int size, bool italic, bool bold )
{
    for ( size_t i = 0, end = m_versions.size(); i != end; ++i )
    {
        if ( m_versions[ i ].Match( size, italic, bold ) )
        {
            return true;
        }
    }

    return false;
}

size_t Font::FindVersion( unsigned int size, bool italic, bool bold )
{
    for ( size_t i = 0, end = m_versions.size(); i != end; ++i )
    {
        if ( m_versions[ i ].Match( size, italic, bold ) )
        {
            return i;
        }
    }

    throw std::exception( "Font version not found!" );
}

Size Font::GlyphDataFromGlyphs( size_t version, const WORD* const glyphIdentifiers, size_t numberOfGlyphIdentifiers, std::map< unsigned int, GlyphData >& glyphMap )
{
    if ( version >= m_versions.size() )
    {
        throw std::exception( "Font version not found!" );
    }
    ID3DXFont * dxFont = m_versions[ version ].GetDxFont();

    float totalWidth = 0.0f;
    float totalHeight = 0.0f;
        
    HRESULT hr = S_OK;

    TEXTMETRICA textMetrics;
    dxFont->GetTextMetricsA( &textMetrics );
    long emptyCharWidth = textMetrics.tmAveCharWidth;
    
    for ( size_t i = 0, size = numberOfGlyphIdentifiers; i < size; ++i )
    {
        GlyphData currentGlyph;
        unsigned int glyph = glyphIdentifiers[ i ];
        
        if ( glyph == 0xffff )
        {
            // This is an unsupported glyph
            glyphMap[ glyph ] = currentGlyph;
            continue;
        }
        
        // Texture will not be used
        hr = dxFont->GetGlyphData( glyph, &currentGlyph.texture, &currentGlyph.glyphDimensions, &currentGlyph.vectorToNextCharacter );        
        assert( SUCCEEDED( hr ) );                 

        if ( 0 == currentGlyph.texture )
        {
            // This is a character that does not require a texture.
            // For example, a space.            
            currentGlyph.glyphDimensions.right = emptyCharWidth;            
        }

        totalWidth += currentGlyph.glyphDimensions.right - currentGlyph.glyphDimensions.left + currentGlyph.vectorToNextCharacter.x;
        
        // assume it is one line for calculating total height
        float height = static_cast< float >( currentGlyph.glyphDimensions.bottom - currentGlyph.glyphDimensions.top + currentGlyph.vectorToNextCharacter.y );

        if ( height > totalHeight )
        {
            totalHeight = height;
        }

        glyphMap[ glyph ] = currentGlyph;
    }

    return Size( totalWidth, totalHeight );
}
