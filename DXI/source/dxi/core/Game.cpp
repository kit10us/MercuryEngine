// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/EffectFactories.h>
#include <dxi/TextureFactory.h>
#include <dxi/VertexShaderFactory.h>
#include <dxi/PixelShaderFactories.h>
#include <dxi/GeometryFactory.h>
#include <dxi/null/Input.h>
#include <dxi/Input.h>

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

bool Game::Initialize( std::shared_ptr< IOS > os )
{
	m_os = os;

	if ( ! Setup( GetOS() ) )
	{
		return false;
	}
	
	// Create asset managers...

	GetResourceHub().AddManager( new rm::ResourceManagerSimple< Texture >( "Texture" ) );
	GetManager< Texture >()->AddFactory( new TextureSourceFactory );

	GetResourceHub().AddManager( new rm::ResourceManagerSimple< Effect >( "Effect" ) );
	GetManager< Effect >()->AddFactory( new EffectSourceFactory );

	GetResourceHub().AddManager( new rm::ResourceManagerSimple< PixelShader >( "PixelShader" ) );
	GetManager< PixelShader >()->AddFactory( new PixelShaderJsonFactory );
	GetManager< PixelShader >()->AddFactory( new PixelShaderXMLFactory );

	GetResourceHub().AddManager( new rm::ResourceManagerSimple< VertexShader >( "VertexShader" ) );
	GetManager< VertexShader >()->AddFactory( new VertexShaderJsonFactory );
	GetManager< VertexShader >()->AddFactory( new VertexShaderXMLFactory );

	GetResourceHub().AddManager( new rm::ResourceManagerSimple< Geometry >( "Geometry" ) );
	GetManager< Geometry >()->AddFactory( new GeometryXMLFactory );

	m_sceneManager.reset( new scene::SceneManager );

	return true;
}

bool Game::Setup( IOS & os )
{
	if ( ! unify::Path( "setup.xml" ).Exists() )
	{
		return true; // Skip this setup step if there is no XML.
	}
	
	qxml::Document doc( "setup.xml" );
	
	qxml::Element * setup = doc.FindElement( "setup" );
	
	if ( setup == nullptr )
	{
		return true; // Skip this setup step if there is no setup node in XML.
	}

	qxml::Element * fullscreen = setup->GetElement( "fullscreen" );
	if( fullscreen != nullptr )
	{
		os.SetFullscreen( unify::Cast< bool, std::string >( fullscreen->GetText() ) );
	}

	qxml::Element * resolution = setup->GetElement( "resolution" );
	if ( resolution != nullptr )
	{
		os.SetResolution( unify::Size< unsigned int >( resolution->GetAttribute< unsigned int >( "width" ), resolution->GetAttribute< unsigned int >( "height" ) ) );
	}

	return true;
}

void Game::Startup()
{
	m_os->Startup();

	// m_input.reset( new null::Input );
	m_input.reset( new Input( *m_os ) );
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

	m_resourceHub.Clear();

	m_os->Shutdown();
}

IOS & Game::GetOS()
{
	return *m_os.get();
}

scene::SceneManager::shared_ptr Game::GetSceneManager()
{
	return m_sceneManager;
}

scene::Scene::shared_ptr Game::FindScene( const std::string & id )
{
	return GetSceneManager() ? GetSceneManager()->Find( id ) : scene::Scene::shared_ptr();
}

template<>
rm::ResourceManagerSimple< Texture > * Game::GetManager()
{
	auto rm = GetResourceHub().GetManager< Texture >( "texture" );
	auto manager = unify::polymorphic_downcast< rm::ResourceManagerSimple< Texture > * >( rm );
	return manager;
}

template<> rm::ResourceManagerSimple< Effect > * Game::GetManager()
{
	auto manager = unify::polymorphic_downcast< rm::ResourceManagerSimple< Effect > * >(GetResourceHub().GetManager< Effect >( "effect" ));
	return manager;
}

template<> rm::ResourceManagerSimple< PixelShader > * Game::GetManager()
{
	auto manager = unify::polymorphic_downcast< rm::ResourceManagerSimple< PixelShader > * >(GetResourceHub().GetManager< PixelShader >( "PixelShader" ));
	return manager;
}

template<> rm::ResourceManagerSimple< VertexShader > * Game::GetManager()
{
	auto manager = unify::polymorphic_downcast< rm::ResourceManagerSimple< VertexShader > * >(GetResourceHub().GetManager< VertexShader >( "VertexShader" ));
	return manager;
}

template<> rm::ResourceManagerSimple< Geometry > * Game::GetManager()
{
	auto manager = unify::polymorphic_downcast< rm::ResourceManagerSimple< Geometry > * >(GetResourceHub().GetManager< Geometry >( "Geometry" ));
	return manager;
}

rm::ResourceHub & Game::GetResourceHub()
{
	return m_resourceHub;
}

const rm::ResourceHub & Game::GetResourceHub() const
{
	return m_resourceHub;
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
