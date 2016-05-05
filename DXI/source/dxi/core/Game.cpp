// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/null/Input.h>
#include <dxi/EffectFactories.h>
#include <dxi/TextureFactory.h>
#include <dxi/VertexShaderFactory.h>
#include <dxi/PixelShaderFactories.h>
#include <dxi/GeometryFactory.h>

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

	bool runGame = Setup( os );
	
	// Create general asset managers...
	if ( runGame )
	{
		m_textureManager.reset( new rm::ResourceManagerSimple< Texture >( "Texture" ) );
		m_textureManager->AddFactory( new TextureSourceFactory );

		m_effectManager.reset( new rm::ResourceManagerSimple< Effect >( "Effect" ) );
		m_effectManager->AddFactory( new EffectSourceFactory );

		m_pixelShaderManager.reset( new rm::ResourceManagerSimple< PixelShader >( "PixelShader" ) );
		m_pixelShaderManager->AddFactory( new PixelShaderJsonFactory );
		m_pixelShaderManager->AddFactory( new PixelShaderXMLFactory );

		m_vertexShaderManager.reset(  new rm::ResourceManagerSimple< VertexShader >( "VertexShader" ) );
		m_vertexShaderManager->AddFactory( new VertexShaderJsonFactory );
		m_vertexShaderManager->AddFactory( new VertexShaderXMLFactory );

		m_geometryManager.reset( new rm::ResourceManagerSimple< Geometry >( "Geometry" ) );
		m_geometryManager->AddFactory( new GeometryFactory );

		m_sceneManager.reset( new scene::SceneManager );
	}

	return runGame;
}

bool Game::Setup( IOS * os )
{
	// Do nothing - stubbed.
	return true;
}

void Game::Startup()
{
	m_os->Startup();

	m_input.reset( new null::Input );
}

void Game::OnDragDrop( const std::vector< unify::Path > & files, const unify::V2< float > & point )
{
	files; // Not used.
	point; // Not used.
}

void Game::BeforeUpdate()
{
	m_input->CallBeforeUpdate( GetOS()->GetResolution(), GetOS()->GetFullscreen() );
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

IOS * Game::GetOS()
{
	return m_os.get();
}

scene::SceneManager::shared_ptr Game::GetSceneManager()
{
	return m_sceneManager;
}

scene::Scene::shared_ptr Game::FindScene( const std::string & id )
{
	return GetSceneManager() ? GetSceneManager()->Find( id ) : scene::Scene::shared_ptr();
}

template<> std::shared_ptr< rm::ResourceManagerSimple< Texture > > Game::GetManager()
{
	return m_textureManager;
}

template<> std::shared_ptr< rm::ResourceManagerSimple< Effect > > Game::GetManager()
{
	return m_effectManager;
}

template<> std::shared_ptr< rm::ResourceManagerSimple< PixelShader > > Game::GetManager()
{
	return m_pixelShaderManager;
}

template<> std::shared_ptr< rm::ResourceManagerSimple< VertexShader > > Game::GetManager()
{
	return m_vertexShaderManager;
}

template<> std::shared_ptr< rm::ResourceManagerSimple< Geometry > > Game::GetManager()
{
	return m_geometryManager;
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
