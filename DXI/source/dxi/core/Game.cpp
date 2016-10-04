// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/core/Game.h>
#include <dxi/factory/EffectFactories.h>
#include <dxi/factory/TextureFactory.h>
#include <dxi/factory/VertexShaderFactory.h>
#include <dxi/factory/PixelShaderFactories.h>
#include <dxi/factory/GeometryFactory.h>
#include <dxi/factory/ShapeFactory.h>
#include <dxi/null/Input.h>
#include <dxi/Input.h>
#include <fstream>
#include <chrono>
#include <ctime>

using namespace dxi;
using namespace core;

Game * Game::s_gameInstance = 0;

Game::Game( unify::Path setup )
: m_setup( setup )
, m_isQuitting( false )
{
	s_gameInstance = this;
}

Game::~Game()
{
	Shutdown();
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
	GetManager< Texture >()->AddFactory( "dds", new TextureSourceFactory );
	GetManager< Texture >()->AddFactory( "bmp", new TextureSourceFactory );
	GetManager< Texture >()->AddFactory( "jpg", new TextureSourceFactory );

	GetResourceHub().AddManager( new rm::ResourceManagerSimple< Effect >( "Effect" ) );
	GetManager< Effect >()->AddFactory( "effect", new EffectXMLFactory );

	GetResourceHub().AddManager( new rm::ResourceManagerSimple< PixelShader >( "PixelShader" ) );
	GetManager< PixelShader >()->AddFactory( new PixelShaderJsonFactory );
	GetManager< PixelShader >()->AddFactory( "pixelshader", new PixelShaderXMLFactory );

	GetResourceHub().AddManager( new rm::ResourceManagerSimple< VertexShader >( "VertexShader" ) );
	GetManager< VertexShader >()->AddFactory( new VertexShaderJsonFactory );
	GetManager< VertexShader >()->AddFactory( "vertexshader", new VertexShaderXMLFactory );

	GetResourceHub().AddManager( new rm::ResourceManagerSimple< Geometry >( "Geometry" ) );
	GetManager< Geometry >()->AddFactory( "geometry", new GeometryXMLFactory );
	GetManager< Geometry >()->AddFactory( "shape", new ShapeXMLFactory );

	m_sceneManager.reset( new scene::SceneManager( this ) );

	return true;
}

bool Game::Setup( IOS & os )
{
	if ( ! unify::Path( "setup.xml" ).Exists() )
	{
		return true; // Skip this setup step if there is no XML.
	}
	
	qxml::Document doc( m_setup );
	
	qxml::Element * setup = doc.FindElement( "setup" );	 	
	if ( setup == nullptr )
	{
		return true; // Skip this setup step if there is no setup node in XML.
	}

	for( auto && node : setup->Children() )
	{
		if ( node.IsTagName( "logfile" ) )
		{
			m_logFile = node.GetText();
		}
		if ( node.IsTagName( "fullscreen" ) )
		{
			os.SetFullscreen( unify::Cast< bool, std::string >( node.GetText() ) );
		}
		else if ( node.IsTagName( "resolution" ) )
		{
			os.SetResolution( unify::Size< unsigned int >( node.GetAttribute< unsigned int >( "width" ), node.GetAttribute< unsigned int >( "height" ) ) );
		}
		else if ( node.IsTagName( "extension" ) )
		{		   
			unify::Path path( node.GetText() );
			AddExtension( std::shared_ptr< core::Extension >( new Extension( node.GetDocument()->GetPath().DirectoryOnly() + path ) ) );
		}
		else if ( node.IsTagName( "gamemodule" ) )
		{
			std::string type = node.GetAttribute< std::string >( "type" );
			auto se = GetScriptEngine( type );
			assert( se ); //TODO: Handle error better.
			m_gameModule = se->LoadModule( node.GetDocument()->GetPath().DirectoryOnly() + node.GetAttribute< std::string >( "source" ) );
		}
	}

	// Log start of program.
	auto now = std::chrono::system_clock::now();
	std::time_t t = std::chrono::system_clock::to_time_t( now );
	Log( "Startup: " + ((!m_os->GetName().empty()) ? m_os->GetName() : "<unknown>") + ", " + std::ctime( &t ) );

	return true;
}

void Game::Startup()
{
	m_os->Startup();

	if ( m_gameModule )
	{
		m_gameModule->OnStart();
	}

	// m_input.reset( new null::Input );
	m_input.reset( new Input( *m_os ) );
}

void Game::OnDragDrop( const std::vector< unify::Path > & files, const unify::V2< float > & point )
{
	files; // Not used.
	point; // Not used.
}

void Game::Tick()
{
	using namespace std::chrono;
	static high_resolution_clock::time_point lastTime = high_resolution_clock::now();
	high_resolution_clock::time_point currentTime = high_resolution_clock::now();
	duration< float > elapsed_d = duration_cast<duration< float >>(currentTime - lastTime);
	auto micro = duration_cast< microseconds >(currentTime - lastTime).count();
	unify::Seconds elapsed = micro * 0.000001f;
	lastTime = currentTime;

	BeforeUpdate();
	m_renderInfo.SetDelta( elapsed ); // TODO: Pull elapsed from Update, since it's now in RenderInfo.
	bool run = Update( elapsed, m_renderInfo, GetInput() );
	AfterUpdate();
}

void Game::BeforeUpdate()
{
	m_input->CallBeforeUpdate( GetOS().GetResolution(), GetOS().GetFullscreen() );
}

bool Game::Update( unify::Seconds elapsed, RenderInfo & renderInfo, IInput & input )
{
	if( m_gameModule )
	{
		m_gameModule->OnUpdate();
	}
	
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

void Game::Draw()
{
	BeforeRender();
	Render( m_renderInfo );
	AfterRender();
}

void Game::BeforeRender()
{
	m_os->GetRenderer()->BeforeRender();
}

void Game::Render( const RenderInfo & renderInfo )
{
    m_sceneManager->Render();
}

void Game::AfterRender()
{
	m_renderInfo.IncrementFrameID();
	m_os->GetRenderer()->AfterRender();
}

void Game::Shutdown()
{
	// Release scripts first...
	m_gameModule.reset();

	// Release asset managers...
    m_sceneManager.reset();

	m_resourceHub.Clear();

	m_scriptEngines.clear();

	// Remove extensions...
	m_extensions.clear();

	auto now = std::chrono::system_clock::now();
	std::time_t t = std::chrono::system_clock::to_time_t( now );
	Log( "Shutdown: " + ( ( ! m_os->GetName().empty() ) ? m_os->GetName() : "<unknown>") + ", " + std::ctime( &t ) );
	LogLine( "  frames: " + unify::Cast< std::string >( m_renderInfo.FrameID() ) );
	LogLine( "" );

	m_os->Shutdown();
}

IOS & Game::GetOS()
{
	return *m_os.get();
}

const RenderInfo & Game::GetRenderInfo() const
{
	return m_renderInfo;
}

void Game::AddScriptEngine( std::string name, std::shared_ptr< scripting::IScriptEngine > se )
{
	m_scriptEngines[name] = se;
}

scripting::IScriptEngine * Game::GetScriptEngine( std::string name )
{
	auto se = m_scriptEngines.find( name );
	return ( se == m_scriptEngines.end() ) ? nullptr : se->second.get();
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

Game * Game::GetInstance()
{
	return s_gameInstance;
}

void Game::AddExtension( std::shared_ptr< Extension > extension )
{
	m_extensions.push_back( extension );
	if ( ! extension->Load( this ) )
	{
		throw exception::FailedToCreate( "Failed to load extension!" );
	}
}

void Game::Log( std::string text )
{
	using namespace std;

	if( m_logFile.Empty() ) return;

	ofstream out( m_logFile.ToString(), ios_base::out | ios_base::app  );
	out << text;
}

void Game::LogLine( std::string line )
{
	Log( line + "\n" );
}
