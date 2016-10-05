#include <TesterBase.h>
#include <Texture.h>
#include <Font.h>
#include <gui/Text.h>
#include <gui/Button.h>
#include <gui/Picture.h>
#include <gui/CycleButton.h>
#include <gui/ThingGroup.h>
#include <assert.h>


class Tester : public TesterBase
{
public:
	void BeforeStartup( const std::string & commandLine );
	void Startup();
	void ResetTextureSettings();
	void RecreateTexture();
	void Update( float delta, Input & input );
	void Render( IDirect3DDevice9 * dxDevice );
	void Shutdown();

protected:
	Font m_font;
	Texture m_texture;
	gui::CycleButton m_MAGButton;
	gui::CycleButton m_MINButton;
	gui::Picture m_original;
	gui::Picture m_slightlySmaller;
	gui::Picture m_slightlyLarger;
	gui::Picture m_doubleSized;

	gui::ThingGroup m_textureThings;
} tester;


void Tester::BeforeStartup( const std::string & commandLine )
{
	commandLine; // NOT USED
	CreatePrimaryDisplay( DisplayDetails::CreateWindowedDirectXDisplay( Size( 800, 600 ), V2( 1290, 10 ) ) );
}

struct RecreateTextureFunctor : public gui::OnEventFunctor
{
	Tester & m_tester;
	RecreateTextureFunctor( Tester & tester )
		: m_tester( tester )
	{
	}

	// Add this function to remove warning C4512: '...' : assignment operator could not be generated 
	RecreateTextureFunctor & operator=( const RecreateTextureFunctor & functor )
	{
		functor; // Not used
		throw std::exception( "WTF!?" );
		return *this;
	}

	void operator()( gui::Thing & thing )
	{
		thing; // Not used
		m_tester.RecreateTexture();
	}
};

struct ResetTextureSettingsFunctor : public gui::OnEventFunctor
{
	Tester & m_tester;
	ResetTextureSettingsFunctor( Tester & tester )
		: m_tester( tester )
	{
	}

	// Add this function to remove warning C4512: '...' : assignment operator could not be generated 
	ResetTextureSettingsFunctor & operator=( const ResetTextureSettingsFunctor & functor )
	{
		functor; // Not used
		throw std::exception( "WTF!?" );
		return *this;
	}

	void operator()( gui::Thing & thing )
	{
		thing; // Not used
		m_tester.ResetTextureSettings();
	}
};

void Tester::ResetTextureSettings()
{
	m_MINButton.SetCurrentChoice( 0 );
	
	m_MAGButton.SetCurrentChoice( 0 );

	/*
	gui::CycleButton * powerOf2Button = dynamic_cast< gui::CycleButton * >( m_textureThings.FindThing( "Power of 2" ) );
	assert( powerOf2Button );
	powerOf2Button->SetCurrentChoice( false );

	gui::CycleButton * mipLevelsButton = dynamic_cast< gui::CycleButton * >( m_textureThings.FindThing( "Mip Levels" ) );
	assert( mipLevelsButton );
	mipLevelsButton->SetCurrentChoice( Texture::MipLevelsFromFile );
	*/

	gui::CycleButton * texture_filter = dynamic_cast< gui::CycleButton * >( m_textureThings.FindThing( "Filter" ) );
	assert( texture_filter );
	texture_filter->SetCurrentChoice( 3 );

	gui::CycleButton * texture_filterDither = dynamic_cast< gui::CycleButton * >( m_textureThings.FindThing( "FilterDither" ) );
	assert( texture_filterDither );
	texture_filterDither->SetCurrentChoice( 1 );

	gui::CycleButton * texture_filterMiror = dynamic_cast< gui::CycleButton * >( m_textureThings.FindThing( "FilterMirror" ) );
	assert( texture_filterMiror );
	texture_filterMiror->SetCurrentChoice( 0 );
	
	RecreateTexture();
}

void Tester::RecreateTexture()
{
	IDirect3DDevice9 & dxDevice = *GetDxDevice();
	
	std::string minString = m_MINButton.GetCurrentText();
	if ( _stricmp( minString.c_str(), "None" ) == 0 )
	{
		dxDevice.SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_NONE );
	}
	else if ( _stricmp( minString.c_str(), "Point" ) == 0 )
	{
		dxDevice.SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
	}
	else if ( _stricmp( minString.c_str(), "Linear" ) == 0 )
	{
		dxDevice.SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	}
	else if ( _stricmp( minString.c_str(), "Anisotropic" ) == 0 )
	{
		dxDevice.SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC );
	}
	else if ( _stricmp( minString.c_str(), "Pyramidal Quad" ) == 0 )
	{
		dxDevice.SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_PYRAMIDALQUAD );
	}
	else if ( _stricmp( minString.c_str(), "Guassian Quad" ) == 0 )
	{
		dxDevice.SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_GAUSSIANQUAD );
	}
	else
	{
		throw std::exception( "Invalid filter selected" );
	}

	std::string magString = m_MAGButton.GetCurrentText();
	if ( _stricmp( magString.c_str(), "None" ) == 0 )
	{
		dxDevice.SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_NONE );
	}
	else if ( _stricmp( magString.c_str(), "Point" ) == 0 )
	{
		dxDevice.SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
	}
	else if ( _stricmp( magString.c_str(), "Linear" ) == 0 )
	{
		dxDevice.SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	}
	else if ( _stricmp( magString.c_str(), "Anisotropic" ) == 0 )
	{
		dxDevice.SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC );
	}
	else if ( _stricmp( magString.c_str(), "Pyramidal Quad" ) == 0 )
	{
		dxDevice.SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_PYRAMIDALQUAD );
	}
	else if ( _stricmp( magString.c_str(), "Guassian Quad" ) == 0 )
	{
		dxDevice.SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_GAUSSIANQUAD );
	}
	else
	{
		throw std::exception( "Invalid filter selected" );
	}



	gui::CycleButton * powerOf2Button = dynamic_cast< gui::CycleButton * >( m_textureThings.FindThing( "Power of 2" ) );
	assert( powerOf2Button );
	bool powerOf2 = powerOf2Button->GetCurrentChoice() == 0 ? false : true;

	gui::CycleButton * mipLevelsButton = dynamic_cast< gui::CycleButton * >( m_textureThings.FindThing( "Mip Levels" ) );
	assert( mipLevelsButton );
	Texture::MipLevels mipLevels = mipLevelsButton->GetCurrentChoice() == 0 ? Texture::MipLevelsFromFile : Texture::MipLevelsAll;

	gui::CycleButton * texture_filter = dynamic_cast< gui::CycleButton * >( m_textureThings.FindThing( "Filter" ) );
	assert( texture_filter );
	Texture::Filter filter = static_cast< Texture::Filter >( texture_filter->GetCurrentChoice() );

	gui::CycleButton * texture_filterDither = dynamic_cast< gui::CycleButton * >( m_textureThings.FindThing( "FilterDither" ) );
	assert( texture_filterDither );
	bool dither = texture_filterDither->GetCurrentChoice() ? true : false;

	gui::CycleButton * texture_filterMiror = dynamic_cast< gui::CycleButton * >( m_textureThings.FindThing( "FilterMirror" ) );
	assert( texture_filterMiror );
	bool mirror = texture_filterMiror->GetCurrentChoice() ? true : false;

	m_texture.Load( "odd_checker_01.png", powerOf2, mipLevels, filter, mirror, dither );
}


void Tester::Startup()
{
	m_font.AddVersion( 24, false, false );

	m_MAGButton.Create( V2( 5, 5 ), Size( 160, 30 ), &m_font );
	m_MAGButton.AddChoice( "None" );
	m_MAGButton.AddChoice( "Point" );
	m_MAGButton.AddChoice( "Linear" );
	m_MAGButton.AddChoice( "Anisotropic" );
	m_MAGButton.AddChoice( "Pyramidal Quad" );
	m_MAGButton.AddChoice( "Guassian Quad" );
	m_MAGButton.SetOnChange( new RecreateTextureFunctor( *this ) );

	m_MINButton.Create( V2( 0, 0 ), Size( 160, 30 ), &m_font );
	m_MINButton.AddChoice( "None" );
	m_MINButton.AddChoice( "Point" );
	m_MINButton.AddChoice( "Linear" );
	m_MINButton.AddChoice( "Anisotropic" );
	m_MINButton.AddChoice( "Pyramidal Quad" );
	m_MINButton.AddChoice( "Guassian Quad" );
	PositionFrom( m_MINButton, m_MAGButton, Origin::Bottom, V2( 0, 5 ) );
	m_MINButton.SetOnChange( new RecreateTextureFunctor( *this ) );


	// Texture creation settings...

	gui::Text * textureText = new gui::Text;
	textureText->Create( V2( 0, 0 ), "Texture settings...", &m_font );
	PositionFrom( *textureText, m_MINButton, Origin::DL, V2( 0, 5 + m_MINButton.GetSize().height ) );
	m_textureThings.AddThing( textureText );

	// Power of 2...
	gui::Text * texture_powerOf2Text = new gui::Text;
	texture_powerOf2Text->Create( V2( 0, 0 ), "Power of 2:", &m_font );
	PositionFrom( *texture_powerOf2Text, *textureText, Origin::DL, V2( 0, 10 ) );
	m_textureThings.AddThing( texture_powerOf2Text );

	gui::CycleButton * texture_powerOf2 = new gui::CycleButton;
	texture_powerOf2->Create( V2( 0, 0 ), Size( 160, 30 ), &m_font );
	PositionFrom( *texture_powerOf2, *texture_powerOf2Text, Origin::DL, V2( 10, 5 ) );
	texture_powerOf2->AddChoice( "False" );
	texture_powerOf2->AddChoice( "True" );
	texture_powerOf2->SetCurrentChoice( 0 );
	texture_powerOf2->SetOnChange( new RecreateTextureFunctor( *this ) );
	m_textureThings.AddThing( texture_powerOf2, "Power of 2" );


	// Mip levels...
	gui::Text * texture_mipLevelsText = new gui::Text;
	texture_mipLevelsText->Create( V2( 0, 0 ), "Mip Levels:", &m_font );
	PositionFrom( *texture_mipLevelsText, *texture_powerOf2, Origin::DL, V2( 0, 5 ) );
	m_textureThings.AddThing( texture_mipLevelsText );

	gui::CycleButton * texture_mipLevels = new gui::CycleButton;
	texture_mipLevels->Create( V2( 0, 0 ), Size( 160, 30 ), &m_font );
	PositionFrom( *texture_mipLevels, *texture_mipLevelsText, Origin::DL, V2( 10, 5 ) );
	texture_mipLevels->AddChoice( "From File" );
	texture_mipLevels->AddChoice( "All" );
	texture_mipLevels->SetCurrentChoice( 0 );
	texture_mipLevels->SetOnChange( new RecreateTextureFunctor( *this ) );
	m_textureThings.AddThing( texture_mipLevels, "Mip Levels" );


	// Filtering...
	gui::Text * texture_filterText = new gui::Text;
	texture_filterText->Create( V2( 0, 0 ), "Filter:", &m_font );
	PositionFrom( *texture_filterText, *texture_mipLevels, Origin::DL, V2( 0, 5 ) );
	m_textureThings.AddThing( texture_filterText );

	gui::Button * texture_filterResetButton = new gui::Button;
	texture_filterResetButton->Create( V2( 0, 0 ), Size( 160, 30 ), "Reset", &m_font );
	texture_filterResetButton->SetOnPress( new ResetTextureSettingsFunctor( *this ) );
	PositionFrom( *texture_filterResetButton, *texture_filterText, Origin::DL, V2( 0, 5 ) );
	m_textureThings.AddThing( texture_filterResetButton );

	gui::CycleButton * texture_filter = new gui::CycleButton;
	texture_filter->Create( V2( 0, 0 ), Size( 160, 30 ), &m_font );
	PositionFrom( *texture_filter, *texture_filterResetButton, Origin::DL, V2( 0, 5 ) );
	texture_filter->AddChoice( "None" );
	texture_filter->AddChoice( "Point" );
	texture_filter->AddChoice( "Linear" );
	texture_filter->AddChoice( "Triangle" );
	texture_filter->AddChoice( "Box" );
	texture_filter->SetCurrentChoice( 3 );
	texture_filter->SetOnChange( new RecreateTextureFunctor( *this ) );
	m_textureThings.AddThing( texture_filter, "Filter" );

	gui::CycleButton * texture_filterDither = new gui::CycleButton;
	texture_filterDither->Create( V2( 0, 0 ), Size( 160, 30 ), &m_font );
	PositionFrom( *texture_filterDither, *texture_filter, Origin::DL, V2( 0, 5 ) );
	texture_filterDither->AddChoice( "Dither Off" );
	texture_filterDither->AddChoice( "Dither On" );
	texture_filterDither->SetCurrentChoice( 1 );
	texture_filterDither->SetOnChange( new RecreateTextureFunctor( *this ) );
	m_textureThings.AddThing( texture_filterDither, "FilterDither" );

	gui::CycleButton * texture_filterMirror = new gui::CycleButton;
	texture_filterMirror->Create( V2( 0, 0 ), Size( 160, 30 ), &m_font );
	PositionFrom( *texture_filterMirror, *texture_filterDither, Origin::DL, V2( 0, 5 ) );
	texture_filterMirror->AddChoice( "Mirror Off" );
	texture_filterMirror->AddChoice( "Mirror On" );
	texture_filterMirror->SetCurrentChoice( 0 );
	texture_filterMirror->SetOnChange( new RecreateTextureFunctor( *this ) );
	m_textureThings.AddThing( texture_filterMirror, "FilterMirror" );

	// Create texture last as it pulls values from Things.
	ResetTextureSettings();
	GetDxDevice()->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_NONE );

	m_original.Create( V2( 10, 10 ), &m_texture );
	PositionFrom( m_original, m_MAGButton, Origin::UR, V2( 40, 0 ) );
	
	m_slightlySmaller.Create( V2( 0, 0 ), &m_texture );
	PositionFrom( m_slightlySmaller, m_original, Origin::Bottom, V2( 0, 10 ) );
	m_slightlySmaller.AdjustSize( Size( -1, -1 ) );

	m_slightlyLarger.Create( V2( 0, 0 ), &m_texture );
	PositionFrom( m_slightlyLarger, m_slightlySmaller, Origin::Bottom, V2( 0, 10 ) );
	m_slightlySmaller.AdjustSize( Size( 1, 1 ) );

	m_doubleSized.Create( V2( 0, 0 ), &m_texture );
	PositionFrom( m_doubleSized, m_original, Origin::UR, V2( 10, 0 ) );
	m_doubleSized.AdjustSize( m_doubleSized.GetSize() );
}

void Tester::Update( float delta, Input & input )
{
	m_original.Update( delta, input );
	m_slightlySmaller.Update( delta, input );
	m_slightlyLarger.Update( delta, input );
	m_doubleSized.Update( delta, input );
	m_MAGButton.Update( delta, input );
	m_MINButton.Update( delta, input );
	
	m_textureThings.Update( delta, input );
}

void Tester::Render( IDirect3DDevice9 * dxDevice )
{
	dxDevice; // Not used

	m_original.Draw();
	m_slightlySmaller.Draw();
	m_slightlyLarger.Draw();
	m_doubleSized.Draw();
	m_MAGButton.Draw();
	m_MINButton.Draw();
	
	m_textureThings.Draw();
}

void Tester::Shutdown()
{
	m_texture.Destroy();
}
