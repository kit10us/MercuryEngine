#include <TesterBase.h>
#include <Font.h>
#include <Draw2D.h>
#include <assert.h>

class Tester : public TesterBase
{
public:
	void BeforeStartup( const std::string & commandLine );
	void Startup();
	void Update( float delta, Input & input );
	void Render( IDirect3DDevice9 * dxDevice );
	void Shutdown();

protected:
	Font m_font;
} tester;


void Tester::BeforeStartup( const std::string & commandLine )
{
	commandLine; // NOT USED
    SetHardwareAcceleration( true );
    CreatePrimaryDisplay( DisplayDetails::CreateWindowedDirectXDisplay( Size( 800, 600 ), V2( 90, 10 ) ) );
	//CreatePrimaryDisplay( DisplayDetails::CreateFullscreenDirectXDisplay( Size( 800, 600 ) ) );
}

void Tester::Startup()
{
	assert( m_font.AddVersion( 24, false, false ) );
	assert( m_font.AddVersion( 24, true, false ) );
	assert( m_font.AddVersion( 24, false, true ) );
	assert( m_font.AddVersion( 24, true, true ) );
	assert( m_font.AddVersion( 25, false, false ) );
	assert( m_font.AddVersion( 26, false, false ) );
	assert( m_font.AddVersion( 27, false, false ) );
	assert( m_font.AddVersion( 28, false, false ) );
	assert( m_font.AddVersion( 10, false, false ) );
}

void Tester::Update( float delta, Input & input )
{
    delta; input;
}

void Tester::Render( IDirect3DDevice9 * dxDevice )
{
    dxDevice;

	Rect rect( 10, 10, 360, 100 );
	Draw2D::DrawRectangle( rect, D3DCOLOR_XRGB( 255, 0, 0 ), D3DCOLOR_XRGB( 0, 255, 0 ), D3DCOLOR_XRGB( 0, 0, 255 ), D3DCOLOR_XRGB( 255, 255, 255 ), 4 );
	
	m_font.Draw( rect.Center(), "C", D3DCOLOR_XRGB( 255, 255, 255 ), Origin::Center );
	m_font.Draw( rect.Center(), "UL", D3DCOLOR_XRGB( 255, 255, 255 ), Origin::UL );
	m_font.Draw( rect.Center(), "T", D3DCOLOR_XRGB( 255, 255, 255 ), Origin::Top );
	m_font.Draw( rect.Center(), "UR", D3DCOLOR_XRGB( 255, 255, 255 ), Origin::UR );
	m_font.Draw( rect.Center(), "R", D3DCOLOR_XRGB( 255, 255, 255 ), Origin::Right );
	m_font.Draw( rect.Center(), "BR", D3DCOLOR_XRGB( 255, 255, 255 ), Origin::DR );
	m_font.Draw( rect.Center(), "B", D3DCOLOR_XRGB( 255, 255, 255 ), Origin::Bottom );
	m_font.Draw( rect.Center(), "BL", D3DCOLOR_XRGB( 255, 255, 255 ), Origin::DL );
	m_font.Draw( rect.Center(), "L", D3DCOLOR_XRGB( 255, 255, 255 ), Origin::Left );

	rect = Rect( 10, 10 + 110, 360, 100 + 110 );
	Draw2D::DrawRectangle( rect, D3DCOLOR_XRGB( 255, 0, 0 ), D3DCOLOR_XRGB( 0, 255, 0 ), D3DCOLOR_XRGB( 0, 0, 255 ), D3DCOLOR_XRGB( 255, 255, 255 ), 4 );
	
	m_font.Draw( rect, "Center", D3DCOLOR_XRGB( 255, 255, 255 ), Origin::Center );
	m_font.Draw( rect, "Upper Left", D3DCOLOR_XRGB( 255, 255, 255 ), Origin::UL );
	m_font.Draw( rect, "Top", D3DCOLOR_XRGB( 255, 255, 255 ), Origin::Top );
	m_font.Draw( rect, "Upper Right", D3DCOLOR_XRGB( 255, 255, 255 ), Origin::UR );
	m_font.Draw( rect, "Right", D3DCOLOR_XRGB( 255, 255, 255 ), Origin::Right );
	m_font.Draw( rect, "Bottom Right", D3DCOLOR_XRGB( 255, 255, 255 ), Origin::DR );
	m_font.Draw( rect, "Bottom ", D3DCOLOR_XRGB( 255, 255, 255 ), Origin::Bottom );
	m_font.Draw( rect, "Bottom Left", D3DCOLOR_XRGB( 255, 255, 255 ), Origin::DL );
	m_font.Draw( rect, "Left", D3DCOLOR_XRGB( 255, 255, 255 ), Origin::Left );

	
    size_t version = 0;
    rect += V2( rect.GetSize().width + 10, 0 );
	Draw2D::DrawRectangle( rect, D3DCOLOR_XRGB( 255, 0, 0 ), D3DCOLOR_XRGB( 0, 255, 0 ), D3DCOLOR_XRGB( 0, 0, 255 ), D3DCOLOR_XRGB( 255, 255, 255 ), 4 );
	
    version = m_font.FindVersion( 24, false, false );
	m_font.Draw( version, rect, "24, BF, IF", D3DCOLOR_XRGB( 255, 255, 255 ), Origin::Center );
    version = m_font.FindVersion( 24, true, false );
	m_font.Draw( version, rect, "24, BT, IF", D3DCOLOR_XRGB( 255, 255, 255 ), Origin::UL );
    version = m_font.FindVersion( 24, false, true );
	m_font.Draw( version, rect, "24, BF, IT", D3DCOLOR_XRGB( 255, 255, 255 ), Origin::Top );
    version = m_font.FindVersion( 24, true, true );
	m_font.Draw( version, rect, "24, BT, IT", D3DCOLOR_XRGB( 255, 255, 255 ), Origin::UR );
    version = m_font.FindVersion( 25, false, false );
	m_font.Draw( version, rect, "25, BF, IF", D3DCOLOR_XRGB( 255, 255, 255 ), Origin::Right );
    version = m_font.FindVersion( 26, false, false );
	m_font.Draw( version, rect, "26, BF, IF", D3DCOLOR_XRGB( 255, 255, 255 ), Origin::DR );
    version = m_font.FindVersion( 27, false, false );
	m_font.Draw( version, rect, "27, BF, IF", D3DCOLOR_XRGB( 255, 255, 255 ), Origin::Bottom );
    version = m_font.FindVersion( 28, false, false );
	m_font.Draw( version, rect, "28, BF, IF", D3DCOLOR_XRGB( 255, 255, 255 ), Origin::DL );
    version = m_font.FindVersion( 10, false, false );
	m_font.Draw( version, rect, "10, BF, IF", D3DCOLOR_XRGB( 255, 255, 255 ), Origin::Left );


    rect = Rect( 10, 10 + 2 * 110, 360, 100 + 2 * 110 );
    Draw2D::DrawRectangle( rect, D3DCOLOR_XRGB( 255, 0, 0 ), D3DCOLOR_XRGB( 0, 255, 0 ), D3DCOLOR_XRGB( 0, 0, 255 ), D3DCOLOR_XRGB( 255, 255, 255 ), 4 );

    m_font.DrawWithGlyphs( rect, "Glyph Test", Origin::Center );
    m_font.DrawWithGlyphs( rect, "Glyph Test", Origin::UL );
    m_font.DrawWithGlyphs( rect, "Glyph Test", Origin::Top );
    m_font.DrawWithGlyphs( rect, "Glyph Test", Origin::UR );    
    m_font.DrawWithGlyphs( rect, "Glyph Test", Origin::Right );    
    m_font.DrawWithGlyphs( rect, "Glyph Test", Origin::DR );
    m_font.DrawWithGlyphs( rect, "Glyph Test", Origin::Bottom );
    m_font.DrawWithGlyphs( rect, "Glyph Test", Origin::DL );    
    m_font.DrawWithGlyphs( rect, "Glyph Test", Origin::Left );    

    rect = Rect( 10, 10 + 3 * 110, 360, 100 + 3 * 110 );
    Draw2D::DrawRectangle( rect, D3DCOLOR_XRGB( 255, 0, 0 ), D3DCOLOR_XRGB( 0, 255, 0 ), D3DCOLOR_XRGB( 0, 0, 255 ), D3DCOLOR_XRGB( 255, 255, 255 ), 4 );

    // Too slow: m_font.DrawWithD3DXCreateText( rect4 + V2( 10, 10 ), "D3DXCreateText Test" );

    m_font.DrawWithD3DXCreateText( rect + V2( 10, 10 ), "D3DXCreateText Test" );
}

void Tester::Shutdown()
{
    m_font.Destroy();
}
