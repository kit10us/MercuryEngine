#include <TesterBase.h>
#include <Texture.h>
#include <assert.h>
#include <Font.h>


class Tester : public TesterBase
{
public:
	void BeforeStartup( const std::string & commandLine );
	void Startup();
	void Update( float delta, Input & input );
	void Render( IDirect3DDevice9 * dxDevice );
	void Shutdown();

protected:
    //Font m_font;
} tester;


void Tester::BeforeStartup( const std::string & commandLine )
{
	commandLine; // Not used

    CreatePrimaryDisplay( DisplayDetails::CreateFullscreenDirectXDisplay( Size( 1280, 800 ) ) );
}

void Tester::Startup()
{
    //assert( m_font.AddVersion( 600, false, false ) );
}

void Tester::Update( float delta, Input & input )
{
    delta; input;
}

void Tester::Render( IDirect3DDevice9 * dxDevice )
{
	dxDevice; // Not used
    //m_font.Draw( Rect( V2( 0, 0 ), V2( 1280, 800 ) ), "1" );
}

void Tester::Shutdown()
{
}
