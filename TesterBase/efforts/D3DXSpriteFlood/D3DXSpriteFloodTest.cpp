#include <TesterBase.h>
#include <Texture.h>
#include <Font.h>
#include <gui/Text.h>
#include <gui/Button.h>
#include <gui/Picture.h>
#include <gui/CycleButton.h>
#include <gui/ThingGroup.h>
#include <cassert>
#include <boost/lexical_cast.hpp>
#include <media/vertex/Textured.h>

struct SpriteInstance
{
	// Default constructor added to remove VS2005 warning C4510.
	SpriteInstance()
		: life( 1.0f )
		, position( D3DXVECTOR3() )
		, textureIndex( size_t() )
	{
	}

	SpriteInstance( V2 position, size_t textureIndex )
		: life( 1.0f )
		, position( position.x, position.y, 1.0f )
		, textureIndex( textureIndex )
	{
	}

	float life;
	D3DXVECTOR3 position;
	size_t textureIndex;
};

class Tester : public TesterBase
{
public:
	Tester();
	void BeforeStartup( const std::string & commandLine );
	void Startup();
	void ResetTextureSettings();
	void RecreateTexture();
	void Update( float delta, Input & input );
	void Render( IDirect3DDevice9 * dxDevice );
	void Shutdown();

protected:
	Font m_font;
	std::vector< Texture * > m_textures;
	Texture m_texture;
	ID3DXSprite * m_d3dxSprite;
	float m_birthClock;
	size_t m_textureIndex;
	std::list< SpriteInstance > m_sprites;
	size_t m_spriteMaxCap;

	const static float SpriteBirthRate;
	const static float SpriteLifetime;
} tester;

const float Tester::SpriteBirthRate = 0.25f;
const float Tester::SpriteLifetime = 3.0f;

Tester::Tester()
: m_birthClock( 0 )
, m_textureIndex( 0 )
, m_spriteMaxCap( 2000 )
{
}


void Tester::BeforeStartup( const std::string & commandLine )
{
	if ( ! commandLine.empty() )
	{
		m_spriteMaxCap = boost::lexical_cast< size_t, const std::string >( commandLine );
	}
	m_d3dxSprite = 0;
	//CreatePrimaryDisplay( DisplayDetails::CreateFullscreenDirectXDisplay( Size( 1280, 800 ) ) );
    CreatePrimaryDisplay( DisplayDetails::CreateWindowedDirectXDisplay( Size( 1280, 800 ), V2( 10, 10 ) ) );
}

void Tester::Startup()
{
	m_font.AddVersion( 24, false, false );
	HRESULT hr = D3DXCreateSprite( GetDxDevice(), &m_d3dxSprite );
	if( FAILED( hr ) )
	{
		throw std::exception( "Failed D3DXCreateSprite!" );
	}

	// Load textures...
	m_textures.push_back( new Texture( "assets/knight/knight_01.dds" ) );
	m_textures.push_back( new Texture( "assets/knight/knight_02.dds" ) );
	m_textures.push_back( new Texture( "assets/knight/knight_03.dds" ) );
	m_textures.push_back( new Texture( "assets/knight/knight_04.dds" ) );
	m_textures.push_back( new Texture( "assets/knight/knight_05.dds" ) );
	m_textures.push_back( new Texture( "assets/knight/knight_06.dds" ) );
	m_textures.push_back( new Texture( "assets/knight/knight_07.dds" ) );
	m_textures.push_back( new Texture( "assets/knight/knight_08.dds" ) );
	m_textures.push_back( new Texture( "assets/knight/knight_09.dds" ) );
	m_textures.push_back( new Texture( "assets/knight/knight_10.dds" ) );
	m_textures.push_back( new Texture( "assets/knight/knight_11.dds" ) );
	m_textures.push_back( new Texture( "assets/knight/knight_12.dds" ) );
	m_textures.push_back( new Texture( "assets/knight/knight_13.dds" ) );
	m_textures.push_back( new Texture( "assets/knight/knight_14.dds" ) );
	m_textures.push_back( new Texture( "assets/knight/knight_15.dds" ) );
	m_textures.push_back( new Texture( "assets/knight/knight_16.dds" ) );
	m_textures.push_back( new Texture( "assets/knight/knight_17.dds" ) );
	m_textures.push_back( new Texture( "assets/knight/knight_18.dds" ) );
	m_textures.push_back( new Texture( "assets/knight/knight_19.dds" ) );
	m_textures.push_back( new Texture( "assets/knight/knight_20.dds" ) );
	m_textures.push_back( new Texture( "assets/knight/knight_21.dds" ) );
	m_textures.push_back( new Texture( "assets/knight/knight_22.dds" ) );
	m_textures.push_back( new Texture( "assets/knight/knight_23.dds" ) );
	m_textures.push_back( new Texture( "assets/knight/knight_24.dds" ) );
	m_textures.push_back( new Texture( "assets/knight/knight_25.dds" ) );
	m_textures.push_back( new Texture( "assets/knight/knight_26.dds" ) );
	m_textures.push_back( new Texture( "assets/knight/knight_27.dds" ) );
	m_textures.push_back( new Texture( "assets/knight/knight_28.dds" ) );
	m_textures.push_back( new Texture( "assets/knight/knight_29.dds" ) );
	m_textures.push_back( new Texture( "assets/knight/knight_30.dds" ) );
	m_textures.push_back( new Texture( "assets/knight/knight_31.dds" ) );
	m_textures.push_back( new Texture( "assets/knight/knight_32.dds" ) );
	m_textures.push_back( new Texture( "assets/knight/knight_33.dds" ) );
	m_textures.push_back( new Texture( "assets/knight/knight_34.dds" ) );
	m_textures.push_back( new Texture( "assets/knight/knight_35.dds" ) );
	m_textures.push_back( new Texture( "assets/knight/knight_36.dds" ) );
}

void Tester::Update( float delta, Input & input )
{
	input; // NOT USED

	// Age all sprites...
	for ( std::list< SpriteInstance >::iterator itr = m_sprites.begin(); itr != m_sprites.end();  )
	{
		itr->life -= delta * 1.0f / SpriteLifetime;
		if ( itr->life >= 1.0f )
		{
			itr = m_sprites.erase( itr );
		}
		else
		{
			++itr;
		}
	}

	m_birthClock += delta;
	while( m_birthClock >= SpriteBirthRate && m_sprites.size() < m_spriteMaxCap )
	{
		// Birth a sprite...
		m_birthClock -= SpriteBirthRate;
		m_sprites.push_back( SpriteInstance( V2( static_cast< float >( rand() % 1280 ), static_cast< float >( rand() % 800 ) ), m_textureIndex ) );
		++m_textureIndex;
		if ( m_textureIndex >= m_textures.size() )
		{
			m_textureIndex = 0;
		}
	}
}

void Tester::Render( IDirect3DDevice9 * dxDevice )
{
	dxDevice; // Not used

	if ( m_d3dxSprite )
	{
		for ( std::list< SpriteInstance >::iterator itr = m_sprites.begin(), end = m_sprites.end(); itr != end; ++itr )
		{
			IDirect3DTexture9 * texture = m_textures[ itr->textureIndex ]->GetTexture();
			m_d3dxSprite->Begin( D3DXSPRITE_ALPHABLEND );
			m_d3dxSprite->Draw( texture, 0, 0, &itr->position, D3DCOLOR_RGBA( 255, 255, 255, static_cast< unsigned char >( itr->life * 255 ) ) );
			m_d3dxSprite->End();
		}
	}
}

void Tester::Shutdown()
{
    m_font.Destroy();

	if ( m_d3dxSprite )
	{
		m_d3dxSprite->Release();
		m_d3dxSprite = 0;
	}

	for ( std::vector< Texture * >::iterator itr = m_textures.begin(), end = m_textures.end(); itr != end; ++itr )
	{
		(*itr)->Destroy();
	}
	m_textures.clear();
}
