// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <me/game/Game.h>
#include <me/game/component/GC_ActionFactory.h>
#include <me/exception/FailedToCreate.h>
#include <me/scene/SceneManager.h>
#include <me/scene/DefaultSceneFactory.h>
#include <me/factory/EffectFactories.h>
#include <me/factory/TextureFactory.h>
#include <me/factory/VertexShaderFactory.h>
#include <me/factory/PixelShaderFactories.h>
#include <me/factory/GeometryFactory.h>
#include <sg/ShapeFactory.h>
#include <fstream>
#include <chrono>
#include <ctime>
#include <functional>

// Temporary...
#include <me/scene/SceneManager.h>
#include <me/object/component/CameraComponent.h>
#include <me/canvas/CanvasComponent.h>
#include <me/canvas/FPS.h>

#include <me/input/ButtonPressedCondition.h>
#include <me/input/action/IA_Action.h>
#include <me/action/QuitGame.h>

using namespace me;
using namespace game;
using namespace scene;
using namespace render;

class GameLogger : public rm::ILogger
{
	game::IGame * m_game;
public:
	GameLogger( game::IGame * gameInstance ) : m_game( gameInstance )
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

void Game::Shutdown()
{
	// STUBBED - Provided by user.
}

Game::Game( unify::Path setup)
	: m_title{ "Mercury Engine" }
	, m_mainSceneFactory{ new me::scene::DefaultSceneFactory( "Main" ) }
	, m_failuresAsCritial(true)
	, m_setup(setup)
	, m_isQuitting(false)
	, m_totalStartupTime{}
	, m_inputOwnership{ unify::Owner::Create("Game") }
	, m_inputManager(this)
{
}

Game::Game(scene::ISceneFactory::ptr mainSceneFactory, unify::Path setup )
	: m_title{ "Mercury Engine" }
	, m_mainSceneFactory{ mainSceneFactory }
	, m_failuresAsCritial( true )
	, m_setup( setup )
	, m_isQuitting( false )
	, m_totalStartupTime{}
	, m_inputOwnership{ unify::Owner::Create( "Game" ) }
	, m_inputManager( this )
{
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
		
			SceneManager * sceneManager = GetComponentT< scene::SceneManager >();
			IScene* scene = sceneManager->GetCurrentScene();

			canvas::CanvasComponent::ptr canvas( new canvas::CanvasComponent( this ) );
			scene->AddComponent( canvas );

			Effect::ptr font2 = GetManager< Effect>()->Add( "font2", unify::Path( "font2.effect" ) );
			canvas->GetLayer()->AddElement( canvas::IElement::ptr( new canvas::FPS( this, font2 ) ) );
		}
	}
	
	return 0;
}

void Game::Initialize( OSParameters osParameters )
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
			m_setup = unify::Path( arg );
		}
	}

	// Check for OS in setup...
	if( !m_setup.Empty() )
	{
		if( !m_setup.Exists() )
		{
			ReportError( ErrorLevel::Critical, "Game", "Setup file, \"" + m_setup.ToString() + "\" not found!" );
		}

		// First loader pass
		std::function< void( unify::Path ) > xmlLoader = [&]( unify::Path source )
		{
			unify::Path pathDiscovery( source );

			// Attempt to discover the source, if we have an OS.
			if( GetOS() )
			{
				pathDiscovery = GetOS()->GetAssetPaths().FindAsset( pathDiscovery );
			}

			qxml::Document doc( pathDiscovery );

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
					else if( node.IsTagName( "assets" ) )
					{
						GetOS()->GetAssetPaths().AddSource( unify::Path( node.GetText() ) );
					}

					// "inputs" handle further on
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
		throw me::exception::FailedToCreate( "Failure attempting to setup with OS configuration!" );
	}

	// Early setup...
	if ( ! m_setup.Empty() )
	{
		if( ! m_setup.Exists() )
		{
			ReportError( ErrorLevel::Critical, "Game", "Setup file, \"" + m_setup.ToString() + "\" not found!" );
		}

		// Second loader pass
		std::function< void( unify::Path ) > xmlLoader = [&]( unify::Path source )
		{
			unify::Path pathDiscovery( GetOS()->GetAssetPaths().FindAsset( source ) );
			qxml::Document doc( pathDiscovery );

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
						m_logFile = unify::Path( node.GetText() );

						// Delete current log.
						GetOS()->DeletePath( m_logFile );
					}
					else if ( node.IsTagName( "failuresAsCritical" ) )
					{
						m_failuresAsCritial = unify::Cast< bool >( node.GetText() );
					}

					// "inputs" handle further on
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

	// Add internal components...
	AddComponent( IGameComponent::ptr( new scene::SceneManager() ) );
	AddComponent( IGameComponent::ptr( new game::component::ActionFactory() ) );
	

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
		// Third and final loader pass.
		std::function< void( unify::Path ) > xmlLoader = [&]( unify::Path source )
		{
			unify::Path pathDiscovery( GetOS()->GetAssetPaths().FindAsset( source ) );
			qxml::Document doc( pathDiscovery );

			LogLine( "Loading setup \"" + source.ToString() + "\"..." );

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
					else if (node.IsTagName("inputs"))
					{
						size_t failures = GetInputManager()->AddInputActions(m_inputOwnership, &node, true );
						LogLine("Add input actions (failures = " + unify::Cast< std::string >(failures) + ")");
					}
				}
			}
		};
		xmlLoader( m_setup );
	}

	LogLine( "GameComponent summary...", 0 );
	for( int i = 0; i < GetComponentCount(); i++ )
	{
		LogLine( GetComponent( i )->GetTypeName() );
	}

	LogLine( "OS Startup Begin", 0 );
	m_os->Startup();
	LogLine( "OS Startup Done", 0 );
	
	LogLine( "Game Component's OnBeforeStartup Begin", 0 );
	for( auto && component : m_components )
	{
		LogLine( "OnBeforeStart \"" + component->GetTypeName() + "\" Begin" );
		component->OnBeforeStartup();
		LogLine( "OnBeforeStart \"" + component->GetTypeName() + "\" Done");
	}
	LogLine( "Game Component's OnBeforeStartup Done", 0 );

	
	LogLine( "Game's Startup Begin", 0 );
	try
	{
		Startup();
	}
	catch( unify::Exception ex )
	{
		LogLine( "Startup FAILED" );
		ReportError( ErrorLevel::Critical, "Game", ex.what() );
	}
	LogLine( "Game's Startup Done", 0 );

	LogLine( "OnAfterStartup Begin", 0 );
	for( auto && component : m_components )
	{
		LogLine("OnAfterStartup \"" + component->GetTypeName() + "\" Begin");
		component->OnAfterStartup();
		LogLine("OnAfterStartup \"" + component->GetTypeName() + "\" Done");
	}
	LogLine( "OnAfterStartup Done", 0 );

	// Basic motivations...
	if ( GetInputManager() )
	{
		input::IInputDevice::ptr keyboard(GetInputManager()->FindSource("keyboard"));
		if (keyboard)
		{
			auto condition = input::IInputCondition::ptr( new input::ButtonPressedCondition( 0, "Escape"));
			input::IInputAction::ptr action( new input::action::Action( action::IAction::ptr{ new action::QuitGame(this) } ) );
			keyboard->AddEvent(m_inputOwnership, condition, action);
		}
	}	
	high_resolution_clock::time_point currentTime = high_resolution_clock::now();
	duration< float > elapsed_d = duration_cast<duration< float >>(currentTime - lastTime);
	auto micro = duration_cast< microseconds >(currentTime - lastTime).count();
	m_totalStartupTime = micro * 0.000001f;

	LogLine( "total startup time: " + unify::Cast< std::string >( m_totalStartupTime ) + "s", 0 );

	LogLine( "Creating Main Scene", 0 );
	auto sceneManager = GetComponentT< scene::SceneManager >();
	if ( m_mainSceneFactory )
	{
		sceneManager->AddScene(m_mainSceneFactory->GetName(), m_mainSceneFactory);
		LogLine( "Main Scene created", 0 );
	}
	else
	{
		LogLine( "No main screen created", 0 );
	}
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

	// Remove expired locks...
	for ( auto itr = m_locks.begin(); itr != m_locks.end();  )
	{
		if ( itr->expired() )
		{
			itr = m_locks.erase( itr );
		}
		else
		{
			itr++;
		}
	}
	
	// Return if we are locked.
	if ( ! m_locks.empty() || ! m_exclusiveLock.expired() )
	{
		return;
	}				  

	m_renderInfo.SetDelta( elapsed );
	auto renderer = GetOS()->GetRenderer( 0 );

	UpdateParams params{ renderer, m_renderInfo };



	for( auto && component : m_components )
	{
		if( !component->IsEnabled() )
		{
			continue;
		}

		component->OnEarlyUpdate( params );
	}

	m_inputManager.Update( params );

	for( auto && component : m_components )
	{
		if( !component->IsEnabled() )
		{
			continue;
		}

		component->OnUpdate( params );
	}

	for( auto && component : m_components )
	{
		if ( ! component->IsEnabled() )
		{
			continue;
		}

		component->OnLateUpdate( params );
	}
}

void Game::Draw()
{
	for( int index = 0; index < GetOS()->RendererCount(); ++index )
	{
		IRenderer * renderer = m_os->GetRenderer( index );
		renderer->BeforeRender();

		render::Params params{ renderer, m_renderInfo };
										  	
		for( auto && component : m_components )
		{
			if ( !component->IsEnabled() )
			{
				continue;
			}

			component->OnRender( params );
		}

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

unify::Owner::ptr Game::GetOwnership()
{
	return m_inputOwnership;
}

input::InputManager * Game::GetInputManager()
{
	return &m_inputManager;
}

const input::InputManager * Game::GetInputManager() const
{
	return &m_inputManager;
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
	if( GetOS() )
	{
		GetOS()->DebugOutput( text );
	}

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

int Game::GetComponentCount() const
{
	return (int)m_components.size();
}

void Game::AddComponent( IGameComponent::ptr component )
{
	try
	{
		component->OnAttach( this );
	}
	catch ( ... )
	{
		throw;
	}
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

IGameComponent::ptr Game::GetComponent( std::string name )
{
	int index = FindComponent( name );
	if ( index == -1 ) return IGameComponent::ptr();
	return GetComponent( index );
}

int Game::FindComponent( std::string typeName ) const
{
	int i = 0;
	for ( auto component : m_components )
	{
		if ( unify::StringIs( component->GetTypeName(), typeName ) ) return i;
		++i;
	}
	return -1;
}

UpdateLock::ptr Game::LockUpdate( bool exclusive )
{
	if ( !m_exclusiveLock.expired() )
	{
		return false;
	}

	if ( exclusive )
	{
		if ( !m_locks.empty() )
		{
			return false;
		}

		UpdateLock::ptr lock( new UpdateLock() );
		m_exclusiveLock = lock;
		return lock;
	}
	else
	{
		UpdateLock::ptr lock( new UpdateLock() );
		m_locks.push_back( lock );
		return lock;
	}
}

bool Game::IsUpdateLocked( bool exclusive ) const
{
	if ( ! m_exclusiveLock.expired() )
	{
		return true;
	} 

	return exclusive ? false : ! m_locks.empty();
}

action::IAction::ptr Game::CreateAction(const qxml::Element * element)
{
	for (auto component : m_components)
	{
		auto action = component->CreateAction(element);
		if (action)
		{
			return action;
		}
	}

	return action::IAction::ptr();
}

object::action::IObjectAction::ptr Game::CreateObjectAction( const qxml::Element * element )
{
	for( auto component : m_components )
	{
		auto action = component->CreateObjectAction( element );
		if( action )
		{
			return action;
		}
	}

	return object::action::IObjectAction::ptr();
}

input::IInputAction::ptr Game::CreateInputAction( const qxml::Element * element )
{
	return input::IInputAction::ptr();
}

Game::~Game()
{
	LogLine( "Shutting down:", 0 );

	LogLine( "OnDetach", 0 );
	for( auto && component : m_components )
	{
		try
		{
			LogLine( "Detaching " + component->GetTypeName() );
			component->OnDetach( this );
		}
		catch( ... )
		{
		}

	}
	LogLine( "OnDetachDone", 0 );


	// Remove all log listeners.
	m_logListeners.clear();

	// Call user shutdown.
	Shutdown();

	m_resourceHub.Clear();

	m_inputManager.Clear();

	m_components.clear();

	if( m_os )
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

void Game::AddCommandListener( unify::Owner::weak_ptr owner, std::string command, ICommandListener::ptr listener )
{
	size_t id = Command( command );
	m_commandListeners[id].push_back( CommandListenerSet{ owner, listener } );
}

size_t Game::Command( std::string command )
{
	size_t id = 0;
	auto itr = m_commandMap.find( command );
	if( itr == m_commandMap.end() )
	{
		id = m_commandListeners.size();
		m_commandList.push_back( command );
		m_commandMap[command] = id;
		m_commandListeners.push_back( std::list< CommandListenerSet >() );
	}
	else
	{
		id = itr->second;
	}

	return id;
}

std::string Game::Command( size_t id )
{
	if( id >= m_commandList.size() )
	{
		return std::string();
	}
	else
	{
		return m_commandList[id];
	}
}

std::string Game::SendCommand( std::string command, std::string extra )
{
	return SendCommand( Command( command ), extra );
}

std::string Game::SendCommand( size_t id, std::string extra )
{
	if( id == std::numeric_limits< size_t >::max() )
	{
		return std::string();
	}

	// Listeners...

	auto & commandListenerSetList = m_commandListeners[ id ];
	std::string lastResult;
	for( auto & itr = commandListenerSetList.begin(); itr != commandListenerSetList.end(); )
	{
		// Check if the owner is expired, if so, remove it.
		if( itr->owner.expired() )
		{
			itr = commandListenerSetList.erase( itr );
		}
		else
		{
			std::string result;
			result = itr->listener->SendCommand( id, extra );
			if( result.empty() )
			{
				lastResult = result;
			}
			itr++;
		}
	}

	// Game components...
	for( auto component : m_components )
	{
		component->SendCommand( id, extra );
	}

	return lastResult;
}

const Debug * Game::Debug() const
{
	return &m_debug;
}

void Game::AddExtension( unify::Path path, const qxml::Element * element )
{
	Extension * extension{ new Extension( path ) };

	try
	{
		if( !extension->Load( this, element ) )
		{
			delete extension;
			ReportError( ErrorLevel::Critical, "Game", "Failed to load extension " + path.ToString() + "!" );
		}
	}
	catch( std::exception ex )
	{
		delete extension;
		ReportError( ErrorLevel::Critical, "Game", "Failed to load extension " + path.ToString() + "! Error:\n" + ex.what() );
	}

	m_extensions.push_back( std::shared_ptr< Extension >{ extension } );
}
