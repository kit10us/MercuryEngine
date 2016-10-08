#include <TesterBase.h>
#include <Texture.h>
#include <assert.h>
#include <Font.h>
#include <memory>

class Tester : public TesterBase
{
public:
	void BeforeStartup( const std::string & commandLine );
	void Startup();
	void Update( float delta, Input & input );
	void Render( IDirect3DDevice9 * dxDevice, int displayIndex );
	void Shutdown();

protected:
    std::shared_ptr< Font > m_font1;
	std::shared_ptr< Font > m_font2;
} tester;


void Tester::BeforeStartup( const std::string & commandLine )
{
	commandLine; // Not used

    AddDisplay( DisplayDetails::CreateWindowedDirectXDisplay( Size( 800, 600 ), V2( 10, 10 ) ) );
	AddDisplay( DisplayDetails::CreateWindowedDirectXDisplay( Size( 800, 600 ), V2( 810, 10 ) ) );
}

void Tester::Startup()
{
	m_font1.reset( new Font( GetDisplay( 0 ).second ) );
    assert( m_font1->AddVersion( 500, false, false ) );

	m_font2.reset( new Font( GetDisplay( 0 ).second ) );
	assert( m_font2->AddVersion( 500, false, false ) );
}

void Tester::Update( float delta, Input & input )
{
    delta; input;
}

void Tester::Render( IDirect3DDevice9 * dxDevice, int displayIndex )
{
	dxDevice; // Not used
	if ( displayIndex == 1 )
	{
		m_font1->Draw( Rect( V2( 0, 0 ), V2( 800, 600 ) ), "1" );
	}
	else
	{
		m_font2->Draw( Rect( V2( 0, 0 ), V2( 800, 600 ) ), "2" );
	}
}

void Tester::Shutdown()
{
	m_font1.reset();
	m_font2.reset();
}
