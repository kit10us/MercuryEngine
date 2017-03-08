// Copyright (c) 2003 - 2014, Quentin S. Smith
// Alol Rights Reserved

#include <me/Game.h>
#include <me/factory/EffectFactories.h>
#include <me/factory/TextureFactory.h>
#include <me/factory/VertexShaderFactory.h>
#include <me/factory/PixelShaderFactories.h>
#include <me/factory/GeometryFactory.h>
#include <sg/ShapeFactory.h>
#include <me/exception/FailedToCreate.h>
#include <me/scene/SceneManager.h>
#include <fstream>
#include <chrono>
#include <ctime>
#include <functional>

// Temporary...
#include <me/scene/SceneManager.h>
#include <me/scene/CameraComponent.h>
#include <me/scene2d/CanvasComponent.h>
#include <me/scene2d/FPS.h>


using namespace me;
using namespace scene;


class GameLogger : public rm::ILogger
{
	IGame * m_game;
public:
	GameLogger( IGame * game ) : m_game( game )
	{
	}

	void WriteLine( std::string text ) override
	{
		m_game->LogLine( text );
	}

};


bool Game::Setup( IOS * os )
{
	// STUBBED - optional for derived game class.
	return true;
}

void Game::Startup()
{
	// STUBBED - optional for derived game class.
}

void Game::Update( UpdateParams params )
{
	// STUBBED - optional for derived game class.
}

void Game::Render( RenderParams params )
{
	// STUBBED - optional for derived game class.
}

void Game::Shutdown()
{
	// STUBBED - Provided by user.
}

Game::Game( unify::Path setup )
	: m_title{ "Mercury Engine" }
	, m_failuresAsCritial( true )
	, m_setup( setup )
	, m_isQuitting( false )
	, m_totalStartupTime{}
	, m_updateEnabled{ true }
{
}

Game::~Game()
{
	LogLine( "Shutting down:", 0 );

	LogLine( "OnDetach", 0 );
	for( auto && component : m_components )
	{
		LogLine( component->GetName() );
		component->OnDetach( this );
	}
	LogLine( "OnDetachDone", 0 );


	// Remove all log listeners.
	m_logListeners.clear();

	// Call user shutdown.
	Shutdown();
	
	m_resourceHub.Clear();

	m_exitMotivation.reset();
	m_inputManager.Clear();

	m_components.clear();

	if ( m_os )
	{
		m_os->Shutdown();
		m_os.reset();
	}

	// Remove extensions...
	m_extensions.clear();

	auto now = std::chrono::system_clock::now();
	std::time_t t = std::chrono::system_clock::to_time_t( now );
	const RenderInfo & renderInfo = GetRenderInfo();
	LogLine( "time: " + std::string( std::ctime( &t ) ) );
	LogLine( "frames: " + unify::Cast< std::string >( renderInfo.FrameID() ) + ", total delta: " + unify::Cast< std::string >( renderInfo.GetTotalDelta() ) + "s,  average fps:" + unify::Cast< std::string >( renderInfo.GetFPS() ) );
}

me::OSParameters Game::GetOSParameters() const
{
	return m_osParameters;
}

void * Game::Feed( std::string target, void * data )
{
	if ( unify::StringIs( target, "OS" ) )
	{
		return m_os->Feed( target, data );
	}
	else if ( unify::StringIs( target, "COMMAND" ) )
	{
		char * command = (char *)data;
		if ( unify::StringIs( command, "QUIT" ) )
		{
			Quit();
		}
		else if ( unify::StringIs( command, "ADDFPS" ) )
		{
			using namespace me;
			using namespace scene;
		
			SceneManager * sceneManager = dynamic_cast< scene::SceneManager * >(GetComponent( "SceneManager", 0 ).get());
			Scene::ptr scene = sceneManager->FindScene( "scene1" );

			scene2d::CanvasComponent::ptr canvas( new scene2d::CanvasComponent( this ) );
			scene->AddComponent( canvas );

			Effect::ptr font2 = GetManager< Effect>()->Add( "font2", "font2.effect" );	
			canvas->GetLayer()->AddElement( scene2d::IElement::ptr( new scene2d::FPS( this, font2 ) ) );
		}
	}
	
	return 0;
}

bool Game::Initialize( OSParameters osParameters )
{
	using namespace std::chrono;
	high_resolution_clock::time_point lastTime = high_resolution_clock::now();

	m_totalStartupTime = {};

	m_osParameters = osParameters;

	// Parse the commandline...
	std::string commandLineString( m_osParameters.lpszCmdLine );
	std::vector< std::string > commandLineVector;
	size_t l = 0;
	size_t r = 0;
	bool inQuote = false;
	std::string working;

	for( size_t l = 0, r = 0; r <= commandLineString.length(); ++r )
	{
		if( !inQuote && (r == commandLineString.length() || commandLineString.at( r ) == ' ') )
		{
			if( l != r )
			{
				working += commandLineString.substr( l, r - l );
			}
			if( working.empty() == false )
			{
				commandLineVector.push_back( working );
				working.clear();
			}
			l = r + 1;
		}
		else if( commandLineString.at( r ) == '\"' )
		{
			// Include partial string...
			working += commandLineString.substr( l, r - l );
			l = r + 1; // One past the double quote.
			inQuote = !inQuote;
		}
	}

	for( auto && arg : commandLineVector )
	{
		if( unify::Path( arg ).IsExtension( ".xml" ) )
		{
			m_setup = arg;
		}
	}

	// Check for OS in setup...
	if( !m_setup.Empty() )
	{
		if( !m_setup.Exists() )
		{
			ReportError( ErrorLevel::Critical, "Game", "Setup file, \"" + m_setup.ToString() + "\" not found!" );
		}

		std::function< void( unify::Path ) > xmlLoader = [&]( unify::Path path )
		{
			qxml::Document doc( path );

			qxml::Element * setup = doc.GetRoot();
			if( setup )
			{
				for( auto && node : setup->Children() )
				{
					if ( node.IsTagName( "include" ) )
					{
						xmlLoader( unify::Path( node.GetText() ) );
					}
					else if( node.IsTagName( "renderer" ) )
					{
						unify::Path path( node.GetAttribute< std::string >( "source" ) );
						AddExtension( node.GetDocument()->GetPath().DirectoryOnly() + path, &node );
					}
				}
			}
		};
		xmlLoader( m_setup );
	}

	if ( ! m_os )
	{
		throw me::exception::FailedToCreate( "No renderer specified, or invalid renderer!" );
	}
					  	
	// User setup...
	if ( ! Setup( GetOS() ) )
	{
		return false;
	}

	// Early setup...
	if ( ! m_setup.Empty() )
	{
		if( ! m_setup.Exists() )
		{
			ReportError( ErrorLevel::Critical, "Game", "Setup file, \"" + m_setup.ToString() + "\" not found!" );
		}

		std::function< void( unify::Path ) > xmlLoader = [&]( unify::Path source )
		{
			qxml::Document doc( source );

			qxml::Element * setup = doc.GetRoot();
			if( setup )
			{
				for( auto && node : setup->Children() )
				{			
					if ( node.IsTagName( "include" ) )
					{
						xmlLoader( unify::Path( node.GetText() ) );
					}
					else if ( node.IsTagName( "title" ) )
					{
						m_title = node.GetText();
					}
					else if( node.IsTagName( "logfile" ) )
					{
						m_logFile = node.GetText();

						// Delete current log.
						DeleteFileA( (char*)m_logFile.ToString().c_str() );
					}
					else if ( node.IsTagName( "FailuresAsCritial" ) )
					{
						m_failuresAsCritial = unify::Cast< bool >( node.GetText() );
					}
					else if( node.IsTagName( "assets" ) )
					{
						GetOS()->GetAssetPaths().AddSource( node.GetText() );
					}
				}
			}
		};
		xmlLoader( m_setup );	
	}

	// Creates displays...
	GetOS()->BuildRenderers( m_title );
	assert( GetOS()->GetRenderer( 0 ) );

	// Get the first handle...
	m_osParameters.hWnd = GetOS()->GetRenderer( 0 )->GetHandle();

	// Create asset managers...

	rm::ILogger::ptr logger( new GameLogger( this ) );

	GetResourceHub().AddManager( std::shared_ptr< rm::IResourceManagerRaw >( new rm::ResourceManagerSimple< ITexture >( "Texture", &GetOS()->GetAssetPaths(), logger ) ) );

	TextureFactoryPtr textureFactoryPtr( new TextureSourceFactory( this ) );
	GetManager< ITexture >()->AddFactory( ".dds", textureFactoryPtr );
	GetManager< ITexture >()->AddFactory( ".png", textureFactoryPtr );
	GetManager< ITexture >()->AddFactory( ".bmp", textureFactoryPtr );
	GetManager< ITexture >()->AddFactory( ".jpg", textureFactoryPtr );

	GetResourceHub().AddManager( std::shared_ptr< rm::IResourceManagerRaw >( new rm::ResourceManagerSimple< Effect >( "Effect", &GetOS()->GetAssetPaths(), logger ) ) );
	GetManager< Effect >()->AddFactory( ".effect", EffectFactoryPtr( new EffectFactory( this ) ) );

	GetResourceHub().AddManager( std::shared_ptr< rm::IResourceManagerRaw >( new rm::ResourceManagerSimple< IPixelShader >( "PixelShader", &GetOS()->GetAssetPaths(), logger ) ) );
	GetManager< IPixelShader >()->AddFactory( ".xml", PixelShaderFactoryPtr( new PixelShaderFactory( this ) ) );

	GetResourceHub().AddManager( std::shared_ptr< rm::IResourceManagerRaw >( new rm::ResourceManagerSimple< IVertexShader >( "VertexShader", &GetOS()->GetAssetPaths(), logger ) ) );
	GetManager< IVertexShader >()->AddFactory( ".xml", VertexShaderFactoryPtr( new VertexShaderFactory( this ) ) );

	GetResourceHub().AddManager( std::shared_ptr< rm::IResourceManagerRaw >( new rm::ResourceManagerSimple< Geometry >( "Geometry", &GetOS()->GetAssetPaths(), logger ) ) );
	GetManager< Geometry >()->AddFactory( ".xml", GeometryFactoryPtr( new GeometryFactory( this ) ) );
	GetManager< Geometry >()->AddFactory( ".shape", GeometryFactoryPtr( new sg::ShapeFactory( this ) ) );

	AddComponent( IGameComponent::ptr( new scene::SceneManager( this ) ) );

	// Log start of program.
	auto now = std::chrono::system_clock::now();
	std::time_t t = std::chrono::system_clock::to_time_t( now );
	LogLine( "Initializing: ", 0 );
	LogLine( "name:    " + ((!m_os->GetName().empty()) ? m_os->GetName() : "<unknown>") );
	LogLine( "program: " + m_os->GetProgramPath().ToString() );
	LogLine( "path:    " + m_os->GetRunPath().ToString() );
	LogLine( "time:    " + std::string( std::ctime( &t ) ) );

	// Our setup...
	if( m_setup.Exists() )
	{

		std::function< void( unify::Path ) > xmlLoader = [&]( unify::Path path )
		{
			qxml::Document doc( path );
			LogLine( "Loading setup \"" + path.ToString() + "\"..." );

			qxml::Element * setup = doc.GetRoot();
			if( setup )
			{
				for( auto && node : setup->Children() )
				{
					if ( node.IsTagName( "include" ) )
					{
						xmlLoader( unify::Path( node.GetText() ) );
					}
					else if( node.IsTagName( "extension" ) )
					{
						unify::Path path( node.GetAttribute< std::string >( "source" ) );
						AddExtension( node.GetDocument()->GetPath().DirectoryOnly() + path, &node );
					}
				}
			}
		};
		xmlLoader( m_setup );
	}

	LogLine( "OS Startup", 0 );
	m_os->Startup();
	LogLine( "OS Startup Done", 0 );
	
	LogLine( "OnBeforeStartup", 0 );
	for( auto && component : m_components )
	{
		LogLine( component->GetName() + "..." );
		component->OnBeforeStartup( this );
	}
	LogLine( "OnBeforeStartup Done", 0 );

	
	LogLine( "Startup", 0 );
	try
	{
		Startup();
	}
	catch( unify::Exception ex )
	{
		LogLine( "Startup FAILED" );
		ReportError( ErrorLevel::Critical, "Game", ex.what() );
	}
	LogLine( "Startup Done", 0 );

	LogLine( "OnAfterStartup", 0 );
	for( auto && component : m_components )
	{
		LogLine( component->GetName() + "..." );
		component->OnAfterStartup( this );
	}
	LogLine( "OnAfterStartup Done", 0 );

	// Basic motivations...
	if ( GetInputManager() )
	{
		input::IInputSource::ptr keyboard( GetInputManager()->FindSource( "keyboard" ) );
		if ( keyboard )
		{
			m_exitMotivation = input::IInputCondition::ptr( new input::ButtonPressedCondition( keyboard, 0, "Escape" ) );
		}
	}	
	high_resolution_clock::time_point currentTime = high_resolution_clock::now();
	duration< float > elapsed_d = duration_cast<duration< float >>(currentTime - lastTime);
	auto micro = duration_cast< microseconds >(currentTime - lastTime).count();
	m_totalStartupTime = micro * 0.000001f;

	LogLine( "total startup time: " + unify::Cast< std::string >( m_totalStartupTime ) + "s", 0 );

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
	
	if ( !m_updateEnabled ) return;

	m_inputManager.Update();

	if ( m_exitMotivation && m_exitMotivation->IsTrue() )
	{
		Quit();
	}

	m_renderInfo.SetDelta( elapsed );			  
	auto renderer = GetOS()->GetRenderer( 0 );

	UpdateParams params{ renderer, m_renderInfo };

	for( auto && component : m_components )
	{
		component->OnUpdate( this, params );
	}

	Update( params );
}

void Game::Draw()
{
	for( int index = 0; index < GetOS()->RendererCount(); ++index )
	{
		IRenderer * renderer = m_os->GetRenderer( index );
		renderer->BeforeRender();

		RenderParams params{ renderer, m_renderInfo };
										  	
		for( auto && component : m_components )
		{
			component->OnRender( this, params );
		}

		Render( params );

		renderer->AfterRender();
	}
	m_renderInfo.IncrementFrameID();
}

const RenderInfo & Game::GetRenderInfo() const
{
	return m_renderInfo;
}

void Game::SetOS( me::IOS::ptr os )
{
	m_os = os;
}

IOS * Game::GetOS()
{
	return m_os.get();
}

template<>
rm::ResourceManagerSimple< ITexture > * Game::GetManager()
{
	auto rm = GetResourceHub().GetManager< ITexture >( "texture" );
	auto manager = unify::polymorphic_downcast< rm::ResourceManagerSimple< ITexture > * >( rm );
	return manager;
}

template<> rm::ResourceManagerSimple< Effect > * Game::GetManager()
{
	auto manager = unify::polymorphic_downcast< rm::ResourceManagerSimple< Effect > * >(GetResourceHub().GetManager< Effect >( "effect" ));
	return manager;
}

template<> rm::ResourceManagerSimple< IPixelShader > * Game::GetManager()
{
	auto manager = unify::polymorphic_downcast< rm::ResourceManagerSimple< IPixelShader > * >(GetResourceHub().GetManager< IPixelShader >( "PixelShader" ));
	return manager;
}

template<> rm::ResourceManagerSimple< IVertexShader > * Game::GetManager()
{
	auto manager = unify::polymorphic_downcast< rm::ResourceManagerSimple< IVertexShader > * >(GetResourceHub().GetManager< IVertexShader >( "VertexShader" ));
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

void Game::Quit()
{
	m_isQuitting = true;
}

bool Game::IsQuitting() const
{
	return m_isQuitting;
}

input::InputManager * Game::GetInputManager()
{
	return &m_inputManager;
}

const input::InputManager * Game::GetInputManager() const
{
	return &m_inputManager;
}

void Game::AddExtension( unify::Path path, const qxml::Element * element )
{
	std::shared_ptr< Extension > extension( new Extension( path ) );

	if ( ! extension->Load( this, element ) )
	{
		ReportError( ErrorLevel::Critical, "Game", "Failed to load extension " + path.ToString() + "!" );
	}

	m_extensions.push_back( extension );
}

void Game::LogLine( std::string line, int indent )
{
	using namespace std;

	std::string text = "";
	for ( int i = 0; i < indent; i++ )
	{
		text += " ";
	}
	text += line + "\n";

	if( m_logFile.Empty() ) return;

	ofstream out( m_logFile.ToString(), ios_base::out | ios_base::app  );
	out << text;
	OutputDebugStringA( text.c_str() );

	for ( auto && listener : m_logListeners )
	{
		listener->Log( text );
	}
}

void Game::AttachLogListener( ILogListener* listener )
{
	using namespace std;

	if( m_logFile.Empty() ) return;

	string line;
	ifstream in( m_logFile.ToString() );
	while ( getline( in, line ) )
	{
		listener->Log( line + "\n" );
	}

	m_logListeners.push_back( listener );
}

void Game::DetachLogListener( ILogListener* listener )
{
	m_logListeners.remove( listener );
}

void Game::ReportError( ErrorLevel level, std::string source, std::string error )
{
	switch( level )
	{
	case ErrorLevel::Critical:
		m_criticalErrors.push_back( "Critical Failure (" + source + "): " + error );
		LogLine( "Critical Failure (" + source + "): " + error  );
		throw unify::Exception( "Critical Failure (" + source + "): " + error );
	case ErrorLevel::Failure:
		LogLine( "Failure (" + source + "): " + error );
		if ( m_failuresAsCritial )
		{
			m_criticalErrors.push_back( "Critical Failure (" + source + "): " + error );
			throw unify::Exception( "Failure (" + source + "): " + error );
		}
		break;
	case ErrorLevel::Warning:
		LogLine( "Warning (" + source + "): " + error );
		break;
	}
	return;
}
						   
bool Game::HadCriticalError() const
{
	return m_criticalErrors.size() != 0;
}

int Game::ComponentCount() const
{
	return (int)m_components.size();
}

void Game::AddComponent( IGameComponent::ptr component )
{
	component->OnAttach( this );
	m_components.push_back( component );
}

void Game::RemoveComponent( IGameComponent::ptr component )
{
	m_components.remove( component );
	component->OnDetach( this );
}

IGameComponent::ptr Game::GetComponent( int index )
{
	if ( index > (int)m_components.size() ) return IGameComponent::ptr();

	int i = 0;
	for ( auto component : m_components )
	{
		if ( index == i ) return component;
		++i;
	}

	assert( 0 );
	return IGameComponent::ptr(); // Should never hit here.
}

IGameComponent::ptr Game::GetComponent( std::string name, int startIndex )
{
	int index = FindComponent( name, startIndex );
	if ( index == -1 ) return IGameComponent::ptr();
	return GetComponent( index );
}

int Game::FindComponent( std::string name, int startIndex ) const
{
	int i = 0;
	for ( auto component : m_components )
	{
		if ( i >= startIndex && unify::StringIs( component->GetName(), name ) ) return i;
		++i;
	}
	return -1;
}

void Game::SetUpdateEnabled( bool enabled )
{
	m_updateEnabled = enabled;
}

bool Game::GetUpdateEnabled() const
{
	return m_updateEnabled;
}
