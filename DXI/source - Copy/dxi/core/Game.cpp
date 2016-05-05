// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/VertexShaderFactory.h>

using namespace dxi;
using namespace core;

Game * Game::s_gameInstance = 0;

Game::Game()
: m_isQuitting( false )
{
	s_gameInstance = this;
}

Game::~Game()
{
}

bool Game::Initialize( IOS * os )
{
	m_os.reset( os );

	bool runGame = Setup( *os );
	
	// Create general asset managers...
	if ( runGame )
	{
        m_textureManager.reset( new TextureManager );
		m_effectManager.reset( new EffectManager );
		m_pixelShaderManager.reset( new PixelShaderManager );
		m_vertexShaderManager.reset( new ResourceManager< VertexShader >( ResourceDesc( "", "xml;vs" ), std::shared_ptr< ResourceManager< VertexShader >::IFactory >( new VertexShaderFactory ) ) );
		m_geometryManager.reset( new GeometryManager( m_textureManager, m_pixelShaderManager, m_vertexShaderManager ) );
		m_sceneManager.reset( new dxi::scene::SceneManager( m_geometryManager ) );
	}

	return runGame;
}

bool Game::Setup( IOS & os )
{
	// Do nothing - stubbed.
	return true;
}

void Game::Startup()
{
	m_os->Startup();

	m_input.reset( new Input( GetOS() ) );
}

void Game::OnDragDrop( const std::vector< unify::Path > & files, const unify::V2< float > & point )
{
	files; // Not used.
	point; // Not used.
}

void Game::BeforeUpdate()
{
	m_input->CallBeforeUpdate( GetOS().GetResolution(), GetOS().GetFullscreen() );
}

bool Game::Update( unify::Seconds elapsed, IInput & input )
{
    if ( input.KeyPressed( Key::Escape ) )
    {
        RequestQuit();
    }

	m_sceneManager->Update( elapsed, input );

	return true;
}

void Game::AfterUpdate()
{
	m_input->CallAfterUpdate();
}

void Game::BeforeRender()
{
	m_os->BeforeRender();
}

void Game::Render()
{
    m_sceneManager->Render();
}

void Game::AfterRender()
{
	m_os->AfterRender();
}

void Game::Shutdown()
{
	// Release asset managers...
    m_sceneManager.reset();
	m_geometryManager.reset();
	m_vertexShaderManager.reset();
	m_pixelShaderManager.reset();
	m_effectManager.reset();
	m_textureManager.reset();

	m_os->Shutdown();
}

IOS & Game::GetOS()
{
	return *m_os.get();
}

TextureManager::shared_ptr Game::GetTextureManager()
{
	return m_textureManager;
}

EffectManager::shared_ptr Game::GetEffectManager()
{
	return m_effectManager;
}

PixelShaderManager::shared_ptr Game::GetPixelShaderManager()
{
	return m_pixelShaderManager;
}

GeometryManager::shared_ptr Game::GetGeometryManager()
{
	return m_geometryManager;
}

scene::SceneManager::shared_ptr Game::GetSceneManager()
{
	return m_sceneManager;
}

Managers Game::GetManagers()
{
	return Managers( GetTextureManager(), GetManager< VertexShader >(), GetPixelShaderManager(), GetEffectManager(), GetGeometryManager() );
}

void Game::RequestQuit()
{
	m_isQuitting = true;
}

bool Game::IsQuitting() const
{
	return m_isQuitting;
}

IInput & Game::GetInput()
{
	return *m_input;
}


Game * Game::GetGameInstance()
{
	return s_gameInstance;
}

template<> 
VertexShaderManager Game::GetManager()
{
	return m_vertexShaderManager;
}

template<>
std::shared_ptr< Texture > Game::Find<Texture>( const std::string & id )
{
	return GetTextureManager() ? GetTextureManager()->Find( id ) : Texture::shared_ptr();
}

template<>
std::shared_ptr< Effect > Game::Find<Effect>( const std::string & id )
{
	return GetEffectManager() ? GetEffectManager()->Find( id ) : Effect::shared_ptr();
}

template<>
std::shared_ptr< PixelShader > Game::Find<PixelShader>( const std::string & id )
{
	return GetPixelShaderManager() ? GetPixelShaderManager()->Find( id ) : PixelShader::shared_ptr();
}

template<>
std::shared_ptr< VertexShader > Game::Find<VertexShader>( const std::string & id )
{
	return GetManager< VertexShader >() ? GetManager< VertexShader >()->Find( id ) : VertexShader::shared_ptr();
}

template<>
std::shared_ptr< geo::Geometry > Game::Find<geo::Geometry>( const std::string & id )
{
	return GetGeometryManager() ? GetGeometryManager()->Find( id ) : geo::Geometry::shared_ptr();
}

template<>
std::shared_ptr< scene::Scene > Game::Find<scene::Scene>( const std::string & id )
{
	return GetSceneManager() ? GetSceneManager()->Find( id ) : scene::Scene::shared_ptr();
}