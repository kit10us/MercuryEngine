#include <TesterBase.h>
#include <Font.h>
#include <Draw2D.h>

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
	//CreatePrimaryDisplay( DisplayDetails::CreateFullscreenDirectXDisplay( Size( 1280, 800 ) ) );
	CreatePrimaryDisplay( DisplayDetails::CreateWindowedDirectXDisplay( Size( 800, 600 ), V2( 90, 10 ) ) );
	
}

void Tester::Startup()
{
	m_font.AddVersion( 24, false, false );
}

void Tester::Update( float delta, Input & input )
{
	delta; input; // NOT USED
}

void Tester::Render( IDirect3DDevice9 * dxDevice )
{
	dxDevice; // NOT USED
	
	media::ImageInstanceData imageInstanceData( D3DXVECTOR3( 0, 0, 1 ) );
	
	//BeginShaderDraw( false, imageInstanceData );

	Draw2D::DrawRectangle( Rect( 10, 10, 110, 110 ), D3DCOLOR_XRGB( 255, 0, 0 ), D3DCOLOR_XRGB( 0, 255, 0 ), D3DCOLOR_XRGB( 0, 0, 255 ), D3DCOLOR_XRGB( 255, 255, 255 ), 0 );

	Draw2D::DrawRectangle( Rect( 10, 10, 110, 110 ) + V2( 110, 0 ), D3DCOLOR_XRGB( 255, 0, 0 ), D3DCOLOR_XRGB( 0, 255, 0 ), D3DCOLOR_XRGB( 0, 0, 255 ), D3DCOLOR_XRGB( 255, 255, 255 ), 10 );

	Draw2D::DrawRectangle( Rect( 10, 10, 110, 110 ) + V2( 110, 0 ) * 2, D3DCOLOR_XRGB( 155, 55, 255) );

	Draw2D::DrawRectangle( Rect( 10, 10, 110, 110 ) + V2( 110, 0 ) * 3, D3DCOLOR_XRGB( 255, 0, 0 ), D3DCOLOR_XRGB( 0, 255, 0 ), D3DCOLOR_XRGB( 0, 0, 255 ), D3DCOLOR_XRGB( 0, 150, 255 ), D3DCOLOR_XRGB( 255, 150, 150 ), D3DCOLOR_XRGB( 150, 255, 150 ), D3DCOLOR_XRGB( 150, 150, 255 ), D3DCOLOR_XRGB( 150, 255, 255 ), 0 );

	Draw2D::DrawRectangle( Rect( 10, 10, 110, 110 ) + V2( 110, 0 ) * 4, D3DCOLOR_XRGB( 255, 0, 0 ), D3DCOLOR_XRGB( 0, 255, 0 ), D3DCOLOR_XRGB( 0, 0, 255 ), D3DCOLOR_XRGB( 0, 150, 255 ), D3DCOLOR_XRGB( 255, 150, 150 ), D3DCOLOR_XRGB( 150, 255, 150 ), D3DCOLOR_XRGB( 150, 150, 255 ), D3DCOLOR_XRGB( 150, 255, 255 ), 10 );

	Draw2D::DrawLine( V2( 10, 40 ) + V2( 0, 110 ), V2( 110, 70 ) + V2( 0, 110 ), D3DCOLOR_XRGB( 0, 255, 0 ), D3DCOLOR_XRGB( 255, 0, 0 ), 30 );

	Draw2D::DrawLine( V2( 110, 10) + V2( 0, 110 ) * 2, V2( 10, 110 ) + V2( 0, 110 ) * 2, D3DCOLOR_XRGB( 255, 0, 255 ) , 10 );

    //EndShaderDraw();	
}
 
void Tester::Shutdown()
{
    m_font.Destroy();
}
