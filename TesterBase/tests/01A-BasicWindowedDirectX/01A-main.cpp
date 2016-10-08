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

    AddDisplay( DisplayDetails::CreateWindowedDirectXDisplay( Size( 800, 600 ), V2( 10, 10 ) ) );
}

void Tester::Startup()
{
    //assert( m_font.AddVersion( 500, false, false ) );
}

void Tester::Update( float delta, Input & input )
{
    delta; input;
}

void Tester::Render( IDirect3DDevice9 * dxDevice )
{
	dxDevice; // Not used
    //m_font.Draw( Rect( V2( 0, 0 ), V2( 800, 600 ) ), "1" );
}

void Tester::Shutdown()
{
    //m_font.Destroy();
}
