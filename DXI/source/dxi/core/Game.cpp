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
#include <dxi/exception/FailedToCreate.h>
#include <dxi/Input.h>
#include <fstream>
#include <chrono>
#include <ctime>

using namespace dxi;
using namespace core;

bool Game::Setup( IOS & os )
{
	// STUBBED - optional for derived game class.
	return true;
}

void Game::Startup()
{
	// STUBBED - optional for derived game class.
}

bool Game::Update( RenderInfo & renderInfo, IInput & input )
{
	// STUBBED - optional for derived game class.
	return true;
}

void Game::Render( int renderer, const RenderInfo & renderInfo, const Viewport & viewport )
{
	// STUBBED - optional for derived game class.
}

void Game::Shutdown()
{
	// STUBBED - Provided by user.
}

Game::Game( unify::Path setup )
: m_setup( setup )
, m_isQuitting( false )
, m_totalStartupTime{}
{
}

Game::~Game()
{
	// Call user shutdown.
	Shutdown();

	// Release scripts.
	m_gameModule.reset();
	m_scriptEngines.clear();

	// Release asset managers...
	m_sceneManager.reset();

	m_resourceHub.Clear();

	m_inputManager.Clear();

	// Remove extensions...
	m_extensions.clear();

	auto now = std::chrono::system_clock::now();
	std::time_t t = std::chrono::system_clock::to_time_t( now );
	Log( "Shutdown: " + std::string( std::ctime( &t ) ) );
	const RenderInfo & renderInfo = GetRenderInfo();
	LogLine( "  frames: " + unify::Cast< std::string >( renderInfo.FrameID() ) + ", total delta: " + unify::Cast< std::string >( renderInfo.GetTotalDelta() ) + "s,  average fps:" + unify::Cast< std::string >( renderInfo.GetFPS() ) );
	LogLine( "" );

	m_os->Shutdown();
}

bool Game::Initialize( std::shared_ptr< IOS > os )
{
	using namespace std::chrono;
	high_resolution_clock::time_point lastTime = high_resolution_clock::now();

	m_totalStartupTime = {};

	m_os = os;
					  	
	// User setup...
	if ( ! Setup( GetOS() ) )
	{
		return false;
	}

	// Early setup...
	if( m_setup.Exists() )
	{
		qxml::Document doc( m_setup );

		qxml::Element * setup = doc.FindElement( "setup" );
		if( setup )
		{
			for( auto && node : setup->Children() )
			{
				if( node.IsTagName( "logfile" ) )
				{
					m_logFile = node.GetText();
				}
				if( node.IsTagName( "display" ) )
				{
					bool fullscreen = node.GetAttributeElse< bool >( "fullscreen", false );
					int width = node.GetAttribute< int >( "width" );
					int height = node.GetAttribute< int >( "height" );
					int x = node.GetAttributeElse< int >( "x", 0 );
					int y = node.GetAttributeElse< int >( "y", 0 );

					if ( fullscreen )
					{
						GetOS().AddDisplay( core::Display::CreateFullscreenDirectXDisplay( unify::Size< float >( (float)width, (float)height ) ) );
					}
					else
					{
						GetOS().AddDisplay( core::Display::CreateWindowedDirectXDisplay( unify::Size< float >( (float)width, (float)height ), unify::V2< float >( (float)x, (float)y ) ) );
					}
				}
			}
		}
	}

	// Creates displays...
	GetOS().Startup();

	// Create asset managers...

	GetResourceHub().AddManager( std::shared_ptr< rm::IResourceManagerEarly >( new rm::ResourceManagerSimple< Texture >( "Texture" ) ) );

	TextureFactoryPtr textureFactoryPtr( new TextureSourceFactory( this ) );
	GetManager< Texture >()->AddFactory( ".dds", textureFactoryPtr ); // TODO: Can't we just share this between types?
	GetManager< Texture >()->AddFactory( ".bmp", textureFactoryPtr );
	GetManager< Texture >()->AddFactory( ".jpg", textureFactoryPtr );

	GetResourceHub().AddManager( std::shared_ptr< rm::IResourceManagerEarly >( new rm::ResourceManagerSimple< Effect >( "Effect" ) ) );
	GetManager< Effect >()->AddFactory( ".effect", EffectFactoryPtr( new EffectFactory( this ) ) );

	GetResourceHub().AddManager( std::shared_ptr< rm::IResourceManagerEarly >( new rm::ResourceManagerSimple< PixelShader >( "PixelShader" ) ) );
	GetManager< PixelShader >()->AddFactory( ".xml", PixelShaderFactoryPtr( new PixelShaderFactory( this ) ) );

	GetResourceHub().AddManager( std::shared_ptr< rm::IResourceManagerEarly >( new rm::ResourceManagerSimple< VertexShader >( "VertexShader" ) ) );
	GetManager< VertexShader >()->AddFactory( ".xml", VertexShaderFactoryPtr( new VertexShaderFactory( this ) ) );

	GetResourceHub().AddManager( std::shared_ptr< rm::IResourceManagerEarly >( new rm::ResourceManagerSimple< Geometry >( "Geometry" ) ) );
	GetManager< Geometry >()->AddFactory( ".xml", GeometryFactoryPtr( new GeometryFactory( this ) ) );
	GetManager< Geometry >()->AddFactory( ".shape", GeometryFactoryPtr( new ShapeFactory( this ) ) );

	m_sceneManager.reset( new scene::SceneManager( this ) );

	// Log start of program.
	auto now = std::chrono::system_clock::now();
	std::time_t t = std::chrono::system_clock::to_time_t( now );
	Log( "Startup: " + ((!m_os->GetName().empty()) ? m_os->GetName() : "<unknown>") + ", " + std::ctime( &t ) );

	// Our setup...
	if( m_setup.Exists() )
	{
		qxml::Document doc( m_setup );

		qxml::Element * setup = doc.FindElement( "setup" );
		if( setup )
		{
			for( auto && node : setup->Children() )
			{
				if( node.IsTagName( "extension" ) )
				{
					unify::Path path( node.GetText() );
					//m_extensions.push_back( std::shared_ptr< core::Extension >( new Extension( node.GetDocument()->GetPath().DirectoryOnly() + path ) ) );
					AddExtension( std::shared_ptr< core::Extension >( new Extension( node.GetDocument()->GetPath().DirectoryOnly() + path ) ) );
				}
				else if( node.IsTagName( "gamemodule" ) )
				{
					std::string type = node.GetAttribute< std::string >( "type" );
					auto se = GetScriptEngine( type );
					assert( se ); //TODO: Handle error better.
					m_gameModule = se->LoadModule( node.GetDocument()->GetPath().DirectoryOnly() + node.GetAttribute< std::string >( "source" ), scene::Object::ptr() );
				}
			}
		}
	}

	if( m_gameModule )
	{
		m_gameModule->OnStart();
	}

	m_input.reset( new null::Input );
	//m_input.reset( new Input( GetOS() ) );

	// User startup...
	Startup();

	high_resolution_clock::time_point currentTime = high_resolution_clock::now();
	duration< float > elapsed_d = duration_cast<duration< float >>(currentTime - lastTime);
	auto micro = duration_cast< microseconds >(currentTime - lastTime).count();
	m_totalStartupTime = micro * 0.000001f;

	LogLine( "  total startup time: " + unify::Cast< std::string >( m_totalStartupTime ) + "s" );

	return true;
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

	m_renderInfo.SetDelta( elapsed );

	if( m_gameModule )
	{
		m_gameModule->OnUpdate();
	}

	if( GetInput().KeyPressed( Key::Escape ) )
	{
		RequestQuit();
	}

	m_sceneManager->Update( m_renderInfo, GetInput() );

	bool run = Update( m_renderInfo, GetInput() );
	AfterUpdate();
}

void Game::BeforeUpdate()
{
	m_os;

	unify::Size< int > resolution( GetOS().GetRenderer( 0 )->GetViewport().GetWidth(), GetOS().GetRenderer( 0 )->GetViewport().GetHeight() );
	m_input->CallBeforeUpdate( resolution, GetOS().GetRenderer(0)->IsFullscreen() );

	m_inputManager.Update();
}

void Game::AfterUpdate()
{
	m_input->CallAfterUpdate();
}

void Game::Draw()
{
	for( int index = 0; index < GetOS().RendererCount(); ++index )
	{
		IRenderer & renderer = *m_os->GetRenderer( index );
		renderer.BeforeRender();
		m_sceneManager->Render( index, renderer.GetViewport() );
		Render( index, m_renderInfo, renderer.GetViewport() );	
		renderer.AfterRender();
	}
	m_renderInfo.IncrementFrameID();
}

const RenderInfo & Game::GetRenderInfo() const
{
	return m_renderInfo;
}

IOS & Game::GetOS()
{
	return *m_os.get();
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

input::InputManager * Game::GetInputManager()
{
	return &m_inputManager;
}

const input::InputManager * Game::GetInputManager() const
{
	return &m_inputManager;
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

void Game::ReportError( ErrorLevel level, std::string source, std::string error )
{
	switch( level )
	{
	case ErrorLevel::Critical:
		m_criticalErrors.push_back( "Critical Failure (" + source + "): \"" + error + "\"." );
		LogLine( "Critical Failure (" + source + "): \"" + error + "\"." );
		return;
	case ErrorLevel::Failure:
		LogLine( "Failure (" + source + "): \"" + error + "\"." );
		break;
	case ErrorLevel::Warning:
		LogLine( "Warning (" + source + "): \"" + error + "\"." );
		break;
	}
	return;
}
						   
bool Game::HadCriticalError() const
{
	return m_criticalErrors.size() != 0;
}

Geometry::ptr Game::CreateShape( unify::Parameters parameters )
{
	return Geometry::ptr( shapes::CreateShape( parameters ) );
}
